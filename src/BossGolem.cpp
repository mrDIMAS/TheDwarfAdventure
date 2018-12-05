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
#include "BossGolem.h"
#include "Level.h"
#include "Player.h"
#include "Animation.h"
#include "Stats.h"
#include "Projectile.h"
#include "VisualEffect.h"
#include "HUD.h"
#include "Arena.h"

void BossGolem::CreateTimeEvents() 
{
	mThrowFireballAnimation->AddTimeEvent(TimeEvent(0.8f, [this] { ThrowFireball(); }));
	mMeleeAttackAnimation->AddTimeEvent(TimeEvent(0.7f, [this] { HandsHit(); }));
	mBlastAnimation->AddTimeEvent(TimeEvent(0.5f, [this] { DoBlast(); }));

	mBehaviour = bt::Builder()
		.Sequence("Fight")
		    .Condition(BTFUNC(!IsDead))
			.Selector("AttackType")
				.Sequence("Blast")
				    .Wait(&mBlastWaitState, SECONDS(16))
		            .Do(BTFUNC(BlastAttack))
				.End()
		        .Sequence("OrdinaryAttacks")
		            .Condition(BTFUNC(BlastAttackEnded))
					.Do(BTFUNC(SetPlayerAsTarget))
					.Do(BTFUNC(LookAtTarget))
		            .Selector("AttackType")
		                .Sequence("Distant")
		                    .Condition(BTFUNC(CanDoDistantAttack))
		                    .Do(BTFUNC(ThrowFireballAttack))
		                .End()
						.Sequence("Melee")			
		                    .Condition(BTFUNC(CanDoMeleeAttack))
		                    .Do(BTFUNC(MeleeAttack))
						.End()
		                .Sequence("WalkCloser")
							.Do(BTFUNC(MoveToTarget))
						.End()
		            .End()		            
		        .End()
			.End()
		.End()
		.Build();
}

void BossGolem::ThrowFireball()
{
	auto location = GetLocation();
	const auto shootPt = mFireballThrowPos->GetGlobalPosition();
	const auto direction = Math::Normalized(mTargetPosition - shootPt);
	location->AddProjectile(make_shared<Projectile>(location, shared_from_this(), ItemType::Projectile_Fireball, shootPt, direction * 40.0f, 0.0f));
}

void BossGolem::HandsHit()
{
	Damage dmg;
	dmg.mWho = shared_from_this();
	dmg.mCount = 20;
	dmg.mClass = DamageClass::Physical;
	dmg.mHitbox = FindHitbox(mLeftLowerArm);
	GetLocation()->PropagateDamage(dmg, this);
}

void BossGolem::DoBlast()
{
	auto location = GetLocation();
	for (auto &actor : location->GetActorList())
	{
		if (actor.get() != this)
		{
			const Vec2f circlePosition = mPivot->GetGlobalPosition();
			const float circleRadius = 250;
			if (actor->IsAnyHitBoxIntersected(circlePosition, circleRadius))
			{
				Damage dmg;
				dmg.mWho = shared_from_this();
				dmg.mCount = 400;
				dmg.mClass = DamageClass::Fire;
				dmg.mFlags = DMG_UNBLOCKABLE;
				actor->ApplyDamage(dmg);
			}
		}
	}
	VisualEffect::MakeExplosion(mModelRoot->GetScene(), mFireballThrowPos->GetGlobalPosition(), { 500, 500 });
}

BossGolem::BossGolem()
{
}

