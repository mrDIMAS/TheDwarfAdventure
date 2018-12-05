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
#include "Terrain.h"
#include "Level.h"
#include "Game.h"
#include "Renderer.h"
#include "Scene.h"
#include "DebrisAtlas.h"
#include "TextureStack.h"
#include "FogOfWar.h"
#include "AttributeGrid.h"

Cell * Terrain::RemapWorldCoordinatesToCell(int & x, int & y)
{
	if (x < 0 || y < 0)
	{
		return nullptr;
	}
	int j = static_cast<int>(x * Cell::InvSize);
	int i = static_cast<int>(y * Cell::InvSize);
	if (j < 0 || j >= mWidthCells || i < 0 || i >= mHeightCells)
	{
		return nullptr;
	}
	x -= j * Cell::Size;
	y -= i * Cell::Size;
	return mCells[i * mWidthCells + j].get();
}

void Terrain::Smooth()
{
	const int8_t wall = PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE | PA_EDITABLE;
	const int8_t empty = PA_EDITABLE;

	const int w = GetWidthPixels();
	const int h = GetHeightPixels();

	vector<uint8_t> alphas;
	ExtractAndConvertMaskLayer(alphas);

	uint8_t *rawAlphas = &alphas[0];

	// do blur
	const int kernelVolume = SmoothKernelSize * SmoothKernelSize;
	const int lw = w - SmoothKernelSize;
	const int lh = h - SmoothKernelSize;
	for (int y = SmoothKernelSize; y < lh; ++y)
	{
		for (int x = SmoothKernelSize; x < lw; ++x)
		{
			const uint8_t *px = rawAlphas + x;
			unsigned int sum = 0;
			for (int dy = -SmoothKernelSize; dy <= SmoothKernelSize; ++dy)
			{
				const uint8_t *py = px + (y + dy) * w;
				for (int dx = -SmoothKernelSize; dx <= SmoothKernelSize; ++dx)
				{
					sum += *(py + dx);
				}
			}

			// apply contrast filter
			const unsigned int average = sum / kernelVolume;

			// write back
			auto pix = GetPixel(x, y);
			if (pix)
			{
				pix->a = (average > ContrastFilterThreshold) ? wall : empty;
			}
		}
	}
}

// returns terrain represented as bitmap, if terrain's pixel visible, out value
// is 255, 0 - otherwise
// this method is for internal purposes
void Terrain::ExtractAndConvertMaskLayer(vector<uint8_t> &out)
{
	out.clear(); // dumb protection
	const int w = GetWidthPixels();
	const int h = GetHeightPixels();
	// extract alpha values contains pixel properties
	// and convert it to "color" alpha using 'Visible' flag
	out.reserve(w * h);
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			auto pixel = GetPixel(x, y);
			if (pixel->a & PA_VISIBLE)
			{
				out.emplace_back(255);
			}
			else
			{
				out.emplace_back(0);
			}
		}
	}
}

Terrain::Terrain()
{
}

Terrain::Terrain(const shared_ptr<Scene> &scene, int widthCells, int heightCells, int backStackEntrySize)
	: Terrain()
{
	mScene = scene;

	mWidthCells = widthCells;
	mHeightCells = heightCells;

	for (int i = 0; i < mHeightCells; ++i)
	{
		for (int j = 0; j < mWidthCells; ++j)
		{
			const shared_ptr<Cell> cell = make_shared<Cell>(Vec2f(j * Cell::SizeF, i * Cell::SizeF));
			cell->SetIndex(i * mWidthCells + j);
			mCells.push_back(cell);
		}
	}

	mQuadTree = make_unique<QuadTree>(this);

	// create fog of war
	const float k = 25; // 100.0f;
	const int fogWidth = static_cast<int>((widthCells * Cell::Size) / k);
	const int fogHeight = static_cast<int>((heightCells * Cell::Size) / k);

	mFogOfWar = make_unique<FogOfWar>(Vec2i(fogWidth, fogHeight));

	mBackgroundTextureStack = make_shared<TextureStack>(backStackEntrySize, backStackEntrySize);

	mAttributeGrid = make_unique<AttributeGrid>(fogWidth, fogHeight);
}

Terrain::~Terrain()
{
	DBG_ECHO();
}

shared_ptr<Scene> Terrain::GetScene() const
{
	return mScene.lock();
}

