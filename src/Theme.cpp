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
#include "Theme.h"

Theme::Theme()
{
}

Theme::Theme(const shared_ptr<Texture> &texture) : mTexture(texture)
{
}

Theme::~Theme()
{
	DBG_ECHO();
}

void Theme::SetArea(
	const Item &item, const sf::Vector2i &leftUpCorner, const sf::Vector2i &rightBottomCorner)
{
	auto &area = mAreaList[item].mArea;
	area.left = static_cast<float>(leftUpCorner.x) / static_cast<float>(mTexture->GetWidth());
	area.top = static_cast<float>(leftUpCorner.y) / static_cast<float>(mTexture->GetHeight());
	area.width = static_cast<float>(rightBottomCorner.x - leftUpCorner.x) /
		static_cast<float>(mTexture->GetWidth());
	area.height = static_cast<float>(rightBottomCorner.y - leftUpCorner.y) /
		static_cast<float>(mTexture->GetHeight());
}

void Theme::SetArea(const Item &item, const sf::IntRect &area)
{
	auto &itemArea = mAreaList[item].mArea;
	itemArea.left = static_cast<float>(area.left) / static_cast<float>(mTexture->GetWidth());
	itemArea.top = static_cast<float>(area.top) / static_cast<float>(mTexture->GetHeight());
	itemArea.width = static_cast<float>(area.width) / static_cast<float>(mTexture->GetWidth());
	itemArea.height = static_cast<float>(area.height) / static_cast<float>(mTexture->GetHeight());
}

void Theme::SetColor(const Item &item, const sf::Color &color)
{
	mAreaList[item].mColor = color;
}

sf::Color Theme::GetColor(const Item &item)
{
	return mAreaList[item].mColor;
}

sf::FloatRect Theme::GetArea(const Item &item)
{
	return mAreaList[item].mArea;
}

void Theme::SetTexture(const shared_ptr<Texture> &texture)
{
	mTexture = texture;
}

shared_ptr<Texture> Theme::GetTexture() const
{
	return mTexture;
}
shared_ptr<sf::Font> Theme::GetFont(const Item &item)
{
	return mAreaList[item].mFont;
}

void Theme::SetFont(const Item &item, const shared_ptr<sf::Font> &font)
{
	mAreaList[item].mFont = font;
}
