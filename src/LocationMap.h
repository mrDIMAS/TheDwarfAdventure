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

#include "Color.h"

class LocationMap
{
private:
	shared_ptr<Texture> mTexture;
	Vec2f mWorldSize;
	float RemapCircleRadiusToMap(float r);
	Vec2f RemapWorldCoordinatesToMap(const Vec2f & p);
public:
	static constexpr Color PassageColor = { 128, 128, 128, 255 };
	static constexpr Color MerchantColor = { 128, 200, 128, 255 };
	static constexpr Color BlacksmithColor = { 200, 200, 128, 255 };
	LocationMap();
	LocationMap(const Vec2f & worldSize);
	shared_ptr<Texture> GetTexture() const;
	// All coordinates of DrawXXX is in world coords
	void DrawRect(const sf::FloatRect &r, const sf::Color &clr);
	void DrawCircle(const Vec2f &p, float radius, const sf::Color &clr);
	void DrawLine(const Vec2f &begin, const Vec2f &end, float width, const sf::Color &clr);
	void DrawPolygon(Math::Polygon & poly, const sf::Color &clr);
	void DrawPixel(const Vec2f & p, const sf::Color & clr);
	void Serialize(Serializer & sav);
};