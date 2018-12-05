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

#include "Widget.h"
#include "ScrollableArea.h"
#include "LineSet.h"
#include "Image.h"

class Table : public Widget
{
private:
	using Row = vector<shared_ptr<Widget>>;
	shared_ptr<ScrollableArea> mItemArea;
	vector<Row> mRowList;
	Row mHeaderRow;
	int mColumnCount;
	Layout mHeaderLayout;
	shared_ptr<LineSet> mTableBorders;
	shared_ptr<Image> mSelector;
	shared_ptr<Image> mSelectedRow;
	shared_ptr<Image> mHeaderImg;
	int mSelectedRowIndex;
	int mHoveredRowIndex;
	virtual void OnResize();
	virtual void DrawSelf();
	void UpdateBorders();
	void ReLayoutHeader();
	void ReLayoutContent();

public:
	Event OnRowHovered;
	Event OnRowClicked;
	Table(const shared_ptr<Canvas> &canvas);
	~Table();
	virtual void LinkComponents();
	virtual void Update() override final;
	void AddColumn(const string &name);
	int AddRow();
	shared_ptr<Widget> Get(unsigned int x, unsigned int y);
	void Set(unsigned int x, unsigned int y, const shared_ptr<Widget> &newWidget);
	void Clear();
	int GetSelectedRowIndex() const;
	int GetHoveredRowIndex() const;
	Row &GetRow(int n);
	virtual Vec2f GetSize() const override final;
	float GetScrollBarPosition() const;
	void SetScrollBarPosition(float position);
};
