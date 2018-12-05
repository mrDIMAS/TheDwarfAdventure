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

#include "Game.h"
#include "Serializer.h"
#include "Texture.h"
#include "NodeType.h"

class SceneNode;
class Sprite;
class Text;
class Light;
class PolyLine;
class ParticleSystem;
class Bar;
class Terrain;
class SimulationIsland;
class WaterBlob;
class Animation;

class BackgroundLayer
{
private:
	float mDepth { 0 };
	Vec2f mTexCoordPosition;
	shared_ptr<Texture> mTexture;
	bool mFullscreen { false };
public:
	BackgroundLayer();
	BackgroundLayer(const shared_ptr<Texture> &tex, float depth);
	void SetDepth(float d);
	float GetDepth() const;
	void SetTexture(const shared_ptr<Texture> &tex);
	shared_ptr<Texture> GetTexture() const;
	void OffsetTexCoords(const Vec2f &offset);
	Vec2f GetTexCoordPosition() const;
	void Serialize(Serializer &sav);
	void SetFullscreen(bool state);
	bool IsFullscreen() const;
};

enum class SceneRenderingMode
{
	// Used by game mode
	ToBackBuffer,
	// Used by editor to embed scene into ImGui window
	ToTexture
};

class Scene : public enable_shared_from_this<Scene>
{
public:
	bool mNeedSortRenderOrder { true };
	static constexpr int TopmostLayer = INT_MAX;
	static constexpr int LowermostLayer = INT_MIN;
	Scene();
	void CreateIsland();
	virtual ~Scene();
	shared_ptr<SceneNode> CreateNode();
	shared_ptr<SceneNode> CreateNode(const NodeType& type);
	const vector<shared_ptr<SceneNode>> &GetNodeList() const;
	shared_ptr<Sprite> CreateSprite();
	const vector<shared_ptr<Sprite>> &GetSpriteList() const;
	virtual shared_ptr<Text> CreateText();
	const vector<shared_ptr<Text>> &GetTextList() const;
	shared_ptr<Light> CreateLight();
	const vector<shared_ptr<Light>> &GetLightList() const;
	shared_ptr<PolyLine> CreatePolyLine();
	const vector<shared_ptr<PolyLine>> &GetPolyLineList() const;
	shared_ptr<ParticleSystem> CreateParticleSystem();
	const vector<shared_ptr<ParticleSystem>> &GetParticleSystemList() const;
	void AddWaterBlob(const shared_ptr<WaterBlob> &blob);
	void RemoveWaterBlob(const shared_ptr<WaterBlob> &blob);
	vector<shared_ptr<WaterBlob>> &GetWaterBlobList();
	void AddAnimation(const shared_ptr<Animation> &animation);
	void RemoveAnimation(const shared_ptr<Animation> &animation);
	vector<shared_ptr<Animation>> &GetAnimationList();
	void AddNode(const shared_ptr<SceneNode> &n);
	// you should call this method when obtained node pointer (through CreateXXX)
	// is not needed anymore
	vector<shared_ptr<SceneNode>>::iterator RemoveNode(const shared_ptr<SceneNode> &node);
	void Update();
	sf::View &GetView();
	Vec2f GetViewVelocity() const;
	Vec2f GetMousePosition();
	sf::Color GetAmbientColor() const;
	void SetAmbientColor(const sf::Color &color);
	int GetLayer() const;
	void SetLayer(int layer);
	void SetVisible(bool vis);
	bool IsVisible() const;
	void SetLightingEnabled(bool state);
	bool IsLightingEnabled() const;
	virtual void Serialize(Serializer &sav);
	shared_ptr<Terrain> &GetTerrain();
	shared_ptr<Terrain> CreateTerrain(const sf::Vector2i &size, int backStackEntrySize);
	void DeleteTerrain();
	vector<shared_ptr<BackgroundLayer>> &GetBackgroundLayers();
	Vec2f TransformToVirtualScreen(const Vec2f &point);
	// each scene has its own separate simulation island for physics
	const shared_ptr<SimulationIsland> &GetSimulationIsland() const;
	// transfers node from previous scene to this
	// also transfers physical body of the node to simulation island of this scene
	// transfers node from previous scene to this
	void AcceptNode(const std::shared_ptr<SceneNode> &node);
	void Clear();
	// will copy all nodes from 'other' scene to this
	// removes 'other' scene from scene list of the game
	void Merge(const shared_ptr<Scene> &other);
	// looks up for node with name 'name'
	// returns nullptr if no such node present
	std::shared_ptr<SceneNode> Find(const string &name);
	// looks up for node with name 'name'
	// throws runtime error if no such node present
	template<typename T>
	shared_ptr<T> FindStrict(const string &name)
	{
		for (auto &node : mNodeList)
		{
			if (node->GetName() == name)
			{
				return dynamic_pointer_cast<T>(node);
			}
		}
		throw Exception("Unable to find node with name " + name);
	}
	void UpdateAnimations();
	void UpdateTransforms();
	// apply scaling directly to properties
	// so for sprite scale will be applied to mSize
	// for particle system - to bounds
	// and so on - actual scaling of transforms will not be changed
	// animation keyframes will be changed too
	//
	// method is useful for character prefabs
	void ScaleToFit(float newWidth, float newHeight);
	void UniformAxisScaleX(float newWidth);
	void UniformAxisScaleY(float newHeight);
	void GetBounds(Vec2f &min, Vec2f &max);
	void SortSpritesByRenderOrder();
	void ChangeRenderOrder(const shared_ptr<Sprite> &sprite, int value);
	shared_ptr<Animation> FindAnimation(const string &str) const;
	shared_ptr<Animation> FindAnimationStrict(const string &str) const;
	shared_ptr<Sound> EmitSound(const string &buf, const shared_ptr<SceneNode> &node, float volume = 100);
	shared_ptr<Sound> EmitSound(const string &buf, const Vec2f &pos, float volume = 100);

	void PostUpdate();
	const vector<shared_ptr<Sound>> &GetSoundList() const;

	shared_ptr<Texture> GetFrame() const;
	void SetFrameSize(const Vec2f & size);
	Vec2f GetFrameSize() const;
	void SetRenderingMode(SceneRenderingMode mode);
	SceneRenderingMode GetRenderingMode() const;
private:
	int mLayer { 0 };
	shared_ptr<SimulationIsland> mSimulationIsland;
	vector<shared_ptr<SceneNode>> mNodeList;
	vector<shared_ptr<Sprite>> mSpriteList;
	vector<shared_ptr<Text>> mTextList;
	vector<shared_ptr<Light>> mLightList;
	vector<shared_ptr<PolyLine>> mPolyLineList;
	vector<shared_ptr<ParticleSystem>> mParticleSystemList;
	vector<shared_ptr<WaterBlob>> mWaterBlobList;
	vector<shared_ptr<BackgroundLayer>> mBackgroundLayerList;
	vector<shared_ptr<Animation>> mAnimationList;
	vector<shared_ptr<Sound>> mSoundList;
	// Texture in which this scene will be rendered
	shared_ptr<Texture> mFrame;
	Vec2f mFrameSize;
	sf::View mView;
	Vec2f mLastViewPosition;
	Vec2f mViewVelocity;
	sf::Color mAmbientColor { 75, 75, 75 };
	SceneRenderingMode mRenderingMode { SceneRenderingMode::ToBackBuffer };
	bool mVisible { true };
	bool mLightingEnabled { true };
	shared_ptr<Terrain> mTerrain;
	void InternalScale(float sx, float sy);
	void UpdateSounds();
};
