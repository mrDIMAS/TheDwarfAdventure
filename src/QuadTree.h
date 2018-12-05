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

// quad tree for terrain

class Cell;
class Terrain;

class Quad
{
private:
	sf::FloatRect mRect;
	unique_ptr<Quad> mChildren[4];
	vector<shared_ptr<Cell>> mCells;
	bool mSplit;

public:
	Quad(const Vec2f &position, const Vec2f &size);
	void SetCells(const vector<shared_ptr<Cell>> &cells);
	bool IsIntersectRect(const Vec2f &position, const Vec2f &size);
	bool IsIntersectCircle(const Vec2f &position, float sqrRadius);
	bool IsPointInside(const Vec2f &p);
	bool IsSplit() const;
	void Split();
	unique_ptr<Quad> &GetChild(int i);
	vector<shared_ptr<Cell>> &GetCells();
	bool IsRayIntersection(const Vec2f &origin, const Vec2f &direction);
};

class QuadTree
{
private:
	static constexpr int MaxCellsPerQuad = 4;
	unique_ptr<Quad> mRoot;
	vector<shared_ptr<Cell>> mTraceBuffer;
	void RecursiveBuild(unique_ptr<Quad> &quad, const vector<shared_ptr<Cell>> &cells);
	void TraceCircleRecursive(unique_ptr<Quad> &quad, const Vec2f &circle, float sqrRadius);
	void TraceRayRecursive(unique_ptr<Quad> &quad, const Vec2f &origin, const Vec2f &direction);

public:
	QuadTree(Terrain *terrain);
	~QuadTree();
	vector<shared_ptr<Cell>> &TraceCircle(const Vec2f &circle, float sqrRadius);
	vector<shared_ptr<Cell>> &TraceRay(const Vec2f &origin, const Vec2f &direction);
};
