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
#include "WorldGenerator.h"
#include "Level.h"
#include "Game.h"
#include "Renderer.h"
#include "Debris.h"
#include "Weapon.h"
#include "Chest.h"
#include "DebrisAtlas.h"
#include "Hook.h"
#include "Drill.h"
#include "PlasmaDrill.h"
#include "Shotgun.h"
#include "GrenadeLauncher.h"
#include "RocketLauncher.h"
#include "SoilGun.h"
#include "Ladder.h"
#include "ItemDatabase.h"
#include "Dialog.h"
#include "MotherCrystal.h"
#include "Remains.h"
#include "QuestBase.h"
#include "Stats.h"
#include "Item.h"
#include "Player.h"
#include "Terrain.h"
#include "Text.h"
#include "Scene.h"
#include "PolyLine.h"
#include "Projectile.h"
#include "LiquidParticle.h"
#include "Inventory.h"
#include "Light.h"
#include "ParticleSystem.h"
#include "Door.h"
#include "Harpoon.h"
#include "TextureStack.h"
#include "RevealGun.h"
#include "AcidGun.h"
#include "RailGun.h"
#include "PassageGenerator.h"
#include "Input.h"
#include "Shield.h"
#include "WaterBlob.h"
#include "CaveGenerator.h"
#include "Animation.h"
#include "AnimationTrack.h"
#include "RoomGenerator.h"
#include "NPC.h"
#include "BossGolem.h"
#include "Sound.h"
#include "Workbench.h"
#include "Smoke.h"
#include "Island.h"
#include "Room.h"
#include "FogGate.h"
#include "NaturalCaveGenerator.h"
#include "LocationMap.h"
#include "FlyingIslandsGenerator.h"
#include "SettlementGenerator.h"
#include "ResourceDatabase.h"
#include "BossAreaGenerator.h"
#include "PerlinNoise.h"
#include "BossCrystal.h"
#include "BankGenerator.h"
#include "HubGenerator.h"
#include "UndergroundSettlementGenerator.h"
#include "Passage.h"
#include "TonnelVertex.h"

void WorldGenerator::CreatePlayer(const shared_ptr<Location> & location, const Vec2f & position)
{
	const shared_ptr<Player> player = make_shared<Player>(location);
	mLevel->SetPlayer(player);
	player->CreateInventory();
	const shared_ptr<Inventory> inventory = player->GetInventory();

	auto preset = mDefinition.mPlayerPreset;

	// Sort items by quick access priority based on item class
	auto sortingCriteria = [] (const pair<ItemType, int> & left, const pair<ItemType, int> & right)
	{
		const auto &db = game->GetItemDatabase();

		const auto &leftDesc = db->Get(left.first);
		const auto &rightDesc = db->Get(right.first);

		int priority[2] = { 0 };
		for (int i = 0; i < 2; ++i)
		{
			const auto &desc = (i == 0) ? leftDesc : rightDesc;

			if (desc.mClass == ItemClass::Weapon)
			{
				priority[i] = 0;
			}
			else if (desc.mClass == ItemClass::Shield)
			{
				priority[i] = 1;
			}
			else if (desc.mClass == ItemClass::Potion)
			{
				priority[i] = 2;
			}
			else
			{
				priority[i] = 3;
			}
		}

		return priority[0] < priority[1];
	};

	sort(preset->startItems.begin(), preset->startItems.end(), sortingCriteria);

	int quickAccessSlot = 0;
	for (auto &kv : preset->startItems)
	{
		const auto &db = game->GetItemDatabase();
		const auto &desc = db->Get(kv.first);
		for (int i = 0; i < kv.second; ++i)
		{
			auto item = db->Make(kv.first, location);
			inventory->AddItem(item);
			// Equip items
			if (desc.mClass == ItemClass::Helmet || desc.mClass == ItemClass::Armor || desc.mClass == ItemClass::Gloves ||
				desc.mClass == ItemClass::Trousers || desc.mClass == ItemClass::Weapon || desc.mClass == ItemClass::Ring)
			{
				player->UseItem(item, Hand::Left);
			}
			if (desc.mClass == ItemClass::Shield)
			{
				player->UseItem(item, Hand::Right);
			}
		}
		// Set items on quick access. The order is set on previous stage
		if (desc.mClass == ItemClass::Weapon || desc.mClass == ItemClass::Potion || desc.mClass == ItemClass::Shield)
		{
			player->SetQuickAccess(inventory->FindGroup(kv.first), quickAccessSlot++);
		}
	}

	inventory->AddMoney(preset->money);

	// Set stats
	auto &stats = player->GetStats();
	stats->Vitality.Set(preset->vitality);
	stats->Strenght.Set(preset->strength);
	stats->Dexterity.Set(preset->dexterity);
	stats->Intelligence.Set(preset->intelligence);
	stats->Endurance.Set(preset->endurance);
	stats->Charisma.Set(preset->charisma);
	stats->Luck.Set(preset->luck);

	// Set position
	player->SetPosition(position);

	// Add player to current location
	location->AddActor(player);
}

