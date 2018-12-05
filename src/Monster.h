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
#include "Router.h"
#include "Location.h"
#include "BehaviourTree.h"

class FogGate;

#define CREATURE_TYPE_ENUM_ENTRIES(X) \
	X(Unknown, "Unknown") \
	X(Monster_Bee, "Monster_Bee") \
	X(Monster_Frog, "Monster_Frog") \
	X(Monster_Grub, "Monster_Grub") \
	X(Monster_Worm, "Monster_Worm") \
	X(Monster_Rat, "Monster_Rat") 

ENUM_DECLARE(CreatureType, CREATURE_TYPE_ENUM_ENTRIES);

#undef CREATURE_TYPE_ENUM_ENTRIES

#define CREATURE_MOVEMENT_TYPE_ENUM_ENTRIES(X) \
	X(Walk, "Walk") \
	X(Jump, "Jump") \
	X(Stand, "Stand") \
	X(Fly, "Fly") 

ENUM_DECLARE(CreatureMovementType, CREATURE_MOVEMENT_TYPE_ENUM_ENTRIES);

#undef CREATURE_MOVEMENT_TYPE_ENUM_ENTRIES

struct MonsterDefinition
{
	float mBodyRadius { 0 };
	string mPrefab;
	string mName;
	float mAttackTime { 0 };
	float mAttackDistance { 0 };
	float mBaseDamage { 100 };
	float mBaseExpirienceDrop { 0 };
	CreatureMovementType mMovementType { CreatureMovementType::Walk };
	float mVitalityFactor { 0 };
	float mStrengthFactor { 0 };
	float mDexterityFactor { 0 };
	float mEnduranceFactor { 0 };
	float mIntelligenceFactor { 0 };
	float mLuckFactor { 0 };
	bool mFlipModel { false };
	float mMaxSpeed { std::numeric_limits<float>::max() };
	string mIdleSound;
	float mIdleSoundVolume;
	vector<string> mAttackSoundList;
	vector<LocationType> mLocations;
	void Serialize(Serializer & sav);
};

class Creature : public Actor
{
private:
	Vec2f mTargetPosition;
	weak_ptr<Actor> mTargetActor;
	shared_ptr<PolyLine> mDebugPathLine;
	MonsterDefinition mDesc;
	CreatureType mType { CreatureType::Unknown };
	shared_ptr<SceneNode> mAttackHitbox;
	// Damage on current level
	float mDamage { 100 };
	Vec2f mLastSetPosition;
	int mTargetLookInterval { 0 };
	int mTargetRecheckTimeout { 600 };
	int mTargetRecheckInterval { 600 };
	Math::Line mPatrolLine;
	bool mPatrolToEnd { true };
	Router mRouter;
	void RestoreFrameListeners();
	void LookForTarget();
	void UpdateVision();
	void LookAt(const Vec2f &p);
	void SetTarget(const shared_ptr<Actor> &t);
	weak_ptr<Sound> mIdleSound;
	shared_ptr<Animation> mWalkAnimation;
	shared_ptr<Animation> mAttackAnimation;
	void DoAttack();
	virtual void UpdateHitBoxesRecursively(const shared_ptr<SceneNode> &entry) override;
public:
	Creature();
	// Creature by default created with level 1
	Creature(const shared_ptr<Location> &location, const MonsterDefinition & desc);
	~Creature();
	CreatureType GetType() const;
	bool IsBusy() const;
	void SetPatrolLine(const Math::Line &line);
	virtual void ApplyDamage(Damage & dmg) override;
	virtual void Think() override final;
	virtual void Serialize(Serializer &sav) override final;
	virtual void SetPosition(const Vec2f &p) override final;
	virtual void OnDie() override final;
	virtual void OnSetLevel() override final;
};
