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
#include "Actor.h"
#include "Weapon.h"
#include "ItemDatabase.h"
#include "Cell.h"
#include "Level.h"
#include "Stats.h"
#include "Inventory.h"
#include "Player.h"
#include "Shield.h"
#include "Terrain.h"
#include "Text.h"
#include "Scene.h"
#include "Renderer.h"
#include "NPC.h"
#include "Debris.h"
#include "Boss.h"
#include "PolyLine.h"
#include "Animation.h"
#include "AnimationTrack.h"
#include "HUD.h"
#include "Door.h"
#include "Damage.h"
#include "Dialog.h"

void Actor::ApplyEquippedItemDamageReduction(const shared_ptr<Item> &item, DamageClass damageClass, float &count)
{
	if (!item)
	{
		return;
	}

	// Broken items has no effect on defenses
	if (item->IsBroken())
	{
		return;
	}

	count = item->GetDefinition().ReduceDamage(count, damageClass);
}

void Actor::OnDie()
{
}

void Actor::ResetHandEquipmentConsumable(const Hand & hand)
{
	mHandEquipmentConsumables[static_cast<int>(hand)].reset();
}

bool Actor::IsInStunLock() const
{
	return mStunLockInterval > 0;
}

void Actor::SetName(const string &name)
{
	mName = name;
	UpdateNameLabel();
}

bool Actor::IsOutOfWeaponConsumables(ItemType weaponType, const Hand & hand) const
{
	ItemDatabase* db = game->GetItemDatabase();
	auto &desc = db->Get(weaponType);

	if (desc.mClass != ItemClass::Weapon)
	{
		return true;
	}

	if (desc.mIsMeleeWeapon)
	{
		return false;
	}

	if (mHandEquipmentConsumables[static_cast<int>(hand)])
	{
		if (mHandEquipmentConsumables[static_cast<int>(hand)]->GetDefinition().mType == desc.mConsumable)
		{
			return false;
		}
	}

	if (!mInventory->FindGroup(desc.mConsumable))
	{
		return true;
	}

	return false;
}

bool Actor::IsSeeActor(const shared_ptr<Actor> &actor) const
{
	return mViewTriangle.IsIntersectCircle(actor->GetPosition(), actor->GetBody()->GetRadius());
}

void Actor::UpdateHitBoxes()
{
	for (auto &hbt : mDebugHitboxes)
	{
		hbt->RemoveFromScene();
	}
	mDebugHitboxes.clear();

	mHitBoxes.clear();
	UpdateHitBoxesRecursively(mPivot);
}

HitBox *Actor::FindHitbox(const shared_ptr<SceneNode> &node)
{
	for (auto &hb : mHitBoxes)
	{
		if (hb.RelatesTo(node))
		{
			return &hb;
		}
	}
	return nullptr;
}

const vector<HitBox> &Actor::GetHitBoxes() const
{
	return mHitBoxes;
}

bool Actor::IsAnyHitBoxIntersected(const Vec2f &p, float r) const
{
	for (auto &hb : mHitBoxes)
	{
		if (hb.IsIntersects(p, r))
		{
			return true;
		}
	}
	return false;
}

bool Actor::IsAnyHitBoxIntersected(const Math::RotatedRectangle &r) const
{
	for (auto &hb : mHitBoxes)
	{
		if (hb.IsIntersects(r))
		{
			return true;
		}
	}
	return false;
}

bool Actor::IsAnyHitBoxIntersected(const HitBox & other) const
{
	for (auto &hb : mHitBoxes)
	{
		if (hb.IsIntersects(other))
		{
			return true;
		}
	}
	return false;
}

void Actor::ChangeReputation(float repChange)
{
	mStats->ChangeReputation(repChange);
}

float Actor::GetEquipmentDamageReduction(const DamageClass &dmgClass)
{
	// Store raw pointers to remove unnecessary copying of smart pointers
	const Item *equipment[] = { mHelmet.get(), mChestArmor.get(), mGloves.get(), mTrousers.get(),
		mRings[0].get(), mRings[1].get(), mRings[2].get(), mRings[3].get() };

	float reduction = 0.0f;

	for (const auto &item : equipment)
	{
		if (!item)
		{
			continue;
		}
		const auto &desc = item->GetDefinition();
		switch (dmgClass)
		{
		case DamageClass::Physical:
			reduction += desc.mPhysicalDamageReduction;
			break;
		case DamageClass::Fire:
			reduction += desc.mFireDamageReduction;
			break;
		case DamageClass::Chemical:
			reduction += desc.mChemicalDamageReduction;
			break;
		case DamageClass::Electrical:
			reduction += desc.mElectricalDamageReduction;
			break;
		}
	}

	return reduction;
}

