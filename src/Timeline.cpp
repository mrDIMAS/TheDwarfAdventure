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
#include "Timeline.h"
#include "Editor.h"
#include "Animation.h"
#include "AnimationTrack.h"
#include "Node.h"
#include "UIHelpers.h"
#include "Scene.h"

namespace ed
{
	void Timeline::OnPlayActiveAnimationClicked()
	{
		mEditor->mActiveAnimation->Rewind();
		mEditor->mActiveAnimation->SetEnabled(true);
		mEditor->mActiveAnimation->SetLooped(true);
	}

	void Timeline::SetActiveAnimationTimePosition(float time)
	{
		mEditor->mActiveAnimation->SetEnabled(false);
		// Manually animate nodes. This eliminates blending, but works predictable
		mEditor->mActiveAnimation->SetTimePosition(time);
		mEditor->mActiveAnimation->ResetAssociatedNodesTransform();
		mEditor->mActiveAnimation->UpdateTracks();
		mEditor->mEditorScene->UpdateTransforms();
	}

	void Timeline::FixOverlappingKeyframes(vector<Keyframe>& keyframeList, float epsilon)
	{
		// fix keyframes overlapping
		for (auto iter = keyframeList.begin(); iter != keyframeList.end();)
		{
			auto &keyframe = *iter;
			bool overlap = false;
			for (auto &otherKeyframe : keyframeList)
			{
				if (&otherKeyframe != &keyframe)
				{
					if (Math::RoughlyEqual(keyframe.GetTime(), otherKeyframe.GetTime(), epsilon))
					{
						overlap = true;
						break;
					}
				}
			}
			// remove overlapped keyframe
			if (overlap)
			{
				iter = keyframeList.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	Timeline::Timeline(Editor * editor) : mEditor(editor)
	{
		mContextMenuId = ui::GenPopupId();
	}

	void Timeline::DrawAndUpdate()
	{
		ImGui::Begin("Timeline");

		// Key context menu
		if (ImGui::BeginPopupEx(mContextMenuId, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove))
		{
			if (ImGui::Button("Delete", { 120, 0 }))
			{
				if (mKeyFrameContextMenu.mKeyIndex >= 0)
				{
					mKeyFrameContextMenu.mTrack->GetKeyframeList().erase(mKeyFrameContextMenu.mTrack->GetKeyframeList().begin() + mKeyFrameContextMenu.mKeyIndex);

					mKeyFrameContextMenu.mKeyIndex = -1;
				}

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		// Main keys
		{
			ImGui::Text("Length: %.1f s", mEditor->mActiveAnimation->GetLength());
			ImGui::SameLine();
			if (ImGui::Button("+1s"))
			{
				mEditor->mActiveAnimation->ChangeLength(1);
			}
			ImGui::SameLine();
			if (ImGui::Button("-1s"))
			{
				mEditor->mActiveAnimation->ChangeLength(-1);
			}
			ImGui::SameLine();
			if (ImGui::Button("Fit"))
			{
				mEditor->mActiveAnimation->ClipLengthToLongestTrack();
			}
			ImGui::SameLine();
			if (ImGui::Button("Play"))
			{
				OnPlayActiveAnimationClicked();
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop"))
			{
				mEditor->mActiveAnimation->SetEnabled(false);
			}
			ImGui::SameLine();
			ImGui::Checkbox("Snap to Grid", &mSnapToGrid);
		}

		// calculate widest track name for proper alignment
		float widestTrackName = 0.0f;
		for (auto &track : mEditor->mActiveAnimation->GetTrackList())
		{
			// rename track to node name
			track->SetName(track->GetAnimatingNode()->GetName());
			// calc widest track name
			float width = ImGui::CalcTextSize(track->GetName().c_str()).x;
			if (width > widestTrackName)
			{
				widestTrackName = width;
			}
		}

		// Add spacing
		widestTrackName += 15;

		ImGui::BeginChild("TimelineRegion", { 0, 0 }, true, ImGuiWindowFlags_HorizontalScrollbar);
		const float localX = ImGui::GetCursorPosX();
		const float localY = ImGui::GetCursorPosY();
		const float globalX = ImGui::GetWindowPos().x - ImGui::GetScrollX();
		const float globalY = ImGui::GetWindowPos().y - ImGui::GetScrollY();
		const float regionWidth = ImGui::GetWindowWidth();
		const float regionHeight = ImGui::GetWindowHeight();

		const float keyWidth = 10;
		const float maxTimeLineHeight = regionHeight;
		const float maxTimeLineWidth = regionWidth;
		mOffsetPerTimeStep = 3.0f * keyWidth * (1.0f / mTimeStep);
		auto drawList = ImGui::GetWindowDrawList();
		const float trackLocalOriginX = localX + widestTrackName;
		const float trackHeight = 20;
		const float rulerHeight = 40;

		static bool dragCursorMode = false;
		static float dragStartedTimePosition = 0.0f;
		static float dragStartedCursorPosition = 0.0f;

		// Draw animation cursor
		{
			const float lx = trackLocalOriginX + mOffsetPerTimeStep * mEditor->mActiveAnimation->GetTimePosition();
			const float cx = globalX + lx;
			const float cursorWidth = 10;
			const float cursorHeight = 16;
			const float cursorTipHeight = 5;
			const float cursorTotalHeight = cursorHeight + cursorTipHeight;
			const float halfCursorWidth = cursorWidth * 0.5f;
			Math::Polygon cursorPolygon;
			cursorPolygon.AddPoint({ cx - halfCursorWidth, globalY });
			cursorPolygon.AddPoint({ cx + halfCursorWidth, globalY });
			cursorPolygon.AddPoint({ cx + halfCursorWidth, globalY + cursorHeight });
			cursorPolygon.AddPoint({ cx, globalY + cursorHeight + cursorTipHeight });
			cursorPolygon.AddPoint({ cx - halfCursorWidth, globalY + cursorHeight });

			const bool cursorHovered = cursorPolygon.Contains(ImGui::GetMousePos()) || dragCursorMode;

			const ImColor cursorColor = cursorHovered ? ImColor(0, 255, 0, 220) : ImColor(0, 200, 0, 200);
			ImVec2* polygonPoints = reinterpret_cast<ImVec2*>(&cursorPolygon.GetPoints()[0]);
			drawList->AddConvexPolyFilled(polygonPoints, cursorPolygon.GetPointCount(), cursorColor);

			const string str = RoundAndConvert(mEditor->mActiveAnimation->GetTimePosition(), 2) + "s";
			ImGui::SetCursorPos({ lx + halfCursorWidth + 2, localY - 5 });
			ImGui::Text(str.c_str(), time);

			const ImVec2 begin = { cx, globalY + cursorTotalHeight };
			const ImVec2 end = { cx, globalY + cursorTotalHeight + maxTimeLineHeight };
			drawList->AddLine(begin, end, ImColor(0, 200, 0));

			if (ImGui::IsMouseDown(0))
			{
				if (!dragCursorMode)
				{
					if (cursorHovered)
					{
						dragCursorMode = true;
						dragStartedTimePosition = mEditor->mActiveAnimation->GetTimePosition();
						dragStartedCursorPosition = ImGui::GetMousePos().x;
					}
				}
			}
			else
			{
				dragCursorMode = false;
			}
		}

		if (dragCursorMode)
		{
			const float dragDelta = (ImGui::GetMousePos().x - dragStartedCursorPosition) / mOffsetPerTimeStep;
			float nextTimePosition = dragStartedTimePosition + dragDelta;
			if (nextTimePosition > mEditor->mActiveAnimation->GetLength())
			{
				nextTimePosition = mEditor->mActiveAnimation->GetLength();
			}
			else if (nextTimePosition < 0)
			{
				nextTimePosition = 0;
			}
			else if (mSnapToGrid)
			{
				nextTimePosition = Math::Snap(nextTimePosition, mTimeStep);
			}
			mEditor->mActiveAnimation->SetEnabled(true);
			SetActiveAnimationTimePosition(nextTimePosition);
		}

		// Add track name separator
		{
			const float sx = globalX + trackLocalOriginX - keyWidth * 0.5f;
			const ImVec2 begin = { sx, globalY };
			const ImVec2 end = { sx, globalY + maxTimeLineHeight };
			drawList->AddLine(begin, end, ImColor(120, 120, 120));
		}

		// Draw ruler
		{
			const float endTimeEpsilon = mTimeStep * 0.1f;
			const float endTime = mEditor->mActiveAnimation->GetLength() + endTimeEpsilon;

			int majorLineCounter = 0;
			for (float time = 0; time <= endTime; time += mTimeStep)
			{
				const float px = globalX + trackLocalOriginX + mOffsetPerTimeStep * time;
				const float py = globalY + rulerHeight;

				const bool majorLine = majorLineCounter % 5 == 0;

				// Draw ruler line		
				{
					const float height = majorLine ? 10.0f : 5.0f;
					const float thickness = majorLine ? 2.0f : 1.0f;
					const ImVec2 begin = { px - thickness * 0.5f, py - height };
					const ImVec2 end = { px - thickness * 0.5f, py };
					drawList->AddLine(begin, end, ImColor(120, 120, 120), thickness);
				}
				// Draw grid line
				{
					const ImVec2 begin = { px, py };
					const ImVec2 end = { px, py + maxTimeLineHeight };
					drawList->AddLine(begin, end, ImColor(100, 100, 100, 120));
				}
				// Draw time mark
				if (majorLine)
				{
					const string str = RoundAndConvert(time, 2) + "s";
					ImGui::SetCursorPos({ trackLocalOriginX + mOffsetPerTimeStep * time + 3, localY + 15 });
					ImGui::Text(str.c_str(), time);
				}

				++majorLineCounter;
			}
		}

		// Draw ruler base
		{
			const float sy = globalY + rulerHeight;
			const ImVec2 begin = { globalX, sy };
			const ImVec2 end = { globalX + maxTimeLineWidth, sy };
			drawList->AddLine(begin, end, ImColor(120, 120, 120));
		}

		// Draw tracks		
		ImGui::SetCursorPos({ localX, localY + rulerHeight });
		for (auto &track : mEditor->mActiveAnimation->GetTrackList())
		{
			sf::FloatRect trackBounds;
			trackBounds.left = globalX + ImGui::GetCursorPosX();
			trackBounds.top = globalY + ImGui::GetCursorPosY();
			trackBounds.width = maxTimeLineWidth;
			trackBounds.height = trackHeight;

			const ImColor highlightColor = ImColor(120, 120, 120, 30);
			const bool mouseInsideTrack = trackBounds.contains(ImGui::GetMousePos());

			// Highlight track
			if (track->GetAnimatingNode() == mEditor->mSelectedNode || mouseInsideTrack)
			{
				ImVec2 trackMin = { trackBounds.left, trackBounds.top };
				ImVec2 trackMax = { trackBounds.left + trackBounds.width, trackBounds.top + trackBounds.height };

				drawList->AddRectFilled(trackMin, trackMax, highlightColor);

				if (mouseInsideTrack)
				{
					if (ImGui::IsMouseClicked(0))
					{
						mEditor->mSelectedNode = track->GetAnimatingNode();
					}
				}
			}

			// print track name
			ImGui::Text(track->GetName().c_str());

			// draw keyframes
			auto &keyframeList = track->GetKeyframeList();
			for (size_t i = 0; i < keyframeList.size(); ++i)
			{
				Keyframe& keyframe = keyframeList[i];

				const float keyframeX = trackLocalOriginX + mOffsetPerTimeStep * keyframe.GetTime() - keyWidth * 0.5f;

				const float kx = globalX + keyframeX;
				ImVec2 keyframePoints[] = {
					{ kx + keyWidth * 0.5f, trackBounds.top },
					{ kx + keyWidth, trackBounds.top + trackHeight  * 0.5f },
					{ kx + keyWidth * 0.5f, trackBounds.top + trackHeight },
					{ kx, trackBounds.top + trackHeight * 0.5f },
				};

				Math::Polygon bounds;
				bounds.AddPoint(keyframePoints[0]);
				bounds.AddPoint(keyframePoints[1]);
				bounds.AddPoint(keyframePoints[2]);
				bounds.AddPoint(keyframePoints[3]);

				const bool keyFrameHovered = bounds.Contains(ImGui::GetMousePos());

				if (keyFrameHovered)
				{
					if (ImGui::IsMouseClicked(0))
					{
						SetActiveAnimationTimePosition(keyframe.GetTime());
					}
					else if (ImGui::IsMouseClicked(1))
					{
						mKeyFrameContextMenu.mKeyIndex = i;
						mKeyFrameContextMenu.mTrack = track.get();

						ImGui::OpenPopupEx(mContextMenuId);
					}

					if (!mDragContext && !dragCursorMode)
					{
						if (ImGui::IsMouseDragging(0, 1))
						{
							mDragContext = make_unique<DragContext>(this, mSnapToGrid, ImGui::GetMousePos().x, mEditor->mActiveAnimation.get());
							mDragContext->AddKeyframe(&keyframe);
						}
					}
				}

				drawList->AddConvexPolyFilled(keyframePoints, 4, ImColor(keyFrameHovered ? 255 : 200, 0, 0));
			}
			if (!mDragContext)
			{
				FixOverlappingKeyframes(keyframeList, mTimeStep / 4);
			}
			ImGui::Separator();
		}

		if (ImGui::IsMouseDown(0))
		{
			if (mDragContext)
			{
				mDragContext->Drag(ImGui::GetMousePos().x);
			}
		}
		else
		{
			mDragContext.reset();
		}

		ImGui::EndChild();
		ImGui::End();
	}

	float Timeline::GetTimeStep() const
	{
		return mTimeStep;
	}

	float Timeline::GetOffsetPerTimeStep() const
	{
		return mOffsetPerTimeStep;
	}

	DragContext::DragContext(Timeline * owner, bool snapToGrid, float mousePosition, Animation* animation) :
		mSnapToGrid(snapToGrid),
		mOwner(owner),
		mDragStartedMousePosition(mousePosition),
		mAnimation(animation)
	{

	}

	void DragContext::AddKeyframe(Keyframe * keyframe)
	{
		DragContext::Item item = { keyframe, keyframe->GetTime() };
		mKeyframes.push_back(move(item));
	}

	void DragContext::Drag(float newMousePosition)
	{
		const float delta = (newMousePosition - mDragStartedMousePosition) / mOwner->GetOffsetPerTimeStep();

		for (DragContext::Item & item : mKeyframes)
		{
			float newTime = item.initialTime + delta;

			if (newTime < 0)
			{
				newTime = 0;
			}
			else if (newTime > mAnimation->GetLength())
			{
				newTime = mAnimation->GetLength();
			}
			else
			{
				newTime = Math::Snap(newTime, mOwner->GetTimeStep());
			}

			item.keyframe->SetTime(newTime);
		}
	}
}