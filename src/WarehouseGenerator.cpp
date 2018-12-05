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
#include "WarehouseGenerator.h"
#include "Ladder.h"
#include "LocationMap.h"
#include "Location.h"
#include "Terrain.h"
#include "ResourceDatabase.h"
#include "Room.h"

void WarehouseGenerator::Generate()
{
	const shared_ptr<Location> location = mParentGenerator->GetLocation();
	const shared_ptr<Scene> scene = location->GetScene();
	const shared_ptr<Terrain> terrain = scene->GetTerrain();

	// Add entrance
	sf::FloatRect ladderPassage;
	ladderPassage.width = Math::Clamp(mBounds.width * 0.2f, 35.0f, 47.0f);
	ladderPassage.left = mBounds.left + mBounds.width * 0.5f - ladderPassage.width * 0.5f;
	ladderPassage.top = mEntranceY - 2 * Terrain::SmoothKernelSize;
	ladderPassage.height = abs(mBounds.top - mEntranceY) + 1 + 2 * Terrain::SmoothKernelSize;

	// Add ladder
	{
		const Vec2f size = {
			0.7f * ladderPassage.width,
			abs(mEntranceY - (mBounds.top + mBounds.height))
		};

		const Vec2f position = {
			ladderPassage.left + ladderPassage.width*0.5f - size.x * 0.5f,
			ladderPassage.top
		};

		shared_ptr<Ladder> ladder = make_shared<Ladder>(location->GetScene());
		ladder->SetPosition(position);
		ladder->SetSize(size);
		location->AddLadder(ladder);
	}

	{
		MaskOperation args;
		args.SetBounds(mBounds);
		args.mFirstPassBitwiseOperation = BitwiseOp::And;
		args.mFirstPassMask = ~(PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE | PA_EDITABLE);
		terrain->Mask(args);
		location->GetMap()->DrawRect(mBounds, LocationMap::PassageColor);

		args.SetBounds(ladderPassage);
		terrain->Mask(args);
		location->GetMap()->DrawRect(ladderPassage, LocationMap::PassageColor);
	}

	// Add lights
	for (int side = 0; side < 2; ++side)
	{
		Vec2f position;

		position.y = mBounds.top + mBounds.height * 0.50f;

		if (side == 0)
		{
			// Left
			position.x = mBounds.left + mBounds.width * 0.25f;
		}
		else
		{
			// Right
			position.x = mBounds.left + mBounds.width * 0.75f;
		}

		mParentGenerator->CreateTorchLight(position, Math::Rnd(40.0f, 60.0f));
	}

	// Add walls
	{
		auto wallTemplate = game->GetResourceDatabase()->GetRandom(WallTemplateType::Metal);

		auto InsertHorizontalWall = [wallTemplate, terrain] (float x, float y, float w, float h)
		{
			MaskOperation args;
			args.SetBounds(sf::FloatRect(x, y, w, h));
			args.mFirstPassBitwiseOperation = BitwiseOp::Or;
			args.mFirstPassMask = PA_VISIBLE | PA_METAL;
			args.mBrushMode = BrushMode::WrapBounds;
			args.SetBrushImage(wallTemplate->mHorizontal);
			terrain->Mask(args);
		};

		auto InsertVerticalWall = [wallTemplate, terrain] (float x, float y, float w, float h)
		{
			MaskOperation args;
			args.SetBounds(sf::FloatRect(x, y, w, h));
			args.mFirstPassBitwiseOperation = BitwiseOp::Or;
			args.mFirstPassMask = PA_VISIBLE | PA_METAL;
			args.mBrushMode = BrushMode::WrapBounds;
			args.SetBrushImage(wallTemplate->mVertical);
			terrain->Mask(args);
		};

		InsertHorizontalWall(mBounds.left, mBounds.top + mBounds.height, mBounds.width, mWallThickness);
		InsertVerticalWall(mBounds.left - mWallThickness, mBounds.top - mWallThickness, mWallThickness, mBounds.height + 2 * mWallThickness);
		InsertVerticalWall(mBounds.left + mBounds.width, mBounds.top - mWallThickness, mWallThickness, mBounds.height + 2 * mWallThickness);
		InsertHorizontalWall(mBounds.left, mBounds.top - mWallThickness, (mBounds.width - ladderPassage.width) * 0.5f, mWallThickness);
		InsertHorizontalWall(mBounds.left + (mBounds.width + ladderPassage.width) * 0.5f, mBounds.top - mWallThickness, (mBounds.width - ladderPassage.width) * 0.5f, mWallThickness);
		InsertVerticalWall(ladderPassage.left - mWallThickness, ladderPassage.top - mWallThickness, mWallThickness, ladderPassage.height + mWallThickness);
		InsertVerticalWall(ladderPassage.left + ladderPassage.width, ladderPassage.top - mWallThickness, mWallThickness, ladderPassage.height + mWallThickness);
	}

	// Define possible decoration group types (group of groups, yeah :) )
	static vector<DecorationTemplateType> decorationGroups = {
		DecorationTemplateType::Cabinet,
		DecorationTemplateType::Barrel,
		DecorationTemplateType::WoodBox,
	};

	// Add decorations
	const int decorationCount = static_cast<int>(mBounds.width / 30.0f);
	vector<sf::FloatRect> decorationBoundsList;
	for (int k = 0; k < decorationCount; ++k)
	{
		auto randomDecoration = game->GetResourceDatabase()->GetRandom(Math::RandomElement(decorationGroups));

		sf::FloatRect decBounds;
		decBounds.width = randomDecoration->mSize.x;
		decBounds.height = randomDecoration->mSize.y;
		decBounds.top = mBounds.top + mBounds.height - decBounds.height;

		int maxTries = 30;
		bool placeIsValid = true;
		while (--maxTries >= 0)
		{
			decBounds.left = mBounds.left + Math::Rnd(0.0f, mBounds.width - decBounds.width);

			placeIsValid = true;
			for (auto & otherDecoration : decorationBoundsList)
			{
				if (otherDecoration.intersects(decBounds))
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

		mParentGenerator->InsertDecoration(decBounds, randomDecoration->mTexture);
		decorationBoundsList.push_back(decBounds);
	}

	const shared_ptr<Room> room = make_shared<Room>();
	room->SetBounds(mBounds);
	room->SetEntrance(ladderPassage);
	location->AddRoom(room);
}

WarehouseGenerator::WarehouseGenerator(LocationGenerator * parentGenerator) : LocationGeneratorModule(parentGenerator)
{

}

void WarehouseGenerator::SetBounds(const sf::FloatRect & bounds)
{
	mBounds = bounds;
}

void WarehouseGenerator::SetEntranceY(const float entranceY)
{
	mEntranceY = entranceY;
}
