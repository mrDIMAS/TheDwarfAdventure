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
#include "UndergroundSettlementGenerator.h"
#include "Location.h"
#include "Scene.h"
#include "Terrain.h"
#include "PassageGenerator.h"
#include "BossAreaGenerator.h"

UndergroundSettlementGenerator::UndergroundSettlementGenerator(Level * level, const shared_ptr<LocationDefinition>& params)
	: LocationGenerator(level, params)
{

}

shared_ptr<Location> UndergroundSettlementGenerator::Generate()
{
	const float lavaHeight = 50;

	CreateBlankLocation();
	CreateRandomTerrain();

	const shared_ptr<Scene> scene = mLocation->GetScene();
	const shared_ptr<Terrain> terrain = scene->GetTerrain();

	terrain->SetTopBackgroundlessPartHeight(0);

	AddBorders(true);

	const float bossAreaSpacing = 200;
	sf::FloatRect bossArea;
	bossArea.width = Math::Rnd(600.0f, 1000.0f);
	bossArea.height = Math::Rnd(300.0f, 350.0f);
	bossArea.left = Math::Rnd(bossAreaSpacing, terrain->GetSizePixels().x - 2 * bossAreaSpacing - bossArea.width);
	bossArea.top = Math::Rnd(bossAreaSpacing, terrain->GetSizePixels().y - 2 * bossAreaSpacing - bossArea.height);
	BossAreaGenerator bossGenerator(this, bossArea);
	bossGenerator.Generate();
			
	PassageGenerator passageGenerator(this);
	passageGenerator.SetDensity(PassageDensity::Medium);
	passageGenerator.AddForbiddenArea(bossArea);
	passageGenerator.SetArea({ { 0, 0 }, { terrain->GetSizePixels().x, terrain->GetSizePixels().y - lavaHeight } });
	passageGenerator.Generate();

	PlaceTreasureChests();

	AddLava(lavaHeight);

	PlaceOre({ { 0, 0 }, mLocation->GetScene()->GetTerrain()->GetSizePixels() });

	return mLocation;
}
