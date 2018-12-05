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
#include "AttachCommand.h"
#include "Node.h"

ed::AttachCommand::AttachCommand(const shared_ptr<SceneNode>& node, const shared_ptr<SceneNode>& parent)
	: mNode(node)
	, mParent(parent)
{

}

void ed::AttachCommand::Execute()
{
	Swap();
}

void ed::AttachCommand::Undo()
{
	Swap();
}

void ed::AttachCommand::Swap()
{
	const shared_ptr<SceneNode> temp = mNode->GetParent();
	if (!mParent)
	{
		mNode->Detach();

		float r;
		Vec2f p, s;
		Math::DecomposeTransform(mNode->GetGlobalTransform(), &p, &s, &r);

		mNode->SetLocalPosition(p);
		mNode->SetLocalScale(s);
		mNode->SetLocalRotation(r);
	}
	else
	{
		mNode->AttachTo(mParent);

		float r;
		Vec2f p, s;
		auto relativeTransform = mParent->GetGlobalTransform().getInverse() * mNode->GetGlobalTransform();
		Math::DecomposeTransform(relativeTransform, &p, &s, &r);
		mNode->SetLocalPosition(p);
		mNode->SetLocalScale(s);
		mNode->SetLocalRotation(r);
	}
	mParent = temp;
}

