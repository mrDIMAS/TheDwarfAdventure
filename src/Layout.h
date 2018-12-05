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

class Widget;
class SceneNode;

class Layout
{
private:
	Vec2f mOrigin;
	float mWidth { 0 };
	int mColumn { 0 };
	int mRow { 0 };
	float mCursorY { 0 };
	float mHighestItemHeightOnRow { 0 };

	struct Row
	{
		int mColumnCount;
		float mSpacing;
		float mItemWidth;
		Row();
		Row(int columnCount, float spacing, float itemWidth);
	};
	Row mTemplateRow;
	unordered_map<unsigned int, Row> mRows;

public:
	static constexpr int TemplateRow = -1;
	Layout();
	Layout(const Vec2f &origin, float width, int columns, float spacing);
	~Layout();
	void Pack(const shared_ptr<Widget> &widget);
	void Reset();
	void SetOrigin(const Vec2f &origin);
	void MoveOrigin(const Vec2f &offset);
	Vec2f GetOrigin() const;
	// to leave parameter without changes, pass value < 0 (like -1)
	// if row doesn't exist, it will be created
	// if row < 0, defaultRow will be changed
	void ConfigureRow(int rowNum, int columnCount = -1, float spacing = -1);
	void PushRow(int columnCount = -1, float spacing = -1);
	int GetRowCount() const;
	void SetWidth(float width);
	void PurgeRows();
	float GetWidth() const;
	float GetSpacing() const;
};
