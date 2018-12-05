

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
#include "GrenadeLauncher.h"
#include "Projectile.h"
#include "Actor.h"
#include "Level.h"
#include "Renderer.h"
#include "Inventory.h"
#include "Terrain.h"

void GrenadeLauncher::OnUpdate()
{
}

void GrenadeLauncher::OnSerialize(Serializer &sav)
{
	UNUSED(sav);
}

GrenadeLauncher::GrenadeLauncher()
{
}

GrenadeLauncher::GrenadeLauncher(const shared_ptr<Location> &location) : Weapon(location, ItemType::Weapon_GrenadeLauncher)
{
	mModel = location->GetScene()->CreateSprite();
	mModel->AttachTo(mNode);
	mModel->SetTexture(
		game->GetRenderer()->RequestTexture("data/textures/weapons/grenadeLauncher.png"));
	mModel->SetSize(30, 15);
	mModel->SetLocalOrigin(5, 7.5);

	mShootPoint = { 15, 7.5 };
}

GrenadeLauncher::~GrenadeLauncher()
{
	DBG_ECHO();
}

bool GrenadeLauncher::Shoot(const Vec2f &direction)
{
	THROW_IF_EXPIRED(mOwner);
	auto owner = mOwner.lock();
	auto location = owner->GetLocation();
	auto scene = location->GetScene();
	auto terrain = location->GetScene()->GetTerrain();
	const auto center = mNode->GetGlobalPosition();
	Vec2f projSpawnPos = GetShootPointGlobal();
	// trace ray along shooting direction, and find if it intersects with terrain
	// and corrent grenade spawn position
	const auto rt = terrain->TraceRay(center, projSpawnPos - center);
	if (rt.mIntersection)
	{
		projSpawnPos = rt.mPosition - direction * 10.0f;
	}
	bool success = true;
	if (mShootTimeout <= 0)
	{
		if (auto consumable = ExtractConsumable(ItemType::Projectile_Grenade))
		{
			this->WearOut(1.0f);
			location->AddProjectile(make_shared<Projectile>(location, GetOwner(), ItemType::Projectile_Grenade, projSpawnPos,
				direction * 25.0f, GetModifier(ItemModifierType::Damage)));
			mShootTimeout = GetDefinition().mShootPeriod;
		}
		else
		{
			success = false;
		}
	}
	return success;
}
