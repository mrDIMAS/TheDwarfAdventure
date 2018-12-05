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

#include "Collision.h"
#include "Item.h"

class WeaponRayTraceResult
{
public:
	shared_ptr<Actor> mActor;
	Vec2f mClosest;
};

class Weapon : public Item
{
protected:
	shared_ptr<Sprite> mModel;
	// position relative to model where projectile should be spawned
	Vec2f mShootPoint;
	weak_ptr<Actor> mOwner;
	int mShootTimeout { 0 };
	shared_ptr<Sprite> mCrosshair;
	bool mShowCrosshair { true };
	void AddDebrisInIntersection(const shared_ptr<Cell> &cell, const sf::Vector2i &pixCoord, const Vec2f &pos);
	shared_ptr<Actor> FindActorByIntersectionResult(const ClosestRayCastResult &rt);
	virtual void OnUpdate() = 0;
	virtual void OnSerialize(Serializer &sav) = 0;
	virtual void OnSetVisible(bool vis);

public:
	Weapon();
	Weapon(const shared_ptr<Location> &location, const ItemType &type);
	virtual ~Weapon();
	void Update();
	virtual void Serialize(Serializer &sav) override final;
	shared_ptr<Actor> GetOwner() const;
	void SetOwner(const shared_ptr<Actor> &actor);
	virtual bool Shoot(const Vec2f &direction) = 0;
	void OrientCrosshair(const Vec2f &direction);
	void SetVisible(bool vis);
	virtual shared_ptr<Texture> GetPictogram() override;
	shared_ptr<Item> ExtractConsumable(ItemType type);
	shared_ptr<Sprite> GetModel() const;
	Vec2f GetShootPointGlobal() const;
	shared_ptr<Actor> PickClosestActorByRay(
		const Vec2f &origin, const Vec2f &dir, Vec2f *outIntPos, float *outSqrDist);
};
