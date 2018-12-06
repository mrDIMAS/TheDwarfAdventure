/**********************************************************************
 * The Dwarf Adventure - 2D adventure RPG with destructible world     *
 *                                                                    *
 * Copyright(C) 2016-2019 Dmitry Stepanov a.k.a mrDIMAS a.k.a v1al    *
 *                                                                    *
 * This program is free software: you can redistribute it and/or      *
 * modify it under the terms of the GNU General Public License as     *
 * published by the Free Software Foundation, either version 3 of     *
 * the License, or (at your option) any later version.                *
 *                                                                    *
 * This program is distributed in the hope that it will be useful,    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * along with this program. If not, see https://www.gnu.org/licenses/ *
 *                                                                    *
 **********************************************************************/

#include "Precompiled.h"
#include "PassageGenerator.h"
#include "Bundle.h"
#include "MotherCrystal.h"
#include "Game.h"
#include "Renderer.h"
#include "ResourceDatabase.h"
#include "PixelAttribute.h"
#include "LocationGenerator.h"
#include "Location.h"
#include "Ladder.h"
#include "Terrain.h"
#include "Scene.h"
#include "LocationMap.h"
#include "RoomGenerator.h"
#include "Door.h"
#include "Sprite.h"
#include "WaterBlob.h"
#include "MonsterGenerator.h"
#include "Passage.h"

enum class MountSide
{
	Top,
	Bottom
};

bool PassageGenerator::IsInsersectAnyPassageLink(const sf::FloatRect & bounds)
{
	for (const shared_ptr<Passage> & passage : mParentGenerator->GetLocation()->GetPassageList())
	{
		for (const weak_ptr<Passage> & weakLink : passage->GetLinkList())
		{
			if (weakLink.lock()->Intersects(bounds))
			{
				return true;
			}
		}
	}

	return false;
}

bool PassageGenerator::IsIntersectAnyForbiddenZone(const sf::FloatRect &bounds)
{
	for (const sf::FloatRect &forbiddenZone : mForbiddenZones)
	{
		if (bounds.intersects(forbiddenZone))
		{
			return true;
		}
	}
	return false;
}

bool PassageGenerator::IsIntersectAnyPassage(const sf::FloatRect &bounds, float xBloat, float yBloat)
{
	const shared_ptr<Location> location = mParentGenerator->GetLocation();

	for (const shared_ptr<Passage> & passage : location->GetPassageList())
	{
		const sf::FloatRect passageBounds = passage->GetBounds();

		const sf::FloatRect bloatedPassage = {
			bounds.left - xBloat,
			bounds.top - yBloat,
			bounds.width + 2 * xBloat,
			bounds.height + 2 * yBloat
		};

		if (bloatedPassage.intersects(passageBounds))
		{
			return true;
		}
	}
	return false;
}

bool PassageGenerator::IsFullyInsideArea(const sf::FloatRect &bounds, const sf::FloatRect &area)
{
	return bounds.left >= area.left && bounds.top >= area.top &&
		bounds.left + bounds.width <= area.left + area.width &&
		bounds.top + bounds.height <= area.top + area.height;
}

