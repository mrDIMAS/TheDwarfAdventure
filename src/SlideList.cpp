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
#include "SlideList.h"

void SlideList::OnResize()
{
	mTextField->SetSize(mSize);

	mPrev->SetSize(Vec2f(mSize.y, mSize.y) - 2.0f * mFrame.mBorderSize);
	mPrev->SetPosition(mFrame.mBorderSize);

	mNext->SetSize(Vec2f(mSize.y, mSize.y) - 2.0f * mFrame.mBorderSize);
	mNext->SetPosition({ mSize.x - mSize.y + mFrame.mBorderSize.x, mFrame.mBorderSize.y });
}

void SlideList::DrawSelf()
{
	mVertices.clear();
	mFrame.mSize = mSize;

	PushScaleableFrame(mFrame);
	DrawVertices(mTexture->GetOpenGLTexture());
}

void SlideList::LinkComponents()
{
	AddChild(mNext);
	AddChild(mTextField);
	AddChild(mPrev);
}

SlideList::SlideList(const shared_ptr<Canvas> &scene) : Widget(scene), mCurrentItem(0), mLastItem(0)
{
	auto theme = scene->GetTheme();

	mTexture = theme->GetTexture();
	mTextureRect = theme->GetArea(Theme::Item::SlideList);

	mPrev = scene->Create<Button>();
	mPrev->SetText("<");

	mPrev->OnClick += [this]
	{
		--mCurrentItem;
		if (mCurrentItem < 0)
		{
			mCurrentItem = 0;
		}
		mTextField->SetText(mItems[mCurrentItem].first);
		if (mCurrentItem != mLastItem)
		{
			OnCurrentItemChanged();
		}
		mLastItem = mCurrentItem;
	};

	mNext = scene->Create<Button>();
	mNext->SetText(">");

	mNext->OnClick += [this]
	{
		++mCurrentItem;
		if (mCurrentItem >= static_cast<int>(mItems.size()))
		{
			mCurrentItem = mItems.size() - 1;
		}

		mTextField->SetText(mItems[mCurrentItem].first);
		if (mCurrentItem != mLastItem)
		{
			OnCurrentItemChanged();
		}
		mLastItem = mCurrentItem;
	};

	mTextField = scene->Create<Label>();
	mTextField->SetFont(theme->GetFont(Theme::Item::SlideList));
	mTextField->SetAlignment(TextAlignment::Center);

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

SlideList::~SlideList()
{
	DBG_ECHO();
}

void SlideList::AddItem(const string &name, const shared_ptr<void> &item)
{
	mItems.push_back(make_pair(name, item));
	mCurrentItem = mItems.size() - 1;
	mLastItem = mCurrentItem;
	mTextField->SetText(name);
}

void SlideList::Update()
{
}

int SlideList::GetCurrentItemIndex() const
{
	return mCurrentItem;
}

shared_ptr<void> SlideList::GetCurrentItem() const
{
	return mItems.at(mCurrentItem).second;
}

void SlideList::SetCurrentItem(int n)
{
	mCurrentItem = Math::Clamp(n, 0, (int)mItems.size() - 1);
	mLastItem = mCurrentItem;
	mTextField->SetText(mItems[mCurrentItem].first);
}
