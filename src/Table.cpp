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
#include "Table.h"
#include "Input.h"

void Table::OnResize()
{
	mItemArea->SetPosition({ 0, 30 });
	mItemArea->SetSize({ mSize.x, mSize.y - 30 });

	mHeaderImg->SetPosition({ 0, 0 });
	mHeaderImg->SetSize({ mSize.x, 32 });

	mSelector->SetSize(
	{ mItemArea->GetLayout().GetWidth() + 2 * mItemArea->GetLayout().GetSpacing(), 30 });

	mHeaderLayout.SetWidth(mItemArea->GetLayout().GetWidth());
	mHeaderLayout.SetOrigin({ mItemArea->GetLayout().GetOrigin().x, 0 });
	ReLayoutHeader();
}

void Table::DrawSelf()
{
	UpdateBorders();

	mVertices.clear();
	PushQuad(mGlobalPosition, mSize, mTextureRect, sf::Color(44, 48, 43));
	DrawVertices(mTexture ? mTexture->GetOpenGLTexture() : 0);
}

void Table::UpdateBorders()
{
	mTableBorders->Clear();
	for (auto &row : mRowList)
	{
		for (auto &w : row)
		{
			const Vec2f topBegin = w->GetGlobalPosition();
			const Vec2f topEnd = w->GetGlobalPosition() + Vec2f(w->GetSize().x, 0);
			const Vec2f bottomBegin = w->GetGlobalPosition() + Vec2f(0, w->GetSize().y);
			const Vec2f bottomEnd = w->GetGlobalPosition() + w->GetSize();
			const Vec2f leftBegin = w->GetGlobalPosition();
			const Vec2f leftEnd = w->GetGlobalPosition() + Vec2f(0, w->GetSize().y);
			const Vec2f rightBegin = w->GetGlobalPosition() + Vec2f(w->GetSize().x, 0);
			const Vec2f rightEnd = w->GetGlobalPosition() + w->GetSize();
			if (bottomBegin.y >= mItemArea->GetGlobalPosition().y)
			{
				mTableBorders->Add(topBegin, topEnd, { 44, 48, 43 });
				mTableBorders->Add(bottomBegin, bottomEnd, { 44, 48, 43 });
				mTableBorders->Add(leftBegin, leftEnd, { 44, 48, 43 });
				mTableBorders->Add(rightBegin, rightEnd, { 44, 48, 43 });
			}
		}
	}
}
void Table::ReLayoutHeader()
{
	mHeaderLayout.Reset();
	mHeaderLayout.PurgeRows();
	mHeaderLayout.ConfigureRow(Layout::TemplateRow, mColumnCount);
	for (auto &hdrItem : mHeaderRow)
	{
		mHeaderLayout.Pack(hdrItem);
	}
}

void Table::ReLayoutContent()
{
	// detach all children for correct further layouting
	mItemArea->GetLayout().Reset();
	mItemArea->GetLayout().PurgeRows();
	mItemArea->GetLayout().ConfigureRow(Layout::TemplateRow, mColumnCount);
	mItemArea->RemoveAllChildren();
	// insert new label into end of each row
	for (auto &row : mRowList)
	{
		// attach children back in correct order
		for (auto &widget : row)
		{
			mItemArea->AddChild(widget);
		}
	}
	// set correct position of each child
	mItemArea->ReLayout();
}

Table::Table(const shared_ptr<Canvas> &canvas)
	: Widget(canvas), mColumnCount(0), mSelectedRowIndex(-1), mHoveredRowIndex(-1)
{
	mItemArea = canvas->Create<ScrollableArea>();
	mItemArea->GetLayout().ConfigureRow(Layout::TemplateRow, -1, 0.0f);

	mTableBorders = canvas->Create<LineSet>();

	mSelector = canvas->Create<Image>();
	mSelector->SetColor({ 100, 100, 100, 120 });

	mSelectedRow = canvas->Create<Image>();
	mSelectedRow->SetColor({ 100, 120, 100, 120 });
	mSelectedRow->SetStencilWriteEnabled(false);

	mHeaderImg = canvas->Create<Image>();
	mHeaderImg->SetColor(sf::Color(44, 48, 43));
}

Table::~Table()
{
	DBG_ECHO();
}

void Table::LinkComponents()
{
	AddChild(mItemArea);
	AddChild(mTableBorders);
	AddChild(mSelector);
	AddChild(mSelectedRow);
	AddChild(mHeaderImg);
}

