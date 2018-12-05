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
#include "BossAreaGenerator.h"
#include "ResourceDatabase.h"
#include "Game.h"
#include "PixelAttribute.h"
#include "Terrain.h"
#include "LocationGenerator.h"
#include "Boss.h"
#include "Bundle.h"
#include "BossCrystal.h"
#include "BossGolem.h"
#include "FogGate.h"
#include "LocationMap.h"
#include "Arena.h"
#include "BossBeast.h"

BossAreaGenerator::BossAreaGenerator(LocationGenerator* parentGenerator, const sf::FloatRect & bounds) 
	: LocationGeneratorModule(parentGenerator)
	, mBounds(bounds)
{

}

void BossAreaGenerator::Generate()
{
	const shared_ptr<Location> location = mParentGenerator->GetLocation();
	const shared_ptr<Terrain> terrain = location->GetScene()->GetTerrain();

	ResourceDatabase* resourceDatabase = game->GetResourceDatabase();

	const TerrainBrush* backgroundBrush = resourceDatabase->GetRandom(TerrainBrushClass::Rock);
	const TerrainBrush* boundsBrush = resourceDatabase->GetRandom(TerrainBrushClass::Crystal);

	// clean area first
	{
		MaskOperation args;
		args.mEditorAccess = true;
		args.SetBounds(mBounds);
		args.mFirstPassBitwiseOperation = BitwiseOp::And;
		args.mFirstPassMask = ~(PA_VISIBLE | PA_ACID | PA_LAVA | PA_COLLIDABLE | PA_RAYTRACEABLE);
		args.mEditBackground = true;
		args.mBackgroundValue = backgroundBrush->mStackId;
		terrain->Mask(args);
	}

	const float boundsThickness = 30;
	
	const shared_ptr<Arena> arena = make_shared<Arena>(location);
	location->AddArena(arena);

	Math::Polygon areaBounds;
	areaBounds.AddPoint({ mBounds.left, mBounds.top });
	areaBounds.AddPoint({ mBounds.left + mBounds.width, mBounds.top });
	areaBounds.AddPoint({ mBounds.left + mBounds.width, mBounds.top + mBounds.height });
	areaBounds.AddPoint({ mBounds.left, mBounds.top + mBounds.height });

	arena->SetFightArea(areaBounds);

	mParentGenerator->CreateCrystalLight(areaBounds.Center(), 200);

	// draw borders
	{
		MaskOperation args;
		args.mFirstPassBitwiseOperation = BitwiseOp::Or;
		args.mFirstPassMask = PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE;
		args.mSecondPassBitwiseOperation = BitwiseOp::And;
		args.mSecondPassMask = ~(PA_EDITABLE | PA_ACID | PA_LAVA);
		args.SetBrushImage(boundsBrush->mFrontTexture);
		terrain->OutlinePolygon(areaBounds, boundsThickness, args);
	}

	// add entrance
	const Vec2f entrancePosition = { mBounds.left - boundsThickness * 0.5f, mBounds.top + mBounds.height / 2 };
	const Vec2f entranceSize = { boundsThickness, mBounds.height / 2 };;
	{
		MaskOperation args;
		args.mEditorAccess = true;
		args.mBounds = Bounds::Rect;
		args.mPosition = entrancePosition;
		args.mRectSize = entranceSize;
		args.mFirstPassBitwiseOperation = BitwiseOp::And;
		args.mFirstPassMask = ~(PA_VISIBLE | PA_ACID | PA_LAVA);
		terrain->Mask(args);
	}

	// add boss
	Bundle<BossType> possibleBosses = {
		{ 0.33f, BossType::Golem },
		{ 0.33f, BossType::Crystal },
		{ 1000.33f, BossType::Beast }
	};

	BossType bossType = possibleBosses.Drop();
	shared_ptr<Boss> boss;
	switch (bossType)
	{
	case BossType::Golem:
		boss = make_shared<BossGolem>(arena);
		break;
	case BossType::Crystal:
		boss = make_shared<BossCrystal>(arena);
		break;
	case BossType::Beast:
		boss = make_shared<BossBeast>(arena);
		break;
	}	
	boss->SetPosition(areaBounds.Center());
	location->AddActor(boss);
	
	// add fog gate entrance	
	const shared_ptr<FogGate> fogGate = make_shared<FogGate>(location, sf::FloatRect(entrancePosition, entranceSize), FogType::BossEntrance);
	location->AddFogGate(fogGate);

	arena->AddFogGate(fogGate);	
	arena->AddBoss(boss);

	location->GetMap()->DrawRect({ Math::RectPosition(mBounds), Math::RectSize(mBounds) }, LocationMap::PassageColor);
}
