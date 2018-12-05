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
#include "QuestBase.h"
#include "Game.h"
#include "Input.h"
#include "Level.h"
#include "Player.h"
#include "HUD.h"
#include "Monster.h"
#include "Boss.h"
#include "Inventory.h"
#include "Item.h"
#include "Stats.h"

QuestStage::QuestStage()
{
}

QuestStage::QuestStage(const string &name, const string &desc)
	: mName(name)
	, mDescription(desc)
	, mStatus(QuestStatus::Active)
{
}

void QuestStage::Serialize(Serializer &sav)
{
	sav &mName;
	sav &mDescription;
	sav &mStatus;
	sav &mReward;
}

const Reward & QuestStage::GetReward() const
{
	return mReward;
}

void QuestStage::SetReward(const Reward & reward)
{
	mReward = reward;
}

const string & QuestStage::GetName() const
{
	return mName;
}

const string & QuestStage::GetDesc() const
{
	return mDescription;
}

void QuestStage::SetStatus(const QuestStatus & status)
{
	mStatus = status;
}

QuestStatus QuestStage::GetStatus() const
{
	return mStatus;
}

bool QuestStage::IsFailed() const
{
	return mStatus == QuestStatus::Failed;
}

bool QuestStage::IsCompleted() const
{
	return mStatus == QuestStatus::Completed;
}

Quest::Quest()
{

}

Quest::Quest(const weak_ptr<Actor>& giver)
	: mGiver(giver)
{

}

Quest::~Quest()
{
}

void Quest::Serialize(Serializer & sav)
{
	sav &mName;
	sav &mStageList;
	sav &mCurrentStage;
	sav & mGiver;
}

string Quest::GetName() const
{
	return mName;
}

const vector<shared_ptr<QuestStage>>& Quest::GetStages() const
{
	return mStageList;
}

QuestStatus Quest::GetStatus()
{
	return mStatus;
}

const shared_ptr<QuestStage>& Quest::GetCurrentStage() const
{
	return mCurrentStage;
}

string Quest::GetRewardDescription() const
{
	ItemDatabase* db = game->GetItemDatabase();

	Reward totalReward;
	for (const shared_ptr<QuestStage>& stage : mStageList)
	{
		totalReward += stage->GetReward();
	}

	// Count items
	unordered_map<ItemType, int> itemsCount;
	for (const ItemType& item : totalReward.GetItems())
	{
		if (itemsCount.find(item) == itemsCount.end())
		{
			itemsCount[item] = 1;
		}
		else
		{
			++itemsCount[item];
		}
	}

	string str;
	for (const pair<ItemType, int>& kv : itemsCount)
	{
		str += "x" + to_string(kv.second) + " - " + db->Get(kv.first).GetTranslatedName() + LineEnd;
	}
	str += game->GetStr("money") + to_string(totalReward.GetMoney()) + LineEnd;
	str += game->GetStr("expirience") + to_string(totalReward.GetExperience()) + LineEnd;

	return str;
}

shared_ptr<QuestStage> Quest::AddStage(const shared_ptr<QuestStage>& stage)
{
	mStageList.push_back(stage);
	if (mCurrentStage == nullptr)
	{
		mCurrentStage = stage;
	}
	return stage;
}

void Quest::CompleteCurrentStageAndGoto(const shared_ptr<QuestStage>& stage)
{
	GotoStage(stage, QuestStatus::Completed);
}

void Quest::FailCurrentStageAndGoto(const shared_ptr<QuestStage>& stage)
{
	GotoStage(stage, QuestStatus::Failed);
}

void Quest::Complete()
{
	for (const shared_ptr<QuestStage> & stage : mStageList)
	{
		if (stage->GetStatus() == QuestStatus::Active)
		{
			stage->GetReward().GiveToActor(game->GetLevel()->GetPlayer());
			stage->SetStatus(QuestStatus::Completed);
		}
	}

	mStatus = QuestStatus::Completed;

	game->GetHUD()->NotifyQuestCompletion(string("Quest completed: ") + mName);
}

void Quest::Fail()
{
	mStatus = QuestStatus::Failed;

	game->GetHUD()->NotifyQuestCompletion(string("Quest failed: ") + mName);
}

bool Quest::IsCompleted() const
{
	return mStatus == QuestStatus::Completed;
}

bool Quest::IsCurrentStage(const shared_ptr<QuestStage>& stage) const
{
	return mCurrentStage == stage;
}

