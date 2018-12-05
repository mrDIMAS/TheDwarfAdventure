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

#include "Collision.h"
#include "Serializer.h"
#include "Texture.h"
#include "Keyframe.h"
#include "NodeType.h"

class Scene;
class Animation;

class SceneNode : public enable_shared_from_this<SceneNode>
{
private:
	bool GetCombinedVisibility() const;

protected:
	// ************************************************************************
	// WARNING: WHEN ADDING NEW FIELD, DO NOT FORGET TO ADD IT TO 'MakeCopy'  *
	// ************************************************************************
	Vec2f mLocalOrigin;
	Vec2f mLocalPosition;
	Vec2f mLocalScale { 1, 1 };
	sf::Transform mLocalTransform;
	sf::Transform mGlobalTransform;
	Vec2f mSize;
	float mRotationLimitMin { -180 };
	float mRotationLimitMax { 180 };
	float mLocalRotation { 0 };
	weak_ptr<Scene> mScene;
	weak_ptr<SceneNode> mParent;
	vector<shared_ptr<SceneNode>> mChildren;
	shared_ptr<Body> mBody;
	string mName;
	bool mVisible { true };      // local visibility
	bool mTrulyVisible { true }; // visibility combined with parent visibility
	bool mEditable { true };
	// life time is useful for temporary objects, like explosion sprite and other
	// effects that lasts few moments on screen < 0 means infinite life time, time
	// set in frames: 60 frames = 1 sec in this game
	int mLifeTime { -1 };
	float ClampAngle(float angle) const;
	bool mNeedUpdateTransform { true };

public:
	SceneNode();
	SceneNode(const shared_ptr<Scene> &scene);
	virtual ~SceneNode();
	void AttachTo(const shared_ptr<SceneNode> &newParent);
	void Detach();
	// returns global transform
	sf::Transform &CalculateTransforms();
	// sets body and positions it at current node's global position
	void SetBodyAtCurrentPosition(const shared_ptr<Body> &body);
	void SetBody(const shared_ptr<Body> & body);
	shared_ptr<Body> &GetBody();
	sf::Transform &GetGlobalTransform();
	virtual bool IsInView(const sf::View &view);
	Vec2f GetGlobalPosition();
	void SetVisible(bool visible);
	bool IsVisible() const;
	shared_ptr<Scene> GetScene() const;
	virtual Vec2f GetSize() const;
	void SetEditingEnabled(bool state);
	bool IsEditingEnabled();
	///////////////////////////////////////////////////////////////////////////////
	// used for rendering
	// rely on last Update call
	bool IsTrulyVisible() const;
	// removes node from scene, if node has only 1 ref, it will be deleted
	void RemoveFromScene();
	virtual void Serialize(Serializer &sav);
	void RemoveBody();
	// set life time in frames, this value will be decreased each frame and when
	// it becomes == 0, scene will automatically destroy object
	// WARNING: do not store strong reference to this object if you want
	// autodeletion work
	void SetLifeTime(int lifeTime);
	int GetLifeTime() const;
	void Update();
	bool IsDead() const;
	virtual void SetSize(float w, float h);
	virtual void SetSize(const Vec2f &s);
	void SetLocalPosition(const Vec2f &p);
	void SetLocalPosition(float x, float y);
	void SetGlobalPosition(const Vec2f &p);
	Vec2f GetLocalPosition() const;
	void SetLocalOrigin(const Vec2f &origin);
	void SetLocalOrigin(float x, float y);
	Vec2f GetLocalOrigin() const;
	void SetLocalScale(const Vec2f &scale);
	void SetLocalScale(float x, float y);
	void SetHorizontalScale(float s);
	void SetVerticalScale(float s);
	Vec2f GetLocalScale() const;
	void SetLocalRotation(float angle);
	float GetLocalRotation() const;
	void Turn(float speed);
	void LocalMove(const Vec2f &offset);
	void LocalMove(float x, float y);
	// for internal use only!
	// use Scene::Transfer to change scene of the node
	void SetScene(const shared_ptr<Scene> &newscene);
	vector<shared_ptr<SceneNode>> &GetChildren();
	shared_ptr<SceneNode> GetParent() const;
	void SetName(const string &name);
	const string &GetName() const;
	shared_ptr<SceneNode> MakeCopy() const;
	// note: this method will NOT reset origin of the node
	// for internal use only!
	void ResetLocalTransform();
	void AddLocalPosition(const Vec2f &p);
	void AddLocalScale(const Vec2f &s);
	void AddLocalRotation(float r);
	void SetMinRotationLimit(float limit);
	void SetMaxRotationLimit(float limit);
	void ExtractAllChildren(vector<shared_ptr<SceneNode>> &outNodes);
	bool HasParent() const;
	virtual NodeType GetType() const;
	// Returns true if child is in hierarchy 	
	bool ContainsChild(const shared_ptr<SceneNode>& node) const;
};
