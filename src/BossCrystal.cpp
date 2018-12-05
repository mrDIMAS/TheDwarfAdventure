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
#include "BossCrystal.h"
#include "Stats.h"
#include "Location.h"
#include "Animation.h"
#include "Location.h"
#include "Level.h"
#include "Player.h"
#include "Arena.h"

void BossCrystal::CreateTimeEvents()
{
	mMeleeAttackAnimation->AddTimeEvent(TimeEvent(0.5f, [this] { MeleeAttack(); }));

	mBehaviour = bt::Builder()
		.Sequence("Attack")		    		    
		    .Condition(BTFUNC(!IsDead))
		    .Selector("SelectType")
		        .Sequence("Whirl")
		            .WaitAndDo(&mWhirlWaitState, SECONDS(16), SECONDS(8))
		            .Parallel()
		                .Sequence()
		                    .Wait(&mWhirlPointChangeState, SECONDS(1))
		                    .Do(BTFUNC(SelectRandomTarget))
		                .End()
                        .Do(BTFUNC(SetVeryFastMoveSpeed))
						.Do(BTFUNC(MoveToTarget))
						.Do(BTFUNC(WhirlAttack))
		            .End()	
		        .End()
		        .Sequence("Melee")
		            .Do(BTFUNC(SetPlayerAsTarget))
		            .Do(BTFUNC(LookAtTarget))
		            .Do(BTFUNC(SetOrdinarySpeed))
		            .Do(BTFUNC(MoveToTarget))
		            .Condition(BTFUNC(IsTargetCloseEnough))	
		            .Do(BTFUNC(DoMeleeAttack))
		        .End()
		    .End()
		.End()
		.Build();
}

BossCrystal::BossCrystal()
{

}

BossCrystal::BossCrystal(const shared_ptr<Arena>& arena) : Boss(arena, 60)
{
	const shared_ptr<Location> location = arena->GetLocation();

	const shared_ptr<Scene> prefab = game->LoadPrefab("data/prefabs/crystal.prefab");

	mModelRoot = prefab->FindStrict<SceneNode>("Root");

	mAttackHitboxSprite = prefab->FindStrict<Sprite>("AttackHitbox");

	mMeleeAttackAnimation = prefab->FindAnimationStrict("AttackMelee");
	mWhirlAttackAnimation = prefab->FindAnimationStrict("Whirl");

	mAnimations = prefab->GetAnimationList();

	mModelRoot->AttachTo(mPivot);
	mModelRoot->SetLocalPosition({ 0, 0 });

	location->GetScene()->Merge(prefab);

	// Manually set stats, resists must go last because they are affected by main stats
	// (any changes of these will recalculate resists)
	mStats->Vitality.Set(60);
	mStats->Dexterity.Set(5);
	mStats->mFireDamageResist.Set(0.9f);
	mStats->mPhysicalDamageResist.Set(0.2f);
	mStats->mChemicalDamageResist.Set(0.4f);

	mName = game->GetStr("bossCrystal");

	CreateTimeEvents();
}

void BossCrystal::OnThink()
{
	mBehaviour->Tick();
}

void BossCrystal::OnIdle()
{

}

void BossCrystal::DoPhysicalAttack(float damage)
{
	Damage dmg;
	dmg.mClass = DamageClass::Physical;
	dmg.mCount = damage;
	dmg.mWho = shared_from_this();
	dmg.mHitbox = FindHitbox(mAttackHitboxSprite);
	GetLocation()->PropagateDamage(dmg, this);
}

void BossCrystal::MeleeAttack()
{
	DoPhysicalAttack(20);
}

bt::Status BossCrystal::WhirlAttack()
{
	if (!mWhirlAttackAnimation->HasInfluence())
	{
		SetAnimation(mWhirlAttackAnimation);
	}

	DoPhysicalAttack(1);

	return bt::Status::Success;
}

bt::Status BossCrystal::SetPlayerAsTarget()
{
	mTargetPosition = game->GetLevel()->GetPlayer()->GetPosition();
	return bt::Status::Success;
}

bool BossCrystal::IsTargetCloseEnough() const
{
	return Math::Distance(mTargetPosition, GetPosition()) < 120;
}

bt::Status BossCrystal::LookAtTarget() const
{
	float xDelta = mTargetPosition.x - GetPosition().x;
	mModelRoot->SetHorizontalScale(xDelta> 0 ? 1.0f : -1.0f);
	return bt::Status::Success;
}

bt::Status BossCrystal::SetVeryFastMoveSpeed()
{
	mStats->mMoveSpeed.Set(5);
	return bt::Status::Success;
}

bt::Status BossCrystal::SetOrdinarySpeed()
{
	mStats->mMoveSpeed.Set(1);
	return bt::Status::Success;
}

bt::Status BossCrystal::MoveToTarget()
{
	Vec2f direction = Math::Normalized(mTargetPosition - GetPosition());
	GetBody()->SetVelocity(direction * mStats->mMoveSpeed.Get());
	return bt::Status::Success;
}

bt::Status BossCrystal::DoMeleeAttack()
{
	if (!mMeleeAttackAnimation->HasInfluence())
	{
		SetAnimation(mMeleeAttackAnimation);
	}

	return bt::Status::Success;
}

bt::Status BossCrystal::SelectRandomTarget()
{
	Vec2f playerPos = game->GetLevel()->GetPlayer()->GetPosition();

	sf::FloatRect zone = { playerPos.x - 100, playerPos.y - 100, 200, 200 };

	mTargetPosition = Math::RandomPoint(zone);

	return bt::Status::Success;
}

void BossCrystal::Serialize(Serializer & sav)
{
	Boss::Serialize(sav);
	sav & mWhirlWaitState;
	sav & mAttackHitboxSprite;
	sav & mMeleeAttackAnimation;
	sav & mWhirlAttackAnimation;
	sav & mWhirlPointChangeState;
	sav & mTargetPosition;
	CreateTimeEvents();
}
