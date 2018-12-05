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

#include "Widget.h"

class Slot : public Widget
{
private:
	shared_ptr<Frame> mFrame;
	shared_ptr<Image> mImage;
	weak_ptr<void> mItem;
	virtual void DrawSelf() override final;
	virtual void OnResize() override final;

public:
	Slot(const shared_ptr<Canvas> &canvas);
	virtual void LinkComponents() override final;
	virtual void Update() override final;
	void SetItem(const weak_ptr<void> &item);
	template <typename T> shared_ptr<T> GetItem() const;
	void SetImage(const shared_ptr<Texture> &img, const sf::FloatRect &r = { 0, 0, 1, 1 });
};

template <typename T> inline shared_ptr<T> Slot::GetItem() const
{
	return static_pointer_cast<T>(mItem.lock());
}
