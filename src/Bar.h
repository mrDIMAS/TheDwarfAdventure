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

#include "Widget.h"

class Bar : public Widget
{
private:
	float mMinLimit { 0 };
	float mMaxLimit { 100 };
	float mNewValue { 100 };
	float mOldValue { 0 };
	sf::Color mBarColor { 255, 0, 0 };
	sf::FloatRect mBarTextureRect;
	FrameDesc mFrame;
	virtual void DrawSelf() override final;

public:
	Bar(const shared_ptr<Canvas> &canvas);
	virtual void LinkComponents() override final;
	void Update() override final;
	void SetBarColor(const sf::Color &color);
	sf::Color GetBarColor() const;
	void SetBarTextureRectangle(const sf::FloatRect &rect);
	void SetValue(float value);
	float GetValue() const;
	void SetMinLimit(float minLimit);
	float GetMinLimit() const;
	void SetMaxLimit(float maxLimit);
	float GetMaxLimit() const;
};
