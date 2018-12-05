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

class Label;

class Tooltip : public Widget
{
private:
	FrameDesc mFrame;
	shared_ptr<Label> mLabel;
	virtual void OnResize() override final;
	virtual void DrawSelf() override final;

public:
	virtual void LinkComponents() override final;
	Tooltip(const shared_ptr<Canvas> &canvas);
	virtual void Update() override final;
	virtual void EarlyUpdate() override final;
	void Show(const string &message, const sf::Color &clr = { 255, 255, 255 });
};
