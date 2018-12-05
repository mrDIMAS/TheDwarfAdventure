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
#include "Arena.h"
#include "Boss.h"
#include "Level.h"
#include "Player.h"
#include "FogGate.h"

Arena::Arena()
{

}

Arena::Arena(const shared_ptr<Location>& location) : mLocation(location)
{

}

void Arena::AddFogGate(const shared_ptr<FogGate>& fogGate)
{
	mControlledFogGates.push_back(fogGate);
}

void Arena::SetFightArea(const Math::Polygon & area)
{
	mFightArea = area;
}

void Arena::AddBoss(const shared_ptr<Boss>& boss)
{
	mBosses.push_back(boss);
}

void Arena::Update()
{
	const shared_ptr<Player> player = game->GetLevel()->GetPlayer();
	
	int aliveBosses = 0;
	for (const weak_ptr<Boss>& weakBossPtr : mBosses)
	{
		const shared_ptr<Boss>& boss = weakBossPtr.lock();

		if (boss && !boss->IsDead())
		{
			// Boss will be hostile only if player is inside an arena
			boss->SetHostile(mFightArea.Contains(player->GetPosition()));

			++aliveBosses;
		}
	}
	
	// When all bosses are dead we have to reveal all fog gates and do a cleanup
	if (aliveBosses == 0)
	{
		const shared_ptr<Location> location = GetLocation();

		for (const weak_ptr<FogGate> & weakFogGatePtr : mControlledFogGates)
		{
			const shared_ptr<FogGate> fogGate = weakFogGatePtr.lock();

			if (fogGate)
			{
				fogGate->Reveal();

				location->RemoveFogGate(fogGate);
			}
		}

		mControlledFogGates.clear();
		mBosses.clear();
	}
}

void Arena::Serialize(Serializer & sav)
{
	sav & mControlledFogGates;
	sav & mFightArea;
	sav & mLocation;
	sav & mBosses;
}

shared_ptr<Location> Arena::GetLocation() const
{
	return mLocation.lock();
}

bool Arena::Contains(const Vec2f & pt) const
{
	return mFightArea.Contains(pt);
}
