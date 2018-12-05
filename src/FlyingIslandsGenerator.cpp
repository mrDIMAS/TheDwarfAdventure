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
#include "FlyingIslandsGenerator.h"
#include "Terrain.h"
#include "PerlinNoise.h"
#include "Location.h"
#include "Scene.h"
#include "LocationMap.h"
#include "WorldGenerator.h"
#include "ResourceDatabase.h"
#include "Renderer.h"
#include "Island.h"

void FlyingInslandsGenerator::ReadConfig()
{
	rapidjson::Document doc;
	LoadJSON("data/generators/flying_island_generator.json", doc);
	auto root = doc.GetObject();

	if (root.HasMember("islandSideSubpointsCount"))
	{
		auto array = root["islandSideSubpointsCount"].GetArray();
		islandSideSubpointsCount.min = array[0].GetInt();
		islandSideSubpointsCount.max = array[1].GetInt();
	}
	if (root.HasMember("xSpaceBetweenIslands"))
	{
		auto array = root["xSpaceBetweenIslands"].GetArray();
		xSpaceBetweenIslands.min = array[0].GetFloat();
		xSpaceBetweenIslands.max = array[1].GetFloat();
	}
	if (root.HasMember("ySpaceBetweenIslands"))
	{
		auto array = root["ySpaceBetweenIslands"].GetArray();
		ySpaceBetweenIslands.min = array[0].GetFloat();
		ySpaceBetweenIslands.max = array[1].GetFloat();
	}
	if (root.HasMember("islandYOffsetRange"))
	{
		auto array = root["islandYOffsetRange"].GetArray();
		islandYOffsetRange.min = array[0].GetFloat();
		islandYOffsetRange.max = array[1].GetFloat();
	}
	if (root.HasMember("islandHeightRange"))
	{
		auto array = root["islandHeightRange"].GetArray();
		islandHeightRange.min = array[0].GetFloat();
		islandHeightRange.max = array[1].GetFloat();
	}
	if (root.HasMember("islandWidthRange"))
	{
		auto array = root["islandWidthRange"].GetArray();
		islandWidthRange.min = array[0].GetFloat();
		islandWidthRange.max = array[1].GetFloat();
	}
	if (root.HasMember("borderSize"))
	{
		mBorderSize = root["borderSize"].GetFloat();
	}
}

