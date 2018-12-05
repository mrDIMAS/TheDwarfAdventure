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
#include "Level.h"
#include "Game.h"
#include "Renderer.h"
#include "Debris.h"
#include "DebrisAtlas.h"
#include "Player.h"
#include "QuestBase.h"
#include "ItemType.h"

void Level::UpdateDebrisAtlas()
{
	// upload debris atlas to gpu
	if (mDebrisAtlas->NeedUploadToGpu)
	{
		game->GetRenderer()->ScheduleTextureGPUUpload(mDebrisAtlas->GetTexture());
		mDebrisAtlas->NeedUploadToGpu = false;
	}
}

Level::Level()
{
	CreateDebrisAtlas();
}

Level::~Level()
{
	DBG_ECHO();
}

void Level::Update()
{
	mPlayer->GetLocation()->Update();
	for (auto & quest : mQuests)
	{
		quest->Update();
	}
	UpdateDebrisAtlas();
}

void Level::CreateDebrisAtlas()
{
	mDebrisAtlas = make_unique<DebrisAtlas>();
}

void Level::SetPlayer(const shared_ptr<Player> & player)
{
	mPlayer = player;
}

shared_ptr<Player> Level::GetPlayer()
{
	return mPlayer;
}

void Level::Serialize(Serializer &sav)
{
	sav &mLocationList;
	sav &mPlayer;
	sav &mQuests;
}

unique_ptr<DebrisAtlas> &Level::GetDebrisAtlas()
{
	return mDebrisAtlas;
}

const vector<shared_ptr<Quest>>& Level::GetQuests() const
{
	return mQuests;
}

void Level::AddQuest(const shared_ptr<Quest>& quest)
{
	mQuests.push_back(quest);
}

shared_ptr<Location> Level::GetMainLocation() const
{
	return mLocationList.at(0);
}

void Level::TransferPlayerToLocation(const shared_ptr<Location> &destLocation, const Vec2f &destPos)
{
	if (!destLocation)
	{
		throw Exception("Unable to transfer player to nullptr location!");
	}

	if (auto previousLocation = mPlayer->GetLocation())
	{
		previousLocation->GetScene()->SetVisible(false);
		previousLocation->GetScene()->GetSimulationIsland()->SetEnabled(false);
		previousLocation->RemoveActor(mPlayer);
	}

	destLocation->GetScene()->AcceptNode(mPlayer->GetPivot());
	mPlayer->SetPosition(destPos);
	mPlayer->SetLocation(destLocation);
	destLocation->GetScene()->SetVisible(true);
	destLocation->GetScene()->GetSimulationIsland()->SetEnabled(true);
	destLocation->AddActor(mPlayer);
	destLocation->Visited = true;
}

void Level::AddLocation(const shared_ptr<Location> & location)
{
	THROW_IF_NULL(location);
	mLocationList.push_back(location);
}

const vector<shared_ptr<Location>> &Level::GetLocationList() const
{
	return mLocationList;
}

int Level::GetMaxBaseLevel() const
{
	return mMaxBaseLevel;
}

void Level::SetDifficulty(Difficulty diff)
{
	mDifficulty = diff;

	switch (mDifficulty)
	{
	case Difficulty::Easy:
		mMaxBaseLevel = 30;
		break;
	case Difficulty::Medium:
		mMaxBaseLevel = 40;
		break;
	case Difficulty::Hard:
		mMaxBaseLevel = 50;
		break;
	case Difficulty::Realistic:
		mMaxBaseLevel = 60;
		break;
	}
}

Difficulty Level::GetDifficulty()
{
	return mDifficulty;
}
