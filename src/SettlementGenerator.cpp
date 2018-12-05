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
#include "SettlementGenerator.h"
#include "Location.h"
#include "Terrain.h"
#include "Scene.h"
#include "WorldGenerator.h"
#include "Renderer.h"
#include "Door.h"
#include "ResourceDatabase.h"
#include "LocationMap.h"
#include "Ladder.h"
#include "Room.h"
#include "Light.h"
#include "NPC.h"
#include "HouseGenerator.h"
#include "WarehouseGenerator.h"

class GuardTowerGenerator
{
private:
	float mPillarWidth { 15 };
	Math::Range<int> mPillarCountRange { 2, 4 };
public:
	void Generate(const sf::FloatRect & mBounds, LocationGenerator* parentGenerator)
	{
		UNUSED(parentGenerator);
		UNUSED(mBounds);

		/*
		auto pillarTemplate = game->GetResourceDatabase()->GetRandom(DecorationTemplateType::Pillar);

		const float xStep = (mBounds.width - mPillarWidth) / mPillarCountRange.RandomValue();
		for (float x = 0; x < mBounds.width; x += xStep)
		{
			wg::Decoration pillar;
			pillar.mPosition.x = mBounds.left + x;
			pillar.mPosition.y = mBounds.top + mBounds.height;
			pillar.mSize.x = mPillarWidth;
			pillar.mTextureId = pillarTemplate->mTexture;		
		}
		*/
	}
};

void SettlementGenerator::FreeHalfOfTerrain()
{
	const shared_ptr<Terrain> terrain = mLocation->GetScene()->GetTerrain();

	// Free half of terrain
	MaskOperation args;
	args.mBounds = Bounds::Rect;
	args.mPosition = { 0, 0 };
	args.mRectSize = { static_cast<float>(terrain->GetWidthPixels()), mCenterY };
	args.mFirstPassBitwiseOperation = BitwiseOp::And;
	args.mFirstPassMask = ~(PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE);
	args.mBackgroundValue = 0;
	args.mEditBackground = true;
	args.mEditorAccess = true;
	args.mPixelModifyCallback = [terr = terrain.get()](const Vec2f & p) { terr->RevealFogOfWarAt(p); };
	terrain->Mask(args);
}

void SettlementGenerator::MakeWavyGround()
{
	const shared_ptr<Scene> scene = mLocation->GetScene();
	const shared_ptr<Terrain> terrain = scene->GetTerrain();

	const float width = static_cast<float>(terrain->GetWidthPixels());

	terrain->SetTopBackgroundlessPartHeight(mCenterY);

	// Make wavy ground
	const float maxRoughness = 15;
	const float xStep = 25;

	// Make polygon with wavy top
	Math::Polygon poly, topWavyPart;
	for (float x = 0; x < terrain->GetWidthPixels(); x += xStep)
	{
		// Make new point
		Vec2f pt = { x, Math::Rnd(mCenterY, mCenterY - maxRoughness) };
		poly.AddPoint(pt);

		// Store new points into additional polygon, to draw grass lines using it
		topWavyPart.AddPoint(pt);
	}

	// Add bottom points (horizontal line)
	poly.AddPoint({ width, mCenterY + 10 });
	poly.AddPoint({ 0, mCenterY + 10 });

	// Rasterize polygon
	MaskOperation args;
	args.SetPolygon(poly);
	args.mFirstPassMask = PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE;
	args.mFirstPassBitwiseOperation = BitwiseOp::Or;
	terrain->Mask(args);

	// Rasterize grass
	const float grassHeight = 7;
	MaskOperation grassArgs;
	grassArgs.mFirstPassBitwiseOperation = BitwiseOp::Or;
	grassArgs.mFirstPassMask = PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE;
	const TerrainBrush* grassBrush = game->GetResourceDatabase()->GetRandom(TerrainBrushClass::Grass);
	grassArgs.SetBrushImage(grassBrush->mFrontTexture);
	for (int i = 1; i < topWavyPart.GetPointCount(); ++i)
	{
		const vector<Vec2f> & pts = topWavyPart.GetPoints();
		terrain->DrawLine(pts[i - 1], pts[i], grassHeight, grassArgs);
	}
}

void SettlementGenerator::GenerateHouses()
{
	const shared_ptr<Scene> scene = mLocation->GetScene();
	const shared_ptr<Terrain> terrain = scene->GetTerrain();

	HouseGenerator houseGen(this);

	Math::Range<float> houseWidthRange = { 200, 350 };
	Math::Range<float> houseHeightRange = { 20, 50 };

	const float width = static_cast<float>(terrain->GetWidthPixels());
	const int houseCount = static_cast<int>(width / (3.5f * houseWidthRange.max));

	vector<sf::FloatRect> housesBoundsList;
	for (int i = 0; i < houseCount; ++i)
	{
		int maxTries = 30;
		while (maxTries-- > 0)
		{
			// Propose bounds
			sf::FloatRect bounds;

			bounds.width = houseWidthRange.RandomValue();
			bounds.left = Math::Rnd(0.0f, width - bounds.width);
			bounds.height = bounds.width + houseHeightRange.RandomValue();
			bounds.top = mCenterY - 1.5f * bounds.height;


			sf::FloatRect bloated = bounds;
			bloated.left -= 20;
			bloated.top -= 20;
			bloated.width += 40;
			bloated.height += 40;
			bool intersect = false;
			for (const sf::FloatRect & otherHouse : housesBoundsList)
			{
				if (bloated.intersects(otherHouse))
				{
					intersect = true;
					break;
				}
			}

			if (!intersect)
			{
				houseGen.SetBounds(bounds);
				houseGen.Generate();
				housesBoundsList.push_back(bounds);
			}
		}
	}
}

