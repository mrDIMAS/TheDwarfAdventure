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
#include "LocationGenerator.h"
#include "LocationMap.h"
#include "Location.h"
#include "Scene.h"
#include "Terrain.h"
#include "ResourceDatabase.h"
#include "TextureStack.h"
#include "Level.h"
#include "Renderer.h"
#include "Light.h"
#include "MotherCrystal.h"
#include "Item.h"
#include "Chest.h"
#include "Inventory.h"
#include "Passage.h"
#include "Cloud.h"
#include "UIHelpers.h"

mutex LocationGenerator::SyncMutex;

LocationGenerator::LocationGenerator(Level* level, const shared_ptr<LocationDefinition>& params)
	: mDefinition(params)
	, mLevel(level)
{
	lock_guard<mutex> guard(SyncMutex);
	mMonsterGenerator = make_unique<MonsterGenerator>();
}

LocationGenerator::~LocationGenerator()
{

}

shared_ptr<Location> LocationGenerator::GetLocation() const
{
	return mLocation;
}

void LocationGenerator::CreateBlankLocation()
{
	lock_guard<mutex> guard(SyncMutex);
	SetProgressMessage("Creating blank location...");
	// create location and disable physics on it until generation is not done
	mLocation = make_shared<Location>(mLevel, mDefinition);
	mDefinition->mLocation = mLocation;
	mLocation->GetScene()->GetSimulationIsland()->SetEnabled(false);
	mLevel->AddLocation(mLocation);
}

void LocationGenerator::CreateRandomTerrain()
{
	SetProgressMessage("Generating random terrain...");
	const int backTexSize = 64;
	const auto scene = mLocation->GetScene();
	auto terrain = scene->CreateTerrain({ mDefinition->mWidth, mDefinition->mHeight }, backTexSize);
	mLocation->SetMap(make_unique<LocationMap>(terrain->GetSizePixels()));

	// Fill background texture stack with all available brushes
	vector<TerrainBrush*> brushes;
	auto resourceDatabase = game->GetResourceDatabase();
	resourceDatabase->GetTerrainBrushes(brushes);

	// Sort brushes by its back texture id. It is strictly necessary to get correct rendering
	// GPU will use this index to fetch corresponding texture from volume texture 
	sort(brushes.begin(), brushes.end(), [] (TerrainBrush* left, TerrainBrush* right)
	{
		return left->mStackId < right->mStackId;
	});
	for (TerrainBrush* brush : brushes)
	{
		terrain->GetBackgroundTextureStack()->Push(brush->mBackTexture->GetSource());
	}

	// Fill terrain
	TerrainBrush* soilBrush = resourceDatabase->GetRandom(TerrainBrushClass::Soil);
	MaskOperation fargs;
	fargs.mEditBackground = true;
	fargs.mBackgroundValue = soilBrush->mStackId;
	terrain->Fill(fargs, soilBrush->mFrontTexture);

	// Paint terrain (front and back)
	const float layerHeight = 200;
	vector<TerrainBrushClass> commonTiles = { TerrainBrushClass::Rock, TerrainBrushClass::Soil };
	for (float y = terrain->GetTopBackgroundlessPartHeight(); y < terrain->GetHeightPixels(); y += layerHeight)
	{
		MaskOperation args;
		args.mFirstPassBitwiseOperation = BitwiseOp::Or;
		auto rndTile = resourceDatabase->GetRandom(Math::RandomElement(commonTiles));
		args.SetBrushImage(rndTile->mFrontTexture);
		args.mEditBackground = true;
		args.mBackgroundValue = rndTile->mStackId;
		terrain->DrawLine({ 0, y }, { (float)terrain->GetWidthPixels(), y - 5 }, layerHeight + 10, args);
	}

	// Randomize terrain by placing random objects on it
	const int detailCount = terrain->GetSize().x * terrain->GetSize().y * 4;
	for (int i = 0; i < detailCount; ++i)
	{
		TerrainDecoration* detail = resourceDatabase->GetRandom(TerrainDecorationType::Generic);
		MaskOperation args;
		args.mBounds = Bounds::Rect;
		args.mPosition = { Math::Rnd(0.0f, terrain->GetSizePixels().x), Math::Rnd(0.0f, terrain->GetSizePixels().y) };
		args.mRectSize = detail->mTexture->GetSize();
		args.mFirstPassBitwiseOperation = BitwiseOp::Or;
		args.mBrushMode = BrushMode::Insert;
		args.SetBrushImage(detail->mTexture);
		terrain->Mask(args);
	}
}

