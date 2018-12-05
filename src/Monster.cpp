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
#include "Monster.h"
#include "Level.h"
#include "Renderer.h"
#include "Remains.h"
#include "Stats.h"
#include "Player.h"
#include "Terrain.h"
#include "Dialog.h"
#include "ItemDatabase.h"
#include "Inventory.h"
#include "Text.h"
#include "Player.h"
#include "IK.h"
#include "NPC.h"
#include "PolyLine.h"
#include "Sound.h"
#include "Animation.h"
#include "AnimationTrack.h"

void Creature::RestoreFrameListeners()
{
	mAttackAnimation->AddTimeEvent(TimeEvent(mDesc.mAttackTime, [this] { DoAttack(); }));
}

void Creature::LookForTarget()
{
	THROW_IF_EXPIRED(mLocation);

	auto location = mLocation.lock();
	auto pos = GetPosition();
	auto terrain = location->GetScene()->GetTerrain();

	if (mTargetRecheckInterval <= 0)
	{
		// we have to check direct visibility to the current target
		if (auto currentTarget = mTargetActor.lock())
		{
			if (terrain->TraceRay(pos, currentTarget->GetPosition() - pos).mIntersection ||
				!IsSeeActor(currentTarget))
			{
				// no direct visibility - target is lost
				mTargetActor.reset();
			}
		}
		mTargetRecheckInterval = mTargetRecheckTimeout;
	}

	// if we have no target
	if (mTargetLookInterval <= 0 && mTargetActor.expired())
	{
		// check each actor on the location
		for (auto &actor : location->GetActorList())
		{

			// target can be only dwarf
			if (!dynamic_pointer_cast<Dwarf>(actor))
			{
				continue;
			}

			// check if some dwarf touch this actor, and if so make actor target
			for (auto &actorHitbox : actor->GetHitBoxes())
			{
				for (auto &hitbox : mHitBoxes)
				{
					if (actorHitbox.IsIntersects(hitbox))
					{
						SetTarget(actor);

						// we have our target, jump out of this mess
						goto nextStage;
					}
				}
			}

			// if "view-triangle" can see an actor and if actor is NPC or Player
			if (IsSeeActor(actor))
			{
				// we have to check direct visibility using ray cast
				if (!terrain->TraceRay(pos, actor->GetPosition() - pos).mIntersection)
				{
					// if we already have target
					if (auto target = mTargetActor.lock())
					{
						// check if new target closer than previous
						if (Math::SqrDistance(actor->GetPosition(), pos) <
							Math::SqrDistance(target->GetPosition(), pos))
						{
							SetTarget(actor);
						}
					}
					else
					{
						// otherwise, we have the target
						SetTarget(actor);
					}
				}
			}
		}

	nextStage:

		// randomize interval to except simultaneous call of this routine of huge
		// amount of monsters at the same time
		mTargetLookInterval = Math::Rnd(45, 60);
	}

	// decrease counters
	--mTargetLookInterval;
	--mTargetRecheckInterval;
}

void Creature::UpdateVision()
{
	// properly orient and position viewing triangle of a monster
	mViewTriangle.SetOrigin(GetPosition());
	if (auto target = mTargetActor.lock())
	{
		// look directly on the target
		mViewTriangle.SetAngle(Math::CartesianToPolar(target->GetPosition() - GetPosition()).mAngle);
	}
	else
	{
		// look left or right when no target
		if (mDesc.mFlipModel)
		{
			mViewTriangle.SetAngle(mModelRoot->GetLocalScale().x > 0.0f ? 0.0f : 180.0f);
		}
		else
		{
			mViewTriangle.SetAngle(mModelRoot->GetLocalScale().x > 0.0f ? 180.0f : 0.0f);
		}
	}
}

void Creature::OnDie()
{
	// drop loot, if got some
	if (mInventory)
	{
		GetLocation()->AddRemains(make_shared<Remains>(GetLocation()->GetScene(), GetPosition(), mInventory));
	}
}

void Creature::OnSetLevel()
{
	const auto characterLevel = static_cast<float>(mStats->GetLevel());
	const auto gameLevel = mLocation.lock()->GetLevel();

	mStats->Vitality.Set(static_cast<int>(ceil(characterLevel * mDesc.mVitalityFactor)));
	mStats->Strenght.Set(static_cast<int>(ceil(characterLevel * mDesc.mStrengthFactor)));
	mStats->Dexterity.Set(static_cast<int>(ceil(characterLevel * mDesc.mDexterityFactor)));
	mStats->Endurance.Set(static_cast<int>(ceil(characterLevel * mDesc.mEnduranceFactor)));
	mStats->Intelligence.Set(static_cast<int>(ceil(characterLevel * mDesc.mIntelligenceFactor)));
	mStats->Luck.Set(static_cast<int>(ceil(characterLevel * mDesc.mLuckFactor)));
	mStats->Charisma.Set(1);
	const float kLevel = characterLevel / static_cast<float>(gameLevel->GetMaxBaseLevel());
	mStats->mExpirienceDrop = static_cast<int>(mDesc.mBaseExpirienceDrop * (1.0f + 3.0f * kLevel));
	mDamage = mDesc.mBaseDamage * (1.0f + 6.0f * kLevel);
}

