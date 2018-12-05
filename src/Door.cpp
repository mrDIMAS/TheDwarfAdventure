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
#include "Door.h"
#include "Sprite.h"
#include "Item.h"
#include "Terrain.h"
#include "Sound.h"
#include "Renderer.h"
#include "Location.h"

void Door::SetOpen(bool open)
{
	MaskOperation op;
	op.mEditorAccess = true;
	op.mPosition = mBox->GetGlobalPosition();
	op.mRectSize = { 10, mBox->GetSize().y };
	op.mBounds = Bounds::Rect;
	if (open)
	{
		op.mFirstPassBitwiseOperation = BitwiseOp::And;
		op.mFirstPassMask = ~(PA_COLLIDABLE | PA_RAYTRACEABLE);
	}
	else
	{
		op.mFirstPassBitwiseOperation = BitwiseOp::Or;
		op.mFirstPassMask = PA_COLLIDABLE | PA_RAYTRACEABLE;
		op.mSecondPassBitwiseOperation = BitwiseOp::And;
		op.mSecondPassMask = ~PA_EDITABLE;
	}
	mDoor->GetScene()->GetTerrain()->Mask(op);
}

Vec2f Door::FetchBoxSize(const DoorModelType & type)
{
	switch (type)
	{
	case DoorModelType::Front:
		return { 30, 50 };
	case DoorModelType::Portal:
		return { 70, 110 };
	case DoorModelType::Side:
		return { 5, 50 };
	}

	return { 0, 0 };
}

Door::Door() : mType(DoorType::Unknown), mState(DoorState::Closed), mLockOnClose(false)
{
}

Door::Door(const shared_ptr<Location> &location, DoorType type, DoorModelType modelType, const Vec2f &position)
	: mType(type), mState(DoorState::Closed), mLocation(location), mModelType(modelType)
{
	const auto scene = location->GetScene();
	mBox = scene->CreateSprite();
	mDoor = scene->CreateSprite();
	mDoor->AttachTo(mBox);

	switch (modelType)
	{
	case DoorModelType::Front:		
		mBox->SetTexture(game->GetRenderer()->RequestTexture("data/textures/objects/door_door.png"));
		mDoor->SetVisible(false);
		break;
	case DoorModelType::Portal:		
		mBox->SetTexture(game->GetRenderer()->RequestTexture("data/textures/objects/portal.png"));
		mDoor->SetVisible(false);
		break;
	case DoorModelType::Side:	
		mBox->SetTexture(game->GetRenderer()->RequestTexture("data/textures/objects/door_side.png"));	
		mDoorSize = { 30, 50 };
		mDoor->SetTexture(game->GetRenderer()->RequestTexture("data/textures/objects/door_door.png"));
		break;
	}
	mBoxSize = FetchBoxSize(modelType);

	mBox->SetSize(mBoxSize);
	mDoor->SetSize(mDoorSize);
	mBox->SetLocalPosition(position);
	if (type == DoorType::Internal)
	{
		SetOpen(false);
		mDoor->SetLocalScale(0.1f, 1);
	}
}

Door::~Door()
{
	mBox->RemoveFromScene();
}

void Door::SetDestinationDoor(const shared_ptr<Door> &door)
{
	if (door->mBox->GetScene() == mBox->GetScene())
	{
		throw Exception("Unable to set destination door of the door with same scene.");
	}
	mDestinationDoor = door;
}

shared_ptr<Door> Door::GetDestinationDoor() const
{
	return mDestinationDoor.lock();
}

bool Door::IsIntersect(const Vec2f &position, float radius) const
{
	bool result;
	if (mType == DoorType::ToOtherScene)
	{
		result = Math::IsCircleRectIntersection(
			mBox->GetGlobalPosition(), mBox->GetSize(), position, radius) ||
			Math::IsCircleRectIntersection(
				mDoor->GetGlobalPosition(), mDoor->GetSize(), position, radius);
	}
	else
	{
		result = Math::IsCircleRectIntersection(mBox->GetGlobalPosition() - Vec2f(20, 20),
			mBox->GetSize() + Vec2f(40, 40), position, radius);
	}
	return result;
}

