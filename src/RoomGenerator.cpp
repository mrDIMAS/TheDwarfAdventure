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
#include "RoomGenerator.h"
#include "Texture.h"
#include "ResourceDatabase.h"
#include "Game.h"
#include "LocationGenerator.h"
#include "Location.h"
#include "Scene.h"
#include "Terrain.h"
#include "Ladder.h"
#include "Door.h"
#include "LocationMap.h"
#include "Room.h"
#include "ItemDatabase.h"
#include "NPC.h"
#include "Workbench.h"
#include "Chest.h"
#include "Level.h"
#include "Inventory.h"

Vec2f ReadVector(const rapidjson::GenericValue<rapidjson::UTF8<>>& value)
{
	auto array = value.GetArray();
	if (array.Size() != 2)
	{
		throw Exception("Vector must have two elements in array!");
	}
	return { array[0].GetFloat(), array[1].GetFloat() };
}

RoomGenerator::RoomGenerator(LocationGenerator * parentGenerator) : LocationGeneratorModule(parentGenerator)
{
	rapidjson::Document doc;
	LoadJSON("data/generators/room_generator.json", doc);
	auto root = doc.GetObject();

	if (root.HasMember("wallThickness"))
	{
		mWallThickness = root["wallThickness"].GetFloat();
	}
	if (root.HasMember("ladderWidth"))
	{
		mLadderWidth = root["ladderWidth"].GetFloat();
	}
}

