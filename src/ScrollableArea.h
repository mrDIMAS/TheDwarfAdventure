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
#include "Button.h"
#include "Layout.h"

class ScrollableArea : public Widget
{
private:
	FrameDesc mFrame;
	Layout mLayout;
	sf::FloatRect mScrollBarTC;
	Vec2f mScrollBarPosition;
	Vec2f mScrollBarSize;
	sf::FloatRect mTopBottomCross;
	sf::FloatRect mCenterBorder;
	sf::FloatRect mBottomTopCross;
	shared_ptr<Button> mScrollUp;
	shared_ptr<Button> mScrollTop;
	shared_ptr<Button> mScrollDown;
	shared_ptr<Button> mScrollBottom;
	float mScrollSpeed { 0 };
	float mScrollSlowdown { 0 };
	virtual void DrawSelf() override final;
	virtual void OnResize() override final;
	bool IsScrollBarHovered();
	void ApplyScrolling(float offset, float scrollBarMinY, float scrollBarMaxY, float fieldHeight,
		float contentHeight);
	bool GetMetrics(
		float &scrollBarMinY, float &scrollBarMaxY, float &fieldHeight, float &contentHeight);

public:
	virtual void LinkComponents() override final;
	ScrollableArea(const shared_ptr<Canvas> &scene);
	void Update() override final;
	virtual void AddChild(const shared_ptr<Widget> &child);
	Layout &GetLayout();
	void ReLayout();
	void ResetScrollBar();
	virtual void RemoveAllChildren() override final;
	void ScrollTop();
	void ScrollBottom();
	float GetScrollBarPosition() const;
	void SetScrollBarPosition(float position);
};
