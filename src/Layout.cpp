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
#include "Layout.h"
#include "Widget.h"

Layout::Layout()
{
}

Layout::Layout(const Vec2f &origin, float width, int columns, float spacing)
	: mWidth(width), mOrigin(origin),
	mTemplateRow(columns, spacing, (width - spacing * columns) / columns), mCursorY(origin.y)
{
}

Layout::~Layout()
{
	DBG_ECHO();
}

void Layout::Pack(const shared_ptr<Widget> &widget)
{
	// Check if mRow not exists and if so, create new using default
	if (mRows.find(mRow) == mRows.end())
	{
		mRows[mRow] = mTemplateRow;
	}

	// Obtain row
	auto &row = mRows[mRow];

	if (widget)
	{
		// Reposition and resize widget
		widget->SetPosition({ mOrigin.x + mColumn * (row.mItemWidth + row.mSpacing), mCursorY });
		widget->SetSize({ row.mItemWidth, widget->GetSize().y });

		// Remember highest item
		if (widget->GetSize().y > mHighestItemHeightOnRow)
		{
			mHighestItemHeightOnRow = widget->GetSize().y;
		}
	}

	// move caret
	++mColumn;
	if (mColumn >= row.mColumnCount)
	{
		mCursorY += mHighestItemHeightOnRow + row.mSpacing;
		mColumn = 0;
		mHighestItemHeightOnRow = 0;
		++mRow;
	}
}

void Layout::Reset()
{
	mColumn = 0;
	mRow = 0;
	mHighestItemHeightOnRow = 0;
	mCursorY = mOrigin.y;
	for (auto &row : mRows)
	{
		const auto totalSpacing = row.second.mSpacing * row.second.mColumnCount;
		row.second.mItemWidth = (mWidth - totalSpacing) / row.second.mColumnCount;
	}
}

void Layout::SetOrigin(const Vec2f &origin)
{
	mOrigin = origin;
}

void Layout::MoveOrigin(const Vec2f &offset)
{
	mOrigin += offset;
}

Vec2f Layout::GetOrigin() const
{
	return mOrigin;
}

// to leave parameter without changes, pass value < 0 (like -1)
// if row doesn't exist, it will be created
void Layout::ConfigureRow(int rowNum, int columnCount, float spacing)
{
	auto &row = rowNum < 0 ? mTemplateRow : mRows[rowNum];
	if (columnCount > 0)
	{
		row.mColumnCount = columnCount;
	}
	if (spacing >= 0)
	{
		row.mSpacing = spacing;
	}
	const auto totalSpacing = row.mSpacing * row.mColumnCount;
	row.mItemWidth = (mWidth - totalSpacing) / row.mColumnCount;
}

void Layout::PushRow(int columnCount, float spacing)
{
	mRows[mRows.size()] = mTemplateRow;
	ConfigureRow(mRows.size() - 1, columnCount, spacing);
}

int Layout::GetRowCount() const
{
	return mRows.size();
}

void Layout::SetWidth(float width)
{
	mWidth = width;
	const auto totalSpacing = mTemplateRow.mSpacing * mTemplateRow.mColumnCount;
	mTemplateRow.mItemWidth = (width - totalSpacing) / mTemplateRow.mColumnCount;
}

void Layout::PurgeRows()
{
	mRows.clear();
}

float Layout::GetWidth() const
{
	return mWidth;
}

float Layout::GetSpacing() const
{
	return mTemplateRow.mSpacing;
}

Layout::Row::Row() : mColumnCount(1), mSpacing(5), mItemWidth(20)
{
}

Layout::Row::Row(int columnCount, float spacing, float itemWidth)
	: mColumnCount(columnCount), mSpacing(spacing), mItemWidth(itemWidth)
{
}
