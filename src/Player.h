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
#include "ViewTriangle.h"

class MotherCrystal;
class Level;
struct ItemGroup;

class Player : public Dwarf
{
public:
	static constexpr int QuickAccessSlotCount = 10;

private:
	shared_ptr<MotherCrystal> mLastVisitedMotherCrystal;
	array<weak_ptr<ItemGroup>, QuickAccessSlotCount> mQuickAccessGroups;
	
	shared_ptr<Light> mLight;

	int mRespawnTime { 180 };
	int mDeathCount { 0 };
	int mKillCount { 0 };
	bool mOnLadder { false };
	float mZoomMin { 1.0f };
	float mZoomMax { 3.0f };
	float mViewScale { 1.0f };

	void InteractWithLadders(const shared_ptr<Body> &body, const shared_ptr<Location> &location);

	// these interaction should be chained according to interaction priorities
	void InteractWithItems();
	void InteractWithActors();
	void InteractWithFogGates();
	void InteractWithRemains();
	void InteractWithChests();
	void InteractWithMotherCrystals();
	void InteractWithDoors();
	void InteractWithRooms();
	void InteractWithBenches();
	void UpdateView(const shared_ptr<Location> &location);
	void UpdateFogOfWar();
	void UpdateMovement();
	void UpdateHandEquipment(const Vec2f &aimDirection, const Controls &controls, Input *input);
	void UpdateCurrentShield(Input *input, bool flip);
	void UpdateQuickAccess(const Controls &controls);

	void Respawn();
public:
	Player();
	Player(const shared_ptr<Location> &location);
	~Player();
	virtual void Think() override final;
	virtual void Serialize(Serializer &sav) override final;
	void SetQuickAccess(const weak_ptr<ItemGroup> &group, int slotNum);
	void CreateInventory();
	virtual void UseItem(const shared_ptr<Item> &item, const Hand & hand) override final;
	void AddKill();
	int GetKillCount() const;
	virtual void OnDie() override final;
	virtual void ChangeReputation(float repChange) override;
	virtual void AddItem(const shared_ptr<Item> &item) override final;
};