void Door::Serialize(Serializer &sav)
{
	sav &mType;
	sav &mBox;
	sav &mDoor;
	sav &mDestinationDoor;
	sav &mState;
	sav &mKey;
	sav &mLockOnClose;
	sav &mLocation;
	sav &mModelType;
	sav &mDoorSize;
	sav &mBoxSize;
}

void Door::SetKey(const weak_ptr<Item> &key)
{
	mKey = key;
}

weak_ptr<Item> Door::GetKey() const
{
	return mKey;
}

void Door::Update()
{
	if (mState == DoorState::Closing)
	{
		mDoor->SetLocalScale(mDoor->GetLocalScale().x - 0.05f, 1.0f);
		if (mDoor->GetLocalScale().x < 0.1f)
		{
			mDoor->SetLocalScale(0.1f, 1.0f);
			if (mLockOnClose)
			{
				mState = DoorState::Locked;
			}
			else
			{
				mState = DoorState::Closed;
			}
			SetOpen(false);
			mDoor->GetScene()->EmitSound("data/sounds/wooden_door_close.ogg", mDoor);
		}
	}
	else if (mState == DoorState::Opening)
	{
		mDoor->SetLocalScale(mDoor->GetLocalScale().x + 0.05f, 1.0f);
		if (mDoor->GetLocalScale().x > 1.0f)
		{
			mDoor->SetLocalScale(1, 1);
			mState = DoorState::Open;
			SetOpen(true);
		}
	}
}

// returns false if key doesnt match
bool Door::Open(const shared_ptr<Item> &key)
{
	if (mState == DoorState::Open || mState == DoorState::Opening)
	{
		return true;
	}

	if (mState == DoorState::Locked && mKey.expired())
	{
		return false;
	}

	// check key if it set to the door
	if (!mKey.expired() && mState == DoorState::Locked)
	{
		if (mKey.lock() != key)
		{
			mState = DoorState::Closed;
			return false;
		}
	}
	if (mState == DoorState::Closed || mState == DoorState::Closing)
	{
		mState = DoorState::Opening;
	}

	mDoor->GetScene()->EmitSound("data/sounds/wooden_door_open.ogg", mDoor);

	return true;
}

bool Door::Close(const shared_ptr<Item> & key)
{
	bool keyMatch = key ? key == mKey.lock() : false;
	if (mState == DoorState::Closed || mState == DoorState::Closing)
	{
		return keyMatch;
	}
	if (mState == DoorState::Open || mState == DoorState::Opening)
	{
		mState = DoorState::Closing;
	}
	mLockOnClose = keyMatch;
	return keyMatch;
}

void Door::Unlock(const shared_ptr<Item> & key)
{
	if (mState == DoorState::Locked)
	{
		if (mKey.lock() == key)
		{
			mState = DoorState::Closed;
		}
	}
}

DoorState Door::GetState() const
{
	return mState;
}

DoorType Door::GetType() const
{
	return mType;
}

shared_ptr<Scene> Door::GetScene() const
{
	return mBox->GetScene();
}

Vec2f Door::GetPosition() const
{
	return mBox->GetLocalPosition();
}

void Door::SetSize(const Vec2f &s)
{
	mDoor->SetSize(mDoorSize.x, s.y);
	mBox->SetSize(mBoxSize.x, s.y);
}

Vec2f Door::GetSize() const
{
	return mDoor->GetSize();
}

void Door::SetHeight(float h)
{
	SetSize({ GetSize().x, h });
}

float Door::GetHeight() const
{
	return GetSize().y;
}

shared_ptr<Location> Door::GetLocation() const
{
	return mLocation.lock();
}

bool Door::IsHasKey() const
{
	return !mKey.expired();
}