void Actor::OnSetLevel()
{
}

void Actor::SetLevel(int level)
{
	mStats->SetLevel(level);
	UpdateNameLabel();
	OnSetLevel();
}

shared_ptr<Shield> Actor::GetEquippedShield() const
{
	for (auto & wpnSlot : mHandSlots)
	{
		if (auto shield = dynamic_pointer_cast<Shield>(wpnSlot))
		{
			return shield;
		}
	}
	return nullptr;
}

bool Actor::IsEquippedShieldRaised() const
{
	auto shield = GetEquippedShield();
	if (!shield)
	{
		return false;
	}
	return shield->IsRaised();
}

const array<shared_ptr<Item>, 2>& Actor::GetHandSlots() const
{
	return mHandSlots;
}

Hand Actor::GetHoldingHand(const shared_ptr<Item> & item) const
{
	if (mHandSlots[0] == item)
	{
		return Hand::Left;
	}
	else if (mHandSlots[1] == item)
	{
		return Hand::Right;
	}
	else
	{
		throw Exception("Invalid hand!");
	}
}

bool Actor::IsHaveBadReputation() const
{
	return mStats->GetReputation() < 0;
}

bool Actor::OpenDoor(const shared_ptr<Door>& door)
{
	// Try to open without a key
	bool opened = door->Open(nullptr);
	if (!opened)
	{
		if (door->IsHasKey())
		{
			// Try each key
			for (auto & group : mInventory->GetGroups())
			{
				if (group->GetAny()->GetDefinition().mClass != ItemClass::Key)
				{
					continue;
				}
				for (auto & key : group->mItems)
				{
					if (door->Open(key))
					{
						opened = true;
						goto end;
					}
				}
			}
		}
	}
end:
	return opened;
}

void Actor::CloseDoor(const shared_ptr<Door>& door, bool lock)
{
	if (door->IsHasKey() && lock)
	{
		// Try each key
		for (auto & group : mInventory->GetGroups())
		{
			if (group->GetAny()->GetDefinition().mClass != ItemClass::Key)
			{
				continue;
			}
			for (auto & key : group->mItems)
			{
				if (door->Close(key))
				{
					return;
				}
			}
		}
	}
	else
	{
		door->Close(nullptr);
	}
}

void Actor::UnlockDoor(const shared_ptr<Door>& door)
{
	if (door->IsHasKey())
	{
		// Try each key
		for (auto & group : mInventory->GetGroups())
		{
			if (group->GetAny()->GetDefinition().mClass != ItemClass::Key)
			{
				continue;
			}
			for (auto & key : group->mItems)
			{
				door->Unlock(key);
			}
		}
	}
}

const Guid & Actor::GetIdentifier() const
{
	return mIdentifier;
}

const string & Actor::GetName() const
{
	return mName;
}

void Actor::SetAnimation(const shared_ptr<Animation> &animation)
{
	for (auto &other : mAnimations)
	{
		if (other != animation)
		{
			other->FadeOut(0.15f);
		}
	}
	animation->FadeIn(0.15f);
	animation->SetEnabled(true);
}

void Actor::SetAnimationEnabledForNode(const shared_ptr<SceneNode> &node, bool state, bool affectChildren)
{
	for (auto &animation : mAnimations)
	{
		if (auto track = animation->FindAssociatedTrack(node))
		{
			track->SetEnabled(state);
		}
	}
	if (affectChildren)
	{
		for (auto &child : node->GetChildren())
		{
			SetAnimationEnabledForNode(child, state);
		}
	}
}

