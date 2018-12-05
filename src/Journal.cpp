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
#include "Journal.h"
#include "Canvas.h"
#include "Window.h"
#include "Table.h"
#include "ScrollableArea.h"
#include "Level.h"
#include "QuestBase.h"
#include "Label.h"

void Journal::ClearQuestArea()
{
	for (auto &lbl : mCurrentQuestMarks)
	{
		lbl->RemoveFromCanvas();
	}
	mCurrentQuestMarks.clear();
	mCurrentQuestArea->RemoveAllChildren();
}

void Journal::Clear()
{
	mQuestTable->Clear();
	ClearQuestArea();
}

void Journal::OnRowClicked()
{
	ClearQuestArea();
	
	const vector<shared_ptr<Quest>>& givenQuests = game->GetLevel()->GetQuests();
	int index = mQuestTable->GetSelectedRowIndex();
	if (index < static_cast<int>(givenQuests.size()))
	{
		auto &quest = givenQuests[index];
		auto caption = mCanvas->Create<Label>();
		caption->SetText(quest->GetName());
		caption->SetAlignment(TextAlignment::Center);
		caption->SetSize({ 0, 25 });

		{
			auto lbl = mCanvas->Create<Label>();						
			lbl->SetText("Reward: " + quest->GetRewardDescription());
			lbl->SetAutoHeight(true);
			mCurrentQuestArea->AddChild(lbl);
		}

		mCurrentQuestArea->AddChild(caption);
		AddStageLabel(activeColor + "Current -> ", quest->GetCurrentStage());
		for (const shared_ptr<QuestStage> & stage : quest->GetStages())
		{
			if (stage->GetStatus() != QuestStatus::Active)
			{
				AddStageLabel("", stage);
			}
		}
	}
}

void Journal::AddStageLabel(const string& prefix, const shared_ptr<QuestStage>& stage)
{
	auto lbl = mCanvas->Create<Label>();
	string txt = prefix;
	if (stage->IsCompleted())
	{
		txt += completeColor + game->GetStr("questStageComplete") + normalColor;
	}
	else if (stage->IsFailed())
	{
		txt += failedColor + game->GetStr("questStageFailed") + normalColor;
	}
	txt += stage->GetName();
	txt += ": ";
	txt += stage->GetDesc();
	lbl->SetText(txt);
	lbl->SetAutoHeight(true);
	mCurrentQuestArea->AddChild(lbl);
}

Journal::Journal()
{
	mCanvas = game->CreateCanvas();

	mWindow = mCanvas->Create<Window>();
	mWindow->SetSize({ 700, 350 });
	mWindow->SetInCenter();
	mWindow->SetCaption(game->GetStr("journal"));
	mWindow->SetAutoLayoutingEnabled(false);

	mQuestTable = mCanvas->Create<Table>();
	mQuestTable->SetSize({ mWindow->GetSize().x * 0.45f, 300 });
	mQuestTable->SetPosition({ 10, 30 });
	mQuestTable->AddColumn(game->GetStr("quest"));
	mQuestTable->AddColumn(game->GetStr("status"));
	mQuestTable->OnRowClicked += [this] { OnRowClicked(); };
	mWindow->AddChild(mQuestTable);

	mCurrentQuestArea = mCanvas->Create<ScrollableArea>();
	mCurrentQuestArea->SetPosition({ mQuestTable->GetSize().x + 10, 30 });
	mCurrentQuestArea->SetSize({ mWindow->GetSize().x * 0.55f - 20, 300 });
	mWindow->AddChild(mCurrentQuestArea);

	mCanvas->SetVisible(false);

	normalColor = ColorToStr({ 255, 255, 255 });
	activeColor = ColorToStr({ 255, 200, 0 });
	failedColor = ColorToStr({ 255, 0, 0 });
	completeColor = ColorToStr({ 0, 255, 0 });

	Log() << "Journal successfully initialized!" << LineEnd;
}

Journal::~Journal()
{
}

void Journal::Update()
{
}

bool Journal::IsVisible() const
{
	return mCanvas->IsVisible();
}

void Journal::SetVisible(bool state)
{
	if (!game->GetLevel())
	{
		return;
	}
	if (!game->GetLevel()->GetPlayer())
	{
		return;
	}
	if (state)
	{
		Sync();
	}
	mCanvas->SetVisible(state);
}

void Journal::SwitchVisible()
{
	SetVisible(!IsVisible());
}

void Journal::Sync()
{
	Clear();
	for (auto &quest : game->GetLevel()->GetQuests())
	{
		int row = mQuestTable->AddRow();
		dynamic_pointer_cast<Label>(mQuestTable->Get(0, row))->SetText(quest->GetName());
		string status;
		switch (quest->GetStatus())
		{
		case QuestStatus::Active:
			status = "Active";
			break;
		case QuestStatus::Failed:
			status = "Failed";
			break;
		case QuestStatus::Completed:
			status = "Complete";
			break;
		}
		dynamic_pointer_cast<Label>(mQuestTable->Get(1, row))->SetText(status);		
	}
}
