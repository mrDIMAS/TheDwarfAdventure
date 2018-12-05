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
#include "BossBeast.h"
#include "Location.h"
#include "Stats.h"
#include "Arena.h"
#include "Player.h"
#include "Level.h"

void BossBeast::CreateTimeEvents()
{
	mBehaviour = bt::Builder()
		.Parallel("Attack")
		    .Do(BTFUNC(SetPlayerAsTarget))
		    .Do(BTFUNC(LookAtTarget))
			.Selector("Fight")		
				.Sequence("Melee Attack")	         
					.Condition(BTFUNC(IsTargetCloseEnoughForMeleeAttack))
					.Do(BTFUNC(DoMeleeAttack))		
				.End()
				.Sequence("Walk closer")
					.Condition(BTFUNC(IsNeedToGoCloser))
					.Do(BTFUNC(MoveToTarget))
				.End()
				.Sequence("Jump Attack")
					.Condition(BTFUNC(IsTargetFarEnoughForJumpAttack))
					.Condition(BTFUNC(IsOnGround))
					.Do(BTFUNC(JumpToTarget))		        
				.End()
			.End()
		.End()
		.Build();
}

void BossBeast::OnThink()
{
	mBehaviour->Tick();
}

void BossBeast::OnIdle()
{

}

bool BossBeast::IsTargetFarEnoughForJumpAttack() const
{
	return Math::Distance(mTargetPosition, GetPosition()) > 150;
}

bt::Status BossBeast::SetPlayerAsTarget()
{
	mTargetPosition = game->GetLevel()->GetPlayer()->GetPosition();
	return bt::Status::Success;
}

bool BossBeast::IsOnGround() const
{
	return GetBody()->IsOnGround();
}

bt::Status BossBeast::JumpToTarget() const
{
	const Vec2f dirVec = mTargetPosition - GetPosition();
	const float length = Math::Length(dirVec);
	Vec2f dir = dirVec / length;
	dir.y = -3.55f;
	GetBody()->SetVelocity(dir * (length / FIXED_FPS));
	return bt::Status::Success;
}

bool BossBeast::IsTargetCloseEnoughForMeleeAttack() const
{
	return Math::Distance(mTargetPosition, GetPosition()) <= MeleeAttackDistance;
}

bool BossBeast::IsNeedToGoCloser() const
{
	const float distance = Math::Distance(mTargetPosition, GetPosition());

	return distance > MeleeAttackDistance && distance < JumpAttackDistance;
}

bt::Status BossBeast::DoMeleeAttack()
{
	return bt::Status::Success;
}

bt::Status BossBeast::LookAtTarget() const
{
	float xDelta = mTargetPosition.x - GetPosition().x;
	mModelRoot->SetHorizontalScale(xDelta > 0 ? -1.0f : 1.0f);
	return bt::Status::Success;
}

bt::Status BossBeast::MoveToTarget()
{
	Vec2f direction = Math::Normalized(mTargetPosition - GetPosition());
	GetBody()->SetVelocity(direction * mStats->mMoveSpeed.Get());
	SetAnimation(mWalkAnimation);
	return bt::Status::Success;
}

BossBeast::BossBeast()
{
	
}

BossBeast::BossBeast(const shared_ptr<Arena>& arena) 
	: Boss(arena, 45)
{
	const shared_ptr<Location> location = arena->GetLocation();

	const shared_ptr<Scene> prefab = game->LoadPrefab("data/prefabs/beast.prefab");

	mModelRoot = prefab->FindStrict<SceneNode>("Root");

	mAttackHitboxSprite = prefab->FindStrict<Sprite>("AttackHitbox");

	mAnimations = prefab->GetAnimationList();

	mWalkAnimation = prefab->FindAnimationStrict("Walk");

	mModelRoot->AttachTo(mPivot);
	mModelRoot->SetLocalPosition({ 0, 0 });

	location->GetScene()->Merge(prefab);

	// Manually set stats, resists must go last because they are affected by main stats
	// (any changes of these will recalculate resists)
	mStats->Vitality.Set(120);
	mStats->Dexterity.Set(5);
	mStats->mFireDamageResist.Set(0.4f);
	mStats->mPhysicalDamageResist.Set(0.2f);
	mStats->mChemicalDamageResist.Set(0.4f);

	mName = game->GetStr("bossBeast");

	CreateTimeEvents();
}

void BossBeast::Serialize(Serializer & sav)
{
	Boss::Serialize(sav);

	if (sav.IsLoading())
	{
		CreateTimeEvents();
	}
}
