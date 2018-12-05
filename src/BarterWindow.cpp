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
#include "BarterWindow.h"
#include "Window.h"
#include "Table.h"
#include "Button.h"
#include "Inventory.h"
#include "Input.h"
#include "Tooltip.h"
#include "Player.h"
#include "Item.h"
#include "Inventory.h"
#include "Frame.h"
#include "NumericUpDown.h"

enum GroupTag
{
	GT_TRADER_ITEMS = BIT(0),
};

void BarterWindow::RejectDeal()
{
	if (mBarterInventory)
	{
		auto groups = mBarterInventory->GetGroups(); // copy
		for (auto &group : groups)
		{
			auto items = group->mItems; // copy
			for (auto &item : items)
			{
				if (group->mFlags & GT_TRADER_ITEMS)
				{
					mTraderInventory->AddItem(item);
					mBarterInventory->RemoveItem(item);
				}
				else
				{
					mPlayerInventory->AddItem(item);
					mBarterInventory->RemoveItem(item);
				}
			}
		}
	}
}

void BarterWindow::DoBarter()
{
	// calculate profits
	int playerProfit = 0, traderProfit = 0;
	for (auto &group : mBarterInventory->GetGroups())
	{
		if (group->mFlags & GT_TRADER_ITEMS)
		{
			traderProfit += group->Count() * group->mItems[0]->GetDefinition().mPrice;
		}
		else
		{
			playerProfit += group->Count() * group->mItems[0]->GetDefinition().mPrice;
		}
	}

	// move items
	auto groups = mBarterInventory->GetGroups(); // copy
	for (auto &group : groups)
	{
		auto items = group->mItems; // copy

		// unequip first
		for (auto &item : items)
		{
			mTrader->UnequipItem(item);
			mPlayer->UnequipItem(item);
		}

		// then move each item
		for (auto &item : items)
		{
			if (group->mFlags & GT_TRADER_ITEMS)
			{
				mPlayerInventory->AddItem(item);
				mBarterInventory->RemoveItem(item);
			}
			else
			{
				mTraderInventory->AddItem(item);
				mBarterInventory->RemoveItem(item);
			}
		}
	}

	// add profits
	mTraderInventory->AddMoney(traderProfit);
	mTraderInventory->RemoveMoney(playerProfit);
	mPlayerInventory->AddMoney(playerProfit);
	mPlayerInventory->RemoveMoney(traderProfit);

	Sync();
}

void BarterWindow::ShowTooltipForItem(const shared_ptr<Item> &item, int count)
{
	if (item)
	{
		mTooltip->Show(item->GetDefinition().GetDescription("", count));
	}
}

