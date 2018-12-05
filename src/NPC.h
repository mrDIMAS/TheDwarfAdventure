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

#include "Dwarf.h"
#include "Router.h"
#include "BehaviourTree.h"

enum class NPCType
{
	Local,
	Blacksmith,
	Merchant,
	Thief,
	Guard,
};

// represent npc
// can talk, barter with player
class NPC : public Dwarf
{
public:
	NPC();
	NPC(const shared_ptr<Location> &location, NPCType type);
	virtual ~NPC();
	virtual void Serialize(Serializer &sav) override;
	virtual void Think() override final;
	void BeginDialog();
	// Called by world generator. Randomizes character appearance, sets level using base level of 
	// the location (location on which this NPC belong to)
	void Randomize();
	virtual void ApplyDamage(Damage & dmg) override;
	virtual void SetPosition(const Vec2f &p) override;
	void SetDefaultPosition(const Vec2f &p);
	void SetRoom(const shared_ptr<Room> &room);
	shared_ptr<Room> GetRoom() const;
	bool IsBusy();
	void ChillOut();
	bool IsAggressiveToPlayer();
	void SetPatrolPoints(const Vec2f & ptBegin, const Vec2f & ptEnd);
	virtual void OnSetLevel() override;
	void MakeHostile();
	bool IsHostile() const;
private:
	Router mRouter;
	Vec2f mWayPoint;
	Vec2f mPatrolEnd;
	Vec2f mDestination;
	Vec2f mPatrolBegin;
	Vec2f mLookPosition;
	weak_ptr<Room> mRoom;
	weak_ptr<Actor> mTargetActor;
	Vec2f mDefaultPosition;
	shared_ptr<PolyLine> mDebugPathLine;
	int mWarningsForPlayer { 5 };
	bool mModelFlipped { false };
	int mTargetLookInterval { 0 };
	shared_ptr<bt::Node> mBehaviour;
	float mThresholdWalkValue { 5 };
	float mDestinationThresholdDistance { 40 };
	float mSafeDistanceToEnemy { 60 };
	int mTargetRecheckTimeout { 600 };
	int mTargetRecheckInterval { 600 };
	int mPatrolTimeout { 180 };
	NPCType mType { NPCType::Blacksmith };
	int mMerchantGoodsUpdateTimer { MerchantGoodsUpdateInterval };
	// When reach zero, some goods will be added to merchant inventory
	static constexpr int MerchantGoodsUpdateInterval = static_cast<int>(10 * 60 * FIXED_FPS);

	void SetTarget(const shared_ptr<Actor> &target);
	virtual void OnDie() override;
	void ShowDebugInfo();
	void UpdateHealthBar();
	bool IsOnLadder() const;

	// Behaviour nodes
	bt::WaitNode::State mWaitPatrolEndState;
	bt::WaitNode::State mWaitPatrolBeginState;
	bool IsTooCloseToEnemy();
	bool NeedHealing() const;
	bool IsEnemyCloseEnoughToDefaultPosition() const;
	bool IsDefaultPositionTooDangerous();
	bool ReachDestination() const;
	bt::Status SetMoveDestinationSafePoint();
	bt::Status LookForTarget();
	bt::Status SelectWeapon();
	bt::Status SetMoveDestinationDefault();
	bt::Status SetMoveDestinationEnemyPosition();
	bt::Status SetMoveDestinationFarEnoughFromEnemy();
	bt::Status UseHealingPotion();
	bt::Status LookAtActor();
	bt::Status LookAtWaypoint();
	bt::Status AimAtLookPoint();
	bt::Status MoveToDestination();
	bt::Status UseWeapons();
	bt::Status OpenNearbyDoors();
	bt::Status StayStill();
	bt::Status LookStraightAhead();
	bool IsReachedPatrolBegin();
	bool IsReachedPatrolEnd();
	bt::Status SetMoveDestinationPatrolBegin();
	bt::Status SetMoveDestinationPatrolEnd();
	bool IsDestinationTooFar() const;
	bt::Status BuildPathToDestination();
	void CreateBehaviour();
};