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

class Inventory;
class Item;
struct ItemGroup;

struct EquipmentSlot
{
	shared_ptr<Frame> mFrame;
	shared_ptr<Image> mImage;
	weak_ptr<Item> mItem;
	EquipmentSlot()
	{
	}
	EquipmentSlot(const shared_ptr<Canvas> &canvas, const Vec2f &size);
	void SetItem(const shared_ptr<Item> &item);
};

class InventoryGUI
{
private:
	shared_ptr<Canvas> mCanvas;
	shared_ptr<Window> mWindow;
	shared_ptr<SlideList> mFilter;
	shared_ptr<Table> mTable;
	shared_ptr<Button> mUseLeftHandBtn;
	shared_ptr<Button> mUseRightHandBtn;
	shared_ptr<Button> mThrowBtn;
	shared_ptr<Label> mMoney;
	shared_ptr<Label> mWeight;
	weak_ptr<Inventory> mInventory;
	shared_ptr<Tooltip> mTooltip;
	shared_ptr<Label> mEquipmentLbl;
	shared_ptr<Slot> mHeadEquipment;
	shared_ptr<Slot> mBodyEquipment;
	shared_ptr<Slot> mArmsEquipment;
	shared_ptr<Slot> mLegsEquipment;
	shared_ptr<Slot> mLeftArmForefingerRing;
	shared_ptr<Slot> mLeftArmRingfingerRing;
	shared_ptr<Slot> mRightArmForefingerRing;
	shared_ptr<Slot> mRightArmRingfingerRing;
	array<shared_ptr<Slot>, 2> mHandSlots;
	array<shared_ptr<Slot>, 2> mHandEquipmentConsumablesSlots;
	vector<shared_ptr<ItemGroup>> mFilteredGroups;
	void ShowTooltipForItem(const string &preText, const shared_ptr<Item> &item, int count);
	void UnequipItemByClick(const shared_ptr<Item> &item);
	void ShowSlotTooltip(const shared_ptr<Item> &item, const string &preText, const string &emptyText);
	void SetSlotItem(const shared_ptr<Slot> &slot, const shared_ptr<Item> &item);
	shared_ptr<Slot> MakeSlot(const Vec2f &size, const Vec2f &position);

public:
	InventoryGUI();
	~InventoryGUI();
	void Update();
	// do nothing without generated level and if no player presented
	void SetVisible(bool state);
	bool IsVisible() const;
	void SwitchVisible();
	void SetInventory(const shared_ptr<Inventory> &inv);
	void Sync();
};
