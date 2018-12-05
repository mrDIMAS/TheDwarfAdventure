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

///////////////////////////////////////////////////////////
enum class TerrainBrushClass
{
	First = 0,
	Rock = First,
	Grass,
	Soil,
	Stone,
	Crystal,
	Lava,
	Metal,
	DurableMetal,
	Count
};

// Every world generator module needs to know which texture it must use to draw a terrain.
struct TerrainBrush
{
	// Size of the textures should be equal
	shared_ptr<Texture> mFrontTexture;
	shared_ptr<Texture> mBackTexture;
	// id of mBackTexture in terrain's background texture stack
	uint8_t mStackId { 0 };
};

///////////////////////////////////////////////////////////
enum class RoomBackgroundType
{
	Metal
};

struct RoomBackground
{
	shared_ptr<Texture> mTexture;
};

///////////////////////////////////////////////////////////
enum class TerrainDecorationType
{
	Generic
};

struct TerrainDecoration
{
	shared_ptr<Texture> mTexture;
};

///////////////////////////////////////////////////////////
enum class DecorationMountSide
{
	Top,
	Bottom
};

enum class DecorationTemplateType
{
	First = 0,
	Vase = First,
	Anvil,
	Bench,
	Cabinet,
	Furnace,
	Painting,
	Table,
	Bed,
	WoodBox,
	Barrel,
	CeilingMoss,
	Pillar,
	Count,
};

struct DecorationTemplate
{
	shared_ptr<Texture> mTexture;
	Vec2f mSize;
	DecorationMountSide mMounting { DecorationMountSide::Bottom };
};

///////////////////////////////////////////////////////////
enum class WeaponModelTemplateType
{
	Hook,
	Pickaxe,
	Shotgun,
};

enum class WeaponPartTemplateType
{
	Model,
	Hook,
	ShotTrail,
	ShotFlame
};

struct WeaponPartTemplate
{
	shared_ptr<Texture> mTexture;
	Vec2f mOrigin;
	Vec2f mSize;
	Vec2f mShootPoint;
};

struct WeaponModelTemplate
{
	unordered_map<WeaponPartTemplateType, WeaponPartTemplate> mParts;
	Vec2f mShootPoint;
};

///////////////////////////////////////////////////////////
enum class BodyPartTemplateType
{
	Head,
	Beard,
	Hair,
};

struct BodyPartTemplate
{
	shared_ptr<Texture> mTexture;
};

///////////////////////////////////////////////////////////
enum class WallTemplateType
{
	Metal,
	MetalMesh,
	Wood,
};

struct WallTemplate
{
	shared_ptr<Texture> mHorizontal;
	shared_ptr<Texture> mVertical;
	bool mIsMetal { false };
};

// Resource database allows user to group resources and retrieve random from specified group,
// which is helpful since mostly all stuff in the game is procedurally generated.
class ResourceDatabase
{
private:
	void LoadTexturesFromFolder(const string & path, vector<shared_ptr<Texture>> & textures);
	void LoadTerrainBrushes();
	void LoadRoomDecorations();
	void LoadWallTemplates();
	void LoadRoomBackgrounds();
	void LoadTerrainDecorations();
	void LoadBodyParts();
	void LoadWeaponModelTemplates();

	template<typename K, typename V>
	using GroupMap = unordered_map<K, vector<unique_ptr<V>>>;

	GroupMap<TerrainBrushClass, TerrainBrush> mTerrainBrushList;
	GroupMap<DecorationTemplateType, DecorationTemplate> mDecorationTemplateList;
	GroupMap<BodyPartTemplateType, BodyPartTemplate> mBodyPartTemplateList;
	GroupMap<WallTemplateType, WallTemplate> mWallTemplateList;
	GroupMap<WeaponModelTemplateType, WeaponModelTemplate> mWeaponModelTemplateList;
	GroupMap<TerrainDecorationType, TerrainDecoration> mTerrainDecorationList;
	GroupMap<RoomBackgroundType, RoomBackground> mRoomBackgroundList;
public:
	ResourceDatabase();
		
	void GetTerrainBrushes(vector<TerrainBrush*> & brushes);

	TerrainBrush* GetRandom(const TerrainBrushClass & type);
	DecorationTemplate* GetRandom(const DecorationTemplateType & type);
	BodyPartTemplate* GetRandom(const BodyPartTemplateType & type);
	WallTemplate* GetRandom(const WallTemplateType & type);
	WeaponModelTemplate* GetRandom(const WeaponModelTemplateType & type);
	TerrainDecoration* GetRandom(const TerrainDecorationType & type);
	RoomBackground* GetRandom(const RoomBackgroundType & type);
};
