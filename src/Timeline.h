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

#include "Keyframe.h"

class Editor;

namespace ed
{
	class Timeline;

	class DragContext
	{
	public:
		DragContext(Timeline* owner, bool snapToGrid, float mousePosition, Animation* animation);
		void AddKeyframe(Keyframe* keyframe);
		void Drag(float newMousePosition);
	private:
		struct Item
		{
			Keyframe* keyframe;
			float initialTime;
		};
		vector<Item> mKeyframes;
		bool mSnapToGrid { false };
		Timeline * mOwner { nullptr };
        float mDragStartedMousePosition { 0 };
		Animation* mAnimation { nullptr };		
	};

	class Timeline
	{
	public:
		Timeline(Editor * editor);
		void DrawAndUpdate();
		float GetTimeStep() const;
		float GetOffsetPerTimeStep() const;
	private:
		ImGuiID mContextMenuId;
		float mTimeStep { 0.075f };
		float mOffsetPerTimeStep { 0 };
		bool mSnapToGrid { true };
		void OnPlayActiveAnimationClicked();
		void SetActiveAnimationTimePosition(float time);
		void FixOverlappingKeyframes(vector<Keyframe> &keyframeList, float epsilon);
		Editor* mEditor { nullptr };
		unique_ptr<DragContext> mDragContext;

		struct
		{
			int mKeyIndex { -1 };
			AnimationTrack* mTrack { nullptr };
		} mKeyFrameContextMenu;
	};
}