void Actor::MakeStunLock()
{
	mStunLockInterval = MaxStunLockTimeout;
	// at least MaxStunLockTimeout actor can do damage, this prevents infinite
	// stunlock on continuous types of damage
	mNextPossibleStunLockTimeout = 2 * MaxStunLockTimeout;

	GetLocation()->ReportMessage(
		game->GetStr("stunlock"), GetPosition() - Vec2f(45, 30), { 255, 255, 0 });
}

void Actor::UpdateHitBoxesRecursively(const shared_ptr<SceneNode> &entry)
{
	// Ignore health signs and name text
	if (entry == mHealSign || entry == mHealthBar || entry == mNameText ||
		entry == mStunLockIndicator)
	{
		return;
	}

	// Ignore weapons
	for (auto wpnSlot : mHandSlots)
	{
		if (wpnSlot && wpnSlot->GetNode() == entry)
		{
			return;
		}
	}

	if (auto sprite = dynamic_pointer_cast<Sprite>(entry))
	{
		mHitBoxes.emplace_back(sprite);
		// Debug
		if (game->Debug.showHitboxes)
		{
			auto polyline = entry->GetScene()->CreatePolyLine();
			polyline->SetThickness(1.5f);
			polyline->SetColor({ 0, 255, 0, 80 });
			for (auto &p : mHitBoxes.back().GetRectangle().points)
			{
				polyline->AddPoint(p);
			}
			polyline->AddPoint(mHitBoxes.back().GetRectangle().points[0]);
			mDebugHitboxes.push_back(polyline);
		}
	}

	for (auto &child : entry->GetChildren())
	{
		UpdateHitBoxesRecursively(child);
	}
}

void Actor::UpdateNameLabel()
{
	mNameText->SetText(mName + "\n" + game->GetStr("actorLevel") + to_string(mStats->GetLevel()));
}

void Actor::SetHandEqupment(const Hand & hand, const shared_ptr<Item> & item)
{
	if (hand == Hand::Left)
	{
		mHandSlots[0] = item;
	}
	else
	{
		mHandSlots[1] = item;
	}
}

void Actor::HandleFallingDamage(const Contact & contact)
{
	const float feetPosition = GetFeetPosition().y;
	const float contactPosition = contact.mPosition.y;

	if (contactPosition >= feetPosition)
	{
		if (contact.mPosition.y - mLastFeetPosition.y > 140)
		{
			Damage dmg;
			dmg.mClass = DamageClass::Physical;
			dmg.mCount = 30;
			dmg.mFlags = DMG_UNBLOCKABLE;
			ApplyDamage(dmg);
			MakeStunLock();
		}

		mLastFeetPosition = GetFeetPosition();
	}
}

Vec2f Actor::GetFeetPosition()
{
	return Vec2f(GetPosition().x, GetPosition().y + GetBody()->GetRadius() * 0.9f);
}

Actor::Actor()
{
	mStats = make_unique<Stats>(this);
	mLastTime = chrono::steady_clock::now();
}

Actor::Actor(const shared_ptr<Location> &location, float bodyRadius) : mLocation(location)
{
	mIdentifier = Guid::Generate();

	mStats = make_unique<Stats>(this);
	mLastTime = chrono::steady_clock::now();

	const auto &scene = location->GetScene();
	const auto renderer = game->GetRenderer();

	mPivot = scene->CreateNode();
	auto body = scene->GetSimulationIsland()->CreateParticle(bodyRadius);
	body->SetFriction(0.93f);
	body->SetFlags(BF_PUSHER);
	body->SetCollisionGroup(CG_ACTOR);
	// Actor-actor collision is disabled
	body->SetCollisionMask(CG_UNIVERSAL & (~CG_ACTOR));
	mPivot->SetBodyAtCurrentPosition(body);

	mHealthBar = scene->CreateSprite();

	mHealthBar->SetSize({ 40, 6 });
	mHealthBar->SetColor({ 200, 0, 0 });
	mHealthBar->AttachTo(mPivot);

	mNameText = scene->CreateText();
	mNameText->AttachTo(mPivot);
	mNameText->SetFont(renderer->GetFont("data/fonts/font.ttf"));
	mNameText->SetCharacterSize(12);
	mNameText->SetAlignment(TextAlignment::Center);
	mNameText->SetSize({ 100, 20 });
	mNameText->SetLocalOrigin(mNameText->GetSize().x / 2, 0);

	mStunLockIndicator = scene->CreateSprite();
	mStunLockIndicator->SetTexture(renderer->RequestTexture("data/textures/gui/stunlock.png"));
	mStunLockIndicator->SetSize({ 9, 9 });
	mStunLockIndicator->AttachTo(mPivot);
	mStunLockIndicator->SetOriginInCenter();

	mHealSign = scene->CreateSprite();
	mHealSign->SetTexture(renderer->RequestTexture("data/textures/gui/healsign.png"));
	mHealSign->SetSize({ 8, 10 });
	mHealSign->AttachTo(mPivot);
	mHealSign->SetOriginInCenter();

	ArrangeIndicators();
}

