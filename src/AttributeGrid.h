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

// Attribute grid used to provide layered low-resolution additional terrain info
// For example, each grid cell can be marked as Visible and then this flag
// can be used in sound volume calculation to muffle sounds that are behind walls
class AttributeGridLayer
{
public:
	enum Flags
	{
		None = 0,
		Visible = BIT(0),
	};

	struct Cell
	{
		// By default each cell is Visible to eliminate questions "why I can't hear sounds" and etc.
		unsigned char mFlags { Visible };

		void Serialize(Serializer & sav);
	};

	AttributeGridLayer()
	{

	}
	AttributeGridLayer(unsigned int width, unsigned int height)
		: mWidth(width)
		, mHeight(height)
	{
		mCells.resize(width * height);
	}
	~AttributeGridLayer()
	{

	}

	unsigned int GetCellCount() const
	{
		return mWidth * mHeight;
	}
	void Clear()
	{
		const unsigned int cellCount = GetCellCount();

		for (unsigned int i = 0; i < cellCount; ++i)
		{
			mCells[i].mFlags = Flags::None;
		}
	}
	unsigned int NormalizedToIndex(const Vec2f & pointNormalizedCoords) const
	{
		unsigned int x = static_cast<unsigned int>(pointNormalizedCoords.x * mWidth);
		unsigned int y = static_cast<unsigned int>(pointNormalizedCoords.y * mHeight);

		const unsigned int cellCount = GetCellCount();
		unsigned int index = y * mWidth + x;

		if (index >= cellCount)
		{
			index = cellCount - 1;
		}

		return index;
	}
	void Set(const Vec2f & normalizedCoords, unsigned int flags)
	{
		mCells[NormalizedToIndex(normalizedCoords)].mFlags |= flags;
	}
	void Reset(const Vec2f & normalizedCoords, unsigned int flags)
	{
		mCells[NormalizedToIndex(normalizedCoords)].mFlags &= ~flags;
	}
	bool IsSet(const Vec2f & normalizedCoords, unsigned int flags) const
	{
		return (mCells[NormalizedToIndex(normalizedCoords)].mFlags & flags) == flags;
	}
	bool IsAnySet(const Vec2f & normalizedCoords, unsigned int flags) const
	{
		return (mCells[NormalizedToIndex(normalizedCoords)].mFlags & flags) != 0;
	}
	Cell* At(unsigned int index)
	{
		return &mCells[index];
	}
	void Serialize(Serializer & sav)
	{
		sav & mWidth;
		sav & mHeight;
		sav & mCells;
	}
	unsigned int GetWidth() const
	{
		return mWidth;
	}
	unsigned int GetHeight() const
	{
		return mHeight;
	}
private:
	unsigned int mWidth { 0 };
	unsigned int mHeight { 0 };
	vector<Cell> mCells;
};

class AttributeGrid
{
public:
	AttributeGrid()
	{

	}

	AttributeGrid(unsigned int width, unsigned int height) 
		: mLootRareness(width, height)
		, mGenericFlags(width, height)
	{

	}

	void Serialize(Serializer & sav)
	{
		sav & mLootRareness;
		sav & mGenericFlags;
	}

	AttributeGridLayer & GetLootRarenessLayer()
	{
		return mLootRareness;
	}

	AttributeGridLayer & GetGenericFlagsLayer()
	{
		return mGenericFlags;
	}
private:
	AttributeGridLayer mLootRareness;
	AttributeGridLayer mGenericFlags;
};