void FlyingInslandsGenerator::GenerateStructure()
{
	SetProgressMessage("Generating islands...");

	const auto scene = mLocation->GetScene();
	const auto terrain = scene->GetTerrain();
	const int width = terrain->GetWidthPixels();
	const int height = terrain->GetHeightPixels();

	float fieldWidth = (width - 2 * mBorderSize);
	float fieldHeight = (height - 2 * mBorderSize);

	int rowCount = static_cast<int>((fieldHeight / islandHeightRange.max) - 1);
	int colCount = static_cast<int>(fieldWidth / islandWidthRange.max);

	float xStep = islandWidthRange.max + xSpaceBetweenIslands.RandomValue();
	float yStep = islandHeightRange.max + ySpaceBetweenIslands.RandomValue();

	for (int row = 0; row < rowCount; ++row)
	{
		shared_ptr<Island> prev;
		for (int col = 0; col <= colCount; ++col)
		{
			const shared_ptr<Island> newIsland = make_shared<Island>();

			// a-b is top of the flying island
			newIsland->mBaseTriangle.mPoints[0].x = col * xStep;
			newIsland->mBaseTriangle.mPoints[0].y = row * yStep + islandYOffsetRange.RandomValue();

			newIsland->mBaseTriangle.mPoints[1].x = newIsland->mBaseTriangle.mPoints[0].x + islandWidthRange.RandomValue();
			newIsland->mBaseTriangle.mPoints[1].y = newIsland->mBaseTriangle.mPoints[0].y;

			// where c defines triangle "height"
			newIsland->mBaseTriangle.mPoints[2].x = Math::Rnd(newIsland->mBaseTriangle.mPoints[0].x, newIsland->mBaseTriangle.mPoints[1].x);
			newIsland->mBaseTriangle.mPoints[2].y = newIsland->mBaseTriangle.mPoints[1].y + islandHeightRange.RandomValue();

			// Generate island polygon based on triangle
			Math::Edge edges[] = {
				{ newIsland->mBaseTriangle.mPoints[0], newIsland->mBaseTriangle.mPoints[1] },
				{ newIsland->mBaseTriangle.mPoints[1], newIsland->mBaseTriangle.mPoints[2] },
				{ newIsland->mBaseTriangle.mPoints[2], newIsland->mBaseTriangle.mPoints[0] },
			};

			int n = 0;
			for (auto & edge : edges)
			{
				Math::Range<float> offset;
				if (n == 0)
				{
					// top side should be as similar as plane so offset here is small
					offset = { -20.0f, 30.0f };
				}
				else
				{
					offset = { -10.0, 60.0f };
				}


				int subPointsCount = islandSideSubpointsCount.RandomValue();

				const float inv = 1.0f / subPointsCount;
				const Vec2f dir = edge.mB - edge.mA;
				const Vec2f step = dir * inv;
				const Vec2f normal = Math::Normalized(Math::Perpendicular(dir));

				if (n == 0)
				{
					newIsland->mTopSide.push_back(edge.mA);
				}
				newIsland->mPolygon.AddPoint(edge.mA);
				for (int i = 1; i < subPointsCount; ++i)
				{
					// Make point on edge
					Vec2f newPt = edge.mA + static_cast<float>(i) * step;

					// Push it away from edge by normal 
					newPt += normal * offset.RandomValue();

					newIsland->mPolygon.AddPoint(newPt);

					if (n == 0)
					{
						newIsland->mTopSide.push_back(newPt);
					}
				}
				if (n == 0)
				{
					newIsland->mTopSide.push_back(edge.mB);
				}
				newIsland->mPolygon.AddPoint(edge.mB);

				++n;
			}

			mLocation->AddIsland(newIsland);

			// link 
			if (prev)
			{
				newIsland->mLinks.push_back(prev);
			}

			prev = newIsland;
		}
	}
}

bool FlyingInslandsGenerator::IsPointInsideAnyIsland(const Vec2f & p) const
{
	for (const shared_ptr<Island> & island : mLocation->GetIslandList())
	{
		if (island->mPolygon.Contains(p))
		{
			return true;
		}
	}

	return false;
}

void FlyingInslandsGenerator::ClearTerrain()
{
	SetProgressMessage("Clearing terrain...");

	const auto terrain = mLocation->GetScene()->GetTerrain();

	// Clear terrain
	const int width = terrain->GetWidthPixels();
	const int height = terrain->GetHeightPixels();
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			// Make each pixel invisible\non-collidable\non-raytraceable
			if (auto pixel = terrain->GetPixel(x, y))
			{
				pixel->a &= ~(PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE);
			}

			Vec2f position;
			position.x = static_cast<float>(x);
			position.y = static_cast<float>(y);

			terrain->RevealFogOfWarAt(position);

			/*
			if (!IsPointInsideAnyIsland(position))
			{
				// Make transparent background 
				if (auto backgroundPixel = terrain->GetBackgroundMaskPixel(x, y))
				{
					*backgroundPixel = 0;
				}
			}*/
			*terrain->GetBackgroundMaskPixel(x, y) = 0;
		}
	}
}

