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
#include "Renderer.h"
#include "Light.h"
#include "Text.h"
#include "Sprite.h"
#include "Scene.h"
#include "PolyLine.h"
#include "ParticleSystem.h"
#include "Game.h"
#include "Terrain.h"
#include "Collision.h"
#include "WaterBlob.h"
#include "Animation.h"
#include "AnimationTrack.h"
#include "Sound.h"
#include "Keyframe.h"
#include "AttributeGrid.h"

template <class T> void FindAndOffset(vector<T> &v, const T &item, int offset)
{
	int pos = -1;
	for (auto &i : v)
	{
		++pos;
		if (i == item)
		{
			break;
		}
	}
	int absOffset = abs(offset);
	if (pos >= 0)
	{
		auto iter = v.begin() + pos;
		if (offset < 0)
		{
			if (pos - absOffset > 0)
			{
				iter_swap(iter, iter - absOffset);
			}
			else
			{
				iter_swap(iter, v.begin());
			}
		}
		if (offset > 0)
		{
			if (static_cast<unsigned int>(pos + absOffset) < v.size() - 1)
			{
				iter_swap(iter, iter + absOffset);
			}
			else
			{
				iter_swap(iter, v.end() - 1);
			}
		}
	}
}

void Scene::AddNode(const shared_ptr<SceneNode> &n)
{
	mNodeList.push_back(n);
	if (auto sprite = dynamic_pointer_cast<Sprite>(n))
	{
		mSpriteList.push_back(sprite);
	}
	else if (auto ps = dynamic_pointer_cast<ParticleSystem>(n))
	{
		mParticleSystemList.push_back(ps);
	}
	else if (auto light = dynamic_pointer_cast<Light>(n))
	{
		mLightList.push_back(light);
	}
	else if (auto polyline = dynamic_pointer_cast<PolyLine>(n))
	{
		mPolyLineList.push_back(polyline);
	}
	else if (auto text = dynamic_pointer_cast<Text>(n))
	{
		mTextList.push_back(text);
	}
}

void Scene::InternalScale(float sx, float sy)
{
	for (auto &node : mNodeList)
	{
		node->SetSize(node->GetSize().x * sx, node->GetSize().y * sy);
		node->SetLocalPosition(node->GetLocalPosition().x * sx, node->GetLocalPosition().y * sy);
		node->SetLocalOrigin(node->GetLocalOrigin().x * sx, node->GetLocalOrigin().y * sy);
	}

	for (auto &anim : mAnimationList)
	{
		for (auto &track : anim->GetTrackList())
		{
			for (auto &keyframe : track->GetKeyframeList())
			{
				keyframe.SetPosition({ keyframe.GetPosition().x * sx, keyframe.GetPosition().y * sy });
			}
		}
	}
}

Scene::Scene() : mFrame(make_shared<Texture>())
{
	mView.setSize(game->VirtualScreen.getSize());
	mView.setCenter(game->VirtualScreen.getSize() / 2.0f);
	mFrameSize = game->VirtualScreen.getSize();
}

void Scene::CreateIsland()
{
	mSimulationIsland = game->GetPhysics()->CreateIsland();
}

Scene::~Scene()
{
	DBG_ECHO();
}

shared_ptr<SceneNode> Scene::CreateNode()
{
	auto node = make_shared<SceneNode>(shared_from_this());
	mNodeList.push_back(node);
	return node;
}

shared_ptr<SceneNode> Scene::CreateNode(const NodeType & type)
{
	switch (type)
	{
	case NodeType::Node:
		return CreateNode();
	case NodeType::Sprite:
		return CreateSprite();
	case NodeType::PolyLine:
		return CreatePolyLine();
	case NodeType::Text:
		return CreateText();
	case NodeType::ParticleSystem:
		return CreateParticleSystem();
	case NodeType::Light:
		return CreateLight();
	default:
		throw Exception("Unable to create a node by enumeration! Unknown value!");
		break;
	}
}

shared_ptr<Sprite> Scene::CreateSprite()
{
	auto node = make_shared<Sprite>(shared_from_this());
	mNodeList.push_back(node);
	mSpriteList.push_back(node);
	return node;
}

shared_ptr<Text> Scene::CreateText()
{
	auto node = make_shared<Text>(shared_from_this());
	mNodeList.push_back(node);
	mTextList.push_back(node);
	return node;
}

shared_ptr<Light> Scene::CreateLight()
{
	auto node = make_shared<Light>(shared_from_this());
	mNodeList.push_back(node);
	mLightList.push_back(node);
	return node;
}

