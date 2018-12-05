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
#include "Canvas.h"
#include "Button.h"
#include "SlideList.h"
#include "ListBox.h"
#include "Renderer.h"
#include "Window.h"
#include "Sound.h"
#include "Input.h"

recursive_mutex &Canvas::GetMutex() const
{
	return game->GetRenderer()->GetMutex();
}

int Canvas::GetLayer() const
{
	return mLayer;
}

void Canvas::SetLayer(int layer)
{
	mLayer = layer;
	game->SortCanvases();
}

Canvas::Canvas()
{
	const auto renderer = game->GetRenderer();
	auto font = renderer->GetFont("data/fonts/font.ttf");

	mTheme = make_shared<Theme>(renderer->RequestTexture("data/textures/gui/gui_skin.png"));

	mTheme->SetArea(Theme::Item::Button, { 0, 0, 140, 30 });
	mTheme->SetColor(Theme::Item::Button, { 255, 255, 255 });
	mTheme->SetFont(Theme::Item::Button, font);

	mTheme->SetArea(Theme::Item::Button_Hovered, { 0, 0, 140, 30 });
	mTheme->SetColor(Theme::Item::Button_Hovered, { 200, 200, 200 });
	mTheme->SetFont(Theme::Item::Button_Hovered, font);

	mTheme->SetArea(Theme::Item::Button_Pressed, { 0, 0, 140, 30 });
	mTheme->SetColor(Theme::Item::Button_Pressed, { 150, 150, 150 });
	mTheme->SetFont(Theme::Item::Button_Pressed, font);

	// HARDCODE!
	mTheme->SetArea(Theme::Item::Window_TopBorder, { 14, 31, 5, 7 });
	mTheme->SetArea(Theme::Item::Window_BottomBorder, { 14, 73, 5, 7 });
	mTheme->SetArea(Theme::Item::Window_RightBorder, { 43, 62, 7, 5 });
	mTheme->SetArea(Theme::Item::Window_Cross, { 20, 44, 17, 17 });
	mTheme->SetArea(Theme::Item::Window_Back, { 51, 31, 15, 15 });
	mTheme->SetArea(Theme::Item::Window_RightBottomCorner, { 38, 68, 12, 12 });
	mTheme->SetArea(Theme::Item::Window_LeftBorder, { 1, 62, 7, 5 });
	mTheme->SetArea(Theme::Item::Window_RightTopCorner, { 38, 31, 12, 12 });
	mTheme->SetArea(Theme::Item::Window_LeftTopCorner, { 1, 31, 12, 12 });
	mTheme->SetArea(Theme::Item::Window_RightLeftCross, { 38, 44, 12, 17 });
	mTheme->SetArea(Theme::Item::Window_LeftRightCross, { 1, 44, 12, 17 });
	mTheme->SetArea(Theme::Item::Window_TopBottomCross, { 20, 31, 17, 12 });
	mTheme->SetArea(Theme::Item::Window_BottomTopCross, { 20, 68, 17, 12 });
	mTheme->SetArea(Theme::Item::Window_LeftBottomCorner, { 1, 68, 12, 12 });
	mTheme->SetArea(Theme::Item::ScrollBar, { 99, 31, 15, 31 });
	mTheme->SetArea(Theme::Item::Next, { 67, 47, 15, 15 });
	mTheme->SetArea(Theme::Item::Previous, { 83, 47, 15, 15 });
	mTheme->SetArea(Theme::Item::Plus, { 67, 31, 15, 15 });
	mTheme->SetArea(Theme::Item::Minus, { 83, 31, 15, 15 });
	mTheme->SetArea(Theme::Item::SlideList, { 51, 47, 15, 15 });
	mTheme->SetColor(Theme::Item::SlideList, { 255, 255, 255 });
	mTheme->SetFont(Theme::Item::SlideList, font);

	mTheme->SetArea(Theme::Item::Inset_Back, { 72, 63, 15, 15 });
	mTheme->SetArea(Theme::Item::Inset_BottomBorder, { 59, 80, 4, 3 });
	mTheme->SetArea(Theme::Item::Inset_LeftBorder, { 51, 71, 3, 4 });
	mTheme->SetArea(Theme::Item::Inset_LeftBottomCorner, { 51, 76, 7, 7 });
	mTheme->SetArea(Theme::Item::Inset_LeftTopCorner, { 51, 63, 7, 7 });
	mTheme->SetArea(Theme::Item::Inset_RightBorder, { 68, 71, 3, 4 });
	mTheme->SetArea(Theme::Item::Inset_RightBottomCorner, { 64, 76, 7, 7 });
	mTheme->SetArea(Theme::Item::Inset_RightTopCorner, { 64, 63, 7, 7 });
	mTheme->SetArea(Theme::Item::Inset_TopBorder, { 59, 63, 4, 3 });
	mTheme->SetArea(Theme::Item::Inset_TopBottomCross, { 88, 63, 5, 4 });
	mTheme->SetArea(Theme::Item::Inset_BottomTopCross, { 88, 72, 5, 4 });
	mTheme->SetArea(Theme::Item::Inset_CenterBorder, { 89, 68, 3, 3 });

	mTheme->SetArea(Theme::Item::Button_Back, { 17, 0, 15, 15 });
	mTheme->SetArea(Theme::Item::Button_BottomBorder, { 7, 14, 2, 2 });
	mTheme->SetArea(Theme::Item::Button_LeftBorder, { 0, 7, 2, 2 });
	mTheme->SetArea(Theme::Item::Button_LeftBottomCorner, { 0, 10, 6, 6 });
	mTheme->SetArea(Theme::Item::Button_LeftTopCorner, { 0, 0, 6, 6 });
	mTheme->SetArea(Theme::Item::Button_RightBorder, { 14, 7, 2, 2 });
	mTheme->SetArea(Theme::Item::Button_RightBottomCorner, { 10, 10, 6, 6 });
	mTheme->SetArea(Theme::Item::Button_RightTopCorner, { 10, 0, 6, 6 });
	mTheme->SetArea(Theme::Item::Button_TopBorder, { 7, 0, 2, 2 });

	mTheme->SetArea(Theme::Item::Control_Back, { 50, 0, 15, 15 });
	mTheme->SetArea(Theme::Item::Control_BottomBorder, { 40, 14, 2, 2 });
	mTheme->SetArea(Theme::Item::Control_LeftBorder, { 33, 7, 2, 2 });
	mTheme->SetArea(Theme::Item::Control_LeftBottomCorner, { 33, 10, 6, 6 });
	mTheme->SetArea(Theme::Item::Control_LeftTopCorner, { 33, 0, 6, 6 });
	mTheme->SetArea(Theme::Item::Control_RightBorder, { 47, 7, 2, 2 });
	mTheme->SetArea(Theme::Item::Control_RightBottomCorner, { 43, 10, 6, 6 });
	mTheme->SetArea(Theme::Item::Control_RightTopCorner, { 43, 0, 6, 6 });
	mTheme->SetArea(Theme::Item::Control_TopBorder, { 40, 0, 2, 2 });

	mTheme->SetArea(Theme::Item::ScrollBottom, { 99, 0, 9, 10 });
	mTheme->SetArea(Theme::Item::ScrollDown, { 109, 0, 9, 6 });
	mTheme->SetArea(Theme::Item::ScrollUp, { 129, 0, 9, 6 });
	mTheme->SetArea(Theme::Item::ScrollTop, { 119, 0, 9, 10 });

	// shit
	mTheme->SetArea(Theme::Item::Window, { 0, 52, 300, 400 });
	mTheme->SetColor(Theme::Item::Window, { 41, 51, 58 });
	mTheme->SetFont(Theme::Item::Window, font);

	mTheme->SetArea(Theme::Item::Button_Disabled, { 0, 0, 220, 50 });
	mTheme->SetColor(Theme::Item::Button_Disabled, { 126, 132, 135 });
	mTheme->SetFont(Theme::Item::Button_Disabled, font);

	mTheme->SetArea(Theme::Item::ListBox, { 302, 52, 200, 300 });
	mTheme->SetColor(Theme::Item::ListBox, { 109, 134, 152 });
	mTheme->SetFont(Theme::Item::ListBox, font);

	mTheme->SetArea(Theme::Item::ListBox_Item, { 221, 0, 220, 50 });
	mTheme->SetColor(Theme::Item::ListBox_Item, { 109, 134, 152 });
	mTheme->SetFont(Theme::Item::ListBox_Item, font);

	mTheme->SetArea(Theme::Item::HUD_Bars, { 507, 0, 349, 122 });
	mTheme->SetArea(Theme::Item::HUD_Minimap, { 507, 124, 128, 128 });
}

