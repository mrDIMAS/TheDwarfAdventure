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
#include "MonsterGenerator.h"

void MonsterGenerator::AddDescFromFile(const string & fn)
{
	using namespace rapidjson;

	Document doc;
	LoadJSON(fn, doc);

	MonsterDefinition desc;
	auto root = doc.GetObject();

	if (root.HasMember("bodyRadius"))
	{
		desc.mBodyRadius = abs(root["bodyRadius"].GetFloat());
	}
	if (root.HasMember("prefab"))
	{
		desc.mPrefab = root["prefab"].GetString();
	}
	if (root.HasMember("name"))
	{
		desc.mName = game->GetStr(root["name"].GetString());
	}
	if (root.HasMember("attackTime"))
	{
		desc.mAttackTime = abs(root["attackTime"].GetFloat());
	}
	if (root.HasMember("attackDistance"))
	{
		desc.mAttackDistance = abs(root["attackDistance"].GetFloat());
	}
	if (root.HasMember("baseDamage"))
	{
		desc.mBaseDamage = abs(root["baseDamage"].GetFloat());
	}
	if (root.HasMember("baseExpirienceDrop"))
	{
		desc.mBaseExpirienceDrop = abs(root["baseExpirienceDrop"].GetFloat());
	}
	if (root.HasMember("movementType"))
	{
		desc.mMovementType = CreatureMovementType(root["movementType"].GetString());
	}
	if (root.HasMember("baseExpirienceDrop"))
	{
		desc.mBaseExpirienceDrop = abs(root["baseExpirienceDrop"].GetFloat());
	}
	if (root.HasMember("flipModel"))
	{
		desc.mFlipModel = root["flipModel"].GetBool();
	}
	if (root.HasMember("attackSounds"))
	{
		auto attackSounds = root["attackSounds"].GetArray();

		for (auto & sound : attackSounds)
		{
			desc.mAttackSoundList.push_back(sound.GetString());
		}
	}
	if (root.HasMember("idleSound"))
	{
		desc.mIdleSound = root["idleSound"].GetString();
	}
	if (root.HasMember("idleSoundVolume"))
	{
		desc.mIdleSoundVolume = root["idleSoundVolume"].GetFloat();
	}
	if (root.HasMember("maxSpeed"))
	{
		desc.mMaxSpeed = root["maxSpeed"].GetFloat();
	}
	if (root.HasMember("locations"))
	{
		auto locations = root["locations"].GetArray();

		if (locations.Empty())
		{
			throw Exception("\"locations\" must contain at least one location!");
		}

		for (auto & loc : locations)
		{
			desc.mLocations.push_back(LocationType(loc.GetString()));
		}
	}
	else
	{
		desc.mLocations.push_back(LocationType::Unknown);
	}

	if (root.HasMember("levelUpFactors"))
	{
		auto & levelUpFactors = root["levelUpFactors"];
		if (levelUpFactors.HasMember("vitality"))
		{
			desc.mVitalityFactor = abs(levelUpFactors["vitality"].GetFloat());
		}
		if (levelUpFactors.HasMember("strength"))
		{
			desc.mStrengthFactor = abs(levelUpFactors["strength"].GetFloat());
		}
		if (levelUpFactors.HasMember("dexterity"))
		{
			desc.mDexterityFactor = abs(levelUpFactors["dexterity"].GetFloat());
		}
		if (levelUpFactors.HasMember("endurance"))
		{
			desc.mEnduranceFactor = abs(levelUpFactors["endurance"].GetFloat());
		}
		if (levelUpFactors.HasMember("intelligence"))
		{
			desc.mIntelligenceFactor = abs(levelUpFactors["intelligence"].GetFloat());
		}
		if (levelUpFactors.HasMember("luck"))
		{
			desc.mIntelligenceFactor = abs(levelUpFactors["luck"].GetFloat());
		}
	}

	mBase.push_back(desc);
}

MonsterGenerator::MonsterGenerator()
{
	for (auto &p : filesystem::recursive_directory_iterator("data/monsters"))
	{
		if (p.path().has_extension())
		{
			auto ext = p.path().extension().string();
			if (ext == ".json")
			{
				AddDescFromFile(p.path().string());
			}
		}
	}
}

shared_ptr<Creature> MonsterGenerator::Generate(const shared_ptr<Location> & location)
{
	int maxTries = 100;
	while (maxTries-- > 0)
	{
		auto desc = Math::RandomElement(mBase);

		if (desc.mLocations.front() == LocationType::Unknown)
		{
			return make_shared<Creature>(location, desc);
		}
		else
		{
			if (find(desc.mLocations.begin(), desc.mLocations.end(), location->GetDefinition()->GetType()) != desc.mLocations.end())
			{
				return make_shared<Creature>(location, desc);
			}
			else
			{
				// continue dropping monster descriptions until got suitable
			}
		}
	}

	return nullptr;
}
