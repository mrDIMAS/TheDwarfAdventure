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
#include "Trigger.h"
#include "Node.h"

Trigger::Trigger() : mType(Trigger::Type::Circle), mRadius(0.0f), mInside(false)
{
}

Trigger::~Trigger()
{
	DBG_ECHO();
}

void Trigger::SetWatchObject(const shared_ptr<SceneNode> &obj)
{
	mObject = obj;
}

void Trigger::Update()
{
	if (!mObject.expired())
	{
		bool intersection = false;
		auto obj = mObject.lock();
		sf::FloatRect rect = { };
		if (mType == Trigger::Type::Circle)
		{
			intersection = Math::IsCircleRectIntersection(
				obj->GetGlobalPosition(), obj->GetSize(), mPosition, mRadius);
		}
		if (mType == Trigger::Type::Rectangle)
		{
			intersection =
				Math::IsRectRectIntersection(obj->GetGlobalPosition(), obj->GetSize(), mPosition, mSize);
		}
		if (intersection)
		{
			OnInside();
			if (!mInside)
			{
				OnEnter();
				mInside = true;
			}
		}
		else
		{
			if (mInside)
			{
				OnLeave();
				mInside = false;
			}
		}
	}
}
