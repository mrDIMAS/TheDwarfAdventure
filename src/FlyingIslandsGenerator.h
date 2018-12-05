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

#include "LocationGenerator.h"

class FlyingInslandsGenerator : public LocationGenerator
{
private:
	// Config values
	// Note: Will be overwritten after ReadConfig
	float mBorderSize = 100;
	Math::Range<float> islandWidthRange = { 300, 700 };
	Math::Range<float> islandHeightRange = { 150, 250 };
	Math::Range<float> islandYOffsetRange = { -50, 50 };
	Math::Range<float> xSpaceBetweenIslands = { 30, 50 };
	Math::Range<float> ySpaceBetweenIslands = { 60, 120 };
	Math::Range<int> islandSideSubpointsCount = { 3, 6 };

	void ReadConfig();
	void GenerateStructure();	
	bool IsPointInsideAnyIsland(const Vec2f & p) const;
	void ClearTerrain();
	void RasterizeIslands();
	void RasterizeLadders();
	void RasterizeIslandGrass();
public:
	shared_ptr<Island> GetRandomIsland() const;
	FlyingInslandsGenerator(Level * level, const shared_ptr<LocationDefinition>& params);
	~FlyingInslandsGenerator();
	shared_ptr<Location> Generate() override;
};