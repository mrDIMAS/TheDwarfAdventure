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

#include "Texture.h"

struct ThemeElement
{
	sf::FloatRect mArea;
	sf::Color mColor;
	shared_ptr<sf::Font> mFont;
};

class Theme
{
public:
	enum class Item
	{
		Window, // texture, font, color
		Window_TopBorder,
		Window_BottomBorder,
		Window_RightBorder,
		Window_Cross,
		Window_Back,
		Window_RightBottomCorner,
		Window_LeftBorder,
		Window_RightTopCorner,
		Window_LeftTopCorner,
		Window_RightLeftCross,
		Window_LeftRightCross,
		Window_TopBottomCross,
		Window_BottomTopCross,
		Window_LeftBottomCorner,

		Inset_TopBorder,
		Inset_BottomBorder,
		Inset_RightBorder,
		Inset_Back,
		Inset_RightBottomCorner,
		Inset_LeftBorder,
		Inset_RightTopCorner,
		Inset_LeftTopCorner,
		Inset_LeftBottomCorner,
		Inset_TopBottomCross,
		Inset_BottomTopCross,
		Inset_CenterBorder,

		Button_TopBorder,
		Button_BottomBorder,
		Button_RightBorder,
		Button_Back,
		Button_RightBottomCorner,
		Button_LeftBorder,
		Button_RightTopCorner,
		Button_LeftTopCorner,
		Button_LeftBottomCorner,

		Control_TopBorder,
		Control_BottomBorder,
		Control_RightBorder,
		Control_Back,
		Control_RightBottomCorner,
		Control_LeftBorder,
		Control_RightTopCorner,
		Control_LeftTopCorner,
		Control_LeftBottomCorner,

		ScrollDown,
		ScrollUp,
		ScrollTop,
		ScrollBottom,

		ScrollBar,
		Next,
		Previous,
		Plus,
		Minus,

		Button,          // texture, font, color
		Button_Hovered,  // texture, color
		Button_Pressed,  // texture, color
		Button_Disabled, // texture, color

		SlideList, // texture, font, color

		ListBox,           // texture, font, color
		ListBox_Scrollbar, // texture, color
		ListBox_Item,      // texture, color

		HUD_Bars,    // texture only
		HUD_Minimap, // texture only
		Count,
	};

private:
	shared_ptr<Texture> mTexture;
	unordered_map<Item, ThemeElement> mAreaList;

public:
	Theme();
	Theme(const shared_ptr<Texture> &texture);
	~Theme();
	void SetArea(
		const Item &item, const sf::Vector2i &leftUpCorner, const sf::Vector2i &rightBottomCorner);
	void SetArea(const Item &item, const sf::IntRect &area);
	void SetColor(const Item &item, const sf::Color &color);
	sf::Color GetColor(const Item &item);
	sf::FloatRect GetArea(const Item &item);
	void SetTexture(const shared_ptr<Texture> &texture);
	shared_ptr<Texture> GetTexture() const;
	shared_ptr<sf::Font> GetFont(const Item &item);
	void SetFont(const Item &item, const shared_ptr<sf::Font> &font);
};