void Creature::LookAt(const Vec2f &p)
{
	float dx = p.x - GetPosition().x;
	float x = mDesc.mFlipModel ? -1.0f : 1.0f;
	mModelRoot->SetLocalScale(dx > 0.0f ? -x : x, 1.0f);
}

void Creature::SetTarget(const shared_ptr<Actor> &t)
{
	if (t == shared_from_this())
	{
		return;
	}
	mTargetActor = t;
	mTargetRecheckInterval = mTargetRecheckTimeout;
	mTargetPosition = t->GetPosition();
}

void Creature::DoAttack()
{
	if (auto actor = mTargetActor.lock())
	{
		if (mAttackHitbox)
		{
			if (actor->IsAnyHitBoxIntersected(Math::RotatedRectangle(mAttackHitbox->GetSize(), mAttackHitbox->GetGlobalTransform())))
			{
				Damage dmg;
				dmg.mCount = mDamage;
				dmg.mClass = DamageClass::Physical;
				dmg.mWho = shared_from_this();
				actor->ApplyDamage(dmg);
			}
		}
	}
}

void Creature::UpdateHitBoxesRecursively(const shared_ptr<SceneNode> &entry)
{
	if (entry == mAttackHitbox)
	{
		return;
	}

	Actor::UpdateHitBoxesRecursively(entry);
}

Creature::Creature()
{
}

Creature::Creature(const shared_ptr<Location> & location, const MonsterDefinition & desc) :
	Actor(location, 1), mDesc(desc)
{
	SetLocation(location);
	auto &scene = location->GetScene();
	auto prefab = game->LoadPrefab(mDesc.mPrefab);
	mModelRoot = prefab->FindStrict<SceneNode>("Root");
	mModelRoot->AttachTo(mPivot);
	mModelRoot->SetLocalPosition({ 0, mPivot->GetBody()->GetRadius() });
	mWalkAnimation = prefab->FindAnimationStrict("Walk");
	mAttackAnimation = prefab->FindAnimationStrict("Attack");
	mAttackHitbox = prefab->FindStrict<SceneNode>("AttackHitbox");
	mAnimations = prefab->GetAnimationList();
	scene->Merge(prefab);
	mStats->mMoveSpeed.SetMax(desc.mMaxSpeed);
	GetBody()->SetRadius(mDesc.mBodyRadius);
	if (mDesc.mMovementType == CreatureMovementType::Fly)
	{
		GetBody()->SetGravity({ 0, 0 });
	}
	SetName(mDesc.mName);
	ArrangeIndicators();
	RestoreFrameListeners();
	SetLevel(1);
}

Creature::~Creature()
{
	if (mDebugPathLine)
	{
		mDebugPathLine->RemoveFromScene();
		mDebugPathLine.reset(); 
	}
	DBG_ECHO();
}

CreatureType Creature::GetType() const
{
	return mType;
}

bool Creature::IsBusy() const
{
	return mTargetActor.expired() == false;
}

void Creature::SetPatrolLine(const Math::Line &line)
{
	mPatrolLine = line;
}

void Creature::ApplyDamage(Damage & dmg)
{
	if (dmg.mWho && dmg.mWho != shared_from_this())
	{
		SetTarget(dmg.mWho);
		LookAt(dmg.mWho->GetPosition());
	}

	Actor::ApplyDamage(dmg);
}