void Actor::ArrangeIndicators()
{
	auto origin = -1.5f * mPivot->GetBody()->GetRadius();
	mHealthBar->SetLocalPosition({ 0, origin });

	origin -= 26;
	mNameText->SetLocalPosition({ 0, origin });

	origin -= 5;
	if (mHealSign->IsVisible() && mStunLockIndicator->IsVisible())
	{
		mHealSign->SetLocalPosition({ -mHealSign->GetSize().x, origin });
		mStunLockIndicator->SetLocalPosition({ mStunLockIndicator->GetSize().x, origin });
	}
	else
	{
		mHealSign->SetLocalPosition({ 0, origin });
		mStunLockIndicator->SetLocalPosition({ 0, origin });
	}
}

Actor::~Actor()
{
	if (mPivot)
	{
		mPivot->RemoveFromScene();
	}
	for (auto &hbt : mDebugHitboxes)
	{
		hbt->RemoveFromScene();
	}
	if (auto location = mLocation.lock())
	{
		for (auto &anim : mAnimations)
		{
			location->GetScene()->RemoveAnimation(anim);
		}
	}
	DBG_ECHO();
}

shared_ptr<Location> Actor::GetLocation()
{
	return mLocation.lock();
}

void Actor::Think()
{
}

void Actor::SetLocation(const shared_ptr<Location> &location)
{
	mLocation = location;
	if (mInventory)
	{
		for (auto &group : mInventory->GetGroups())
		{
			for (auto &item : group->mItems)
			{
				location->GetScene()->AcceptNode(item->GetNode());
			}
		}
	}
}

shared_ptr<Body> Actor::GetBody() const
{
	return mPivot->GetBody();
}

void Actor::SetPosition(const Vec2f &p)
{	
	mPivot->GetBody()->SetPosition(p);
	mLastFeetPosition = GetFeetPosition();
}

Vec2f Actor::GetPosition() const
{
	return mPivot->GetBody()->GetPosition();
}

void Actor::AddForce(const Vec2f &v)
{
	mPivot->GetBody()->AddForce(v);
}

void Actor::Serialize(Serializer &sav)
{
	sav &mInventory;
	sav &mLocation;
	sav &mPivot;
	for (auto & wpnSlot : mHandSlots)
	{
		sav &wpnSlot;
	}
	for (auto & cons : mHandEquipmentConsumables)
	{
		sav &cons;
	}
	sav &mHelmet;
	sav &mGloves;
	sav &mTrousers;
	sav &mChestArmor;
	sav &mNameText;
	sav &mHealthBar;
	sav &mStunLockIndicator;
	sav &mConsumedDamage;
	sav &mStunLockInterval;
	sav &mNextPossibleStunLockTimeout;
	sav &mName;
	sav &mHealSign;
	sav &mHealSignTimer;
	sav &mViewTriangle;
	sav &mAnimations;
	sav &mModelRoot;
	sav &mIdentifier;
	sav &mLastFeetPosition;	
	for (auto &ring : mRings)
	{
		sav &ring;
	}
	mStats->Serialize(sav);
	sav & mDialogs;
}

shared_ptr<Inventory> Actor::GetInventory() const
{
	return mInventory;
}

const unique_ptr<Stats> &Actor::GetStats() const
{
	return mStats;
}

