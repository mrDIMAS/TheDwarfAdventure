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
#include "FogGate.h"
#include "Node.h"
#include "Scene.h"
#include "ParticleSystem.h"
#include "Renderer.h"
#include "Location.h"
#include "Terrain.h"

FogGate::FogGate()
{
}

FogGate::FogGate(const shared_ptr<Location>& location, const sf::FloatRect & rect, FogType type)
	: mRect(rect)
	, mType(type)
	, mLocation(location)
{
	mModel = location->GetScene()->CreateParticleSystem();
	mModel->CreateParticles(256);
	mModel->SetTexture(game->GetRenderer()->RequestTexture("data/textures/particles/generic.png"));
	mModel->SetBoundsType(ParticleSystem::BoundsType::Rect);
	mModel->SetRectangle({ 0, 0, rect.width, rect.height });
	mModel->SetLocalPosition({ rect.left, rect.top });
	mModel->SetParticleSize(5);
	mModel->SetMaxSpeed({ 0.015f, 0.03f });
	mModel->SetMinSpeed({ -0.015f, -0.05f });
}

FogGate::~FogGate()
{
	mModel->RemoveFromScene();
}

void FogGate::Serialize(Serializer & sav)
{
	sav &mModel;
	sav &mRect;
	sav &mType;
	sav &mLocation;
}

void FogGate::Reveal() const
{
	MaskOperation op;
	op.mEditorAccess = true;
	op.mBounds = Bounds::Rect;
	op.mRectSize = { mRect.width, mRect.height + 20 };
	op.mPosition = { mRect.left, mRect.top - 20 };
	op.mFirstPassBitwiseOperation = BitwiseOp::And;
	op.mFirstPassMask = ~(PA_VISIBLE | PA_ACID | PA_LAVA | PA_COLLIDABLE | PA_RAYTRACEABLE);
	mLocation.lock()->GetScene()->GetTerrain()->Mask(op);
}