shared_ptr<PolyLine> Scene::CreatePolyLine()
{
	auto pl = make_shared<PolyLine>(shared_from_this());
	mNodeList.push_back(pl);
	mPolyLineList.push_back(pl);
	return pl;
}

shared_ptr<ParticleSystem> Scene::CreateParticleSystem()
{
	auto ps = make_shared<ParticleSystem>(shared_from_this());
	mNodeList.push_back(ps);
	mParticleSystemList.push_back(ps);
	return ps;
}

const vector<shared_ptr<SceneNode>> &Scene::GetNodeList() const
{
	return mNodeList;
}

const vector<shared_ptr<Sprite>> & Scene::GetSpriteList() const
{
	return mSpriteList;
}

const vector<shared_ptr<Text>> &Scene::GetTextList() const
{
	return mTextList;
}

const vector<shared_ptr<Light>> &Scene::GetLightList() const
{
	return mLightList;
}

const vector<shared_ptr<PolyLine>> &Scene::GetPolyLineList() const
{
	return mPolyLineList;
}

const vector<shared_ptr<ParticleSystem>> & Scene::GetParticleSystemList() const
{
	return mParticleSystemList;
}

void Scene::AddWaterBlob(const shared_ptr<WaterBlob> &blob)
{
	mWaterBlobList.push_back(blob);
}

void Scene::RemoveWaterBlob(const shared_ptr<WaterBlob> &blob)
{
	mWaterBlobList.erase(
		std::remove(mWaterBlobList.begin(), mWaterBlobList.end(), blob), mWaterBlobList.end());
}

void Scene::AddAnimation(const shared_ptr<Animation> &animation)
{
	auto iter = find(mAnimationList.begin(), mAnimationList.end(), animation);
	if (iter == mAnimationList.end())
	{
		mAnimationList.push_back(animation);
	}
}

void Scene::RemoveAnimation(const shared_ptr<Animation> &animation)
{
	mAnimationList.erase(
		std::remove(mAnimationList.begin(), mAnimationList.end(), animation), mAnimationList.end());
}

vector<shared_ptr<Animation>> &Scene::GetAnimationList()
{
	return mAnimationList;
}

vector<shared_ptr<WaterBlob>> &Scene::GetWaterBlobList()
{
	return mWaterBlobList;
}

void Scene::Update()
{
	if (mVisible)
	{
		UpdateAnimations();
		UpdateTransforms();
		for (const auto &node : mNodeList)
		{
			node->Update();
		}

		for (const auto &spr : mSpriteList)
		{
			spr->UpdateFrameAnimation();
		}

		for (const auto &ps : mParticleSystemList)
		{
			ps->Update();
		}

		for (const auto &light : mLightList)
		{
			light->Update();
		}

		for (const auto &blob : mWaterBlobList)
		{
			blob->Update();
		}

		// handle autodeletion
		for (auto nodeIter = mNodeList.begin(); nodeIter != mNodeList.end();)
		{
			const auto & node = *nodeIter;
			if (node->IsDead())
			{
				nodeIter = RemoveNode(node);
			}
			else
			{
				++nodeIter;
			}
		}

		if (mTerrain)
		{
			mTerrain->Update();
		}

		mViewVelocity = mView.getCenter() - mLastViewPosition;
		mLastViewPosition = mView.getCenter();

		mViewVelocity.y = 0.0f;

		// update parallax background
		for (auto &bl : mBackgroundLayerList)
		{
			bl->OffsetTexCoords(0.0005f * mViewVelocity / bl->GetDepth());
		}
	}
}

sf::View &Scene::GetView()
{
	return mView;
}

Vec2f Scene::GetViewVelocity() const
{
	return mViewVelocity;
}

Vec2f Scene::GetMousePosition()
{
	auto &window = game->GetRenderer()->GetWindow();
	return window.mapPixelToCoords(sf::Mouse::getPosition(window), mView);
}

sf::Color Scene::GetAmbientColor() const
{
	return mAmbientColor;
}

void Scene::SetAmbientColor(const sf::Color &color)
{
	mAmbientColor = color;
}

int Scene::GetLayer() const
{
	return mLayer;
}

void Scene::SetLayer(int layer)
{
	mLayer = layer;
	game->SortScenes();
}

void Scene::SetVisible(bool vis)
{
	mVisible = vis;
}

bool Scene::IsVisible() const
{
	return mVisible;
}

