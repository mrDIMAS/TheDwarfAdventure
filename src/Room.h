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

class Door;

enum class RoomType
{
	UndergroundApartment,
	SettlementHouse,
	UndergroundWarehouse,
	Store,
	Forge,
};

class Room
{
public:
	// When player exceed TrespassingLimit, inhabitant will put player away from his room  
	static constexpr int TrespassingLimit = 3;
	// Interval (in frames) between warnings for player
	static constexpr int TrespassingTimeout = 300;
private:
	// Inhabitant will ask player what he wants, when player knocks the door. If player have good
	// reputation, actor will open the door otherwise - he won't
	vector<weak_ptr<Actor>> mInhabitants;
	sf::FloatRect mBounds;
	sf::FloatRect mEntrance;
	vector<sf::FloatRect> mIllegalZones;
	int mTrespassingCount { 0 };
	int mTrespassingTimer { 0 };
	vector<shared_ptr<Door>> mDoors;
	RoomType mType;
public:
	Room();	
	void SetBounds(const sf::FloatRect & bounds);
	sf::FloatRect GetBounds() const;
	void AddDoor(const shared_ptr<Door> & door);
	void Serialize(Serializer &sav);
	bool IsEntranceContainsPoint(const Vec2f & pt);
	const vector<shared_ptr<Door>>  & GetDoors() const;
	bool IsPlayerInsideIllegalZone();
	void AddIllegalZone(const sf::FloatRect & zone);
	const vector<weak_ptr<Actor>>& GetInhabitants() const;
	void AddInhabitant(const shared_ptr<Actor> & inhabitant);
	void EvictInhabitant(const shared_ptr<Actor>& inhabitant);
	void EvictAllInhabitants();
	void SetEntrance(const sf::FloatRect & entrance);
	const vector<shared_ptr<Door>> & GetDoors();
	sf::FloatRect GetEntrance() const;
	void Update();
	bool IsLocked() const;
	void SetType(const RoomType & type);
	RoomType GetType() const;
	bool Intersects(const sf::FloatRect & bounds);
	int GetAliveInhabitantsCount() const;
	shared_ptr<Actor> GetMostPowerfulInhabitant();
};
