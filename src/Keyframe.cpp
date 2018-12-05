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
#include "Keyframe.h"
#include "AnimationTrack.h"

Keyframe::Keyframe()
{

}

void Keyframe::Serialize(Serializer &sav)
{
	sav &mPosition;
	sav &mScale;
	sav &mRotation;
	sav &mTime;
}

void Keyframe::SetPosition(const Vec2f &p)
{
	mPosition = p;
}

Vec2f Keyframe::GetPosition() const
{
	return mPosition;
}

void Keyframe::SetScale(const Vec2f &s)
{
	mScale = s;
}

Vec2f Keyframe::GetScale() const
{
	return mScale;
}

void Keyframe::SetRotation(float r)
{
	mRotation = r;
}

float Keyframe::GetRotation() const
{
	return mRotation;
}

void Keyframe::SetTime(float t)
{
	if (t != mTime)
	{
		mTime = t;

		if (mTrack)
		{
			mTrack->SortKeyframesByTime();
		}
	}
}

float Keyframe::GetTime() const
{
	return mTime;
}

void Keyframe::ChangeTime(float value)
{
	SetTime(mTime + value);
}

bool Keyframe::operator==(const Keyframe &other) const
{
	return 
		mTrack == other.mTrack && 
		mPosition == other.mPosition && 
		mScale == other.mScale &&
		mRotation == other.mRotation &&
		mTime == other.mTime;
}
