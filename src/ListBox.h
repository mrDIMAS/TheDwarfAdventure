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

#pragma once

#include "Button.h"
#include "Image.h"

class ListBoxItem
{
public:
	shared_ptr<Image> mBackground;
	shared_ptr<Label> mText;
	sf::Color mNormalColor;
	sf::Color mSelectedColor;
	bool mSelected;
	ListBoxItem();
	ListBoxItem(const shared_ptr<Image> &background, const shared_ptr<Label> &text);
};

class ListBox : public Widget
{
private:
	FrameDesc mFrame;
	shared_ptr<Texture> mElementTexture;
	shared_ptr<Button> mScrollBar;
	vector<ListBoxItem> mItems;
	float mElementHeight;
	int mMaxVisibleElements;
	int mScrollPosition;
	int mSelectedItemNumber;

	void OnResize() override final;
	virtual void DrawSelf() override final;
	void RepositionElements();

public:
	void LinkComponents() override final;
	Event OnSelectedItemChanged;
	ListBox(const shared_ptr<Canvas> &scene);
	~ListBox();
	int AddItem(const string &item);
	void SetElementHeight(float height);
	void Update() override final;
	void SetScrollTexture(const shared_ptr<Texture> &texture);
	int GetSelectedItemNumber() const;
	string GetItem(int n) const;
	void SetItem(int n, const string &item);
	void ResetSelection();
};
