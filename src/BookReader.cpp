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
#include "BookReader.h"
#include "Canvas.h"
#include "Window.h"
#include "Label.h"

BookReader::BookReader()
{
	mCanvas = game->CreateCanvas();

	mWindow = mCanvas->Create<Window>();
	mWindow->SetSize({ 600, 400 });
	mWindow->SetInCenter();


	mText = mCanvas->Create<Label>();
	mText->SetSize({ mWindow->GetSize().x - 40, 0 });
	mText->SetAutoHeight(true);
	mWindow->AddChild(mText);

	SetVisible(false);

	Log() << "Book reader successfully initialized!" << LineEnd;
}

void BookReader::SetVisible(bool state)
{
	mCanvas->SetVisible(state);
}

void BookReader::SetText(const string & caption, const string & text)
{
	mWindow->SetCaption(caption);
	mText->SetText(text);
}

bool BookReader::IsVisible() const
{
	return mCanvas->IsVisible();
}
