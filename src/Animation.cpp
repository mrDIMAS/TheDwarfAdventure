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
#include "Animation.h"
#include "AnimationTrack.h"
#include "Node.h"

void Animation::UpdateTracks()
{
	for (auto &track : mTrackList)
	{
		track->AnimateNode(mTimePosition, mWeight);
	}
}

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::SetEnabled(bool state)
{
	mEnabled = state;
}

bool Animation::IsEnabled() const
{
	return mEnabled;
}

void Animation::SetLooped(bool state)
{
	mLooped = state;
}

bool Animation::IsLooped() const
{
	return mLooped;
}

void Animation::SetSpeed(float speed)
{
	mSpeed = speed;
}

float Animation::GetSpeed() const
{
	return mSpeed;
}

void Animation::SetTimePosition(float time)
{
	if (mLooped)
	{
		mTimePosition = Math::Wrap(time, 0.0f, mLength);
	}
	else
	{
		mTimePosition = Math::Clamp(time, 0.0f, mLength);
	}
}

float Animation::GetTimePosition() const
{
	return mTimePosition;
}

void Animation::ChangeTimePosition(float amount)
{
	SetTimePosition(mTimePosition + amount);
}

void Animation::Rewind()
{
	mTimePosition = 0.0f;
	mWeight = 1.0f;
}

void Animation::ClipLengthToLongestTrack()
{
	mLength = 0.0f;
	for (auto &track : mTrackList)
	{
		if (track->GetMaxTime() > mLength)
		{
			mLength = track->GetMaxTime();
		}
	}
}

void Animation::FadeOut(float time)
{
	mFadeStep = -1.0f / abs(time);
}

void Animation::FadeIn(float time)
{
	mFadeStep = 1.0f / abs(time);
}

bool Animation::IsFading() const
{
	return mFadeStep != 0.0f;
}

void Animation::Update(float dt)
{
	float nextTimePos = mTimePosition + dt * mSpeed;

	for (auto &evt : mTimeEvents)
	{
		if (mTimePosition < evt.mTime && nextTimePos >= evt.mTime)
		{
			if (evt.mAction)
			{
				evt.mAction();
			}
		}
	}

	SetTimePosition(nextTimePos);
	UpdateTracks();
	// handle fading (part of animation blending)
	if (mFadeStep != 0.0f)
	{
		mWeight += mFadeStep * dt;
		if (mFadeStep < 0 && mWeight <= 0)
		{
			mWeight = 0;
			mEnabled = false;
			mFadeStep = 0;
		}
		else if (mFadeStep > 0 && mWeight >= 1)
		{
			mWeight = 1;
			mFadeStep = 0;
		}
	}
}

shared_ptr<AnimationTrack> Animation::AddTrack()
{
	auto track = make_shared<AnimationTrack>();

	mTrackList.push_back(track);

	return track;
}

shared_ptr<AnimationTrack> Animation::FindTrack(const string &name)
{
	for (auto &track : mTrackList)
	{
		if (track->GetName() == name)
		{
			return track;
		}
	}
	return nullptr;
}

shared_ptr<AnimationTrack> Animation::FindAssociatedTrack(const shared_ptr<SceneNode> &node)
{
	for (auto &track : mTrackList)
	{
		if (track->GetAnimatingNode() == node)
		{
			return track;
		}
	}
	return nullptr;
}

const vector<shared_ptr<AnimationTrack>> &Animation::GetTrackList() const
{
	return mTrackList;
}

void Animation::Serialize(Serializer &sav)
{
	sav &mTrackList;
	sav &mEnabled;
	sav &mLooped;
	sav &mSpeed;
	sav &mLength;
	sav &mTimePosition;
	sav &mName;
	sav &mWeight;
	sav &mFadeStep;
}

void Animation::SetLength(float length)
{
	mLength = Math::Max(0.0f, length);
}

float Animation::GetLength() const
{
	return mLength;
}

void Animation::ChangeLength(float value)
{
	mLength = Math::Max(0.0f, mLength + value);
}

void Animation::SetWeight(float weight)
{
	mWeight = weight;
}

float Animation::GetWeight() const
{
	return mWeight;
}

void Animation::RemoveTrack(const shared_ptr<AnimationTrack> &track)
{
	mTrackList.erase(std::remove(mTrackList.begin(), mTrackList.end(), track), mTrackList.end());
}

shared_ptr<Animation> Animation::MakeCopy() const
{
	shared_ptr<Animation> copy = make_shared<Animation>();
	copy->mName = mName;
	copy->mEnabled = mEnabled;
	copy->mLooped = mLooped;
	copy->mSpeed = mSpeed;
	copy->mLength = mTimePosition;
	for (size_t i = 0; i < mTrackList.size(); ++i)
	{
		copy->mTrackList.push_back(mTrackList[i]->MakeCopy());
	}
	return copy;
}

void Animation::SetName(const string &name)
{
	mName = name;
}

const string &Animation::GetName() const
{
	return mName;
}

void Animation::ResetAssociatedNodesTransform()
{
	for (auto &track : GetTrackList())
	{
		if (track->IsEnabled())
		{
			if (track->GetKeyframeList().size())
			{
				if (auto node = track->GetAnimatingNode())
				{
					node->ResetLocalTransform();
				}
			}
		}
	}
}

bool Animation::IsEnded() const
{
	return mTimePosition == mLength;
}

void Animation::AddTimeEvent(TimeEvent &&evt)
{
	mTimeEvents.push_back(move(evt));
}

bool Animation::HasInfluence() const
{
	return mWeight >= FLT_EPSILON;
}
