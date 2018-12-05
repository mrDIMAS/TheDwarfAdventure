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
#include "Remains.h"
#include "Serializer.h"
#include "Sprite.h"
#include "Inventory.h"
#include "Renderer.h"

Remains::Remains()
{
}

Remains::Remains(const shared_ptr<Scene> &scene, const Vec2f &pos, const shared_ptr<Inventory> &inv)
	: mInventory(inv)
{
	mModel = scene->CreateSprite();
	mModel->SetLocalPosition(pos);
	mModel->SetSize({ 13, 13 });
	mModel->SetTexture(game->GetRenderer()->RequestTexture("data/textures/objects/remains.png"));
}

Remains::~Remains()
{
	mModel->RemoveFromScene();
}

void Remains::Serialize(Serializer &sav)
{
	sav &mModel;
	sav &mInventory;
}

shared_ptr<Inventory> Remains::GetInventory() const
{
	return mInventory;
}

Vec2f Remains::GetPosition() const
{
	return mModel->GetLocalPosition();
}

bool Remains::IsEmpty() const
{
	return mInventory->IsEmpty();
}
