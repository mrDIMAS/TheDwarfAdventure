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

class Sprite;
class Scene;

class LiquidParticle
{
public:
	enum class Type
	{
		Unknown,
		Water,
		Blood, // ULTRA VIOLENCE! >:-D
		Acid   // leaves trace on map, which doing damage when some actor touches it
	};

private:
	Type mType;
	shared_ptr<Sprite> mModel;
	int mLifeTime;
	int mPaintTime;
	bool mContactedWithTerrain;
	Vec2f mLastPosition;
	Vec2f mVelocity;
	weak_ptr<Location> mLocation;
public:
	LiquidParticle();
	LiquidParticle(const shared_ptr<Location> &location, const Type &type, const Vec2f &position, const Vec2f &velocity);
	~LiquidParticle();
	void Update();
	bool IsDead() const;
	void SetVelocity(const Vec2f &vel);
};
