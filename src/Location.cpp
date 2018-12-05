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
#include "Location.h"
#include "Game.h"
#include "Renderer.h"
#include "Debris.h"
#include "Weapon.h"
#include "Chest.h"
#include "DebrisAtlas.h"
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
#include "Input.h"
#include "Shield.h"
#include "WaterBlob.h"
#include "Animation.h"
#include "AnimationTrack.h"
#include "RoomGenerator.h"
#include "NPC.h"
#include "Sound.h"
#include "Workbench.h"
#include "Smoke.h"
#include "Level.h"
#include "Room.h"
#include "FogGate.h"
#include "LocationMap.h"
#include "Passage.h"
#include "Island.h"
#include "TonnelVertex.h"
#include "Cloud.h"
#include "AttributeGrid.h"
#include "Arena.h"

enum ZoneID : uint16_t
{
	OutOfBoundsPixel,
	VisiblePixel,
	InvisiblePixel,
	ZoneIDOrigin,
};

enum
{
	Passable,
	Impassable
};

Location::Location()
{
}

Location::Location(Level * level, const shared_ptr<LocationDefinition>& definition)
	: mLevel(level)
	, mClass(LocationClass::Terrestrial)
	, mDefinition(definition)
{
	lock_guard<recursive_mutex> guard(game->GetRenderer()->GetMutex());
		
	mIdentifier = Guid::Generate();

	mScene = game->CreateScene();
	// prevent rendering during generation
	mScene->SetVisible(false);
}

Location::~Location()
{
	game->RemoveScene(mScene);
}

void Location::AddDebris(const shared_ptr<Debris> &debris)
{
	mDebrisList.push_back(debris);
}

void Location::AddActor(const shared_ptr<Actor> &actor)
{
	if (actor)
	{
		mActorList.push_back(actor);	
	}
}

void Location::RemoveActor(const shared_ptr<Actor> &actor)
{
	if (actor)
	{
		mActorList.erase(std::remove(mActorList.begin(), mActorList.end(), actor), mActorList.end());
	}
}

void Location::AddItem(const shared_ptr<Item> &item)
{
	mItemList.push_back(item);
}

void Location::RemoveItem(const shared_ptr<Item> &item)
{
	mItemList.erase(std::remove(mItemList.begin(), mItemList.end(), item), mItemList.end());
}

vector<shared_ptr<Actor>> &Location::GetActorList()
{
	return mActorList;
}

void Location::AddProjectile(const shared_ptr<Projectile>& projectile)
{
	mProjectileList.push_back(projectile);
}

void Location::AddLiquidParticle(const shared_ptr<LiquidParticle>& liquid)
{
	mLiquidParticleList.push_back(liquid);
}

void Location::RemoveLiquidParticle(const shared_ptr<LiquidParticle> &p)
{
	mLiquidParticleList.erase(std::remove(mLiquidParticleList.begin(), mLiquidParticleList.end(), p), mLiquidParticleList.end());
}

void Location::AddChest(const shared_ptr<Chest> &chest)
{
	mChestList.push_back(chest);
}

void Location::RemoveChest(const shared_ptr<Chest> &chest)
{
	mChestList.erase(std::remove(mChestList.begin(), mChestList.end(), chest), mChestList.end());
}

vector<shared_ptr<Chest>> &Location::GetChestList()
{
	return mChestList;
}

vector<shared_ptr<Item>> &Location::GetItemList()
{
	return mItemList;
}

void Location::AddLadder(const shared_ptr<Ladder> &ladder)
{
	mLadderList.push_back(ladder);
}

void Location::RemoveLadder(const shared_ptr<Ladder> &ladder)
{
	mLadderList.erase(std::remove(mLadderList.begin(), mLadderList.end(), ladder), mLadderList.end());
}

vector<shared_ptr<Ladder>> &Location::GetLadderList()
{
	return mLadderList;
}

