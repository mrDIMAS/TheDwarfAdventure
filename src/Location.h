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

class Game;
class Debris;
class Chest;
class DebrisAtlas;
class Ladder;
class MotherCrystal;
class Remains;
class Quest;
class Item;
class Player;
class PolyLine;
class Scene;
class Text;
class LiquidParticle;
class Projectile;
class Terrain;
class Actor;
class Sprite;
class Texture;
class ParticleSystem;
class Door;
class Creature;
class PassageGenerator;
class CaveGenerator;
class Renderer;
class Boss;
class Workbench;
class Smoke;
class Room;
class FogGate;
class LocationMap;
struct Damage;
class Passage;
class Level;
class Island;
class TonnelVertex;
class Cloud;
class AttributeGrid;
class Arena;

#include "LocationType.h"

enum class LocationClass {
	// Location is fully underground and have no ways to surface
	Underground,
	// Location flying in the skies
	FlyingIsland,
	// Location have way to surface
	Terrestrial,
};

class LocationDefinition
{
public:	
	friend class LocationGenerator;

	LocationDefinition()
	{

	}

	LocationDefinition(const LocationType& type, int width, int height) 
		: mType(type) 
		, mWidth(width)
		, mHeight(height)
	{

	}

	LocationType GetType() const
	{
		return mType;
	}

	int GetHeight() const
	{
		return mHeight;
	}

	int GetWidth() const
	{
		return mWidth;
	}

	Vec2f GetPosition() const
	{
		return mPosition;
	}

	void SetPosition(const Vec2f& pos)
	{
		mPosition = pos;
	}

	void SetBaseLevel(int baseLevel)
	{
		mBaseLevel = baseLevel;
	}

	int GetBaseLevel() const
	{
		return mBaseLevel;
	}

	void LinkTo(const shared_ptr<LocationDefinition>& other)
	{
		mLinked.push_back(other);
	}

	const vector<weak_ptr<LocationDefinition>>& GetLinkedLocations() const
	{
		return mLinked;
	}

	shared_ptr<Location> GetLocation() const
	{
		return mLocation.lock();
	}

	void SetDistanceToPlayerLocation(float distance)
	{
		mDistanceToPlayerLocation = distance;
	}

	float GetDistanceToPlayerLocation() const
	{
		return mDistanceToPlayerLocation;
	}

	void Serialize(Serializer& sav)
	{
		sav & mLocation;
		sav & mHeight;
		sav & mType;
		sav & mPosition;
		sav & mLinked;		
		sav & mBaseLevel;
		sav & mName;
	}

	const string& GetName() const
	{
		return mName;
	}

	void SetName(const string& name)
	{
		mName = name;
	}
private:
	weak_ptr<Location> mLocation;
    LocationType mType { LocationType::Unknown };
	int mWidth { 0 };
	int mHeight { 0 };	
	Vec2f mPosition;
	vector<weak_ptr<LocationDefinition>> mLinked;
	int mBaseLevel { 0 };
	float mDistanceToPlayerLocation { 0 };
	string mName;
};

class Location : public enable_shared_from_this<Location> {
public:

	bool Visited { false };

	Location();
	Location(Level *level, const shared_ptr<LocationDefinition>& definition);
	~Location();

	void AddDebris(const shared_ptr<Debris> &debris);
	void UpdateDebris();

	void AddActor(const shared_ptr<Actor> &actor);
	void RemoveActor(const shared_ptr<Actor> &actor);
	vector<shared_ptr<Actor>> &GetActorList();
	void UpdateActors();

	void AddItem(const shared_ptr<Item> &item);
	void RemoveItem(const shared_ptr<Item> &item);
	vector<shared_ptr<Item>> &GetItemList();
	void UpdateItems();

	void AddProjectile(const shared_ptr<Projectile> & projectile);
	void UpdateProjectiles();

	void AddLiquidParticle(const shared_ptr<LiquidParticle> & liquid);
	void RemoveLiquidParticle(const shared_ptr<LiquidParticle> &p);
	void UpdateLiquid();

	void AddChest(const shared_ptr<Chest> &chest);
	void RemoveChest(const shared_ptr<Chest> &chest);
	vector<shared_ptr<Chest>> &GetChestList();

	void AddLadder(const shared_ptr<Ladder> &ladder);
	void RemoveLadder(const shared_ptr<Ladder> &ladder);
	vector<shared_ptr<Ladder>> &GetLadderList();

