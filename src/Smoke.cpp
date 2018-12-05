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
#include "Smoke.h"
#include "Node.h"
#include "Scene.h"
#include "Sprite.h"
#include "Renderer.h"

Smoke::Smoke()
{
}

Smoke::Smoke(const shared_ptr<Scene> &scene, const Vec2f &position) : Smoke()
{
	const float radius = Math::Rnd(7.0f, 13.0f);

	static array<string, 3> smoke = { "data/textures/particles/smoke1.png",
		"data/textures/particles/smoke2.png", "data/textures/particles/smoke3.png" };

	mNode = scene->CreateSprite();
	mNode->SetBlowable(false);
	mNode->SetSize(radius * 2, radius * 2);
	mNode->SetTexture(game->GetRenderer()->RequestTexture(Math::RandomElement(smoke)));
	mNode->SetColor({ 180, 180, 180, 60 });
	mNode->SetOriginInCenter();
	mNode->SetLocalRotation(Math::Rnd(-180.0f, 180.0f));

	auto body = scene->GetSimulationIsland()->CreateParticle(radius);
	body->SetFlags(BF_IGNORE_PP_CONTACT_INFO);
	body->ResetFlags(BF_RESPONSE_ENABLED);
	mNode->SetBodyAtCurrentPosition(body);
	body->SetGravity({ 0, -Body::DefaultGravityY / 4 });
	body->SetCollisionGroup(CG_SMOKE);
	body->SetCollisionMask(CG_NONE);

	body->AddForce({ 3000.0f * Math::Rnd(-1.0f, 1.0f), 3000.0f * Math::Rnd(0.1f, 1.0f) });

	body->SetPosition(position);

	mTurnDir = Math::Rnd(-1.0f, 1.0f);
}

Smoke::~Smoke()
{
	mNode->RemoveFromScene();
}

void Smoke::Update()
{
	--mLifeTime;
	const float k = 1.008f;
	auto scale = mNode->GetLocalScale();
	mNode->SetLocalScale(scale.x * k, scale.y * k);
	mNode->Turn(3 * mTurnDir);
	if (mLifeTime < 0)
	{
		mNode->Fade(0.9f);
	}
}

bool Smoke::IsDead() const
{
	return mNode->GetColor().a <= 3;
}

void Smoke::Serialize(Serializer &sav)
{
	sav &mNode;
	sav &mLifeTime;
}
