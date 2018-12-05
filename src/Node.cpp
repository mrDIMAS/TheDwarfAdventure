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
#include "Node.h"
#include "Scene.h"
#include "Animation.h"

bool SceneNode::GetCombinedVisibility() const
{
	bool v = mVisible;
	if (!mParent.expired())
	{
		v = v && mParent.lock()->GetCombinedVisibility();
	}
	return v;
}

float SceneNode::ClampAngle(float angle) const
{
	return Math::Clamp(Math::SimplifyAngle(angle), mRotationLimitMin, mRotationLimitMax);
}

SceneNode::SceneNode()
{
}

SceneNode::SceneNode(const shared_ptr<Scene> &scene) : mScene(scene)
{
	mName = StringBuilder("UnnamedNode") << scene->GetNodeList().size();
}

SceneNode::~SceneNode()
{
	RemoveBody();
	DBG_ECHO();
}

void SceneNode::AttachTo(const shared_ptr<SceneNode> &newParent)
{
	if (newParent == shared_from_this())
	{
		return;
	}

	if (newParent)
	{
		if (mParent.lock() != newParent)
		{
			Detach();
			mParent = newParent;
			newParent->mChildren.push_back(shared_from_this());
			mNeedUpdateTransform = true;
		}
	}
}

void SceneNode::Detach()
{
	if (auto currentParent = mParent.lock())
	{
		currentParent->mChildren.erase(
			find(currentParent->mChildren.begin(), currentParent->mChildren.end(), shared_from_this()));
	}
	mParent.reset();
	mNeedUpdateTransform = true;
}

sf::Transform &SceneNode::CalculateTransforms()
{
	if (mNeedUpdateTransform || mBody)
	{
		if (mBody)
		{
			mLocalPosition = mBody->GetPosition();
		}

		mLocalTransform = Math::ComposeTransform(mLocalPosition, mLocalOrigin, mLocalScale, mLocalRotation);

		if (mParent.expired())
		{
			mGlobalTransform = mLocalTransform;
		}
		else
		{
			mGlobalTransform = mParent.lock()->GetGlobalTransform() * mLocalTransform;
		}

		mNeedUpdateTransform = false;

		for (auto &child : mChildren)
		{
			child->mNeedUpdateTransform = true;
			child->CalculateTransforms();
		}
	}

	return mGlobalTransform;
}

void SceneNode::SetBodyAtCurrentPosition(const shared_ptr<Body> &body)
{
	CalculateTransforms();
	body->SetPosition(GetGlobalPosition());
	mBody = body;
}

void SceneNode::SetBody(const shared_ptr<Body> & body)
{
	mBody = body;
}

shared_ptr<Body> &SceneNode::GetBody()
{
	return mBody;
}

sf::Transform &SceneNode::GetGlobalTransform()
{
	if (mNeedUpdateTransform)
	{
		return CalculateTransforms();
	}
	return mGlobalTransform;
}

bool SceneNode::IsInView(const sf::View &view)
{
	UNUSED(view);
	return true;
}

Vec2f SceneNode::GetGlobalPosition()
{
	if (mNeedUpdateTransform)
	{
		CalculateTransforms();
	}
	const auto m = mGlobalTransform.getMatrix();
	return { m[12], m[13] };
}

void SceneNode::SetVisible(bool visible)
{
	mVisible = visible;
}

bool SceneNode::IsVisible() const
{
	return mVisible;
}

shared_ptr<Scene> SceneNode::GetScene() const
{
	return mScene.lock();
}

Vec2f SceneNode::GetSize() const
{
	return mSize;
}

void SceneNode::SetEditingEnabled(bool state)
{
	mEditable = state;
}

bool SceneNode::IsEditingEnabled()
{
	return mEditable;
}

bool SceneNode::IsTrulyVisible() const
{
	return mTrulyVisible;
}

void SceneNode::RemoveFromScene()
{
	if (auto scene = mScene.lock())
	{
		scene->RemoveNode(shared_from_this());
	}
}

void SceneNode::Serialize(Serializer &sav)
{
	sav &mLocalPosition;
	sav &mLocalOrigin;
	sav &mLocalScale;
	sav &mLocalRotation;
	sav &mVisible;
	sav &mEditable;
	sav &mLifeTime;
	sav &mScene;
	sav &mParent;
	sav &mChildren;
	sav &mBody;
	sav &mName;
	sav &mSize;
	sav &mRotationLimitMin;
	sav &mRotationLimitMax;
}

void SceneNode::RemoveBody()
{
	if (mBody)
	{
		mBody->RemoveFromWorld();
		mLocalPosition = mBody->GetPosition();
		mBody.reset();
	}
}

void SceneNode::SetLifeTime(int lifeTime)
{
	mLifeTime = lifeTime;
	for (auto &child : mChildren)
	{
		child->SetLifeTime(lifeTime);
	}
}

int SceneNode::GetLifeTime() const
{
	return mLifeTime;
}

void SceneNode::Update()
{
	mTrulyVisible = GetCombinedVisibility();
	if (mLifeTime > 0)
	{
		--mLifeTime;
	}
}

bool SceneNode::IsDead() const
{
	// < 0 is infinite time
	return mLifeTime == 0;
}

void SceneNode::SetSize(float w, float h)
{
	mSize.x = w;
	mSize.y = h;
}

void SceneNode::SetSize(const Vec2f &s)
{
	mSize = s;
}

void SceneNode::SetLocalPosition(const Vec2f &p)
{
	if (mBody)
	{
		mBody->SetPosition(p);
	}
	else
	{
		mLocalPosition = p;
	}
	mNeedUpdateTransform = true;
}

