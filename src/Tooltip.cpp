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
#include "Tooltip.h"
#include "Label.h"
#include "Canvas.h"
#include "Label.h"
#include "Input.h"
#include "Renderer.h"
#include "Game.h"

void Tooltip::OnResize()
{
	mFrame.mSize = mSize;
	mLabel->SetSize({ mSize.x - 2.0f * mFrame.mBorderSize.x, mLabel->GetSize().y });
	mLabel->SetPosition(mFrame.mBorderSize);
}

void Tooltip::DrawSelf()
{
	mVertices.clear();
	PushScaleableFrame(mFrame);
	DrawVertices(mTexture->GetOpenGLTexture());
}

void Tooltip::LinkComponents()
{
	AddChild(mLabel);
}

Tooltip::Tooltip(const shared_ptr<Canvas> &canvas) : Widget(canvas)
{
	auto theme = canvas->GetTheme();

	mTexture = theme->GetTexture();

	mLabel = canvas->Create<Label>();
	mLabel->SetAlignment(TextAlignment::CenterTop);
	mLabel->SetAutoHeight(true);

	mFrame.mTopBorder = theme->GetArea(Theme::Item::Window_TopBorder);
	mFrame.mBottomBorder = theme->GetArea(Theme::Item::Window_BottomBorder);
	mFrame.mRightBorder = theme->GetArea(Theme::Item::Window_RightBorder);
	mFrame.mBack = theme->GetArea(Theme::Item::Window_Back);
	mFrame.mRightBottomCorner = theme->GetArea(Theme::Item::Window_RightBottomCorner);
	mFrame.mLeftBorder = theme->GetArea(Theme::Item::Window_LeftBorder);
	mFrame.mRightTopCorner = theme->GetArea(Theme::Item::Window_RightTopCorner);
	mFrame.mLeftTopCorner = theme->GetArea(Theme::Item::Window_LeftTopCorner);
	mFrame.mLeftBottomCorner = theme->GetArea(Theme::Item::Window_LeftBottomCorner);
	mFrame.mCornerSize = { 12, 12 };
	mFrame.mBorderSize = { 7, 7 };
}

void Tooltip::Update()
{
}

void Tooltip::EarlyUpdate()
{
	// autohide
	SetLocalVisibility(false);
}

void Tooltip::Show(const string &message, const sf::Color &clr)
{
	auto input = game->GetInput();
	auto renderer = game->GetRenderer();
	SetSize({ GetSize().x, mLabel->GetSize().y + mFrame.mBorderSize.y * 2.0f });
	auto pos = renderer->ToVirtualScreen(input->GetMousePosition());
	if (pos.y + mSize.y > game->VirtualScreen.getSize().y)
	{
		pos.y += game->VirtualScreen.getSize().y - (pos.y + mSize.y) - 2 * mFrame.mBorderSize.y;
	}
	SetPosition(pos);
	mLabel->SetText(message);
	mLabel->SetColor(clr);
	SetLocalVisibility(true);
}
