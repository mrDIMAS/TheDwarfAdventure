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
#include "SetNodeOriginCommand.h"
#include "Node.h"

SetNodeOriginCommand::SetNodeOriginCommand(const shared_ptr<SceneNode> & node, const Vec2f & newOrigin, const Vec2f & lastOrigin)
	: mNode(node), mOrigin(newOrigin), mLastOrigin(lastOrigin)
{
	mDesc = StringBuilder("SetOrigin(") << mOrigin.x << "; " << mOrigin.y << ")";
}

void SetNodeOriginCommand::Execute()
{
	if (auto node = mNode.lock())
	{
		node->SetLocalOrigin(mOrigin);
	}
}

void SetNodeOriginCommand::Undo()
{
	if (auto node = mNode.lock())
	{
		node->SetLocalOrigin(mLastOrigin);
	}
}