void Terrain::MeltCicle(
	const Vec2f &position, float radius, const sf::Color &borderColor, float borderSize)
{
	MaskOperation args;
	args.mBorderColor = borderColor;
	args.mBorderThickness = borderSize + 1; // HAAAAX
	args.mBounds = Bounds::Circle;
	args.mFirstPassBitwiseOperation = BitwiseOp::And;
	args.mFirstPassMask = ~(PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE | PA_LAVA);
	args.mBorderMask = PA_LAVA;
	args.mBorderBitwiseOperation = BitwiseOp::Or;
	args.mRadius = radius;
	args.mPosition = position;
	args.mMeltMetalPixels = true;

	Mask(args);
}

int Terrain::GetWidthPixels() const
{
	return mWidthCells * Cell::Size;
}

int Terrain::GetHeightPixels() const
{
	return mHeightCells * Cell::Size;
}
 
Cell *Terrain::GetAt(int x, int y)
{
	if (x < 0 || x >= mWidthCells || y < 0 || y >= mHeightCells)
	{
		return nullptr;
	}
	return mCells[y * mWidthCells + x].get();
}

bool IsCircleFullyInsideCircle(const Vec2f &pA, float rA, const Vec2f &pB, float rB)
{
	if (rB > rA)
	{
		return Math::Distance(pA, pB) < rB - rA;
	}
	return false;
}

int Terrain::GetCellsNear(const Vec2f &p, float radius, array<Cell *, 9> &out)
{
	// pre-calculate some constants at compile-time
	constexpr float halfCellSize = static_cast<float>(Cell::Size) / 2.0f;

	// since terrain stands at (0, 0) in the world and divided uniformly into 	DwarfDig.exe!Physics::ParticleTerrainCalculationThread(Physics * phys) Line 390	C++

	// square cells we can obtain cell index easily by dividing coordinate of
	// point 'p' by cell size
	const int x = static_cast<int>(p.x * Cell::InvSize);
	const int y = static_cast<int>(p.y * Cell::InvSize);

	if (radius < halfCellSize)
	{
		// case 1: particle can fit into single cell
		// this also means that it can intersect three additional cells (four all)

		// get central cell
		out[0] = GetAt(x, y);

		if (!out[0])
		{
			// out of terrain bounds
			return 0;
		}

		// check, if particle fit into single cell
		if (p.x - radius > out[0]->GetPosition().x && p.y - radius > out[0]->GetPosition().y &&
			p.x + radius < out[0]->mRight && p.y + radius < out[0]->mBottom)
		{
			// best case
			return 1;
		}
		else
		{
			// nope, then particle can intersect up to four cells
			// select three last cells
			//
			// description:
			//
			// let O be the center of the cell, then by position of * relative to O
			// we can pick four cells for collision detection (or other needs)
			//
			// in such case:
			// __________________________________
			// |          |          |          |
			// |          |          |          |
			// |          |          |          |
			// |          |          |          |
			// |__________|__________|__________|
			// |          |          |          |
			// |          |          |          |
			// |          |     O    |          |
			// |          |       *  |          |
			// |__________|__________|__________|
			// |          |          |          |
			// |          |          |          |
			// |          |          |          |
			// |          |          |          |
			// |__________|__________|__________|
			//
			// picked cells will be:
			// __________________________________
			// |          |          |          |
			// |          |          |          |
			// |          |          |          |
			// |          |          |          |
			// |__________|__________|__________|
			// |          |..........|..........|
			// |          |..........|..........|
			// |          |.....O....|..........|
			// |          |.......*..|..........|
			// |__________|__________|__________|
			// |          |..........|..........|
			// |          |..........|..........|
			// |          |..........|..........|
			// |          |..........|..........|
			// |__________|__________|__________|
			//
			//
			if (p.x > out[0]->mCenter.x)
			{
				if (p.y > out[0]->mCenter.y)
				{
					// right bottom corner from central cell
					out[1] = GetAt(x + 1, y);
					out[2] = GetAt(x, y + 1);
					out[3] = GetAt(x + 1, y + 1);
				}
				else
				{
					// right up corner from central cell
					out[1] = GetAt(x + 1, y);
					out[2] = GetAt(x, y - 1);
					out[3] = GetAt(x + 1, y - 1);
				}
			}
			else
			{
				if (p.y < out[0]->mCenter.y)
				{
					// left up corner from central cell
					out[1] = GetAt(x - 1, y);
					out[2] = GetAt(x, y - 1);
					out[3] = GetAt(x - 1, y - 1);
				}
				else
				{
					// left bottom corner from central cell
					out[1] = GetAt(x - 1, y);
					out[2] = GetAt(x, y + 1);
					out[3] = GetAt(x - 1, y + 1);
				}
			}
			// normal case
			return 4;
		}
	}
	else
	{
		// case 2: particle too large and can intersect up to nine cells
		// dont bother with precise detection which cells particle intersects
		// just gather all nine
		out[0] = GetAt(x, y);
		out[1] = GetAt(x + 1, y);
		out[2] = GetAt(x - 1, y);
		out[3] = GetAt(x, y + 1);
		out[4] = GetAt(x, y - 1);
		out[5] = GetAt(x + 1, y + 1);
		out[6] = GetAt(x - 1, y + 1);
		out[7] = GetAt(x + 1, y - 1);
		out[8] = GetAt(x - 1, y - 1);
	}
	// worst case
	return 9;
}

