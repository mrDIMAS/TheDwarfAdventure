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
#include "LineSet.h"

void LineSet::DrawSelf()
{
	mVertices.clear();
	glLineWidth(mWidth);
	for (auto &line : mLineList)
	{
		mVertices.emplace_back(line.mBegin, line.mColor, Vec2f(0, 0));
		mVertices.emplace_back(line.mEnd, line.mColor, Vec2f(0, 0));
	}
	DrawVertices(0, GL_LINES);
}

LineSet::LineSet(const shared_ptr<Canvas> &canvas) : Widget(canvas), mWidth(3)
{
	SetSize({ 1000, 1000 });
}

void LineSet::LinkComponents()
{
}

void LineSet::Update()
{
}

void LineSet::Clear()
{
	mLineList.clear();
}

void LineSet::Add(const Vec2f &begin, const Vec2f &end, const sf::Color &clr)
{
	mLineList.emplace_back(begin, end, clr);
}

///////////////////////////////////////////////////////////////////////////////
void LineSet::SetWidth(float width)
{
	mWidth = width;
}

LineSet::Line::Line(const Vec2f &begin, const Vec2f &end, const sf::Color &clr)
	: mBegin(begin), mEnd(end), mColor(clr)
{
}
