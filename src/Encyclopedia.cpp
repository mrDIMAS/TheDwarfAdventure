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
#include "Encyclopedia.h"
#include "Canvas.h"
#include "Window.h"
#include "Label.h"
#include "ScrollableArea.h"

Encyclopedia::Encyclopedia()
{
	mCanvas = game->CreateCanvas();

	mWindow = mCanvas->Create<Window>();
	mWindow->SetSize({ 600, 400 });
	mWindow->SetInCenter();
	mWindow->SetAutoLayoutingEnabled(false);

	mTopicsArea = mCanvas->Create<ScrollableArea>();
	mTopicsArea->SetPosition({ 20, 20 });
	mTopicsArea->SetSize({ mWindow->GetSize().x * 0.25f, mWindow->GetSize().y - 40 });

	mTextArea = mCanvas->Create<ScrollableArea>();
	mTextArea->SetPosition(mTopicsArea->GetPosition() + Vec2f(mTopicsArea->GetSize().x, 0));
	mTextArea->SetSize({ mWindow->GetSize().x * 0.75f, mWindow->GetSize().y - 40 });

	SetVisible(false);

	Log() << "Encyclopedia successfully initialized!" << LineEnd;
}

void Encyclopedia::SetVisible(bool state)
{
	mCanvas->SetVisible(state);
}

bool Encyclopedia::IsVisible() const
{
	return mCanvas->IsVisible();
}

void Encyclopedia::SwitchVisible()
{
	SetVisible(!IsVisible());
}