const vector<shared_ptr<Cell>> &Terrain::GetCells() const 
{
	return mCells;
}

unique_ptr<QuadTree> &Terrain::GetQuadTree()
{
	return mQuadTree;
}

TerrainRayTraceResult Terrain::TraceRay(const Vec2f &origin, const Vec2f &direction)
{
	TerrainRayTraceResult closestResult;
	closestResult.mPosition = Vec2f(FLT_MAX, FLT_MAX);
	auto &cells = mQuadTree->TraceRay(origin, direction);
	for (auto &cell : cells)
	{
		TerrainRayTraceResult result = cell->RayIntersection(origin, direction);
		if (result.mIntersection)
		{
			if (Math::SqrDistance(result.mPosition, origin) <
				Math::SqrDistance(closestResult.mPosition, origin))
			{
				closestResult = result;
			}
		}
	}
	return closestResult;
}

void Terrain::Update()
{
	mFogOfWar->Update();

	for (auto &cell : mCells)
	{
		cell->Update();
	}
}

void Terrain::Mask(const MaskOperation &args)
{
	for (auto &cell : mCells)
	{
		cell->Mask(args);
	}
}

sf::Vector2i Terrain::GetSize() const
{
	return sf::Vector2i(mWidthCells, mHeightCells);
}

void Terrain::Fill(MaskOperation &args, const shared_ptr<Texture> &tex)
{
	for (auto &cell : mCells)
	{
		cell->Fill(args, tex);
	}
}

void Terrain::Serialize(Serializer &sav)
{
	sav &mScene;
	sav &mWidthCells;
	sav &mHeightCells;
	sav &mCells;
	sav &mFogOfWar;
	sav &mFogOfWarEnabled;
	sav &mTopBackgroundlessPartHeight;
	sav &mBackgroundTextureStack;
	sav &mAttributeGrid;
	if (sav.IsLoading())
	{
		mQuadTree = make_unique<QuadTree>(this);
		BuildNavMesh();
	}
}

void Terrain::DrillCicle(const Vec2f &position, float radius)
{
	MaskOperation args;
	args.mBounds = Bounds::Circle;
	args.mFirstPassBitwiseOperation = BitwiseOp::And;
	args.mFirstPassMask = ~(PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE | PA_LAVA);
	args.mRadius = radius;
	args.mPosition = position;
	Mask(args);
}

shared_ptr<Cell> Terrain::PickCell(const Vec2f &position)
{
	int j = static_cast<int>(position.x * Cell::InvSize);
	int i = static_cast<int>(position.y * Cell::InvSize);
	if (j < 0)
		j = 0;
	if (j >= mWidthCells)
		j = mWidthCells - 1;
	if (i < 0)
		i = 0;
	if (i >= mHeightCells)
		i = mHeightCells - 1;
	return mCells[i * mWidthCells + j];
}

FogOfWar * Terrain::GetFogOfWar() const
{
	return mFogOfWar.get();
}

void Terrain::RevealFogOfWarAt(const Vec2f & point, unsigned int time)
{
	mFogOfWar->RevealAt(AbsoluteToNormalized(point), time);
}

void Terrain::SetFogOfWarEnabled(bool state)
{
	mFogOfWarEnabled = state;
}

bool Terrain::IsFogOfWarEnabled() const
{
	return mFogOfWarEnabled;
}

CellPixel *Terrain::GetPixel(int x, int y)
{
	if (auto cell = RemapWorldCoordinatesToCell(x, y))
	{
		return cell->GetPixel(x, y);
	}
	return nullptr;
}

uint8_t * Terrain::GetBackgroundMaskPixel(int x, int y)
{
	if (auto cell = RemapWorldCoordinatesToCell(x, y))
	{
		return cell->GetBackgroundMaskPixel(x, y);
	}
	return nullptr;
}

