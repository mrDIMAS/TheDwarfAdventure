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

class Scene;
class Sprite;
class Sound;
class Actor;

#include "ItemDatabase.h"

enum ProjectileFlags
{
	// projectile will explode on impact
	PRF_EXPLOSIVE = BIT(0),
	// projectile will fly on straight line
	PRF_FLYING = BIT(1),
	// projectile will die impacted terrain,
	// otherwise it die only when lifetime < 0
	PRF_DIE_ON_TERRAIN_IMPACT = BIT(2),
	// projectiles's model will be attached on impact point
	PRF_ATTACH_MODEL_ON_IMPACT_POINT = BIT(3),
	// (any valid intersected node)
	PRF_IMPACT_DAMAGE = BIT(4),
};

class Projectile : public enable_shared_from_this<Projectile>
{
private:
	shared_ptr<Sprite> mModel;
	int mLifeTime { 120 };
	ItemType mType { "Unknown" };
	int mFlags { 0 };
	float mSpeed { 0 };
	weak_ptr<Actor> mOwner;
	bool mDead { false };
	float mDamageRadius { 0 };
	// damage mod derived from weapon
	float mDamageMod { 0 };
	shared_ptr<Sound> mIdleSound;
	void Explode();
	Vec2f mDirection;
	int mSmokeTimer { 0 };
	weak_ptr<Location> mLocation;
public:
	Projectile();
	Projectile(const shared_ptr<Location> &location, const shared_ptr<Actor> &owner, ItemType type, const Vec2f &position,
		const Vec2f &velocity, float damageMod);
	~Projectile();
	void Update();
	bool IsDead() const;
	void Serialize(Serializer &sav);
};
