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
#include "Torch.h"
#include "Scene.h"
#include "Light.h"
#include "ItemDatabase.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Actor.h"
#include "Level.h"
#include "Smoke.h"
#include "Stats.h"

void Torch::OnUpdate()
{
	if (IsBroken())
	{
		mLight->SetRadius(0.0f);
	}
	else
	{
		mLight->SetRadius(180.0f);
	}

	WearOut(0.02f);

	if (mSmokeCastingInterval <= 0)
	{
		auto owner = mOwner.lock();
		auto location = owner->GetLocation();
		location->AddSmoke(make_shared<Smoke>(location->GetScene(), GetShootPointGlobal()));
		mSmokeCastingInterval = 30;
	}
	--mSmokeCastingInterval;
}

void Torch::OnSerialize(Serializer &sav)
{
	UNUSED(sav);
}

void Torch::OnSetVisible(bool vis)
{
	mLight->SetVisible(vis);
}

Torch::Torch()
{
}

Torch::Torch(const shared_ptr<Location> &location) : Weapon(location, ItemType::Weapon_Torch)
{
	const auto scene = location->GetScene();

	mModel = scene->CreateSprite();
	mModel->SetTexture(game->GetRenderer()->RequestTexture("data/textures/objects/animated_torch.png"));
	mModel->AttachTo(mNode);
	mModel->SetSize(12, 24);
	mModel->SetLocalOrigin({ 6, 24 });
	mModel->GetAnimation().LoadFromFile("data/animations/torch.fra");
	mModel->GetAnimation().SetSequence("burn");
	mModel->GetAnimation().SetEnabled(true);

	mShootPoint = { 6, 20 };

	mLight = scene->CreateLight();
	mLight->AttachTo(mNode);

	mShowCrosshair = false;
}

Torch::~Torch()
{
}

bool Torch::Shoot(const Vec2f &direction)
{
	UNUSED(direction);
	if (IsBroken())
	{
		return false;
	}

	auto owner = mOwner.lock();
	auto location = owner->GetLocation();
	auto scene = mModel->GetScene();

	const Vec2f shootPt = GetShootPointGlobal();
	const auto &desc = GetDefinition();

	// hit actors
	for (auto &actor : location->GetActorList())
	{
		if (actor.get() != owner.get())
		{
			if (actor->IsAnyHitBoxIntersected(shootPt, 10))
			{
				Damage dmg;
				dmg.mWho = owner;
				dmg.mCount = desc.mDamage * owner->GetStats()->GetMeleeWeaponDamageScaleCoefficient();
				dmg.mClass = desc.mDamageClass;
				dmg.mFlags = (DamageType)desc.mDamageType;
				actor->ApplyDamage(dmg);
				for (int i = 0; i < 4; ++i)
				{
					location->AddSmoke(make_shared<Smoke>(scene, shootPt + Vec2f(Math::Rnd(-7.0f, 7.0f), Math::Rnd(-7.0f, 7.0f))));
				}
				WearOut(0.5);
			}
		}
	}

	return true;
}

shared_ptr<Texture> Torch::GetPictogram()
{
	return GetDefinition().mTexture;
}
