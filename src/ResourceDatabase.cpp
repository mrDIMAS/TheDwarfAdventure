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
#include "ResourceDatabase.h"
#include "Renderer.h"
#include "Game.h"

void ResourceDatabase::LoadTexturesFromFolder(const string & path, vector<shared_ptr<Texture>>& textures)
{
	for (auto &p : filesystem::recursive_directory_iterator(path))
	{
		if (p.path().has_extension())
		{
			auto ext = p.path().extension().string();
			if (ext == ".png" || ext == ".jpg" || ext == ".bmp" || ext == ".tga")
			{
				textures.push_back(game->GetRenderer()->RequestTexture(p.path().string()));
			}
		}
	}
}

void ResourceDatabase::LoadTerrainBrushes()
{
	vector<shared_ptr<Texture>> brushTextures;
	LoadTexturesFromFolder("data/textures/terrain_textures/", brushTextures);

	vector<pair<string, TerrainBrushClass>> resourceMapping = {
		{ "crystal_", TerrainBrushClass::Crystal },
		{ "lava_", TerrainBrushClass::Lava },
		{ "rock_", TerrainBrushClass::Rock },
		{ "soil_", TerrainBrushClass::Soil },
		{ "stone_", TerrainBrushClass::Stone },
		{ "metal_", TerrainBrushClass::Metal },
		{ "durable_metal_", TerrainBrushClass::DurableMetal },
		{ "grass_", TerrainBrushClass::Grass }
	};

	uint8_t stackId = 1;
	for (auto &textureId : brushTextures)
	{
		auto path = filesystem::path(textureId->GetSource());
		auto fileName = path.filename().u8string();

		for (auto & mapping : resourceMapping)
		{
			if (fileName.find(mapping.first) == string::npos)
			{
				// Ignore textures with incorrect name template
				continue;
			}

			if (fileName.find("_back") != string::npos)
			{
				// Ignore background textures
				continue;
			}

			auto brush = make_unique<TerrainBrush>();
			brush->mFrontTexture = textureId;

			// Look for back pair texture
			const auto name = fileName.substr(0, fileName.find_last_of('.'));
			const string backName = name + "_back";
			for (auto & otherTexture : brushTextures)
			{
				auto otherPath = filesystem::path(otherTexture->GetSource());
				if (otherPath.filename().string().find(backName) == 0)
				{
					brush->mBackTexture = otherTexture;
					break;
				}
			}

			if (brush->mBackTexture == nullptr)
			{
				throw Exception("Unable to find back pair for " + fileName + ". Back pair should have tile_number_back name format");
			}

			brush->mStackId = stackId++;

			mTerrainBrushList[mapping.second].push_back(move(brush));
		}
	}

	Log() << (int)stackId << " terrain brushes was loaded!" << LineEnd;
}

void ResourceDatabase::LoadRoomDecorations()
{
	struct ResourceTemplate
	{
		string name;
		DecorationTemplateType type;
		Vec2f defaultSize;
		string configName;
		DecorationMountSide mounting;
	};

	vector<ResourceTemplate> templates = {
		{ "bed_", DecorationTemplateType::Bed, { 52, 22 }, "bedSize", DecorationMountSide::Bottom },
		{ "table_", DecorationTemplateType::Table, { 60, 30 }, "tableSize", DecorationMountSide::Bottom },
		{ "vase_", DecorationTemplateType::Vase, { 16, 18 }, "vaseSize", DecorationMountSide::Bottom },
		{ "painting_", DecorationTemplateType::Painting, { 11, 11 }, "paintingSize", DecorationMountSide::Bottom },
		{ "cabinet_", DecorationTemplateType::Cabinet, { 25, 50 }, "cabinetSize", DecorationMountSide::Bottom },
		{ "anvil_", DecorationTemplateType::Anvil, { 42, 34 }, "anvilSize", DecorationMountSide::Bottom },
		{ "bench_", DecorationTemplateType::Bench, { 70, 61 }, "benchSize", DecorationMountSide::Bottom },
		{ "furnace_", DecorationTemplateType::Furnace, { 42, 60 }, "furnaceSize", DecorationMountSide::Bottom },
		{ "box_", DecorationTemplateType::WoodBox, { 52, 30 }, "woodBoxSize", DecorationMountSide::Bottom },
		{ "barrel_", DecorationTemplateType::Barrel, { 28, 39 }, "barrelSize", DecorationMountSide::Bottom },
		{ "ceiling_moss_", DecorationTemplateType::CeilingMoss, { 40, 10 }, "ceilingMoss", DecorationMountSide::Top },
		{ "pillar_", DecorationTemplateType::Pillar, { 16, 4 }, "pillar", DecorationMountSide::Top }
	};

	vector<shared_ptr<Texture>> textures;
	LoadTexturesFromFolder("data/textures/decorations/", textures);

	rapidjson::Document doc;
	LoadJSON("data/generators/room_generator.json", doc);
	auto root = doc.GetObject();

	int loadedCount = 0;
	for (auto &texture : textures)
	{
		ResourceTemplate * resourceTemplate = nullptr;

		for (auto & decorTemplateDesc : templates)
		{
			if (texture->GetSource().find(decorTemplateDesc.name) != string::npos)
			{
				resourceTemplate = &decorTemplateDesc;
				break;
			}
		}

		if (resourceTemplate)
		{
			auto decoration = make_unique<DecorationTemplate>();

			decoration->mTexture = texture;

			if (root.HasMember(resourceTemplate->configName.c_str()))
			{
				auto array = root[resourceTemplate->configName.c_str()].GetArray();
				decoration->mSize = { array[0].GetFloat(), array[1].GetFloat() };
			}
			else
			{
				decoration->mSize = resourceTemplate->defaultSize;
			}

			decoration->mMounting = resourceTemplate->mounting;

			mDecorationTemplateList[resourceTemplate->type].push_back(move(decoration));

			++loadedCount;
		}
	}

	Log() << loadedCount << " Room decorations was loaded!" << LineEnd;
}

