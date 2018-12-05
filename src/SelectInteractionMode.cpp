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
#include "SelectInteractionMode.h"
#include "Node.h"
#include "Scene.h"
#include "Editor.h"
#include "Renderer.h"
#include "Input.h"

SelectInteractionMode::SelectInteractionMode(Editor * owner)
	: InteractionMode(owner)
{
	mDescription = "Select Object\n";
	SetShortcut({ sf::Keyboard::S });
	mText = "SEL";
}

void SelectInteractionMode::DrawSettingsUI()
{

}

void SelectInteractionMode::DrawPreviewerOverlay()
{
	// Draw selection frame
	if (mFrameVisible)
	{
		const shared_ptr<Scene> scene = mOwner->mEditorScene;
		const sf::View& view = scene->GetView();
		ImDrawList* const drawList = ImGui::GetWindowDrawList();
		const sf::IntRect viewport = game->GetRenderer()->GetWindow().getViewport(view);

		const Vec2f min = Math::MapWorldToScreen(mMin, view.getTransform(), viewport);
		const Vec2f max = Math::MapWorldToScreen(mMax, view.getTransform(), viewport);

		drawList->AddRect(min, max, ImColor(163, 73, 164));
	}
}

void SelectInteractionMode::Update()
{
	const shared_ptr<Scene> scene = mOwner->mEditorScene;

	const sf::View& view = scene->GetView();
	const sf::IntRect viewport = game->GetRenderer()->GetWindow().getViewport(view);
	const Vec2f screenMousePos = game->GetInput()->GetMousePosition();

	if (ImGui::IsMouseClicked(0))
	{
		mMin = mOwner->ScreenToWorld(screenMousePos);

		mFrameVisible = true;

		for (const shared_ptr<SceneNode>& node : scene->GetNodeList())
		{
			const Math::RotatedRectangle bounds = {
				node->GetSize(),
				node->GetGlobalTransform(),
				view.getTransform(), viewport
			};

			if (bounds.IsPointInside(screenMousePos))
			{
				mOwner->mSelectedNode = node;

				break;
			}
		}
	}

	if (ImGui::IsMouseDragging(0, 0))
	{
		mMax = mOwner->ScreenToWorld(screenMousePos);
	}

	if (!ImGui::IsMouseDown(0))
	{
		mFrameVisible = false; 
	}
}
