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

#include "Theme.h"

class Widget;

class Canvas : public enable_shared_from_this<Canvas>
{
private:
	int mLayer { 0 };
	shared_ptr<Theme> mTheme;
	vector<shared_ptr<Widget>> mWidgetList;
	bool mVisible { true };
	recursive_mutex &GetMutex() const;

public:
	static constexpr int TopmostLayer = INT_MAX;
	static constexpr int LowermostLayer = INT_MIN;
	int GetLayer() const;
	void SetLayer(int layer);
	Canvas();
	virtual ~Canvas();
	void SetTheme(const shared_ptr<Theme> &theme);
	shared_ptr<Theme> GetTheme() const;
	void Update();
	vector<shared_ptr<Widget>> &GetWidgetList();
	void SetVisible(bool visible);
	bool IsVisible() const;
	void Remove(const shared_ptr<Widget> &widget);
	template <class T> shared_ptr<T> Create()
	{
		lock_guard<recursive_mutex> guard(GetMutex());
		shared_ptr<T> widget = make_shared<T>(shared_from_this());
		mWidgetList.emplace_back(widget);
		widget->LinkComponents();
		return widget;
	}
	bool IsAnythingHovered();
};
