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

#include "Sprite.h"
#include "Event.h"
#include "Text.h"
#include "Canvas.h"
#include "Widget.h"
#include "Label.h"

class Image;

class Button : public Widget
{
private:
	bool mMouseInputEnabled { true };
	shared_ptr<Label> mLabel;
	shared_ptr<Image> mImage;
	sf::Color mHoveredColor;
	sf::Color mPressedColor;
	sf::Color mNormalColor;
	sf::Color mDisabledColor;
	FrameDesc mFrame;
	void OnResize() override final;
	virtual void DrawSelf() override final;

public:
	virtual void LinkComponents() override final;
	Event OnClick;
	Button(const shared_ptr<Canvas> &scene);
	~Button();
	virtual void Update() override final;
	void SetText(const string &t);
	void SetTextSize(int size);
	void SetTextColor(const sf::Color &clr);
	void SetTextAlignment(const TextAlignment &alignment);
	void SetNormalColor(const sf::Color &clr);
	void SetHoveredColor(const sf::Color &clr);
	void SetPressedColor(const sf::Color &clr);
	void SetDisabledColor(const sf::Color &clr);
	// sets colors according to canvas theme
	void ResetColors();
	void SetImage(const shared_ptr<Texture> &texture, const sf::FloatRect &texRect = { 0, 0, 1, 1 });
	void SetMouseInputEnabled(bool state);
};
