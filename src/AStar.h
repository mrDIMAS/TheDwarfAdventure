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

enum PassabilityFlags
{
	// For ground units
	PassabilityFlag_Walk = BIT(0),  
	// For flying units
	PassabilityFlag_Fly = BIT(1),   
	// Internal use only
	PassabilityFlag_Passable = BIT(2), 
};

class GraphVertex
{
protected:
	Vec2f mPosition;
	float mGScore { FLT_MAX };
	float mFScore { FLT_MAX };
	int mPassabilityFlags { 0 };
	vector<GraphVertex*> mNeighbours;
public:
	void * UserData { nullptr };
	GraphVertex();
	GraphVertex(const Vec2f &p);
	void Reset();
	Vec2f GetPosition() const;
	float GetGScore() const;
	float GetFScore() const;
	void SetGScore(float gScore);
	void SetFScore(float fScore);
	int GetFlags() const;
	void SetFlags(int flags);
	void ResetFlags(int flags);
	float SqrDistance(GraphVertex *other);
	void Link(GraphVertex *neighbour);
	void AddNeighbour(GraphVertex *neighbour);
	size_t GetNeighbourCount() const;
	GraphVertex *GetNeighbour(size_t i);
};

enum class PathType
{
	// Path from start to goal is found
	Full,
	// Path from start to nearest graph vertex to the goal is found
	Partial,
	// Path not found
	Empty,
};

class AStarPathfinder
{
private:
	Vec2i mNavmeshSize;
	vector<unique_ptr<GraphVertex>> mVertices;
	float Heuristic(GraphVertex* vertex, GraphVertex* goal);
	void ReconstructPath(unordered_map<GraphVertex*, GraphVertex*> &cameFrom, GraphVertex* current, vector<GraphVertex*> &outPath);
public:
	AStarPathfinder();
	~AStarPathfinder();
	PathType BuildPath(const vector<unique_ptr<GraphVertex>> &vertices, GraphVertex* start, GraphVertex* goal, vector<GraphVertex*> &outPath, int vFlags);
	PathType BuildPath(GraphVertex* start, GraphVertex* goal, vector<Vec2f> &outPath, int vFlags);
	void SetNavmesh(vector<unique_ptr<GraphVertex>> &&v, const Vec2i &size);
	vector<unique_ptr<GraphVertex>> &GetNavmesh();
	Vec2i GetNavmeshSize() const;
	static void RunTests();
};

