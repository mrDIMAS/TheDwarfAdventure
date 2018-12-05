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
#include "AStar.h"
#include <unordered_set>
#include "Terrain.h"

float AStarPathfinder::Heuristic(GraphVertex * vertex, GraphVertex * goal)
{
	return Math::SqrDistance(vertex->GetPosition(), goal->GetPosition());
}

void AStarPathfinder::ReconstructPath(unordered_map<GraphVertex*, GraphVertex*>& cameFrom, GraphVertex * current, vector<GraphVertex*>& outPath)
{
	outPath.clear();
	outPath.push_back(current);

	auto cur = current;
	while (cameFrom.find(cur) != cameFrom.end())
	{
		cur = cameFrom[cur];
		outPath.push_back(cur);
	}

	reverse(outPath.begin(), outPath.end());
}

AStarPathfinder::AStarPathfinder()
{

}

AStarPathfinder::~AStarPathfinder()
{

}

PathType AStarPathfinder::BuildPath(const vector<unique_ptr<GraphVertex>>& vertices, GraphVertex * start, GraphVertex * goal, vector<GraphVertex*>& outPath, int vFlags)
{
	if (!start || !goal || vertices.empty())
	{
		return PathType::Empty;
	}

	for (auto &v : vertices)
	{
		v->Reset();
	}

	unordered_set<GraphVertex*> openSet, closedSet;
	openSet.insert(start);

	start->SetGScore(0);
	start->SetFScore(Heuristic(start, goal));

	unordered_map<GraphVertex*, GraphVertex*> cameFrom;

	while (!openSet.empty())
	{
		GraphVertex* current = *openSet.begin();
		for (auto &v : openSet)
		{
			if (v->GetFScore() < current->GetFScore())
			{
				current = v;
			}
		}

		if (current == goal)
		{
			ReconstructPath(cameFrom, current, outPath);

			return PathType::Full;
		}

		openSet.erase(current);
		closedSet.insert(current);

		for (size_t i = 0; i < current->GetNeighbourCount(); ++i)
		{
			auto neighbour = current->GetNeighbour(i);

			if (vFlags)
			{
				if (!(neighbour->GetFlags() & vFlags))
				{
					continue;
				}
			}

			if (closedSet.find(neighbour) != closedSet.end())
			{
				continue;
			}

			if (openSet.find(neighbour) == openSet.end())
			{
				openSet.insert(neighbour);
			}

			float gScore = current->GetGScore() + current->SqrDistance(neighbour);

			if (gScore >= neighbour->GetGScore())
			{
				continue;
			}

			cameFrom[neighbour] = current;
			neighbour->SetGScore(gScore);
			neighbour->SetFScore(gScore + Heuristic(neighbour, goal));
		}
	}

	auto current = vertices[0].get();
	for (auto &v : vertices)
	{
		if (v->GetFScore() < current->GetFScore())
		{
			current = v.get();
		}
	}

	ReconstructPath(cameFrom, current, outPath);

	return outPath.empty() ? PathType::Empty : PathType::Partial;
}

PathType AStarPathfinder::BuildPath(GraphVertex * start, GraphVertex * goal, vector<Vec2f>& outPath, int vFlags)
{
	static vector<GraphVertex*> vPath;
	outPath.clear();
	const auto result = BuildPath(mVertices, start, goal, vPath, vFlags);
	for (auto &v : vPath)
	{
		outPath.push_back(v->GetPosition());
	}
	return result;
}

void AStarPathfinder::SetNavmesh(vector<unique_ptr<GraphVertex>>&& v, const Vec2i & size)
{
	mNavmeshSize = size;
	mVertices = move(v);
}

vector<unique_ptr<GraphVertex>>& AStarPathfinder::GetNavmesh()
{
	return mVertices;
}

Vec2i AStarPathfinder::GetNavmeshSize() const
{
	return mNavmeshSize;
}

void AStarPathfinder::RunTests()
{
	AStarPathfinder as;
	vector<unique_ptr<GraphVertex>> verts;

	int size = 40;

	for (float y = 0; y < size; ++y)
	{
		for (float x = 0; x < size; ++x)
		{
			verts.push_back(make_unique<GraphVertex>(Vec2f(x, y)));
		}
	}

	for (int y = 0; y < size - 1; ++y)
	{
		for (int x = 0; x < size - 1; ++x)
		{
			verts[y * size + x]->Link(verts[y * size + x + 1].get());
			verts[y * size + x]->Link(verts[(y + 1) * size + x].get());
		}
	}

	for (int i = 0; i < 1000; ++i)
	{
		int sx = Math::Rnd(0, size - 1);
		int sy = Math::Rnd(0, size - 1);

		int gx = Math::Rnd(0, size - 1);
		int gy = Math::Rnd(0, size - 1);

		vector<Vec2f> outPath;
		const auto from = verts[sy * size + sx].get();
		const auto to = verts[gy * size + gx].get();
		auto success = as.BuildPath(from, to, outPath, PassabilityFlag_Walk | PassabilityFlag_Fly);

		cout << "Path is " << (success == PathType::Full ? "found " : "not found ") << "from (" << sx << "; " << sy << ") to (" << gx << "; " << gy << ")" << endl;
	}
}

GraphVertex::GraphVertex()
{
}

GraphVertex::GraphVertex(const Vec2f & p) : mPosition(p)
{
}

void GraphVertex::Reset()
{
	mGScore = FLT_MAX;
	mFScore = FLT_MAX;
}

Vec2f GraphVertex::GetPosition() const
{
	return mPosition;
}

float GraphVertex::GetGScore() const
{
	return mGScore;
}

float GraphVertex::GetFScore() const
{
	return mFScore;
}

void GraphVertex::SetGScore(float gScore)
{
	mGScore = gScore;
}

void GraphVertex::SetFScore(float fScore)
{
	mFScore = fScore;
}

int GraphVertex::GetFlags() const
{
	return mPassabilityFlags;
}

void GraphVertex::SetFlags(int flags)
{
	mPassabilityFlags |= flags;
}

void GraphVertex::ResetFlags(int flags)
{
	mPassabilityFlags &= ~flags;
}

float GraphVertex::SqrDistance(GraphVertex * other)
{
	return Math::SqrDistance(mPosition, other->mPosition);
}

void GraphVertex::Link(GraphVertex * neighbour)
{
	AddNeighbour(neighbour);
	neighbour->AddNeighbour(this);
}

void GraphVertex::AddNeighbour(GraphVertex * neighbour)
{
	mNeighbours.push_back(neighbour);
}

size_t GraphVertex::GetNeighbourCount() const
{
	return mNeighbours.size();
}

GraphVertex * GraphVertex::GetNeighbour(size_t i)
{
	return mNeighbours[i];
}
