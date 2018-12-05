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

class BossGolem final : public Boss
{
protected:
	Math::Range<float> mMeleeAttackRange { 0, 100 };
	Math::Range<float> mDistantAttackRange { 100, 400 };
	shared_ptr<Animation> mStandAnimation;
	shared_ptr<Animation> mWalkAnimation;
	shared_ptr<Animation> mMeleeAttackAnimation;
	shared_ptr<Animation> mThrowFireballAnimation;
	shared_ptr<Animation> mFallAttackAnimation;
	shared_ptr<Animation> mBlastAnimation;
	shared_ptr<Animation> mDeathAnimation;
	bt::WaitNode::State mBlastWaitState;
	Vec2f mTargetPosition;
	shared_ptr<SceneNode> mLeftLowerArm;
	shared_ptr<SceneNode> mFireballThrowPos;
	void CreateTimeEvents();
	void ThrowFireball();
	void HandsHit();
	void DoBlast();

	virtual void OnThink() override;
	virtual void OnIdle() override;

	// Behaviour nodes
	bt::Status BlastAttack();
	bt::Status ThrowFireballAttack();
	bt::Status MeleeAttack();
	bool CanDoMeleeAttack() const;
	bool CanDoDistantAttack() const;
	bool BlastAttackEnded() const;
	bt::Status LookAtTarget() const;
	bt::Status SetPlayerAsTarget();
	bt::Status MoveToTarget();
public:
	BossGolem();
	BossGolem(const shared_ptr<Arena>& arena);
	~BossGolem();
	virtual void Serialize(Serializer &sav) override final;
};