BarterWindow::BarterWindow()
{
	mCanvas = game->CreateCanvas();

	mWindow = mCanvas->Create<Window>();
	mWindow->SetSize({ 750, 400 });
	mWindow->SetAutoLayoutingEnabled(false);
	mWindow->SetInCenter();
	mWindow->SetCaption("Barter");
	mWindow->AddHelpButton();
	mWindow->GetHelpButton()->OnHover += [this]
	{
		mTooltip->Show(game->GetStr("BarterUI_Help"));
	};


	Layout layout({ 20, 30 }, 700, 3, 5);

	mTraderItemsLbl = mCanvas->Create<Label>();
	mTraderItemsLbl->SetText(game->GetStr("traderItems"));
	mTraderItemsLbl->SetAlignment(TextAlignment::Center);
	layout.Pack(mTraderItemsLbl);
	mWindow->AddChild(mTraderItemsLbl);

	mBarterItemsLbl = mCanvas->Create<Label>();
	mBarterItemsLbl->SetText(game->GetStr("barterItems"));
	mBarterItemsLbl->SetAlignment(TextAlignment::Center);
	layout.Pack(mBarterItemsLbl);
	mWindow->AddChild(mBarterItemsLbl);

	mPlayerItemsLbl = mCanvas->Create<Label>();
	mPlayerItemsLbl->SetText(game->GetStr("playerItems"));
	mPlayerItemsLbl->SetAlignment(TextAlignment::Center);
	layout.Pack(mPlayerItemsLbl);
	mWindow->AddChild(mPlayerItemsLbl);

	mTraderItems = mCanvas->Create<Table>();
	mTraderItems->AddColumn(game->GetStr("bItem"));
	mTraderItems->SetSize({ 0, 250 });
	mTraderItems->OnRowHovered += [this]
	{
		if (mTraderItems->GetHoveredRowIndex() >= 0)
		{
			auto &group = mTraderInventory->GetGroups()[mTraderItems->GetHoveredRowIndex()];
			ShowTooltipForItem(group->mItems[0], group->Count());
		}
	};
	layout.Pack(mTraderItems);
	mWindow->AddChild(mTraderItems);

	mBarterItems = mCanvas->Create<Table>();
	mBarterItems->AddColumn(game->GetStr("bItem"));
	mBarterItems->SetSize({ 0, 250 });
	mBarterItems->OnRowHovered += [this]
	{
		if (mBarterItems->GetHoveredRowIndex() >= 0)
		{
			auto &group = mBarterInventory->GetGroups()[mBarterItems->GetHoveredRowIndex()];
			ShowTooltipForItem(group->mItems[0], group->Count());
		}
	};
	layout.Pack(mBarterItems);
	mWindow->AddChild(mBarterItems);

	mPlayerItems = mCanvas->Create<Table>();
	mPlayerItems->AddColumn(game->GetStr("bItem"));
	mPlayerItems->SetSize({ 0, 250 });
	mPlayerItems->OnRowHovered += [this]
	{
		if (mPlayerItems->GetHoveredRowIndex() >= 0)
		{
			auto &group = mPlayerInventory->GetGroups()[mPlayerItems->GetHoveredRowIndex()];
			ShowTooltipForItem(group->mItems[0], group->Count());
		}
	};
	layout.Pack(mPlayerItems);
	mWindow->AddChild(mPlayerItems);

	mTraderMoney = mCanvas->Create<Label>();
	mTraderMoney->SetAlignment(TextAlignment::Center);
	layout.Pack(mTraderMoney);
	mWindow->AddChild(mTraderMoney);

	mDealCost = mCanvas->Create<Label>();
	mDealCost->SetAlignment(TextAlignment::Center);
	layout.Pack(mDealCost);
	mWindow->AddChild(mDealCost);

	mPlayerMoney = mCanvas->Create<Label>();
	mPlayerMoney->SetAlignment(TextAlignment::Center);
	layout.Pack(mPlayerMoney);
	mWindow->AddChild(mPlayerMoney);

	mDoBarter = mCanvas->Create<Button>();
	mDoBarter->SetSize({ 0, 30 });
	mDoBarter->SetText(game->GetStr("confirmBarter"));
	mDoBarter->OnClick += [this] { DoBarter(); };
	layout.Pack(mDoBarter);
	mWindow->AddChild(mDoBarter);

	mClear = mCanvas->Create<Button>();
	mClear->SetText(game->GetStr("clearBarter"));
	mClear->SetSize({ 0, 30 });
	mClear->OnClick += [this]
	{
		RejectDeal();
		Sync();
	};
	layout.Pack(mClear);
	mWindow->AddChild(mClear);

	mClose = mCanvas->Create<Button>();
	mClose->OnClick += [this] { SetVisible(false); };
	mClose->SetText(game->GetStr("close"));
	mClose->SetSize({ 0, 30 });
	layout.Pack(mClose);
	mWindow->AddChild(mClose);

	// modal window
	{
		mCntCountWnd = mCanvas->Create<Frame>();
		mCntCountWnd->SetSize({ 250, 80 });
		mCntCountWnd->SetInCenter();

		Layout lay { { 10, 10 }, mCntCountWnd->GetSize().x - 20, 2, 5 };

		mCntText = mCanvas->Create<Label>();
		mCntText->SetSize({ 125, 30 });
		mCntText->SetText(game->GetStr("howMuchItems"));
		mCntCountWnd->AddChild(mCntText);
		lay.Pack(mCntText);

		mCntCount = mCanvas->Create<NumericUpDown>();
		mCntCount->SetSize({ 105, 30 });
		mCntCountWnd->AddChild(mCntCount);
		lay.Pack(mCntCount);

		mCntOk = mCanvas->Create<Button>();
		mCntOk->SetSize({ 105, 30 });
		mCntOk->SetText(game->GetStr("okItems"));
		mCntCountWnd->AddChild(mCntOk);
		lay.Pack(mCntOk);

		mCntCancel = mCanvas->Create<Button>();
		mCntCancel->SetSize({ 105, 30 });
		mCntCancel->SetText(game->GetStr("cancelItems"));
		mCntCountWnd->AddChild(mCntCancel);
		lay.Pack(mCntCancel);

		mCntCountWnd->SetLocalVisibility(false);
	}

	mTooltip = mCanvas->Create<Tooltip>();
	mTooltip->SetSize({ 340, 420 });

	SetVisible(false);

	Log() << "Barter window successfully initialized!" << LineEnd;
}

BarterWindow::~BarterWindow()
{
}