void LocationGenerator::InsertDecoration(const sf::FloatRect & bounds, const shared_ptr<Texture> & texture)
{
	MaskOperation args;
	args.mEditorAccess = true;
	args.mFirstPassBitwiseOperation = BitwiseOp::Or;
	args.mFirstPassMask = PA_VISIBLE;
	args.SetBounds(bounds);
	args.mBrushMode = BrushMode::Insert;
	args.mDoBitmaskOnlyOnVisiblePixelsOfReplaceImage = true;
	args.SetBrushImage(texture);
	mLocation->GetScene()->GetTerrain()->Mask(args);
}

void LocationGenerator::CreateParallaxBackground(bool singleLayer, const vector<shared_ptr<Texture>>& layers, bool fullscreen)
{
	SetProgressMessage("Creating parallax background...");

	const shared_ptr<Scene> scene = mLocation->GetScene();
	if (singleLayer)
	{
		scene->GetBackgroundLayers().push_back(make_shared<BackgroundLayer>(layers.back(), 0.0f));
		return;
	}

	int kDepth = 0;
	for (const shared_ptr<Texture> &layerTexture : layers)
	{
		// exponential depth growing
		float depth = exp(kDepth++ / 1.5f);
		auto layer = make_shared<BackgroundLayer>(layerTexture, depth);
		layer->SetFullscreen(fullscreen);
		scene->GetBackgroundLayers().push_back(layer);
	}
}

void LocationGenerator::CreateClouds(int cloudCount, float thickness)
{
	SetProgressMessage("Creating clouds...");

	Renderer* renderer = game->GetRenderer();

	vector<shared_ptr<Texture>> clouds = {
		renderer->RequestTexture("data/textures/objects/cloud.png"),
		renderer->RequestTexture("data/textures/objects/cloud2.png")
	};

	const shared_ptr<Terrain> terrain = mLocation->GetScene()->GetTerrain();
	const float boundsWidth = static_cast<float>(terrain->GetWidthPixels());
	for (int i = 0; i < cloudCount; ++i)
	{
		const float width = Math::Rnd(80.0f, 128.0f);
		const shared_ptr<Sprite> cloud = mLocation->GetScene()->CreateSprite();
		cloud->SetTexture(Math::RandomElement(clouds));
		cloud->SetSize(width, width * 0.5f);
		cloud->SetLocalPosition(Math::Rnd(0.0f, boundsWidth), Math::Rnd(-200.0f, thickness));
		mLocation->mCloudList.push_back(make_shared<Cloud>(Math::Rnd(0.1f, 1.0f), cloud));
	}
}

void LocationGenerator::CreateCrystalLight(const Vec2f & position, float radius)
{
	const shared_ptr<Sprite> crystal = mLocation->GetScene()->CreateSprite();
	crystal->SetSize(6, 6);
	crystal->SetLocalOrigin(3, 3);
	crystal->SetTexture(game->GetRenderer()->RequestTexture("data/textures/objects/light_crystal.png"));

	const shared_ptr<Light> light = mLocation->GetScene()->CreateLight();
	light->SetRadius(radius);
	light->AttachTo(crystal);
	light->SetRadiusJitter(20);
	light->SetColor({ 220, 253, 255 });

	crystal->SetLocalPosition(position);
}

