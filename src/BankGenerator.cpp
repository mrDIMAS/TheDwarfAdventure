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
#include "BankGenerator.h"
#include "Location.h"
#include "Scene.h"
#include "Terrain.h"
#include "PassageGenerator.h"
#include "NPC.h"
#include "Passage.h"

BankGenerator::BankGenerator(Level * level, const shared_ptr<LocationDefinition>& params)
	: LocationGenerator(level, params)
{

}

shared_ptr<Location> BankGenerator::Generate()
{
	CreateBlankLocation();
	CreateRandomTerrain();
	mLocation->SetClass(LocationClass::Underground);

	const shared_ptr<Terrain> terrain = mLocation->GetScene()->GetTerrain();
	terrain->SetTopBackgroundlessPartHeight(0);

	sf::FloatRect area;
	area.width = terrain->GetSizePixels().x;
	area.height = terrain->GetSizePixels().y;

	sf::FloatRect visitorArea;
	// Visitor area is quarter of entire bank area
	visitorArea.left = 0;
	visitorArea.top = 0;
	visitorArea.width = area.width / 3;
	visitorArea.height = area.height / 3;

	// Predefined passages for each bank and visitor areas
	sf::FloatRect visitorPassage;
	visitorPassage.height = 120;
	visitorPassage.width = 600;
	visitorPassage.left = visitorArea.width - visitorPassage.width;
	visitorPassage.top = visitorArea.height / 2;

	sf::FloatRect bankPassage;
	bankPassage.height = 120;
	bankPassage.width = 600;
	bankPassage.left = visitorPassage.left + visitorPassage.width;
	bankPassage.top = visitorPassage.top;

	// Generate visitor area first  
	PassageGenerator visitorPassageGenerator(this);
	visitorPassageGenerator.AllowGenerateMonsters(false);
	visitorPassageGenerator.AllowGenerateInhabitants(false);
	visitorPassageGenerator.SetArea(visitorArea);
	visitorPassageGenerator.AddPredefinedPassageBounds(visitorPassage);
	visitorPassageGenerator.Generate();

	// Generate rest of bank area
	PassageGenerator bankPassageGenerator(this);
	bankPassageGenerator.AllowGenerateMonsters(false);
	bankPassageGenerator.AllowGenerateInhabitants(false);
	bankPassageGenerator.SetArea(area);
	bankPassageGenerator.AddPredefinedPassageBounds(bankPassage);
	// Slightly decrease size of forbidden zone to make sure that predefined passages will be added
	sf::FloatRect forbiddenZone = visitorArea;
	forbiddenZone.width -= 20;
	forbiddenZone.height -= 20;
	bankPassageGenerator.AddForbiddenArea(forbiddenZone);
	bankPassageGenerator.Generate();

	// Add guards into bank passages
	for (const shared_ptr<Passage> & passage : mLocation->GetPassageList())
	{
		const sf::FloatRect passageBounds = passage->GetBounds();

		Vec2f position = {
			passageBounds.left + passageBounds.width / 2,
			passageBounds.top + passageBounds.height / 2
		};

		auto guard = make_shared<NPC>(mLocation, NPCType::Guard);
		guard->Randomize();
		guard->SetLevel(mLocation->GetDefinition()->GetBaseLevel() + 5);
		guard->SetPosition(position);
		guard->SetPatrolPoints(
		{ passageBounds.left + 40, passageBounds.top + passageBounds.height - 10 },
		{ passageBounds.left + passageBounds.width - 40, passageBounds.top + passageBounds.height - 10 });

		mLocation->AddActor(guard);
	}

	//PlaceWaterInPassages(location, v->mPassageGen->GetPassageList());

	PlaceOre({ { 0, 0 }, mLocation->GetScene()->GetTerrain()->GetSizePixels() });
	PlaceTreasureChests();
	
	return mLocation;
}
