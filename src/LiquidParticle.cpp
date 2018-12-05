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
#include "LiquidParticle.h"
#include "Level.h"
#include "Renderer.h"
#include "Terrain.h"
#include "Sprite.h"
#include "Actor.h"

LiquidParticle::LiquidParticle()
	: mType(Type::Unknown), mLifeTime(600), mPaintTime(25), mContactedWithTerrain(false)
{
}

LiquidParticle::LiquidParticle(const shared_ptr<Location> &location, const Type &type, const Vec2f &position, const Vec2f &velocity)
	: mType(type), mLifeTime(600), mPaintTime(25), mContactedWithTerrain(false), mLocation(location)
{
	const auto scene = location->GetScene();
	const auto renderer = game->GetRenderer();

	const float radius = 2;
	mModel = scene->CreateSprite();
	mModel->SetBlowable(false);
	mModel->SetSize(radius * 2, radius * 2);

	switch (mType)
	{
	case Type::Blood:
		mModel->SetTexture(renderer->RequestTexture("data/textures/particles/blood.png"));
		break;
	case Type::Water:
		mModel->SetTexture(renderer->RequestTexture("data/textures/particles/water.png"));
		break;
	case Type::Acid:
		mModel->SetTexture(renderer->RequestTexture("data/textures/particles/acid.png"));
		break;
	}

	auto nvel = Math::Normalized(velocity);
	auto body = scene->GetSimulationIsland()->CreateParticle(radius);
	body->AddForce(
		Math::Normalized(Vec2f(Math::Rnd(-0.2f, 0.2f), Math::Rnd(-0.2f, 0.2f)) + nvel) * 20000.0f);
	body->ResetFlags(BF_RESPONSE_ENABLED);
	body->SetCollisionMask(0); // no collision
	mModel->SetBodyAtCurrentPosition(body);

	body->SetPosition(position);
}

LiquidParticle::~LiquidParticle()
{
	mModel->RemoveFromScene();
	DBG_ECHO();
}

void LiquidParticle::Update()
{
	auto scene = mModel->GetScene();
	auto location = mLocation.lock();

	if (!mContactedWithTerrain)
	{
		auto &body = mModel->GetBody();
		for (int i = 0; i < body->GetContactCount(); ++i)
		{
			if (body->GetContact(i).mCell)
			{
				mContactedWithTerrain = true;
				mVelocity = body->GetVelocity();
				mModel->RemoveBody();
				break;
			}
		}

		if (mType == Type::Acid)
		{
			for (auto &actor : location->GetActorList())
			{
				if (actor->IsAnyHitBoxIntersected(mModel->GetGlobalPosition(), mModel->GetSize().x))
				{
					Damage dmg;					
					dmg.mCount = 3;
					dmg.mClass = DamageClass::Chemical;					
					actor->ApplyDamage(dmg);
					mLifeTime = 0;
					break;
				}
			}
		}
	}
	else
	{
		mModel->SetVisible(false);

		// prepare args for masking
		MaskOperation args;
		if (mType == Type::Acid)
		{
			args.mFirstPassMask = args.mFirstPassMask | PA_ACID;
		}
		args.mFirstPassBitwiseOperation = BitwiseOp::Or;
		args.mPosition = mModel->GetGlobalPosition();
		args.mBounds = Bounds::Circle;
		args.mRadius = mModel->GetSize().x * 0.5f;
		args.mBlendOp = BlendOperation::Replace;
		args.mBrushMode = BrushMode::Insert;

		const float slowDownRate = 0.25f;
		if (mVelocity.x > 0.0f)
		{
			mVelocity.x -= slowDownRate;
		}
		if (mVelocity.x < 0.0f)
		{
			mVelocity.x += slowDownRate;
		}
		if (mVelocity.y < 0.4f)
		{
			mVelocity.y += slowDownRate;
		}
		if (mVelocity.y > 0.4f)
		{
			mVelocity.y -= 3 * slowDownRate;
		}

		mModel->LocalMove(mVelocity.x + Math::Rnd(-0.5f, 0.5f), mVelocity.y);

		if (mModel->GetTexture())
		{
			auto texture = mModel->GetTexture();
			args.SetBrushImage(mModel->GetTexture());
		}

		// do masking
		scene->GetTerrain()->Mask(args);

		--mPaintTime;
	}

	--mLifeTime;

	auto position =
		mModel->GetBody() ? mModel->GetBody()->GetPosition() : mModel->GetGlobalPosition();
	auto polar = Math::CartesianToPolar(position - mLastPosition);
	mModel->SetLocalRotation(polar.mAngle - 90);
	mLastPosition = position;
}

bool LiquidParticle::IsDead() const
{
	return mLifeTime < 0 || mPaintTime < 0;
}

void LiquidParticle::SetVelocity(const Vec2f &vel)
{
	mModel->GetBody()->SetVelocity(vel);
}