void ResourceDatabase::LoadWallTemplates()
{
	vector<shared_ptr<Texture>> walls;
	LoadTexturesFromFolder("data/textures/walls/", walls);

	struct ResourceTemplate
	{
		string horizontalName;
		string verticalName;
		WallTemplateType type;
	};

	vector<ResourceTemplate> resourceMapping = {
		{ "wall_metal_horizontal_", "wall_metal_vertical_", WallTemplateType::Metal },
		{ "wall_wood_horizontal_", "wall_wood_vertical_", WallTemplateType::Wood },
		{ "wall_metal_mesh_horizontal_", "wall_metal_mesh_vertical_", WallTemplateType::MetalMesh },
	};

	int count = 0;
	for (auto &texture : walls)
	{
		auto path = filesystem::path(texture->GetSource());
		auto fileName = path.filename().u8string();

		for (auto & mapping : resourceMapping)
		{
			int position = fileName.find(mapping.verticalName);
			if (position == string::npos)
			{
				// Ignore textures with incorrect name template
				continue;
			}

			auto wallTemplate = make_unique<WallTemplate>();
			wallTemplate->mVertical = texture;


			// Look for horizontal pair texture
			auto right = fileName.substr(position + mapping.verticalName.length());
			auto horizontalName = mapping.horizontalName + right;
			for (auto & otherTexture : walls)
			{
				if (otherTexture->GetSource().find(mapping.horizontalName) != string::npos)
				{
					wallTemplate->mHorizontal = otherTexture;
					break;
				}
			}

			if (wallTemplate->mHorizontal == nullptr)
			{
				throw Exception("Unable to find horizontal pair for " + fileName);
			}

			if (mapping.type == WallTemplateType::Metal)
			{
				wallTemplate->mIsMetal = true;
			}
			else
			{
				wallTemplate->mIsMetal = false;
			}

			mWallTemplateList[mapping.type].push_back(move(wallTemplate));
			++count;
		}
	}

	Log() << count << " Wall templates was loaded!" << LineEnd;
}

void ResourceDatabase::LoadRoomBackgrounds()
{
	vector<shared_ptr<Texture>> textures;
	LoadTexturesFromFolder("data/textures/room_generator/backgrounds/", textures);

	for (auto & texture : textures)
	{
		auto background = make_unique<RoomBackground>();

		background->mTexture = texture;

		mRoomBackgroundList[RoomBackgroundType::Metal].push_back(move(background));
	}
}

void ResourceDatabase::LoadTerrainDecorations()
{
	vector<shared_ptr<Texture>> textures;
	LoadTexturesFromFolder("data/textures/terrain_randomizer/", textures);

	for (auto & texture : textures)
	{
		auto decoration = make_unique<TerrainDecoration>();

		decoration->mTexture = texture;

		mTerrainDecorationList[TerrainDecorationType::Generic].push_back(move(decoration));
	}
}

void ResourceDatabase::LoadBodyParts()
{
	vector<shared_ptr<Texture>> textures;
	LoadTexturesFromFolder("data/textures/body_parts/", textures);

	struct ResourceTemplate
	{
		string name;
		BodyPartTemplateType type;
	};

	vector<ResourceTemplate> templates = {
		{ "beard_", BodyPartTemplateType::Beard },
		{ "head", BodyPartTemplateType::Head },
		{ "hair_", BodyPartTemplateType::Hair }
	};

	for (auto & texture : textures)
	{
		filesystem::path path = texture->GetSource();
		auto filename = path.filename().u8string();

		for (auto & bpTemplate : templates)
		{
			if (filename.find(bpTemplate.name) == 0)
			{
				auto bodypart = make_unique<BodyPartTemplate>();

				bodypart->mTexture = texture;

				mBodyPartTemplateList[bpTemplate.type].push_back(move(bodypart));
			}
		}
	}
}