void Terrain::GatherPixelsInsideRect(const Vec2f &rectPosition, const Vec2f &rectSize, int dx,
	int dy, vector<CellTraceResult> &outPositions, uint8_t gatherMask)
{
	outPositions.clear();
	for (auto &cell : mCells)
	{
		cell->GatherPixelsInsideRect(rectPosition, rectSize, dx, dy, outPositions, gatherMask);
	}
}

void Terrain::GatherPixelsInsideCircle(const Vec2f &circlePosition, float radius, int dx, int dy,
	vector<CellTraceResult> &outPositions, uint8_t gatherMask)
{
	outPositions.clear();
	for (auto &cell : mCells)
	{
		cell->GatherPixelsInsideCircle(circlePosition, radius, dx, dy, outPositions, gatherMask);
	}
}

float Terrain::GetTopBackgroundlessPartHeight() const
{
	return mTopBackgroundlessPartHeight;
}

void Terrain::SetTopBackgroundlessPartHeight(float height)
{
	mTopBackgroundlessPartHeight = height;
}

void Terrain::DrawLine(
	const Vec2f &begin, const Vec2f &end, float brushWidth, MaskOperation &args)
{
	args.mBounds = Bounds::Circle;
	args.mPosition = begin;
	args.mRadius = brushWidth * 0.5f;
	Vec2f d = end - begin;
	const int steps = static_cast<int>(Math::Length(d) / args.mRadius);
	d /= static_cast<float>(steps);
	for (int i = 0; i < steps; i++)
	{
		args.mPosition += d;
		Mask(args);
	}
}

void Terrain::DrawWireRect(const sf::FloatRect &r, float sideWidth, MaskOperation &args)
{
	args.mEditorAccess = true;
	args.mBounds = Bounds::Rect;
	// top
	args.mPosition = { r.left, r.top };
	args.mRectSize = { r.width, sideWidth };
	Mask(args);
	// right
	args.mPosition = { r.left + r.width - sideWidth, r.top };
	args.mRectSize = { sideWidth, r.height };
	Mask(args);
	// bottom
	args.mPosition = { r.left, r.top + r.height };
	args.mRectSize = { r.width, sideWidth };
	Mask(args);
	// left
	args.mPosition = { r.left, r.top };
	args.mRectSize = { sideWidth, r.height };
	Mask(args);
}

void Terrain::OutlinePolygon(const Math::Polygon & poly, float brushWidth, MaskOperation & args)
{
	const int ptCount = poly.GetPointCount();
	for (int i = 0; i < ptCount; ++i)
	{
		Vec2f begin = poly.GetPoint(i);
		Vec2f end = poly.GetPoint((i + 1 == ptCount) ? 0 : i + 1);
		DrawLine(begin, end, brushWidth, args);
	}
}

shared_ptr<TextureStack> Terrain::GetBackgroundTextureStack()
{
	return mBackgroundTextureStack;
}



Vec2f Terrain::GetSizePixels() const
{
	return { static_cast<float>(GetWidthPixels()), static_cast<float>(GetHeightPixels()) };
}