void PassageGenerator::GenerateDecorations()
{
	shared_ptr<Location> location = mParentGenerator->GetLocation();

	// list of occupied places on the location in which any other decoration can't be created
	vector<sf::FloatRect> occupiedPlaces;
	const int maxTries = 20;

	// will randomize width by this coefficient (height will be increased\reduced by aspect ratio)
	const float maxWidthRandCoeff = 1.35f;
	Renderer* renderer = game->GetRenderer();

	struct DecorationDesc
	{
		Vec2f size;
		shared_ptr<Texture> texture;
		MountSide mountSide;
	};

	// declare possible decorations
	Bundle<DecorationDesc> decorations = {
		{ 0.33f, { { 28, 39 }, renderer->RequestTexture("data/textures/decorations/barrel_1.png"), MountSide::Bottom } },
		{ 0.33f, { { 52, 30 }, renderer->RequestTexture("data/textures/decorations/box_1.png"), MountSide::Bottom } },
		{ 0.33f, { { 40, 10 }, renderer->RequestTexture("data/textures/decorations/ceiling_moss_1.png"), MountSide::Top } }
	};

	// calculate average decoration width
	float averageDecWidth = 0.0f;
	for (const BundleItem<DecorationDesc> &bundleItem : decorations.GetItems())
	{
		averageDecWidth += bundleItem.mValue.size.x;
	}
	averageDecWidth /= decorations.GetItems().size();

	for (const shared_ptr<Passage> & passage : location->GetPassageList())
	{
		const sf::FloatRect passageBounds = passage->GetBounds();

		int columnCount = static_cast<int>(passageBounds.width / mColumnPlacementDistance);

		// place columns
		for (int i = 0; i < columnCount; ++i)
		{
			const sf::FloatRect columnBounds = {
				passageBounds.left + i * mColumnPlacementDistance,
				passageBounds.top,
				26,
				passageBounds.height
			};

			sf::FloatRect tallBounds = columnBounds;
			tallBounds.top -= 10;
			tallBounds.height += 20;

			// column can be added only if it does not intersect any passage link
			bool addColumn = true;

			if (IsInsersectAnyPassageLink(tallBounds))
			{
				addColumn = false;
			}

			// column also must not intersect any passage spacers
			for (const sf::FloatRect &spacer : passage->GetSpacerBounds())
			{
				if (spacer.intersects(columnBounds))
				{
					addColumn = false;
					break;
				}
			}

			if (addColumn)
			{
				mParentGenerator->AddSpriteDecoration(columnBounds, renderer->RequestTexture("data/textures/objects/column.png"));
				passage->AddDecorationBounds(columnBounds);
				occupiedPlaces.push_back(columnBounds);
			}
		}
		int maxDec = static_cast<int>((passageBounds.width / averageDecWidth) * 0.5f);
		int count = Math::Rnd(maxDec / 2, maxDec);
		for (int k = 0; k < count; ++k)
		{
			const DecorationDesc decoration = decorations.Drop();
			float invAspect = decoration.size.y / decoration.size.x;
			for (int i = 0; i < maxTries; ++i)
			{
				// define bounds 
				sf::FloatRect bounds;
				float wk = Math::Rnd(1.0f, maxWidthRandCoeff);
				bounds.width = decoration.size.x * wk;
				bounds.height = bounds.width * invAspect;
				bounds.left = passageBounds.left + Math::Rnd(bounds.width, passageBounds.width - bounds.width);
				switch (decoration.mountSide)
				{
				case MountSide::Bottom:
					bounds.top = passageBounds.top + passageBounds.height - bounds.height;
					break;
				case MountSide::Top:
					bounds.top = passageBounds.top;
					break;
				}
				sf::FloatRect tallBounds = bounds;
				switch (decoration.mountSide)
				{
				case MountSide::Bottom:
					// make bounds a bit taller
					tallBounds.height += 20;
					break;
				case MountSide::Top:
					// take bounds a bit higher
					tallBounds.top -= 20;
					break;
				}
				// tall bounds should not intersect any passage link
				bool add = true;

				if (IsInsersectAnyPassageLink(tallBounds))
				{
					add = false;
				}

				// bounds should not intersect any already occupied place
				if (add)
				{
					for (const sf::FloatRect &ocpPlace : occupiedPlaces)
					{
						if (ocpPlace.intersects(bounds))
						{
							add = false;
							break;
						}
					}
				}
				// bounds also must not itersect any passage spacers
				for (const sf::FloatRect &spacer : passage->GetSpacerBounds())
				{
					if (spacer.intersects(bounds))
					{
						add = false;
						break;
					}
				}
				// should not intersect any ladders
				for (const shared_ptr<Ladder> &ladder : location->GetLadderList())
				{
					if (ladder->GetBounds().intersects(bounds))
					{
						add = false;
						break;
					}
				}
				if (add)
				{
					mParentGenerator->AddSpriteDecoration(bounds, decoration.texture);
					passage->AddDecorationBounds(bounds);
					occupiedPlaces.emplace_back(bounds);
					// decoration is added, leave inner 'try' loop
					break;
				}
			}
		}
	}
}