void LocationGenerator::CreateTorchLight(const Vec2f & position, float radius)
{
	auto scene = mLocation->GetScene();
	auto torch = scene->CreateSprite();
	torch->GetAnimation().LoadFromFile("data/animations/torch.fra");
	torch->GetAnimation().SetSequence("burn");
	torch->GetAnimation().SetEnabled(true);
	torch->SetSize(16, 32);
	torch->SetLocalOrigin(8, 16);
	torch->SetTexture(game->GetRenderer()->RequestTexture("data/textures/objects/animated_torch.png"));

	auto light = scene->CreateLight();
	light->SetRadius(radius);
	light->AttachTo(torch);
	light->SetRadiusJitter(20);

	torch->SetLocalPosition(position);
}

void LocationGenerator::CreateMotherCrystal(const Vec2f & position)
{
	auto motherCrystal = make_shared<MotherCrystal>(mLocation);
	motherCrystal->SetPosition(position);
	mLocation->mMotherCrystalList.push_back(motherCrystal);
}

void LocationGenerator::DrawPassage(const sf::FloatRect & bounds, TerrainBrush * brush)
{
	const float wallThickness = 20;

	shared_ptr<Terrain> terrain = mLocation->GetScene()->GetTerrain();
	MaskOperation args;
	args.mBounds = Bounds::Rect;

	auto k = 0;// Terrain::PassageThicknessCoefficient;

	args.mPosition = { bounds.left - k, bounds.top - k };
	args.mRectSize = { bounds.width + 2 * k, bounds.height + 2 * k };
	args.mDoBitmaskOnlyOnVisibleDestPixels = false;
	args.mFirstPassBitwiseOperation = BitwiseOp::And;
	args.mFirstPassMask = ~(PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE | PA_LAVA | PA_ACID);
	args.mEditBackground = true;
	args.mBackgroundValue = brush->mStackId;
	terrain->Mask(args);

	args.mPosition -= {wallThickness / 2, wallThickness / 2};
	args.mRectSize += {wallThickness, wallThickness};

	args.mFirstPassBitwiseOperation = BitwiseOp::Or;
	args.mFirstPassMask = PA_NONE;
	args.mDoBitmaskOnlyOnVisibleDestPixels = true;
	args.SetBrushImage(brush->mFrontTexture);
	terrain->Mask(args);

	mLocation->GetMap()->DrawRect({ args.mPosition, args.mRectSize }, LocationMap::PassageColor);
}

MonsterGenerator * LocationGenerator::GetMonsterGenerator()
{
	return mMonsterGenerator.get();
}

void LocationGenerator::PlaceOre(const sf::FloatRect & area)
{
	SetProgressMessage("Creating ore...");

	Bundle<ItemType> mOreBundle = game->GetItemDatabase()->MakeBundleOfClassByRarity(ItemClass::Ore);

	const auto scene = mLocation->GetScene();
	const auto terrain = scene->GetTerrain();

	int oreCount = terrain->GetSize().x * terrain->GetSize().y / 2;

	switch (mLevel->GetDifficulty())
	{
	case Difficulty::Easy:
		oreCount *= 2;
		break;
	case Difficulty::Medium:
		oreCount = static_cast<int>(oreCount * 1.6f);
		break;
	case Difficulty::Hard:
		oreCount = static_cast<int>(oreCount * 1.2f);
		break;
    case Difficulty::Realistic:
        oreCount = static_cast<int>(oreCount * 1.0f);
        break;
	}

	vector<CellTraceResult> traceResult;

	int maxTries = oreCount * 20;
	int oreAdded = 0;
	while (oreAdded < oreCount && maxTries > 0)
	{
		const Vec2f pos = {
			Math::Rnd(area.left, area.left + area.width),
			Math::Rnd(area.top, area.top + area.height)
		};

		const float side = 20;
		terrain->GatherPixelsInsideRect(pos, { side, side }, 1, 1, traceResult, PA_VISIBLE | PA_COLLIDABLE);

		if (traceResult.size() >= side * side)
		{
			const Vec2f orePosition = pos - Vec2f(side, side) / 2.0f;

			auto item = game->GetItemDatabase()->Make(mOreBundle.Drop(), mLocation);
			item->GetNode()->SetLocalPosition(orePosition);
			mLocation->AddItem(item);

			++oreAdded;
		}

		--maxTries;
	}
}

