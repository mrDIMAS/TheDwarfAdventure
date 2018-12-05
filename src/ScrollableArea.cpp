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
#include "ScrollableArea.h"
#include "Renderer.h"
#include "ListBox.h"
#include "Input.h"
#include "Table.h"

void ScrollableArea::DrawSelf()
{
	const auto separatorPos = mGlobalPosition + Vec2f(mScrollBarPosition.x - 8, 0);
	mVertices.clear();
	mFrame.mSize = mSize;

	PushScaleableFrame(mFrame);
	PushQuad(separatorPos, { 5, 4 }, mTopBottomCross);
	PushQuad(separatorPos + Vec2f(1, 4), { 3, mSize.y - 8 }, mCenterBorder);
	PushQuad(separatorPos + Vec2f(0, mSize.y - 4), { 5, 4 }, mBottomTopCross);

	PushQuad(mGlobalPosition + mScrollBarPosition, mScrollBarSize, mScrollBarTC);
	DrawVertices(mTexture->GetOpenGLTexture());
}

void ScrollableArea::OnResize()
{
	mScrollBarPosition.x = mSize.x - 20;
	mLayout.SetWidth(mSize.x - 40);
	mScrollTop->SetPosition({ mScrollBarPosition.x, 0 });
	mScrollUp->SetPosition({ mScrollBarPosition.x, mScrollUp->GetSize().y });
	mScrollDown->SetPosition({ mScrollBarPosition.x, mSize.y - 2 * mScrollDown->GetSize().y });
	mScrollBottom->SetPosition({ mScrollBarPosition.x, mSize.y - mScrollBottom->GetSize().y });
	ReLayout();
}

bool ScrollableArea::IsScrollBarHovered()
{
	THROW_IF_EXPIRED(mCanvas);
	auto canvas = mCanvas.lock();
	auto input = game->GetInput();
	auto renderer = game->GetRenderer();
	auto mousePosition = renderer->ToVirtualScreen(input->GetMousePosition());
	return IsGloballyVisible() && Math::IsPointInsideRectangle(mousePosition,
		GetGlobalPosition() + mScrollBarPosition, mScrollBarSize);
}

void ScrollableArea::ApplyScrolling(float offset, float scrollBarMinY, float scrollBarMaxY,
	float fieldHeight, float contentHeight)
{
	mScrollBarPosition.y = Math::Clamp(mScrollBarPosition.y + offset, scrollBarMinY, scrollBarMaxY);
	const float kScroll = Math::Min((mScrollBarPosition.y - scrollBarMinY) / fieldHeight, 1.0f);
	const float consumedHeight = Math::Max(contentHeight - mSize.y, 0.0f);
	mLayout.SetOrigin({ mLayout.GetOrigin().x, -kScroll * consumedHeight });
	ReLayout();
}

bool ScrollableArea::GetMetrics(
	float &scrollBarMinY, float &scrollBarMaxY, float &fieldHeight, float &contentHeight)
{
	// Define scroll bar field metrics
	scrollBarMinY = 5.0f + 2.0f * mScrollBarSize.x;
	scrollBarMaxY = mSize.y - 5.0f - mScrollBarSize.y - 2.0f * mScrollBarSize.x;
	fieldHeight = scrollBarMaxY - scrollBarMinY;
	bool scrollingEnabled = true;
	float minY = FLT_MAX;
	float maxY = -FLT_MAX;
	for (auto &child : mChildren)
	{
		if (child != mScrollUp && child != mScrollTop && child != mScrollDown &&
			child != mScrollBottom)
		{
			// Also check intersection with scrollable children and disable
			// scrolling if intersected
			if (scrollingEnabled)
			{
				if ((dynamic_pointer_cast<ListBox>(child) || dynamic_pointer_cast<ScrollableArea>(child) ||
					dynamic_pointer_cast<Table>(child)) &&
					child->IsHovered())
				{
					scrollingEnabled = false;
				}
			}
			minY = Math::Min(child->GetPosition().y, minY);
			maxY = Math::Max(child->GetPosition().y + child->GetSize().y, maxY);
		}
	}
	contentHeight = maxY - minY;
	return scrollingEnabled;
}

void ScrollableArea::ReLayout()
{
	mLayout.Reset();
	for (auto &child : mChildren)
	{
		if (child != mScrollUp && child != mScrollTop && child != mScrollDown &&
			child != mScrollBottom)
		{
			mLayout.Pack(child);
		}
	}
}

void ScrollableArea::ResetScrollBar()
{
	mScrollBarPosition.y = 5.0f + 2.0f * mScrollBarSize.x;
	mLayout.SetOrigin({ mLayout.GetOrigin().x, 0 });
	ReLayout();
}

void ScrollableArea::RemoveAllChildren()
{
	mChildren.clear();
	mChildren.push_back(mScrollUp);
	mChildren.push_back(mScrollTop);
	mChildren.push_back(mScrollDown);
	mChildren.push_back(mScrollBottom);
}

void ScrollableArea::ScrollTop()
{
	float scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight;
	if (GetMetrics(scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight))
	{
		ApplyScrolling(0, scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight);
	}
}

void ScrollableArea::ScrollBottom()
{
	float scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight;
	if (GetMetrics(scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight))
	{
		ApplyScrolling(9999.0f, scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight);
	}
}

float ScrollableArea::GetScrollBarPosition() const
{
	return mScrollBarPosition.y;
}

