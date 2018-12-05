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
#include "Pickaxe.h"
#include "Sprite.h"
#include "Scene.h"
#include "Renderer.h"
#include "Sound.h"
#include "Actor.h"
#include "Terrain.h"
#include "Level.h"
#include "DebrisAtlas.h"
#include "LiquidParticle.h"
#include "VisualEffect.h"
#include "Inventory.h"
#include "Stats.h"
#include "ResourceDatabase.h"

void Pickaxe::OnUpdate()
{
}

void Pickaxe::OnSerialize(Serializer &sav)
{
	for (auto &snd : mImpactSounds)
	{
		sav &snd;
	}
	sav &mMetalImpactSound;
	sav &mFleshImpactSound;
}

Pickaxe::Pickaxe()
{
}

Pickaxe::Pickaxe(const shared_ptr<Location> &location) : Weapon(location, ItemType::Weapon_Pickaxe)
{
	const auto scene = location->GetScene();

	auto wpnTemplate = game->GetResourceDatabase()->GetRandom(WeaponModelTemplateType::Pickaxe);
	auto modelPart = wpnTemplate->mParts[WeaponPartTemplateType::Model];

	mModel = scene->CreateSprite();
	mModel->AttachTo(mNode);
	mModel->SetTexture(modelPart.mTexture);
	mModel->SetSize(modelPart.mSize);
	mModel->SetLocalOrigin(modelPart.mOrigin);

	mShootPoint = { 32, 20 };

	mImpactSounds[0] = "data/sounds/pickaxe_impact1.ogg";
	mImpactSounds[1] = "data/sounds/pickaxe_impact2.ogg";
	mImpactSounds[2] = "data/sounds/pickaxe_impact3.ogg";

	mMetalImpactSound = "data/sounds/metal_impact1.ogg";
	mFleshImpactSound = "data/sounds/flesh_impact1.ogg";

	mShowCrosshair = false;
}

Pickaxe::~Pickaxe()
{
}

bool Pickaxe::Shoot(const Vec2f &direction)
{
	THROW_IF_EXPIRED(mOwner);
	auto owner = mOwner.lock();
	auto location = owner->GetLocation();
	auto scene = mModel->GetScene();
	auto terrain = scene->GetTerrain();
	auto level = game->GetLevel();
	auto &debrisAtlas = level->GetDebrisAtlas();

	//if (mShootTimeout > 0) {
	//  return true;
   // }

   // mShootTimeout = GetDesc().mShootPeriod;

	UNUSED(direction);

	bool hasImpact = false;
	bool metalImpact = false;
	bool fleshImpact = false;

	const float impactRadius = 12;
	const auto impactPoint = GetShootPointGlobal();

	// Create debris first
	uint8_t gatherMask = PA_VISIBLE ; // gather pixels only with this attributes
	static array<sf::Color, Cell::PixelCount> tempBuffer;
	vector<CellTraceResult> intersectedPixelsPositions;
	array<Cell *, 9> cells;
	int cellCount = terrain->GetCellsNear(impactPoint, impactRadius, cells);
	for (int i = 0; i < cellCount; ++i)
	{
		if (!cells[i])
		{
			continue;
		}

		constexpr int gatherStep = 4;
		cells[i]->TransferPixelsFromGridToLinearOrder(tempBuffer);
		intersectedPixelsPositions.clear();
		cells[i]->GatherPixelsInsideCircle(impactPoint, impactRadius, gatherStep, gatherStep, intersectedPixelsPositions, gatherMask);
		if (intersectedPixelsPositions.size())
		{
			hasImpact = true;
		}
		for (auto &rt : intersectedPixelsPositions)
		{
			if ((rt.mColor.a & PA_METAL) )
			{
				metalImpact = true;
				// do not create debris pieces of metal (cuz pickaxe cant destroy metal)
				continue;
			}

			if (rt.mColor.a & PA_EDITABLE)
			{
				sf::FloatRect texCoords;
				int pieceIndex = debrisAtlas->Pack(tempBuffer.data(), Cell::Size, Cell::Size, rt.mRelative.x, rt.mRelative.y, texCoords);
				auto debris = make_shared<Debris>(location, rt.mPosition, debrisAtlas->GetTexture(), texCoords, pieceIndex);
				location->AddDebris(debris);

				mBlockPixelCounter += 4;

				if (mBlockPixelCounter >= 200)
				{
					owner->AddItem(game->GetItemDatabase()->Make(ItemType::Projectile_ConstructionBlock, location));
					mBlockPixelCounter = 0;
				}
			}
		}
	}

	// if (hasImpact) {
	terrain->DrillCicle(impactPoint, impactRadius);
	const Vec2f zoneSize = { 64, 64 };
	location->AnalyzeAndBreakUnstableZones(impactPoint - zoneSize * 0.5f, zoneSize);
	//}

	const auto &desc = GetDefinition();

	// hit actors
	for (auto &actor : location->GetActorList())
	{
		if (actor.get() != owner.get())
		{
			if (actor->IsAnyHitBoxIntersected(impactPoint, impactRadius))
			{
				Damage dmg;
				dmg.mWho = owner;
				dmg.mCount = desc.mDamage * owner->GetStats()->GetMeleeWeaponDamageScaleCoefficient();
				dmg.mClass = desc.mDamageClass;
				dmg.mFlags = (DamageType)desc.mDamageType;
				actor->ApplyDamage(dmg);
				for (int i = 0; i < 4; ++i)
				{
					location->AddLiquidParticle(make_shared<LiquidParticle>(location, LiquidParticle::Type::Blood, impactPoint, direction));
				}
				hasImpact = true;
				fleshImpact = true;
			}
		}
	}

	if (hasImpact)
	{
		if (metalImpact)
		{
			scene->EmitSound(mMetalImpactSound, mModel);
			VisualEffect::MakeSparks(scene, impactPoint);
		}
		else if (fleshImpact)
		{
			scene->EmitSound(mFleshImpactSound, mModel);
		}
		else
		{
			scene->EmitSound(Math::RandomElement(mImpactSounds), mModel);
		}
	}
	return true;
}