const vector<shared_ptr<MotherCrystal>> &Location::GetMotherCrystalList() const
{
	return mMotherCrystalList;
}

void Location::AddRemains(const shared_ptr<Remains> &remains)
{
	mRemains.push_back(remains);
}

void Location::RemoveRemains(const shared_ptr<Remains> &remains)
{
	mRemains.erase(std::remove(mRemains.begin(), mRemains.end(), remains), mRemains.end());
}

const vector<shared_ptr<Remains>> &Location::GetRemainsList() const
{
	return mRemains;
}

void Location::AddDoor(const shared_ptr<Door> &door)
{
	mDoorList.push_back(door);
}

const vector<shared_ptr<Door>> &Location::GetDoorList() const
{
	return mDoorList;
}

void Location::AddTonnelVertex(const shared_ptr<TonnelVertex>& tonnelVertex)
{
	mTonnelVertices.push_back(tonnelVertex);
}

const vector<shared_ptr<TonnelVertex>>& Location::GetTonnelVertices() const
{
	return mTonnelVertices;
}

void Location::AddArena(const shared_ptr<Arena>& arena)
{
	mArenaList.push_back(arena);
}

const vector<shared_ptr<Arena>>& Location::GetArenaList() const
{
	return mArenaList;
}

shared_ptr<Scene> &Location::GetScene()
{
	return mScene;
}