void Actor::ApplyDamage(Damage & dmg)
{
	if (dmg.mClass == DamageClass::Unknown)
	{
		dmg.mCount = 0.0f;
		return;
	}

	if (IsDead())
	{
		dmg.mCount = 0.0f;
		return;
	}

	bool damageDodged = false;
	if (mNextPossibleDodgeTimeout <= 0)
	{
		if (Math::ProbabilityEventAppeared(mStats->GetDodgeChance()))
		{
			if (!(dmg.mFlags & DMG_UNBLOCKABLE))
			{
				dmg.mCount = 0.0f;
				damageDodged = true;

				mNextPossibleDodgeTimeout = DodgeTimeout;
			}
		}
	}

	float kStamina = Math::Clamp(dmg.mCount, 0.0f, ShieldBlockMaxDamage) / ShieldBlockMaxDamage;
	float baseStaminaConsumption = ShieldMaxStaminaConsumption * kStamina;

	// Handle any raised shield
	bool damageBlockable = (dmg.mFlags & DMG_UNBLOCKABLE) == 0;
	bool damageBlocked = false;

	if (auto shield = GetEquippedShield())
	{
		if (shield->IsRaised() && damageBlockable)
		{
			//auto & hitbox = shield->GetHitbox();



			GetLocation()->GetScene()->EmitSound("data/sounds/metal_shield_impact.ogg", shield->GetNode());

			float reduction = 1.0f - shield->GetDefinition().mStability;
			if (mStats->ConsumeStamina(baseStaminaConsumption * reduction))
			{
				// Full block
				dmg.mCount = shield->GetDefinition().ReduceDamage(dmg.mCount, dmg.mClass);
				damageBlocked = true;
			}
			else
			{
				// Partial block
				float k = (mStats->mStamina / baseStaminaConsumption);
				mStats->ConsumeStamina(mStats->mStamina);
				switch (dmg.mClass)
				{
				case DamageClass::Physical:
					dmg.mCount *= 1.0f - k * shield->GetDefinition().mPhysicalDamageReduction;
					break;
				case DamageClass::Fire:
					dmg.mCount *= 1.0f - k * shield->GetDefinition().mFireDamageReduction;
					break;
				case DamageClass::Chemical:
					dmg.mCount *= 1.0f - k * shield->GetDefinition().mChemicalDamageReduction;
					break;
				case DamageClass::Electrical:
					dmg.mCount *= 1.0f - k * shield->GetDefinition().mElectricalDamageReduction;
					break;
				}
				// Partial block gives stunlock to actor
				if (mNextPossibleStunLockTimeout <= 0)
				{
					MakeStunLock();
				}
			}
		}
	}

	// Handle actor's stats
	dmg.mCount = mStats->ReduceDamage(dmg.mCount, dmg.mClass);

	// Apply equipment effects
	vector<shared_ptr<Item>> equipment = { mHelmet, mGloves, mTrousers, mChestArmor };
	equipment.insert(equipment.end(), mRings.begin(), mRings.end());
	for (auto &item : equipment)
	{
		ApplyEquippedItemDamageReduction(item, dmg.mClass, dmg.mCount);
	}

	// If actor wearing armor, then damage will wear out it
	Item *armor[] = { mHelmet.get(), mGloves.get(), mTrousers.get(), mChestArmor.get() };
	for (auto &armorElement : armor)
	{
		if (armorElement)
		{
			auto &desc = armorElement->GetDefinition();
			// Higher damage reduction coefficient results in more damage to
			// durability of an equipment
			const float maxItemDamagePerHit = 10.0f;
			const float maxDamage = 200.0f;
			float k = dmg.mCount / maxDamage;
			float averageDefense = (desc.mPhysicalDamageReduction + desc.mChemicalDamageReduction + desc.mFireDamageReduction) / 3.0f;
			armorElement->WearOut(k * averageDefense * maxItemDamagePerHit);
		}
	}

	// Probably this not best place for counting frags for player :)
	if (auto player = dynamic_pointer_cast<Player>(dmg.mWho))
	{
		if (mStats->mHealth - dmg.mCount <= 0)
		{
			player->AddKill();
		}
	}

	bool willBeDead = mStats->mHealth - dmg.mCount <= 0;

	if (willBeDead)
	{
		Explode();
		// Add expirience to a killer
		// Check who, cuz damage can come from environment
		if (dmg.mWho)
		{
			auto &stats = dmg.mWho->GetStats();
			stats->AddExpirience(mStats->GetExpirienceDrop());
		}
	}
	// display damage
	bool enoughInterval = false;
	auto now = chrono::steady_clock::now();
	if (chrono::duration_cast<chrono::milliseconds>(now - mLastTime).count() > 300)
	{
		enoughInterval = true;
		mLastTime = now;
		if (damageDodged)
		{
			GetLocation()->ReportMessage(game->GetStr("dodge"), GetPosition() - Vec2f(45, 30), { 0, 255, 0 });
		}
		else if (damageBlocked)
		{
			GetLocation()->ReportMessage(game->GetStr("damageBlocked"), GetPosition() - Vec2f(45, 30), { 0, 255, 0 });
		}
		else
		{
			mConsumedDamage += abs(dmg.mCount);
			char buffer[256];
			sprintf(buffer, mConsumedDamage < 1 ? "%.1f" : "%.0f", mConsumedDamage);
			GetLocation()->ReportMessage(buffer, GetPosition() - Vec2f(30, 30));
			mConsumedDamage = 0.0f;
		}
	}

	// handle stunlock
	if (mNextPossibleStunLockTimeout <= 0)
	{
		mStunLockConsumedDamage += dmg.mCount;
		// this routine gives around of 5 stun-locks, probably this is wrong
		// todo: try to find a better solution
		if (mStunLockConsumedDamage > mStats->mMaxHealth.Get() * 0.20f)
		{
			MakeStunLock();
			mStunLockConsumedDamage = 0;
		}
	}

	// change reputation of killer
	if (dmg.mWho)
	{
		if (GetStats()->GetHealth() - dmg.mCount < 0)
		{
			float repChange = 0;
			if (dynamic_pointer_cast<NPC>(shared_from_this()))
			{
				repChange = -20;
			}
			else if (dynamic_pointer_cast<Boss>(shared_from_this()))
			{
				repChange = 5;
			}
			else if (dynamic_pointer_cast<Creature>(shared_from_this()))
			{
				repChange = 0.25;
			}
			dmg.mWho->ChangeReputation(repChange);
		}
	}

	// finally apply modified damage to stats
	mStats->Damage(dmg.mCount);

	if (dmg.mCount > 0 && enoughInterval)
	{
		const auto scene = GetLocation()->GetScene();

		switch (dmg.mClass)
		{
		case DamageClass::Physical:
			scene->EmitSound("data/sounds/flesh_impact2.ogg", GetPosition());
			break;
		case DamageClass::Fire:
			scene->EmitSound("data/sounds/hot_ground.ogg", GetPosition());
			break;
		case DamageClass::Chemical:
			scene->EmitSound("data/sounds/acid_ground.ogg", GetPosition());
			break;
		}
	}
}

