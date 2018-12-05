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
#include "Button.h"
#include "Input.h"
#include "Renderer.h"
#include "Sound.h"
#include "Image.h"

void Button::OnResize()
{
	mLabel->SetSize(mSize);
	if (mImage)
	{
		const auto offset = Vec2f(3, 3);
		mImage->SetSize(mSize - 2.0f * offset);
	}
}

void Button::DrawSelf()
{
	mVertices.clear();
	mFrame.mSize = mSize;
	mFrame.mColor = mColor;
	PushScaleableFrame(mFrame);
	DrawVertices(mTexture->GetOpenGLTexture());
}

void Button::LinkComponents()
{
	AddChild(mLabel);
}

Button::Button(const shared_ptr<Canvas> &scene) : Widget(scene)
{
	auto theme = scene->GetTheme();

	mTexture = theme->GetTexture();

	mNormalColor = theme->GetColor(Theme::Item::Button);
	mHoveredColor = theme->GetColor(Theme::Item::Button_Hovered);
	mPressedColor = theme->GetColor(Theme::Item::Button_Pressed);
	mDisabledColor = theme->GetColor(Theme::Item::Button_Disabled);

	mLabel = scene->Create<Label>();
	mLabel->SetFont(theme->GetFont(Theme::Item::Button));

	OnMouseEnter += [this]
	{
		if (IsEnabled())
		{
			auto snd = game->EmitSound2D("data/sounds/menupick.ogg");
			snd->Play();
			snd->SetVolume(40);
		}
	};

	OnClick += [this]
	{
		auto snd = game->EmitSound2D("data/sounds/menuhit.ogg");
		snd->Play();
		snd->SetVolume(70);
	};

	mFrame.mTopBorder = theme->GetArea(Theme::Item::Button_TopBorder);
	mFrame.mBottomBorder = theme->GetArea(Theme::Item::Button_BottomBorder);
	mFrame.mRightBorder = theme->GetArea(Theme::Item::Button_RightBorder);
	mFrame.mBack = theme->GetArea(Theme::Item::Button_Back);
	mFrame.mRightBottomCorner = theme->GetArea(Theme::Item::Button_RightBottomCorner);
	mFrame.mLeftBorder = theme->GetArea(Theme::Item::Button_LeftBorder);
	mFrame.mRightTopCorner = theme->GetArea(Theme::Item::Button_RightTopCorner);
	mFrame.mLeftTopCorner = theme->GetArea(Theme::Item::Button_LeftTopCorner);
	mFrame.mLeftBottomCorner = theme->GetArea(Theme::Item::Button_LeftBottomCorner);
	mFrame.mCornerSize = { 6, 6 };
	mFrame.mBorderSize = { 2, 2 };
}

Button::~Button()
{
	DBG_ECHO();
}

void Button::Update()
{
	if (mEnabled)
	{
		if (mMouseInputEnabled)
		{
			if (IsGloballyVisible())
			{
				auto input = game->GetInput();
				if (IsHovered())
				{
					if (input->IsMouseButtonPressed(sf::Mouse::Left))
					{
						mColor = mPressedColor;
					}
					else
					{
						mColor = mHoveredColor;
					}
					if (input->IsMouseButtonUp(sf::Mouse::Left))
					{
						OnClick();
					}
				}
				else
				{
					mColor = mNormalColor;
				}
			}
		}
		else
		{
			mColor = mNormalColor;
		}
	}
	else
	{
		mColor = mDisabledColor;
	}
}

void Button::SetText(const string &t)
{
	mLabel->SetText(t);
	mLabel->SetAlignment(TextAlignment::Center);
}

void Button::SetTextSize(int size)
{
	mLabel->SetCharacterSize(size);
}

void Button::SetTextColor(const sf::Color &clr)
{
	mLabel->SetColor(clr);
}

void Button::SetTextAlignment(const TextAlignment &alignment)
{
	mLabel->SetAlignment(alignment);
}

void Button::SetNormalColor(const sf::Color &clr)
{
	mNormalColor = clr;
}

void Button::SetHoveredColor(const sf::Color &clr)
{
	mHoveredColor = clr;
}

void Button::SetPressedColor(const sf::Color &clr)
{
	mPressedColor = clr;
}

void Button::SetDisabledColor(const sf::Color &clr)
{
	mDisabledColor = clr;
}

void Button::ResetColors()
{
	auto theme = GetCanvas()->GetTheme();
	mNormalColor = theme->GetColor(Theme::Item::Button);
	mHoveredColor = theme->GetColor(Theme::Item::Button_Hovered);
	mPressedColor = theme->GetColor(Theme::Item::Button_Pressed);
	mDisabledColor = theme->GetColor(Theme::Item::Button_Disabled);
}

void Button::SetImage(const shared_ptr<Texture> &texture, const sf::FloatRect &texRect)
{
	const auto offset = Vec2f(3, 3);
	if (!mImage)
	{
		mImage = GetCanvas()->Create<Image>();
	}
	mImage->SetTexture(texture);
	mImage->SetSize(mSize - 2.0f * offset);
	mImage->SetPosition(offset);
	mImage->SetTextureRectangle(texRect);
	AddChild(mImage);
}

void Button::SetMouseInputEnabled(bool state)
{
	mMouseInputEnabled = state;
}
