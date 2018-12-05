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
#include "MoveInteractionMode.h"
#include "Renderer.h"
#include "Input.h"
#include "Game.h"
#include "Editor.h"
#include "Node.h"
#include "SetNodePositionCommand.h"

MoveInteractionMode::MoveInteractionMode(Editor * owner)
	: InteractionMode(owner)
{
	mDescription = "Move Object\n";
	SetShortcut({ sf::Keyboard::Q });
	mText = "MOV";
}

void MoveInteractionMode::DrawSettingsUI()
{

}

void MoveInteractionMode::DrawPreviewerOverlay()
{

}

void MoveInteractionMode::Update()
{
	const shared_ptr<SceneNode> node = mOwner->mSelectedNode;
	const Vec2f mousePos = ImGui::GetMousePos();

	if (node)
	{
		if (ImGui::IsMouseDown(0))
		{
			if (!mIsDragging)
			{
				if (mOwner->IsPointInNodeBounds(mousePos, node))
				{
					mIsDragging = true;

					mInitialPosition = node->GetLocalPosition();
					mClickPosition = mOwner->ScreenToWorld(mousePos);
				}
			}

			if (mIsDragging)
			{
				const Vec2f delta = mOwner->ScreenToWorld(mousePos) - mClickPosition;

				node->SetLocalPosition(mInitialPosition + delta);
			}
		}
		else
		{
			// Commit changes
			if (mIsDragging)
			{
				mIsDragging = false;

				mOwner->GetCommandStack().Do(make_shared<SetNodePositionCommand>(node, node->GetLocalPosition(), mInitialPosition));
			}
		}
	}
}
