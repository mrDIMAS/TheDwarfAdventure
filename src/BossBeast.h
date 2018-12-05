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

class BossBeast : public Boss
{
public:
	BossBeast();
	BossBeast(const shared_ptr<Arena>& arena);
	virtual void Serialize(Serializer &sav) override final;
private:
	static constexpr float MeleeAttackDistance = 80;
	static constexpr float JumpAttackDistance = 200;
	void CreateTimeEvents();
	shared_ptr<Sprite> mAttackHitboxSprite;
	shared_ptr<Animation> mWalkAnimation;
	Vec2f mTargetPosition;
	virtual void OnThink() override;
	virtual void OnIdle() override;
	// Behaviour tree methods
	bool IsTargetFarEnoughForJumpAttack() const;
	bt::Status SetPlayerAsTarget();
	bool IsOnGround() const;
	bt::Status JumpToTarget() const;
	bool IsTargetCloseEnoughForMeleeAttack() const;
	bool IsNeedToGoCloser() const;
	bt::Status DoMeleeAttack();
	bt::Status LookAtTarget() const;
	bt::Status MoveToTarget();
};