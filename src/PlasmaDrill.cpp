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
#include "PlasmaDrill.h"
#include "Level.h"
#include "Renderer.h"
#include "Actor.h"
#include "Inventory.h"
#include "Terrain.h"
#include "Terrain.h"
#include "LiquidParticle.h"
#include "Sound.h"

void PlasmaDrill::OnUpdate()
{
	mPlasmaBeam->SetVisible(false);
	mPlasmaHalo->SetVisible(false);
	if (auto snd = mLoopSound.lock())
	{
		snd->Pause();
	}
}

void PlasmaDrill::OnSerialize(Serializer &sav)
{
	sav &mPlasmaBeam;
	sav &mPlasmaHalo;
}

void PlasmaDrill::OnSetVisible(bool vis)
{
	UNUSED(vis);
	mPlasmaBeam->SetVisible(false);
	mPlasmaHalo->SetVisible(false);
}

PlasmaDrill::PlasmaDrill()
{
}

PlasmaDrill::PlasmaDrill(const shared_ptr<Location> &location) : Weapon(location, ItemType::Weapon_PlasmaDrill)
{
	const auto scene = location->GetScene();

	// model
	mModel = scene->CreateSprite();
	mModel->AttachTo(mNode);
	mModel->SetTexture(game->GetRenderer()->RequestTexture("data/textures/weapons/plasmaDrill.png"));
	mModel->SetSize(30, 15);
	mModel->SetLocalOrigin(5, 7.5);
	mModel->SetName("Model");
	// plasma beam
	mPlasmaBeam = scene->CreateSprite();
	mPlasmaBeam->SetTexture(
		game->GetRenderer()->RequestTexture("data/textures/weapons/plasmaBeam.png"));
	mPlasmaBeam->SetSize(64, 8);
	mPlasmaBeam->SetRenderFlags(RF_NO_LIGHTING);
	mPlasmaBeam->SetLocalOrigin(0, 4);
	mPlasmaBeam->SetLocalPosition(24, 6);
	mPlasmaBeam->SetVisible(false);
	mPlasmaBeam->SetName("PlasmaBeam");
	// plasma halo
	mPlasmaHalo = scene->CreateSprite();
	mPlasmaHalo->SetTexture(
		game->GetRenderer()->RequestTexture("data/textures/weapons/plasmaHalo.png"));
	mPlasmaHalo->SetSize(16, 40);
	mPlasmaHalo->SetRenderFlags(RF_NO_LIGHTING);
	mPlasmaHalo->SetLocalOrigin(9, 16);
	mPlasmaHalo->SetVisible(false);
	mPlasmaHalo->SetName("PlasmaHalo");
	// other
	mShootPoint = { 24, 6 };
}

PlasmaDrill::~PlasmaDrill()
{
	DBG_ECHO();
}

bool PlasmaDrill::Shoot(const Vec2f &direction)
{
	THROW_IF_EXPIRED(mOwner);
	auto owner = mOwner.lock();
	auto location = owner->GetLocation();
	auto scene = mModel->GetScene();
	auto terrain = scene->GetTerrain();
	auto &desc = GetDefinition();
	// get prepared weapon consumable and check it
	auto holdingHand = owner->GetHoldingHand(shared_from_this());
	auto consumable = owner->GetHandEquipmentConsumable(holdingHand);
	if (!consumable || consumable->GetType() != ItemType::Crystal_SmallPower)
	{
		// replace consumable to correct type
		owner->UseItem(owner->GetInventory()->ExtractConsumable(ItemType::Crystal_SmallPower), holdingHand);
	}
	bool success = true;
	if (consumable)
	{
		if (consumable->IsBroken())
		{
			success = false;
			// extract new crystal and place it as weapon consumable
			owner->UseItem(owner->GetInventory()->ExtractConsumable(ItemType::Crystal_SmallPower), holdingHand);
		}
		else
		{
			if (mLoopSound.expired())
			{
				mLoopSound = scene->EmitSound("data/sounds/plasma_beam_loop.ogg", mModel);
			}
			else
			{
				mLoopSound.lock()->Play();
			}
			// reduce durability of consumable
			consumable->WearOut(0.1f);
			// reduce durability of this weapon
			this->WearOut(0.1f);
			// adjust beam
			mPlasmaBeam->SetVisible(true);
			mPlasmaBeam->SetSize(desc.mShootDistance, 8);
			// since beam and halo does not attached to weapon model, we have to
			// transfer it from scene to scene manually
			scene->AcceptNode(mPlasmaBeam);
			scene->AcceptNode(mPlasmaHalo);
			const auto origin = GetShootPointGlobal();
			const auto rayDir = direction * desc.mShootDistance;
			const float beamAngle = Math::CartesianToPolar(direction).mAngle;
			mPlasmaBeam->SetLocalRotation(beamAngle);
			mPlasmaBeam->SetLocalPosition(origin);
			// pick closest actor by ray
			Vec2f intPos;
			float actorSqrDist;
			shared_ptr<Actor> closestActor = PickClosestActorByRay(origin, rayDir, &intPos, &actorSqrDist);
			// and trace ray with terrain
			auto result = terrain->TraceRay(origin, rayDir);
			// if actor closer than terrain
			if (actorSqrDist < Math::SqrDistance(origin, result.mPosition))
			{
				// if we have picked actor - damage it
				if (closestActor)
				{
					// make sure that we close enough
					if (actorSqrDist <= Math::Sqr(desc.mShootDistance))
					{
						// adjust beam length
						mPlasmaBeam->SetSize(Math::Length(intPos - mPlasmaBeam->GetGlobalPosition()), 8);
						// put some blood
						location->AddLiquidParticle(make_shared<LiquidParticle>(location, LiquidParticle::Type::Blood, intPos, direction));
						Damage dmg;
						dmg.mWho = owner;
						dmg.mCount = GetModifiedValue(ItemModifierType::Damage);
						dmg.mClass = desc.mDamageClass;
						dmg.mFlags = (DamageType)desc.mDamageType;
						closestActor->ApplyDamage(dmg);
					}
				}
			}
			else
			{
				// otherwise cut off terrain piece
				if (result.mIntersection && result.mCell)
				{
					const float radius = 22;
					terrain->MeltCicle(result.mPosition - direction * (radius - 1), radius, sf::Color(255, 0, 0));
					// orient plasma halo
					mPlasmaHalo->SetVisible(true);
					mPlasmaHalo->SetLocalPosition(result.mPosition);
					mPlasmaHalo->SetLocalRotation(beamAngle);
					// adjust beam length
					mPlasmaBeam->SetSize(
						Math::Length(result.mPosition - mPlasmaBeam->GetGlobalPosition()), 8);
					// check for unstable zones
					const Vec2f analysisZoneSize = { 128, 128 };
					location->AnalyzeAndBreakUnstableZones(result.mPosition - analysisZoneSize * 0.5f, analysisZoneSize);
				}
			}
		}
	}
	else
	{
		success = false;
	}
	return success;
}