void Table::Update()
{
	int n = 0;
	bool somethingHovered = false;
	for (auto &row : mRowList)
	{
		for (auto &widget : row)
		{
			if (widget->IsHovered())
			{
				somethingHovered = true;
				OnRowHovered();
				const float selectorX = mItemArea->GetLayout().GetOrigin().x;
				const float selectorY = mItemArea->GetPosition().y + row.front()->GetPosition().y - mItemArea->GetLayout().GetSpacing();
				mSelector->SetPosition({ selectorX, selectorY });
				mSelector->SetSize({ mSelector->GetSize().x, widget->GetSize().y });
				if (game->GetInput()->IsMouseButtonUp(sf::Mouse::Left))
				{
					mSelectedRowIndex = n;
					row.front()->AddChild(mSelectedRow);
					mSelectedRow->SetSize(mSelector->GetSize());
					OnRowClicked();
				}
				mHoveredRowIndex = n;
				break;
			}
		}
		++n;
	}
	mSelector->SetLocalVisibility(somethingHovered);
}

void Table::AddColumn(const string &name)
{
	++mColumnCount;
	// add new header element
	auto hdrLbl = GetCanvas()->Create<Label>();
	hdrLbl->SetText(name);
	hdrLbl->SetAlignment(TextAlignment::Center);
	mHeaderImg->AddChild(hdrLbl);
	mHeaderRow.push_back(hdrLbl);
	ReLayoutHeader();
	// detach all children for correct further layouting
	mItemArea->GetLayout().Reset();
	mItemArea->GetLayout().PurgeRows();
	mItemArea->GetLayout().ConfigureRow(Layout::TemplateRow, mColumnCount);
	mItemArea->RemoveAllChildren();
	// insert new label into end of each row
	for (auto &row : mRowList)
	{
		auto lbl = GetCanvas()->Create<Label>();
		lbl->SetText(StringBuilder("Item ") << row.size());
		lbl->SetSize({ 0, 35 });
		lbl->SetStencilWriteEnabled(false);
		lbl->SetAlignment(TextAlignment::Center);
		row.push_back(lbl);
		// attach children back in correct order
		for (auto &widget : row)
		{
			mItemArea->AddChild(widget);
		}
	}
	// set correct position of each child
	mItemArea->ReLayout();
}

int Table::AddRow()
{
	// detach all children for correct further layouting
	mItemArea->GetLayout().Reset();
	mItemArea->GetLayout().PurgeRows();
	mItemArea->GetLayout().ConfigureRow(Layout::TemplateRow, mColumnCount);
	mItemArea->RemoveAllChildren();
	// insert new label into end of each row
	mRowList.push_back(Row());
	for (int i = 0; i < mColumnCount; ++i)
	{
		auto lbl = GetCanvas()->Create<Label>();
		lbl->SetText(StringBuilder("Item ") << mRowList.back().size());
		lbl->SetStencilWriteEnabled(false);
		lbl->SetSize({ 0, 35 });
		lbl->SetAlignment(TextAlignment::Center);
		mRowList.back().push_back(lbl);
	}
	// attach children back in correct order
	for (auto &row : mRowList)
	{
		for (auto &widget : row)
		{
			mItemArea->AddChild(widget);
		}
	}
	// set correct position of each child
	mItemArea->ReLayout();
	// return index of the new row
	return mRowList.size() - 1;
}

shared_ptr<Widget> Table::Get(unsigned int x, unsigned int y)
{
	if (y >= mRowList.size())
	{
		throw Exception("Table: Row index out of bounds!");
	}
	auto &row = mRowList[y];
	if (x >= row.size())
	{
		throw Exception("Table: Column index out of bounds!");
	}
	return row[x];
}

void Table::Set(unsigned int x, unsigned int y, const shared_ptr<Widget> &newWidget)
{
	if (y >= mRowList.size())
	{
		throw Exception("Table: Row index out of bounds!");
	}
	auto &row = mRowList[y];
	if (x >= row.size())
	{
		throw Exception("Table: Column index out of bounds!");
	}
	row[x]->RemoveFromCanvas();
	newWidget->SetSize({ 0, 35 });
	row[x] = newWidget;

	ReLayoutContent();
}

void Table::Clear()
{
	mItemArea->RemoveAllChildren();
	for (auto &row : mRowList)
	{
		for (auto &widget : row)
		{
			widget->RemoveFromCanvas();
		}
	}
	mRowList.clear();
	mSelectedRowIndex = -1;
	mHoveredRowIndex = -1;
	mSelectedRow->SetSize({ 0, 0 });
	mItemArea->ResetScrollBar();
}

int Table::GetSelectedRowIndex() const
{
	return mSelectedRowIndex;
}

int Table::GetHoveredRowIndex() const
{
	return mHoveredRowIndex;
}

Table::Row &Table::GetRow(int n)
{
	if (n < 0 || n >= static_cast<int>(mRowList.size()))
	{
		throw Exception("Table: Index out of bounds!");
	}
	return mRowList[n];
}

Vec2f Table::GetSize() const
{
	return { mSize.x, mSize.y };
}

float Table::GetScrollBarPosition() const
{
	return mItemArea->GetScrollBarPosition();
}

void Table::SetScrollBarPosition(float position)
{
	return mItemArea->SetScrollBarPosition(position);
}
