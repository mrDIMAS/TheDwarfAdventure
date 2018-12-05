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

#include "Boss.h"

class BossCrystal : public Boss
{
private:
	void CreateTimeEvents();
	shared_ptr<Animation> mMeleeAttackAnimation;
	shared_ptr<Animation> mWhirlAttackAnimation;
	bt::WaitAndDoNode::State mWhirlWaitState;
	bt::WaitNode::State mWhirlPointChangeState;
	shared_ptr<Sprite> mAttackHitboxSprite;
	Vec2f mTargetPosition;
	virtual void OnThink() override;
	virtual void OnIdle() override;

	void DoPhysicalAttack(float damage);
	void MeleeAttack();
	// Behaviour nodes
	bt::Status WhirlAttack();
	bt::Status SetPlayerAsTarget();
	bool IsTargetCloseEnough() const;
	bt::Status LookAtTarget() const;
	bt::Status SetVeryFastMoveSpeed();
	bt::Status SetOrdinarySpeed();
	bt::Status MoveToTarget();
	bt::Status DoMeleeAttack();
	bt::Status SelectRandomTarget();
public:
	BossCrystal();
	BossCrystal(const shared_ptr<Arena>& arena);
	virtual void Serialize(Serializer &sav) override final;
};