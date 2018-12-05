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
#include "Room.h"
#include "Actor.h"
#include "Door.h"
#include "Level.h"
#include "Player.h"
#include "Dialog.h"
#include "Stats.h"

Room::Room()
{
}

void Room::SetBounds(const sf::FloatRect & bounds)
{
	mBounds = bounds;
}

sf::FloatRect Room::GetBounds() const
{
	return mBounds;
}

void Room::AddDoor(const shared_ptr<Door>& door)
{
	mDoors.push_back(door);
}

const vector<shared_ptr<Door>>& Room::GetDoors()
{
	return mDoors;
}

void Room::Serialize(Serializer & sav)
{
	sav &mBounds;
	sav &mInhabitants;
	sav &mDoors;
	sav &mEntrance;
	sav &mIllegalZones;
	sav &mTrespassingCount;
	sav &mTrespassingTimer;
	sav &mDoors;
	sav &mType;
}

bool Room::IsEntranceContainsPoint(const Vec2f & pt)
{
	return mEntrance.contains(pt);
}

const vector<shared_ptr<Door>> & Room::GetDoors() const
{
	return mDoors;
}

bool Room::IsPlayerInsideIllegalZone()
{
	for (auto & zone : mIllegalZones)
	{
		if (zone.contains(game->GetLevel()->GetPlayer()->GetPosition()))
		{
			return true;
		}
	}
	return false;
}

void Room::AddIllegalZone(const sf::FloatRect & zone)
{
	mIllegalZones.push_back(zone);
}

const vector<weak_ptr<Actor>>& Room::GetInhabitants() const
{
	return mInhabitants;
}

void Room::AddInhabitant(const shared_ptr<Actor>& inhabitant)
{
	mInhabitants.push_back(inhabitant);
}

void Room::EvictInhabitant(const shared_ptr<Actor>& inhabitant)
{
	for (auto iter = mInhabitants.begin(); iter != mInhabitants.end(); ++iter)
	{
		if (!iter->expired())
		{
			if (iter->lock() == inhabitant)
			{
				mInhabitants.erase(iter);

				break;
			}
		}
	}	
}

void Room::EvictAllInhabitants()
{
	mInhabitants.clear();

	mIllegalZones.clear();
}

void Room::SetEntrance(const sf::FloatRect & entrance)
{
	mEntrance = entrance;
}


sf::FloatRect Room::GetEntrance() const
{
	return mEntrance;
}

void Room::Update()
{
	// Remove pointers to dead inhabitants
	for (auto iter = mInhabitants.begin(); iter != mInhabitants.end(); )
	{
		if (iter->expired())
		{
			iter = mInhabitants.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	if (!mInhabitants.empty())
	{
		bool playerRoom = false;

		// Check if this is player room
		for (const weak_ptr<Actor>& weakActorPtr : mInhabitants) 
		{ 
			if (weakActorPtr.lock() == game->GetLevel()->GetPlayer())
			{
				playerRoom = true;

				break;
			}
		}
		
		const shared_ptr<NPC> speaker = dynamic_pointer_cast<NPC>(mInhabitants[0].lock());
		
		if (speaker && !speaker->IsHostile())
		{
			if (IsPlayerInsideIllegalZone() && !playerRoom)
			{
				--mTrespassingTimer;
				if (mTrespassingTimer <= 0)
				{
					mTrespassingTimer = TrespassingTimeout;
					++mTrespassingCount;
					if (mTrespassingCount >= TrespassingLimit)
					{
						mTrespassingCount = 0;

						speaker->BeginDialogOfType<KickOutOfRoomDialog>();
						
						// kick out player from room
						Vec2f kickOutPosition;
						kickOutPosition.x = mEntrance.left + mEntrance.width / 2;
						kickOutPosition.y = mEntrance.top + mEntrance.height / 2;
						game->GetLevel()->GetPlayer()->SetPosition(kickOutPosition);
					}
					else
					{
						speaker->BeginDialogOfType<IllegalZoneDialog>();
					}
				}
			}
		}
	}
}

bool Room::IsLocked() const
{
	bool locked = false;
	for (auto & door : mDoors)
	{
		locked |= door->GetState() == DoorState::Locked;
	}
	return locked;
}

void Room::SetType(const RoomType & type)
{
	mType = type;
}

RoomType Room::GetType() const
{
	return mType;
}

bool Room::Intersects(const sf::FloatRect & bounds)
{
	return mBounds.intersects(bounds) || mEntrance.intersects(bounds);
}

int Room::GetAliveInhabitantsCount() const
{
	int alive = 0;

	for (const weak_ptr<Actor>& weakActorPtr : mInhabitants)
	{
		if (!weakActorPtr.expired())
		{
			const shared_ptr<Actor> actor = weakActorPtr.lock();

			if (!actor->IsDead())
			{
				++alive;
			}
		}
	}

	return alive;
}

shared_ptr<Actor> Room::GetMostPowerfulInhabitant()
{
	shared_ptr<Actor> mostPowerfulActor;

	for (const weak_ptr<Actor>& weakActorPtr : mInhabitants)
	{
		const shared_ptr<Actor>& actor = weakActorPtr.lock();

		if (actor)
		{
			if (mostPowerfulActor)
			{
				if (actor->GetStats()->GetLevel() > mostPowerfulActor->GetStats()->GetLevel())
				{
					mostPowerfulActor = actor;
				}
			}
			else
			{
				mostPowerfulActor = actor;
			}
		}
	}

	return mostPowerfulActor;
}

