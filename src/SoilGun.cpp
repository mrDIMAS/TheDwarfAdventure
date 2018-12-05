

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
#include "SoilGun.h"
#include "Debris.h"
#include "Level.h"
#include "Renderer.h"
#include "Actor.h"

void SoilGun::OnUpdate()
{
}

void SoilGun::OnSerialize(Serializer &sav)
{
	sav &mSoilTexture;
}

SoilGun::SoilGun()
{
}

SoilGun::SoilGun(const shared_ptr<Location> &location) : Weapon(location, ItemType::Weapon_SoilGun)
{
	const auto scene = location->GetScene();
	const auto renderer = game->GetRenderer();

	mModel = scene->CreateSprite();
	mModel->AttachTo(mNode);
	mModel->SetTexture(renderer->RequestTexture("data/textures/weapons/shotgun.png"));
	mModel->SetSize(30, 12);
	mModel->SetLocalOrigin(5, 6);

	mShootPoint = { 29, 6 };

	mShowCrosshair = false;

	mSoilTexture = renderer->RequestTexture("data/textures/terrain_textures/soil_3.png");
}

SoilGun::~SoilGun()
{
	DBG_ECHO();
}

bool SoilGun::Shoot(const Vec2f &direction)
{
	for (int i = 0; i < 3; ++i)
	{
		const float piece = (Debris::Radius * 2.0f) / mSoilTexture->GetWidth();
		const float upLimit = 1.0f - piece;
		auto location = GetOwner()->GetLocation();
		const Vec2f offset = Vec2f(Math::Rnd(0.0f, 6.0f), Math::Rnd(-6.0f, 6.0f));
		sf::FloatRect randomPiece = { Math::Rnd(0.0f, upLimit), Math::Rnd(0.0f, upLimit), piece, piece };
		auto debris =
			make_shared<Debris>(location, GetShootPointGlobal() + offset, mSoilTexture, randomPiece);
		debris->SetVelocity(6.0f * direction);
		location->AddDebris(debris);
	}
	return true; // fix
}
