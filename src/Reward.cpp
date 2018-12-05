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
#include "Reward.h"
#include "Actor.h"
#include "Inventory.h"
#include "Stats.h"

Reward::Reward()
{

}

Reward::Reward(vector<ItemType>&& items, int money, int experience)
	: mItems(std::move(items))
	, mMoney(money)
	, mExperience(experience)
{

}

void Reward::Serialize(Serializer & sav)
{
	sav & mItems;
	sav & mMoney;
	sav & mExperience;
}

void Reward::GiveToActor(const shared_ptr<Actor>& actor) const
{
	if (const shared_ptr<Inventory>& inventory = actor->GetInventory())
	{
		for (const ItemType& itemType : mItems)
		{
			inventory->AddItem(game->GetItemDatabase()->Make(itemType, actor->GetLocation()));
		}

		inventory->AddMoney(mMoney);
	}

	actor->GetStats()->AddExpirience(mExperience);

	mMoney = 0;
	mItems.clear();
	mExperience = 0;
}

Reward& Reward::operator += (const Reward& other)
{
	for (const ItemType& itemType : other.mItems)
	{
		mItems.push_back(itemType);
	}
	mMoney += other.mMoney;
	mExperience += other.mExperience;
	return *this;
}

const vector<ItemType>& Reward::GetItems() const
{
	return mItems;
}

int Reward::GetMoney() const
{
	return mMoney;
}

int Reward::GetExperience() const
{
	return mExperience;
}
