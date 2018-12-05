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
#include "ViewTriangle.h"

void ViewTriangle::RecalculateVertices()
{
	float halfFov = 0.5f * mFov;
	Vec2f defaultDir = { mViewDistance / sin(Math::Deg2Rad(halfFov)), 0 };

	Vec2f dirA = Math::RotateVector(defaultDir, halfFov);
	Vec2f dirB = Math::RotateVector(defaultDir, -halfFov);

	dirA = Math::RotateVector(dirA, mAngle);
	dirB = Math::RotateVector(dirB, mAngle);

	mPtA = mOrigin + dirA;
	mPtB = mOrigin + dirB;
}

ViewTriangle::ViewTriangle()
{
	RecalculateVertices();
}

ViewTriangle::~ViewTriangle()
{
}

void ViewTriangle::SetFov(float angle)
{
	mFov = Math::Clamp(angle, 10.0f, 179.0f);
	RecalculateVertices();
}

float ViewTriangle::GetFov() const
{
	return mFov;
}

bool ViewTriangle::IsPointInside(const Vec2f &pt) const
{
	// use barycentric coordinates for check
	auto v0 = mPtB - mOrigin;
	auto v1 = mPtA - mOrigin;
	auto v2 = pt - mOrigin;

	float d00 = Math::Dot(v0, v0);
	float d01 = Math::Dot(v0, v1);
	float d11 = Math::Dot(v1, v1);
	float d20 = Math::Dot(v2, v0);
	float d21 = Math::Dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;

	if (abs(denom) <= Math::Epsilon)
	{
		return false;
	}

	float invDenom = 1.0f / denom;

	float v = (d11 * d20 - d01 * d21) * invDenom;
	float w = (d00 * d21 - d01 * d20) * invDenom;
	float u = 1.0f - v - w;

	return (u >= 0.0f) && (v >= 0.0f) && (u + v < 1.0f);
}

bool ViewTriangle::IsIntersectCircle(const Vec2f &circlePos, float radius) const
{
	// case 1: any point of triangle is inside circle
	bool anyVertexInside = Math::IsPointInsideCircle(mOrigin, circlePos, radius) ||
		Math::IsPointInsideCircle(mPtA, circlePos, radius) ||
		Math::IsPointInsideCircle(mPtB, circlePos, radius);
	if (anyVertexInside)
	{
		return true;
	}

	// case 2: circle's center is inside triangle
	bool circleCenterInside = IsPointInside(circlePos);
	if (circleCenterInside)
	{
		return true;
	}

	// case 3 (worst speed): circle touch triangle
	float sqrRadius = Math::Sqr(radius);

	// define triangle's edges
	Math::Edge edges[] = {
		{ mOrigin, mPtB }, // OB
		{ mPtB, mPtA },    // BA
		{ mPtA, mOrigin }  // AO
	};

	for (auto &edge : edges)
	{
		// project circle position onto each egde
		auto projection = Math::ProjectPointOnLine(circlePos, edge.mA, edge.mB);

		// if projection lies on edge
		if (Math::IsPointOnLine(projection, edge.mA, edge.mB))
		{
			// check distance to projection
			if (Math::SqrDistance(projection, circlePos) <= sqrRadius)
			{
				// if less than circle's radius, we have intersection
				return true;
			}
		}
	}

	return false;
}

void ViewTriangle::SetOrigin(const Vec2f &origin)
{
	mOrigin = origin;
	RecalculateVertices();
}

Vec2f ViewTriangle::GetOrigin() const
{
	return mOrigin;
}

void ViewTriangle::SetAngle(float angle)
{
	mAngle = angle;
	RecalculateVertices();
}

float ViewTriangle::GetAngle() const
{
	return mAngle;
}

Vec2f ViewTriangle::GetPtA() const
{
	return mPtA;
}

Vec2f ViewTriangle::GetPtB() const
{
	return mPtB;
}

void ViewTriangle::SetViewDistance(float d)
{
	mViewDistance = d;
	RecalculateVertices();
}

float ViewTriangle::GetViewDistance() const
{
	return mViewDistance;
}

void ViewTriangle::Serialize(Serializer &sav)
{
	sav &mOrigin;
	sav &mPtA;
	sav &mPtB;
	sav &mViewDistance;
	sav &mFov;
	sav &mAngle;
}