void Location::UpdateActors()
{
	// static vector to decrease unnecessary allocations during runtime
	static vector<shared_ptr<Actor>> actors;
	actors = mActorList; // copy actors, cuz mActorList can be modified inside

    // Actor::Update/Actor::Think
	for (const auto & actor : actors)
	{
		actor->Update();
		actor->Think();
	}
	// clear static list to be sure, that we do not store strong ref to any actor here
	actors.clear();
	// erase dead actors
	for (auto iter = mActorList.begin(); iter != mActorList.end();)
	{
		auto actor = *iter;
		if (actor->IsDead() && actor != mLevel->GetPlayer()) // keep player alive (HAAAX)
		{
			// remove actor
			iter = mActorList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Location::UpdateDebris()
{
	for (auto iter = mDebrisList.begin(); iter != mDebrisList.end();)
	{
		auto &debris = *iter;
		if (debris->IsDead())
		{
			iter = mDebrisList.erase(iter);
		}
		else
		{
			debris->Update();
			++iter;
		}
	}
}

void Location::UpdateLiquid()
{
	for (auto iter = mLiquidParticleList.begin(); iter != mLiquidParticleList.end();)
	{
		auto &liquid = *iter;
		if (liquid->IsDead())
		{
			iter = mLiquidParticleList.erase(iter);
		}
		else
		{
			liquid->Update();
			++iter;
		}
	}
}

void Location::UpdateProjectiles()
{
	for (auto iter = mProjectileList.begin(); iter != mProjectileList.end();)
	{
		auto projectile = *iter;
		if (projectile->IsDead())
		{
			iter = mProjectileList.erase(iter);
		}
		else
		{
			projectile->Update();
			++iter;
		}
	}
}

void Location::UpdateItems()
{
	for (auto &item : mItemList)
	{
		item->Update();
	}
}

void Location::UpdateMessageMarks()
{
	for (auto iter = mReportMessageList.begin(); iter != mReportMessageList.end();)
	{
		auto mark = *iter;
		const float fadeOutFactor = 0.99f;
		auto c = mark->GetColor();
		c.a = static_cast<uint8_t>(c.a * fadeOutFactor);
		mark->SetColor(c);
		mark->LocalMove({ 0, -0.4f });
		if (c.a < 1)
		{
			mark->RemoveFromScene();
			iter = mReportMessageList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Location::UpdateRemains()
{
	for (auto iter = mRemains.begin(); iter != mRemains.end();)
	{
		if ((*iter)->IsEmpty())
		{
			iter = mRemains.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Location::UpdateClouds()
{
	for (auto &cloud : mCloudList)
	{
		cloud->mSprite->LocalMove({ cloud->mSpeed, 0 });
		if (cloud->mSprite->GetGlobalPosition().x > mScene->GetTerrain()->GetWidthPixels())
		{
			cloud->mSprite->SetLocalPosition(-cloud->mSprite->GetSize().x, cloud->mSprite->GetGlobalPosition().y);
		}
	}
}

void Location::UpdateDoors()
{
	for (auto &door : mDoorList)
	{
		door->Update();
	}
}

LocationClass Location::GetClass() const
{
	return mClass;
}

void Location::SetClass(LocationClass locClass)
{
	mClass = locClass;
}

void Location::Serialize(Serializer &sav)
{
	mLevel = game->GetLevel().get();
	sav &mScene;
	sav &mActorList;
	sav &mProjectileList;
	sav &mDebrisList;
	sav &mItemList;
	sav &mChestList;
	sav &mCloudList;
	sav &mLadderList;
	sav &mMotherCrystalList;
	sav &mReportMessageList;
	sav &mDoorList;
	sav &mRoomList;
	sav &mMap;
	sav &mNavMeshRebuildTimer;
	sav &mWorkbenchList;
	sav &mFogGates;
	sav &mSmokeList;
	sav &mDefinition;
	sav &mClass;
	sav &mIdentifier;
	sav &mPassageList;
	sav &mIslandList;
	sav &mTonnelVertices;
	sav &mArenaList;	
}

Level *Location::GetLevel() const
{
	return mLevel;
}

void Location::ReportMessage(const string &msg, const Vec2f &position, const sf::Color &clr)
{
	auto mark = mScene->CreateText();
	mark->SetLocalPosition(position);
	mark->SetAlignment(TextAlignment::Center);
	mark->SetFont(game->GetRenderer()->GetFont("data/fonts/font.ttf"));
	mark->SetSize({ 90, 20 });
	mark->SetColor(clr);
	mark->SetCharacterSize(13);
	mark->SetText(msg);
	mReportMessageList.push_back(mark);
}

void Location::Update()
{
	UpdateActors();
	UpdateDebris();
	UpdateLiquid();
	UpdateProjectiles();
	UpdateItems();
	UpdateMessageMarks();
	UpdateRemains();
	UpdateClouds();
	UpdateDoors();

	for (const shared_ptr<Arena>& arena : mArenaList)
	{
		arena->Update();
	}

	for (auto &motherCrystal : mMotherCrystalList)
	{
		motherCrystal->Update();
	}

	for (auto iter = mSmokeList.begin(); iter != mSmokeList.end();)
	{
		auto &smoke = *iter;
		if (smoke->IsDead())
		{
			iter = mSmokeList.erase(iter);
		}
		else
		{
			smoke->Update();
			++iter;
		}
	}

	for (auto &room : mRoomList)
	{
		room->Update();
	}

	// if player has a bad reputation, close entrance doors in all rooms
	if (auto player = mLevel->GetPlayer())
	{
		for (auto &room : mRoomList)
		{
			const shared_ptr<Actor> inhabitant = room->GetMostPowerfulInhabitant();

			if (inhabitant)
			{
				if (inhabitant == player)
				{
					continue;
				}

				for (auto & door : room->GetDoors())
				{
					if (player->IsHaveBadReputation())
					{
						inhabitant->CloseDoor(door, true);
					}
					else
					{
						inhabitant->UnlockDoor(door);
					}
				}
			}
		}
	}

	// rebuild navmesh
	if (mNavMeshRebuildTimer <= 0)
	{
		// debug
		{
			for (auto &p : mDebugWalkableVertices)
			{
				p->RemoveFromScene();
			}
			mDebugWalkableVertices.clear();
		}

		auto terrain = mScene->GetTerrain();
		terrain->BuildNavMesh();
		// mark on-ladder vertices as walkable
		for (auto &v : terrain->GetPathfinder().GetNavmesh())
		{
			sf::FloatRect vRect = { v->GetPosition(), { Terrain::NavmeshStepF, Terrain::NavmeshStepF } };
			for (auto &ladder : mLadderList)
			{
				if (vRect.intersects(ladder->GetBounds()))
				{
					v->SetFlags(PassabilityFlag_Walk);
				}
			}
		}
		for (auto &v : terrain->GetPathfinder().GetNavmesh())
		{
			int walkableNeighbours = 0;
			for (size_t i = 0; i < v->GetNeighbourCount(); ++i)
			{
				auto n = v->GetNeighbour(i);

				if (n->GetFlags() & PassabilityFlag_Walk)
				{
					++walkableNeighbours;
					if (walkableNeighbours > 1)
					{
						v->SetFlags(PassabilityFlag_Walk);
						break;
					}
				}
			}
			// debug
			if (game->Debug.showWalkableVertices)
			{
				if (v->GetFlags() & PassabilityFlag_Walk)
				{
					auto p = mScene->CreateSprite();
					p->SetLocalPosition(v->GetPosition());
					p->SetRenderFlags(RF_NO_LIGHTING);
					mDebugWalkableVertices.push_back(p);
				}
			}
		}

		mNavMeshRebuildTimer = 60;
	}
	mNavMeshRebuildTimer--;
}

LocationMap * Location::GetMap() const
{
	return mMap.get();
}

void Location::SetMap(unique_ptr<LocationMap>&& locMap)
{
	mMap = move(locMap);
}

void Location::AddRoom(const shared_ptr<Room> &room)
{
	mRoomList.push_back(room);
}

const vector<shared_ptr<Room>> &Location::GetRoomList() const
{
	return mRoomList;
}

void Location::AddWorkbench(const shared_ptr<Workbench> &workbench)
{
	mWorkbenchList.push_back(workbench);
}

const vector<shared_ptr<Workbench>> &Location::GetWorkbenchList() const
{
	return mWorkbenchList;
}

void Location::AddFogGate(const shared_ptr<FogGate>& fg)
{
	mFogGates.push_back(fg);
}

const vector<shared_ptr<FogGate>> &Location::GetFogGateList() const
{
	return mFogGates;
}

void Location::RemoveFogGate(const shared_ptr<FogGate> &fg)
{
	mFogGates.erase(std::remove(mFogGates.begin(), mFogGates.end(), fg), mFogGates.end());
}

void Location::AddSmoke(const shared_ptr<Smoke> &smoke)
{
	mSmokeList.push_back(smoke);
}

const vector<shared_ptr<Smoke>> &Location::GetSmokeList() const
{
	return mSmokeList;
}

// stack-based flood fill algorithm
// returns count of filled pixels
template <class T>
static int FloodFill(vector<T> &input, int width, int height, sf::Vector2i p, T fillValue, T oldValue)
{
	int filledCount = 0;
	if (p.x >= 0 && p.x < width && p.y >= 0 && p.y < height)
	{
		if (input[p.y * width + p.x] == fillValue || input[p.y * width + p.x] != oldValue)
		{
			return filledCount;
		}
		std::stack<sf::Vector2i> stack;
		static const sf::Vector2i offset[4] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
		while (true)
		{
			input[p.y * width + p.x] = fillValue;
			++filledCount;
			for (int i = 0; i < 4; i++)
			{
				const sf::Vector2i np = p + offset[i];
				if (np.x >= 0 && np.x < width && np.y >= 0 && np.y < height &&
					(input[np.y * width + np.x] == oldValue))
				{
					stack.push(np);
				}
			}
			if (stack.empty())
			{
				break;
			}
			p = stack.top();
			stack.pop();
		}
	}
	return filledCount;
}

const Guid & Location::GetIdentifier() const
{
	return mIdentifier;
}

// This method is fully experimental
// Analyzes circle zone for unstable structures inside terrain
// Algorithm:
// - Select N points inside passed rectangle
// - Do flood fill from each point
//    1) If pixel count exceed max threshold - ignore zone - we are inside solid
//    ground 2) Otherwise - remember this zone
// - Check each zone found by flood fill
//    1) If pixel count inside zone less than some threshold value - break zone
//    into debris pieces 2) Otherwise - use scan line algorithm to find thick
//    lines inside zone, like this:
//       ----------------
//        --------------
//         -----------
//           ---   <<---- weak line - break underlying zone into debris pieces if
//          ------        underlying mass exceed some defined threshold
//         ----------
//       ---------------
//
void Location::AnalyzeAndBreakUnstableZones(const Vec2f & position, const Vec2f & size)
{
	using Zone = vector<sf::Vector2i>;

	const auto terrain = mScene->GetTerrain();

	const uint8_t FullyVisible = 255;

	if (game->Debug.verboseUnstableZoneAnalysisOutput)
	{
		cout << "\nPerforming analysis at (" << position.x << "; " << position.y << "; " << size.x << "; "
			<< size.y << ")" << endl;
	}

	// Step 1: Gather pixels inside defined rectangle
	sf::Vector2i posi = sf::Vector2i(position);
	vector<uint16_t> pixelsForAnalysis;
	vector<sf::Color> colorPixels;
	for (int y = 0; y < size.y; ++y)
	{
		for (int x = 0; x < size.x; ++x)
		{
			auto pix = terrain->GetPixel(posi.x + x, posi.y + y);
			if (pix)
			{
				constexpr uint8_t mask = PA_VISIBLE | PA_EDITABLE;
				if ((pix->a & mask) == mask && !(pix->a & PA_METAL))
				{
					pixelsForAnalysis.push_back(VisiblePixel);
					colorPixels.emplace_back(pix->r, pix->g, pix->b, FullyVisible);
				}
				else
				{
					pixelsForAnalysis.push_back(InvisiblePixel);
					colorPixels.emplace_back(0, 0, 0, 0);
				}
			}
			else
			{
				pixelsForAnalysis.push_back(OutOfBoundsPixel);
				colorPixels.emplace_back(0, 0, 0, 0);
			}
		}
	}

	if (game->Debug.verboseUnstableZoneAnalysisOutput)
	{
		vector<sf::Color> pixels;
		for (int i = 0; i < size.x * size.y; ++i)
		{
			if (pixelsForAnalysis[i] == OutOfBoundsPixel)
			{
				pixels.emplace_back(0, 0, 0);
			}
			else if (pixelsForAnalysis[i] == VisiblePixel)
			{
				pixels.emplace_back(0, 255, 0);
			}
			else
			{
				pixels.emplace_back(255, 0, 255);
			}
		}
		sf::Image img;
		img.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y), &pixels[0].r);
		img.saveToFile("analyze_step_1.png");
		img.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y), &colorPixels[0].r);
		img.saveToFile("analyze_step_1a.png");
	}

	sf::Vector2i sizei = sf::Vector2i(size);
	// Step 2: Do flood fill
	uint16_t zoneNum = ZoneIDOrigin;
	for (int y = 0; y < size.y; ++y)
	{
		for (int x = 0; x < size.x; ++x)
		{
			int filledCount = FloodFill(pixelsForAnalysis, sizei.x, sizei.y, { x, y }, zoneNum,
				static_cast<uint16_t>(VisiblePixel));
			if (filledCount)
			{
				++zoneNum;
				if (game->Debug.verboseUnstableZoneAnalysisOutput)
				{
					cout << "Zone found at (" << position.x + x << "; " << position.y + y << ") with "
						<< filledCount << " pixels" << endl;
				}
			}
		}
	}

	if (game->Debug.verboseUnstableZoneAnalysisOutput)
	{
		vector<sf::Color> pixels;
		for (int i = 0; i < size.x * size.y; ++i)
		{
			if (pixelsForAnalysis[i] == OutOfBoundsPixel)
			{
				pixels.emplace_back(0, 0, 0);
			}
			else if (pixelsForAnalysis[i] == VisiblePixel)
			{
				pixels.emplace_back(0, 255, 0);
			}
			else if (pixelsForAnalysis[i] >= ZoneIDOrigin)
			{
				pixels.emplace_back(pixelsForAnalysis[i] * 20, 0, 0);
			}
			else
			{
				pixels.emplace_back(0, 0, 0);
			}
		}
		sf::Image img;
		img.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y), &pixels[0].r);
		img.saveToFile("analyze_step_2.png");
	}

	if (game->Debug.verboseUnstableZoneAnalysisOutput)
	{
		int zoneCount = zoneNum - ZoneIDOrigin;
		cout << "Total zone count: " << zoneCount << endl;
	}

	// Step 3: Look for 'stalactite-like' structures and 'flying islands' at each
	// zone and break weak zones into debris pieces
	for (int zone = ZoneIDOrigin; zone < zoneNum; ++zone)
	{
		int weakestRow = -1;
		int weakestRowWidth = INT_MAX;
		bool flyingIsland = true;
		sf::Vector2i min = { INT_MAX, INT_MAX };
		sf::Vector2i max = { -INT_MAX, -INT_MAX };
		for (int y = 0; y < size.y; ++y)
		{
			int rowBegin = -1, rowEnd = -1;
			for (int x = 0; x < size.x - 1; ++x)
			{
				int index = static_cast<int>(y * size.x + x);
				auto pixel = pixelsForAnalysis[index];
				auto nextPixel = pixelsForAnalysis[index + 1];
				bool pixelsReferToZone = pixel == zone || nextPixel == zone;
				// compute bounds
				if (pixelsReferToZone)
				{
					if (x < min.x)
					{
						min.x = x;
					}
					if (x > max.x)
					{
						max.x = x;
					}
					if (y < min.y)
					{
						min.y = y;
					}
					if (y > max.y)
					{
						max.y = y;
					}
				}
				bool isBorder = (x == 0) || (x + 1 >= size.x - 1) || (y == 0) ||
					(y >= size.y - 1); // || pixel == OutOfBoundsPixel;
				if (isBorder)
				{
					// zone is 'flying island' only if it's pixels does not intersect
					// borders of passed rect
					if (pixelsReferToZone)
					{
						flyingIsland = false;
					}
				}
				else
				{
					if (pixel != zone && nextPixel == zone && rowBegin < 0)
					{
						rowBegin = x;
					}
					if (pixel == zone && nextPixel != zone && rowBegin >= 0)
					{
						rowEnd = x;
						if (rowEnd - rowBegin < weakestRowWidth)
						{
							weakestRowWidth = rowEnd - rowBegin;
							weakestRow = y;
						}
					}
				}
			}
		}

		if (game->Debug.verboseUnstableZoneAnalysisOutput)
		{
			if (weakestRow >= 0)
			{
				cout << "Zone " << zone << "; Weakest row: " << weakestRow << "; Width: " << weakestRowWidth
					<< "; Flying Island: " << flyingIsland << endl;
				cout << "\t Bounds (" << min.x << "; " << min.y << "); (" << max.x << "; " << max.y << ")"
					<< endl;
			}
		}

		// break 'flying islands' into pieces
		if (flyingIsland)
		{
			if (game->Debug.verboseUnstableZoneAnalysisOutput)
			{
				cout << "Zone " << zone << " is flying island " << endl;
			}

			auto level = game->GetLevel();
			auto &debrisAtlas = level->GetDebrisAtlas();

			// slightly increase bounds, we have to do this because of rounding issues
			// on previous steps
			--min.y;
			--min.x;

			++max.x;
			++max.y;

			for (int y = min.y; y < max.y; y += Debris::Radius * 2)
			{
				for (int x = min.x; x < max.x; x += Debris::Radius * 2)
				{
					// check content of the piece
					// if at least one pixel is visible then create debris piece
					bool addPiece = false;
					for (int i = y; i < y + Debris::Radius * 2; ++i)
					{
						for (int j = x; j < x + Debris::Radius * 2; ++j)
						{
							unsigned int index = static_cast<unsigned int>(i * size.x + j);

							if (index < colorPixels.size())
							{
								if (colorPixels[index].a == FullyVisible)
								{
									addPiece = true;
									break;
								}
							}
						}
					}

					if (addPiece)
					{
						const Vec2f offset = { static_cast<float>(x), static_cast<float>(y) };
						sf::FloatRect texCoords;
						int pieceIndex =
							debrisAtlas->Pack(colorPixels.data(), sizei.x, sizei.y, x, y, texCoords);
						const auto p = position + offset;
						auto debris =
							make_shared<Debris>(shared_from_this(), p, debrisAtlas->GetTexture(), texCoords, pieceIndex);
						AddDebris(debris);

						// cut rect out of terrain
						MaskOperation args;
						args.mBounds = Bounds::Rect;
						args.mPosition = position + offset;
						args.mRectSize = { Debris::Radius * 2, Debris::Radius * 2 };
						args.mFirstPassBitwiseOperation = BitwiseOp::And;
						args.mFirstPassMask = ~(PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE);

						terrain->Mask(args);
					}
				}
			}
		}
	}
}

void Location::BlowSprite(const shared_ptr<Sprite> & sprite, bool castBlood, bool castAcid)
{
	const auto texture = sprite->GetTexture();
	const auto size = sprite->GetSize();
	if (sprite->IsBlowable() && texture)
	{
		// clamp max debris count below 256 pieces per sprite
		const float wCount = Math::Clamp(size.x / (Debris::Radius * 2), 0.0f, 16.0f);
		const float hCount = Math::Clamp(size.y / (Debris::Radius * 2), 0.0f, 16.0f);
		const float dx = 1.0f / wCount;
		const float dy = 1.0f / hCount;
		for (int segY = 0; segY < static_cast<int>(hCount); ++segY)
		{
			for (int segX = 0; segX < static_cast<int>(wCount); ++segX)
			{
				const float nx = segX / wCount;
				const float ny = segY / hCount;
				const Vec2f p = sprite->GetGlobalPosition() + Vec2f(nx * size.x, ny * size.y);
				auto debris = make_shared<Debris>(shared_from_this(), p, texture, sf::FloatRect(nx, ny, dx, dy));
				debris->GetNode()->ResetRenderFlags(RF_NO_LIGHTING);
				if (castBlood)
				{
					debris->EnableBloodCasting(true);
				}
				if (castAcid)
				{
					debris->EnableAcidCasting(true);
				}
				AddDebris(debris);
			}
		}
		sprite->RemoveFromScene();
	}
}

void Location::PropagateDamage(Damage & dmg, Actor * ignoreActor)
{
	if (dmg.mHitbox)
	{
		for (auto &actor : mActorList)
		{
			if (actor.get() != ignoreActor)
			{
				if (actor->IsAnyHitBoxIntersected(*dmg.mHitbox))
				{
					actor->ApplyDamage(dmg);
				}
			}
		}
	}
}

void Location::AddPassage(const shared_ptr<Passage>& passage)
{
	mPassageList.push_back(passage);
}

const vector<shared_ptr<Passage>> & Location::GetPassageList() const
{
	return mPassageList;
}

void Location::AddIsland(const shared_ptr<Island>& island)
{
	mIslandList.push_back(island);
}

const vector<shared_ptr<Island>>& Location::GetIslandList() const
{
	return mIslandList;
}

shared_ptr<LocationDefinition> Location::GetDefinition() const
{
	return mDefinition;
}

