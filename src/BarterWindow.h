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

class Inventory;
struct ItemGroup;

class BarterWindow
{
private:
	shared_ptr<Canvas> mCanvas;
	shared_ptr<Window> mWindow;
	shared_ptr<Label> mTraderItemsLbl;
	shared_ptr<Table> mTraderItems;
	shared_ptr<Label> mBarterItemsLbl;
	shared_ptr<Table> mBarterItems;
	shared_ptr<Label> mPlayerItemsLbl;
	shared_ptr<Table> mPlayerItems;

	shared_ptr<Label> mTraderMoney;
	shared_ptr<Label> mDealCost;
	shared_ptr<Label> mPlayerMoney;
	shared_ptr<Button> mDoBarter;
	shared_ptr<Button> mClose;
	shared_ptr<Button> mClear;
	shared_ptr<Inventory> mTraderInventory;
	shared_ptr<Inventory> mPlayerInventory;
	shared_ptr<Actor> mTrader;
	shared_ptr<Player> mPlayer;
	shared_ptr<Tooltip> mTooltip;

	shared_ptr<Frame> mCntCountWnd;
	shared_ptr<Label> mCntText;
	shared_ptr<NumericUpDown> mCntCount;
	shared_ptr<Button> mCntOk;
	shared_ptr<Button> mCntCancel;

	shared_ptr<Inventory> mBarterInventory;

	void RejectDeal();

	void DoBarter();
	void ShowTooltipForItem(const shared_ptr<Item> &item, int count);

public:
	BarterWindow();
	~BarterWindow();
	void Update();
	void SetVisible(bool state);
	void Open(const shared_ptr<Actor> &trader, const shared_ptr<Player> &player);
	bool IsVisible() const;
	void Sync();
};