void LocationGenerator::AddLava(float lavaHeight)
{
	SetProgressMessage("Adding lava...");
	MaskOperation args;
	auto rndLavaTile = game->GetResourceDatabase()->GetRandom(TerrainBrushClass::Lava);
	args.SetBrushImage(rndLavaTile->mFrontTexture);
	args.mFirstPassBitwiseOperation = BitwiseOp::Or;
	args.mFirstPassMask = PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE | PA_LAVA;
	args.mSecondPassBitwiseOperation = BitwiseOp::And;
	args.mSecondPassMask = ~PA_EDITABLE;
	args.mEditorAccess = true;
	args.mDoNotAddLavaPixelsToChillList = true;
	const shared_ptr<Terrain> terrain = mLocation->GetScene()->GetTerrain();
	const float y = terrain->GetHeightPixels() - lavaHeight / 2;
	const Vec2f begin = { 0, y };
	const Vec2f end = { (float)terrain->GetWidthPixels(), y };
	terrain->DrawLine(begin, end, lavaHeight, args);
}

// Each location have a set of treasure chests with random loot
// Loot defined by its rarity and difficulty level. More rare loot
// will be placed on most far locations (from player's start location)
void LocationGenerator::PlaceTreasureChests()
{
	SetProgressMessage("Adding treasure chests...");

	const shared_ptr<LocationDefinition>& definition = mLocation->GetDefinition();

	const Difficulty difficulty = mLevel->GetDifficulty();

	// Determine item per chest range
	const float itemPerChestCoeff =
		difficulty == Difficulty::Easy ? 1.0f :
		difficulty == Difficulty::Medium ? 0.8f :
		difficulty == Difficulty::Hard ? 0.55f :
		difficulty == Difficulty::Realistic ? 0.4f
		: 1.0f;

	const Math::Range<int> itemPerChestRange =
	{
		static_cast<int>(4 * itemPerChestCoeff),
		static_cast<int>(20 * itemPerChestCoeff)
	};

	// Determine chest count range
	const float chestCountCoeff =
		difficulty == Difficulty::Easy ? 1.0f :
		difficulty == Difficulty::Medium ? 0.9f :
		difficulty == Difficulty::Hard ? 0.8f :
		difficulty == Difficulty::Realistic ? 0.7f
		: 1.0f;

	const int baseChestCount = static_cast<int>(0.111f * definition->GetWidth() * definition->GetHeight());

	const Math::Range<int> chestCountRange =
	{
		static_cast<int>(baseChestCount * chestCountCoeff),
		static_cast<int>(0.5f * baseChestCount * chestCountCoeff)
	};

	// Determine money range
	const int baseMoneyCount = 150;

	const float moneyCountCoeff =
		difficulty == Difficulty::Easy ? 1.5f :
		difficulty == Difficulty::Medium ? 1.0f :
		difficulty == Difficulty::Hard ? 0.7f :
		difficulty == Difficulty::Realistic ? 0.5f
		: 1.0f;

	const Math::Range<int> moneyCountRange
	{
		static_cast<int>(baseMoneyCount * moneyCountCoeff),
		static_cast<int>(0.5f * baseMoneyCount * moneyCountCoeff)
	};

	const int maxRarity = std::min(20 + ItemDefinition::MaxRarity * (definition->GetBaseLevel() / mLevel->GetMaxBaseLevel()), ItemDefinition::MaxRarity);

	ItemDatabase* const db = game->GetItemDatabase();

	// Create bundles of item classes filtered by max rarity
	const Bundle<Bundle<ItemType>> bundles =
	{
		{ 0.2f, db->MakeBundleOfClassByRarity(ItemClass::Rubbish, maxRarity) },
		{ 0.8f, db->MakeBundleOfClassByRarity(ItemClass::Metal, maxRarity) },
		{ 0.15f, db->MakeBundleOfClassByRarity(ItemClass::Weapon, maxRarity) },
		{ 0.3f, db->MakeBundleOfClassByRarity(ItemClass::Projectile, maxRarity) },
		{ 0.8f, db->MakeBundleOfClassByRarity(ItemClass::Potion, maxRarity) },
		{ 0.8f, db->MakeBundleOfClassByRarity(ItemClass::Crystal, maxRarity) },
		{ 0.2f, db->MakeBundleOfClassByRarity(ItemClass::Blueprint, maxRarity) },
		{ 0.2f, db->MakeBundleOfClassByRarity(ItemClass::Armor, maxRarity) },
		{ 0.2f, db->MakeBundleOfClassByRarity(ItemClass::Helmet, maxRarity) },
		{ 0.2f, db->MakeBundleOfClassByRarity(ItemClass::Trousers, maxRarity) },
		{ 0.2f, db->MakeBundleOfClassByRarity(ItemClass::Gloves, maxRarity) },
		{ 0.1f, db->MakeBundleOfClassByRarity(ItemClass::Ring, maxRarity) },
		{ 0.1f, db->MakeBundleOfClassByRarity(ItemClass::Shield, maxRarity) },
		{ 0.1f, db->MakeBundleOfClassByRarity(ItemClass::Necklace, maxRarity) },
		{ 1.0f, db->MakeBundleOfClassByRarity(ItemClass::Ingredient, maxRarity) },
		{ 0.09f, db->MakeBundleOfClassByRarity(ItemClass::Toolset, maxRarity) },
		{ 0.09f, db->MakeBundleOfClassByRarity(ItemClass::Book, maxRarity) }
	};

	const shared_ptr<Terrain> terrain = mLocation->GetScene()->GetTerrain();

	const int currentChestCount = chestCountRange.RandomValue();
	for (int i = 0; i < currentChestCount; ++i)
	{
		shared_ptr<Chest> chest;
		
		// Find place for a chest
		const int maxTries = 100;
		for (int j = 0; j < maxTries; ++j)
		{
			sf::FloatRect bounds;
			bounds.width = Math::Rnd(40.0f, 70.0f);
			bounds.height = Math::Rnd(47.0f, 70.0f);
			bounds.left = Math::Rnd(0.0f, terrain->GetWidthPixels() - bounds.width);
			bounds.top = Math::Rnd(0.0f, terrain->GetHeightPixels() - bounds.height);

			float borderSize = 14;
			sf::FloatRect bloated;
			bloated.left = bounds.left - borderSize;
			bloated.top = bounds.top - borderSize;
			bloated.width = bounds.width + 2.0f * borderSize;
			bloated.height = bounds.height + 2.0f * borderSize;

			vector<CellTraceResult> out;
			terrain->GatherPixelsInsideRect({ bloated.left, bloated.top }, { bloated.width, bloated.height }, 1, 1, out, PA_VISIBLE | PA_EDITABLE);
			if (out.size() >= static_cast<size_t>(bloated.width * bloated.height * 0.95f))
			{
				// Cut room
				MaskOperation args;
				args.mPosition = { bounds.left, bounds.top };
				args.mRectSize = { bounds.width, bounds.height };
				args.mBounds = Bounds::Rect;
				args.mFirstPassBitwiseOperation = BitwiseOp::And;
				args.mFirstPassMask = ~(PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE);
				terrain->Mask(args);

				// Add walls
				args.mPosition.x -= 10;
				args.mPosition.y -= 10;
				args.mRectSize.x += 20;
				args.mRectSize.y += 20;
				args.mFirstPassBitwiseOperation = BitwiseOp::And;
				args.mFirstPassMask = 0xFFFF;
				args.SetBrushImage(game->GetResourceDatabase()->GetRandom(TerrainBrushClass::Stone)->mFrontTexture);
				terrain->Mask(args);

				// Create chest
				chest = make_shared<Chest>(mLocation->GetScene());				
				chest->SetPosition({ bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2 });
				mLocation->AddChest(chest);

				// Proper place was found, leave 'try' loop
				break;
			}
		}

		// Add chest loot
		if (chest)
		{			
			const shared_ptr<Inventory> inventory = chest->GetInventory();
			inventory->AddMoney(moneyCountRange.RandomValue());
			const int chestItemCount = itemPerChestRange.RandomValue();
			for (int k = 0; k < chestItemCount; )
			{
				// Get a bundle for a random class
				const Bundle<ItemType>& bundle = bundles.Drop();

				if (!bundle.IsEmpty())
				{
					// Drop random item from class
					const ItemType itemType = bundle.Drop();

					inventory->AddItem(db->Make(itemType, mLocation));

					++k;
				}
			}
		}
	}
}

