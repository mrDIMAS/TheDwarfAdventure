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
#include "HubGenerator.h"
#include "Renderer.h"
#include "Cell.h"
#include "Location.h"
#include "Scene.h"
#include "Terrain.h"
#include "Passage.h"
#include "PassageGenerator.h"
#include "CaveGenerator.h"
#include "ResourceDatabase.h"
#include "FogGate.h"

void HubGenerator::CutTerrainTop()
{
	const shared_ptr<Terrain> terrain = mLocation->GetScene()->GetTerrain();

	const float lineWidth = terrain->GetTopBackgroundlessPartHeight();
	const auto begin = Vec2f(0, lineWidth);
	const auto end = Vec2f(static_cast<float>(terrain->GetWidthPixels()), lineWidth);

	MaskOperation args;
	args.mBounds = Bounds::Circle;
	args.mFirstPassBitwiseOperation = BitwiseOp::And;
	args.mFirstPassMask = ~(PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE | PA_LAVA | PA_ACID);
	args.mRadius = lineWidth;
	args.mPosition = { 0, 0 };
	Vec2f d = end - begin;

	const int steps = static_cast<int>(Math::Length(d) / (lineWidth * 0.5f));
	d /= (float)steps;
	for (int i = 0; i < steps; i++)
	{
		args.mPosition += d;
		terrain->Mask(args);

		// reveal fog of war at top of the terrain
		vector<CellTraceResult> result;
		terrain->GatherPixelsInsideCircle(args.mPosition, lineWidth * 1.1f, 20, 20, result, 0);
		for (auto &pix : result)
		{
			terrain->RevealFogOfWarAt(pix.mPosition);
		}
	}
}

void HubGenerator::Erase(const Vec2f & begin, const Vec2f & end, float brushWidth)
{
	MaskOperation args;
	args.mFirstPassBitwiseOperation = BitwiseOp::And;
	args.mFirstPassMask = ~(PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE | PA_ACID | PA_LAVA);
	mLocation->GetScene()->GetTerrain()->DrawLine(begin, end, brushWidth, args);
}

void HubGenerator::DrawIndestructibleCrystalLine(const shared_ptr<Texture> & crystalTex, const Vec2f & begin, const Vec2f & end, float brushWidth)
{
	MaskOperation args;
	args.SetBrushImage(crystalTex);
	args.mFirstPassBitwiseOperation = BitwiseOp::Or;
	args.mFirstPassMask = PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE;
	args.mSecondPassBitwiseOperation = BitwiseOp::And;
	args.mSecondPassMask = ~(PA_EDITABLE | PA_LAVA | PA_ACID);
	mLocation->GetScene()->GetTerrain()->DrawLine(begin, end, brushWidth, args);
}

HubGenerator::HubGenerator(Level * level, const shared_ptr<LocationDefinition>& params)
	: LocationGenerator(level, params)
{

}

void HubGenerator::PlaceTrees(const shared_ptr<Location> & location, int count)
{
	auto scene = location->GetScene();
	auto terrain = scene->GetTerrain();
	const float xStep = static_cast<float>(terrain->GetWidthPixels() / count);
	for (int i = 0; i < count; ++i)
	{
		auto tex = game->GetRenderer()->RequestTexture("data/textures/objects/pine.png");
		auto tree = scene->CreateSprite();
		tree->SetTexture(tex);
		tree->SetLocalPosition(i * Math::Rnd(0.9f, 1.1f) * xStep,
			terrain->GetTopBackgroundlessPartHeight() - 0.96f * tex->GetHeight());
		tree->SetSize(Vec2f(sf::Vector2i(tex->GetWidth(), tex->GetHeight())));
	}
}

