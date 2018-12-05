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
#include "CreateSpriteCommand.h"
#include "Sprite.h"

CreateSpriteCommand::CreateSpriteCommand(const shared_ptr<Scene> & scene) : mScene(scene)
{
	mDesc = "CreateSprite";
}

void CreateSpriteCommand::Execute()
{
	if (auto scene = mScene.lock())
	{
		auto sprite = scene->CreateSprite();
		sprite->SetSize({ 30, 30 });
		sprite->SetRenderFlags(RF_NO_LIGHTING);
		mResult = sprite;
	}
}

void CreateSpriteCommand::Undo()
{
	if (auto scene = mScene.lock())
	{
		scene->RemoveNode(mResult.lock());
		mResult.reset();
	}
}

shared_ptr<Sprite> CreateSpriteCommand::GetResult()
{
	return mResult.lock();
}