unique_ptr<LocationGenerator> WorldGenerator::CreateLocationGenerator(const shared_ptr<LocationDefinition>& definition)
{
	switch (definition->GetType())
	{
	case LocationType::Bank: return make_unique<BankGenerator>(mLevel.get(), definition);
	case LocationType::Caves: return make_unique<CaveGenerator>(mLevel.get(), definition);
	case LocationType::Hub: return make_unique<HubGenerator>(mLevel.get(), definition);
	case LocationType::Settlement: return make_unique<SettlementGenerator>(mLevel.get(), definition);
	case LocationType::NaturalCaves: return make_unique<NaturalCaveGenerator>(mLevel.get(), definition);
	case LocationType::FlyingIsland: return make_unique<FlyingInslandsGenerator>(mLevel.get(), definition);
	case LocationType::UndergroundSettlement: return make_unique<UndergroundSettlementGenerator>(mLevel.get(), definition);
	default: throw Exception("Generator is not specified for " + definition->GetType().ToString() + " location type!");
	}
}

void WorldGenerator::InvokeLocationGenerator(const shared_ptr<LocationDefinition>& definition, vector<thread>& generationThreads)
{
	lock_guard<mutex> guard(Mutex);

	unique_ptr<LocationGenerator> generator = CreateLocationGenerator(definition);

	if (game->Debug.useMultithreadedWorldGenerator)
	{
		auto generationThread = [this, rawGeneratorPtr = generator.get()]
		{
			try
			{
				auto type = rawGeneratorPtr->GetDefinition()->GetType();

				Mutex.lock();
				Log() << "Generation thread for location " << type.ToString() << " was created!" << LineEnd;
				Mutex.unlock();

				rawGeneratorPtr->Generate();
				rawGeneratorPtr->MarkAsDone();

				Mutex.lock();
				Log() << "Generation thread for location " << type.ToString() << " was done its job! Location is generated successfully!" << LineEnd;
				Mutex.unlock();
			}
			catch (std::exception &e)
			{
				ReportError(e.what());
			}
		};

		generationThreads.push_back(thread(generationThread));
	}
	else
	{
		generator->Generate();
		generator->MarkAsDone();
	}

	mGeneratorList.push_back(std::move(generator));
}

string WorldGenerator::GetProgressMessage() const
{
	lock_guard<mutex> guard(Mutex);

	mProgressString.clear();

	if (game->Debug.useMultithreadedWorldGenerator)
	{
		mProgressString += ColorToStr({ 255, 255, 255 }) + "Using multithreaded world generator\n";
	}
	else
	{
		mProgressString += ColorToStr({ 255, 0, 0 }) + "Using singlethreaded world generator (slow)\n" + ColorToStr({ 255, 255, 255 });
	}

	auto now = chrono::high_resolution_clock::now();
	auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(now - mTimeGenerationStarted).count();

	mProgressString += "Elapsed time: " + RoundAndConvert(elapsedTime / 1000.0f, 2) + " seconds\n";

	// Gather messages from all generators
	int locationNum = 0;
	for (const unique_ptr<LocationGenerator> & generator : mGeneratorList)
	{
		const shared_ptr<Location> location = generator->GetLocation();
		const string typeName = location ? location->GetDefinition()->GetType().ToString() : "Unknown";

		mProgressString += "L" + to_string(locationNum) + " " + typeName + " - ";
		if (generator->IsDone())
		{
			mProgressString += ColorToStr({ 0, 255, 0 }) + "Done!\n" + ColorToStr({ 255, 255, 255 });
		}
		else
		{
			mProgressString += ColorToStr({ 255, 255, 255 }) + generator->GetProgressMessage() + "\n";
		}
		++locationNum;
	}

	return mProgressString;
}