void PassageGenerator::GenerateMotherCrystals()
{
	// Mother crystals can be placed only in horizontal passages
	vector<shared_ptr<Passage>> horizontalPassages;
	for (const shared_ptr<Passage> & passage : mParentGenerator->GetLocation()->GetPassageList())
	{
		if (passage->GetType() == PassageType::Horizontal)
		{
			horizontalPassages.push_back(passage);
		}
	}

	int motherCrystalCount = Math::Rnd(1, 3);
	for (int i = 0; i < motherCrystalCount; ++i)
	{	
		const shared_ptr<Passage> passage = Math::RandomElement(horizontalPassages);
		const sf::FloatRect passageBounds = passage->GetBounds();

		int iterationCount = 40;
		while (iterationCount-- > 0)
		{
			// Propose mother crystal position
			Vec2f position;
			position.x = Math::Rnd(passageBounds.left, passageBounds.left + passageBounds.width * 0.8f);
			position.y = passageBounds.top + passageBounds.height * 0.6f;

			bool doNextTry = false;

			const sf::FloatRect bounds = { position, { MotherCrystal::Width, MotherCrystal::Height } };

			// Check intersection with decorations and other objects
			for (const sf::FloatRect &decBounds : passage->GetDecorationBounds())
			{
				if (bounds.intersects(decBounds))
				{
					doNextTry = true;
					break;
				}
			}

			for (const sf::FloatRect &spacer : passage->GetSpacerBounds())
			{
				if (spacer.intersects(bounds))
				{
					doNextTry = true;
					break;
				}
			}

			if (doNextTry)
			{
				continue;
			}

			mParentGenerator->CreateMotherCrystal(position);

			break;
		}
	}
}

void PassageGenerator::GeneratePassageSpacers()
{
	const shared_ptr<Location> location = mParentGenerator->GetLocation();
	const shared_ptr<Terrain> terrain = location->GetScene()->GetTerrain();

	// add passage spacers
	Bundle<bool> placeDoor = { { 0.8f, true }, { 0.2f, false } };
	int maxTries = 80;
	for (const shared_ptr<Passage> & passage : location->GetPassageList())
	{
		const sf::FloatRect passageBounds = passage->GetBounds();

		float spacerDistanceMax = 200.0f;
		float doorHeight = 50;
		float lowerBoundsHeight = 5;
		int spacerCount = Math::Rnd(0, static_cast<int>(ceil(passageBounds.width / spacerDistanceMax)));
		float dx = passageBounds.width / spacerCount;
		float x = passageBounds.left + dx;
		for (int i = 0; i < spacerCount - 1; ++i)
		{
			// give algorithm some tries to succeed
			for (int j = 0; j < maxTries; ++j)
			{
				sf::FloatRect upBounds;
				upBounds.width = Math::Rnd(12.0f, 18.0f);
				upBounds.height = passageBounds.height - doorHeight - lowerBoundsHeight;
				upBounds.left = x;
				upBounds.top = passageBounds.top;
				sf::FloatRect tallBounds = upBounds;
				tallBounds.top -= 20;
				tallBounds.height += 40;

				bool doNextTry = false;

				// spacer can be added if it's up part does not itersect any link
				// between passages

				if (IsInsersectAnyPassageLink(tallBounds))
				{
					doNextTry = true;
				}

				if (doNextTry)
				{
					continue;
				}

				// define lower bounds
				sf::FloatRect lowBounds;
				lowBounds.left = upBounds.left;
				lowBounds.top = upBounds.top + upBounds.height + doorHeight;
				lowBounds.height = lowerBoundsHeight;
				lowBounds.width = upBounds.width;
				sf::FloatRect tallLow = lowBounds;
				tallLow.top -= 20;
				tallLow.height += 40;
				// spacer can be added if it's bottom part does not itersect any link
				// between passages

				if (IsInsersectAnyPassageLink(tallLow))
				{
					doNextTry = true;
				}

				if (doNextTry)
				{
					continue;
				}

				passage->AddSpacerBounds(upBounds);
				passage->AddSpacerBounds(lowBounds);

				{
					const TerrainBrush* tile = game->GetResourceDatabase()->GetRandom(TerrainBrushClass::Metal);

					MaskOperation args;
					args.mPosition = { upBounds.left, upBounds.top - 4 };     // hack
					args.mRectSize = { upBounds.width, upBounds.height + 4 }; // hack
					args.mBounds = Bounds::Rect;
					args.SetBrushImage(tile->mFrontTexture);
					args.mBrushMode = BrushMode::Wrap;
					args.mFirstPassBitwiseOperation = BitwiseOp::Or;
					args.mFirstPassMask = PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE | PA_METAL;
					terrain->Mask(args);

					args.mPosition = { lowBounds.left, lowBounds.top };
					args.mRectSize = { lowBounds.width, lowBounds.height + 4 }; // hack
					terrain->Mask(args);

					if (placeDoor.Drop())
					{
						const Vec2f doorPosition = {
							upBounds.left + upBounds.width / 2,
							upBounds.top + upBounds.height
						};

						const shared_ptr<Door> door = make_shared<Door>(location, DoorType::Internal, DoorModelType::Side, doorPosition);
						door->SetHeight(doorHeight);
						location->AddDoor(door);
					}
				}

				// offset next spacer
				x += dx;
				// leave 'try' loop
				break;
			}
		}
	}
}

