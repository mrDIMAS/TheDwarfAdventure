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
#include "QuadTree.h"
#include "Cell.h"
#include "Terrain.h"

void QuadTree::RecursiveBuild(unique_ptr<Quad> &quad, const vector<shared_ptr<Cell>> &cells)
{
	if (cells.size() <= MaxCellsPerQuad)
	{
		quad->SetCells(cells);
	}
	else
	{
		quad->Split();

		for (int i = 0; i < 4; ++i)
		{
			vector<shared_ptr<Cell>> leafCells;
			auto &child = quad->GetChild(i);

			for (auto &cell : cells)
			{
				if (child->IsIntersectRect(cell->GetPosition(),
				{ static_cast<float>(Cell::Size), static_cast<float>(Cell::Size) }))
				{
					leafCells.push_back(cell);
				}
			}

			RecursiveBuild(child, leafCells);
		}
	}
}

void QuadTree::TraceCircleRecursive(unique_ptr<Quad> &quad, const Vec2f &circle, float sqrRadius)
{
	if (quad->IsSplit())
	{
		for (int i = 0; i < 4; ++i)
		{
			TraceCircleRecursive(quad->GetChild(i), circle, sqrRadius);
		}
	}
	else
	{
		if (quad->IsIntersectCircle(circle, sqrRadius))
		{
			auto &cells = quad->GetCells();
			mTraceBuffer.insert(mTraceBuffer.end(), cells.begin(), cells.end());
		}
	}
}

void QuadTree::TraceRayRecursive(
	unique_ptr<Quad> &quad, const Vec2f &origin, const Vec2f &direction)
{
	if (quad->IsSplit())
	{
		for (int i = 0; i < 4; ++i)
		{
			TraceRayRecursive(quad->GetChild(i), origin, direction);
		}
	}
	else
	{
		if (quad->IsRayIntersection(origin, direction))
		{
			auto &cells = quad->GetCells();
			mTraceBuffer.insert(mTraceBuffer.end(), cells.begin(), cells.end());
		}
	}
}

QuadTree::QuadTree(Terrain *terrain)
{
	const Vec2f size { static_cast<float>(terrain->GetWidthPixels()),
		static_cast<float>(terrain->GetHeightPixels()) };
	mRoot = make_unique<Quad>(Vec2f(0, 0), size);
	RecursiveBuild(mRoot, terrain->GetCells());
}

QuadTree::~QuadTree()
{
	DBG_ECHO();
}

vector<shared_ptr<Cell>> &QuadTree::TraceCircle(const Vec2f &circle, float sqrRadius)
{
	mTraceBuffer.clear();
	TraceCircleRecursive(mRoot, circle, sqrRadius);
	return mTraceBuffer;
}

vector<shared_ptr<Cell>> &QuadTree::TraceRay(const Vec2f &origin, const Vec2f &direction)
{
	mTraceBuffer.clear();
	TraceRayRecursive(mRoot, origin, direction);
	return mTraceBuffer;
}

Quad::Quad(const Vec2f &position, const Vec2f &size) : mSplit(false), mRect(position, size)
{
}

void Quad::SetCells(const vector<shared_ptr<Cell>> &cells)
{
	mCells = cells;
}

bool Quad::IsIntersectRect(const Vec2f &position, const Vec2f &size)
{
	return mRect.intersects(sf::FloatRect(position, size));
}

bool Quad::IsIntersectCircle(const Vec2f &position, float sqrRadius)
{
	const auto r = mRect.left + mRect.width;
	const auto b = mRect.top + mRect.height;
	// find the closest point to the circle within the rectangle
	const float closestX = Math::Clamp(position.x, mRect.left, r);
	const float closestY = Math::Clamp(position.y, mRect.top, b);
	// calculate the distance between the circle's center and this closest point
	const float distanceX = position.x - closestX;
	const float distanceY = position.y - closestY;
	// if the distance is less than the circle's radius, an intersection occurs
	const float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
	return distanceSquared < sqrRadius;
}

bool Quad::IsPointInside(const Vec2f &p)
{
	return mRect.contains(p);
}

bool Quad::IsSplit() const
{
	return mSplit;
}

void Quad::Split()
{
	mSplit = true;
	float halfW = mRect.width / 2;
	float halfH = mRect.height / 2;
	auto halfSize = Vec2f(halfW, halfH);
	mChildren[0] = make_unique<Quad>(Vec2f(mRect.left, mRect.top), halfSize);
	mChildren[1] = make_unique<Quad>(Vec2f(mRect.left + halfW, mRect.top), halfSize);
	mChildren[2] = make_unique<Quad>(Vec2f(mRect.left, mRect.top + halfH), halfSize);
	mChildren[3] = make_unique<Quad>(Vec2f(mRect.left + halfW, mRect.top + halfH), halfSize);
}

unique_ptr<Quad> &Quad::GetChild(int i)
{
	return mChildren[i];
}

vector<shared_ptr<Cell>> &Quad::GetCells()
{
	return mCells;
}

bool Quad::IsRayIntersection(const Vec2f &origin, const Vec2f &direction)
{
	Vec2f min(mRect.left, mRect.top);
	Vec2f max(mRect.left + mRect.width, mRect.top + mRect.height);
	float tmin, tmax, tymin, tymax;
	if (direction.x >= 0)
	{
		tmin = (min.x - origin.x) / direction.x;
		tmax = (max.x - origin.x) / direction.x;
	}
	else
	{
		tmin = (max.x - origin.x) / direction.x;
		tmax = (min.x - origin.x) / direction.x;
	}
	if (direction.y >= 0)
	{
		tymin = (min.y - origin.y) / direction.y;
		tymax = (max.y - origin.y) / direction.y;
	}
	else
	{
		tymin = (max.y - origin.y) / direction.y;
		tymax = (min.y - origin.y) / direction.y;
	}
	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	return ((tmin < 1.0f) && (tmax > 0.0f));
}