void Quest::GotoStage(const shared_ptr<QuestStage>& stage, const QuestStatus & currentStageStatus)
{
	if (!mCurrentStage || mStageList.empty())
	{
		return;
	}

	const char* suffix = currentStageStatus == QuestStatus::Completed ? "completed: " : "failed: ";
	const string color = currentStageStatus == QuestStatus::Completed ? ColorToStr(sf::Color(0, 200, 0)) : ColorToStr(sf::Color(200, 0, 0));
	const string white = ColorToStr(sf::Color(255, 255, 255));

	game->GetHUD()->NotifyQuestCompletion(string("Quest stage ") + color + suffix + white + mCurrentStage->GetName());

	mCurrentStage->GetReward().GiveToActor(game->GetLevel()->GetPlayer());

	mCurrentStage->SetStatus(currentStageStatus);

	mCurrentStage = stage;
}

FindItemQuest::FindItemQuest()
{
}

FindItemQuest::FindItemQuest(const weak_ptr<Actor>& giver, const shared_ptr<Item>& item)
	: Quest(giver)
	, mItem(item)
{
	const shared_ptr<Actor>& giverActor = giver.lock();

	string name = item->GetDefinition().GetTranslatedName();

	mName = "Find " + name;

	string giverName = giverActor->GetName();

	// Stage Find
	{
		const int experience = giverActor->GetStats()->GetLevel() * 4;
		mStageFind = AddStage(make_shared<QuestStage>("Find item", giverName + " asked you to find " + name + " for him."));
		mStageFind->SetReward(Reward({ }, 0, experience));
	}

	// Stage Bring
	{
		const int experience = giverActor->GetStats()->GetLevel() * 14;
		mStageBring = AddStage(make_shared<QuestStage>("Bring item", "You found the " + name + ". You should take it to " + giverName));
		mStageBring->SetReward(Reward({ }, 200, experience));
	}
}

void FindItemQuest::Update()
{
	if (!IsCompleted())
	{
		const shared_ptr<Player>& player = game->GetLevel()->GetPlayer();

		if (IsCurrentStage(mStageFind))
		{
			if (player->GetInventory()->Contains(mItem))
			{
				CompleteCurrentStageAndGoto(mStageBring);
			}
		}
		else if (IsCurrentStage(mStageBring))
		{
			if (auto giver = mGiver.lock())
			{
				if (giver->GetInventory())
				{
					if (giver->GetInventory()->Contains(mItem))
					{
						Complete();
					}
				}
			}
		}
	}
}

void FindItemQuest::Serialize(Serializer & sav)
{
	Quest::Serialize(sav);

	sav & mItem;
	sav & mStageFind;
	sav & mStageBring;
}

KillQuest::KillQuest()
{

}

KillQuest::KillQuest(const weak_ptr<Actor>& giver, vector<weak_ptr<Actor>>&& targets, vector<sf::FloatRect>&& areas)
	: Quest(giver)
	, mTargets(std::move(targets))
	, mAreas(std::move(areas))
{
	const shared_ptr<Actor>& giverActor = giver.lock();

	mName = "Kill " + to_string(mTargets.size()) + " enemies";

	string giverName = giverActor->GetName();

	// Kill Stage
	{
		const string desc = giverName + " asked you to kill " + to_string(mTargets.size()) + " enemies. "
			+ "Target locations are marked on your map";
		mKillStage = AddStage(make_shared<QuestStage>("Kill enemies", desc));
		mKillStage->SetReward(Reward({ }, 0, 100));
	}

	// Go for reward Stage
	{
		const string desc = "All enemies are dead. You should return for reward to " + giverName + ".";
		mGoForReward = AddStage(make_shared<QuestStage>("Return for reward", desc));
		mGoForReward->SetReward(Reward({ ItemType::Weapon_Shotgun }, 0, 100));
	}
}

void KillQuest::Update()
{
	if (mGiver.expired() || mGiver.lock()->IsDead())
	{
		Fail();
	}
	else
	{
		if (IsCurrentStage(mKillStage))
		{
			int deadTargets = 0;
			for (const weak_ptr<Actor> & weakTarget : mTargets)
			{
				if (weakTarget.expired())
				{
					++deadTargets;
				}
				else
				{
					if (weakTarget.lock()->IsDead())
					{
						++deadTargets;
					}
				}
			}

			if (deadTargets == static_cast<int>(mTargets.size()))
			{
				CompleteCurrentStageAndGoto(mGoForReward);
			}
		}
		else if (IsCurrentStage(mGoForReward))
		{
			Complete();
		}
	}
}

void KillQuest::Serialize(Serializer & sav)
{
	Quest::Serialize(sav);

	sav & mTargets;
	sav & mAreas;
}