void PassageGenerator::GenerateLights()
{
	for (const shared_ptr<Passage> & passage : mParentGenerator->GetLocation()->GetPassageList())
	{
		const sf::FloatRect passageBounds = passage->GetBounds();

		int count = static_cast<int>(passageBounds.width / mPassageLightDistance) - 1;
		for (int i = 0; i < count; ++i)
		{
			mParentGenerator->CreateTorchLight({ passageBounds.left + (i + 1) * mPassageLightDistance, passageBounds.top + passageBounds.height / 2 }, 100);
		}
	}
}

void PassageGenerator::GenerateRooms()
{
	const shared_ptr<Location> location = mParentGenerator->GetLocation();

	for (const shared_ptr<Passage> & passage : location->GetPassageList())
	{
		// Ignore vertical passages
		if (passage->GetType() != PassageType::Horizontal)
		{
			continue;
		}

		const sf::FloatRect passageBounds = passage->GetBounds();

		for (int side = 0; side <= 1; ++side)
		{
			float roomWidth = Math::Rnd(mRoomWidthMin, mRoomWidthMax);
			float roomHeight = Math::Rnd(mRoomHeightMin, mRoomHeightMax);

			sf::FloatRect newRoom;
			newRoom.top = passageBounds.top - roomHeight + passageBounds.height;
			newRoom.width = roomWidth;
			newRoom.height = roomHeight;

			sf::FloatRect corridor;
			corridor.top = passageBounds.top + passageBounds.height / 2;
			corridor.width = mRoomCorridorLength;
			corridor.height = passageBounds.height / 2;

			if (side == 0)
			{
				// left
				newRoom.left = passageBounds.left - roomWidth - mRoomCorridorLength;
				corridor.left = passageBounds.left - mRoomCorridorLength;
			}
			else
			{
				// right
				newRoom.left = passageBounds.left + passageBounds.width + mRoomCorridorLength;
				corridor.left = passageBounds.left + passageBounds.width;
			}

			sf::FloatRect bloatedRoom;
			bloatedRoom.left = newRoom.left - mRoomSpacing;
			bloatedRoom.top = newRoom.top - mRoomSpacing;
			bloatedRoom.width = newRoom.width + 2 * mRoomSpacing;
			bloatedRoom.height = newRoom.height + 2 * mRoomSpacing;

			const bool outOfBoundsLeft = bloatedRoom.left < mArea.left;
			const bool outOfBoundsRight = bloatedRoom.left + bloatedRoom.width > mArea.left + mArea.width;
			const bool outOfBoundsTop = bloatedRoom.top < mArea.top;
			const bool outOfBoundsBottom = bloatedRoom.top + bloatedRoom.height > mArea.top + mArea.height;
			bool doNotAddRoom = outOfBoundsLeft || outOfBoundsRight || outOfBoundsTop || outOfBoundsBottom;

			// check room/corridor intersection with other passages
			if (!doNotAddRoom)
			{
				for (const shared_ptr<Passage> & otherPassage : mParentGenerator->GetLocation()->GetPassageList())
				{
					const sf::FloatRect otherPassageBounds = otherPassage->GetBounds();

					if (bloatedRoom.intersects(otherPassageBounds) || corridor.intersects(otherPassageBounds))
					{
						doNotAddRoom = true;
						break;
					}
				}
			}
			if (!doNotAddRoom)
			{
				// check room/corridor intersection with links
				if (IsInsersectAnyPassageLink(bloatedRoom) || IsInsersectAnyPassageLink(corridor))
				{
					doNotAddRoom = true;
				}
			}
			if (!doNotAddRoom)
			{
				// check room/corridor intersection with other rooms
				for (const shared_ptr<Room> &otherRoom : location->GetRoomList())
				{
					if (otherRoom->Intersects(bloatedRoom) || otherRoom->Intersects(corridor))
					{
						doNotAddRoom = true;
						break;
					}
				}
			}
			if (!doNotAddRoom)
			{
				for (const sf::FloatRect &forbiddenZone : mForbiddenZones)
				{
					if (bloatedRoom.intersects(forbiddenZone))
					{
						doNotAddRoom = true;
						break;
					}
				}
			}
			if (!doNotAddRoom)
			{
				RoomGenerator roomGen(mParentGenerator);
				roomGen.mBounds = newRoom;
				roomGen.mEntranceBounds = corridor;
				roomGen.AllowInhabitants(mAllowInhabitants);
				roomGen.Generate();
			}
		}
	}
}

