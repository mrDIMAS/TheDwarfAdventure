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
#include "NaturalCaveGenerator.h"
#include "Terrain.h"
#include "PerlinNoise.h"
#include "Location.h"
#include "Scene.h"
#include "LocationMap.h"

NaturalCaveGenerator::NaturalCaveGenerator(Level * level, const shared_ptr<LocationDefinition>& params)
	: LocationGenerator(level, params)
{

}

NaturalCaveGenerator::~NaturalCaveGenerator()
{

}

shared_ptr<Location> NaturalCaveGenerator::Generate()
{
	CreateBlankLocation();	
	CreateRandomTerrain();
	mLocation->SetClass(LocationClass::Underground);
	shared_ptr<Terrain> terrain = mLocation->GetScene()->GetTerrain();
	terrain->SetTopBackgroundlessPartHeight(0);

	PerlinNoise png;
	const int width = terrain->GetWidthPixels();
	const int height = terrain->GetHeightPixels();
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			auto pixel = terrain->GetPixel(x, y);
			float noiseX = 10 * (x / static_cast<float>(width));
			float noiseY = 10 * (y / static_cast<float>(height));
			float alpha = png.OctaveNoise(noiseX, noiseY, 0.0f, 2, 0.7f);

			if (alpha > mAlphaThresholdValue)
			{
				pixel->a |= PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE;
			}
			else
			{
				pixel->a &= ~(PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE);
				Vec2f position;
				position.x = static_cast<float>(x);
				position.y = static_cast<float>(y);
				mLocation->GetMap()->DrawPixel(position, { 128, 128, 128 });
			}
		}
	}

	PlaceOre({ { 0, 0 }, mLocation->GetScene()->GetTerrain()->GetSizePixels() });
	PlaceTreasureChests();

	return mLocation;
}
