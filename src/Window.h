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
#include "Label.h"
#include "Canvas.h"
#include "Widget.h"
#include "ScrollableArea.h"

class Window : public Widget
{
protected:
	FrameDesc mFrame;
	static constexpr float mCaptionHeight = 40;
	shared_ptr<Label> mCaption;
	shared_ptr<ScrollableArea> mClientArea;
	shared_ptr<Button> mHelpButton;
	bool mAutoLayouting;
	void OnResize() override final;
	virtual void DrawSelf() override final;

public:
	virtual void AddChild(const shared_ptr<Widget> &child);
	virtual void LinkComponents() override final;
	Window(const shared_ptr<Canvas> &scene);
	~Window();
	void SetCaption(const string &caption);
	void Update() override final;
	Layout &GetLayout();
	void SetAutoLayoutingEnabled(bool state);
	bool IsAutoLayoutingEnabled() const;
	shared_ptr<ScrollableArea> GetClientArea() const;
	void SetInCenter();
	void AddHelpButton();
	shared_ptr<Button> GetHelpButton();
};