void Scene::SetLightingEnabled(bool state)
{
	mLightingEnabled = state;
}

bool Scene::IsLightingEnabled() const
{
	return mLightingEnabled;
}

template<typename T, typename P>
typename vector<T>::iterator RemoveByPointer(vector<T> & vec, P ptr)
{
	typename vector<T>::iterator result;

	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (vec[i].get() == ptr)
		{
			result = vec.erase(vec.begin() + i);

			break;
		}
	}

	return result;
}

vector<shared_ptr<SceneNode>>::iterator Scene::RemoveNode(const shared_ptr<SceneNode> &node, bool removeAnimTracks)
{
	if (removeAnimTracks)
	{
		for (const shared_ptr<Animation>& anim : mAnimationList)
		{
			anim->RemoveTrack(anim->FindAssociatedTrack(node));
		}
	}

	switch (node->GetType())
	{
	case NodeType::Sprite:
		RemoveByPointer(mSpriteList, node.get());
		break;
	case NodeType::Text:
		RemoveByPointer(mTextList, node.get());
		break;
	case NodeType::Light:
		RemoveByPointer(mLightList, node.get());
		break;
	case NodeType::ParticleSystem:
		RemoveByPointer(mParticleSystemList, node.get());
		break;
	case NodeType::PolyLine:
		RemoveByPointer(mPolyLineList, node.get());
		break;
    case NodeType::Node:
        // Make gcc happy
        break;
	}

	for (const shared_ptr<SceneNode> & child : node->GetChildren())
	{
		RemoveNode(child, removeAnimTracks);
	}

	mNeedSortRenderOrder = true;

	return RemoveByPointer(mNodeList, node.get());
}

void Scene::Serialize(Serializer &sav)
{
	sav &mSimulationIsland;
	sav &mLayer;
	sav &mAmbientColor;
	sav &mVisible;
	sav &mLightingEnabled;
	sav &mNodeList;
	sav &mLightList;
	sav &mParticleSystemList;
	sav &mPolyLineList;
	sav &mTextList;
	sav &mSpriteList;
	sav &mBackgroundLayerList;
	sav &mTerrain;
	sav &mWaterBlobList;
	sav &mAnimationList;
}

shared_ptr<Terrain> &Scene::GetTerrain()
{
	return mTerrain;
}

shared_ptr<Terrain> Scene::CreateTerrain(const sf::Vector2i &size, int backStackEntrySize)
{
	lock_guard<recursive_mutex>(game->GetRenderer()->GetMutex());
	DeleteTerrain();
	mTerrain = make_shared<Terrain>(shared_from_this(), size.x, size.y, backStackEntrySize);
	mSimulationIsland->SetTerrain(mTerrain);
	return mTerrain;
}

void Scene::DeleteTerrain()
{
	mTerrain.reset();
}

vector<shared_ptr<BackgroundLayer>> &Scene::GetBackgroundLayers()
{
	sort(mBackgroundLayerList.begin(), mBackgroundLayerList.end(),
		[] (const auto &left, const auto &right) { return left->GetDepth() > right->GetDepth(); });
	return mBackgroundLayerList;
}

Vec2f Scene::TransformToVirtualScreen(const Vec2f &point)
{
	auto renderer = game->GetRenderer();
	Vec2f normalized = mView.getTransform().transformPoint(point);
	sf::IntRect viewport = renderer->GetWindow().getViewport(mView);
	return { (normalized.x + 1.f) / 2.f * viewport.width + viewport.left,
		(-normalized.y + 1.f) / 2.f * viewport.height + viewport.top };
}

const shared_ptr<SimulationIsland> &Scene::GetSimulationIsland() const
{
	return mSimulationIsland;
}


// transfers node from previous scene to this
// also transfers physical body of the node to simulation island of this scene
// transfers node from previous scene to this

void Scene::AcceptNode(const std::shared_ptr<SceneNode>& node)
{
	// save strong ref to node
	auto n = node;
	auto prevScene = n->GetScene();

	if (prevScene == shared_from_this())
	{
		return;
	}
	// remove from prev
	prevScene->RemoveNode(n, false);
	// add to this
	AddNode(n);
	n->SetScene(shared_from_this());
	// transfer body too
	if (auto body = n->GetBody())
	{
		mSimulationIsland->AcceptBody(body);
	}

	// move animations to new scene
	for (auto &anim : prevScene->GetAnimationList())
	{
		AddAnimation(anim);
	}
	prevScene->GetAnimationList().clear();

	// apply to children
	for (auto &child : n->GetChildren())
	{
		AcceptNode(child);
	}

	mNeedSortRenderOrder = true;
}

