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
#include "HouseGenerator.h"
#include "Bundle.h"
#include "Room.h"
#include "Terrain.h"
#include "ResourceDatabase.h"
#include "Renderer.h"
#include "LocationMap.h"
#include "Ladder.h"
#include "Door.h"
#include "NPC.h"

HouseGenerator::HouseGenerator(LocationGenerator * parentGenerator) : LocationGeneratorModule(parentGenerator)
{

}

void HouseGenerator::SetBounds(const sf::FloatRect & bounds)
{
	mBounds = bounds;
}

void HouseGenerator::Generate()
{
	mParentGenerator->SetProgressMessage("Generating house...");

	vector<DecorationTemplateType> mDecorationGroups = {
		DecorationTemplateType::Cabinet,
		DecorationTemplateType::Bed
	};

	const shared_ptr<Location> location = mParentGenerator->GetLocation();
	const shared_ptr<Scene> scene = location->GetScene();
	const shared_ptr<Terrain> terrain = scene->GetTerrain();

	const float wallsOffset = 20.0f;

	shared_ptr<Room> room = make_shared<Room>();
	location->AddRoom(room);

	sf::FloatRect livingBounds = mBounds;
	livingBounds.left += wallsOffset;
	livingBounds.width -= 2 * wallsOffset;
	room->SetBounds(livingBounds);

	// Add pillars
	{
		auto pillarTemplate = game->GetResourceDatabase()->GetRandom(DecorationTemplateType::Pillar);
		const float xStep = (mBounds.width - mPillarWidth) / mPillarCountRange.RandomValue();
		for (float x = 0; x < mBounds.width; x += xStep)
		{
			shared_ptr<Sprite> pillar = scene->CreateSprite();
			pillar->SetLocalPosition({
				mBounds.left + x,
				mBounds.top + mBounds.height
			});

			auto traceResult = terrain->TraceRay(pillar->GetGlobalPosition(), { 0, FLT_MAX });

			pillar->SetSize(mPillarWidth, abs(traceResult.mPosition.y - pillar->GetGlobalPosition().y));
			pillar->SetTexture(pillarTemplate->mTexture);
		}
	}

	sf::FloatRect base;
	{
		base.left = mBounds.left;
		base.top = mBounds.top + mBounds.height - mBaseHeight;
		base.width = mBounds.width;
		base.height = mBaseHeight;

		// Draw base
		MaskOperation args;
		args.mEditorAccess = true;
		args.SetBounds(base);
		args.mFirstPassMask = PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE | PA_METAL;
		args.mFirstPassBitwiseOperation = BitwiseOp::Or;
		args.SetBrushImage(game->GetRenderer()->RequestTexture("data/textures/objects/concrete.png"));
		terrain->Mask(args);
	}

	Math::Polygon outerWalls;
	{
		const float roofPart = 0.45f;
		outerWalls.AddPoint({ 0, mBounds.height * roofPart });
		outerWalls.AddPoint({ mBounds.width * 0.5f, 0 });
		outerWalls.AddPoint({ mBounds.width, mBounds.height * roofPart });
		outerWalls.AddPoint({ mBounds.width - wallsOffset, mBounds.height * roofPart });
		outerWalls.AddPoint({ mBounds.width - wallsOffset, mBounds.height - mBaseHeight });
		outerWalls.AddPoint({ wallsOffset, mBounds.height - mBaseHeight });
		outerWalls.AddPoint({ wallsOffset, mBounds.height * roofPart });
		for (auto & pt : outerWalls.GetPoints())
		{
			pt += {mBounds.left, mBounds.top};
		}

		WallTemplate* wallTemplate = game->GetResourceDatabase()->GetRandom(WallTemplateType::Wood);

		MaskOperation args;
		args.mEditorAccess = true;
		args.SetBounds(outerWalls);
		args.mFirstPassMask = PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE | PA_EDITABLE | PA_METAL;
		args.mFirstPassBitwiseOperation = BitwiseOp::Or;
		args.SetBrushImage(wallTemplate->mHorizontal);
		terrain->Mask(args);

		location->GetMap()->DrawPolygon(outerWalls, LocationMap::PassageColor);
	}

	{
		Math::Polygon livingSpace = outerWalls;
		livingSpace.UniformScale(livingSpace.Center() - Vec2f(0, 2.5f * mBaseHeight), 0.95f);

		MaskOperation args;
		args.mEditorAccess = true;
		args.SetBounds(livingSpace);
		args.mFirstPassMask = ~(PA_EDITABLE | PA_COLLIDABLE | PA_RAYTRACEABLE | PA_METAL);
		args.mFirstPassBitwiseOperation = BitwiseOp::And;
		args.SetBrushImage(game->GetRenderer()->RequestTexture("data/textures/objects/log.png"));
		terrain->Mask(args);
	}

	const int side = Math::Rnd(0, 1);

	// Add door
	{
		const float doorHeight = 65;
		const Vec2f doorPosition = side == 0 ?
			Vec2f { mBounds.left + wallsOffset, mBounds.top + mBounds.height - 1.2f * doorHeight - mBaseHeight } :
			Vec2f { mBounds.left + mBounds.width - wallsOffset - 5, mBounds.top + mBounds.height - 1.2f * doorHeight - mBaseHeight };
		shared_ptr<Door> door = make_shared<Door>(location, DoorType::Internal, DoorModelType::Side, doorPosition);
		door->SetHeight(doorHeight);
		location->AddDoor(door);
	}

	// Add ladder
	{
		shared_ptr<Ladder> ladder = make_shared<Ladder>(location->GetScene());
		const float width = 30;
		ladder->SetPosition({
			side == 0 ? base.left - width : base.left + base.width + 1,
			base.top
		});
		auto traceResult = terrain->TraceRay(ladder->GetPosition(), { 0, FLT_MAX });
		ladder->SetSize({ width, abs(traceResult.mPosition.y - ladder->GetPosition().y) });
		location->AddLadder(ladder);
	}

	// Add lights
	mParentGenerator->CreateTorchLight(Math::RectCenter(mBounds), 70);

	// Add decorations
	const int decorationCount = static_cast<int>(livingBounds.width / 30.0f);
	vector<sf::FloatRect> decorationBoundsList;
	for (int k = 0; k < decorationCount; ++k)
	{
		auto randomDecoration = game->GetResourceDatabase()->GetRandom(Math::RandomElement(mDecorationGroups));

		sf::FloatRect decorationBounds;
		decorationBounds.width = randomDecoration->mSize.x;
		decorationBounds.height = randomDecoration->mSize.y;
		decorationBounds.top = livingBounds.top + livingBounds.height - decorationBounds.height - mBaseHeight;

		int maxTries = 30;
		bool placeIsValid = true;
		while (--maxTries >= 0)
		{
			decorationBounds.left = livingBounds.left + Math::Rnd(0.0f, livingBounds.width - decorationBounds.width - 6);

			placeIsValid = true;
			for (auto & otherDecoration : decorationBoundsList)
			{
				if (otherDecoration.intersects(decorationBounds))
				{
					placeIsValid = false;
					break;
				}
			}

			if (placeIsValid)
			{
				break;
			}
		}

		if (!placeIsValid)
		{
			// No free space, leave loop
			break;
		}

		mParentGenerator->InsertDecoration(decorationBounds, randomDecoration->mTexture);
		decorationBoundsList.push_back(decorationBounds);
	}

	// Add inhabitant(s)
	{
		int inhabitantCount = Math::Rnd(1, 2);
		float relativeX = 20;
		const float xStep = (livingBounds.width - 2 * relativeX) / (inhabitantCount + 1);
		for (int i = 0; i < inhabitantCount; ++i)
		{
			const Vec2f position = {
				livingBounds.left + relativeX,
				livingBounds.top + livingBounds.height * 0.5f
			};

			auto npc = make_shared<NPC>(location, NPCType::Local);
			npc->Randomize();
			npc->SetPosition(position);
			npc->SetDefaultPosition(position);
			location->AddActor(npc);

			relativeX += xStep;
		}
	}
}