shared_ptr<Location> HubGenerator::Generate()
{
	static constexpr float LayerHeightCells = 10.0f;

	float layerWidth = static_cast<float>(Cell::Size * mDefinition->GetWidth());
	float layerHeight = LayerHeightCells * Cell::Size;
	float lavaAreaHeight = 0.1f * Cell::Size;
	float mLayerSeparatorHeight = 25;
	float bossAreaWidth = 1000;
	float bossAreaHeight = 300;
	int cloudCount = 100;
	int treeCount = mDefinition->GetWidth();
	int layerCount = 2;

	CreateBlankLocation();
	CreateRandomTerrain();
	AddBorders(false);

	auto renderer = game->GetRenderer();

	vector<shared_ptr<Texture>> backgroundLayers = {
		renderer->RequestTexture("data/textures/backs/sunny/layer0.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer1.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer2.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer3.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer4.png"),
		renderer->RequestTexture("data/textures/backs/sunny/layer5.png")
	};
	CreateParallaxBackground(false, backgroundLayers, false);

	CutTerrainTop();

	const shared_ptr<Terrain> terrain = mLocation->GetScene()->GetTerrain();

	// pass 0: generate structure
	for (int layerNum = 0; layerNum < layerCount; ++layerNum)
	{
		PassageGenerator passageGenerator(this);

		sf::FloatRect passageArea;
		passageArea.left = 0;
		passageArea.top = terrain->GetTopBackgroundlessPartHeight() + layerNum * layerHeight;
		passageArea.width = layerWidth;
		passageArea.height = layerHeight - mLayerSeparatorHeight - terrain->GetTopBackgroundlessPartHeight();

		passageGenerator.SetArea(passageArea);

		sf::FloatRect mSeparator;
		mSeparator.left = passageArea.left;
		mSeparator.top = passageArea.top + passageArea.height;
		mSeparator.width = passageArea.width;
		mSeparator.height = mLayerSeparatorHeight;

		sf::FloatRect caveArea;
		caveArea.width = passageArea.width * 0.5f;
		caveArea.height = passageArea.height * 0.5f;
		caveArea.left = Math::Rnd(passageArea.left, passageArea.left + passageArea.width - caveArea.width);
		caveArea.top = Math::Rnd(passageArea.top, passageArea.top + passageArea.height - caveArea.height);

		sf::FloatRect mBossArea;
		mBossArea.left = caveArea.left + caveArea.width / 2 - bossAreaWidth / 2;
		mBossArea.top = caveArea.top + caveArea.height / 2 - bossAreaHeight / 2;
		mBossArea.width = bossAreaWidth;
		mBossArea.height = bossAreaHeight;

		// set forbidden zones
		passageGenerator.AddForbiddenArea(mBossArea);
		passageGenerator.AddForbiddenArea(caveArea);
		passageGenerator.Generate();

		// build caves first
		/*
		CaveGenerator mCaveGenerator(this);
		mCaveGenerator.mArea = caveArea;
		mCaveGenerator.mPointCount = (int)(caveArea.width * caveArea.height * 0.00005f);
		mCaveGenerator.Generate();	

		auto boss = GenerateBoss(location, layer.mBossArea);
		*/
		PlaceTreasureChests();

		const float nextLevelEntranceWidth = 200;
		const float separatorHeight = 30;
		const Vec2f beginA = { mSeparator.left, mSeparator.top };
		const Vec2f endA = {
			mSeparator.left + mSeparator.width / 2 - nextLevelEntranceWidth / 2,
			mSeparator.top
		};
		const Vec2f endB = {
			mSeparator.left + mSeparator.width,
			mSeparator.top
		};

		const TerrainBrush* randomCrystalBrush = game->GetResourceDatabase()->GetRandom(TerrainBrushClass::Crystal);
		DrawIndestructibleCrystalLine(randomCrystalBrush->mFrontTexture, beginA, endB, separatorHeight);
		const sf::FloatRect fogGateBounds = sf::FloatRect(endA - Vec2f(0, separatorHeight), { nextLevelEntranceWidth, separatorHeight * 2.1 });
		const shared_ptr<FogGate> fogGate = make_shared<FogGate>(mLocation, fogGateBounds, FogType::BossBlock);
		mLocation->AddFogGate(fogGate);
	}

	PlaceTrees(mLocation, treeCount);

	AddLava(lavaAreaHeight);

	CreateClouds(cloudCount, terrain->GetTopBackgroundlessPartHeight());

	//PlaceWaterInPassages(location, v->mPassageGen->GetPassageList());
	PlaceOre({ { 0, 0 }, mLocation->GetScene()->GetTerrain()->GetSizePixels() });

	return mLocation;
}
