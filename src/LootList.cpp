

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
#include "LootList.h"
#include "Window.h"
#include "Level.h"
#include "Player.h"
#include "Input.h"
#include "Item.h"
#include "Canvas.h"
#include "Table.h"
#include "Tooltip.h"
#include "Inventory.h"
#include "Label.h"

void LootWindow::ShowTooltipForItem(const shared_ptr<Item> &item, int count)
{
	if (item)
	{
		mTooltip->Show(item->GetDefinition().GetDescription("", count));
	}
}

LootWindow::LootWindow()
{
	mCanvas = game->CreateCanvas();

	Layout layout({ 20, 30 }, 360, 1, 5);

	mWindow = mCanvas->Create<Window>();
	mWindow->SetSize({ 400, 400 });
	mWindow->SetAutoLayoutingEnabled(false);
	mWindow->SetInCenter();
	mWindow->SetCaption(game->GetStr("loot"));

	mTable = mCanvas->Create<Table>();
	mTable->SetSize({ 0, 300 });
	mTable->AddColumn(game->GetStr("name"));
	mTable->OnRowHovered += [this]
	{
		if (mTable->GetHoveredRowIndex() >= 0)
		{
			auto &group = mInventory->GetGroups()[mTable->GetHoveredRowIndex()];
			ShowTooltipForItem(group->mItems[0], group->Count());
		}
	};
	layout.Pack(mTable);
	mWindow->AddChild(mTable);

	mMoney = mCanvas->Create<Label>();
	mMoney->SetAlignment(TextAlignment::Center);
	layout.Pack(mMoney);
	mWindow->AddChild(mMoney);

	layout.ConfigureRow(Layout::TemplateRow, 2);

	mGrabAllBtn = mCanvas->Create<Button>();
	mGrabAllBtn->SetSize({ 0, 30 });
	mGrabAllBtn->SetText(game->GetStr("grabAll"));
	mGrabAllBtn->OnClick += [this]
	{
		game->GetLevel()->GetPlayer()->GetInventory()->GrabAllFrom(mInventory);
		SetVisible(false);
	};
	layout.Pack(mGrabAllBtn);
	mWindow->AddChild(mGrabAllBtn);

	mCloseBtn = mCanvas->Create<Button>();
	mCloseBtn->SetSize({ 0, 30 });
	mCloseBtn->SetText(game->GetStr("close"));
	mCloseBtn->OnClick += [this] { SetVisible(false); };
	layout.Pack(mCloseBtn);
	mWindow->AddChild(mCloseBtn);

	mTooltip = mCanvas->Create<Tooltip>();
	mTooltip->SetSize({ 340, 155 });

	SetVisible(false);

	Log() << "Loot window successfully initialized!" << LineEnd;
}

LootWindow::~LootWindow()
{
}

bool LootWindow::IsVisible() const
{
	return mCanvas->IsVisible();
}

void LootWindow::SetVisible(bool state)
{
	mCanvas->SetVisible(state);
	if (!state)
	{
		mInventory.reset();
	}
}

void LootWindow::Open(const shared_ptr<Inventory> &inv)
{
	mInventory = inv;
	Sync();
	SetVisible(true);
}

void LootWindow::Update()
{
	if (mCanvas->IsVisible())
	{
		auto &controls = game->GetControls();
		if (mTable->GetSelectedRowIndex() >= 0)
		{
			if (controls.IsKeyHit(ControlKey::Use))
			{
				auto group = mInventory->GetGroups()[mTable->GetSelectedRowIndex()];
				auto player = game->GetLevel()->GetPlayer();
				mInventory->TransferItem(group->mItems.at(0), player->GetInventory());
				if (mInventory->IsEmpty())
				{
					SetVisible(false);
					return;
				}
				else
				{
					Sync();
				}
			}
		}
	}
}

void LootWindow::Sync()
{
	if (mInventory)
	{
		auto groups = mInventory->GetGroups();
		mTable->Clear();
		for (auto &group : groups)
		{
			int row = mTable->AddRow();
			dynamic_pointer_cast<Label>(mTable->Get(0, row))
				->SetText(StringBuilder("x")
					<< group->Count() << " - " << group->mItems.at(0)->GetDefinition().GetTranslatedName());
		}
		mMoney->SetText(game->GetStr("money") + to_string(mInventory->GetMoney()));
	}
}
