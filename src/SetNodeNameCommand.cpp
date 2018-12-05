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
#include "SetNodeNameCommand.h"
#include "Node.h"

SetNodeNameCommand::SetNodeNameCommand(const shared_ptr<SceneNode> & node, const string & newName, const string & lastName)
	: mNode(node), mLastName(lastName), mName(newName)
{
	mDesc = StringBuilder("SetName(") << newName << ")";
}

void SetNodeNameCommand::Execute()
{
	if (auto node = mNode.lock())
	{
		node->SetName(mName);
	}
}

void SetNodeNameCommand::Undo()
{
	if (auto node = mNode.lock())
	{
		node->SetName(mLastName);
	}
}
