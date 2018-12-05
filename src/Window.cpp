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
#include "Window.h"
#include "Label.h"
#include "Renderer.h"
#include "Input.h"
#include "ListBox.h"
#include "Button.h"

void Window::OnResize()
{
	mCaption->SetSize({ mSize.x, mCaptionHeight });
	mCaption->SetPosition({ 0, 0 });
	mClientArea->SetSize(
	{ mSize.x - 2 * mFrame.mCornerSize.x, mSize.y - mCaptionHeight - 2 * mFrame.mBorderSize.y });
	mClientArea->SetPosition({ mFrame.mCornerSize.x, mClientArea->GetPosition().y });
}

void Window::DrawSelf()
{
	mFrame.mSize = mSize;

	mVertices.clear();
	PushScaleableFrame(mFrame);
	DrawVertices(mTexture->GetOpenGLTexture());
}

void Window::AddChild(const shared_ptr<Widget> &child)
{
	if (child != mClientArea && child != mCaption && child != mHelpButton && mAutoLayouting)
	{
		// delegate child to scrollable area
		mClientArea->AddChild(child);
	}
	else
	{
		// attach self components as usual
		Widget::AddChild(child);
	}
}

void Window::LinkComponents()
{
	AddChild(mCaption);
	AddChild(mClientArea);
}

Window::Window(const shared_ptr<Canvas> &scene) : Widget(scene), mAutoLayouting(true)
{
	auto theme = scene->GetTheme();

	mTexture = theme->GetTexture();
	mTextureRect = theme->GetArea(Theme::Item::Window);
	mColor = theme->GetColor(Theme::Item::Window);

	mCaption = scene->Create<Label>();
	mCaption->SetAlignment(TextAlignment::Center);
	mCaption->SetFont(theme->GetFont(Theme::Item::Window));

	mClientArea = scene->Create<ScrollableArea>();
	mClientArea->SetPosition({ 0, mCaptionHeight });

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

Window::~Window()
{
	DBG_ECHO();
}

void Window::SetCaption(const string &caption)
{
	mCaption->SetText(caption);
	mCaption->SetSize({ mSize.x, mCaptionHeight });
}

void Window::Update()
{
}

Layout &Window::GetLayout()
{
	return mClientArea->GetLayout();
}

void Window::SetAutoLayoutingEnabled(bool state)
{
	mAutoLayouting = state;
	mClientArea->SetLocalVisibility(state);
}

bool Window::IsAutoLayoutingEnabled() const
{
	return mAutoLayouting;
}

shared_ptr<ScrollableArea> Window::GetClientArea() const
{
	return mClientArea;
}

void Window::SetInCenter()
{
	SetPosition(game->VirtualScreen.getCenter() - GetSize() * 0.5f);
}

void Window::AddHelpButton()
{
	if (!mHelpButton)
	{
		mHelpButton = GetCanvas()->Create<Button>();
		mHelpButton->SetText("?");
		mHelpButton->SetSize({ 22, 22 });
		mHelpButton->SetPosition({ mFrame.mBorderSize.x, mFrame.mBorderSize.y });
		AddChild(mHelpButton);
	}
}

shared_ptr<Button> Window::GetHelpButton()
{
	return mHelpButton;
}
