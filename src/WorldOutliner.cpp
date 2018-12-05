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
#include "WorldOutliner.h"
#include "Editor.h"
#include "Sprite.h"
#include "Scene.h"
#include "AttachCommand.h"
#include "UIHelpers.h"

namespace ed
{
	WorldOutliner::WorldOutliner(Editor * owner) : mOwner(owner)
	{
		ContextMenuId = ui::GenPopupId();
	}

	void WorldOutliner::DrawAndUpdate()
	{
		ImGui::Begin("World Outliner");

		// Node context menu
		if (ImGui::BeginPopupEx(ContextMenuId, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove))
		{
			ImGui::Text(mContextMenuPlacementTarget->GetName().c_str());

			ImGui::Separator();

			if (ImGui::Button("Detach", { 120, 0 }))
			{
				mOwner->mCommandStack.Do(make_shared<AttachCommand>(mDragSource, nullptr));

				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Delete", { 120, 0 }))
			{
				mContextMenuPlacementTarget->RemoveFromScene();

				mOwner->mSelectedNode.reset();

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		const shared_ptr<Scene> scene = mOwner->mEditorScene;

		for (const shared_ptr<SceneNode>& node : scene->GetNodeList())
		{
			if (!node->GetParent())
			{
				DrawAndUpdateTreeRecursive(node);
			}
		}

		// Actual attach - performed here because it is invalid to changed collection
		// which being iterated
		if (mDragTarget && mDragSource)
		{
			if (mDragSource->GetParent() != mDragTarget)
			{
				mOwner->mCommandStack.Do(make_shared<AttachCommand>(mDragSource, mDragTarget));			
			}

			mDragTarget = nullptr;
			mDragSource = nullptr;
		}	

		ImGui::End();
	}

	void WorldOutliner::DrawAndUpdateTreeRecursive(const shared_ptr<SceneNode>& node)
	{
		int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

		if (node->GetChildren().empty())
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		if (node == mOwner->mSelectedNode)
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		if (ImGui::TreeNodeEx(node->GetName().c_str(), flags))
		{			
			if (ImGui::IsItemClicked(1))
			{
				ImGui::OpenPopupEx(ContextMenuId);

				mContextMenuPlacementTarget = node;
			}
			else if (ImGui::IsItemClicked(0))
			{
				mOwner->mSelectedNode = node;
			}

			// Dragging
			{
				if (ImGui::BeginDragDropSource())
				{
					ImGui::Text(node->GetName().c_str());
					SceneNode* const rawPtr = node.get();
					ImGui::SetDragDropPayload(DragDropType, &rawPtr, sizeof(rawPtr));
					mDragSource = node;
					ImGui::EndDragDropSource();
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DragDropType))
					{
						SceneNode* const draggedNode = *reinterpret_cast<SceneNode**>(payload->Data);

						if (draggedNode != node.get())
						{
							if (!draggedNode->ContainsChild(node))
							{
								// Delay attach
								mDragTarget = node;								
							}
						}
					}
					ImGui::EndDragDropTarget();
				}
			}

			for (const shared_ptr<SceneNode>& child : node->GetChildren())
			{
				DrawAndUpdateTreeRecursive(child);
			}			

			ImGui::TreePop();
		}
	}
}