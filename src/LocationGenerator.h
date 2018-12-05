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

#include "LocationType.h"
#include "MonsterGenerator.h"

class Level;
class LocationGeneratorModule;

class LocationGenerator
{
public:
	static mutex SyncMutex;
	LocationGenerator(Level* level, const shared_ptr<LocationDefinition>& params);
	virtual ~LocationGenerator();
	virtual shared_ptr<Location> Generate() = 0;
	shared_ptr<Location> GetLocation() const;
	void CreateBlankLocation();
	void CreateRandomTerrain();
	void InsertDecoration(const sf::FloatRect& bounds, const shared_ptr<Texture>& texture);
	void CreateParallaxBackground(bool singleLayer, const vector<shared_ptr<Texture>> & layers, bool fullscreen);
	void CreateClouds(int cloudCount, float thickness);
	void CreateCrystalLight(const Vec2f & position, float radius);
	void CreateTorchLight(const Vec2f & position, float radius);
	void AddBorders(bool addCap);
	shared_ptr<Sprite> AddSpriteDecoration(const Vec2f & position, const Vec2f & size, const shared_ptr<Texture> & texture);
	shared_ptr<Sprite> AddSpriteDecoration(const sf::FloatRect & bounds, const shared_ptr<Texture> & texture);
	void AddTerrainDecoration(const Vec2f & position, const Vec2f & size, const shared_ptr<Texture> & texture);
	void CreateMotherCrystal(const Vec2f & position);
	void DrawPassage(const sf::FloatRect & bounds, TerrainBrush * brush);
	MonsterGenerator* GetMonsterGenerator();
	void PlaceOre(const sf::FloatRect & area);
	void AddLava(float lavaHeight);
	void PlaceTreasureChests();
	const string & GetProgressMessage() const;
	void SetProgressMessage(const string & message);
	void MarkAsDone();
	bool IsDone() const;
	const shared_ptr<LocationDefinition> GetDefinition() const;
protected:		
	bool mDone { false };	
	shared_ptr<LocationDefinition> mDefinition;
    Level* const mLevel;
	shared_ptr<Location> mLocation;
	unique_ptr<MonsterGenerator> mMonsterGenerator;
	vector<unique_ptr<LocationGeneratorModule>> mModuleList;
private:
	string mProgressMessage;
};

class LocationGeneratorModule
{
public:
	virtual ~LocationGeneratorModule();
	virtual void Generate() = 0;

protected:
	friend class LocationGenerator;
	LocationGeneratorModule(LocationGenerator * parentGenerator);
	LocationGenerator * const mParentGenerator;
};