void Actor::Update()
{
	if (IsDead())
	{
		if (GetBody())
		{
			GetBody()->SetVelocity({ 0, GetBody()->GetVelocity().y });
		}
	}
	else
	{


		// Flags indicate that such type of damage is recieved and no more such
		// damage need to be applied
		bool acidDamage = false;
		bool lavaDamage = false;
		for (int i = 0; i < mPivot->GetBody()->GetContactCount(); ++i)
		{
			auto &contact = mPivot->GetBody()->GetContact(i);
			
			if (contact.mCell)
			{
				HandleFallingDamage(contact);

				Damage dmg;
				if (contact.mPixel.a & PA_ACID)
				{
					if (!acidDamage)
					{
						dmg.mCount = 0.1f;
						dmg.mClass = DamageClass::Chemical;
						dmg.mFlags = DMG_UNBLOCKABLE;						
						acidDamage = true;
					}
				}
				if (contact.mPixel.a & PA_LAVA)
				{
					if (!lavaDamage)
					{
						dmg.mCount = 1.5f;
						dmg.mClass = DamageClass::Fire;
						dmg.mFlags = DMG_UNBLOCKABLE;
						lavaDamage = true;
					}
				}
				if (dmg.mCount != 0.0f)
				{
					ApplyDamage(dmg);
				}
			}
		}

		mStats->Update(IsEquippedShieldRaised());

		UpdateHitBoxes();
	}

	mStunLockIndicator->Turn(10.0f);
	mStunLockIndicator->SetVisible(IsInStunLock());
	--mStunLockInterval;
	--mNextPossibleStunLockTimeout;
	--mNextPossibleDodgeTimeout;
	if (mHealSign)
	{
		--mHealSignTimer;
		mHealSign->SetVisible(mHealSignTimer > 0 || mStats->IsHealing());
	}

	ArrangeIndicators();
}

