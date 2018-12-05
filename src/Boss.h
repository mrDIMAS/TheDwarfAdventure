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

#pragma once

#include "Actor.h"
#include "BehaviourTree.h"

class FogGate;
class Arena;

#define BOSS_TYPE_ENUM_ENTRIES(X) \
	X(Golem, "Golem") \
	X(Crystal, "Crystal") \
	X(Beast, "Beast") \

ENUM_DECLARE(BossType, BOSS_TYPE_ENUM_ENTRIES)

#undef BOSS_TYPE_ENUM_ENTRIES

// boss can fight only with player by design
class Boss : public Actor
{
public:
	Boss();
	Boss(const shared_ptr<Arena> &arena, float bodyRadius);
	~Boss();
	virtual void Serialize(Serializer &sav) override;
	virtual void OnDie() override final;
	void SetHostile(bool state);
	bool IsHostile() const;
protected:
	virtual void Think() override final;
	virtual void OnThink() = 0;
	virtual void OnIdle() = 0;
	shared_ptr<bt::Node> mBehaviour;
	weak_ptr<Arena> mArena;
	bool mHostile { false };
};