void PassageGenerator::GeneratePassages()
{
	const shared_ptr<Location> location = mParentGenerator->GetLocation();
	const shared_ptr<Scene> scene = location->GetScene();
	const shared_ptr<Terrain> terrain = scene->GetTerrain();

	float div;
	switch (mDensity)
	{
	case PassageDensity::Low:
		div = 200;
		break;
	case PassageDensity::Medium:
		div = 120;
		break;
	case PassageDensity::High:
		div = 40;
		break;
	case PassageDensity::Max:
		div = 10;
		break;
	default:
		div = 1;
	}
	int mPassageCount = static_cast<int>(sqrt(mArea.width * mArea.height) / div);

	// place passages
	for (int i = 0; i < mPassageCount; ++i)
	{
		for (int k = 0; k < MaxTries; ++k)
		{
			// propose passage
			sf::FloatRect passageBounds;
			if (HasAnotherPredefinedPassage())
			{
				passageBounds = GetAnotherPredefinedPassage();
			}
			else
			{
				passageBounds.height = Math::Rnd(mPassageHeightMin, mPassageHeightMax);
				passageBounds.left = Math::Rnd(mArea.left, mArea.left + mArea.width);
				passageBounds.top = Math::Rnd(mArea.top, mArea.top + mArea.height - passageBounds.height);
				passageBounds.width = Math::Rnd(mPassageMinWidth, mPassageMaxWidth);
			}

			if (!IsFullyInsideArea(passageBounds, mArea))
			{
				continue;
			}

			if (IsIntersectAnyForbiddenZone(passageBounds))
			{
				continue;
			}

			if (IsIntersectAnyPassage(passageBounds, mPassageSpacing, mPassageSpacing))
			{
				continue;
			}

			mParentGenerator->DrawPassage(passageBounds, game->GetResourceDatabase()->GetRandom(TerrainBrushClass::Stone));

			const shared_ptr<Passage> passage = make_shared<Passage>(passageBounds, PassageType::Horizontal);

			location->AddPassage(passage);

			{
				const shared_ptr<Sprite> pipe = scene->CreateSprite();
				pipe->SetTexture(game->GetRenderer()->RequestTexture("data/textures/objects/pipe.png"));
				pipe->SetSize({ 20, 20 });

				Vec2f position, size;
				size.x = 20;
				size.y = 20;
				position.x = Math::Rnd(passageBounds.left + size.x, passageBounds.left + passageBounds.width - size.x);
				position.y = passageBounds.top + size.x + 5;

				pipe->SetLocalPosition(position);

				const shared_ptr<WaterBlob> blob = make_shared<WaterBlob>(scene, position, WaterBlobInitialState::Rect, 0.0f, size);
				blob->FillCircleWithSourcePoints(position + size * 0.5f, 10, 4, 4);
				scene->AddWaterBlob(blob);
			}

			if (mGenerateMonsters && Math::ProbabilityEventAppeared(0.65f))
			{
				const shared_ptr<Creature> monster = mParentGenerator->GetMonsterGenerator()->Generate(location);

				Math::Line patrolLine;
				patrolLine.mBegin = {
					passageBounds.left + 20,
					passageBounds.top + passageBounds.height / 2
				};
				patrolLine.mEnd = {
					passageBounds.left + passageBounds.width - 20,
					passageBounds.top + passageBounds.height / 2
				};
				monster->SetPatrolLine(patrolLine);

				Vec2f position;

				position.x = Math::Rnd(passageBounds.left + 10, passageBounds.left + passageBounds.width * 0.75f);
				position.y = passageBounds.top + passageBounds.height / 2;

				monster->SetPosition(position);

				monster->SetLevel(location->GetDefinition()->GetBaseLevel() + Math::Rnd(0, 2));
				location->AddActor(monster);
			}

			break;
		}
	}
}

