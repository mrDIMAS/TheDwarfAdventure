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
#include "Weapon.h"
#include "Level.h"
#include "Debris.h"
#include "Renderer.h"
#include "Collision.h"
#include "Input.h"
#include "DebrisAtlas.h"
#include "Actor.h"
#include "Terrain.h"
#include "Sprite.h"
#include "Sound.h"
#include "Inventory.h"

void Weapon::AddDebrisInIntersection(const shared_ptr<Cell> &cell, const sf::Vector2i &pixCoord, const Vec2f &pos)
{
	THROW_IF_EXPIRED(mOwner);
	sf::FloatRect texCoords;
	auto location = mOwner.lock()->GetLocation();
	auto &debrisAtlas = location->GetLevel()->GetDebrisAtlas();
	static array<sf::Color, Cell::PixelCount> tempBuffer;
	cell->TransferPixelsFromGridToLinearOrder(tempBuffer); // kinda slow to do this op for every piece
	int pieceIndex = debrisAtlas->Pack(tempBuffer.data(), Cell::Size, Cell::Size, pixCoord.x, pixCoord.y, texCoords);
	auto debris = make_shared<Debris>(location, pos, debrisAtlas->GetTexture(), texCoords, pieceIndex);
	debris->GetNode()->GetBody()->AddForce(Math::Normalized(pos - mModel->GetGlobalPosition()) * 2.5f);
	location->AddDebris(debris);
}

shared_ptr<Actor> Weapon::FindActorByIntersectionResult(const ClosestRayCastResult &rt)
{
	THROW_IF_EXPIRED(mOwner);
	if (rt.mIntersection)
	{
		auto owner = mOwner.lock();
		for (auto act : owner->GetLocation()->GetActorList())
		{
			if (act->GetBody() == rt.mBody)
			{
				return act;
			}
		}
	}
	return nullptr;
}

void Weapon::OnSetVisible(bool vis)
{
	UNUSED(vis);
}

Weapon::Weapon()
{
}

Weapon::Weapon(const shared_ptr<Location> &location, const ItemType &type) : Item(location, type)
{
	mCrosshair = location->GetScene()->CreateSprite();
	mCrosshair->SetRenderFlags(RF_NO_LIGHTING);
	mCrosshair->SetVisible(false);
}

Weapon::~Weapon()
{
	if (mCrosshair)
	{
		mCrosshair->RemoveFromScene();
	}

	DBG_ECHO();
}

void Weapon::Update()
{
	--mShootTimeout;
	OnUpdate();
}

void Weapon::Serialize(Serializer &sav)
{
	Item::Serialize(sav);
	sav &mModel;
	sav &mOwner;
	sav &mShootPoint;
	sav &mShootTimeout;
	sav &mCrosshair;
	sav &mShowCrosshair;
	OnSerialize(sav);
}

shared_ptr<Actor> Weapon::GetOwner() const
{
	return mOwner.lock();
}

void Weapon::SetOwner(const shared_ptr<Actor> &actor)
{
	mOwner = actor;
}

void Weapon::OrientCrosshair(const Vec2f &direction)
{
	if (mShowCrosshair)
	{
		mModel->GetScene()->AcceptNode(mCrosshair);
		const Vec2f origin = GetShootPointGlobal();
		auto rt = mModel->GetScene()->GetTerrain()->TraceRay(origin, 4000.0f * direction);
		if (rt.mIntersection)
		{
			float len = Math::Length(rt.mPosition - origin);
			mCrosshair->SetSize({ len, 1 });
		}
		else
		{
			mCrosshair->SetSize({ 2000, 1 });
		}

		mCrosshair->SetLocalPosition(origin);
		mCrosshair->SetLocalRotation(Math::CartesianToPolar(direction).mAngle);

		mCrosshair->SetColor({ 255, 255, 255, 70 });
	}
	mCrosshair->SetVisible(mShowCrosshair);
}

void Weapon::SetVisible(bool vis)
{
	mNode->SetVisible(vis);
	mCrosshair->SetVisible(vis);
	OnSetVisible(vis);
}

shared_ptr<Texture> Weapon::GetPictogram()
{
	return mModel->GetTexture();
}

shared_ptr<Item> Weapon::ExtractConsumable(ItemType type)
{
	THROW_IF_EXPIRED(mOwner);
	auto owner = mOwner.lock();
	// get prepared weapon consumable and check it
	auto holdingHand = owner->GetHoldingHand(shared_from_this());
	auto consumable = owner->GetHandEquipmentConsumable(holdingHand);
	auto inventory = owner->GetInventory();
	if (inventory)
	{
		if (!consumable || consumable->GetType() != type || consumable->IsBroken())
		{
			// replace consumable to correct type
			consumable = owner->GetInventory()->ExtractConsumable(type);
			owner->UseItem(consumable, holdingHand);
		}
		else
		{
			// extract new consumable and prepare it for next call
			auto nextConsumable = owner->GetInventory()->ExtractConsumable(type);

			if (nextConsumable)
			{
				owner->UseItem(nextConsumable, holdingHand);
			}
			else
			{
				owner->ResetHandEquipmentConsumable(holdingHand);
			}
		}
	}
	return consumable;
}

shared_ptr<Sprite> Weapon::GetModel() const
{
	return mModel;
}

Vec2f Weapon::GetShootPointGlobal() const
{
	return mModel->CalculateTransforms().transformPoint(mShootPoint);
}

shared_ptr<Actor> Weapon::PickClosestActorByRay(const Vec2f &origin, const Vec2f &dir, Vec2f *outIntPos, float *outSqrDist)
{
	THROW_IF_EXPIRED(mOwner);

	auto owner = mOwner.lock();
	auto location = owner->GetLocation();
	auto scene = mModel->GetScene();

	// pick actor by ray
	shared_ptr<Actor> closestActor;
	float sqrClosestDistance = FLT_MAX;

	Vec2f closest = { FLT_MAX, FLT_MAX };
	for (auto &actor : location->GetActorList())
	{
		if (actor.get() != owner.get())
		{
			for (auto &hb : actor->GetHitBoxes())
			{
				Vec2f p;
				if (hb.IsIntersects(origin, origin + dir, &p))
				{
					float sqrDist = Math::SqrDistance(origin, p);
					if (sqrDist < sqrClosestDistance)
					{
						closestActor = actor;
						sqrClosestDistance = sqrDist;
						closest = p;
					}
				}
			}
		}
	}

	if (outIntPos)
	{
		*outIntPos = closest;
	}

	if (outSqrDist)
	{
		*outSqrDist = sqrClosestDistance;
	}

	return closestActor;
}
