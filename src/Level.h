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

#include "ItemDatabase.h"

#include "Bundle.h"
#include "Monster.h"
#include "NPC.h"
#include "Location.h" 

class QuestBase;

class Level : public enable_shared_from_this<Level>
{
private:
	vector<shared_ptr<Location>> mLocationList;
	shared_ptr<Player> mPlayer;
	vector<shared_ptr<Quest>> mQuests;
	unique_ptr<DebrisAtlas> mDebrisAtlas;
	int mMaxBaseLevel { 0 };
	void UpdateDebrisAtlas();
	Difficulty mDifficulty { Difficulty::Medium };
public:
	Level();
	~Level();
	void Update();
	void CreateDebrisAtlas();
	void SetPlayer(const shared_ptr<Player> & player);
	shared_ptr<Player> GetPlayer();
	void Serialize(Serializer &sav);
	unique_ptr<DebrisAtlas> &GetDebrisAtlas();
	const vector<shared_ptr<Quest>> & GetQuests() const;
	void AddQuest(const shared_ptr<Quest> & quest);
	shared_ptr<Location> GetMainLocation() const;
	void TransferPlayerToLocation(const shared_ptr<Location> &destLocation, const Vec2f &destPos);
	void AddLocation(const shared_ptr<Location> & location);
	const vector<shared_ptr<Location>> &GetLocationList() const;		
	int GetMaxBaseLevel() const;
	void SetDifficulty(Difficulty diff);
	Difficulty GetDifficulty();
};