void PassageGenerator::GeneratePassageLinks()
{
	const shared_ptr<Location> location = mParentGenerator->GetLocation();

	// Copy passages 
	vector<shared_ptr<Passage>> sortedPassages = location->GetPassageList();

	// And sort them from top to bottom
	sort(sortedPassages.begin(), sortedPassages.end(), [] (const shared_ptr<Passage> &lhs, const shared_ptr<Passage> &rhs)
	{
		return lhs->GetBounds().top < rhs->GetBounds().top;
	});

	// Link passages
	for (const shared_ptr<Passage> & passage : sortedPassages)
	{
		const sf::FloatRect passageBounds = passage->GetBounds();

		const float linkWidth = Math::Rnd(mLinkMinWidth, mLinkMaxWidth);

		for (int iteration = 0; iteration < MaxTries; ++iteration)
		{
			sf::FloatRect newLinkBounds;
			newLinkBounds.left = Math::Rnd(passageBounds.left, passageBounds.left + passageBounds.width - linkWidth);
			newLinkBounds.top = passageBounds.top + passageBounds.height;
			newLinkBounds.width = linkWidth;
			newLinkBounds.height = mLinkMaxHeight;

			sf::FloatRect bloatedLink;
			bloatedLink.left = newLinkBounds.left - mPassageSpacing;
			bloatedLink.top = newLinkBounds.top - mPassageSpacing;
			bloatedLink.width = newLinkBounds.width + 2 * mPassageSpacing;
			bloatedLink.height = newLinkBounds.height + 2 * mPassageSpacing;

			sf::FloatRect tallLink = newLinkBounds;
			tallLink.top = newLinkBounds.top - 2 * passageBounds.height;
			tallLink.height = newLinkBounds.height + 4 * passageBounds.height;

			for (const shared_ptr<Passage> & otherPassage : sortedPassages)
			{
				const sf::FloatRect otherPassageBounds = otherPassage->GetBounds();

				if (otherPassage == passage || !newLinkBounds.intersects(otherPassageBounds))
				{
					continue;
				}

				// link should be fully in other passage bounds
				if (newLinkBounds.left < otherPassageBounds.left ||
					newLinkBounds.left + newLinkBounds.width > otherPassageBounds.left + otherPassageBounds.width)
				{
					continue;
				}

				// check new link with already linked passages
				for (const weak_ptr<Passage> & weakLinkPassage : passage->GetLinkList())
				{
					const shared_ptr<Passage> linkPassage = weakLinkPassage.lock();

					// check if already linked
					if (linkPassage == otherPassage)
					{
						goto linkNextTry;
					}

					// check if intersect already linked passage
					if (linkPassage->Intersects(bloatedLink))
					{
						goto linkNextTry;
					}
				}

				// Check intersection with other links
				if (IsInsersectAnyPassageLink(bloatedLink) || IsInsersectAnyPassageLink(tallLink))
				{
					goto linkNextTry;
				}

				if (IsIntersectAnyForbiddenZone(bloatedLink))
				{
					continue;
				}

				newLinkBounds.height = otherPassageBounds.top - passageBounds.top - passageBounds.height;

				mParentGenerator->DrawPassage(newLinkBounds, game->GetResourceDatabase()->GetRandom(TerrainBrushClass::Stone));

				// Create new link
				{
					shared_ptr<Passage> newLink = make_shared<Passage>(newLinkBounds, PassageType::Vertical);

					location->AddPassage(newLink);

					// Let both passages know that they're linked
					passage->AddLink(newLink);
					otherPassage->AddLink(newLink);
				}

				// Add ladder
				{
					shared_ptr<Ladder> ladder = make_shared<Ladder>(location->GetScene());
					ladder->SetPosition({ newLinkBounds.left + newLinkBounds.width / 2 - mLadderWidth / 2, newLinkBounds.top });
					ladder->SetSize({ mLadderWidth, newLinkBounds.height + otherPassageBounds.height });
					location->AddLadder(ladder);
				}
				break;

			linkNextTry:;
			}
		}
	}
}

