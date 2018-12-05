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
#include "Checkbox.h"
#include "Theme.h"
#include "Canvas.h"
#include "Input.h"
#include "Label.h"
#include "Game.h"

void Checkbox::OnResize()
{
	mLabel->SetSize({ mSize.x - mSize.y, mSize.y });
	mFrame.mPosition.x = mSize.x - mSize.y;
	mCheckFrame.mPosition = mFrame.mPosition + Vec2f(3, 3);
	mCheckFrame.mSize = { mSize.y - 6, mSize.y - 6 };
	// always square
	mFrame.mSize.x = mSize.y;
	mFrame.mSize.y = mFrame.mSize.x;
}

void Checkbox::DrawSelf()
{
	mVertices.clear();

	mFrame.mColor = mColor;
	PushScaleableFrame(mFrame);
	if (mChecked)
	{
		PushScaleableFrame(mCheckFrame);
	}
	DrawVertices(mTexture->GetOpenGLTexture());
}

Checkbox::Checkbox(const shared_ptr<Canvas> &canvas) : Widget(canvas)
{
	mChecked = false;

	auto theme = canvas->GetTheme();

	SetTexture(theme->GetTexture());

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

	mCheckFrame.mTopBorder = theme->GetArea(Theme::Item::Inset_TopBorder);
	mCheckFrame.mBottomBorder = theme->GetArea(Theme::Item::Inset_BottomBorder);
	mCheckFrame.mRightBorder = theme->GetArea(Theme::Item::Inset_RightBorder);
	mCheckFrame.mBack = theme->GetArea(Theme::Item::Inset_Back);
	mCheckFrame.mRightBottomCorner = theme->GetArea(Theme::Item::Inset_RightBottomCorner);
	mCheckFrame.mLeftBorder = theme->GetArea(Theme::Item::Inset_LeftBorder);
	mCheckFrame.mRightTopCorner = theme->GetArea(Theme::Item::Inset_RightTopCorner);
	mCheckFrame.mLeftTopCorner = theme->GetArea(Theme::Item::Inset_LeftTopCorner);
	mCheckFrame.mLeftBottomCorner = theme->GetArea(Theme::Item::Inset_LeftBottomCorner);
	mCheckFrame.mCornerSize = { 7, 7 };
	mCheckFrame.mBorderSize = { 3, 3 };

	mLabel = canvas->Create<Label>();

	SetSize({ 20, 20 });

	SetStencilWriteEnabled(false);
}

Checkbox::~Checkbox()
{
	DBG_ECHO();
}

void Checkbox::Update()
{
	auto input = game->GetInput();
	if (IsHovered())
	{
		if (input->IsMouseButtonUp(sf::Mouse::Left))
		{
			mChecked = !mChecked;
			OnCheckedChanged();
		}
	}
}

void Checkbox::LinkComponents()
{
	AddChild(mLabel);
}

void Checkbox::SetText(const string &text)
{
	mLabel->SetText(text);
}

bool Checkbox::IsChecked() const
{
	return mChecked;
}