void Terrain::BuildNavMesh()
{
	auto lastTime = chrono::high_resolution_clock::now();

	const int w = mWidthCells * Cell::Size;
	const int h = mHeightCells * Cell::Size;

	const int xSteps = w / NavmeshStep;
	const int ySteps = h / NavmeshStep;

	// step 1: place vertices uniformly and mark each as passable\impassable
	vector<unique_ptr<GraphVertex>> vertices;
	vertices.reserve(xSteps * ySteps);
	for (int i = 0; i < ySteps; ++i)
	{
		int y = i * NavmeshStep;
		for (int j = 0; j < xSteps; ++j)
		{
			int x = j * NavmeshStep;
			auto pixelAttrib = GetPixel(x, y)->a;
			vertices.push_back(make_unique<GraphVertex>(Vec2f(Vec2i(x, y))));
			if (!((pixelAttrib & PA_VISIBLE) && (pixelAttrib & PA_COLLIDABLE)))
			{
				vertices.back()->SetFlags(PassabilityFlag_Passable | PassabilityFlag_Fly);
			}
		}
	}

	// step 1a: set additional flags to vertices
	int walkableCount = 0;
	for (int i = 1; i < ySteps; ++i)
	{
		int rowOffset = i * xSteps;
		int prevRowOffset = (i - 1) * xSteps;
		for (int j = 0; j < xSteps; ++j)
		{
			auto &current = vertices[rowOffset + j];
			auto &up = vertices[prevRowOffset + j];
			auto currentNotPassable = !(current->GetFlags() & PassabilityFlag_Passable);
			auto upIsPassable = up->GetFlags() & PassabilityFlag_Passable;
			if (currentNotPassable && upIsPassable)
			{
				// mark up vertex as 'walkable' for ground units
				up->SetFlags(PassabilityFlag_Walk);
				++walkableCount;
			}
		}
	}

	// step 2: link vertices
	int yEnd = ySteps - 1;
	int xEnd = xSteps - 1;
	for (int y = 0; y < yEnd; ++y)
	{
		int rowOffset = y * xSteps;
		int nextRowOffset = rowOffset + xSteps;
		for (int x = 0; x < xEnd; ++x)
		{
			auto &current = vertices[rowOffset + x];
			if (current->GetFlags() & PassabilityFlag_Passable)
			{
				// todo: add raycast passability check between linked vertices
				auto &right = vertices[rowOffset + x + 1];
				auto &bottom = vertices[nextRowOffset + x];
				// link right
				if (right->GetFlags() & PassabilityFlag_Passable)
				{
					current->Link(right.get());
				}
				// link bottom
				if (bottom->GetFlags() & PassabilityFlag_Passable)
				{
					current->Link(bottom.get());
				}
			}
		}
	}

	auto elapsedTime = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - lastTime).count();
	Log() << "Terrain NavMesh was rebuilt in " << elapsedTime << " microseconds!" << LineEnd;
	mPathfinder.SetNavmesh(move(vertices), { xSteps, ySteps });
}

AStarPathfinder &Terrain::GetPathfinder()
{
	return mPathfinder;
}

GraphVertex* Terrain::GetNearestPathVertex(const Vec2f &position, int vFlags)
{
	// terrain uniformly filled with graph vertices with 'NavmeshStepF' step
	// so we can easily get index of graph vertex only know it's position on
	// terrain
	int ix = static_cast<int>(position.x / NavmeshStepF);
	int iy = static_cast<int>(position.y / NavmeshStepF);
	// for some reason (bugs, of course) indices may become invalid
	const auto size = mPathfinder.GetNavmeshSize();
	if (ix < 0 || ix >= size.x || iy < 0 || iy >= size.y)
	{
		return nullptr;
	}
	// using these indices we can get nearest graph vertex
	auto &vertices = mPathfinder.GetNavmesh();
	// but we are not done yet
	// we have to ensure that 'v' vertex is closest and match some passed flags
	// to do this, we define square (on boundary - rectangular) zone arond 'v'
	// vertex and look for closest vertex inside of it
	const int lookupKernelSize = 3;
	const int minX = Math::Clamp(ix - lookupKernelSize, 0, size.x);
	const int maxX = Math::Clamp(ix + lookupKernelSize, 0, size.x);
	const int minY = Math::Clamp(iy - lookupKernelSize, 0, size.y);
	const int maxY = Math::Clamp(iy + lookupKernelSize, 0, size.y);
	GraphVertex* closest = nullptr;
	float closestDistance = FLT_MAX;
	for (int y = minY; y < maxY; ++y)
	{
		int ky = y * size.x;
		for (int x = minX; x < maxX; ++x)
		{
			auto &vertex = vertices[ky + x];
			if (vertex->GetFlags() & vFlags)
			{
				float d = Math::SqrDistance(vertex->GetPosition(), position);
				if (d < closestDistance)
				{
					// 'vertex' is closest, remember it
					closest = vertex.get();
					closestDistance = d;
				}
			}
		}
	}
	return closest;
}

// Remaps absolute coordinates to normalized [0; 1]

Vec2f Terrain::AbsoluteToNormalized(const Vec2f & position)
{
	float nx = Math::Clamp(position.x / static_cast<float>(mWidthCells * Cell::Size), 0.0f, 1.0f);
	float ny = Math::Clamp(position.y / static_cast<float>(mHeightCells * Cell::Size), 0.0f, 1.0f);

	return { nx, ny };
}

AttributeGrid * Terrain::GetAttributeGrid()
{
	return mAttributeGrid.get();
}

void Terrain::SetCellAt(const shared_ptr<Cell>& cell, int index)
{
	if (index >= 0 && index < (int)mCells.size())
	{
		// Make sure that renderer knows that this cell needs to be re-uploaded to gpu
		cell->mNeedUpdate = true;

		mCells[index] = cell;
	}
}
