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
#include "Shield.h"
#include "Sprite.h"
#include "Location.h"
#include "Renderer.h"

Shield::Shield()
{
}

Shield::Shield(const shared_ptr<Location> &location, ItemType type) : Item(location, type)
{
	const auto scene = location->GetScene();

	auto spr = scene->CreateSprite();
	spr->SetTexture(game->GetRenderer()->RequestTexture("data/textures/weapons/shield.png"));
	spr->SetSize({ 10, 35 });
	spr->SetOriginInCenter();
	mNode = spr;
}

void Shield::Raise(bool state)
{
	mRaised = state;
	mNode->SetVisible(state);
}

bool Shield::IsRaised() const
{
	return mRaised;
}

const HitBox & Shield::GetHitbox() const
{
	return mHitbox;
}

void Shield::Serialize(Serializer &sav)
{
	Item::Serialize(sav);
	sav &mRaised;
	sav &mHitbox;
}
