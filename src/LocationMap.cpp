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
#include "LocationMap.h"
#include "Texture.h"
#include "Renderer.h"
#include "Game.h"

LocationMap::LocationMap()
{

}

LocationMap::LocationMap(const Vec2f & worldSize) : mWorldSize(worldSize)
{
	int mapWidth = static_cast<int>(worldSize.x / 30);
	int mapHeight = static_cast<int>(worldSize.y / 30);
	mTexture = game->GetRenderer()->CreateTexture(mapWidth, mapHeight, { 0, 0, 0 });
}

shared_ptr<Texture> LocationMap::GetTexture() const
{
	return mTexture;
}

void LocationMap::DrawRect(const sf::FloatRect & r, const sf::Color & clr)
{
	Vec2f position = RemapWorldCoordinatesToMap({ r.left, r.top });
	Vec2f size = RemapWorldCoordinatesToMap({ r.width, r.height });
	mTexture->DrawRect({ position, size }, clr);
}

void LocationMap::DrawCircle(const Vec2f & p, float radius, const sf::Color & clr)
{
	mTexture->DrawCircle(RemapWorldCoordinatesToMap(p), RemapCircleRadiusToMap(radius), clr);
}

void LocationMap::DrawLine(const Vec2f & begin, const Vec2f & end, float width, const sf::Color & clr)
{
	float radius = width / 2;
	auto p = begin;
	Vec2f d = end - begin;
	const int steps = static_cast<int>(Math::Length(d) / radius);
	d /= static_cast<float>(steps);
	for (int i = 0; i < steps; i++)
	{
		p += d;
		DrawCircle(p, radius, clr);
	}
}

void LocationMap::DrawPolygon(Math::Polygon & poly, const sf::Color & clr)
{
	// Remap polygon from global coordinates to texture coordinates
	Math::Polygon remappedPoly;

	for (const auto & pt : poly.GetPoints())
	{
		remappedPoly.AddPoint(RemapWorldCoordinatesToMap(pt));
	}

	// Check each pixel of texture
	for (int row = 0; row < mTexture->GetWidth(); ++row)
	{
		for (int col = 0; col < mTexture->GetHeight(); ++col)
		{
			Vec2f p = Vec2f(Vec2i(col, row));

			// And draw if pixel inside polygon
			if (remappedPoly.Contains(p))
			{
				mTexture->DrawPixel(p, clr);
			}
		}
	}
}

void LocationMap::DrawPixel(const Vec2f & p, const sf::Color & clr)
{
	mTexture->DrawPixel(RemapWorldCoordinatesToMap(p), clr);
}

float LocationMap::RemapCircleRadiusToMap(float r)
{
	// can be wrong
	return r * (mTexture->GetWidth() / mWorldSize.x);
}

Vec2f LocationMap::RemapWorldCoordinatesToMap(const Vec2f & p)
{
	float kx = mTexture->GetWidth() / mWorldSize.x;
	float ky = mTexture->GetHeight() / mWorldSize.y;
	return { p.x * kx, p.y * ky };
}

void LocationMap::Serialize(Serializer & sav)
{
	sav & mTexture;
	sav & mWorldSize;
}
