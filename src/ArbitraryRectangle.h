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

/*
a -> b
|    |
d <- c
*/
namespace Math
{
	struct RotatedRectangle
	{
		// corner points (order - a, b, c, d)
		array<Vec2f, 4> points;
		RotatedRectangle();
		RotatedRectangle(const sf::FloatRect & r);
		// construct rectangle with bounds 'size', transformed by worldTransform
		// corner points in such way are given in world coordinates
		RotatedRectangle(const Vec2f &size, const sf::Transform &worldTransform);
		// construct rectangle with bounds 'size', transformed by worldTransform and
		// projected by viewTransform onto screen corner points in such way are given
		// in screen coordinates
		RotatedRectangle(const Vec2f &size, const sf::Transform &worldTransform, const sf::Transform &viewTransform, const sf::IntRect &viewport);
		bool IsPointInside(const Vec2f &m) const;
		bool IsIntersectCircle(const Vec2f &pos, float radius) const;
		bool IsIntersectOther(const RotatedRectangle &other) const;
		bool IsIntersectLine(const Vec2f &a, const Vec2f &b, Vec2f *intPt) const;
		// for the first call min should be set to FLT_MAX, max to -FLT_MAX
		void GetAxisAlignedRectangle(Vec2f &min, Vec2f &max);
		void Serialize(Serializer &sav);
	};
}