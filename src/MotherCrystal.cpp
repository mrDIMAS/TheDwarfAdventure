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
#include "MotherCrystal.h"
#include "Sprite.h"
#include "Renderer.h"
#include "Light.h"
#include "Location.h"

MotherCrystal::MotherCrystal()
{
}

MotherCrystal::MotherCrystal(const shared_ptr<Location> &location) : mLocation(location)
{
	const auto scene = location->GetScene();

	mNode = scene->CreateSprite();
	mNode->SetTexture(game->GetRenderer()->RequestTexture("data/textures/objects/mothercrystal.png"));
	mNode->SetSize({ Width, Height });
	mNode->SetColor({ 120, 120, 120 });
	mNode->SetOriginInCenter();

	mLight = scene->CreateLight();
	mLight->AttachTo(mNode);
	mLight->SetRadius(0);
	mLight->SetColor({ 255, 100, 100 });
}

MotherCrystal::~MotherCrystal()
{
	DBG_ECHO();
}

void MotherCrystal::SetPosition(const Vec2f &p)
{
	mNode->SetLocalPosition(p);
	mPosition = p;
}

Vec2f MotherCrystal::GetPosition() const
{
	return mNode->GetGlobalPosition();
}

Vec2f MotherCrystal::GetSize() const
{
	return mNode->GetSize();
}

void MotherCrystal::Serialize(Serializer &sav)
{
	sav &mNode;
	sav &mActivated;
	sav &mPosition;
	sav &mOffset;
	sav &mFloatUp;
	sav &mLight;
	sav &mLocation;
}

void MotherCrystal::Activate()
{
	mActivated = true;
	mNode->SetColor({ 220, 220, 220 });
	mLight->SetRadius(150);
}

bool MotherCrystal::IsActive() const
{
	return mActivated;
}

shared_ptr<Scene> MotherCrystal::GetScene() const
{
	return mNode->GetScene();
}

void MotherCrystal::Update()
{
	constexpr float floatSpeed = 0.15f;
	if (mFloatUp)
	{
		mOffset.y -= floatSpeed;
		if (mOffset.y <= -MaxFloatOffset)
		{
			mFloatUp = false;
		}
	}
	else
	{
		mOffset.y += floatSpeed;
		if (mOffset.y >= MaxFloatOffset)
		{
			mFloatUp = true;
		}
	}
	mNode->SetLocalPosition(mPosition + mOffset);
}

shared_ptr<Location> MotherCrystal::GetLocation() const
{
	return mLocation.lock();
}
