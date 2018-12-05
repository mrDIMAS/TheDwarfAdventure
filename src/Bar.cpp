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
#include "Bar.h"
#include "Texture.h"
#include "Canvas.h"

void Bar::DrawSelf()
{
	sf::Color newColor = mBarColor;
	sf::Color oldColor;
	oldColor.r = static_cast<uint8_t>(mBarColor.r * 0.8f);
	oldColor.g = static_cast<uint8_t>(mBarColor.g * 0.8f);
	oldColor.b = static_cast<uint8_t>(mBarColor.b * 0.8f);
	mOldValue += (mNewValue - mOldValue) * 0.025f;
	const float borderWidth = mFrame.mBorderSize.x;
	const float newK = Math::Clamp((mNewValue - mMinLimit) / (mMaxLimit - mMinLimit), 0.0f, 1.0f);
	const float oldK = Math::Clamp((mOldValue - mMinLimit) / (mMaxLimit - mMinLimit), 0.0f, 1.0f);
	const Vec2f newBarSize = { newK * (mSize.x - borderWidth * 2.0f), mSize.y - borderWidth * 2.0f };
	const Vec2f oldBarSize = { oldK * (mSize.x - borderWidth * 2.0f), mSize.y - borderWidth * 2.0f };
	mVertices.clear();
	mFrame.mSize = mSize;
	mFrame.mColor = mColor;
	PushScaleableFrame(mFrame);
	Vec2f pos = mGlobalPosition + Vec2f(borderWidth, borderWidth);

	PushQuad(pos, oldBarSize, mBarTextureRect, oldColor);
	PushQuad(pos, newBarSize, mBarTextureRect, newColor);

	DrawVertices(mTexture->GetOpenGLTexture());
}

Bar::Bar(const shared_ptr<Canvas> &canvas) : Widget(canvas)
{
	auto theme = canvas->GetTheme();

	mTexture = theme->GetTexture();

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

void Bar::LinkComponents()
{
}

void Bar::Update()
{
}

void Bar::SetBarColor(const sf::Color &color)
{
	mBarColor = color;
}

sf::Color Bar::GetBarColor() const
{
	return mBarColor;
}

void Bar::SetBarTextureRectangle(const sf::FloatRect &rect)
{
	mBarTextureRect = rect;
}

void Bar::SetValue(float value)
{
	mNewValue = Math::Clamp(value, mMinLimit, mMaxLimit);
}

float Bar::GetValue() const
{
	return mNewValue;
}

void Bar::SetMinLimit(float minLimit)
{
	mMinLimit = minLimit;
}

float Bar::GetMinLimit() const
{
	return mMinLimit;
}

void Bar::SetMaxLimit(float maxLimit)
{
	mMaxLimit = maxLimit;
}

float Bar::GetMaxLimit() const
{
	return mMaxLimit;
}
