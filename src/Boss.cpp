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
#include "Boss.h"
#include "Level.h"
#include "Terrain.h"
#include "Dialog.h"
#include "Remains.h"
#include "Animation.h"
#include "AnimationTrack.h"
#include "HUD.h"
#include "FogGate.h"
#include "Stats.h"
#include "Arena.h"

void Boss::Think()
{
	HUD* hud = game->GetHUD();

	if (mHostile)
	{
		if (!IsDead())
		{
			hud->SetBossFightModeEnabled(true);
			hud->SetBossName(mName);
			hud->SetBossHealth(mStats->mHealth, mStats->mMaxHealth.Get());
		}

		OnThink();
	}
	else
	{
		hud->SetBossFightModeEnabled(false);
		GetBody()->SetHorizontalVelocity(0);
		
		OnIdle();
	}	
}

Boss::Boss()
{
}

Boss::Boss(const shared_ptr<Arena>& arena, float bodyRadius)
	: Actor(arena->GetLocation(), bodyRadius)
	, mArena(arena)
{
	mHealthBar->SetVisible(false);
	mHealSign->SetVisible(false);
}

Boss::~Boss()
{

}

void Boss::Serialize(Serializer &sav)
{
	Actor::Serialize(sav);
	sav & mArena;	
	sav & mHostile;
}

void Boss::OnDie()
{
	if (mInventory)
	{
		GetLocation()->AddRemains(make_shared<Remains>(GetLocation()->GetScene(), GetPosition(), mInventory));
	}
	
	game->GetHUD()->SetBossFightModeEnabled(false);
}

void Boss::SetHostile(bool state)
{
	mHostile = state;
}

bool Boss::IsHostile() const
{
	return mHostile;
}

