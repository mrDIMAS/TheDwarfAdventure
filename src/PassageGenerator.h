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

enum class PassageDensity
{
	Low,
	Medium,
	High,
	Max,
};

// Used to create man-made structures
class PassageGenerator : public LocationGeneratorModule
{
public:
	PassageGenerator(LocationGenerator * parentGenerator);
	~PassageGenerator();
	// ====================
	// Input values
	void SetArea(const sf::FloatRect & area);
	void AddForbiddenArea(const sf::FloatRect & area);
	// Note: If predefined passages intersects, last added intersected passage will be discarded!
	void AddPredefinedPassageBounds(const sf::FloatRect & bounds);
	sf::FloatRect GetArea() const;
	void Generate() override;
	void SetDensity(const PassageDensity & density);
	void AllowGenerateMonsters(bool state);
	void AllowGenerateInhabitants(bool state);
private:
	friend class LocationGenerator;
	
	// Methods
	bool IsInsersectAnyPassageLink(const sf::FloatRect & bounds);

	bool IsIntersectAnyForbiddenZone(const sf::FloatRect &bounds);
	bool IsIntersectAnyPassage(const sf::FloatRect &bounds, float xBloat = 0, float yBloat = 0);
	bool IsFullyInsideArea(const sf::FloatRect &bounds, const sf::FloatRect &area);
	void GenerateDecorations();
	void GenerateMotherCrystals();
	void GeneratePassageSpacers();
	void GenerateLights();
	void GenerateRooms();
	void GeneratePassages();
	void GeneratePassageLinks();
	void GenerateDetails();
	// Fuse, prevents infinite loops
	int MaxTries { 50 };
	// These default values will be overwritten when JSON config will be loaded
	float mPassageSpacing { 80 };
	float mLinkMaxHeight { 400 };
	float mLinkMinWidth { 60 };
	float mLinkMaxWidth { 80 };
	float mRoomCorridorLength { 100 };
	float mPassageLightDistance { 150 };
	float mLadderWidth { 30 };
	float mPassageMinWidth { 250 };
	float mPassageMaxWidth { 1000 };
	float mPassageHeightMin { 100 };
	float mPassageHeightMax { 140 };
	float mRoomWidthMin { 150 };
	float mRoomWidthMax { 240 };
	float mRoomHeightMin { 160 };
	float mRoomHeightMax { 200 };
	float mRoomSpacing { 10 };
	float mColumnPlacementDistance { 80 };
	PassageDensity mDensity { PassageDensity::Low };
	bool mGenerateMonsters { true };
	bool mAllowInhabitants { true };
	
	// Structure will be generated inside this bounds
	sf::FloatRect mArea;
	// List of forbidden areas: no rooms, passages, links will be placed inside
	vector<sf::FloatRect> mForbiddenZones;
	queue<sf::FloatRect> mPredefinedPassageBounds;
	bool HasAnotherPredefinedPassage();
	sf::FloatRect GetAnotherPredefinedPassage();
};
