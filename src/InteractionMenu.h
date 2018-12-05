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

enum class InteractionType
{
	First = 0, // must be first
	GrabItem = First,
	Talk,
	UseDoor,
	UseMotherCrystal,
	PillageRemains,
	UseChest,
	UseFogGate,
	UseBench,
	Apologize,
	Count // must be last
};

class InteractionMenu
{
private:
	shared_ptr<Canvas> mCanvas;
	shared_ptr<Frame> mFrame;
	int mSelectedItem { 0 };
	unordered_map<InteractionType, shared_ptr<Button>> mOptions;
	void UpdateLayout();

public:
	InteractionMenu();
	~InteractionMenu();
	void Update();
	void SetOption(InteractionType type, const function<void()> &action);
	void HideOption(InteractionType type);
};
