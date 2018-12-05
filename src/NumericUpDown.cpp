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
#include "NumericUpDown.h"

void NumericUpDown::OnResize()
{
	mTextField->SetSize(mSize);

	mPrev->SetSize(Vec2f(mSize.y, mSize.y) - 2.0f * mFrame.mBorderSize);
	mPrev->SetPosition(mFrame.mBorderSize);

	mNext->SetSize(Vec2f(mSize.y, mSize.y) - 2.0f * mFrame.mBorderSize);
	mNext->SetPosition({ mSize.x - mSize.y + mFrame.mBorderSize.x, mFrame.mBorderSize.y });
}

void NumericUpDown::DrawSelf()
{
	mVertices.clear();
	mFrame.mSize = mSize;
	PushScaleableFrame(mFrame);
	DrawVertices(mTexture->GetOpenGLTexture());
}

void NumericUpDown::LinkComponents()
{
	AddChild(mPrev);
	AddChild(mNext);
	AddChild(mTextField);
}

NumericUpDown::NumericUpDown(const shared_ptr<Canvas> &canvas)
	: Widget(canvas), mMinLimit(0.0f), mMaxLimit(1.0f), mValue(0.0f), mStep(0.1f),
	mDecimalPlaces(0)
{

	auto theme = canvas->GetTheme();

	mTexture = theme->GetTexture();
	mTextureRect = theme->GetArea(Theme::Item::SlideList);

	mTextField = canvas->Create<Label>();
	mTextField->SetAlignment(TextAlignment::Center);

	mPrev = canvas->Create<Button>();
	mPrev->SetText("-");

	mPrev->OnClick += [this]
	{
		if (DecreaseCondition)
		{
			if (DecreaseCondition())
			{
				if (mValue > mMinLimit)
				{
					SetValue(mValue - mStep);
					OnDecrease();
					OnChange();
				}
			}
		}
		else
		{
			if (mValue > mMinLimit)
			{
				SetValue(mValue - mStep);
				OnDecrease();
				OnChange();
			}
		}
	};

	mNext = canvas->Create<Button>();
	mNext->SetText("+");

	mNext->OnClick += [this]
	{
		if (IncreaseCondition)
		{     // check
			if (IncreaseCondition())
			{ // actual call
				if (mValue < mMaxLimit)
				{
					SetValue(mValue + mStep);
					OnIncrease();
					OnChange();
				}
			}
		}
		else
		{
			if (mValue < mMaxLimit)
			{
				SetValue(mValue + mStep);
				OnIncrease();
				OnChange();
			}
		}
	};

	SetValue(0.0f);

	mFrame.mTopBorder = theme->GetArea(Theme::Item::Inset_TopBorder);
	mFrame.mBottomBorder = theme->GetArea(Theme::Item::Inset_BottomBorder);
	mFrame.mRightBorder = theme->GetArea(Theme::Item::Inset_RightBorder);
	mFrame.mBack = theme->GetArea(Theme::Item::Inset_Back);
	mFrame.mRightBottomCorner = theme->GetArea(Theme::Item::Inset_RightBottomCorner);
	mFrame.mLeftBorder = theme->GetArea(Theme::Item::Inset_LeftBorder);
	mFrame.mRightTopCorner = theme->GetArea(Theme::Item::Inset_RightTopCorner);
	mFrame.mLeftTopCorner = theme->GetArea(Theme::Item::Inset_LeftTopCorner);
	mFrame.mLeftBottomCorner = theme->GetArea(Theme::Item::Inset_LeftBottomCorner);

	mFrame.mCornerSize = { 7, 7 };
	mFrame.mBorderSize = { 3, 3 };
}

NumericUpDown::~NumericUpDown()
{
	DBG_ECHO();
}

void NumericUpDown::Update()
{
}

void NumericUpDown::SetValue(float value)
{
	mValue = Math::Clamp(value, mMinLimit, mMaxLimit);
	if (mAdditionalText.size())
	{
		mTextField->SetText(
			StringBuilder() << RoundAndConvert(mValue, mDecimalPlaces) << " " << mAdditionalText);
	}
	else
	{
		mTextField->SetText(RoundAndConvert(mValue, mDecimalPlaces));
	}
}

float NumericUpDown::GetValue() const
{
	return mValue;
}

void NumericUpDown::SetMinLimit(float value)
{
	mMinLimit = value;
	if (mValue < mMinLimit)
	{
		SetValue(mMinLimit);
	}
}

float NumericUpDown::GetMinLimit() const
{
	return mMinLimit;
}

void NumericUpDown::SetMaxLimit(float value)
{
	mMaxLimit = value;
	if (mValue > mMaxLimit)
	{
		SetValue(mMaxLimit);
	}
}

float NumericUpDown::GetMaxLimit() const
{
	return mMaxLimit;
}

void NumericUpDown::SetStep(float step)
{
	mStep = step;
}

float NumericUpDown::GetStep() const
{
	return mStep;
}

void NumericUpDown::SetDecimalPlaces(int count)
{
	mDecimalPlaces = count;
}

int NumericUpDown::GetDecimalPlaces() const
{
	return mDecimalPlaces;
}

void NumericUpDown::SetAdditionalText(const string &str)
{
	mAdditionalText = str;
	SetValue(mValue);
}

string NumericUpDown::GetAdditionalText() const
{
	return mAdditionalText;
}

void NumericUpDown::SetEnabled(bool state)
{
	Widget::SetEnabled(state);
	mPrev->SetEnabled(state);
	mNext->SetEnabled(state);
}