BossGolem::BossGolem(const shared_ptr<Arena>& arena) : Boss(arena, 60)
{
	const shared_ptr<Location> location = arena->GetLocation();

	const shared_ptr<Scene> prefab = game->LoadPrefab("data/prefabs/golem.prefab");

	mModelRoot = prefab->FindStrict<SceneNode>("Root");

	mAnimations = prefab->GetAnimationList();

	mFireballThrowPos = prefab->FindStrict<SceneNode>("Fireball");

	mStandAnimation = prefab->FindAnimationStrict("Stand");
	mStandAnimation->SetSpeed(0.5f);

	mWalkAnimation = prefab->FindAnimationStrict("Walk");
	mMeleeAttackAnimation = prefab->FindAnimationStrict("Melee");
	mLeftLowerArm = prefab->FindStrict<SceneNode>("LeftLowerArm");
	mThrowFireballAnimation = prefab->FindAnimationStrict("Fireball");

	mBlastAnimation = prefab->FindAnimationStrict("Blast");
	mBlastAnimation->SetEnabled(false);
	mBlastAnimation->SetLooped(false);
	mBlastAnimation->SetSpeed(0.5f);

	mDeathAnimation = prefab->FindAnimationStrict("Death");
	mDeathAnimation->SetEnabled(false);
	mDeathAnimation->SetLooped(false);

	mModelRoot->AttachTo(mPivot);
	mModelRoot->SetLocalPosition({ 0, 0 });

	location->GetScene()->Merge(prefab);

	mStats->Vitality.Set(60);
	mStats->Dexterity.Set(1);
	mStats->mFireDamageResist.Set(0.9f);
	mStats->mPhysicalDamageResist.Set(0.2f);
	mStats->mChemicalDamageResist.Set(0.4f);
	mStats->mExpirienceDrop = 600;

	mName = game->GetStr("golem");

	CreateTimeEvents();
}

BossGolem::~BossGolem()
{
}

void BossGolem::OnThink()
{
	mBehaviour->Tick();
}

void BossGolem::OnIdle()
{
	SetAnimation(mStandAnimation);
}

bt::Status BossGolem::BlastAttack()
{
	mBlastAnimation->Rewind();
	SetAnimation(mBlastAnimation);
	return bt::Status::Success;
}

bt::Status BossGolem::ThrowFireballAttack()
{
	SetAnimation(mThrowFireballAnimation);
	return bt::Status::Success;
}

bt::Status BossGolem::MeleeAttack()
{
	SetAnimation(mMeleeAttackAnimation);
	return bt::Status::Success;
}

bool BossGolem::CanDoMeleeAttack() const
{
	bool closeEnough = mMeleeAttackRange.Contains(Math::Distance(mTargetPosition, GetPosition()));
	return closeEnough && (mThrowFireballAnimation->IsEnded() || !mThrowFireballAnimation->IsEnabled());
}

bool BossGolem::CanDoDistantAttack() const
{
	bool closeEnough = mDistantAttackRange.Contains(Math::Distance(mTargetPosition, GetPosition()));
	return closeEnough && (mMeleeAttackAnimation->IsEnded() || !mMeleeAttackAnimation->IsEnabled());
}

bool BossGolem::BlastAttackEnded() const
{
	return mBlastAnimation->IsEnded() || !mBlastAnimation->IsEnabled();
}

bt::Status BossGolem::LookAtTarget() const
{
	float xDelta = mTargetPosition.x - GetPosition().x;
	mModelRoot->SetHorizontalScale(xDelta> 0 ? 1.0f : -1.0f);
	return bt::Status::Success;
}

bt::Status BossGolem::SetPlayerAsTarget()
{
	mTargetPosition = game->GetLevel()->GetPlayer()->GetPosition();
	return bt::Status::Success;
}

bt::Status BossGolem::MoveToTarget()
{
	Vec2f direction = Math::Normalized(mTargetPosition - GetPosition());
	GetBody()->SetVelocity(direction * mStats->mMoveSpeed.Get());
	SetAnimation(mWalkAnimation);
	return bt::Status::Success;
}

void BossGolem::Serialize(Serializer &sav)
{
	Boss::Serialize(sav);
	sav &mMeleeAttackRange;
	sav &mDistantAttackRange;
	sav &mStandAnimation;
	sav &mWalkAnimation;
	sav &mMeleeAttackAnimation;
	sav &mThrowFireballAnimation;
	sav &mFallAttackAnimation;
	sav &mBlastAnimation;
	sav &mBlastWaitState;
	sav &mTargetPosition;
	sav & mLeftLowerArm;
	sav & mFireballThrowPos;	
	if (sav.IsLoading())
	{
		CreateTimeEvents();
	}
}
