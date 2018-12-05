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

#include "Bundle.h"
#include "AStar.h"
#include "CaveGenerator.h"
#include "PassageGenerator.h"
#include "Location.h"
#include "MonsterGenerator.h"

class ItemType;
class Level;
class Boss;
class CreatureType;

struct PlayerPreset
{
	string name { "Unnamed Preset" };
	int vitality { 0 };
	int strength { 0 };
	int dexterity { 0 };
	int endurance { 0 };
	int intelligence { 0 };
	int charisma { 0 };
	int luck { 0 };
	int money { 0 };
	int priority { 0 };
	vector<pair<ItemType, int>> startItems;
};

struct LevelGeneratorParameters
{
	int mMinLocationWidthInCells { 8 };
	int mMinLocationHeightInCells { 8 };
	int mSublocationsCount { 5 };
	Difficulty mDifficulty;
	shared_ptr<PlayerPreset> mPlayerPreset;
};

class WorldGenerator
{
public:
	mutable mutex Mutex;
	LevelGeneratorParameters mDefinition;
	MonsterGenerator mMonsterGenerator;
	static constexpr const char *Version = "1.8";
	static constexpr float monsterPlacementCavernProbability = 0.25f;
	static constexpr float monsterPlacementPassageProbability = 0.75f;

	WorldGenerator(const LevelGeneratorParameters &params);
	// Method is designed for async execution in a separate thread
	shared_ptr<Level> Generate();
	bool IsGenerated() const;
	// Use this method to get progress string from "Generate" method
	// The method is thread-safe
	string GetProgressMessage() const;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> mTimeGenerationStarted;

	shared_ptr<Level> mLevel;
	mutable string mProgressString;
	atomic_bool mGenerated { false };

	// Bundles
	Bundle<ItemType> mOreBundle;
	Bundle<ItemType> mLegendaryChestBundle;
	Math::Range<int> mMonsterCountPerIsolatedPassage;
	Math::Range<int> mLegendaryChestMoneyRange;
	Math::Range<int> mRareChestMoneyRange;
	Bundle<ItemType> mRareChestBlueprintsBundle;
	Bundle<ItemType> mRareChestItemsBundle;
	Bundle<ItemType> mRareChestCommonItemsBundle;
	vector<unique_ptr<LocationGenerator>> mGeneratorList;

	void CreatePlayer(const shared_ptr<Location> & location, const Vec2f & position);

	unique_ptr<LocationGenerator> CreateLocationGenerator(const shared_ptr<LocationDefinition>& definition);
	void InvokeLocationGenerator(const shared_ptr<LocationDefinition>& definition, vector<thread> & generationThreads);
};
