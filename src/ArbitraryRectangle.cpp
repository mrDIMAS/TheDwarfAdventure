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

namespace Math
{
	RotatedRectangle::RotatedRectangle()
	{
	}

	RotatedRectangle::RotatedRectangle(const sf::FloatRect & r)
	{
		points[0] = { r.left, r.top };
		points[1] = { r.left + r.width, r.top };
		points[2] = { r.left + r.width, r.top + r.height };
		points[3] = { r.left + r.width, r.top };
	}

	RotatedRectangle::RotatedRectangle(const Vec2f & size, const sf::Transform & worldTransform)
	{
		points[0] = worldTransform.transformPoint(0, 0);
		points[1] = worldTransform.transformPoint(size.x, 0);
		points[2] = worldTransform.transformPoint(size);
		points[3] = worldTransform.transformPoint(0, size.y);
	}

	RotatedRectangle::RotatedRectangle(const Vec2f & size, const sf::Transform & worldTransform, const sf::Transform & viewTransform, const sf::IntRect & viewport)
	{
		points[0] = MapWorldToScreen({ 0, 0 }, worldTransform, viewTransform, viewport);
		points[1] = MapWorldToScreen({ size.x, 0 }, worldTransform, viewTransform, viewport);
		points[2] = MapWorldToScreen(size, worldTransform, viewTransform, viewport);
		points[3] = MapWorldToScreen({ 0, size.y }, worldTransform, viewTransform, viewport);
	}

	bool RotatedRectangle::IsPointInside(const Vec2f & m) const
	{
		const auto AB = points[1] - points[0];
		const auto AM = m - points[0];
		const auto BC = points[2] - points[1];
		const auto BM = m - points[1];
		const auto dotABAM = Dot(AB, AM);
		const auto dotABAB = Dot(AB, AB);
		const auto dotBCBM = Dot(BC, BM);
		const auto dotBCBC = Dot(BC, BC);
		return 0 < dotABAM && dotABAM < dotABAB && 0 < dotBCBM && dotBCBM < dotBCBC;
	}

	bool RotatedRectangle::IsIntersectCircle(const Vec2f & pos, float radius) const
	{
		const float sqrRadius = radius * radius;

		const bool centerInside = IsPointInside(pos);

		if (centerInside)
		{
			return true;
		}

		for (auto &p : points)
		{
			if (Math::IsPointInsideCircle(p, pos, radius))
			{
				return true;
			}
		}

		Math::Edge edges[] = { { points[0], points[1] }, { points[1], points[2] }, { points[2], points[3] },
		{ points[3], points[0] } };
		for (auto &edge : edges)
		{
			// project circle position onto each egde
			const auto projection = Math::ProjectPointOnLine(pos, edge.mA, edge.mB);

			// if projection lies on edge
			if (Math::IsPointOnLine(projection, edge.mA, edge.mB))
			{
				// check distance to projection
				if (Math::SqrDistance(projection, pos) <= sqrRadius)
				{
					// if less than circle's radius, we have intersection
					return true;
				}
			}
		}

		return false;
	}

	bool RotatedRectangle::IsIntersectOther(const RotatedRectangle & other) const
	{
		for (int polyi = 0; polyi < 2; ++polyi)
		{
			const auto &polygon = polyi == 0 ? this->points : other.points;

			for (size_t i1 = 0; i1 < polygon.size(); ++i1)
			{
				const int i2 = (i1 + 1) % polygon.size();

				Vec2f normal = { polygon[i2].y - polygon[i1].y, polygon[i1].x - polygon[i2].x };

				float minA = numeric_limits<float>::max();
				float maxA = numeric_limits<float>::min();
				for (auto &pt : points)
				{
					auto projected = Dot(normal, pt);
					minA = Min(minA, projected);
					maxA = Max(maxA, projected);
				}

				float minB = numeric_limits<float>::max();
				float maxB = numeric_limits<float>::min();
				for (auto &pt : other.points)
				{
					auto projected = Dot(normal, pt);
					minB = Min(minB, projected);
					maxB = Max(maxB, projected);
				}

				if (maxA < minB || maxB < minA)
				{
					return false;
				}
			}
		}

		return true;
	}

	bool RotatedRectangle::IsIntersectLine(const Vec2f & a, const Vec2f & b, Vec2f * intPt) const
	{
		Math::Line edges[] = { { points[0], points[1] }, { points[1], points[2] }, { points[2], points[3] },
		{ points[3], points[0] } };

		bool intersection = false;
		Vec2f closest = { FLT_MAX, FLT_MAX };
		for (auto &edge : edges)
		{
			Vec2f p;
			if (IsLineIntersectLine(a, b, edge.mBegin, edge.mEnd, p))
			{
				if (Math::SqrDistance(a, p) < Math::SqrDistance(a, closest))
				{
					closest = p;
				}
				intersection = true;
			}
		}

		if (intPt)
		{
			*intPt = closest;
		}

		return intersection;
	}

	void RotatedRectangle::GetAxisAlignedRectangle(Vec2f & min, Vec2f & max)
	{
		for (auto &pt : points)
		{
			if (pt.x < min.x)
			{
				min.x = pt.x;
			}
			if (pt.y < min.y)
			{
				min.y = pt.y;
			}

			if (pt.x > max.x)
			{
				max.x = pt.x;
			}
			if (pt.y > max.y)
			{
				max.y = pt.y;
			}
		}
	}
	void RotatedRectangle::Serialize(Serializer & sav)
	{
		for (auto &pt : points)
		{
			sav &pt;
		}
	}
}