void RoomGenerator::Generate()
{
	mParentGenerator->SetProgressMessage("Generating room...");

	const shared_ptr<Location> location = mParentGenerator->GetLocation();
	const shared_ptr<Terrain> terrain = location->GetScene()->GetTerrain();

	// Create room
	const shared_ptr<Room> room = make_shared<Room>();
	location->AddRoom(room);
	room->SetBounds(mBounds);
	room->SetEntrance(mEntranceBounds);

	location->GetMap()->DrawRect(mBounds, LocationMap::PassageColor);
	location->GetMap()->DrawRect(mEntranceBounds, LocationMap::PassageColor);

	// Random room generation
	room->SetType(mPossibleRoomTypes.Drop());

	// Default inhabitant position
	Vec2f inhabitantPosition;
	inhabitantPosition.x = mBounds.left + 0.5f * mBounds.width;
	inhabitantPosition.y = mBounds.top + mBounds.height - mAverageInhabitantHeight - mWallThickness;

	// Default chest position
	Vec2f chestPosition;
	chestPosition.x = mBounds.left + 0.5f * mBounds.width;
	chestPosition.y = mBounds.top + 0.5f * mBounds.height;

	const auto resourceDatabase = game->GetResourceDatabase();

	const bool isEntranceRight = mEntranceBounds.left > mBounds.left;

	// Draw backgrounds
	{
		auto background = resourceDatabase->GetRandom(RoomBackgroundType::Metal);

		// Draw room background
		{
			MaskOperation args;
			args.mEditorAccess = true;
			args.mBounds = Bounds::Rect;
			args.mFirstPassBitwiseOperation = BitwiseOp::And;
			args.mFirstPassMask = ~(PA_EDITABLE | PA_RAYTRACEABLE | PA_COLLIDABLE | PA_LAVA | PA_ACID);
			args.mSecondPassBitwiseOperation = BitwiseOp::Or;
			args.mSecondPassMask = PA_VISIBLE;
			args.mPosition = { mBounds.left, mBounds.top };
			args.mRectSize = { mBounds.width, mBounds.height };
			args.mBrushMode = BrushMode::Insert;
			args.SetBrushImage(background->mTexture);
			terrain->Mask(args);
		}

		// Draw entrance background
		{
			MaskOperation args;
			args.mEditorAccess = true;
			args.mBounds = Bounds::Rect;
			args.mFirstPassBitwiseOperation = BitwiseOp::And;
			args.mFirstPassMask = ~(PA_EDITABLE | PA_RAYTRACEABLE | PA_COLLIDABLE | PA_LAVA | PA_ACID);
			args.mSecondPassBitwiseOperation = BitwiseOp::Or;
			args.mSecondPassMask = PA_VISIBLE;
			args.mPosition = { mEntranceBounds.left, mEntranceBounds.top };
			args.mRectSize = { mEntranceBounds.width, mEntranceBounds.height };
			args.mBrushMode = BrushMode::Insert;
			args.SetBrushImage(background->mTexture);
			terrain->Mask(args);
		}
	}

	// define walls
	auto wallTemplate = resourceDatabase->GetRandom(WallTemplateType::Metal);

	auto InsertWall = [&] (const shared_ptr<Texture> & texture, float x, float y, float w, float h)
	{
		MaskOperation args;
		args.mEditorAccess = true;
		args.SetBounds(sf::FloatRect(x, y, w, h));
		args.mFirstPassBitwiseOperation = BitwiseOp::Or;
		args.mFirstPassMask = PA_VISIBLE | PA_METAL | PA_COLLIDABLE | PA_RAYTRACEABLE;
		args.mBrushMode = BrushMode::WrapBounds;
		args.SetBrushImage(texture);
		terrain->Mask(args);
	};

	auto InsertHorizontalWall = [&] (float x, float y, float w, float h)
	{
		InsertWall(wallTemplate->mHorizontal, x, y, w, h);
	};

	auto InsertVerticalWall = [&] (float x, float y, float w, float h)
	{
		InsertWall(wallTemplate->mVertical, x, y, w, h);
	};

	// Left wall
	InsertVerticalWall(mBounds.left, mBounds.top, mWallThickness, isEntranceRight ? mBounds.height : mBounds.height - mEntranceBounds.height);
	// Top wall
	InsertHorizontalWall(mBounds.left, mBounds.top, mBounds.width, mWallThickness);
	// Right wall
	InsertVerticalWall(mBounds.left + mBounds.width - mWallThickness, mBounds.top, mWallThickness, isEntranceRight ? mBounds.height - mEntranceBounds.height : mBounds.height);
	// Bottom wall
	InsertHorizontalWall(mBounds.left, mBounds.top + mBounds.height - mWallThickness, mBounds.width, mWallThickness);
	// Entrance top wall
	InsertHorizontalWall(isEntranceRight ? mEntranceBounds.left - mWallThickness : mEntranceBounds.left, mEntranceBounds.top, mEntranceBounds.width + mWallThickness, mWallThickness);
	// Entrance Bottom wall
	InsertHorizontalWall(mEntranceBounds.left, mEntranceBounds.top + mEntranceBounds.height - mWallThickness, mEntranceBounds.width, mWallThickness);

    bool needChest = false;
    
	if (room->GetType() == RoomType::UndergroundApartment)
	{
		// Define floor separators - ceilings for each floor
		int floorCount = static_cast<int>(mBounds.height / mAverageInhabitantHeight);

		if (floorCount > 1)
		{
			float yStep = mBounds.height / floorCount;
			float y = mBounds.top + yStep;
			for (int i = 0; i < floorCount - 1; ++i)
			{
				float x = mBounds.left + mWallThickness + mLadderWidth;
				float w = mBounds.width - mLadderWidth - 2 * mWallThickness;
				if (i == 0)
				{
					// Chest is on upper floor
					needChest = true;
					chestPosition.x = x + w * 0.5f;
					chestPosition.y = y - yStep * 0.5f;
				}

				InsertHorizontalWall(x, y, w, mWallThickness);
				// Move next separator down
				y += yStep;
			}
		}

		// Add bed		
		auto bedTemplate = resourceDatabase->GetRandom(DecorationTemplateType::Bed);
		const Vec2f bedPosition = {
			mBounds.left + mBounds.width - bedTemplate->mSize.x - mWallThickness,
			mBounds.top + mBounds.height - mWallThickness - bedTemplate->mSize.y
		};
		mParentGenerator->AddTerrainDecoration(bedPosition, bedTemplate->mSize, bedTemplate->mTexture);

		// Add table		
		auto tableTemplate = resourceDatabase->GetRandom(DecorationTemplateType::Table);
		const Vec2f tablePosition = {
			mBounds.left + tableTemplate->mSize.x / 2 + mWallThickness,
			mBounds.top + mBounds.height - mWallThickness - tableTemplate->mSize.y
		};
		mParentGenerator->AddTerrainDecoration(tablePosition, tableTemplate->mSize, tableTemplate->mTexture);

		// Add vase		
		auto vaseTemplate = resourceDatabase->GetRandom(DecorationTemplateType::Vase);
		const Vec2f vasePosition = {
			tablePosition.x + vaseTemplate->mSize.x / 2 - vaseTemplate->mSize.x / 2,
			tablePosition.y - vaseTemplate->mSize.y
		};
		mParentGenerator->AddTerrainDecoration(vasePosition, vaseTemplate->mSize, vaseTemplate->mTexture);

		// Add painting				
		auto paintingTemplate = resourceDatabase->GetRandom(DecorationTemplateType::Painting);
		const Vec2f paintingPosition = {
			mBounds.left + mBounds.width / 2 - paintingTemplate->mSize.x / 2,
			vasePosition.y - 1.5f * paintingTemplate->mSize.y
		};
		mParentGenerator->AddTerrainDecoration(paintingPosition, paintingTemplate->mSize, paintingTemplate->mTexture);

		// Add light
		mParentGenerator->CreateTorchLight({ mBounds.left + mBounds.width / 2, mBounds.top + mBounds.height / 2 }, 100.0f);

		// Add cabinets
		if (floorCount > 1)
		{
			float yStep = mBounds.height / floorCount;
			float y = mBounds.top + yStep;
			for (int i = 0; i < floorCount - 1; ++i)
			{
				auto cabinetTemplate = resourceDatabase->GetRandom(DecorationTemplateType::Cabinet);
				float x = mBounds.left + mWallThickness + mLadderWidth;
				int cabinetCount = static_cast<int>((mBounds.width - mLadderWidth - 2 * mWallThickness) / cabinetTemplate->mSize.x);
				float spacing = 2;
				for (int j = 0; j < cabinetCount; ++j)
				{
					cabinetTemplate = resourceDatabase->GetRandom(DecorationTemplateType::Cabinet);

					mParentGenerator->AddTerrainDecoration({ x, y - cabinetTemplate->mSize.y }, cabinetTemplate->mSize, cabinetTemplate->mTexture);

					x += cabinetTemplate->mSize.x + spacing;
					// out of bounds check
					if (x + cabinetTemplate->mSize.x > mBounds.left + mBounds.width - mWallThickness)
					{
						break;
					}
				}
				y += yStep;
			}
		}

		// Define ladder
		if (floorCount > 1)
		{
			float floorHeight = mBounds.height / floorCount;
			float y = mBounds.top + floorHeight;
			float h = mBounds.height - floorHeight;

			shared_ptr<Ladder> ladder = make_shared<Ladder>(location->GetScene());
			ladder->SetPosition({ mBounds.left + mWallThickness, y });
			ladder->SetSize({ mLadderWidth, h });
			location->AddLadder(ladder);
		}
	}
	else if (room->GetType() == RoomType::Forge)
	{
		float groundY = mBounds.top + mBounds.height - mWallThickness;

		// Add furnace
		auto furnaceTemplate = resourceDatabase->GetRandom(DecorationTemplateType::Furnace);
		const Vec2f furnacePosition = {
			mBounds.left + mBounds.width - furnaceTemplate->mSize.x - mWallThickness,
			groundY - furnaceTemplate->mSize.y
		};
		mParentGenerator->AddTerrainDecoration(furnacePosition, furnaceTemplate->mSize, furnaceTemplate->mTexture);

		// Add anvil
		auto anvilTemplate = resourceDatabase->GetRandom(DecorationTemplateType::Anvil);
		const Vec2f anvilPosition = {
			furnacePosition.x - 2 - anvilTemplate->mSize.x,
			groundY - anvilTemplate->mSize.y
		};
		mParentGenerator->AddTerrainDecoration(anvilPosition, anvilTemplate->mSize, anvilTemplate->mTexture);

		// Add bench
		auto benchTemplate = resourceDatabase->GetRandom(DecorationTemplateType::Bench);
		const sf::FloatRect workbenchBounds = {
			anvilPosition.x - 2 - benchTemplate->mSize.x,
			groundY - benchTemplate->mSize.y,
			benchTemplate->mSize.x,
			benchTemplate->mSize.y
		};
		location->AddWorkbench(make_shared<Workbench>(location->GetScene(), workbenchBounds, benchTemplate->mTexture));

		// Add light
		mParentGenerator->CreateTorchLight({ mBounds.left + mBounds.width / 2, mBounds.top + mBounds.height / 2 }, 100);
	}
	else if (room->GetType() == RoomType::Store)
	{
		// Inner ceiling
		const float innerCeilingX = mBounds.left + mWallThickness + mLadderWidth;
		const float innerCeilingY = mBounds.top + mBounds.height / 2 - mWallThickness;
		const float innerCeilingW = mBounds.width - 2 * (mWallThickness + mLadderWidth);
		InsertHorizontalWall(innerCeilingX, innerCeilingY, innerCeilingW, mWallThickness);

		// Door box
		const float doorBoxX = mBounds.left + mBounds.width / 2;
		const float doorBoxY = mBounds.top + mWallThickness;
		const float doorBoxH = innerCeilingY - mBounds.top - mAverageInhabitantHeight;
		InsertVerticalWall(doorBoxX, doorBoxY, mWallThickness, doorBoxH);

		// Door
		{
			const float doorHeight = mAverageInhabitantHeight - mWallThickness;
			const Vec2f doorPosition = { doorBoxX + 2, doorBoxY + doorBoxH };
			shared_ptr<Door> door = make_shared<Door>(location, DoorType::Internal, DoorModelType::Side, doorPosition);
			door->SetHeight(doorHeight);
			location->AddDoor(door);
			room->AddDoor(door);
		}

		// Talk mesh		
		InsertWall(resourceDatabase->GetRandom(WallTemplateType::MetalMesh)->mVertical, doorBoxX, innerCeilingY + mWallThickness, mWallThickness, innerCeilingY - mBounds.top);

		// Left light
		mParentGenerator->CreateTorchLight({ innerCeilingX, mBounds.top + 0.75f * mBounds.height }, 100);

		// Right light
		mParentGenerator->CreateTorchLight({ innerCeilingX + innerCeilingW, mBounds.top + 0.75f * mBounds.height }, 100);

		// Trader position
		inhabitantPosition.y = mBounds.top + mBounds.height - mAverageInhabitantHeight * 0.5f - mWallThickness;
		if (isEntranceRight)
		{
			inhabitantPosition.x = mBounds.left + 0.25f * mBounds.width;
		}
		else
		{
			inhabitantPosition.x = mBounds.left + 0.75f * mBounds.width;
		}

		// Chest is on trader position
		needChest = true;
		chestPosition = inhabitantPosition;

		// Define illegal zone
		sf::FloatRect illegalZone;
		illegalZone.top = mBounds.top + mWallThickness;
		illegalZone.width = mBounds.width * 0.5f - 2 * mWallThickness;
		illegalZone.height = mBounds.height - 2 * mWallThickness;
		if (isEntranceRight)
		{
			illegalZone.left = mBounds.left + mWallThickness;
		}
		else
		{
			illegalZone.left = mBounds.left + mWallThickness + mBounds.width * 0.5f;
		}
		room->AddIllegalZone(illegalZone);

		// Left ladder
		{
			shared_ptr<Ladder> ladder = make_shared<Ladder>(location->GetScene());
			ladder->SetPosition({ mBounds.left + mWallThickness, mBounds.top + mBounds.height / 2 });
			ladder->SetSize({ mLadderWidth, mBounds.height / 2 - mWallThickness });
			location->AddLadder(ladder);
		}

		// Right ladder
		{
			shared_ptr<Ladder> ladder = make_shared<Ladder>(location->GetScene());
			ladder->SetPosition({ mBounds.left + mBounds.width - mWallThickness - mLadderWidth, mBounds.top + mBounds.height / 2 });
			ladder->SetSize({ mLadderWidth, mBounds.height / 2 - mWallThickness });
			location->AddLadder(ladder);
		}
	}

	// Entrance door position
	{
		const float doorHeight = mEntranceBounds.height;
		const Vec2f doorPosition = isEntranceRight ? Vec2f(mEntranceBounds.left + mEntranceBounds.width, mEntranceBounds.top) : Vec2f(mEntranceBounds.left, mEntranceBounds.top);
		shared_ptr<Door> door = make_shared<Door>(location, DoorType::Internal, DoorModelType::Side, doorPosition);
		door->SetHeight(doorHeight);
		location->AddDoor(door);
		room->AddDoor(door);
	}

	NPCType npcType;
	switch (room->GetType())
	{
	case RoomType::Forge:
		npcType = NPCType::Blacksmith;
		break;
	case RoomType::Store:
		npcType = NPCType::Merchant;
		break;
	default:
		npcType = NPCType::Local;
	}

	// Create chest
    if(needChest)
	{
		ItemDatabase* db = game->GetItemDatabase();

		const Bundle<Bundle<ItemType>> bundles =
		{			
			{ 0.8f, db->MakeBundleOfClassByRarity(ItemClass::Metal, 30) },
			{ 0.15f, db->MakeBundleOfClassByRarity(ItemClass::Weapon, 15) },
			{ 0.3f, db->MakeBundleOfClassByRarity(ItemClass::Projectile, 15) },
			{ 0.8f, db->MakeBundleOfClassByRarity(ItemClass::Potion, 30) },
			{ 0.8f, db->MakeBundleOfClassByRarity(ItemClass::Crystal, 15) },
			{ 0.1f, db->MakeBundleOfClassByRarity(ItemClass::Armor, 15) },
			{ 0.1f, db->MakeBundleOfClassByRarity(ItemClass::Helmet, 15) },
			{ 0.1f, db->MakeBundleOfClassByRarity(ItemClass::Trousers, 15) },
			{ 0.1f, db->MakeBundleOfClassByRarity(ItemClass::Gloves, 15) },
			{ 1.0f, db->MakeBundleOfClassByRarity(ItemClass::Ingredient, 30) },
			{ 0.09f, db->MakeBundleOfClassByRarity(ItemClass::Book, 15) }
		};
		
		const Difficulty difficulty = mParentGenerator->GetLocation()->GetLevel()->GetDifficulty();

		const float itemPerChestCoeff =
			difficulty == Difficulty::Easy ? 1.0f :
			difficulty == Difficulty::Medium ? 0.8f :
			difficulty == Difficulty::Hard ? 0.55f :
			difficulty == Difficulty::Realistic ? 0.4f
			: 1.0f;

		const Math::Range<int> itemPerChestRange =
		{
			static_cast<int>(2 * itemPerChestCoeff),
			static_cast<int>(6 * itemPerChestCoeff)
		};

		const float moneyCountCoeff =
			difficulty == Difficulty::Easy ? 1.5f :
			difficulty == Difficulty::Medium ? 1.0f :
			difficulty == Difficulty::Hard ? 0.7f :
			difficulty == Difficulty::Realistic ? 0.5f
			: 1.0f;

		const Math::Range<int> moneyCountRange
		{
			static_cast<int>(5 * moneyCountCoeff),
			static_cast<int>(25 * moneyCountCoeff)
		};
		
		const shared_ptr<Chest> chest = make_shared<Chest>(location->GetScene());

		const int itemCount = itemPerChestRange.RandomValue();
		for (int k = 0; k < itemCount; )
		{
			// Get a bundle for a random class
			const Bundle<ItemType>& bundle = bundles.Drop();

			if (!bundle.IsEmpty())
			{
				// Drop random item from class
				const ItemType itemType = bundle.Drop();

				chest->GetInventory()->AddItem(db->Make(itemType, location));

				++k;
			}
		}

		chest->GetInventory()->AddMoney(moneyCountRange.RandomValue());		
		
		// Fix position using ray cast
		TerrainRayTraceResult rt = terrain->TraceRay(chestPosition, Vec2f(0, 100));
		if (rt.mCell)
		{
			chestPosition = rt.mPosition;
			chestPosition.y -= chest->GetModel()->GetSize().y * 0.5f;
		}

		chest->SetPosition(chestPosition);
		location->AddChest(chest);
	}

	if (mAllowInhabitants)
	{
		const shared_ptr<NPC> npc = std::make_shared<NPC>(location, npcType);
		npc->Randomize();
		npc->SetPosition(inhabitantPosition);
		npc->SetDefaultPosition(inhabitantPosition);
		npc->SetRoom(room);
		location->AddActor(npc);
		room->AddInhabitant(npc);

		shared_ptr<Item> key = game->GetItemDatabase()->Make(ItemType::Key_Rusty, location);
		npc->AddItem(key);

		for (const shared_ptr<Door> & door : room->GetDoors())
		{
			door->SetKey(key);
		}
	}
}

void RoomGenerator::AllowInhabitants(bool state)
{
	mAllowInhabitants = state;
}
