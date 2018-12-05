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

#include "Serializer.h"
/*
 represent 2d analog of "view frustum"

            O
           /|\
          / | \
         /  |  \
        /   |   \
       /    |    \ 
      /_____|_____\
     A      D      B


 where O - origin (with fov angle at vertex); A, B - corner points
 D - view distance

 extensively used in AI for mob's vision

 can be rotated at arbitrary angle
*/
class ViewTriangle
{
private:
	// see picture
	Vec2f mOrigin;
	Vec2f mPtA, mPtB;
	float mViewDistance { 200 };
	float mFov { 60 };
	// looking direction
	float mAngle { 0 };
	void RecalculateVertices();

public:
	ViewTriangle();
	~ViewTriangle();
	void SetFov(float angle);
	float GetFov() const;
	bool IsPointInside(const Vec2f &pt) const;
	bool IsIntersectCircle(const Vec2f &circlePos, float radius) const;
	void SetOrigin(const Vec2f &origin);
	Vec2f GetOrigin() const;
	void SetAngle(float angle);
	float GetAngle() const;
	Vec2f GetPtA() const;
	Vec2f GetPtB() const;
	void SetViewDistance(float d);
	float GetViewDistance() const;
	void Serialize(Serializer &sav);
};
