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
#include "AcidGun.h"
#include "Sprite.h"
#include "Inventory.h"
#include "Light.h"
#include "Actor.h"
#include "Projectile.h"
#include "Level.h"
#include "Renderer.h"

void AcidGun::OnUpdate()
{
}

void AcidGun::OnSerialize(Serializer &sav)
{
	UNUSED(sav);
}

AcidGun::AcidGun()
{
}

AcidGun::AcidGun(const shared_ptr<Location> &location) : Weapon(location, ItemType::Weapon_AcidGun)
{
	mModel = location->GetScene()->CreateSprite();
	mModel->AttachTo(mNode);
	mModel->SetTexture(game->GetRenderer()->RequestTexture("data/textures/weapons/acid_gun.png"));
	mModel->SetSize(24, 12);
	mModel->SetLocalOrigin(2, 6);

	mShootPoint = { 15, 5 };
}

AcidGun::~AcidGun()
{
}
 
bool AcidGun::Shoot(const Vec2f &direction)
{
	THROW_IF_EXPIRED(mOwner);
	auto owner = mOwner.lock();
	auto location = owner->GetLocation();
	bool success = true;
	if (mShootTimeout <= 0)
	{
		if (auto consumable = ExtractConsumable(ItemType::Projectile_AcidBlob))
		{
			location->AddProjectile(make_shared<Projectile>(location, GetOwner(), ItemType::Projectile_AcidBlob,
				GetShootPointGlobal(), direction * 10.0f, GetModifier(ItemModifierType::Damage)));
			mShootTimeout = GetDefinition().mShootPeriod;
		}
		else
		{
			success = false;
		}
	}
	return success;
}
