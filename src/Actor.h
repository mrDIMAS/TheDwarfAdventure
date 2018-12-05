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

#include "Sprite.h"
#include "Event.h"
#include "ItemDatabase.h"
#include "ViewTriangle.h"
#include "Hitbox.h"

class Inventory;
class Weapon;
class Item;
class Stats;
class Location;
class Shield;
struct Damage;
class Dialog;

enum class Hand
{
	Left,
	Right
};

class Actor : public enable_shared_from_this<Actor>
{
public:
	static constexpr int MaxRings { 4 };

	Vec2f GetFeetPosition();
	// all times given in frames
	// game routines running at fixed speed - 60 fps - so 60 frames gives 1 second
	static constexpr int MaxStunLockTimeout = 60;
	// amount of damage that completely drains out stamina
	static constexpr float ShieldBlockMaxDamage = 250;
	static constexpr float ShieldMaxStaminaConsumption = 50;
	Actor();
	Actor(const shared_ptr<Location> &location, float bodyRadius);
	void ArrangeIndicators();
	virtual ~Actor();
	shared_ptr<Location> GetLocation();
	virtual void Think();
	void SetLocation(const shared_ptr<Location> &location);
	shared_ptr<Body> GetBody() const;
	virtual void SetPosition(const Vec2f &p);
	Vec2f GetPosition() const;
	void AddForce(const Vec2f &v);
	virtual void Serialize(Serializer &sav);
	shared_ptr<Inventory> GetInventory() const;
	const unique_ptr<Stats> &GetStats() const;
	virtual void ApplyDamage(Damage & dmg);
	// Generic update method (includes some basic interaction methods, i.e. damaging from environment)
	void Update();
	bool IsDead() const;
	// Will break an actor into pieces.
	// Method does not do anything with the current actor model: model is rendered into texture and it
	// is breaking into pieces. After calling this method, you may want to delete the actor
	virtual void Explode();
	shared_ptr<SceneNode> GetPivot();
	virtual void UseItem(const shared_ptr<Item> &item, const Hand & hand);
	virtual void UnequipItem(const shared_ptr<Item> &item);
	virtual void AddItem(const shared_ptr<Item> &item);
	virtual void AddAndUseItem(const shared_ptr<Item> &item, const Hand & hand);
	shared_ptr<Item> GetHandEquipment(const Hand & hand) const;
	shared_ptr<Item> GetHandEquipmentConsumable(const Hand & hand) const;
	shared_ptr<Item> GetHelmet() const;
	shared_ptr<Item> GetGloves() const;
	shared_ptr<Item> GetTrousers() const;
	shared_ptr<Item> GetChestArmor() const;
	const array<shared_ptr<Item>, MaxRings> &GetRings() const;
	bool IsGroundContact() const;
	bool IsIntersect(const sf::FloatRect &r) const;
	bool IsHookable() const;
	virtual void OnDie();
	void ResetHandEquipmentConsumable(const Hand & hand);
	bool IsInStunLock() const;
	void SetName(const string &name);
	bool IsOutOfWeaponConsumables(ItemType weaponType, const Hand & hand) const;
	// Uses vision triangle to check visibility of other actor.
	bool IsSeeActor(const shared_ptr<Actor> &actor) const;
	void UpdateHitBoxes();
	HitBox *FindHitbox(const shared_ptr<SceneNode> &node);
	const vector<HitBox> &GetHitBoxes() const;
	bool IsAnyHitBoxIntersected(const Vec2f &p, float r) const;
	bool IsAnyHitBoxIntersected(const Math::RotatedRectangle &r) const;
	bool IsAnyHitBoxIntersected(const HitBox &other) const;
	virtual void ChangeReputation(float repChange);
	float GetEquipmentDamageReduction(const DamageClass &dmgClass);
	// Any derived class may override this method to set proper rules for relative stat changes, based
	// on passed level
	virtual void OnSetLevel();
	// Used for actor level adjusting.
	// Every location in game adjusts actor's (NPCs, monsters, but not Player) level according to
	// various set of rules. It's easy to adjust only level, but not the whole set of stats.
	void SetLevel(int level);
	// Returns equipped shield if have any, nullptr otherwise
	shared_ptr<Shield> GetEquippedShield() const;
	bool IsEquippedShieldRaised() const;
	const array<shared_ptr<Item>, 2> & GetHandSlots() const;
	Hand GetHoldingHand(const shared_ptr<Item> & item) const;
	bool IsHaveBadReputation() const;
	// returns true if door is opened
	bool OpenDoor(const shared_ptr<Door> & door);
	void CloseDoor(const shared_ptr<Door> & door, bool lock = false);
	void UnlockDoor(const shared_ptr<Door> & door);
	const Guid& GetIdentifier() const;
	const string & GetName() const;
	template<class T> 
	shared_ptr<T> GetDialogOfType() const;
	template<class T>
	bool BeginDialogOfType() const;
protected:
	// List of dialogs that can happen with a player
	vector<shared_ptr<Dialog>> mDialogs;
	// list of hit boxes for an actor
	// should not contain any special hit boxes (attack hitbox i.e.)
	// damage system rely on these
	vector<HitBox> mHitBoxes;
	vector<shared_ptr<PolyLine>> mDebugHitboxes;
	weak_ptr<Location> mLocation;
	shared_ptr<SceneNode> mPivot;
	shared_ptr<Inventory> mInventory;
	// 0 - left hand, 1 - right hand
	array<shared_ptr<Item>, 2> mHandSlots;
	array<shared_ptr<Item>, 2> mHandEquipmentConsumables;
	shared_ptr<Item> mHelmet;
	shared_ptr<Item> mGloves;
	shared_ptr<Item> mTrousers;
	shared_ptr<Item> mChestArmor;
	// Name showing above actor's head
	string mName;
	// Unique identifier to access actor from scripts
	Guid mIdentifier;
	array<shared_ptr<Item>, MaxRings> mRings;
	unique_ptr<Stats> mStats;
	bool mHookable { false }; // true if hooking using Hook gun is allowed
	void ApplyEquippedItemDamageReduction(const shared_ptr<Item> &item, DamageClass damageClass, float &count);
	chrono::steady_clock::time_point mLastTime;
	ViewTriangle mViewTriangle;
	int mHealSignTimer { 0 };
	shared_ptr<Sprite> mHealSign;
	shared_ptr<Sprite> mHealthBar;
	shared_ptr<Sprite> mStunLockIndicator;
	shared_ptr<Text> mNameText;
	// >0 means creature in stun-lock and can't do anything
	int mStunLockInterval { 0 };
	// used for stun-lock
	float mConsumedDamage { 0 };
	float mStunLockConsumedDamage { 0 };
	int mNextPossibleStunLockTimeout { 0 };
	int mNextPossibleDodgeTimeout { 0 };
	static constexpr int DodgeTimeout { 30 };
	Vec2f mLastFeetPosition;
	shared_ptr<SceneNode> mModelRoot;
	vector<shared_ptr<Animation>> mAnimations;
	virtual void SetAnimation(const shared_ptr<Animation> &animation);
	void SetAnimationEnabledForNode(const shared_ptr<SceneNode> &node, bool state, bool affectChildren = true);
	void MakeStunLock();
	virtual void UpdateHitBoxesRecursively(const shared_ptr<SceneNode> &entry);
	void UpdateNameLabel();
	void SetHandEqupment(const Hand & hand, const shared_ptr<Item> & item);
	void HandleFallingDamage(const Contact & contact);
};

template<class T>
shared_ptr<T> Actor::GetDialogOfType() const
{
	for (const shared_ptr<Dialog>& dialog : mDialogs)
	{
		if (const shared_ptr<T> casted = dynamic_pointer_cast<T>(dialog))
		{
			return casted;
		}
	}
	return nullptr;
}

template<class T>
bool Actor::BeginDialogOfType() const
{
	if (const shared_ptr<T> dialog = GetDialogOfType<T>())
	{
		game->RaiseDialog(dialog);

		return true;
	}

	return false;
}