void Scene::Clear()
{
	mSimulationIsland->Clear();
	mNodeList.clear();
	mSpriteList.clear();
	mTextList.clear();
	mLightList.clear();
	mPolyLineList.clear();
	mParticleSystemList.clear();
	mWaterBlobList.clear();
	mBackgroundLayerList.clear();
	mTerrain.reset();
}

void Scene::Merge(const shared_ptr<Scene> &other)
{
	lock_guard<recursive_mutex> guard(game->GetRenderer()->GetMutex());

	for (auto &node : other->mNodeList)
	{
		node->SetScene(shared_from_this());
		mNodeList.push_back(node);
	}
	for (auto &sprite : other->mSpriteList)
	{
		mSpriteList.push_back(sprite);
	}
	for (auto &light : other->mLightList)
	{
		mLightList.push_back(light);
	}
	for (auto &ps : other->mParticleSystemList)
	{
		mParticleSystemList.push_back(ps);
	}
	for (auto &pl : other->mPolyLineList)
	{
		mPolyLineList.push_back(pl);
	}
	for (auto &blob : other->mWaterBlobList)
	{
		AddWaterBlob(blob);
	}
	for (auto &anim : other->mAnimationList)
	{
		anim->SetEnabled(false);
		AddAnimation(anim);
	}

	mNeedSortRenderOrder = true;

	game->RemoveScene(other);
}

// looks up for node with name 'name'
// returns nullptr if no such node present

std::shared_ptr<SceneNode> Scene::Find(const string & name)
{
	for (auto &node : mNodeList)
	{
		if (node->GetName() == name)
		{
			return node;
		}
	}
	return nullptr;
}

void Scene::UpdateAnimations()
{
	// for animation blending, we have to set node's transform to identity before
	// animate it
	for (auto &animation : mAnimationList)
	{
		if (animation->IsEnabled())
		{
			animation->ResetAssociatedNodesTransform();
		}
	}
	// now update animation
	for (auto &animation : mAnimationList)
	{
		if (animation->IsEnabled())
		{
			animation->Update();
		}
	}
}

void Scene::UpdateTransforms()
{
	for (auto &node : mNodeList)
	{
		node->CalculateTransforms();
	}
}

void Scene::ScaleToFit(float newWidth, float newHeight)
{
	Vec2f min, max;
	GetBounds(min, max);

	float w = (max.x - min.x);
	float h = (max.y - min.y);

	InternalScale(newWidth / w, newHeight / h);
}

void Scene::UniformAxisScaleX(float newWidth)
{
	Vec2f min, max;
	GetBounds(min, max);

	float w = (max.x - min.x);

	InternalScale(newWidth / w, newWidth / w);
}

void Scene::UniformAxisScaleY(float newHeight)
{
	Vec2f min, max;
	GetBounds(min, max);

	float h = (max.y - min.y);

	InternalScale(newHeight / h, newHeight / h);
}

void Scene::GetBounds(Vec2f &min, Vec2f &max)
{
	min = { FLT_MAX, FLT_MAX };
	max = { -FLT_MAX, -FLT_MAX };
	for (auto &node : mNodeList)
	{
		Math::RotatedRectangle(node->GetSize(), node->GetGlobalTransform())
			.GetAxisAlignedRectangle(min, max);
	}
}

void Scene::SortSpritesByRenderOrder()
{
	sort(mSpriteList.begin(), mSpriteList.end(), [] (const shared_ptr<Sprite> &left, const shared_ptr<Sprite> &right)
	{
		if (left && right)
		{
			return left->mRenderOrder < right->mRenderOrder;
		}
		return true;
	});
}

void Scene::ChangeRenderOrder(const shared_ptr<Sprite> &sprite, int value)
{
	FindAndOffset(mSpriteList, sprite, value);
	int n = 0;
	for (auto &otherSprite : mSpriteList)
	{
		otherSprite->mRenderOrder = n++;
	}
	SortSpritesByRenderOrder();
	mNeedSortRenderOrder = false;
}

shared_ptr<Animation> Scene::FindAnimation(const string &str) const
{
	for (auto &animation : mAnimationList)
	{
		if (animation->GetName() == str)
		{
			return animation;
		}
	}
	return nullptr;
}