void FlyingInslandsGenerator::RasterizeIslands()
{
	SetProgressMessage("Rasterizing islands...");

	const shared_ptr<Terrain> terrain = mLocation->GetScene()->GetTerrain();

	MaskOperation args;
	args.mBounds = Bounds::Polygon;
	args.mFirstPassBitwiseOperation = BitwiseOp::Or;
	args.mFirstPassMask = PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE;
	args.mEditorAccess = true;
	args.mBackgroundValue = 1;
	args.mEditBackground = true;

	for (const shared_ptr<Island> & island : mLocation->GetIslandList())
	{
		args.SetPolygon(island->mPolygon);

		terrain->Mask(args);

		mLocation->GetMap()->DrawPolygon(island->mPolygon, { 128, 128, 128 });
	}
}

void FlyingInslandsGenerator::RasterizeLadders()
{
	SetProgressMessage("Rasterizing islands...");

	const auto terrain = mLocation->GetScene()->GetTerrain();

	MaskOperation args;
	args.mFirstPassBitwiseOperation = BitwiseOp::Or;
	args.mFirstPassMask = PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE | PA_METAL;
	args.mEditorAccess = true;
	auto metalBrush = game->GetResourceDatabase()->GetRandom(TerrainBrushClass::Metal);
	args.SetBrushImage(metalBrush->mFrontTexture);

	for (const shared_ptr<Island> & island : mLocation->GetIslandList())
	{
		for (const weak_ptr<Island> & weakLink : island->mLinks)
		{
			const shared_ptr<Island> link = weakLink.lock();

			Math::Edge islandTop = { island->mBaseTriangle.mPoints[0], island->mBaseTriangle.mPoints[1] };
			Math::Edge linkTop = { link->mBaseTriangle.mPoints[0], link->mBaseTriangle.mPoints[1] };

			Vec2f begin = islandTop.GetClosestPointTo(link->mPolygon.Center());
			Vec2f end = linkTop.GetClosestPointTo(island->mPolygon.Center());

			terrain->DrawLine(begin, end, 8, args);
		}
	}
}

void FlyingInslandsGenerator::RasterizeIslandGrass()
{
	SetProgressMessage("Rasterizing grass...");

	const auto terrain = mLocation->GetScene()->GetTerrain();

	MaskOperation args;
	args.mFirstPassBitwiseOperation = BitwiseOp::Or;
	args.mFirstPassMask = PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE;
	args.mEditorAccess = true;
	auto grassBrush = game->GetResourceDatabase()->GetRandom(TerrainBrushClass::Grass);
	args.SetBrushImage(grassBrush->mFrontTexture);

	for (auto & island : mLocation->GetIslandList())
	{
		for (size_t i = 1; i < island->mTopSide.size(); ++i)
		{
			terrain->DrawLine(island->mTopSide[i - 1], island->mTopSide[i], 16, args);
		}
	}
}

shared_ptr<Island> FlyingInslandsGenerator::GetRandomIsland() const
{
	return Math::RandomElement(mLocation->GetIslandList());
}

FlyingInslandsGenerator::FlyingInslandsGenerator(Level * level, const shared_ptr<LocationDefinition>& params) :
	LocationGenerator(level, params)
{
	ReadConfig();
}

FlyingInslandsGenerator::~FlyingInslandsGenerator()
{

}

shared_ptr<Location> FlyingInslandsGenerator::Generate()
{
	CreateBlankLocation();
	CreateRandomTerrain();

	const auto scene = mLocation->GetScene();
	scene->SetAmbientColor({ 180, 180, 180 });
	const auto terrain = scene->GetTerrain();
	GenerateStructure();
	ClearTerrain();
	RasterizeIslands();
	terrain->Smooth();
	RasterizeLadders();
	RasterizeIslandGrass();
	Renderer* renderer = game->GetRenderer();
	vector<shared_ptr<Texture>> layers = {
		renderer->RequestTexture("data/textures/backs/sunny/layer0.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer1.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer2.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer3.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer4.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer5.png")
	};
	CreateParallaxBackground(true, layers, false);
	CreateClouds(20, static_cast<float>(terrain->GetHeightPixels()));

	PlaceTreasureChests();

	return mLocation;
}
