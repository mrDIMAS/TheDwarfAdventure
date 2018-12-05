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
#include "RevealGun.h"
#include "Node.h"
#include "Scene.h"
#include "Sprite.h"
#include "Actor.h"
#include "Inventory.h"
#include "Terrain.h"
#include "Light.h"
#include "Renderer.h"
#include "Location.h"

void RevealGun::OnUpdate()
{
	mProbeLight->SetVisible(mShootTimeout > 0);
	if (mShootTimeout > 0)
	{
		auto scene = mModel->GetScene();
		auto terrain = scene->GetTerrain();
		const float revealRadius = 100.0f;
		vector<CellTraceResult> result;
		terrain->GatherPixelsInsideCircle(mProbePosition, revealRadius, 20, 20, result, 0);
		for (auto &pix : result)
		{
			terrain->RevealFogOfWarAt(pix.mPosition, 5 * 60);
		}
		mProbePosition += mProbeVelocity;
		mProbeLight->SetRadius(revealRadius);
		mProbeLight->SetLocalPosition(mProbePosition);
	}
}

void RevealGun::OnSerialize(Serializer &sav)
{
	sav &mProbePosition;
	sav &mProbeVelocity;
	sav &mProbeLight;
}

RevealGun::RevealGun()
{
}

RevealGun::RevealGun(const shared_ptr<Location> &location) : Weapon(location, ItemType::Weapon_RevealGun)
{
	const auto scene = location->GetScene();

	mModel = scene->CreateSprite();
	mModel->AttachTo(mNode);
	mModel->SetTexture(game->GetRenderer()->RequestTexture("data/textures/weapons/reveal_gun.png"));
	mModel->SetSize(24, 12);
	mModel->SetLocalOrigin(2, 6);

	mProbeLight = scene->CreateLight();
	mProbeLight->SetColor({ 0, 255, 175 });

	mShowCrosshair = false;
}

RevealGun::~RevealGun()
{
}

bool RevealGun::Shoot(const Vec2f &direction)
{
	THROW_IF_EXPIRED(mOwner);
	auto owner = mOwner.lock();
	auto location = owner->GetLocation();
	auto scene = mModel->GetScene();
	auto terrain = scene->GetTerrain();

	scene->AcceptNode(mProbeLight);

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
		if (mShootTimeout <= 0)
		{
			if (consumable->IsBroken())
			{
				success = false;
				// extract new crystal and place it as weapon consumable
				owner->UseItem(owner->GetInventory()->ExtractConsumable(ItemType::Crystal_SmallPower), holdingHand);
			}
			else
			{
				consumable->WearOut(1.0f);
				mProbePosition = mModel->GetGlobalPosition();
				mProbeVelocity = Math::Normalized(direction) * 20.0f;
				mShootTimeout = GetDefinition().mShootPeriod;
			}
		}
	}
	else
	{
		success = false;
	}
	return success;
}