bool WorldGenerator::IsGenerated() const
{
	return mGenerated;
}

WorldGenerator::WorldGenerator(const LevelGeneratorParameters & params)
	: mDefinition(params)
{

}

shared_ptr<Level> WorldGenerator::Generate()
{
	if (game->Debug.useMultithreadedWorldGenerator)
	{
		Log() << "Using multithreaded world generator" << LineEnd;
	}
	else
	{
		Log() << "WARNING: Using single threaded world generator. This may take a while to generate world!" << LineEnd;
	}

	mGenerated = false;

	mLevel = make_shared<Level>();
	mLevel->SetDifficulty(mDefinition.mDifficulty);

	mTimeGenerationStarted = chrono::high_resolution_clock::now();

	const Math::Range<int> locationSizeRange = { 6, 10 };

	// Define locations
	vector<shared_ptr<LocationDefinition>> locationDefinitions;
	{
		// Fill bundle of generic locations
		Bundle<LocationType> genericLocations = {
			{ 0.2f, LocationType::Caves },
			{ 0.2f, LocationType::UndergroundSettlement },
			{ 0.2f, LocationType::NaturalCaves },
		};

		// Add unique locations
		locationDefinitions.push_back(make_shared<LocationDefinition>(LocationType::Hub, locationSizeRange.RandomValue(), locationSizeRange.RandomValue()));
		locationDefinitions.push_back(make_shared<LocationDefinition>(LocationType::Bank, locationSizeRange.RandomValue(), locationSizeRange.RandomValue()));
		locationDefinitions.push_back(make_shared<LocationDefinition>(LocationType::Settlement, locationSizeRange.RandomValue(), locationSizeRange.RandomValue()));
		locationDefinitions.push_back(make_shared<LocationDefinition>(LocationType::FlyingIsland, locationSizeRange.RandomValue(), locationSizeRange.RandomValue()));
		locationDefinitions.push_back(make_shared<LocationDefinition>(LocationType::UndergroundSettlement, locationSizeRange.RandomValue(), locationSizeRange.RandomValue()));

		const int genericCount = 3;
		// Add some generic locations
		for (int i = 0; i < genericCount; ++i)
		{
			locationDefinitions.push_back(make_shared<LocationDefinition>(genericLocations.Drop(), locationSizeRange.RandomValue(), locationSizeRange.RandomValue()));
		}
	}

	
	shared_ptr<LocationDefinition> playerLocationDefinition;
	for (const shared_ptr<LocationDefinition>& definition : locationDefinitions)
	{
		// Generate names 
		definition->SetName(definition->GetType().ToString());

		// Find player location definition
		if (definition->GetType() == LocationType::UndergroundSettlement)
		{
			playerLocationDefinition = definition;
		}
	}

	// Generate global structure first
	{
		// The world structure generated as relative neighbourhood graph
		// This graph also used as global world map
		//
		// All coordinates of vertices are given in normalized coords [0; 1]
		vector<unique_ptr<GraphVertex>> graphVertices;

		GraphVertex *playerApartments = nullptr;
		for (const shared_ptr<LocationDefinition>& definition : locationDefinitions)
		{
			float x = Math::Rnd(0.0f, 1.0f);
			float y = Math::Rnd(0.0f, 1.0f);

			// Make sure, that vertices does not overlap
			for (const shared_ptr<LocationDefinition> & otherDefinition : locationDefinitions)
			{
				int maxIterations = 300;
				while (Math::Distance(otherDefinition->GetPosition(), { x, y }) < 0.11 && maxIterations > 0)
				{
					x = Math::Rnd(0.0f, 1.0f);
					y = Math::Rnd(0.0f, 1.0f);

					--maxIterations;
				}
			}

			definition->SetPosition({ x, y });

			unique_ptr<GraphVertex> graphVertex = make_unique<GraphVertex>(definition->GetPosition());
			graphVertex->UserData = definition.get();
			if (definition == playerLocationDefinition)
			{
				playerApartments = graphVertex.get();
			}
			graphVertices.push_back(move(graphVertex));
		}

		// Connect vertices
		for (size_t aid = 0; aid < locationDefinitions.size(); aid++)
		{
			for (size_t bid = aid + 1; bid < locationDefinitions.size(); bid++)
			{
				bool isEdge = true;
				const shared_ptr<LocationDefinition>& a = locationDefinitions[aid];
				const shared_ptr<LocationDefinition>& b = locationDefinitions[bid];
				float abDist = Math::SqrDistance(a->GetPosition(), b->GetPosition());
				for (size_t cid = 0; cid < locationDefinitions.size(); cid++)
				{
					if (cid == aid || cid == bid)
					{
						continue;
					}
					const shared_ptr<LocationDefinition>& c = locationDefinitions[cid];
					const float acDist = Math::SqrDistance(a->GetPosition(), c->GetPosition());
					const float bcDist = Math::SqrDistance(b->GetPosition(), c->GetPosition());
					if (acDist < abDist && bcDist < abDist)
					{
						isEdge = false;
						break;
					}
				}
				if (isEdge)
				{
					a->LinkTo(b);

					graphVertices[aid]->Link(graphVertices[bid].get());
				}
			}
		}

		// Initialize pathfinder
		AStarPathfinder pathfinder;

		float maxDistanceToPlayerApartment = 0.0f;
		for (const unique_ptr<GraphVertex> & v : graphVertices)
		{
			// Calculate distance to players apartment and calculate base level of creatures for current
			// location according to it
			vector<GraphVertex*> path;
			pathfinder.BuildPath(graphVertices, playerApartments, v.get(), path, 0);

			LocationDefinition* definition = reinterpret_cast<LocationDefinition*>(v->UserData);
			const float distance = static_cast<float>(path.size());
			definition->SetDistanceToPlayerLocation(distance);

			if (distance > maxDistanceToPlayerApartment)
			{
				maxDistanceToPlayerApartment = distance;
			}
		}

		// Adjust base level of each location
		for (const shared_ptr<LocationDefinition> & definition : locationDefinitions)
		{
			const float k = definition->GetDistanceToPlayerLocation() / maxDistanceToPlayerApartment;
			definition->SetBaseLevel(static_cast<int>(Math::Max(1.0f, mLevel->GetMaxBaseLevel() * k)));
		}
	}

	vector<thread> generationThreads;
	for (const shared_ptr<LocationDefinition>& definition : locationDefinitions)
	{
		InvokeLocationGenerator(definition, generationThreads);
	}

	// Wait until all threads do their job
	for (thread & genThread : generationThreads)
	{
		if (genThread.joinable())
		{
			genThread.join();
		}
	}

	for (const shared_ptr<Location> & location : mLevel->GetLocationList())
	{
		// Adjust actors level
		for (const shared_ptr<Actor> & actor : location->GetActorList())
		{
			actor->SetLevel(location->GetDefinition()->GetBaseLevel());
		}
	}

	// Create player

	for (const shared_ptr<Location> & location : mLevel->GetLocationList())
	{
		if (location->GetDefinition() == playerLocationDefinition)
		{
			const shared_ptr<Room> room = Math::RandomElement(location->GetRoomList());

			// Kill previous inhabitant first
			for (const weak_ptr<Actor>& weakInhabitantPtr : room->GetInhabitants())
			{
				const shared_ptr<Actor> inhabitant = weakInhabitantPtr.lock();

				if (inhabitant)
				{
					location->RemoveActor(inhabitant);
				}
			}
			room->EvictAllInhabitants();
			CreatePlayer(location, Math::RectCenter(room->GetBounds()));
			// And take his apartment
			room->AddInhabitant(mLevel->GetPlayer());

			break;
		}
	}
	
	auto GetLocationDoorPosition = [this] (const shared_ptr<Location> & location, const Vec2f & doorSize) -> Vec2f
	{
		Vec2f doorPosition;

		switch (location->GetDefinition()->GetType())
		{
		case LocationType::Caves:
		{
			doorPosition = Math::RandomElement(location->GetTonnelVertices())->GetPosition();
			break;
		}
		case LocationType::FlyingIsland:
		{
			const shared_ptr<Island> island = Math::RandomElement(location->GetIslandList());
			doorPosition.x = (island->mBaseTriangle.mPoints[0].x + island->mBaseTriangle.mPoints[1].x) / 2;
			doorPosition.y = island->mBaseTriangle.mPoints[0].y - doorSize.y;
			break;
		}
		case LocationType::NaturalCaves:
		{
			doorPosition = { 100, 100 }; // todo: fix
			break;
		}
		case LocationType::Settlement:
		{
			const shared_ptr<Room> room = Math::RandomElement(location->GetRoomList());

			doorPosition = {
				room->GetBounds().left + room->GetBounds().width / 2 - doorSize.x / 2,
				room->GetBounds().top + room->GetBounds().height - doorSize.y
			};

			break;
		}
		default:
		{
			if (location->GetPassageList().empty())
			{
				doorPosition = { 100, 100 }; // todo: fix
			}
			else
			{
				// Find random horizontal passage
				shared_ptr<Passage> passage;
				do
				{
					passage = Math::RandomElement(location->GetPassageList());
				} while (passage->GetType() != PassageType::Horizontal);

				const sf::FloatRect passageBounds = passage->GetBounds();

				int tryCount = 30;
				while (tryCount-- > 0)
				{
					doorPosition = {
						Math::Rnd(passageBounds.left, passageBounds.left + passageBounds.width - doorSize.x),
						passageBounds.top + passageBounds.height - doorSize.y
					};

					for (const sf::FloatRect & decoration : passage->GetDecorationBounds())
					{
						if (decoration.intersects({ doorPosition, doorSize }))
						{
							goto nextTry;
						}
					}

					for (auto & passageSpacer : passage->GetSpacerBounds())
					{
						if (passageSpacer.intersects({ doorPosition, doorSize }))
						{
							goto nextTry;
						}
					}

				nextTry:;
				}
			}

			break;
		}
		}

		return doorPosition;
	};

	for (const shared_ptr<Location> & location : mLevel->GetLocationList())
	{
		for (const weak_ptr<LocationDefinition> &weakNeighbourLocation : location->GetDefinition()->GetLinkedLocations())
		{
			const shared_ptr<Location> neighbourLocation = weakNeighbourLocation.lock()->GetLocation();

			DoorModelType dmt;
			switch (location->GetDefinition()->GetType())
			{
			case LocationType::FlyingIsland:
				dmt = DoorModelType::Portal;
				break;
			default:
				dmt = DoorModelType::Front;
			}

			const Vec2f doorSize = Door::FetchBoxSize(dmt);

			Vec2f srcDoorPosition = GetLocationDoorPosition(location, doorSize);
			const shared_ptr<Door> srcDoor = make_shared<Door>(location, DoorType::ToOtherScene, dmt, srcDoorPosition);
			location->AddDoor(srcDoor);

			Vec2f destDoorPosition = GetLocationDoorPosition(neighbourLocation, doorSize);
			const shared_ptr<Door> destDoor = make_shared<Door>(neighbourLocation, DoorType::ToOtherScene, dmt, destDoorPosition);
			neighbourLocation->AddDoor(destDoor);

			srcDoor->SetDestinationDoor(destDoor);
			destDoor->SetDestinationDoor(srcDoor);
		}
	}

	// load maps on gpu
	for (auto &loc : mLevel->GetLocationList())
	{
		game->GetRenderer()->ScheduleTextureGPUUpload(loc->GetMap()->GetTexture());
	}
	
	auto now = chrono::high_resolution_clock::now();
	auto elapsedTime = chrono::duration_cast<chrono::milliseconds>(now - mTimeGenerationStarted).count();

	Log() << "Successfully generated in " << elapsedTime << " ms." << LineEnd;

	mGenerated = true;

	return mLevel;
}

