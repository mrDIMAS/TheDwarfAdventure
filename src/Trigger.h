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

#pragma once

#include "Event.h"

class SceneNode;

class Trigger
{
public:
	enum class Type { Circle, Rectangle };

private:
	Type mType;
	float mRadius;
	Vec2f mPosition;
	Vec2f mSize;
	weak_ptr<SceneNode> mObject;
	bool mInside;

public:
	Event OnEnter;
	Event OnLeave;
	Event OnInside;
	Trigger();
	~Trigger();
	void SetWatchObject(const shared_ptr<SceneNode> &obj);
	void Update();
};
