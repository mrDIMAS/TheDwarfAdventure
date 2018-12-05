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
#include "ListBox.h"
#include "Input.h"
#include "Renderer.h"
#include "Canvas.h"

ListBox::ListBox(const shared_ptr<Canvas> &scene)
	: Widget(scene), mElementHeight(30), mScrollPosition(0), mMaxVisibleElements(0),
	mSelectedItemNumber(0)
{

	auto theme = scene->GetTheme();

	mCanvas = scene;
	mColor = sf::Color { 89, 112, 128 };
	mTexture = theme->GetTexture();
	mTextureRect = theme->GetArea(Theme::Item::ListBox);

	mScrollBar = scene->Create<Button>();
	mScrollBar->SetSize({ 16, 40 });

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

ListBox::~ListBox()
{
	DBG_ECHO();
}

int ListBox::AddItem(const string &item)
{
	THROW_IF_EXPIRED(mCanvas);
	auto canvas = mCanvas.lock();
	auto theme = canvas->GetTheme();
	ListBoxItem newItem(canvas->Create<Image>(), canvas->Create<Label>());
	newItem.mBackground->SetTexture(theme->GetTexture());
	newItem.mBackground->SetTextureRectangle(theme->GetArea(Theme::Item::ListBox_Item));
	newItem.mBackground->SetSize({ mSize.x - 33, mElementHeight });
	newItem.mBackground->SetStencilWriteEnabled(false);
	AddChild(newItem.mBackground);
	newItem.mBackground->SetColor(theme->GetColor(Theme::Item::ListBox_Item));
	newItem.mNormalColor = theme->GetColor(Theme::Item::ListBox_Item);
	newItem.mSelectedColor = theme->GetColor(Theme::Item::ListBox_Item) * sf::Color(200, 200, 200);
	newItem.mText->SetFont(theme->GetFont(Theme::Item::ListBox_Item));
	newItem.mText->SetText(item);
	newItem.mText->SetAlignment(TextAlignment::LeftCenter);
	newItem.mText->SetPosition({ 8, 0 });
	newItem.mText->SetSize(newItem.mBackground->GetSize());
	newItem.mText->SetStencilWriteEnabled(false);
	newItem.mBackground->AddChild(newItem.mText);
	mItems.push_back(newItem);
	RepositionElements();
	return mItems.size() - 1;
}

void ListBox::SetElementHeight(float height)
{
	mElementHeight = height;
}

void ListBox::Update()
{
	if (IsGloballyVisible())
	{
		auto input = game->GetInput();
		auto renderer = game->GetRenderer();
		auto mousePosition = renderer->ToVirtualScreen(input->GetMousePosition());
		const float sbMinLimit = 5;
		const float smMaxLimit = mSize.y - 5 - mScrollBar->GetSize().y;
		const int dItems = mItems.size() - mMaxVisibleElements;

		// scrolling
		if (IsHovered())
		{
			// rough clipping
			/*
			for(auto & element : mItems) {
				if(element.mBackground->GetPosition().y + mElementHeight > GetSize().y
			|| element.mBackground->GetPosition().y < 5) {
					element.mBackground->SetVisible(false);
				} else {
					element.mBackground->SetVisible(true);
				}
			}*/
			if (input->GetMouseWheelSpeed() != 0)
			{
				mScrollPosition -= static_cast<int>(input->GetMouseWheelSpeed());
				mScrollPosition = Math::Clamp(mScrollPosition, 0, dItems);
				RepositionElements();
				mScrollBar->SetPosition(
				{ mScrollBar->GetPosition().x, mScrollPosition * (smMaxLimit - sbMinLimit) / dItems });
			}
		}

		// move scroll bar
		if (mScrollBar->IsHovered())
		{
			if (input->IsMouseButtonPressed(sf::Mouse::Left))
			{
				mScrollBar->Move({ 0, input->GetMouseSpeed().y });
				mScrollPosition =
					static_cast<int>(mScrollBar->GetPosition().y / (smMaxLimit - sbMinLimit) * dItems);
				RepositionElements();
			}
		}

		// clamp scroll bar
		if (mScrollBar->GetPosition().y < 5)
		{
			mScrollBar->SetPosition({ mScrollBar->GetPosition().x, sbMinLimit });
		}
		if (mScrollBar->GetPosition().y > GetSize().y - 5 - mScrollBar->GetSize().y)
		{
			mScrollBar->SetPosition({ mScrollBar->GetPosition().x, smMaxLimit });
		}

		// item selection
		int n = 0;
		for (auto &item : mItems)
		{
			if (item.mBackground->IsHovered())
			{
				if (input->IsMouseButtonUp(sf::Mouse::Left))
				{
					item.mSelected = true;
					item.mBackground->SetColor(item.mSelectedColor);
					mSelectedItemNumber = n;
					OnSelectedItemChanged();
					for (auto &other : mItems)
					{
						if (&item != &other)
						{
							other.mSelected = false;
							other.mBackground->SetColor(other.mNormalColor);
						}
					}
				}
			}
			++n;
		}
	}
}

void ListBox::SetScrollTexture(const shared_ptr<Texture> &texture)
{
	mScrollBar->SetTexture(texture);
}

int ListBox::GetSelectedItemNumber() const
{
	return mSelectedItemNumber;
}

string ListBox::GetItem(int n) const
{
	return mItems[n].mText->GetText();
}

void ListBox::SetItem(int n, const string &item)
{
	mItems[n].mText->SetText(item);
}

void ListBox::ResetSelection()
{
	mSelectedItemNumber = -1;
	for (auto &other : mItems)
	{
		other.mSelected = false;
		other.mBackground->SetColor(other.mNormalColor);
	}
}

void ListBox::OnResize()
{
	const Vec2f roundedSize = { mSize.x, mSize.y - fmodf(mSize.y, mElementHeight + 5) };
	mMaxVisibleElements = static_cast<int>(roundedSize.y / mElementHeight);
	mSize = roundedSize;
	mScrollBar->SetPosition({ mSize.x - 22, 5 });
	RepositionElements();
}

void ListBox::DrawSelf()
{
	mVertices.clear();
	mFrame.mSize = mSize;
	PushScaleableFrame(mFrame);
	DrawVertices(mTexture->GetOpenGLTexture());
}

void ListBox::RepositionElements()
{
	for (int i = 0; i < static_cast<int>(mItems.size()); ++i)
	{
		mItems[i].mBackground->SetPosition({ 5, 5 + (i - mScrollPosition) * mElementHeight });
	}
}

void ListBox::LinkComponents()
{
	AddChild(mScrollBar);
}

ListBoxItem::ListBoxItem() : mSelected(false)
{
}

ListBoxItem::ListBoxItem(const shared_ptr<Image> &background, const shared_ptr<Label> &text)
	: mBackground(background), mText(text), mSelected(false)
{
}
