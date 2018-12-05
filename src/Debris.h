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

#include "Sprite.h"

class Location;

// represents small piece of something. have a physical body
// when life time is over, merges with terrain and deletes
class Debris
{
private:
	weak_ptr<Location> mLocation;
	shared_ptr<Sprite> mNode;
	int mLifeTime { 60 }; // in frames
	int mAtlasPieceId { -1 };
	int mLiquidCastingInterval { 0 };
	bool mMerged { false };
	bool mCastBlood { false };
	bool mCastAcid { false };

public:
	static constexpr int LiquidCastingTime = 10; // in frames
	static constexpr int Radius = 2;
	Debris();
	~Debris();
	Debris(const shared_ptr<Location> &location, const Vec2f &position, const shared_ptr<Texture> &texture,
		const sf::FloatRect &textureRect, int atlasPieceId = -1);
	void Update();
	bool IsDead() const;
	void SetLocation(const shared_ptr<Location> &location);
	void Serialize(Serializer &sav);
	shared_ptr<Sprite> GetNode();
	shared_ptr<Body> GetBody() const;
	void EnableBloodCasting(bool state);
	void EnableAcidCasting(bool state);
	void SetVelocity(const Vec2f &force);
};
