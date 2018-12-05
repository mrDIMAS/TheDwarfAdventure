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

class LineSet : public Widget
{
private:
	float mWidth;
	struct Line
	{
		Vec2f mBegin;
		Vec2f mEnd;
		sf::Color mColor;
		Line(const Vec2f &begin, const Vec2f &end, const sf::Color &clr);
	};
	vector<Line> mLineList;
	virtual void DrawSelf() override final;

public:
	LineSet(const shared_ptr<Canvas> &canvas);
	virtual void LinkComponents() override final;
	virtual void Update() override final;
	void Clear();
	void Add(const Vec2f &begin, const Vec2f &end, const sf::Color &clr);
	void SetWidth(float width);
};
