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

#include "Serializer.h"
#include "Reward.h"

enum class QuestStatus
{ 
	Active,
	Completed,		
	Failed 
};

class QuestStage
{
public:
	QuestStage();
	QuestStage(const string &name, const string &desc);
	void Serialize(Serializer &sav);
	const Reward& GetReward() const;
	void SetReward(const Reward& reward);
	const string& GetName() const;
	const string& GetDesc() const;
	void SetStatus(const QuestStatus& status);
	QuestStatus GetStatus() const;
	bool IsFailed() const;
	bool IsCompleted() const;
private:
	string mName;
	string mDescription;
	Reward mReward;
	QuestStatus mStatus { QuestStatus::Active };
};

class Quest
{
public:
	Quest();
	Quest(const weak_ptr<Actor>& giver);
	virtual ~Quest();
	virtual void Serialize(Serializer &sav);			
	string GetName() const;
	const vector<shared_ptr<QuestStage>> &GetStages() const;
	virtual void Update() = 0;
	QuestStatus GetStatus();
	const shared_ptr<QuestStage>& GetCurrentStage() const;
	string GetRewardDescription() const;
protected:			
	string mName;
	QuestStatus mStatus { QuestStatus::Active };
	weak_ptr<Actor> mGiver;

	shared_ptr<QuestStage> AddStage(const shared_ptr<QuestStage>& stage);
	void CompleteCurrentStageAndGoto(const shared_ptr<QuestStage>& stage);
	void FailCurrentStageAndGoto(const shared_ptr<QuestStage>& stage);
	void Complete();
	void Fail();
	bool IsCompleted() const;
	bool IsCurrentStage(const shared_ptr<QuestStage>& stage) const;
private:	
	shared_ptr<QuestStage> mCurrentStage;
	vector<shared_ptr<QuestStage>> mStageList;

	void GotoStage(const shared_ptr<QuestStage>& stage, const QuestStatus& currentStageStatus);
};

class FindItemQuest : public Quest
{
public:
	FindItemQuest();
	FindItemQuest(const weak_ptr<Actor>& giver, const shared_ptr<Item>& item);
	virtual void Update() override;
	virtual void Serialize(Serializer &sav);
private:	
	shared_ptr<Item> mItem;
	shared_ptr<QuestStage> mStageFind;
	shared_ptr<QuestStage> mStageBring;
};

class KillQuest : public Quest
{
public:
	KillQuest();	
	KillQuest(const weak_ptr<Actor>& giver, vector<weak_ptr<Actor>>&& targets, vector<sf::FloatRect>&& areas);
	virtual void Update() override;
	virtual void Serialize(Serializer &sav);
private:
	vector<weak_ptr<Actor>> mTargets;
	vector<sf::FloatRect> mAreas;
	shared_ptr<QuestStage> mKillStage;
	shared_ptr<QuestStage> mGoForReward;
};