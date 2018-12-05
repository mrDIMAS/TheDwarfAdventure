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
#include "PolyLine.h"
#include "Scene.h"

PolyLine::PolyLine()
{
}

PolyLine::PolyLine(const shared_ptr<Scene> &scene) : PolyLine()
{
	mScene = scene;
}

PolyLine::~PolyLine()
{
	DBG_ECHO();
}

void PolyLine::Clear()
{
	mPoints.clear();
}

void PolyLine::AddPoint(const Vec2f &pt)
{
	mPoints.push_back(pt);
}

vector<Vec2f> &PolyLine::GetPoints()
{
	return mPoints;
}

void PolyLine::SetColor(const sf::Color &color)
{
	mColor = color;
}

sf::Color PolyLine::GetColor() const
{
	return mColor;
}

void PolyLine::Serialize(Serializer &sav)
{
	SceneNode::Serialize(sav);
	sav &mPoints;
}

void PolyLine::SetThickness(float t)
{
	mThickness = t;
}

float PolyLine::GetThickness() const
{
	return mThickness;
}

void PolyLine::MakeCircle(const Vec2f &center, float radius)
{
	constexpr float segments = 60.0f;
	for (float angle = 0.0f; angle < 2 * Math::Pi; angle += 2 * Math::Pi / segments)
	{
		float x = center.x + cos(angle) * radius;
		float y = center.y + sin(angle) * radius;
		AddPoint({ x, y });
	}
}

NodeType PolyLine::GetType() const
{
	return NodeType::PolyLine;
}
