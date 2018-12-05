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
#include "CreateAnimationCommand.h"
#include "Scene.h"
#include "Animation.h"
#include "AnimationTrack.h"

CreateAnimationCommand::CreateAnimationCommand(const shared_ptr<Scene> & scene) : mScene(scene)
{
	mDesc = "CreateAnimation";
}

void CreateAnimationCommand::Execute()
{
	if (auto scene = mScene.lock())
	{
		auto newAnimation = make_shared<Animation>();
		scene->AddAnimation(newAnimation);
		for (auto &node : scene->GetNodeList())
		{
			newAnimation->AddTrack()->SetAnimatingNode(node);
		}
		mNewAnimation = newAnimation;
	}
}

void CreateAnimationCommand::Undo()
{
	if (auto scene = mScene.lock())
	{
		scene->RemoveAnimation(mNewAnimation.lock());
	}
}
