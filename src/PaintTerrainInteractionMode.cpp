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
#include "PaintTerrainInteractionMode.h"
#include "UIHelpers.h"
#include "Cell.h"
#include "Game.h"
#include "Renderer.h"
#include "Scene.h"
#include "Terrain.h"
#include "Editor.h"
#include "ModifyTerrainCommand.h"
#include "Input.h"

PaintTerrainInteractionMode::PaintTerrainInteractionMode(Editor * owner)
	: InteractionMode(owner)
{
	mDescription = "Paint Terrain\n";
	SetShortcut({ sf::Keyboard::A });
	mText = "PNT";
}

void PaintTerrainInteractionMode::DrawSettingsUI()
{
	if (ImGui::TreeNode("Brush Options"))
	{
		ui::BitFieldDefinition args[8] = {
			{ "Visible", PA_VISIBLE },
			{ "Collidable", PA_COLLIDABLE },
			{ "Raytraceable", PA_RAYTRACEABLE },
			{ "Editable", PA_EDITABLE },
			{ "Lava", PA_LAVA },
			{ "Acid", PA_ACID },
			{ "Hydrophobic", PA_HYDROPHOBIC },
			{ "Metal", PA_METAL }
		};

		ImGui::Columns(2);

		ui::EnumTableField("Bounds", mMaskArgs.mBounds);
		ui::EnumTableField("First Pass Op", mMaskArgs.mFirstPassBitwiseOperation);
		ui::BitTableField("First Pass mask", mMaskArgs.mFirstPassMask, args);
		ui::EnumTableField("Second Pass Op", mMaskArgs.mSecondPassBitwiseOperation);
		ui::BitTableField("Second Pass mask", mMaskArgs.mSecondPassMask, args);
		if (mMaskArgs.mBounds == Bounds::Rect)
		{
			ui::Vector2TableField("Rect Size", mMaskArgs.mRectSize, 1.0f, 0, 0);
		}
		else if (mMaskArgs.mBounds == Bounds::Circle)
		{
			ui::FloatTableField("Circle Radius", mMaskArgs.mRadius, 1);
		}
		ui::ColorTableField("Border Color", mMaskArgs.mBorderColor);
		ui::FloatTableField("Border Thickness", mMaskArgs.mBorderThickness, 1);
		ui::EnumTableField("Border Op", mMaskArgs.mBorderBitwiseOperation);
		ui::BitTableField("Border Mask", mMaskArgs.mBorderMask, args);
		ui::EnumTableField("Blend Op", mMaskArgs.mBlendOp);
		ui::EnumTableField("Brush Mode", mMaskArgs.mBrushMode);
		ui::BoolField("Direct Access", mMaskArgs.mEditorAccess);
		ui::ByteField("Background Value", mMaskArgs.mBackgroundValue, 1, 0, 255);
		ui::BoolField("Allow Only On Visible Dest Pix", mMaskArgs.mDoBitmaskOnlyOnVisibleDestPixels);
		ui::BoolField("Allow Only On Visible Brush Pix", mMaskArgs.mDoBitmaskOnlyOnVisiblePixelsOfReplaceImage);
		mMaskArgs.mDoNotAddLavaPixelsToChillList = true;
		ImGui::TreePop();
	}
}

void PaintTerrainInteractionMode::DrawPreviewerOverlay()
{
	DrawTerrainCells();
	DrawTerrainBrush();
}

void PaintTerrainInteractionMode::Update()
{
	const shared_ptr<Scene> scene = mOwner->mEditorScene;
	const shared_ptr<Terrain> terrain = scene->GetTerrain();

	if (ImGui::IsMouseDown(0))
	{
		if (!mPainting)
		{
			mPainting = true;
		}

		if (terrain && mPainting)
		{
			// Make copy of each cell that will be modified
			for (const shared_ptr<Cell>& cell : terrain->GetCells())
			{
				if (mModifiedCells.find(cell) == mModifiedCells.end())
				{
					if (cell->IsIntersectsWithMaskOpBounds(mMaskArgs))
					{
						mModifiedCells[cell] = cell->MakeCompressedCopy();
					}
				}
			}

			terrain->Mask(mMaskArgs);
		}
	}
	else
	{
		if (mPainting)
		{
			// Commit changes
			vector<shared_ptr<Cell>> currentState;
			vector<shared_ptr<Cell>> oldState;
			for (const auto & kv : mModifiedCells)
			{
				// Current (compress)
				currentState.push_back(kv.first->MakeCompressedCopy());
				
				// Old (already compressed)
				oldState.push_back(kv.second);
			}
			
			mOwner->GetCommandStack().Do(make_shared<ModifyTerrainCommand>(terrain, currentState, oldState));
			
			mModifiedCells.clear();

			mPainting = false;
		}
	}
}

void PaintTerrainInteractionMode::DrawTerrainBrush()
{
	const shared_ptr<Scene> scene = mOwner->mEditorScene;
	const sf::View& view = scene->GetView();
	const shared_ptr<Terrain> terrain = scene->GetTerrain();
	const Input* input = game->GetInput();

	if (!terrain)
	{
		return;
	}

	ImDrawList* const drawList = ImGui::GetWindowDrawList();
	const sf::IntRect viewport = game->GetRenderer()->GetWindow().getViewport(view);

	const Vec2f screenPos = ImGui::GetMousePos();

	const Vec2f worldPos = mOwner->ScreenToWorld(screenPos);

	// Lock movement to horizontal or vertical
	if (input->IsKeyPressed(sf::Keyboard::LShift))
	{
		mMaskArgs.mPosition.x = worldPos.x;
	}
	else if (input->IsKeyPressed(sf::Keyboard::LControl))
	{
		mMaskArgs.mPosition.y = worldPos.y;
	}
	else
	{
		mMaskArgs.mPosition = worldPos;
	}

	if (mMaskArgs.mBounds == Bounds::Circle)
	{		
		drawList->AddCircle(screenPos, mMaskArgs.mRadius / mOwner->mZoom, ImColor(255, 0, 0), 20, 1);
	}
	else if (mMaskArgs.mBounds == Bounds::Rect)
	{
		const Vec2f min = screenPos;
		const Vec2f max = screenPos + mMaskArgs.mRectSize / mOwner->mZoom;

		drawList->AddRect(min, max, ImColor(255, 0, 0));
	}
}

void PaintTerrainInteractionMode::DrawTerrainCells()
{
	const shared_ptr<Scene> scene = mOwner->mEditorScene;
	const sf::View& view = scene->GetView();
	const shared_ptr<Terrain> terrain = scene->GetTerrain();

	if (!terrain)
	{
		return;
	}

	ImDrawList* const drawList = ImGui::GetWindowDrawList();
	const sf::IntRect viewport = game->GetRenderer()->GetWindow().getViewport(view);

	const Vec2i size = terrain->GetSize();

	for (int y = 0; y < size.y; ++y)
	{
		for (int x = 0; x < size.x; ++x)
		{
			const Vec2f min = Math::MapWorldToScreen({ x * Cell::SizeF, y * Cell::SizeF }, view.getTransform(), viewport);
			const Vec2f max = Math::MapWorldToScreen({ (x + 1) * Cell::SizeF, (y + 1) * Cell::SizeF }, view.getTransform(), viewport);
			drawList->AddRect(min, max, ImColor(0, 255, 0, 120));
		}
	}
}