void SceneNode::SetLocalPosition(float x, float y)
{
	mLocalPosition.x = x;
	mLocalPosition.y = y;
	mNeedUpdateTransform = true;
}

void SceneNode::SetGlobalPosition(const Vec2f &p)
{
	if (mParent.expired())
	{
		SetLocalPosition(p);
	}
	else
	{
		auto parent = mParent.lock();
		auto parentPosition = parent->GetGlobalPosition();
		auto d = p - parentPosition;
		SetLocalPosition(d);
	}
}

Vec2f SceneNode::GetLocalPosition() const
{
	return mLocalPosition;
}

void SceneNode::SetLocalOrigin(const Vec2f &origin)
{
	mLocalOrigin = origin;
	mNeedUpdateTransform = true;
}

void SceneNode::SetLocalOrigin(float x, float y)
{
	mLocalOrigin.x = x;
	mLocalOrigin.y = y;
	mNeedUpdateTransform = true;
}

Vec2f SceneNode::GetLocalOrigin() const
{
	return mLocalOrigin;
}

void SceneNode::SetLocalScale(const Vec2f &scale)
{
	mLocalScale = scale;
	mNeedUpdateTransform = true;
}

void SceneNode::SetLocalScale(float x, float y)
{
	mLocalScale.x = x;
	mLocalScale.y = y;
	mNeedUpdateTransform = true;
}

void SceneNode::SetHorizontalScale(float s)
{
	SetLocalScale(s, mLocalScale.y);
}

void SceneNode::SetVerticalScale(float s)
{
	SetLocalScale(mLocalScale.x, s);
}

Vec2f SceneNode::GetLocalScale() const
{
	return mLocalScale;
}

void SceneNode::SetLocalRotation(float angle)
{
	mLocalRotation = ClampAngle(angle);
	mNeedUpdateTransform = true;
}

float SceneNode::GetLocalRotation() const
{
	return mLocalRotation;
}

void SceneNode::Turn(float speed)
{
	mLocalRotation += speed;
}

void SceneNode::LocalMove(const Vec2f &offset)
{
	mLocalPosition += offset;
	mNeedUpdateTransform = true;
}

void SceneNode::LocalMove(float x, float y)
{
	mLocalPosition.x += x;
	mLocalPosition.y += y;
	mNeedUpdateTransform = true;
}

void SceneNode::SetScene(const shared_ptr<Scene> &newscene)
{
	mScene = newscene;
}

vector<shared_ptr<SceneNode>> &SceneNode::GetChildren()
{
	return mChildren;
}

shared_ptr<SceneNode> SceneNode::GetParent() const
{
	return mParent.lock();
}

void SceneNode::SetName(const string &name)
{
	mName = name;
}

const string &SceneNode::GetName() const
{
	return mName;
}

shared_ptr<SceneNode> SceneNode::MakeCopy() const
{
	shared_ptr<SceneNode> copy = make_shared<SceneNode>();
	copy->mLocalOrigin = mLocalOrigin;
	copy->mLocalPosition = mLocalPosition;
	copy->mLocalScale = mLocalScale;
	copy->mLocalTransform = mLocalTransform;
	copy->mGlobalTransform = mGlobalTransform;
	copy->mSize = mSize;
	copy->mLocalRotation = mLocalRotation;
	copy->mScene = mScene;
	copy->mName = mName;
	copy->mVisible = mVisible;
	copy->mTrulyVisible = mTrulyVisible;
	copy->mEditable = mEditable;
	copy->mLifeTime = mLifeTime;
	copy->mRotationLimitMin = mRotationLimitMin;
	copy->mRotationLimitMax = mRotationLimitMax;
	GetScene()->AddNode(copy);
	return copy;
}

void SceneNode::ResetLocalTransform()
{
	mLocalPosition = { 0, 0 };
	mLocalScale = { 1, 1 };
	mLocalRotation = 0.0f;
	mNeedUpdateTransform = true;
}

void SceneNode::AddLocalPosition(const Vec2f &p)
{
	mLocalPosition += p;
	mNeedUpdateTransform = true;
}

void SceneNode::AddLocalScale(const Vec2f &s)
{
	mLocalScale.x *= s.x;
	mLocalScale.y *= s.y;
	mNeedUpdateTransform = true;
}

void SceneNode::AddLocalRotation(float r)
{
	mLocalRotation = ClampAngle(mLocalRotation + r);
	mNeedUpdateTransform = true;
}

void SceneNode::SetMinRotationLimit(float limit)
{
	mRotationLimitMin = Math::SimplifyAngle(limit);
	if (mRotationLimitMin > mRotationLimitMax)
	{
		swap(mRotationLimitMin, mRotationLimitMax);
	}
}

void SceneNode::SetMaxRotationLimit(float limit)
{
	mRotationLimitMax = Math::SimplifyAngle(limit);
	if (mRotationLimitMax < mRotationLimitMin)
	{
		swap(mRotationLimitMin, mRotationLimitMax);
	}
}

void SceneNode::ExtractAllChildren(vector<shared_ptr<SceneNode>> &outNodes)
{
	outNodes.push_back(shared_from_this());
	for (auto &child : mChildren)
	{
		child->ExtractAllChildren(outNodes);
	}
}

bool SceneNode::HasParent() const
{
	return !mParent.expired();
}

NodeType SceneNode::GetType() const
{
	return NodeType::Node;
}

bool SceneNode::ContainsChild(const shared_ptr<SceneNode>& node) const
{
	bool result = false;

	for (const shared_ptr<SceneNode>& child : mChildren)
	{
		if (child == node)
		{
			result |= true;

			break;
		}
		else if (child->ContainsChild(node))
		{
			result |= true;

			break;
		}
	}

	return result;
}
