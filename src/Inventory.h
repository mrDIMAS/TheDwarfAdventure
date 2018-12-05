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

#include "ItemDatabase.h"

class Player;
class Actor;
class Item;

struct ItemGroup
{
	int mFlags { 0 };
	ItemType mType;         // item type
	vector<shared_ptr<Item>> mItems; // actual items
	// retval: false - not enough items (nothing removed), true - items removed
	bool RemoveCount(int i);
	void Add(const shared_ptr<Item> &item);
	int Count() const;
	void Serialize(Serializer &sav);
	void TransferCount(int count, const shared_ptr<ItemGroup> &reciever);
	shared_ptr<Item> GetAny() const;
	bool Contains(const shared_ptr<Item>& item) const;
};

class Inventory : public enable_shared_from_this<Inventory>
{
public:
	Inventory();
	// for chests owner should be nullptr
	// in this case, ThrowItem does nothing - so chest can AddItem, RemoveItem and
	// TransferItem only
	Inventory(const weak_ptr<Actor> &owner);
	virtual ~Inventory();
	virtual void Serialize(Serializer &sav);
	void AddItem(const shared_ptr<Item> &item, int groupFlags = 0);
	void RemoveItem(const shared_ptr<Item> &item);
	void ThrowItem(const shared_ptr<Item> &item);
	void TransferItem(const shared_ptr<Item> &item, const shared_ptr<Inventory> &other);
	weak_ptr<Actor> GetOwner();
	const vector<shared_ptr<ItemGroup>> &GetGroups() const;
	// returns true if consumable was found, false otherwise
	// method used for fast extraction of consumbles (rocket, grenades, etc.)
	shared_ptr<Item> ExtractConsumable(ItemType type);
	shared_ptr<ItemGroup> FindGroup(ItemType type);
	void GrabAllFrom(const shared_ptr<Inventory> &other);
	bool IsEmpty() const;
	int GetMoney() const;
	void AddMoney(int count);
	void RemoveMoney(int count);
	bool TransferItems(const shared_ptr<Inventory> &reciever, ItemType type, int count);
	shared_ptr<ItemGroup> FindItemGroup(const ItemType &type);
	void MoveGroup(const shared_ptr<ItemGroup> &group, const shared_ptr<Inventory> &reciever);
	float GetTotalWeight() const;
	bool Contains(const shared_ptr<Item>& item) const;
protected:
	// items are grouped
	vector<shared_ptr<ItemGroup>> mItemGroups;
	weak_ptr<Actor> mOwner;
	int mMoney;
};