void SettlementGenerator::GenerateWarehouses()
{
	const shared_ptr<Scene> scene = mLocation->GetScene();
	const shared_ptr<Terrain> terrain = scene->GetTerrain();

	const float capHeight = 100;
	const Math::Range<float> warehouseWidthRange = { 200, 350 };
	const Math::Range<float> warehouseHeightRange = { 150, 200 };
	const int count = static_cast<int>(terrain->GetWidthPixels() / (3.0f * warehouseWidthRange.max));
	
	WarehouseGenerator warehouseGenerator(this);

	vector<sf::FloatRect> warehouseBoundsList;
	for (int i = 0; i < count; ++i)
	{
		// Propose bounds
		int maxTries = 40;
		sf::FloatRect bounds;
		while (--maxTries >= 0)
		{
			bounds.width = warehouseWidthRange.RandomValue();
			bounds.left = Math::Rnd(0.0f, terrain->GetSizePixels().x - bounds.width);
			bounds.height = warehouseHeightRange.RandomValue();
			bounds.top = mCenterY + capHeight;

			bool boundsOk = true;
			for (const sf::FloatRect & warehouse : warehouseBoundsList)
			{
				if (warehouse.intersects(bounds))
				{
					boundsOk = false;
					break;
				}
			}

			if (boundsOk)
			{
				break;
			}
		}

		warehouseGenerator.SetBounds(bounds);
		warehouseGenerator.SetEntranceY(mCenterY);
		warehouseGenerator.Generate();
		warehouseBoundsList.push_back(bounds);
	}
}

void SettlementGenerator::GenerateGuardTowers()
{
	const shared_ptr<Scene> scene = mLocation->GetScene();
	const shared_ptr<Terrain> terrain = scene->GetTerrain();

	const Math::Range<float> widthRange = { 200, 350 };
	const Math::Range<float> heightRange = { 150, 200 };
	const Math::Range<float> heightAboveGroundRange = { 300, 500 };
	const int count = static_cast<int>(terrain->GetWidthPixels() / (2.0f * widthRange.max));
	GuardTowerGenerator gtg;

	vector<sf::FloatRect> towerBoundsList;
	for (int i = 0; i < count; ++i)
	{
		// Propose bounds
		int maxTries = 40;
		sf::FloatRect bounds;
		while (--maxTries >= 0)
		{
			bounds.width = widthRange.RandomValue();
			bounds.left = Math::Rnd(0.0f, terrain->GetSizePixels().x - bounds.width);
			bounds.height = heightRange.RandomValue();
			bounds.top = mCenterY - heightAboveGroundRange.RandomValue();

			bool boundsOk = true;
			for (const sf::FloatRect & tower : towerBoundsList)
			{
				if (tower.intersects(bounds))
				{
					boundsOk = false;
					break;
				}
			}

			if (boundsOk)
			{
				break;
			}
		}

		// Generate warehouse
		gtg.Generate(bounds, this);
		towerBoundsList.push_back(bounds);
	}
}

void SettlementGenerator::RasterizeGuardTowers()
{
	const shared_ptr<Scene> scene = mLocation->GetScene();
	const shared_ptr<Terrain> terrain = scene->GetTerrain();


	UNUSED(scene);
	UNUSED(terrain);
}

SettlementGenerator::SettlementGenerator(Level* level, const shared_ptr<LocationDefinition>& params)
	: LocationGenerator(level, params)
{

}

SettlementGenerator::~SettlementGenerator()
{

}

shared_ptr<Location> SettlementGenerator::Generate()
{
	CreateBlankLocation();
	CreateRandomTerrain();

	const shared_ptr<Scene> scene = mLocation->GetScene();
	const shared_ptr<Terrain> terrain = scene->GetTerrain();
	terrain->SetTopBackgroundlessPartHeight(0);

	mCenterY = terrain->GetHeightPixels() / 2.0f;

	FreeHalfOfTerrain();
	terrain->Smooth();
	MakeWavyGround();
	Renderer* renderer = game->GetRenderer();

	vector<shared_ptr<Texture>> backgroundLayers = {
		renderer->RequestTexture("data/textures/backs/sunny/layer0.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer1.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer2.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer3.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer4.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer5.png")
	};

	CreateParallaxBackground(false, backgroundLayers, false);
	CreateClouds(30, mCenterY - 250);
	AddLava(20);
	GenerateHouses();
	GenerateWarehouses();

	PlaceOre({ { 0, 0 }, mLocation->GetScene()->GetTerrain()->GetSizePixels() });
	PlaceTreasureChests();

	return mLocation;
}
