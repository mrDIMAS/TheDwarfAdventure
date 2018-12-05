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
#include "Inventory.h"
#include "Player.h"
#include "Level.h"
#include "Renderer.h"
#include "Weapon.h"
#include "Item.h"

// retval: false - not enough items (nothing removed), true - items removed
bool ItemGroup::RemoveCount(int i)
{
	if (mItems.begin() + i <= mItems.end())
	{
		mItems.erase(mItems.begin(), mItems.begin() + i);
		return true;
	}
	else
	{
		return false;
	}
}

void ItemGroup::Add(const shared_ptr<Item> &item)
{
	mItems.push_back(item);
}

int ItemGroup::Count() const
{
	return mItems.size();
}

void ItemGroup::Serialize(Serializer &sav)
{
	sav &mType;
	sav &mItems;
}

void ItemGroup::TransferCount(int count, const shared_ptr<ItemGroup> &reciever)
{
	if (static_cast<int>(mItems.size()) >= count)
	{
		for (int i = 0; i < count; ++i)
		{
			reciever->mItems.push_back(mItems.front());
			mItems.erase(mItems.begin());
		}
	}
}

shared_ptr<Item> ItemGroup::GetAny() const
{
	if (mItems.empty())
	{
		throw Exception("Empty ItemGroup can't exist!");
	}
	return mItems.front();
}

bool ItemGroup::Contains(const shared_ptr<Item>& item) const
{
	for (const shared_ptr<Item>& otherItem : mItems)
	{
		if (otherItem == item)
		{
			return true;
		}
	}

	return false;
}

Inventory::Inventory() : mMoney(0)
{
}

Inventory::Inventory(const weak_ptr<Actor> &owner) : mOwner(owner), mMoney(0)
{
}

Inventory::~Inventory()
{
	DBG_ECHO();
}

void Inventory::Serialize(Serializer &sav)
{
	sav &mItemGroups;
	sav &mOwner;
}

void Inventory::AddItem(const shared_ptr<Item> &item, int groupFlags)
{
	shared_ptr<ItemGroup> group;
	auto sameGroupCriteria = [&] (const shared_ptr<ItemGroup> &group) -> bool
	{
		auto iClass = item->GetDefinition().mClass;
		if (iClass == ItemClass::Weapon || iClass == ItemClass::Shield)
		{
			return false;
		}
		auto sameType = group->mType == item->GetType();
		auto sameDurability = group->mItems.front()->GetCurrentDurability() == item->GetCurrentDurability();
		auto nonModified = !group->mItems.front()->IsModified();
		return sameType && sameDurability && nonModified;
	};
	auto itemGroupIter = std::find_if(mItemGroups.begin(), mItemGroups.end(), sameGroupCriteria);
	bool flagsDoesntMatch = false;
	if (itemGroupIter != mItemGroups.end())
	{
		flagsDoesntMatch = (*itemGroupIter)->mFlags != groupFlags;
	}
	if (itemGroupIter == mItemGroups.end() || flagsDoesntMatch)
	{
		group = make_shared<ItemGroup>();
		group->mType = item->GetType();
		group->mFlags = groupFlags;
		mItemGroups.push_back(group);
	}
	else
	{
		group = *itemGroupIter;
	}
	group->Add(item);
	item->SetPosition({ -9999.0f, -9999.0f });
	item->OnPut();
	auto location = item->GetLocation();
	THROW_IF_NULL(location);
	location->RemoveItem(item);
}

