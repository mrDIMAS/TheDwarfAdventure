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
#include "AnimationTrack.h"
#include "Node.h"

void AnimationTrack::SortKeyframesByTime()
{
	sort(mKeyframeList.begin(), mKeyframeList.end(), [] (const auto &left, const auto &right) { return left.GetTime() < right.GetTime(); });

	mMaxTime = 0.0f;
	for (auto &keyframe : mKeyframeList)
	{
		if (keyframe.GetTime() > mMaxTime)
		{
			mMaxTime = keyframe.GetTime();
		}
	}
}

void AnimationTrack::AddKeyFrame(const Keyframe &newKeyframe)
{
	auto keyframe = newKeyframe;

	keyframe.mTrack = this;

	if (keyframe.GetTime() > mMaxTime)
	{
		mKeyframeList.push_back(keyframe);

		mMaxTime = keyframe.GetTime();
	}
	else
	{
		auto it = mKeyframeList.begin();
		for (; it != mKeyframeList.end(); ++it)
		{
			if (keyframe.GetTime() < it->GetTime())
			{
				break;
			}
		}
		mKeyframeList.insert(it, keyframe);
	}
}

AnimationTrack::AnimationTrack()
{
}

AnimationTrack::~AnimationTrack()
{
}


Keyframe AnimationTrack::GetKeyframe(float time)
{
	Keyframe *left = nullptr;
	Keyframe *right = nullptr;
	float interpolator = 0.0f;

	time = Math::Clamp(time, 0.0f, mMaxTime);

	if (time >= mMaxTime)
	{
		left = &mKeyframeList.back();
		right = left;

		interpolator = 0.0f;
	}
	else
	{
		int rightIndex = -1;
		for (size_t i = 0; i < mKeyframeList.size(); ++i)
		{
			if (mKeyframeList[i].GetTime() >= time)
			{
				rightIndex = i;
				break;
			}
		}

		if (rightIndex == 0)
		{
			left = &mKeyframeList.front();
			right = left;

			interpolator = 0.0f;
		}
		else
		{
			left = &mKeyframeList[rightIndex - 1];
			right = &mKeyframeList[rightIndex];

			interpolator = (time - left->GetTime()) / (right->GetTime() - left->GetTime());
		}
	}

	if (left && right)
	{
		if (interpolator == 0.0f)
		{
			return *left;
		}
		else
		{
			Keyframe out;
			out.SetTime(Math::Lerp(left->GetTime(), right->GetTime(), interpolator));
			out.SetPosition(Math::Lerp(left->GetPosition(), right->GetPosition(), interpolator));
			out.SetScale(Math::Lerp(left->GetScale(), right->GetScale(), interpolator));
			out.SetRotation(Math::AngleLerp(left->GetRotation(), right->GetRotation(), interpolator));
			return out;
		}
	}
	else
	{
		throw Exception("AnimationTrack was empty!");
	}
}

void AnimationTrack::SetAnimatingNode(const shared_ptr<SceneNode> &node)
{
	mAnimatingNode = node;
}

shared_ptr<SceneNode> AnimationTrack::GetAnimatingNode() const
{
	return mAnimatingNode;
}

void AnimationTrack::Serialize(Serializer &sav)
{
	sav &mName;
	sav &mMaxTime;
	sav &mKeyframeList;
	sav &mAnimatingNode;
	if (sav.IsLoading())
	{
		for (auto &keyframe : mKeyframeList)
		{
			keyframe.mTrack = this;
		}
	}
}

shared_ptr<AnimationTrack> AnimationTrack::MakeCopy() const
{
	shared_ptr<AnimationTrack> copy = make_shared<AnimationTrack>();
	copy->mName = mName;
	copy->mMaxTime = mMaxTime;
	copy->mKeyframeList = mKeyframeList;
	for (auto &keyframe : copy->mKeyframeList)
	{
		keyframe.mTrack = copy.get();
	}
	return copy;
}

void AnimationTrack::SetEnabled(bool state)
{
	mEnabled = state;
}

bool AnimationTrack::IsEnabled() const
{
	return mEnabled;
}

void AnimationTrack::AnimateNode(float time, float weight)
{
	if (mEnabled && mAnimatingNode)
	{
		if (mKeyframeList.size())
		{
			auto keyframe = GetKeyframe(time);
			mAnimatingNode->AddLocalPosition(weight * keyframe.mPosition);
			mAnimatingNode->AddLocalScale(keyframe.mScale);
			mAnimatingNode->AddLocalRotation(weight * keyframe.mRotation);
		}
	}
}

void AnimationTrack::SetName(const string &name)
{
	mName = name;
}

const string &AnimationTrack::GetName() const
{
	return mName;
}

float AnimationTrack::GetMaxTime() const
{
	return mMaxTime;
}

vector<Keyframe> &AnimationTrack::GetKeyframeList()
{
	return mKeyframeList;
}
