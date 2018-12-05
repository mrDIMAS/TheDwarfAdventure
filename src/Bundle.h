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

template <class T> struct BundleItem
{
	float mProbability { 0.0f };
	T mValue;
	BundleItem()
	{
	}
	BundleItem(float probability, T value) : mProbability(probability), mValue(value)
	{
	}
};

template <class T> class Bundle
{
private:
	mutable vector<BundleItem<T>> mItems;
	float mProbabilitySum { 0 };
	mutable bool mSorted { false };

public:
	Bundle()
	{
	}

	Bundle(std::initializer_list<BundleItem<T>> il) : mItems(il.begin(), il.end())
	{
		mProbabilitySum = 0;
		for (auto &i : mItems)
		{
			mProbabilitySum += i.mProbability;
		}
		mSorted = false;
	}

	const T& Drop() const
	{
		if (mItems.empty())
		{
			throw Exception("Bundle is empty! Unable to drop item!");
		}

		if (!mSorted)
		{
			sort(mItems.begin(), mItems.end(),
				[] (const auto &a, const auto &b) { return a.mProbability < b.mProbability; });
			mSorted = true;
		}

		float p = Math::Rnd(0.0f, mProbabilitySum);
		float n = 0.0f;
		for (auto &item : mItems)
		{
			n += item.mProbability;
			if (p <= n)
			{
				return item.mValue;
			}
		}
		return mItems[0].mValue;
	}

	const vector<BundleItem<T>> &GetItems() const
	{
		return mItems;
	}

	void Add(BundleItem<T>&& item)
	{
		mItems.emplace_back(item);
		mSorted = false;
		mProbabilitySum += item.mProbability;
	}

	bool IsEmpty() const
	{
		return mItems.empty();
	}

	void Clear()
	{
		mSorted = false;
		mProbabilitySum = 0;
		mItems.clear();
	}
};
