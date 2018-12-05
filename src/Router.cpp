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
#include "Router.h"
#include "Terrain.h"

Router::Router()
{
}

Router::~Router()
{
}

void Router::Update(const shared_ptr<Terrain> &terrain, int vFlags)
{
	// rebuild path if needed
	if (mPathRebuildTimer <= 0)
	{
		auto start = terrain->GetNearestPathVertex(mStart, vFlags);
		auto goal = terrain->GetNearestPathVertex(mGoal, vFlags);
		// rebuild path only if any vertex has changed
		if (mLastStartVertex != start || mLastGoalVertex != goal)
		{
			auto type = terrain->GetPathfinder().BuildPath(start, goal, mPath, vFlags);
			if (type != PathType::Empty)
			{
				// reset vertex index
				mCurrentPathVertex = 0;
			}
			else
			{
				// no path was found
				mCurrentPathVertex = -1;
			}
			mLastGoalVertex = goal;
			mLastStartVertex = start;
		}
		// randomize inter
		mPathRebuildTimer = Math::Rnd(20, 30);
	}
	--mPathRebuildTimer;

	// switch current vertex when controllable actor is close enough to it
	constexpr float sqrDistanceThreshold = 512.0f; // in sqr pixels
	if (mCurrentPathVertex >= 0 && mCurrentPathVertex < static_cast<int>(mPath.size()))
	{
		if (Math::SqrDistance(mPath[mCurrentPathVertex], mStart) < sqrDistanceThreshold)
		{
			if (mCurrentPathVertex < static_cast<int>(mPath.size()) - 1)
			{
				++mCurrentPathVertex;
			}
		}
	}
}

void Router::SetGoal(const Vec2f &goal)
{
	mGoal = goal;
}

void Router::SetStart(const Vec2f &start)
{
	mStart = start;
}

bool Router::IsPathReady() const
{
	return mCurrentPathVertex >= 0;
}

Vec2f Router::GetTarget() const
{
	if (mCurrentPathVertex >= 0)
	{
		return mPath[mCurrentPathVertex];
	}
	return { 0, 0 };
}

void Router::Serialize(Serializer &sav)
{
	sav &mGoal;
	sav &mStart;
	sav &mPath;
	sav &mCurrentPathVertex;
	sav &mPathRebuildTimer;
}

const vector<Vec2f> &Router::GetPath() const
{
	return mPath;
}

bool Router::IsReachGoal() const
{
	return mCurrentPathVertex == static_cast<int>(mPath.size()) - 1;
}
