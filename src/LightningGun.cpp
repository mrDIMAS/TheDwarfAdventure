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
#include "LightningGun.h"
#include "Scene.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Actor.h"
#include "Terrain.h"
#include "PolyLine.h"
#include "LiquidParticle.h"
#include "Level.h"
#include "Inventory.h"

void LightningGun::OnUpdate()
{
	mVoltaicArc->SetVisible(false);
	mVoltaicArcCenter->SetVisible(false);
}

void LightningGun::OnSerialize(Serializer &sav)
{
	sav &mVoltaicArc;
	sav &mVoltaicArcCenter;
}

LightningGun::LightningGun()
{
}

LightningGun::LightningGun(const shared_ptr<Location> &location) : Weapon(location, ItemType::Weapon_LightningGun)
{
	const auto scene = location->GetScene();

	auto renderer = game->GetRenderer();

	mModel = scene->CreateSprite();
	mModel->AttachTo(mNode);
	mModel->SetTexture(renderer->RequestTexture("data/textures/weapons/lightning_gun.png"));
	mModel->SetSize(30, 12);
	mModel->SetLocalOrigin(5, 6);

	mShootPoint = { 29, 6 };

	mVoltaicArc = scene->CreatePolyLine();
	mVoltaicArc->SetColor({ 111, 211, 255, 200 });
	mVoltaicArc->SetThickness(5.0f);

	mVoltaicArcCenter = scene->CreatePolyLine();
	mVoltaicArcCenter->SetColor({ 185, 234, 255, 200 });
	mVoltaicArcCenter->SetThickness(2.5f);

	mShowCrosshair = false;
}

LightningGun::~LightningGun()
{
}

bool LightningGun::Shoot(const Vec2f &direction)
{
	THROW_IF_EXPIRED(mOwner);
	auto owner = mOwner.lock();
	auto location = owner->GetLocation();
	auto scene = mModel->GetScene();
	auto terrain = scene->GetTerrain();
	auto &desc = GetDefinition();
	const auto origin = GetShootPointGlobal();
	const auto rayDir = Math::Normalized(direction) * desc.mShootDistance;
	scene->AcceptNode(mVoltaicArc);
	scene->AcceptNode(mVoltaicArcCenter);
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
			// reduce durability of consumable
			consumable->WearOut(0.25f);
			// reduce durability of this weapon
			this->WearOut(0.1f);
			// pick closest actor by ray
			Vec2f intPos;
			float actorSqrDist;
			shared_ptr<Actor> closestActor = PickClosestActorByRay(origin, rayDir, &intPos, &actorSqrDist);
			// and trace ray with terrain
			auto result = terrain->TraceRay(origin, rayDir);
			Vec2f closestPt = origin + rayDir;
			// if actor closer than terrain
			if (actorSqrDist < Math::SqrDistance(origin, result.mPosition))
			{
				// if we have picked actor - damage it
				if (closestActor)
				{
					// make sure that we close enough
					if (actorSqrDist <= Math::Sqr(desc.mShootDistance))
					{
						closestPt = intPos;
						Damage dmg;
						dmg.mWho = owner;
						dmg.mCount = GetModifiedValue(ItemModifierType::Damage);
						dmg.mClass = desc.mDamageClass;
						dmg.mFlags = (DamageType)desc.mDamageType;
						closestActor->ApplyDamage(dmg);
						location->AddLiquidParticle(make_shared<LiquidParticle>(location, LiquidParticle::Type::Blood, intPos, direction));
					}
				}
			}
			else
			{
				if (result.mIntersection && result.mCell)
				{
					closestPt = result.mPosition;
				}
			}
			// draw voltaic arc
			const float segmentLength = 7;
			int segmentCount = static_cast<int>(Math::Distance(origin, closestPt) / segmentLength);
			const Vec2f dSegment = (closestPt - origin) / static_cast<float>(segmentCount);
			mVoltaicArc->Clear();
			mVoltaicArcCenter->Clear();
			Vec2f ptPos = origin;
			for (int i = 0; i < segmentCount; ++i)
			{
				const Vec2f newPt =
					Vec2f(ptPos.x + Math::Rnd(-2.0f, 2.0f), ptPos.y + Math::Rnd(-2.0f, 2.0f));
				mVoltaicArc->AddPoint(newPt);
				mVoltaicArcCenter->AddPoint(newPt);
				ptPos += dSegment;
			}
			// add last point at closest point
			mVoltaicArc->AddPoint(closestPt);
			mVoltaicArcCenter->AddPoint(closestPt);
			mVoltaicArc->SetVisible(true);
			mVoltaicArcCenter->SetVisible(true);
		}
	}
	else
	{
		success = false;
	}
	return true;
}
