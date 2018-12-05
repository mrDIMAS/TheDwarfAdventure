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
#include "RocketLauncher.h"
#include "Projectile.h"
#include "Level.h"
#include "Renderer.h"
#include "Actor.h"
#include "Inventory.h"

void RocketLauncher::OnUpdate()
{
}

void RocketLauncher::OnSerialize(Serializer &sav)
{
	UNUSED(sav);
}

RocketLauncher::RocketLauncher()
{
}

RocketLauncher::RocketLauncher(const shared_ptr<Location> &location) : Weapon(location, ItemType::Weapon_RocketLauncher)
{
	const auto scene = location->GetScene();

	mModel = scene->CreateSprite();
	mModel->AttachTo(mNode);
	mModel->SetTexture(
		game->GetRenderer()->RequestTexture("data/textures/weapons/rocketLauncher.png"));
	mModel->SetSize(29, 10);
	mModel->SetLocalOrigin(0, 5);

	mShootPoint = { 15, 5 };
}

RocketLauncher::~RocketLauncher()
{
	DBG_ECHO();
}

bool RocketLauncher::Shoot(const Vec2f &direction)
{
	THROW_IF_EXPIRED(mOwner);
	auto owner = mOwner.lock();
	auto location = owner->GetLocation();
	auto scene = mModel->GetScene();
	bool success = true;
	if (mShootTimeout <= 0)
	{
		if (auto consumable = ExtractConsumable(ItemType::Projectile_Rocket))
		{
			this->WearOut(1.0f);
			location->AddProjectile(make_shared<Projectile>(location, GetOwner(), ItemType::Projectile_Rocket,
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
