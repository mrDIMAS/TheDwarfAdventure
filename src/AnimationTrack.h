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

#include "Serializer.h"
#include "Keyframe.h"

class SceneNode;

class AnimationTrack
{
private:
	string mName { "UnnamedTrack" };
	float mMaxTime { 0 };
	vector<Keyframe> mKeyframeList;
	shared_ptr<SceneNode> mAnimatingNode;
	bool mEnabled { true };

public:
	AnimationTrack();
	~AnimationTrack();
	void SortKeyframesByTime();
	void AddKeyFrame(const Keyframe &newKeyframe);
	Keyframe GetKeyframe(float time);
	void SetAnimatingNode(const shared_ptr<SceneNode> &node);
	shared_ptr<SceneNode> GetAnimatingNode() const;
	// before call, make sure that associated node's tranform is set to identity!
	void AnimateNode(float time, float weight);
	void SetName(const string &name);
	const string &GetName() const;
	float GetMaxTime() const;
	vector<Keyframe> &GetKeyframeList();
	void Serialize(Serializer &sav);
	shared_ptr<AnimationTrack> MakeCopy() const;
	void SetEnabled(bool state);
	bool IsEnabled() const;
};
