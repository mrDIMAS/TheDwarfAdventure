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
#include "Frame.h"
#include "Canvas.h"
#include "Game.h"

void Frame::OnResize()
{
	mFrame.mSize = mSize;
}

void Frame::DrawSelf()
{
	mVertices.clear();
	mFrame.mColor = mColor;
	PushScaleableFrame(mFrame);
	DrawVertices(mTexture->GetOpenGLTexture());
}

Frame::Frame(const shared_ptr<Canvas> &canvas) : Widget(canvas)
{
	auto theme = canvas->GetTheme();
	mTexture = theme->GetTexture();

	mFrame.mTopBorder = theme->GetArea(Theme::Item::Control_TopBorder);
	mFrame.mBottomBorder = theme->GetArea(Theme::Item::Control_BottomBorder);
	mFrame.mRightBorder = theme->GetArea(Theme::Item::Control_RightBorder);
	mFrame.mBack = theme->GetArea(Theme::Item::Control_Back);
	mFrame.mRightBottomCorner = theme->GetArea(Theme::Item::Control_RightBottomCorner);
	mFrame.mLeftBorder = theme->GetArea(Theme::Item::Control_LeftBorder);
	mFrame.mRightTopCorner = theme->GetArea(Theme::Item::Control_RightTopCorner);
	mFrame.mLeftTopCorner = theme->GetArea(Theme::Item::Control_LeftTopCorner);
	mFrame.mLeftBottomCorner = theme->GetArea(Theme::Item::Control_LeftBottomCorner);
	mFrame.mCornerSize = { 6, 6 };
	mFrame.mBorderSize = { 2, 2 };
}

void Frame::Update()
{
}

void Frame::LinkComponents()
{
}

void Frame::SetInCenter()
{
	SetPosition(game->VirtualScreen.getCenter() - GetSize() * 0.5f);
}
