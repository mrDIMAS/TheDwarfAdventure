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
#include "AStar.h"

class Router
{
private:
	Vec2f mGoal;
	Vec2f mStart;
	vector<Vec2f> mPath;
	int mCurrentPathVertex { -1 };
	int mPathRebuildTimer { 0 };
	// since we are storing pointers as id's, we can use raw pointers (cuz there is no dereferencing of them)
	GraphVertex* mLastGoalVertex { nullptr };
	GraphVertex* mLastStartVertex { nullptr };

public:
	Router();
	~Router();
	void Update(const shared_ptr<Terrain> &terrain, int vFlags);
	void SetGoal(const Vec2f &goal);
	void SetStart(const Vec2f &start);
	bool IsPathReady() const;
	Vec2f GetTarget() const;
	void Serialize(Serializer &sav);
	const vector<Vec2f> &GetPath() const;
	bool IsReachGoal() const;
};