bool Actor::IsDead() const
{
	return mStats->IsDead();
}

void Actor::Explode()
{
	const auto location = GetLocation();
	const auto renderer = game->GetRenderer();
	Vec2f min, max;
	// Render actor to texture
	const auto texture = renderer->RenderToTexture(mPivot, &min, &max);
	// Debug
	if (game->Debug.dumpExplodeTexture)
	{
		sf::Image img;
		img.create(texture->GetWidth(), texture->GetHeight(), (sf::Uint8 *)texture->GetPixels());
		img.saveToFile("explode.png");
	}
	// Break texture into pieces
	const Vec2f size = max - min;
	const int pixCount = texture->GetWidth() * texture->GetHeight();
	const sf::Color *pixels = texture->GetPixels();
	const int w = static_cast<int>(size.x);
	const int h = static_cast<int>(size.y);
	const int debrisSize = Debris::Radius * 2;
	const float dx = debrisSize / size.x;
	const float dy = debrisSize / size.y;
	for (int y = 0; y < h; y += debrisSize)
	{
		for (int x = 0; x < w; x += debrisSize)
		{
			const int destX = x + debrisSize;
			const int destY = y + debrisSize;
			// Check each pixel of debris piece
			for (int sy = y; sy < destY; ++sy)
			{
				const int row = sy * w;
				for (int sx = x; sx < destX; ++sx)
				{
					auto index = row + sx;
					if (index >= pixCount)
					{
						continue;
					}
					// If any pixel is visible
					if (pixels[index].a != 0)
					{
						// Create debris piece
						const float relX = x / size.x;
						const float relY = y / size.y;
						const Vec2f p = { min.x + x, min.y + y };
						const sf::FloatRect texRect = { relX, relY, dx, dy };
						auto debris = make_shared<Debris>(location, p, texture, texRect);
						// We have to disable collision between actor and it's debris
						debris->GetBody()->SetCollisionMask(CG_UNIVERSAL & (~CG_ACTOR));
						location->AddDebris(debris);
						// Jump over nested loops
						goto nextPiece;
					}
				}
			}
		nextPiece:;
		}
	}
}

shared_ptr<SceneNode> Actor::GetPivot()
{
	return mPivot;
}

void Actor::UseItem(const shared_ptr<Item> &item, const Hand & hand)
{
	UnequipItem(item);
	if (item)
	{
		auto &desc = item->GetDefinition();
		const auto iclass = desc.mClass;
		const auto itype = item->GetType();
		if (iclass == ItemClass::Weapon)
		{
			if (auto prevWeapon = dynamic_pointer_cast<Weapon>(GetHandEquipment(hand)))
			{
				prevWeapon->SetVisible(false);
			}
			if (auto lastShield = dynamic_pointer_cast<Shield>(GetHandEquipment(hand)))
			{
				lastShield->Raise(false);
			}
			if (auto wpn = dynamic_pointer_cast<Weapon>(item))
			{
				wpn->SetOwner(shared_from_this());
				SetHandEqupment(hand, item);
				wpn->SetVisible(true);
			}
		}

		if (iclass == ItemClass::Projectile)
		{
			mHandEquipmentConsumables[static_cast<int>(hand)] = item;
		}
		else if (iclass == ItemClass::Crystal)
		{
			mHandEquipmentConsumables[static_cast<int>(hand)] = item;
		}

		if (iclass == ItemClass::Helmet)
		{
			mHelmet = item;
		}
		else if (iclass == ItemClass::Armor)
		{
			mChestArmor = item;
		}
		else if (iclass == ItemClass::Trousers)
		{
			mTrousers = item;
		}
		else if (iclass == ItemClass::Gloves)
		{
			mGloves = item;
		}
		else if (iclass == ItemClass::Shield)
		{
			if (auto shield = dynamic_pointer_cast<Shield>(item))
			{
				if (auto prevWeapon = dynamic_pointer_cast<Weapon>(GetHandEquipment(hand)))
				{
					prevWeapon->SetVisible(false);
				}
				if (auto lastShield = dynamic_pointer_cast<Shield>(GetHandEquipment(hand)))
				{
					lastShield->Raise(false);
				}
				shield->GetNode()->AttachTo(mPivot);
				SetHandEqupment(hand, item);
			}
		}
		else if (iclass == ItemClass::Potion)
		{
			if (desc.mHeal > 0.0f)
			{
				mStats->Heal(desc.mHeal, desc.mHealFrames);
				mInventory->RemoveItem(item);
			}
			if (desc.mStaminaRestoration > 0.0f)
			{
				mStats->RestoreStamina(desc.mStaminaRestoration, desc.mStaminaRestorationFrames);
				mInventory->RemoveItem(item);
			}
		}
		else if (iclass == ItemClass::Ring)
		{
			for (auto &ring : mRings)
			{
				if (!ring)
				{
					ring = item;
					if (itype == ItemType::Ring_RingOfLife)
					{
						mStats->mMaxHealth.Modify(200);
					}
					break;
				}
			}
		}
	}
}