void BarterWindow::Update()
{
	if (mCanvas->IsVisible())
	{
		int playerProfit = 0;
		int traderProfit = 0;

		for (auto &group : mBarterInventory->GetGroups())
		{
			if (group->mFlags & GT_TRADER_ITEMS)
			{
				traderProfit += group->Count() * group->mItems[0]->GetDefinition().mPrice;
			}
			else
			{
				playerProfit += group->Count() * group->mItems[0]->GetDefinition().mPrice;
			}
		}

		if (playerProfit > mTraderInventory->GetMoney() ||
			traderProfit > mPlayerInventory->GetMoney())
		{
			mDoBarter->SetEnabled(false);
			mDoBarter->OnHover.Clear();
			if (playerProfit > mTraderInventory->GetMoney())
			{
				mDoBarter->OnHover += [this] { mTooltip->Show(game->GetStr("traderNotEnoughMoney")); };
			}
			else
			{
				mDoBarter->OnHover += [this] { mTooltip->Show(game->GetStr("playerNotEnoughMoney")); };
			}
		}
		else
		{
			mDoBarter->SetEnabled(true);
		}

		auto &controls = game->GetControls();
		if (mTraderItems->GetSelectedRowIndex() >= 0)
		{
			if (controls.IsKeyHit(ControlKey::Use))
			{
				auto traderGroup = mTraderInventory->GetGroups()[mTraderItems->GetSelectedRowIndex()];
				auto &item = traderGroup->mItems.front();
				mBarterInventory->AddItem(item, GT_TRADER_ITEMS);
				mTraderInventory->RemoveItem(item);
				Sync();
			}
		}
		if (mPlayerItems->GetSelectedRowIndex() >= 0)
		{
			if (controls.IsKeyHit(ControlKey::Use))
			{
				auto playerGroup = mPlayerInventory->GetGroups()[mPlayerItems->GetSelectedRowIndex()];
				auto &item = playerGroup->mItems.front();
				mBarterInventory->AddItem(item);
				mPlayerInventory->RemoveItem(item);
				Sync();
			}
		}

		// cancel item sell
		if (mBarterItems->GetSelectedRowIndex() >= 0)
		{
			if (controls.IsKeyHit(ControlKey::Use))
			{
				auto &group = mBarterInventory->GetGroups()[mBarterItems->GetSelectedRowIndex()];
				auto &item = group->mItems.front();
				if (group->mFlags & GT_TRADER_ITEMS)
				{
					mTraderInventory->AddItem(item);
					mBarterInventory->RemoveItem(item);
				}
				else
				{
					mPlayerInventory->AddItem(item);
					mBarterInventory->RemoveItem(item);
				}
				Sync();
			}
		}
	}
}

void BarterWindow::SetVisible(bool state)
{
	mCanvas->SetVisible(state);
	if (!state)
	{
		RejectDeal();
		mTraderInventory.reset();
		mPlayerInventory.reset();
		mTrader.reset();
		mPlayer.reset();
		mBarterInventory.reset();
	}
}

void BarterWindow::Open(const shared_ptr<Actor> &trader, const shared_ptr<Player> &player)
{
	SetVisible(true);
	mTrader = trader;
	mPlayer = player;
	mTraderInventory = trader->GetInventory();
	mPlayerInventory = player->GetInventory();
	mBarterInventory = make_shared<Inventory>();
	Sync();
}

bool BarterWindow::IsVisible() const
{
	return mCanvas->IsVisible();
}

void BarterWindow::Sync()
{

	if (mTraderInventory)
	{
		float scrollBar = mTraderItems->GetScrollBarPosition();
		mTraderItems->Clear();
		for (auto &group : mTraderInventory->GetGroups())
		{
			int row = mTraderItems->AddRow();
			dynamic_pointer_cast<Label>(mTraderItems->Get(0, row))
				->SetText(
					StringBuilder() << group->mItems.at(0)->GetDefinition().GetTranslatedName() << " x" << group->Count());
		}
		mTraderItems->SetScrollBarPosition(scrollBar);
	}

	if (mPlayerInventory)
	{
		float scrollBar = mPlayerItems->GetScrollBarPosition();
		mPlayerItems->Clear();
		for (auto &group : mPlayerInventory->GetGroups())
		{
			int row = mPlayerItems->AddRow();
			dynamic_pointer_cast<Label>(mPlayerItems->Get(0, row))
				->SetText(
					StringBuilder() << group->mItems.at(0)->GetDefinition().GetTranslatedName() << " x" << group->Count());
		}
		mPlayerItems->SetScrollBarPosition(scrollBar);
	}

	if (mBarterInventory)
	{
		float scrollBar = mBarterItems->GetScrollBarPosition();
		mBarterItems->Clear();
		for (auto &group : mBarterInventory->GetGroups())
		{
			int row = mBarterItems->AddRow();

			bool playerBuy = group->mFlags & GT_TRADER_ITEMS;

			dynamic_pointer_cast<Label>(mBarterItems->Get(0, row))
				->SetText(StringBuilder(playerBuy ? "" : "< ")
					<< group->mItems.at(0)->GetDefinition().GetTranslatedName() << ": x" << group->Count()
					<< (playerBuy ? " >" : ""));
		}
		mBarterItems->SetScrollBarPosition(scrollBar);
	}

	mTraderMoney->SetText(game->GetStr("traderMoney") + to_string(mTraderInventory->GetMoney()) +
		game->GetStr("bCoins"));
	mPlayerMoney->SetText(game->GetStr("playerMoney") + to_string(mPlayerInventory->GetMoney()) +
		game->GetStr("bCoins"));

	int dealCost = 0;
	for (auto &group : mBarterInventory->GetGroups())
	{
		bool playerBuy = group->mFlags & GT_TRADER_ITEMS;
		dealCost += (playerBuy ? -1 : 1) * group->Count() * group->mItems[0]->GetDefinition().mPrice;
	}
	mDealCost->SetText(game->GetStr("dealMoney") + to_string(dealCost) + game->GetStr("bCoins"));
}
