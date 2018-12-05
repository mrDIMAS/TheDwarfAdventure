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

class AnimationTrack;

class Keyframe
{
private:
	friend class AnimationTrack;
	AnimationTrack *mTrack { nullptr };
	Vec2f mPosition { 0, 0 };
	Vec2f mScale { 1, 1 };
	float mRotation { 0 };
	float mTime { 0 };

public:
	Keyframe();
	void Serialize(Serializer &sav);
	void SetPosition(const Vec2f &p);
	Vec2f GetPosition() const;
	void SetScale(const Vec2f &s);
	Vec2f GetScale() const;
	void SetRotation(float r);
	float GetRotation() const;
	void SetTime(float t);
	float GetTime() const;
	void ChangeTime(float value);
	bool operator==(const Keyframe &other) const;
};