void PassageGenerator::GenerateDetails()
{
	const shared_ptr<Location> location = mParentGenerator->GetLocation();
	const shared_ptr<Terrain> terrain = location->GetScene()->GetTerrain();

	const float wallSide = 16;

	for (const shared_ptr<Passage> & passage : location->GetPassageList())
	{
		// Ignore vertical passages
		if (passage->GetType() != PassageType::Horizontal)
		{
			continue;
		}

		const sf::FloatRect passageBounds = passage->GetBounds();

		// Add indestructible ground 
		{
			sf::FloatRect bounds;
			bounds.left = passageBounds.left;
			bounds.top = passageBounds.top + passageBounds.height;
			bounds.width = passageBounds.width;
			bounds.height = wallSide;

			MaskOperation args;
			args.SetBounds(bounds);
			args.mBrushMode = BrushMode::WrapBounds;
			args.mDoBitmaskOnlyOnVisibleDestPixels = true;
			args.SetBrushImage(game->GetRenderer()->RequestTexture("data/textures/objects/metal_floor.png"));
			args.mFirstPassBitwiseOperation = BitwiseOp::Or;
			args.mFirstPassMask = PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE | PA_METAL;
			args.mSecondPassBitwiseOperation = BitwiseOp::And;
			args.mSecondPassMask = ~(PA_EDITABLE);
			terrain->Mask(args);

			// And ceiling
			bounds.top = passageBounds.top - bounds.height;
			args.SetBounds(bounds);
			terrain->Mask(args);
		}
		

		// Add indestructible ground
		MaskOperation args;
		sf::FloatRect bounds;
		bounds.top = passageBounds.top - wallSide;
		bounds.width = wallSide;
		bounds.height = passageBounds.height + 2 * wallSide;
		const int side = Math::Rnd(0, 1);
		if (side == 0)
		{
			// Left
			bounds.left = passageBounds.left - bounds.width;
			args.SetBrushImage(game->GetRenderer()->RequestTexture("data/textures/objects/metal_wall.png"));
		}
		else if (side == 1)
		{
			// Right
			bounds.left = passageBounds.left + passageBounds.width;
			args.SetBrushImage(game->GetRenderer()->RequestTexture("data/textures/objects/metal_wall.png"));
		}
		args.SetBounds(bounds);
		args.mBrushMode = BrushMode::WrapBounds;
		args.mDoBitmaskOnlyOnVisibleDestPixels = true;

		args.mFirstPassBitwiseOperation = BitwiseOp::Or;
		args.mFirstPassMask = PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE | PA_METAL;
		args.mSecondPassBitwiseOperation = BitwiseOp::And;
		args.mSecondPassMask = ~(PA_EDITABLE);

		terrain->Mask(args);

	}
}