void Inventory::RemoveItem(const shared_ptr<Item> &item)
{
	for (auto &group : mItemGroups)
	{
		group->mItems.erase(
			std::remove(group->mItems.begin(), group->mItems.end(), item), group->mItems.end());
	}
	// erase empty groups
	for (auto iter = mItemGroups.begin(); iter != mItemGroups.end();)
	{
		if ((*iter)->Count() == 0)
		{
			iter = mItemGroups.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Inventory::ThrowItem(const shared_ptr<Item> &item)
{
	THROW_IF_EXPIRED(mOwner);
	auto location = mOwner.lock()->GetLocation();
	item->SetPosition(mOwner.lock()->GetPosition());
	item->GetNode()->Detach();
	// also transfer item model to scene of owner
	location->GetScene()->AcceptNode(item->GetNode());
	location->AddItem(item);
	RemoveItem(item);
	item->OnPullOut();
}

void Inventory::TransferItem(const shared_ptr<Item> &item, const shared_ptr<Inventory> &other)
{
	RemoveItem(item);
	other->AddItem(item);
}

weak_ptr<Actor> Inventory::GetOwner()
{
	return mOwner;
}

const vector<shared_ptr<ItemGroup>> &Inventory::GetGroups() const
{
	return mItemGroups;
}

// returns true if consumable was found, false otherwise
// method used for fast extraction of consumbles (rocket, grenades, etc.)
shared_ptr<Item> Inventory::ExtractConsumable(ItemType type)
{
	for (auto &group : mItemGroups)
	{
		if (group->mType == type)
		{
			auto item = group->mItems.front();
			RemoveItem(group->mItems.front());
			return item;
		}
	}
	return nullptr;
}

shared_ptr<ItemGroup> Inventory::FindGroup(ItemType type)
{
	auto g = find_if(mItemGroups.begin(), mItemGroups.end(),
		[type] (const auto &group) { return type == group->mType; });
	return (g != mItemGroups.end()) ? *g : nullptr;
}

void Inventory::GrabAllFrom(const shared_ptr<Inventory> &other)
{
	if (other.get())
	{
		// copy items from other inventory to corresponding groups
		for (auto grIter = other->mItemGroups.begin(); grIter != other->mItemGroups.end();)
		{
			auto &otherGroup = *grIter;
			bool itemsCopied = false;
			for (auto group : mItemGroups)
			{
				// found group with same items
				if (group->mType == otherGroup->mType)
				{
					// copy items to the group
					group->mItems.insert(
						group->mItems.end(), otherGroup->mItems.begin(), otherGroup->mItems.end());
					itemsCopied = true;
					break;
				}
			}
			if (itemsCopied)
			{
				// items are copied, group can be removed
				grIter = other->mItemGroups.erase(grIter);
			}
			else
			{
				// group stay unchanged and will be copied directly to 'this' inventory
				++grIter;
			}
		}
		mMoney += other->mMoney;
		// rest of groups are copied directly
		mItemGroups.insert(mItemGroups.end(), other->mItemGroups.begin(), other->mItemGroups.end());
		other->mItemGroups.clear();
		other->mMoney = 0;
	}
}

bool Inventory::IsEmpty() const
{
	for (auto &group : mItemGroups)
	{
		if (group->Count())
		{
			return false;
		}
	}
	return true;
}

int Inventory::GetMoney() const
{
	return mMoney;
}

void Inventory::AddMoney(int count)
{
	mMoney += abs(count);
}

void Inventory::RemoveMoney(int count)
{
	mMoney -= abs(count);
}

bool Inventory::TransferItems(const shared_ptr<Inventory> &reciever, ItemType type, int count)
{
	auto groupIter = find_if(
		mItemGroups.begin(), mItemGroups.end(), [type] (const auto &g) { return g->mType == type; });
	if (groupIter != mItemGroups.end())
	{
		auto group = *groupIter;
		if (group->Count() >= count)
		{
			auto destGroup = find_if(reciever->mItemGroups.begin(), reciever->mItemGroups.end(),
				[type] (const shared_ptr<ItemGroup> &g) { return g->mType == type; });
			shared_ptr<ItemGroup> recieverGroup;
			if (destGroup == reciever->mItemGroups.end())
			{
				recieverGroup = make_shared<ItemGroup>();
				recieverGroup->mType = type;
				reciever->mItemGroups.push_back(recieverGroup);
			}
			else
			{
				recieverGroup = *destGroup;
			}
			group->TransferCount(count, recieverGroup);
			if (group->mItems.size() == 0)
			{
				mItemGroups.erase(groupIter);
			}
			return true;
		}
	}
	return false;
}

shared_ptr<ItemGroup> Inventory::FindItemGroup(const ItemType &type)
{
	for (auto &group : mItemGroups)
	{
		if (group->mType == type)
		{
			return group;
		}
	}
	return nullptr;
}

void Inventory::MoveGroup(const shared_ptr<ItemGroup> &group, const shared_ptr<Inventory> &reciever)
{
	if (group)
	{
		reciever->mItemGroups.push_back(group);
		mItemGroups.erase(remove(mItemGroups.begin(), mItemGroups.end(), group));
	}
}

float Inventory::GetTotalWeight() const
{
	float weight = 0;
	for (const auto &group : mItemGroups)
	{
		const auto &desc = group->GetAny()->GetDefinition();
		weight += desc.mWeight * group->Count();
	}
	return weight;
}

bool Inventory::Contains(const shared_ptr<Item>& item) const
{
	for (const shared_ptr<ItemGroup>& group : mItemGroups)
	{
		if (group->Contains(item))
		{
			return true;
		}
	}
	return false;
}
