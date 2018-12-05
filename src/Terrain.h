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

#include "Collision.h"
#include "Node.h"
#include "Cell.h"
#include "QuadTree.h"
#include "AStar.h"
#include "FogOfWar.h"

class Scene;
class TextureStack;
class AttributeGrid;

class Terrain : public enable_shared_from_this<Terrain>
{
public:
	static constexpr int NavmeshStep = 32;
	static constexpr float NavmeshStepF = static_cast<float>(NavmeshStep);
	// threshold value for alpha channel of a pixel
	// when pixel.a >= threshold pixel becomes visible
	// max value should not exceed UINT8_MAX
	static constexpr int ContrastFilterThreshold = 80;
	static constexpr int SmoothKernelSize = 8;

	void Smooth();
	// returns terrain represented as bitmap, if terrain's pixel visible, out
	// value is 255, 0 - otherwise this method is for internal purposes
	void ExtractAndConvertMaskLayer(vector<uint8_t> &out);
	Terrain();
	Terrain(const shared_ptr<Scene> &scene, int widthCells, int heightCells, int backStackEntrySize);
	~Terrain();
	shared_ptr<Scene> GetScene() const;
	void MeltCicle(const Vec2f &position, float radius, const sf::Color &borderColor, float borderSize = 4);
	int GetWidthPixels() const;
	int GetHeightPixels() const;
	Cell *GetAt(int x, int y);
	// returns count of cells
	int GetCellsNear(const Vec2f &p, float radius, array<Cell *, 9> &out);
	const vector<shared_ptr<Cell>> &GetCells() const;
	unique_ptr<QuadTree> &GetQuadTree();
	TerrainRayTraceResult TraceRay(const Vec2f &origin, const Vec2f &direction);
	void Update();
	void Mask(const MaskOperation &args);
	sf::Vector2i GetSize() const;
	void Fill(MaskOperation &args, const shared_ptr<Texture> &tex);
	void Serialize(Serializer &sav);
	void DrillCicle(const Vec2f &position, float radius);
	shared_ptr<Cell> PickCell(const Vec2f &position);
	FogOfWar* GetFogOfWar() const;
	void RevealFogOfWarAt(const Vec2f &point, unsigned int time = FogOfWar::InfiniteTime);
	void SetFogOfWarEnabled(bool state);
	bool IsFogOfWarEnabled() const;
	CellPixel *GetPixel(int x, int y);
	uint8_t * GetBackgroundMaskPixel(int x, int y);
	/// pass 0 as gatherMask to gather all pixels
	void GatherPixelsInsideRect(const Vec2f &rectPosition, const Vec2f &rectSize, int dx, int dy, vector<CellTraceResult> &outPositions, uint8_t gatherMask = PA_VISIBLE);
	/// pass 0 as gatherMask to gather all pixels
	void GatherPixelsInsideCircle(const Vec2f &circlePosition, float radius, int dx, int dy, vector<CellTraceResult> &outPositions, uint8_t gatherMask = PA_VISIBLE);
	float GetTopBackgroundlessPartHeight() const;
	void SetTopBackgroundlessPartHeight(float height);
	void DrawLine(const Vec2f &begin, const Vec2f &end, float brushWidth, MaskOperation &args);
	void DrawWireRect(const sf::FloatRect &r, float sideWidth, MaskOperation &args);
	void OutlinePolygon(const Math::Polygon & poly, float brushWidth, MaskOperation &args);
	shared_ptr<TextureStack> GetBackgroundTextureStack();
	Vec2f GetSizePixels() const;
	void BuildNavMesh();
	AStarPathfinder &GetPathfinder();
	GraphVertex* GetNearestPathVertex(const Vec2f &position, int vFlags);
	// Remaps absolute coordinates to normalized [0; 1]
	Vec2f AbsoluteToNormalized(const Vec2f & position);
	AttributeGrid* GetAttributeGrid();
	void SetCellAt(const shared_ptr<Cell>& cell, int index);
private:
	weak_ptr<Scene> mScene;
	int mWidthCells { 0 };
	int mHeightCells { 0 };
	vector<shared_ptr<Cell>> mCells;
	unique_ptr<QuadTree> mQuadTree;
	unique_ptr<FogOfWar> mFogOfWar;
	unique_ptr<AttributeGrid> mAttributeGrid;
	bool mFogOfWarEnabled { true };
	AStarPathfinder mPathfinder;
	float mTopBackgroundlessPartHeight { 220 };
	shared_ptr<TextureStack> mBackgroundTextureStack;
	Cell * RemapWorldCoordinatesToCell(int & x, int & y);
};
