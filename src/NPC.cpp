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
#include "NPC.h"
#include "Weapon.h"
#include "Stats.h"
#include "Dialog.h"
#include "Level.h"
#include "Inventory.h"
#include "Terrain.h"
#include "Player.h"
#include "Ladder.h"
#include "Remains.h"
#include "PolyLine.h"
#include "HUD.h"
#include "WorldGenerator.h"
#include "Room.h"
#include "Door.h"
#include "NameDatabase.h"
#include "ResourceDatabase.h"

bt::Status NPC::LookForTarget()
{
	const auto location = mLocation.lock();
	const auto position = GetPosition();
	const auto terrain = location->GetScene()->GetTerrain();

	if (mTargetRecheckInterval <= 0)
	{
		// we have to check direct visibility to the current target
		if (auto currentTarget = mTargetActor.lock())
		{
			if (terrain->TraceRay(position, currentTarget->GetPosition() - position).mIntersection || !IsSeeActor(currentTarget))
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
			// if "view-triangle" can see an actor and if actor is NPC or Player
			if (IsSeeActor(actor) && dynamic_pointer_cast<Creature>(actor))
			{
				// we have to check direct visibility using ray cast
				if (!terrain->TraceRay(position, actor->GetPosition() - position).mIntersection)
				{
					// if we already have target
					if (auto target = mTargetActor.lock())
					{
						// check if new target closer than previous
						if (Math::SqrDistance(actor->GetPosition(), position) < Math::SqrDistance(target->GetPosition(), position))
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

		// randomize interval to except simultaneous call of this routine of huge
		// amount of monsters at the same time
		mTargetLookInterval = Math::Rnd(45, 60);
	}

	// decrease counters
	--mTargetLookInterval;
	--mTargetRecheckInterval;

	return mTargetActor.expired() ? bt::Status::Fail : bt::Status::Success;
}

bt::Status NPC::SelectWeapon()
{
	// Check weapons
	bool needSelectWeapons = false;
	int handIndex = 0;
	for (auto & handEquipment : GetHandSlots())
	{
		if (handEquipment)
		{
			if (IsOutOfWeaponConsumables(handEquipment->GetDefinition().mType, static_cast<Hand>(handIndex)))
			{
				needSelectWeapons = true;
			}
		}
		else
		{
			needSelectWeapons = true;
		}
		++handIndex;
	}

	// Select weapons if needed
	if (needSelectWeapons)
	{
		handIndex = 0;
		for (auto &group : mInventory->GetGroups())
		{
			const auto &firstItem = group->mItems.front();
			const auto &groupDesc = firstItem->GetDefinition();
			if (groupDesc.mClass == ItemClass::Weapon)
			{
				// ignore weapons without ammo
				if (!IsOutOfWeaponConsumables(groupDesc.mType, static_cast<Hand>(handIndex)) || groupDesc.mIsMeleeWeapon)
				{
					for (auto &item : group->mItems)
					{
						// ignore broken weapons
						if (!item->IsBroken())
						{
							UseItem(item, static_cast<Hand>(handIndex));

							++handIndex;
							if (handIndex >= 2)
							{
								// All hands are busy, exit loops
								goto endSelection;
							}
						}
					}
				}
			}
		}
	endSelection:;
	}

	bool gotAnyWeapon = false;
	handIndex = 0;
	for (auto & handEquipment : GetHandSlots())
	{
		if (handEquipment)
		{
			if (!IsOutOfWeaponConsumables(handEquipment->GetDefinition().mType, static_cast<Hand>(handIndex)))
			{
				gotAnyWeapon = true;
				break;
			}
		}
		++handIndex;
	}

	return gotAnyWeapon ? bt::Status::Success : bt::Status::Fail;
}

void NPC::SetTarget(const shared_ptr<Actor> &target)
{
	if (!target)
	{
		return;
	}

	if (target == shared_from_this())
	{
		return;
	}

	mTargetActor = target;
	mTargetRecheckInterval = mTargetRecheckTimeout;
}

void NPC::OnDie()
{
	if (mInventory)
	{
		GetLocation()->AddRemains(make_shared<Remains>(GetLocation()->GetScene(), GetPosition(), mInventory));
	}
}

void NPC::ShowDebugInfo()
{
	if (game->Debug.showPaths)
	{
		if (mRouter.IsPathReady())
		{
			if (!mDebugPathLine)
			{
				mDebugPathLine = mLocation.lock()->GetScene()->CreatePolyLine();
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

void NPC::UpdateHealthBar()
{
	mHealthBar->SetSize({ 40 * (mStats->GetHealth() / mStats->mMaxHealth.Get()), 5 });
	mHealthBar->SetOriginInCenter();
}

bool NPC::IsOnLadder() const
{
	const auto body = GetBody();

	for (auto &ladder : mLocation.lock()->GetLadderList())
	{
		if (ladder->Intersects(GetPosition(), GetBody()->GetRadius()))
		{
			return true;
		}
	}

	return false;
}

bt::Status NPC::SetMoveDestinationDefault()
{
	mDestination = mDefaultPosition;
	return bt::Status::Success;
}

bt::Status NPC::SetMoveDestinationEnemyPosition()
{
	if (!mTargetActor.expired())
	{
		mDestination = mTargetActor.lock()->GetPosition();
	}
	return bt::Status::Success;
}

bt::Status NPC::SetMoveDestinationFarEnoughFromEnemy()
{
	if (auto actor = mTargetActor.lock())
	{
		auto position = GetPosition();
		auto enemyPosition = actor->GetPosition();

		if (position.x < enemyPosition.x)
		{
			mDestination.x = enemyPosition.x - mSafeDistanceToEnemy;
		}
		else
		{
			mDestination.x = enemyPosition.x + mSafeDistanceToEnemy;
		}

		mDestination.y = enemyPosition.y;

		return bt::Status::Success;
	}
	else
	{
		return bt::Status::Fail;
	}
}

bt::Status NPC::UseHealingPotion()
{
	if (!GetInventory())
	{
		return bt::Status::Fail;
	}

	// Try to find potion
	ItemType potions[3] = {
		ItemType::Potion_StrongHeal,
		ItemType::Potion_MediumHeal,
		ItemType::Potion_WeakHeal,
	};

	for (auto potion : potions)
	{
		auto group = GetInventory()->FindGroup(potion);
		if (group && group->Count())
		{
			UseItem(group->mItems.at(0), Hand::Left);
			mHealSign->SetVisible(true);
			mHealSignTimer = 60;

			return bt::Status::Success;
		}
	}

	return bt::Status::Fail;
}

bt::Status NPC::LookAtActor()
{
	if (!mTargetActor.expired())
	{
		mLookPosition = mTargetActor.lock()->GetPosition();
	}
	return bt::Status::Success;
}

bt::Status NPC::LookAtWaypoint()
{
	mLookPosition = mWayPoint;
	return bt::Status::Success;
}

bool NPC::IsTooCloseToEnemy()
{
	if (!mTargetActor.expired())
	{
		if (Math::Distance(mTargetActor.lock()->GetPosition(), GetPosition()) < mSafeDistanceToEnemy)
		{
			return true;
		}
	}
	return false;
}

bool NPC::NeedHealing() const
{
	return mStats->GetHealth() < mStats->mMaxHealth.Get() * 0.35f;
}

bool NPC::IsEnemyCloseEnoughToDefaultPosition() const
{
	if (mTargetActor.expired())
	{
		return false;
	}

	return Math::Distance(mDefaultPosition, mTargetActor.lock()->GetPosition()) < 700;
}

bool NPC::IsDefaultPositionTooDangerous()
{
	bool dangerous = false;
	const float probeRadius = 150;
	// Probe area near default position
	for (auto & actor : GetLocation()->GetActorList())
	{
		if (Math::IsCircleCircleIntersection(actor->GetPosition(), actor->GetBody()->GetRadius(), mDefaultPosition, probeRadius))
		{
			if (dynamic_cast<Creature*>(actor.get()))
			{
				dangerous = true;
			}
		}
	}
	return dangerous;
}

bool NPC::ReachDestination() const
{
	return Math::Distance(mDestination, GetPosition()) < mDestinationThresholdDistance;
}

bt::Status NPC::SetMoveDestinationSafePoint()
{
	mDestination = { 0, 0 }; // Fixme

	return bt::Status::Success;
}

bt::Status NPC::AimAtLookPoint()
{
	mViewTriangle.SetOrigin(GetPosition());
	if (auto target = mTargetActor.lock())
	{
		mViewTriangle.SetAngle(Math::CartesianToPolar(target->GetPosition() - GetPosition()).mAngle);
	}
	else
	{
		mViewTriangle.SetAngle(mModelRoot->GetLocalScale().x > 0.0f ? 0.0f : 180.0f);
	}

	const auto direction = Math::Normalized(mLookPosition - GetPosition());

	// Flip model only if look position is far enough
	// This prevents NPC from insanely fast switching of look directions
	auto posX = mPivot->GetGlobalPosition().x;
	if (abs(mLookPosition.x - posX) > mThresholdWalkValue)
	{
		mModelFlipped = mLookPosition.x < posX;
	}
	
	mModelRoot->SetLocalScale(mModelFlipped ? -1.0f : 1.0f, 1.0f);
	AnimateHandsAndHeadAndUpdateWeapons(direction, mModelFlipped);
	return bt::Status::Success;
}

bt::Status NPC::MoveToDestination()
{
	auto router = &mRouter;
	auto body = GetBody();
	auto position = body->GetPosition();
	auto moveSpeed = GetStats()->mMoveSpeed.Get();

	router->SetStart(position);
	router->SetGoal(mDestination);
	router->Update(GetLocation()->GetScene()->GetTerrain(), PassabilityFlag_Walk);

	if (!router->IsPathReady())
	{
		return bt::Status::Fail;
	}

	mWayPoint = router->GetTarget();

	const Vec2f unnormDirection = mWayPoint - position;
	const Vec2f targetDirection = Math::Normalized(unnormDirection);

	if (IsInStunLock())
	{
		SetAnimation(mStunLockAnimation);
	}
	else
	{
		if (IsOnLadder())
		{
			mLastFeetPosition = GetFeetPosition();

			// Disable gravity when NPC is on a ladder, which allows NPC climb up and down
			body->SetGravity({ 0, 0 });

			// NPC standing on a ladder - lock vertical velocity 
			body->SetVerticalVelocity(0);

			body->SetVelocity(targetDirection * moveSpeed);

			SetAnimation(mWalkAnimation);
		}
		else if (IsGroundContact())
		{
			// Disable gravity when NPC standing on solid ground
			// Prevents sliding down from steep surfaces
			body->SetGravity({ 0, 0 });

			body->SetHorizontalVelocity(targetDirection.x * moveSpeed);			

			if (abs(unnormDirection.x) >= mThresholdWalkValue)
			{
				SetAnimation(mWalkAnimation);
			}
			else
			{
				SetAnimation(mStandAnimation);
			}
		}
		else
		{
			body->SetGravity({ Body::DefaultGravityX, Body::DefaultGravityY });
		}
	}

	return IsInStunLock() ? bt::Status::Fail : bt::Status::Success;
}

bt::Status NPC::UseWeapons()
{
	const auto toTarget = Math::Normalized(mTargetActor.lock()->GetPosition() - GetPosition());

	int attackHandIndex = 0;
	for (auto & handEquipment : GetHandSlots())
	{
		if (auto wpn = dynamic_pointer_cast<Weapon>(handEquipment))
		{
			if (wpn->GetDefinition().mIsMeleeWeapon)
			{
				if (mMeleeAttackState[attackHandIndex] == MeleeAttackState::Ready && mStats->ConsumeStamina(10))
				{
					mMeleeAttackState[attackHandIndex] = MeleeAttackState::Swing;
				}
			}
			else
			{
				wpn->Shoot(toTarget);
			}
		}
		++attackHandIndex;
	}

	return bt::Status::Success;
}

bt::Status NPC::OpenNearbyDoors()
{
	bool allDoorsAreOpened = true;

	// Check doors
	for (auto & door : mLocation.lock()->GetDoorList())
	{
		sf::FloatRect bounds = { door->GetPosition(), door->GetSize() };
		bounds.left -= 20;
		bounds.top -= 20;
		bounds.width += 40;
		bounds.height += 40;
		if (bounds.contains(GetPosition()))
		{
			OpenDoor(door);
			if (door->GetState() != DoorState::Open)
			{
				allDoorsAreOpened = false;
			}
		}
	}


	return allDoorsAreOpened ? bt::Status::Success : bt::Status::Fail;
}

bt::Status NPC::StayStill()
{
	if (IsInStunLock())
	{
		SetAnimation(mStunLockAnimation);
	}
	else
	{
		SetAnimation(mStandAnimation);
	}

	float dx = mModelFlipped ? -1.0f : 1.0f;
	GetBody()->SetHorizontalVelocity(0);
	mModelRoot->SetLocalScale(dx, 1.0f);
	AnimateHandsAndHeadAndUpdateWeapons({ dx, 0 }, mModelFlipped);

	return bt::Status::Success;
}

bt::Status NPC::LookStraightAhead()
{
	mLookPosition = mWayPoint;
	mLookPosition.y = GetPosition().y;
	return bt::Status::Success;
}

bool NPC::IsReachedPatrolBegin()
{
	return Math::Distance(GetPosition(), mPatrolBegin) < mDestinationThresholdDistance;
}

bool NPC::IsReachedPatrolEnd()
{
	return Math::Distance(GetPosition(), mPatrolEnd) < mDestinationThresholdDistance;
}

bt::Status NPC::SetMoveDestinationPatrolBegin()
{
	mDestination = mPatrolBegin;
	return bt::Status::Success;
}

bt::Status NPC::SetMoveDestinationPatrolEnd()
{
	mDestination = mPatrolEnd;
	return bt::Status::Success;
}

bool NPC::IsDestinationTooFar() const
{
	return Math::Distance(GetPosition(), mDestination) > 1000;
}

bt::Status NPC::BuildPathToDestination()
{
	auto router = &mRouter;

	router->SetStart(GetBody()->GetPosition());
	router->SetGoal(mDestination);
	router->Update(GetLocation()->GetScene()->GetTerrain(), PassabilityFlag_Walk);

	if (!router->IsPathReady())
	{
		return bt::Status::Fail;
	}

	mWayPoint = router->GetTarget();

	return bt::Status::Success;
}

void NPC::CreateBehaviour()
{
	const shared_ptr<bt::Node> genericHeal = bt::Builder()
		.Sequence("Heal")
			.Condition(BTFUNC(NeedHealing))
			.Do(BTFUNC(UseHealingPotion))
		.End()
		.Build();

	const shared_ptr<bt::Node> genericAttack = bt::Builder()
		.Sequence("Attack")
			.Condition(BTFUNC(!NeedHealing))
			.Do(BTFUNC(LookForTarget))
			.Do(BTFUNC(SelectWeapon))
			.Selector("WalkDestination")
				.Sequence("Default") 
					.Condition(BTFUNC(!IsEnemyCloseEnoughToDefaultPosition))
					.Do(BTFUNC(SetMoveDestinationDefault))
				.End()
				.Sequence("AwayFromEnemy")
					.Condition(BTFUNC(IsTooCloseToEnemy))
					.Do(BTFUNC(SetMoveDestinationFarEnoughFromEnemy))
				.End()					
				.Do(BTFUNC(SetMoveDestinationEnemyPosition))
			.End()
			.Do(BTFUNC(OpenNearbyDoors))
			.Do(BTFUNC(MoveToDestination))
			.Do(BTFUNC(LookAtActor))
			.Do(BTFUNC(AimAtLookPoint))
			.Do(BTFUNC(UseWeapons))
		.End()
		.Build();

	const shared_ptr<bt::Node> walkToWaypoint = bt::Builder()
		.Sequence()
			.Do(BTFUNC(LookStraightAhead))
			.Do(BTFUNC(AimAtLookPoint))
			.Do(BTFUNC(OpenNearbyDoors))
			.Do(BTFUNC(MoveToDestination))
		.End()
		.Build();

	const shared_ptr<bt::Node> genericWalk = bt::Builder()
		.Sequence("WalkAway")
			.Selector("Position")
				.Sequence("Danger")
					.Condition(BTFUNC(IsDefaultPositionTooDangerous))
					.Do(BTFUNC(SetMoveDestinationSafePoint))
				.End()
				.Do(BTFUNC(SetMoveDestinationDefault))
			.End()				
		    .Inject(walkToWaypoint)
		.End()
		.Build();

	const shared_ptr<bt::Node> genericIdle = bt::Builder()
		.Sequence("Idle")
			.Condition(BTFUNC(ReachDestination))
			.Do(BTFUNC(StayStill))
		.End()
		.Build();

	const shared_ptr<bt::Node> genericPatrol = bt::Builder()
		.Parallel()
			.Selector("PatrolDestination")
				.Sequence("Begin")
					.Condition(BTFUNC(IsReachedPatrolEnd))
		            .Wait(&mWaitPatrolEndState, mPatrolTimeout)
					.Do(BTFUNC(SetMoveDestinationPatrolBegin))
				.End()
				.Sequence("End")
					.Condition(BTFUNC(IsReachedPatrolBegin))
		            .Wait(&mWaitPatrolBeginState, mPatrolTimeout)
					.Do(BTFUNC(SetMoveDestinationPatrolEnd))
				.End()		        		        
			.End()
		    .Inject(walkToWaypoint)
		.End()
		.Build();

	switch (mType)
	{
	default:
	case NPCType::Local:
	case NPCType::Merchant:
	case NPCType::Blacksmith:
	case NPCType::Thief:
		mBehaviour = bt::Builder()
			.Parallel()
				.Inject(genericHeal)
				.Selector("Behaviour")				
					.Inject(genericAttack)				
					.Inject(genericWalk)
					.Inject(genericIdle)
				.End()	
			.End()
			.Build();
		break;
	case NPCType::Guard:
		mBehaviour = bt::Builder()
			.Parallel()
				.Inject(genericHeal)
				.Selector("Behaviour")
					.Inject(genericAttack)					
					.Inject(genericPatrol)
				.End()
			.End()
			.Build();
		break;
	}
}

NPC::NPC()
{
	CreateBehaviour();	
}

NPC::NPC(const shared_ptr<Location> &location, NPCType type)
	: Dwarf(location)
	, mType(type)
{
	string name;

	// NPCs killing is not encouraged
	mStats->mExpirienceDrop = 0;

	switch (mType)
	{
	case NPCType::Local:
		name = game->GetStr("local");
		break;
	case NPCType::Merchant:
		name = game->GetStr("merchant");
		break;
	case NPCType::Blacksmith:
		name = game->GetStr("blacksmith");
		break;
	case NPCType::Thief:
		name = game->GetStr("thief");
		break;
	case NPCType::Guard:
		name = game->GetStr("guard");
		break;
	}

	SetName(name + " " + game->GetNameDatabase()->GetRandomMaleName());

	CreateBehaviour();

	// Default level is 1
	SetLevel(1);
}

NPC::~NPC()
{
}

void NPC::Serialize(Serializer &sav)
{
	Dwarf::Serialize(sav);
	sav &mRouter;
	sav &mRoom;
	sav &mWarningsForPlayer;
	sav &mWaitPatrolEndState;
	sav &mWaitPatrolBeginState;
}

void NPC::Think()
{
	mBehaviour->Tick();
	UpdateHealthBar();
	ShowDebugInfo();
}

void NPC::BeginDialog()
{
	if (game->GetLevel()->GetPlayer()->GetStats()->GetReputation() < 0)
	{
		BeginDialogOfType<BadReputationDialog>();		
	}
	else
	{
		switch (mType)
		{
		case NPCType::Blacksmith:
			BeginDialogOfType<BlacksmithDialog>();			
			break;
		case NPCType::Thief:
			BeginDialogOfType<ThiefTalkDialog>();			
			break;
		case NPCType::Merchant:
			BeginDialogOfType<MerchantTalkDialog>();			
			break;
		case NPCType::Local:
			BeginDialogOfType<LocalTalkDialog>();			
			break;
        case NPCType::Guard:
            // TODO:
            break;
		}
	}
}

void NPC::Randomize()
{
	THROW_IF_EXPIRED(mLocation);
	const auto location = mLocation.lock();
	const auto scene = location->GetScene();
	mInventory = make_shared<Inventory>(shared_from_this());
	const auto &db = game->GetItemDatabase();

	{
		mDialogs.push_back(make_shared<BadReputationDialog>(shared_from_this()));
		mDialogs.push_back(make_shared<FirstWarningDialog>(shared_from_this()));
		mDialogs.push_back(make_shared<KickOutOfRoomDialog>(shared_from_this()));
		mDialogs.push_back(make_shared<IllegalZoneDialog>(shared_from_this()));
		mDialogs.push_back(make_shared<ApologizeDialog>(shared_from_this()));

		switch (mType)
		{
		case NPCType::Local:		
			mDialogs.push_back(make_shared<LocalTalkDialog>(shared_from_this()));
			break;
		case NPCType::Merchant:			
			mDialogs.push_back(make_shared<MerchantTalkDialog>(shared_from_this()));
			break;
		case NPCType::Blacksmith:			
			mDialogs.push_back(make_shared<BlacksmithDialog>(shared_from_this()));
			break;
		case NPCType::Thief:			
			mDialogs.push_back(make_shared<ThiefTalkDialog>(shared_from_this()));
			break;
		case NPCType::Guard:
			break;
		}
	}

	// add potions
	Bundle<ItemType> potions = {
		{ 0.6f, ItemType::Potion_WeakHeal },
		{ 0.3f, ItemType::Potion_MediumHeal },
		{ 0.1f, ItemType::Potion_StrongHeal }
	};

	for (int i = 0; i < 6; ++i)
	{
		mInventory->AddItem(db->Make(potions.Drop(), location));
	}

	mInventory->AddMoney(Math::Rnd(150, 250));

	// create random weapon
	Bundle<ItemType> weapons = {
		{ 0.5f, ItemType::Weapon_Shotgun },
		{ 0.25f, ItemType::Weapon_Drill },
		{ 0.1f, ItemType::Weapon_RocketLauncher },
		{ 0.1f, ItemType::Weapon_GrenadeLauncher },
		{ 0.05f, ItemType::Weapon_PlasmaDrill }
	};

	auto wpnType = weapons.Drop();
	auto wpnDesc = db->Get(wpnType);
	auto wpn = db->Make(wpnType, location);
	mInventory->AddItem(wpn);
	UseItem(wpn, Hand::Left);

	// add melee weapon
	mInventory->AddItem(db->Make(ItemType::Weapon_Pickaxe, location));

	// add consumables
	auto consumableCount = Math::Rnd(10, 30);
	for (int i = 0; i < consumableCount; ++i)
	{
		mInventory->AddItem(db->Make(wpnDesc.mConsumable, location));
	}

	auto resourceDatabase = game->GetResourceDatabase();

	if (auto beard = dynamic_pointer_cast<Sprite>(mBeard))
	{		
		beard->SetTexture(resourceDatabase->GetRandom(BodyPartTemplateType::Beard)->mTexture);
		beard->SetTextureRectangle({ 0, 0, 1, 1 });

		beard->SetColor(Math::RndOpaqueColor(100, 255));
	}

	if (auto hair = dynamic_pointer_cast<Sprite>(mHair))
	{
		hair->SetTexture(resourceDatabase->GetRandom(BodyPartTemplateType::Hair)->mTexture);
		hair->SetTextureRectangle({ 0, 0, 1, 1 });

		hair->SetColor(Math::RndOpaqueColor(100, 255));
	}

	if (auto head = dynamic_pointer_cast<Sprite>(mHead))
	{
		head->SetTexture(resourceDatabase->GetRandom(BodyPartTemplateType::Head)->mTexture);
		head->SetTextureRectangle({ 0, 0, 1, 1 });
	}

	// Give armor
	switch (mType)
	{
	case NPCType::Merchant:
		AddAndUseItem(db->Make(ItemType::Armor_LeatherArmor, location), Hand::Left);
		AddAndUseItem(db->Make(ItemType::Gloves_LeatherGloves, location), Hand::Left);
		AddAndUseItem(db->Make(ItemType::Trousers_LeatherTrousers, location), Hand::Left);
		AddAndUseItem(db->Make(ItemType::Helmet_IronHelmet, location), Hand::Left);
		break;
		// fall-thru
	case NPCType::Local:
	case NPCType::Thief:
		AddAndUseItem(db->Make(ItemType::Armor_LightLeatherArmor, location), Hand::Left);
		AddAndUseItem(db->Make(ItemType::Gloves_LightLeatherGloves, location), Hand::Left);
		AddAndUseItem(db->Make(ItemType::Trousers_LightLeatherTrousers, location), Hand::Left);
		AddAndUseItem(db->Make(ItemType::Helmet_IronHelmet, location), Hand::Left);
		break;
		// fall-thru
	case NPCType::Guard:
	case NPCType::Blacksmith:
		AddAndUseItem(db->Make(ItemType::Armor_SteelArmor, location), Hand::Left);
		AddAndUseItem(db->Make(ItemType::Gloves_SteelGloves, location), Hand::Left);
		AddAndUseItem(db->Make(ItemType::Trousers_SteelTrousers, location), Hand::Left);
		AddAndUseItem(db->Make(ItemType::Helmet_SteelHelmet, location), Hand::Left);
		break;
	}

	if (mType == NPCType::Blacksmith)
	{
		mInventory->AddItem(db->Make(ItemType::Blueprint_MetalCopper, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_MetalGold, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_MetalIron, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_MetalPlatinum, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_MetalSilver, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_MetalTin, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_MetalTungsten, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_GrenadeLauncher, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_Rocket, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_Rocket, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_Rocket, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_HarpoonProj, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_AcidBlob, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_Rail, location));
		int count = Math::Rnd(100, 200);
		for (int i = 0; i < count; ++i)
		{
			mInventory->AddItem(db->Make(ItemType::Projectile_Shells, location));
		}
	}
	else if (mType == NPCType::Merchant)
	{
		int count = Math::Rnd(20, 35);
		for (int i = 0; i < count; ++i)
		{
			mInventory->AddItem(db->Make(ItemType::Potion_WeakHeal, location));
		}
		count = Math::Rnd(16, 20);
		for (int i = 0; i < count; ++i)
		{
			mInventory->AddItem(db->Make(ItemType::Potion_MediumHeal, location));
		}
		count = Math::Rnd(8, 16);
		for (int i = 0; i < count; ++i)
		{
			mInventory->AddItem(db->Make(ItemType::Potion_StrongHeal, location));
		}
		mInventory->AddItem(db->Make(ItemType::Blueprint_Rocket, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_Rocket, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_Rocket, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_HarpoonProj, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_AcidBlob, location));
		mInventory->AddItem(db->Make(ItemType::Blueprint_Rail, location));

		if (Math::ProbabilityEventAppeared(0.1f))
		{
			mInventory->AddItem(db->Make(ItemType::Book_DiscoveryOfArkalith, location));
		}
		if (Math::ProbabilityEventAppeared(0.1f))
		{
			mInventory->AddItem(db->Make(ItemType::Book_EpochBeforeArkalith, location));
		}
		if (Math::ProbabilityEventAppeared(0.1f))
		{
			mInventory->AddItem(db->Make(ItemType::Book_Seasons, location));
		}
	}

	// NPC level can vary in wide range, but it's always at least same as base level of the location
	SetLevel(location->GetDefinition()->GetBaseLevel() + Math::Rnd(0, 10));
}

void NPC::ApplyDamage(Damage & dmg)
{

	auto target = mTargetActor.lock();
	auto player = dynamic_pointer_cast<Player>(dmg.mWho);

	if (player)
	{
		if (target != player)
		{
			player->GetStats()->ChangeReputation(-0.5f);
			game->GetHUD()->AddNotification(StringBuilder() << game->GetStr("reputationDecreased") << 0.5);
			if (mWarningsForPlayer > 0)
			{
				BeginDialogOfType<FirstWarningDialog>();				
			}
			--mWarningsForPlayer;
		}
	}

	SetTarget(dmg.mWho);

	Dwarf::ApplyDamage(dmg);
}

void NPC::SetPosition(const Vec2f &p)
{
	Dwarf::SetPosition(p);
}

void NPC::SetDefaultPosition(const Vec2f &p)
{
	mDefaultPosition = p;
}

void NPC::SetRoom(const shared_ptr<Room> &room)
{
	mRoom = room;
}

shared_ptr<Room> NPC::GetRoom() const
{
	return mRoom.lock();
}

bool NPC::IsBusy()
{
	return !mTargetActor.expired();
}

void NPC::ChillOut()
{
	mTargetActor.reset();
}

bool NPC::IsAggressiveToPlayer()
{
	return mTargetActor.lock() == game->GetLevel()->GetPlayer();
}

void NPC::SetPatrolPoints(const Vec2f & ptBegin, const Vec2f & ptEnd)
{
	mDefaultPosition = ptBegin;
	mPatrolBegin = ptBegin;
	mPatrolEnd = ptEnd;
	SetMoveDestinationPatrolBegin();
}

void NPC::OnSetLevel()
{
	const auto characterLevel = static_cast<float>(mStats->GetLevel());

	// Stat adjustment coefficients
	constexpr float kVit = 0.70f;
	constexpr float kStr = 0.30f;
	constexpr float kDex = 0.20f;
	constexpr float kEnd = 0.20f;
	constexpr float kInt = 0.30f;
	constexpr float kLck = 0.20f;
	constexpr float kChr = 0.20f;

	mStats->Vitality.Set(static_cast<int>(ceil(characterLevel * kVit)));
	mStats->Strenght.Set(static_cast<int>(ceil(characterLevel * kStr)));
	mStats->Dexterity.Set(static_cast<int>(ceil(characterLevel * kDex)));
	mStats->Endurance.Set(static_cast<int>(ceil(characterLevel * kEnd)));
	mStats->Intelligence.Set(static_cast<int>(ceil(characterLevel * kInt)));
	mStats->Luck.Set(static_cast<int>(ceil(characterLevel * kLck)));
	mStats->Charisma.Set(static_cast<int>(ceil(characterLevel * kChr)));
}

void NPC::MakeHostile()
{
	mTargetActor = game->GetLevel()->GetPlayer();
}

bool NPC::IsHostile() const
{
	return !mTargetActor.expired();
}


