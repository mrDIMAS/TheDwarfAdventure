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
#include "InteractionMenu.h"
#include "Canvas.h"
#include "Button.h"
#include "Frame.h"
#include "Layout.h"
#include "Input.h"
#include "Controls.h"
#include "Game.h"

void InteractionMenu::UpdateLayout()
{
	Layout layout { { 6, 6 }, mFrame->GetSize().x - 12, 1, 3 };
	float minY = FLT_MAX;
	float maxY = -FLT_MAX;
	bool anyVisible = false;
	for (auto &kv : mOptions)
	{
		if (kv.second->IsLocalVisible())
		{
			layout.Pack(kv.second);
			auto p = kv.second->GetGlobalPosition();
			auto s = kv.second->GetSize();
			minY = Math::Min(minY, p.y);
			maxY = Math::Max(maxY, p.y + s.y);
			anyVisible = true;
		}
	}
	mFrame->SetSize({ mFrame->GetSize().x, maxY - minY + 12 });
	mFrame->SetLocalVisibility(anyVisible);
}

InteractionMenu::InteractionMenu()
{
	mCanvas = game->CreateCanvas();
	mFrame = mCanvas->Create<Frame>();
	mFrame->SetSize({ 200, 60 });

	const int first = static_cast<int>(InteractionType::First);
	const int last = static_cast<int>(InteractionType::Count);
	for (int i = first; i < last; ++i)
	{
		auto type = static_cast<InteractionType>(i);
		auto opt = mCanvas->Create<Button>();
		opt->SetMouseInputEnabled(false);
		opt->SetLocalVisibility(false);
		opt->SetSize({ 100, 20 });
		opt->SetTextSize(10);
		mOptions[type] = opt;
		mFrame->AddChild(opt);
	}

	mFrame->SetPosition({ game->VirtualScreen.getSize().x / 2 - mFrame->GetSize().x / 2, 10 });

	UpdateLayout();

	Log() << "Interaction menu successfully initialized!" << LineEnd;
}

InteractionMenu::~InteractionMenu()
{
}

void InteractionMenu::Update()
{
	auto input = game->GetInput();

	int activeOpts = -1;
	for (auto &kv : mOptions)
	{
		if (kv.second->IsLocalVisible())
		{
			++activeOpts;
		}
	}

	int delta = static_cast<int>(input->GetMouseWheelSpeed());
	if (game->GetControls().IsKeyHit(ControlKey::Back))
	{
		delta = -1;
	}
	if (game->GetControls().IsKeyHit(ControlKey::Next))
	{
		delta = 1;
	}
	mSelectedItem = Math::Clamp(mSelectedItem - delta, 0, activeOpts);

	int n = 0;
	for (auto &kv : mOptions)
	{
		if (kv.second->IsLocalVisible())
		{
			if (mSelectedItem == n)
			{
				kv.second->SetTextColor({ 0, 0, 200 });
				if (game->GetControls().IsKeyHit(ControlKey::Use))
				{
					kv.second->OnClick();
				}
			}
			else
			{
				kv.second->SetTextColor({ 255, 255, 255 });
			}
			++n;
		}
	}
}

void InteractionMenu::SetOption(InteractionType type, const function<void()> &action)
{
	if (action)
	{
		auto &opt = mOptions[type];
		opt->SetLocalVisibility(true);
		opt->OnClick.Clear();
		opt->OnClick += action;
		UpdateLayout();

		auto useKeyName = game->GetControls().GetKeyName(ControlKey::Use);

		for (auto &kv : mOptions)
		{
			if (kv.second->IsLocalVisible())
			{
				string text = "<undefined>";
				switch (kv.first)
				{
				case InteractionType::GrabItem:
					text = game->GetStr("pickUp");
					break;
				case InteractionType::PillageRemains:
					text = game->GetStr("pillageRemains");
					break;
				case InteractionType::Talk:
					text = game->GetStr("talkNPC");
					break;
				case InteractionType::UseChest:
					text = game->GetStr("lootChest");
					break;
				case InteractionType::UseDoor:
					text = game->GetStr("useDoor");
					break;
				case InteractionType::UseFogGate:
					text = game->GetStr("enterFog");
					break;
				case InteractionType::UseMotherCrystal:
					text = game->GetStr("useMotherCrystal");
					break;
				case InteractionType::UseBench:
					text = game->GetStr("useBench");
					break;
				case InteractionType::Apologize:
					text = game->GetStr("apologize");
					break;
				}
				kv.second->SetText("[" + useKeyName + "] - " + text);
			}
		}
	}
}

void InteractionMenu::HideOption(InteractionType type)
{
	auto &opt = mOptions[type];
	opt->SetLocalVisibility(false);
	UpdateLayout();
}