void ScrollableArea::SetScrollBarPosition(float position)
{
	mScrollBarPosition.y = position;
	float scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight;
	GetMetrics(scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight);
	ApplyScrolling(0, scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight);
}

void ScrollableArea::LinkComponents()
{
	AddChild(mScrollUp);
	AddChild(mScrollTop);
	AddChild(mScrollDown);
	AddChild(mScrollBottom);
}

ScrollableArea::ScrollableArea(const shared_ptr<Canvas> &canvas) : Widget(canvas)
{
	mLayout = { { 15, 0 }, 100, 1, 5 };

	auto theme = canvas->GetTheme();

	mScrollBarSize = { 15, 31 };
	mTexture = theme->GetTexture();
	mScrollBarTC = theme->GetArea(Theme::Item::ScrollBar);

	mFrame.mTopBorder = theme->GetArea(Theme::Item::Inset_TopBorder);
	mFrame.mBottomBorder = theme->GetArea(Theme::Item::Inset_BottomBorder);
	mFrame.mRightBorder = theme->GetArea(Theme::Item::Inset_RightBorder);
	mFrame.mBack = theme->GetArea(Theme::Item::Inset_Back);
	mFrame.mRightBottomCorner = theme->GetArea(Theme::Item::Inset_RightBottomCorner);
	mFrame.mLeftBorder = theme->GetArea(Theme::Item::Inset_LeftBorder);
	mFrame.mRightTopCorner = theme->GetArea(Theme::Item::Inset_RightTopCorner);
	mFrame.mLeftTopCorner = theme->GetArea(Theme::Item::Inset_LeftTopCorner);
	mFrame.mLeftBottomCorner = theme->GetArea(Theme::Item::Inset_LeftBottomCorner);
	mTopBottomCross = theme->GetArea(Theme::Item::Inset_TopBottomCross);
	mBottomTopCross = theme->GetArea(Theme::Item::Inset_BottomTopCross);
	mCenterBorder = theme->GetArea(Theme::Item::Inset_CenterBorder);
	mFrame.mCornerSize = { 7, 7 };
	mFrame.mBorderSize = { 3, 3 };

	mScrollUp = canvas->Create<Button>();
	mScrollUp->SetSize({ mScrollBarSize.x, mScrollBarSize.x });
	mScrollUp->SetImage(theme->GetTexture(), theme->GetArea(Theme::Item::ScrollUp));
	mScrollUp->OnClick += [this]
	{
		float scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight;
		if (GetMetrics(scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight))
		{
			ApplyScrolling(-15.0f, scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight);
		}
	};

	mScrollTop = canvas->Create<Button>();
	mScrollTop->SetSize({ mScrollBarSize.x, mScrollBarSize.x });
	mScrollTop->SetImage(theme->GetTexture(), theme->GetArea(Theme::Item::ScrollTop));
	mScrollTop->OnClick += [this]
	{
		float scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight;
		if (GetMetrics(scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight))
		{
			ApplyScrolling(-9999.0f, scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight);
		}
	};

	mScrollDown = canvas->Create<Button>();
	mScrollDown->SetSize({ mScrollBarSize.x, mScrollBarSize.x });
	mScrollDown->SetImage(theme->GetTexture(), theme->GetArea(Theme::Item::ScrollDown));
	mScrollDown->OnClick += [this] { ScrollTop(); };

	mScrollBottom = canvas->Create<Button>();
	mScrollBottom->SetSize({ mScrollBarSize.x, mScrollBarSize.x });
	mScrollBottom->SetImage(theme->GetTexture(), theme->GetArea(Theme::Item::ScrollBottom));
	mScrollBottom->OnClick += [this] { ScrollBottom(); };

	ResetScrollBar();
}

void ScrollableArea::Update()
{
	if (IsHovered())
	{
		auto input = game->GetInput();
		auto renderer = game->GetRenderer();

		float scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight;
		bool scrollingEnabled = GetMetrics(scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight);

		// mouse wheel scrolling
		if (input->GetMouseWheelSpeed() && scrollingEnabled)
		{
			const float k = Math::Min(1.0f, fieldHeight / contentHeight);
			mScrollSlowdown = 1 * k;
			mScrollSpeed = -15 * input->GetMouseWheelSpeed() * k;
		}

		if (abs(mScrollSpeed) > 0)
		{
			ApplyScrolling(mScrollSpeed, scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight);
		}

		if (mScrollSpeed < 0)
		{
			mScrollSpeed += mScrollSlowdown;
			if (mScrollSpeed > 0)
			{
				mScrollSpeed = 0;
			}
		}
		if (mScrollSpeed > 0)
		{
			mScrollSpeed -= mScrollSlowdown;
			if (mScrollSpeed < 0)
			{
				mScrollSpeed = 0;
			}
		}


		// mouse click scrolling
		if (input->IsMouseButtonHit(sf::Mouse::Left))
		{
			// todo
		}

		// mouse drag scrolling
		if (input->IsMouseButtonPressed(sf::Mouse::Left))
		{
			if (IsScrollBarHovered())
			{
				auto mouseVelocity = renderer->ToVirtualScreen(input->GetMouseSpeed());
				ApplyScrolling(mouseVelocity.y, scrollBarMinY, scrollBarMaxY, fieldHeight, contentHeight);
			}
		}
	}
}

void ScrollableArea::AddChild(const shared_ptr<Widget> &child)
{
	Widget::AddChild(child);
	ReLayout();
}

Layout &ScrollableArea::GetLayout()
{
	return mLayout;
}