bool PassageGenerator::HasAnotherPredefinedPassage()
{
	return !mPredefinedPassageBounds.empty();
}

sf::FloatRect PassageGenerator::GetAnotherPredefinedPassage()
{
	sf::FloatRect passage = mPredefinedPassageBounds.front();
	mPredefinedPassageBounds.pop();
	return passage;
}

PassageGenerator::PassageGenerator(LocationGenerator * parentGenerator)
	: LocationGeneratorModule(parentGenerator)
{
	rapidjson::Document doc;
	LoadJSON("data/generators/passage_generator.json", doc);
	auto root = doc.GetObject();
	if (root.HasMember("passageSpacing"))
	{
		mPassageSpacing = root["passageSpacing"].GetFloat();
	}
	if (root.HasMember("maxLinkHeight"))
	{
		mLinkMaxHeight = root["maxLinkHeight"].GetFloat();
	}
	if (root.HasMember("minLinkWidth"))
	{
		mLinkMinWidth = root["minLinkWidth"].GetFloat();
	}
	if (root.HasMember("maxLinkWidth"))
	{
		mLinkMaxWidth = root["maxLinkWidth"].GetFloat();
	}
	if (root.HasMember("roomCorridorLength"))
	{
		mRoomCorridorLength = root["roomCorridorLength"].GetFloat();
	}
	if (root.HasMember("passageLightDistance"))
	{
		mPassageLightDistance = root["passageLightDistance"].GetFloat();
	}
	if (root.HasMember("ladderWidth"))
	{
		mLadderWidth = root["ladderWidth"].GetFloat();
	}
	if (root.HasMember("minPassageWidth"))
	{
		mPassageMinWidth = root["minPassageWidth"].GetFloat();
	}
	if (root.HasMember("maxPassageWidth"))
	{
		mPassageMaxWidth = root["maxPassageWidth"].GetFloat();
	}
	if (root.HasMember("minPassageHeight"))
	{
		mPassageHeightMin = root["minPassageHeight"].GetFloat();
	}
	if (root.HasMember("maxPassageHeight"))
	{
		mPassageHeightMax = root["maxPassageHeight"].GetFloat();
	}
	if (root.HasMember("minRoomWidth"))
	{
		mRoomWidthMin = root["minRoomWidth"].GetFloat();
	}
	if (root.HasMember("maxRoomWidth"))
	{
		mRoomWidthMax = root["maxRoomWidth"].GetFloat();
	}
	if (root.HasMember("minRoomHeight"))
	{
		mRoomHeightMin = root["minRoomHeight"].GetFloat();
	}
	if (root.HasMember("maxRoomHeight"))
	{
		mRoomHeightMax = root["maxRoomHeight"].GetFloat();
	}
	if (root.HasMember("roomSpacing"))
	{
		mRoomSpacing = root["roomSpacing"].GetFloat();
	}
	if (root.HasMember("columnPlacementDistance"))
	{
		mColumnPlacementDistance = root["columnPlacementDistance"].GetFloat();
	}
}

PassageGenerator::~PassageGenerator()
{

}

void PassageGenerator::SetArea(const sf::FloatRect & area)
{
	mArea = area;
}

void PassageGenerator::AddForbiddenArea(const sf::FloatRect & area)
{
	mForbiddenZones.push_back(area);
}

void PassageGenerator::AddPredefinedPassageBounds(const sf::FloatRect & bounds)
{
	mPredefinedPassageBounds.push(bounds);
}

sf::FloatRect PassageGenerator::GetArea() const
{
	return mArea;
}

void PassageGenerator::Generate()
{
	GeneratePassages();
	GeneratePassageLinks();
	GenerateRooms();
	GenerateLights();
	GeneratePassageSpacers();
	GenerateMotherCrystals();
	GenerateDecorations();
	GenerateDetails();
}

void PassageGenerator::SetDensity(const PassageDensity & density)
{
	mDensity = density;
}

void PassageGenerator::AllowGenerateMonsters(bool state)
{
	mGenerateMonsters = state;
}

void PassageGenerator::AllowGenerateInhabitants(bool state)
{
	mAllowInhabitants = state;
}