shared_ptr<Animation> Scene::FindAnimationStrict(const string &str) const
{
	for (auto &animation : mAnimationList)
	{
		if (animation->GetName() == str)
		{
			return animation;
		}
	}
	throw Exception("Animation " + str + " was not found!");
}

shared_ptr<Sound> Scene::EmitSound(const string &buf, const shared_ptr<SceneNode> &node, float volume)
{
	auto snd = make_shared<Sound>(buf);
	mSoundList.push_back(snd);
	snd->AttachTo(node);

	if (mTerrain)
	{
		const AttributeGridLayer & genericFlagsLayer = mTerrain->GetAttributeGrid()->GetGenericFlagsLayer();

		if(!genericFlagsLayer.IsSet(mTerrain->AbsoluteToNormalized(node->GetGlobalPosition()), AttributeGridLayer::Flags::Visible))
		{
			snd->FadeOut(true);
		}
	}

	snd->SetVolume(volume);
	return snd;
}

shared_ptr<Sound> Scene::EmitSound(const string &buf, const Vec2f &pos, float volume)
{
	auto snd = make_shared<Sound>(buf);
	mSoundList.push_back(snd);
	snd->SetPosition(pos);
	snd->SetVolume(volume);

	if (mTerrain)
	{
		const AttributeGridLayer & genericFlagsLayer = mTerrain->GetAttributeGrid()->GetGenericFlagsLayer();

		if (!genericFlagsLayer.IsSet(mTerrain->AbsoluteToNormalized(pos), AttributeGridLayer::Flags::Visible))
		{
			snd->FadeOut(true);
		}
	}


	return snd;
}

void Scene::UpdateSounds()
{
	for (auto it = mSoundList.begin(); it != mSoundList.end();)
	{
		auto &snd = *it;
		if (snd->IsStopped())
		{
			it = mSoundList.erase(it);
		}
		else
		{			
			// Experimental - use fog of war to muffle sounds
			if (mTerrain)
			{
#if 0
				FogOfWar* fogOfWar = mTerrain->GetFogOfWar();

				FogOfWarProbeResult probeResult = fogOfWar->Probe(mTerrain->AbsoluteToNormalized(snd->GetPosition()));

				if (probeResult.mColor.a > 200)
				{
					// Muffle sound
					snd->FadeOut();
				}
#endif
				const AttributeGridLayer& genericFlagsLayer = mTerrain->GetAttributeGrid()->GetGenericFlagsLayer();

				if (!genericFlagsLayer.IsSet(mTerrain->AbsoluteToNormalized(snd->GetPosition()), AttributeGridLayer::Flags::Visible))
				{
					// Muffle sound
					snd->FadeOut();
				}
				else
				{
					snd->FadeIn();
				}
			}

			snd->Update();

			++it;
		}
	}
}

void Scene::PostUpdate()
{
	UpdateSounds();
}

const vector<shared_ptr<Sound>> &Scene::GetSoundList() const
{
	return mSoundList;
}

shared_ptr<Texture> Scene::GetFrame() const
{
	return mFrame;
}

void Scene::SetFrameSize(const Vec2f & size)
{
	mFrameSize = size;
}

Vec2f Scene::GetFrameSize() const
{
	return mFrameSize;
}

void Scene::SetRenderingMode(SceneRenderingMode mode)
{
	mRenderingMode = mode;
}

SceneRenderingMode Scene::GetRenderingMode() const
{
	return mRenderingMode;
}

BackgroundLayer::BackgroundLayer()
{
}

BackgroundLayer::BackgroundLayer(const shared_ptr<Texture> &tex, float depth)
	: mDepth(depth), mTexture(tex)
{
}

void BackgroundLayer::SetDepth(float d)
{
	mDepth = d;
}

float BackgroundLayer::GetDepth() const
{
	return mDepth;
}

void BackgroundLayer::SetTexture(const shared_ptr<Texture> &tex)
{
	mTexture = tex;
}

shared_ptr<Texture> BackgroundLayer::GetTexture() const
{
	return mTexture;
}

void BackgroundLayer::OffsetTexCoords(const Vec2f &offset)
{
	mTexCoordPosition += offset;
}

Vec2f BackgroundLayer::GetTexCoordPosition() const
{
	return mTexCoordPosition;
}

void BackgroundLayer::Serialize(Serializer &sav)
{
	sav &mDepth;
	sav &mTexture;
	sav &mFullscreen;
}

void BackgroundLayer::SetFullscreen(bool state)
{
	mFullscreen = state;
}

bool BackgroundLayer::IsFullscreen() const
{
	return mFullscreen;
}