void ResourceDatabase::LoadWeaponModelTemplates()
{
	struct ResourcePartTemplate
	{
		WeaponPartTemplateType id;
		string texture;
		Vec2f origin;
		Vec2f size;
	};

	struct ResourceTemplate
	{
		WeaponModelTemplateType type;
		vector<ResourcePartTemplate> parts;
		Vec2f shootPoint;
	};

	vector<ResourcePartTemplate> hookParts = {
		{ WeaponPartTemplateType::Hook, "data/textures/weapons/hook.png", { 18, 3.5 }, { 18, 7 } },
		{ WeaponPartTemplateType::Model, "data/textures/weapons/hookLauncher.png", { 0, 5 }, { 24, 10 } },
	};

	vector<ResourcePartTemplate> pickaxeParts = {
		{ WeaponPartTemplateType::Model, "data/textures/weapons/pickaxe.png", { 8, 10 }, { 32, 20 } }
	};

	vector<ResourcePartTemplate> shotgunParts = {
		{ WeaponPartTemplateType::Model, "data/textures/weapons/shotgun.png", { 5, 6 }, { 30, 12 } },
		{ WeaponPartTemplateType::ShotTrail, "data/textures/weapons/shottrail.png", { 0, 0 }, { 0, 0 } },
		{ WeaponPartTemplateType::ShotFlame, "data/textures/weapons/shotflame.png", { 3.5, 2.5 }, { 7, 5 } }
	};

	vector<ResourceTemplate> weapons = {
		{ WeaponModelTemplateType::Hook, hookParts, { 18, 3.5 } },
		{ WeaponModelTemplateType::Pickaxe, pickaxeParts, { 32, 20 } },
		{ WeaponModelTemplateType::Shotgun, shotgunParts, { 30, 4 } }
	};

	for (auto & resTemplate : weapons)
	{
		auto wpnModelTemplate = make_unique<WeaponModelTemplate>();

		for (auto & resPartTemplate : resTemplate.parts)
		{
			WeaponPartTemplate part;
			part.mTexture = game->GetRenderer()->RequestTexture(resPartTemplate.texture);
			part.mOrigin = resPartTemplate.origin;
			part.mSize = resPartTemplate.size;

			wpnModelTemplate->mParts[resPartTemplate.id] = part;
		}

		wpnModelTemplate->mShootPoint = resTemplate.shootPoint;

		mWeaponModelTemplateList[resTemplate.type].push_back(move(wpnModelTemplate));
	}
}

ResourceDatabase::ResourceDatabase()
{
	LoadTerrainBrushes();
	LoadRoomDecorations();
	LoadWallTemplates();
	LoadRoomBackgrounds();
	LoadTerrainDecorations();
	LoadWeaponModelTemplates();
	LoadBodyParts();
}

TerrainBrush * ResourceDatabase::GetRandom(const TerrainBrushClass & type)
{
	return Math::RandomElement(mTerrainBrushList.at(type)).get();
}

void ResourceDatabase::GetTerrainBrushes(vector<TerrainBrush*>& brushes)
{
	for (auto & group : mTerrainBrushList)
	{
		for (auto & brush : group.second)
		{
			brushes.push_back(brush.get());
		}
	}
}

DecorationTemplate * ResourceDatabase::GetRandom(const DecorationTemplateType & type)
{
	return Math::RandomElement(mDecorationTemplateList.at(type)).get();
}

BodyPartTemplate * ResourceDatabase::GetRandom(const BodyPartTemplateType & type)
{
	return Math::RandomElement(mBodyPartTemplateList.at(type)).get();
}

WallTemplate * ResourceDatabase::GetRandom(const WallTemplateType & type)
{
	return Math::RandomElement(mWallTemplateList.at(type)).get();
}

WeaponModelTemplate * ResourceDatabase::GetRandom(const WeaponModelTemplateType & type)
{
	return Math::RandomElement(mWeaponModelTemplateList.at(type)).get();
}

TerrainDecoration * ResourceDatabase::GetRandom(const TerrainDecorationType & type)
{
	return Math::RandomElement(mTerrainDecorationList.at(type)).get();
}

RoomBackground * ResourceDatabase::GetRandom(const RoomBackgroundType & type)
{
	return Math::RandomElement(mRoomBackgroundList.at(type)).get();
}