const string & LocationGenerator::GetProgressMessage() const
{
	return mProgressMessage;
}

void LocationGenerator::SetProgressMessage(const string & message)
{
	mProgressMessage = message;
}

void LocationGenerator::MarkAsDone()
{
	mDone = true;
}

bool LocationGenerator::IsDone() const
{
	return mDone;
}

const shared_ptr<LocationDefinition> LocationGenerator::GetDefinition() const
{
	return mDefinition;
}

void LocationGenerator::AddBorders(bool addCap)
{
	SetProgressMessage("Adding borders...");

	auto terrain = mLocation->GetScene()->GetTerrain();

	const float w = static_cast<float>(terrain->GetWidthPixels());
	const float h = static_cast<float>(terrain->GetHeightPixels());
	const float y = terrain->GetTopBackgroundlessPartHeight();

	auto DrawBorderLine = [this] (const Vec2f & begin, const Vec2f & end)
	{
		const float lineWidth = 40;
		MaskOperation args;
		args.mFirstPassBitwiseOperation = BitwiseOp::Or;
		args.mFirstPassMask = PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE;
		args.mSecondPassBitwiseOperation = BitwiseOp::And;
		args.mSecondPassMask = ~(PA_EDITABLE | PA_ACID | PA_LAVA);
		mLocation->GetScene()->GetTerrain()->DrawLine(begin, end, lineWidth, args);
	};

	if (addCap)
	{
		// Top border
		DrawBorderLine({ 0, y }, { w, y });
	}
	// Bottom border
	DrawBorderLine({ 0, h }, { w, h });
	// Right border
	DrawBorderLine({ w, y }, { w, h });
	// Left border
	DrawBorderLine({ 0, y }, { 0, h });
}

