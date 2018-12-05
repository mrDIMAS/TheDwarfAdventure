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
#include "RotateInteractionMode.h"
#include "Editor.h"
#include "Node.h"
#include "SetNodeRotationCommand.h"

RotateInteractionMode::RotateInteractionMode(Editor * owner)
	: InteractionMode(owner)
{
	mDescription = "Rotate Object\n";
	SetShortcut({ sf::Keyboard::W });
	mText = "ROT";
}

void RotateInteractionMode::DrawSettingsUI()
{

}

void RotateInteractionMode::DrawPreviewerOverlay()
{

}

void RotateInteractionMode::Update()
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

					mInitialRotation = node->GetLocalRotation();
					mClickPosition = mOwner->ScreenToWorld(mousePos);
				}
			}

			if (mIsDragging)
			{
				const Vec2f delta = mOwner->ScreenToWorld(mousePos) - mClickPosition;

				node->SetLocalRotation(mInitialRotation + delta.x);
			}
		}
		else
		{
			// Commit changes
			if (mIsDragging)
			{
				mIsDragging = false;

				mOwner->GetCommandStack().Do(make_shared<SetNodeRotationCommand>(node, node->GetLocalRotation(), mInitialRotation));
			}
		}
	}
}
