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
#include "Drill.h"
#include "Actor.h"
#include "Level.h"
#include "Renderer.h"
#include "Inventory.h"
#include "Terrain.h"
#include "LiquidParticle.h"
#include "Sound.h"
#include "Debris.h"
#include "VisualEffect.h"

void Drill::OnUpdate()
{
	if (auto snd = mDrillSound.lock())
	{
		snd->Pause();
	}
}

void Drill::OnSerialize(Serializer &sav)
{
	UNUSED(sav);
}

Drill::Drill()
{
}

Drill::Drill(const shared_ptr<Location> &location) : Weapon(location, ItemType::Weapon_Drill)
{
	mModel = location->GetScene()->CreateSprite();
	mModel->AttachTo(mNode);
	mModel->SetTexture(game->GetRenderer()->RequestTexture("data/textures/weapons/drill.png"));
	mModel->SetSize(30, 15);
	mModel->SetLocalOrigin(5, 7.5);

	mShowCrosshair = false;
}

Drill::~Drill()
{
	DBG_ECHO();
}

bool Drill::Shoot(const Vec2f &direction)
{
	THROW_IF_EXPIRED(mOwner);
	auto owner = mOwner.lock();
	auto location = owner->GetLocation();
	auto scene = mModel->GetScene();
	auto &desc = GetDefinition();

	const auto origin = mModel->GetGlobalPosition() + mModel->GetLocalOrigin();
	const auto rayDir = direction * desc.mShootDistance;

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
			consumable->WearOut(0.025f);

			this->WearOut(0.05f);

			if (mDrillSound.expired())
			{
				mDrillSound = scene->EmitSound("data/sounds/drill.ogg", mModel);
			}
			else
			{
				mDrillSound.lock()->Play();
			}

			// pick actor by ray
			Vec2f intPos;
			float actorSqrDistance;
			shared_ptr<Actor> closestActor = PickClosestActorByRay(origin, rayDir, &intPos, &actorSqrDistance);

			auto terrain = scene->GetTerrain();

			// and trace ray with terrain
			auto result = terrain->TraceRay(origin, rayDir);

			// if actor closer than terrain
			if (actorSqrDistance < Math::SqrDistance(origin, result.mPosition))
			{
				// if we have picked actor - damage it
				if (closestActor)
				{
					if (actorSqrDistance <= Math::Sqr(desc.mShootDistance))
					{
						// add some blood
						location->AddLiquidParticle(make_shared<LiquidParticle>(location, LiquidParticle::Type::Blood, intPos, 2.5f * Math::Normalized(direction)));

						//HitBox hitbox;
						//hitbox.SetRectange({ GetShootPointGlobal(), { 20, 20 });

						Damage dmg;
						dmg.mWho = mOwner.lock();
						dmg.mCount = GetModifiedValue(ItemModifierType::Damage);
						dmg.mClass = desc.mDamageClass;
						dmg.mFlags = (DamageType)desc.mDamageType;
						closestActor->ApplyDamage(dmg);
					}
				}
			}
			else
			{
				// otherwise, retrace ray with terrain and cut piece off it
				if (result.mIntersection && result.mCell)
				{
					Vec2i dPix;
					dPix.x = static_cast<int>(Debris::Radius * Math::Sign(rayDir.x));
					dPix.y = static_cast<int>(Debris::Radius * Math::Sign(rayDir.y));

					if (result.mPixel.a & PA_METAL)
					{
						VisualEffect::MakeSparks(scene, result.mPosition);
					}
					else
					{
						if ((result.mPixel.a & PA_VISIBLE) && (result.mPixel.a & PA_COLLIDABLE) &&
							(result.mPixel.a & PA_EDITABLE))
						{
							for (int i = 0; i < 8; ++i)
							{
								AddDebrisInIntersection(result.mCell, result.mPixelCoord + dPix, result.mPosition);
							}
						}
					}

					terrain->DrillCicle(result.mPosition, 10);

					const Vec2f zoneSize = { 128, 128 };
					location->AnalyzeAndBreakUnstableZones(result.mPosition - zoneSize * 0.5f, zoneSize);
				}
			}
			return true;
		}
	}
	else
	{
		success = false;
	}
	return success;
}
