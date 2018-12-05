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

class Sprite;
class Scene;
class Item;
class Game;

enum class DoorType
{
	Unknown,
	Internal,    // door inside scene
	ToOtherScene // door to other scene (do not forget to set destination scene)
};

enum class DoorModelType
{
	Side, // For Internal door type
	Front, // Ordinary door
	Portal, // Usually portal to flying islands	
};

enum class DoorState
{
	Unknown,
	Locked,
	Open,
	Closed,
	Opening,
	Closing,
};

class Door
{
private:
	Vec2f mDoorSize;
	Vec2f mBoxSize;
	shared_ptr<Sprite> mBox;
	shared_ptr<Sprite> mDoor;
	DoorType mType;
	DoorState mState;
	weak_ptr<Item> mKey;
	bool mLockOnClose { false };
	weak_ptr<Door> mDestinationDoor; // only for DoorType::ToOtherScene
	void SetOpen(bool open);
	weak_ptr<Location> mLocation;
	DoorModelType mModelType { DoorModelType::Side };
	
public:
	static Vec2f FetchBoxSize(const DoorModelType & type);
	Door();
	Door(const shared_ptr<Location> &location, DoorType type, DoorModelType modelType, const Vec2f &position);
	~Door();
	void SetDestinationDoor(const shared_ptr<Door> &door);
	shared_ptr<Door> GetDestinationDoor() const;
	bool IsIntersect(const Vec2f &position, float radius) const;
	void Serialize(Serializer &sav);
	void SetKey(const weak_ptr<Item> &key);
	weak_ptr<Item> GetKey() const;
	void Update();
	// returns false if key doesnt match
	bool Open(const shared_ptr<Item> &key);
	// returns true if key match
	// if key != nullptr, door will be locked if key match
	bool Close(const shared_ptr<Item> & key);
	void Unlock(const shared_ptr<Item> & key);
	DoorState GetState() const;
	DoorType GetType() const;
	shared_ptr<Scene> GetScene() const;
	Vec2f GetPosition() const;
	void SetSize(const Vec2f &s);
	Vec2f GetSize() const;	
	void SetHeight(float h);	
	float GetHeight() const;
	shared_ptr<Location> GetLocation() const;
	bool IsHasKey() const;
};