void Creature::Think()
{
	if (IsDead())
	{
		return;
	}

	THROW_IF_EXPIRED(mLocation);

	auto location = mLocation.lock();
	auto scene = location->GetScene();
	auto body = GetBody();

	LookForTarget();

	mHealthBar->SetSize({ 40 * (mStats->GetHealth() / mStats->mMaxHealth.Get()), 5 });
	mHealthBar->SetOriginInCenter();

	if (mDesc.mMovementType == CreatureMovementType::Stand)
	{
		body->SetHorizontalVelocity(0);
	}
	else
	{
		mRouter.SetGoal(mTargetPosition);
		mRouter.SetStart(GetPosition());
		auto vertexFlag = (mDesc.mMovementType == CreatureMovementType::Fly) ? PassabilityFlag_Fly : PassabilityFlag_Walk;
		mRouter.Update(location->GetScene()->GetTerrain(), vertexFlag);
		// debug
		if (game->Debug.showPaths)
		{
			if (mRouter.IsPathReady())
			{
				if (!mDebugPathLine)
				{
					mDebugPathLine = scene->CreatePolyLine();
				}
				mDebugPathLine->Clear();
				for (auto &v : mRouter.GetPath())
				{
					mDebugPathLine->AddPoint(v);
				}
			}
		}
		else
		{
			if (mDebugPathLine)
			{
				mDebugPathLine->RemoveFromScene();
				mDebugPathLine.reset();
			}
		}
	}

	UpdateVision();

	if (mIdleSound.expired())
	{
		if (!mDesc.mIdleSound.empty()) {
			mIdleSound = scene->EmitSound(mDesc.mIdleSound, mModelRoot);
			mIdleSound.lock()->SetVolume(mDesc.mIdleSoundVolume);
		}
	}

	float distanceToTargetActor = FLT_MAX;
	if (auto targetActor = mTargetActor.lock())
	{
		mTargetPosition = targetActor->GetPosition();
		distanceToTargetActor = Math::Length(targetActor->GetPosition() - GetPosition());
	}
	else
	{
		// just patrolling
		if (mPatrolToEnd)
		{
			if (Math::SqrDistance(GetPosition(), mPatrolLine.mEnd) < 1024)
			{
				mPatrolToEnd = false;
				mTargetPosition = mPatrolLine.mBegin;
			}
			else
			{
				mTargetPosition = mPatrolLine.mEnd;
			}
		}
		else
		{
			if (Math::SqrDistance(GetPosition(), mPatrolLine.mBegin) < 1024)
			{
				mPatrolToEnd = true;
				mTargetPosition = mPatrolLine.mEnd;
			}
			else
			{
				mTargetPosition = mPatrolLine.mBegin;
			}
		}
	}

	if (mRouter.IsPathReady())
	{
		// follow path
		Vec2f moveDir = Math::Normalized(mRouter.GetTarget() - GetPosition());
		if (mDesc.mMovementType != CreatureMovementType::Stand)
		{
			// close enough to attack target
			if (distanceToTargetActor <= mDesc.mAttackDistance || IsInStunLock())
			{
				if (mDesc.mMovementType == CreatureMovementType::Fly)
				{
					body->SetVelocity({ 0, 0 });
				}
				else
				{
					body->SetHorizontalVelocity(0);
				}
				// too far away from target, move to it
			}
			else if (distanceToTargetActor > mDesc.mAttackDistance)
			{
				if (mDesc.mMovementType == CreatureMovementType::Fly)
				{
					body->SetVelocity(moveDir * mStats->mMoveSpeed.Get());
				}
				else
				{
					if (mDesc.mMovementType == CreatureMovementType::Jump)
					{
						// pulse movement while jumping
						if (IsGroundContact())
						{
							body->SetVelocity({ moveDir.x * mStats->mMoveSpeed.Get(), -mStats->mJumpVerticalSpeed.Get() });
						}
					}
					else
					{
						// continuous movement
						body->SetHorizontalVelocity(moveDir.x * mStats->mMoveSpeed.Get());
					}
				}
			}
		}
		// walking or flying monsters looks on the router target (current path vertex)
		LookAt(mRouter.GetTarget());
	}

	// standing monsters looks directly to target
	if (mDesc.mMovementType == CreatureMovementType::Stand)
	{
		LookAt(mTargetPosition);
	}

	if (IsInStunLock())
	{
		mStunLockIndicator->Turn(10.0f);
		mStunLockIndicator->SetVisible(true);
	}
	else
	{
		mStunLockIndicator->SetVisible(false);
		if (distanceToTargetActor < mDesc.mAttackDistance)
		{
			SetAnimation(mAttackAnimation);
		}
		else
		{
			SetAnimation(mWalkAnimation);
		}
	}

	--mStunLockInterval;
	--mNextPossibleStunLockTimeout;
	if (mHealSign)
	{
		--mHealSignTimer;
		mHealSign->SetVisible(mHealSignTimer > 0 || mStats->IsHealing());
	}
}

void Creature::Serialize(Serializer &sav)
{
	Actor::Serialize(sav);
	sav &mDesc;
	sav &mTargetPosition;
	sav &mTargetActor;
	sav &mType;
	sav &mDamage;
	sav &mTargetLookInterval;
	sav &mLastSetPosition;
	sav &mTargetRecheckTimeout;
	sav &mTargetRecheckInterval;
	sav &mPatrolLine.mBegin;
	sav &mPatrolLine.mEnd;
	sav &mPatrolToEnd;
	sav &mRouter;
	sav &mAttackAnimation;
	sav &mWalkAnimation;
	sav &mAttackHitbox;
	if (sav.IsLoading())
	{
		RestoreFrameListeners();
	}
}

void Creature::SetPosition(const Vec2f &p)
{
	Actor::SetPosition(p);


	mLastSetPosition = p;
}

void MonsterDefinition::Serialize(Serializer & sav)
{
	sav & mBodyRadius;
	sav & mPrefab;
	sav & mAttackTime;
	sav & mAttackDistance;
	sav & mBaseDamage;
	sav & mBaseExpirienceDrop;
	sav & mMovementType;
	sav & mVitalityFactor;
	sav & mStrengthFactor;
	sav & mDexterityFactor;
	sav & mEnduranceFactor;
	sav & mIntelligenceFactor;
	sav & mLuckFactor;
	sav & mFlipModel;
	sav & mAttackSoundList;
}