Canvas::~Canvas()
{
	DBG_ECHO();
}

void Canvas::SetTheme(const shared_ptr<Theme> &theme)
{
	mTheme = theme;
}

shared_ptr<Theme> Canvas::GetTheme() const
{
	return mTheme;
}

void Canvas::Update()
{
	auto virtualScreenMousePosition = game->GetRenderer()->ToVirtualScreen(game->GetInput()->GetMousePosition());

	// Update hierarchy for each root node
	for (auto widgetIterator = mWidgetList.begin(); widgetIterator != mWidgetList.end(); )
	{
		auto & widget = *widgetIterator;

		if (widget->mWantsBeDeleted)
		{
			widgetIterator = mWidgetList.erase(widgetIterator);
		}
		else
		{
			if (widget->mParent.expired())
			{
				widget->UpdateHierarchy(nullptr, mVisible, virtualScreenMousePosition);
			}

			++widgetIterator;
		}
	}

	// call EarlyUpdate	
	for (auto & widget : mWidgetList)
	{
		if (widget->IsGloballyVisible())
		{
			widget->EarlyUpdate();
		}
	}

	auto widgets = mWidgetList;

	// call Update
	for (auto & widget : widgets)
	{
		if (widget->IsGloballyVisible())
		{
			if (widget->IsHovered())
			{
				if (!widget->mAlreadyHovered)
				{
					widget->OnMouseEnter();
				}
				widget->mAlreadyHovered = true;
				widget->OnHover();
			}
			else
			{
				widget->mAlreadyHovered = false;
			}
			widget->Update();
		}
	}
}

vector<shared_ptr<Widget>> &Canvas::GetWidgetList()
{
	return mWidgetList;
}

void Canvas::SetVisible(bool visible)
{
	mVisible = visible;
}

bool Canvas::IsVisible() const
{
	return mVisible;
}

void Canvas::Remove(const shared_ptr<Widget> &widget)
{
	mWidgetList.erase(remove(mWidgetList.begin(), mWidgetList.end(), widget), mWidgetList.end());
}

bool Canvas::IsAnythingHovered()
{
	for (auto &widget : mWidgetList)
	{
		if (widget->IsHovered())
		{
			return true;
		}
	}
	return false;
}