	const vector<shared_ptr<MotherCrystal>> &GetMotherCrystalList() const;

	void AddRemains(const shared_ptr<Remains> &remains);
	void RemoveRemains(const shared_ptr<Remains> &remains);
	const vector<shared_ptr<Remains>> &GetRemainsList() const;
	void UpdateRemains();

	void AddDoor(const shared_ptr<Door> &door);
	const vector<shared_ptr<Door>> &GetDoorList() const;
	void UpdateDoors();

	void AddRoom(const shared_ptr<Room> &room);
	const vector<shared_ptr<Room>> &GetRoomList() const;

	void AddWorkbench(const shared_ptr<Workbench> &workbench);
	const vector<shared_ptr<Workbench>> &GetWorkbenchList() const;

	void AddFogGate(const shared_ptr<FogGate> & fg);
	const vector<shared_ptr<FogGate>> &GetFogGateList() const;
	void RemoveFogGate(const shared_ptr<FogGate> &fg);

	void AddSmoke(const shared_ptr<Smoke> &smoke);
	const vector<shared_ptr<Smoke>> &GetSmokeList() const;

	void AddPassage(const shared_ptr<Passage> & passage);
	const vector<shared_ptr<Passage>> & GetPassageList() const;

	void AddIsland(const shared_ptr<Island> & island);
	const vector<shared_ptr<Island>> & GetIslandList() const;

	void AddTonnelVertex(const shared_ptr<TonnelVertex> & tonnelVertex);
	const vector<shared_ptr<TonnelVertex>> & GetTonnelVertices() const;

	void AddArena(const shared_ptr<Arena>& arena);
	const vector<shared_ptr<Arena>>& GetArenaList() const;

	shared_ptr<Scene> &GetScene();

	// spawns floating message 'msg' at 'position' with color 'clr'
	// TODO: fix message location
	void ReportMessage(const string &msg, const Vec2f &position, const sf::Color &clr = { 255, 0, 0 });
	void UpdateMessageMarks();

	void UpdateClouds();

	LocationClass GetClass() const;
	void SetClass(LocationClass locClass);
	void Serialize(Serializer &sav);
	Level *GetLevel() const;

	void Update();

	LocationMap * GetMap() const;
	void SetMap(unique_ptr<LocationMap> && locMap);
	
	const Guid& GetIdentifier() const;
	void AnalyzeAndBreakUnstableZones(const Vec2f &position, const Vec2f &size);
	void BlowSprite(const shared_ptr<Sprite> & sprite, bool castBlood = false, bool castAcid = false);
	// Requires hitbox to be set
	void PropagateDamage(Damage & dmg, Actor* ignoreActor = nullptr);

	shared_ptr<LocationDefinition> GetDefinition() const;


private:
	friend class Level;
	friend class WorldGenerator;
	friend class LocationGenerator;
	// Identifier of location using in scripts
	Guid mIdentifier;
	shared_ptr<Scene> mScene;
	vector<shared_ptr<Actor>> mActorList;
	vector<shared_ptr<Projectile>> mProjectileList;
	vector<shared_ptr<Debris>> mDebrisList;
	vector<shared_ptr<Item>> mItemList;
	vector<shared_ptr<LiquidParticle>> mLiquidParticleList;
	vector<shared_ptr<Chest>> mChestList;
	vector<shared_ptr<Cloud>> mCloudList;
	vector<shared_ptr<Ladder>> mLadderList;
	vector<shared_ptr<MotherCrystal>> mMotherCrystalList;
	vector<shared_ptr<Text>> mReportMessageList;
	vector<shared_ptr<Remains>> mRemains;
	vector<shared_ptr<Door>> mDoorList;
	vector<shared_ptr<Room>> mRoomList;
	vector<shared_ptr<Workbench>> mWorkbenchList;
	vector<shared_ptr<FogGate>> mFogGates;
	vector<shared_ptr<Smoke>> mSmokeList;
	vector<shared_ptr<Passage>> mPassageList;
	vector<shared_ptr<Island>> mIslandList;
	vector<shared_ptr<TonnelVertex>> mTonnelVertices;
	vector<shared_ptr<Arena>> mArenaList;
	unique_ptr<LocationMap> mMap;
	Level *mLevel { nullptr };
	LocationClass mClass;
	int mNavMeshRebuildTimer { 60 };
	vector<shared_ptr<Sprite>> mDebugWalkableVertices;
	shared_ptr<LocationDefinition> mDefinition;
};
