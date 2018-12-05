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

#include "EditorCommand.h"
#include "Keyframe.h"

class SetTrackKeyframeCommand final : public EditorCommand
{
private:
	weak_ptr<AnimationTrack> mTrack;

	float mTime;
	Vec2f mPosition;
	Vec2f mScale;
	float mRotation;

	Keyframe mLastKeyframe;
	bool mHasLastKeyframe;

public:
	SetTrackKeyframeCommand(const shared_ptr<AnimationTrack> &track, float time,
		const Vec2f &position, const Vec2f &scale, float rotation);
	virtual void Execute() override;
	virtual void Undo() override;
};
