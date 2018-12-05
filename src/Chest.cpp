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
#include "Chest.h"
#include "Sprite.h"
#include "Scene.h"
#include "Game.h"
#include "Renderer.h"
#include "Inventory.h"

Chest::Chest()
{
}

Chest::Chest(const shared_ptr<Scene> &scene)
{
	mModel = scene->CreateSprite();
	mModel->SetSize(30, 15);
	mModel->SetTexture(game->GetRenderer()->RequestTexture("data/textures/objects/chest.png"));
	mModel->SetOriginInCenter();

	//auto body = scene->GetSimulationIsland()->CreateParticle(10);
	//body->SetFriction(0.3f);
	//body->SetCollisionGroup(CG_CHEST);
	//body->SetCollisionMask(CG_CHEST | CG_WATER | CG_DEBRIS);
	//mModel->SetBodyAtCurrentPosition(body);

	mInventory = make_shared<Inventory>();
}

Chest::~Chest()
{
	DBG_ECHO();
}

shared_ptr<Inventory> Chest::GetInventory()
{
	return mInventory;
}

void Chest::Serialize(Serializer &sav)
{
	sav &mModel;
	sav &mInventory;
}

shared_ptr<Sprite> Chest::GetModel()
{
	return mModel;
}

void Chest::SetPosition(const Vec2f &pos)
{
	if (mModel->GetBody())
	{
		mModel->GetBody()->SetPosition(pos);
	}
	else
	{
		mModel->SetLocalPosition(pos);
	}
}