shared_ptr<Sprite> LocationGenerator::AddSpriteDecoration(const Vec2f & position, const Vec2f & size, const shared_ptr<Texture> & texture)
{
	auto sprite = mLocation->GetScene()->CreateSprite();
	sprite->SetTexture(texture);
	sprite->SetLocalPosition(position);
	sprite->SetSize(size);
	return sprite;
}

shared_ptr<Sprite> LocationGenerator::AddSpriteDecoration(const sf::FloatRect & bounds, const shared_ptr<Texture> & texture)
{
	return AddSpriteDecoration({ bounds.left, bounds.top }, { bounds.width, bounds.height }, texture);
}

void LocationGenerator::AddTerrainDecoration(const Vec2f & position, const Vec2f & size, const shared_ptr<Texture> & texture)
{
	MaskOperation args;
	args.mEditorAccess = true;
	args.mBounds = Bounds::Rect;
	args.mFirstPassBitwiseOperation = BitwiseOp::Or;
	args.mFirstPassMask = PA_VISIBLE;
	args.mPosition = position;
	args.mRectSize = size;
	args.mBrushMode = BrushMode::Insert;
	args.mDoBitmaskOnlyOnVisiblePixelsOfReplaceImage = true;
	args.SetBrushImage(texture);
	mLocation->GetScene()->GetTerrain()->Mask(args);
}

LocationGeneratorModule::~LocationGeneratorModule()
{

}

LocationGeneratorModule::LocationGeneratorModule(LocationGenerator * parentGenerator) : mParentGenerator(parentGenerator)
{

}