void Actor::UnequipItem(const shared_ptr<Item> &item)
{
	if (item)
	{
		for (auto & handEquipment : mHandSlots)
		{
			if (handEquipment == item)
			{
				if (auto wpn = dynamic_pointer_cast<Weapon>(handEquipment))
				{
					wpn->SetVisible(false);
				}
				if (auto shield = dynamic_pointer_cast<Shield>(handEquipment))
				{
					shield->Raise(false);
				}
				handEquipment.reset();
			}
		}
		for (auto & handEquipmentConsumable : mHandEquipmentConsumables)
		{
			if (item == handEquipmentConsumable)
			{
				handEquipmentConsumable.reset();
			}
		}
		if (item == mHelmet)
		{
			mHelmet.reset();
		}
		else if (item == mGloves)
		{
			mGloves.reset();
		}
		else if (item == mTrousers)
		{
			mTrousers.reset();
		}
		else if (item == mChestArmor)
		{
			mChestArmor.reset();
		}
		for (auto &ring : mRings)
		{
			if (item == ring)
			{
				if (ring->GetType() == ItemType::Ring_RingOfLife)
				{
					mStats->mMaxHealth.Modify(-200);
				}
				ring.reset();
			}
		}
	}
}

void Actor::AddItem(const shared_ptr<Item> &item)
{
	if (mInventory)
	{
		mInventory->AddItem(item);
	}
	else
	{
		throw Exception("Inventory was nullptr");
	}
}

void Actor::AddAndUseItem(const shared_ptr<Item> &item, const Hand & hand)
{
	AddItem(item);
	UseItem(item, hand);
}

shared_ptr<Item> Actor::GetHandEquipment(const Hand & hand) const
{
	return hand == Hand::Left ? mHandSlots[0] : mHandSlots[1];
}

shared_ptr<Item> Actor::GetHandEquipmentConsumable(const Hand & hand) const
{
	return mHandEquipmentConsumables[static_cast<int>(hand)];
}

shared_ptr<Item> Actor::GetHelmet() const
{
	return mHelmet;
}

shared_ptr<Item> Actor::GetGloves() const
{
	return mGloves;
}

shared_ptr<Item> Actor::GetTrousers() const
{
	return mTrousers;
}

shared_ptr<Item> Actor::GetChestArmor() const
{
	return mChestArmor;
}

const array<shared_ptr<Item>, Actor::MaxRings> &Actor::GetRings() const
{
	return mRings;
}

bool Actor::IsGroundContact() const
{
	bool groundContact = false;
	for (int i = 0; i < mPivot->GetBody()->GetContactCount(); ++i)
	{
		if (mPivot->GetBody()->GetContact(i).mPosition.y > GetPosition().y + 5)
		{
			groundContact = true;
			break;
		}
	}
	return groundContact;
}

bool Actor::IsIntersect(const sf::FloatRect &r) const
{
	if (GetBody())
	{
		return Math::IsCircleRectIntersection(
		{ r.left, r.top }, { r.width, r.height }, GetPosition(), GetBody()->GetRadius());
	}
	return false;
}

bool Actor::IsHookable() const
{
	return mHookable;
}

