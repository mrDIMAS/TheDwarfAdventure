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
#include "SetTrackKeyframeCommand.h"
#include "AnimationTrack.h"

SetTrackKeyframeCommand::SetTrackKeyframeCommand(const shared_ptr<AnimationTrack> & track, float time, const Vec2f & position, const Vec2f & scale, float rotation)
	: mTrack(track), mTime(time), mPosition(position), mScale(scale),
	mRotation(rotation), mHasLastKeyframe(false)
{
	mDesc = "SetKeyframe";
}

void SetTrackKeyframeCommand::Execute()
{
	if (auto track = mTrack.lock())
	{
		// check if we already got keyframe on the same time pos
		Keyframe *existing = nullptr;
		for (auto &keyframe : track->GetKeyframeList())
		{
			if (Math::RoughlyEqual(keyframe.GetTime(), mTime))
			{
				// and save it if so
				mLastKeyframe = keyframe;
				mHasLastKeyframe = true;
				existing = &keyframe;
				break;
			}
		}

		if (existing)
		{
			// overwrite existing keyframe
			existing->SetPosition(mPosition);
			existing->SetScale(mScale);
			existing->SetRotation(mRotation);
		}
		else
		{
			// create new keyframe
			Keyframe newFrame;
			newFrame.SetTime(mTime);
			newFrame.SetPosition(mPosition);
			newFrame.SetScale(mScale);
			newFrame.SetRotation(mRotation);
			track->AddKeyFrame(newFrame);
		}
	}
}

void SetTrackKeyframeCommand::Undo()
{
	if (auto track = mTrack.lock())
	{
		auto &keyframeList = track->GetKeyframeList();
		for (auto iter = keyframeList.begin(); iter != keyframeList.end();)
		{
			auto &keyframe = *iter;
			// find keyframe with matching time
			if (keyframe.GetTime() == mTime)
			{
				if (mHasLastKeyframe)
				{
					// return last keyframe back
					keyframe = mLastKeyframe;
					++iter;
				}
				else
				{
					// remove keyframe from track
					iter = keyframeList.erase(iter);
				}
			}
			else
			{
				++iter;
			}
		}
		track->SortKeyframesByTime();
	}
}
