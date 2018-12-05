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
#include "Debris.h"
#include "DebrisAtlas.h"
#include "Level.h"
#include "LiquidParticle.h"
#include "Terrain.h"

Debris::Debris()
{
}

Debris::~Debris()
{
	if (!mLocation.expired())
	{
		auto location = mLocation.lock();
		location->GetScene()->RemoveNode(mNode);
		location->GetLevel()->GetDebrisAtlas()->Free(mAtlasPieceId);
	}
	DBG_ECHO();
}

Debris::Debris(const shared_ptr<Location> &location, const Vec2f &position, const shared_ptr<Texture> &texture,
	const sf::FloatRect &textureRect, int atlasPieceId)
	: mLocation(location), mAtlasPieceId(atlasPieceId)
{
	auto &scene = location->GetScene();

	mNode = scene->CreateSprite();
	mNode->SetBlowable(false);
	mNode->SetSize(Radius * 2, Radius * 2);
	mNode->SetTexture(texture);
	mNode->SetTextureRectangle(textureRect);
	mNode->SetOriginInCenter();

	auto body = scene->GetSimulationIsland()->CreateParticle(static_cast<float>(Radius));
	body->SetFlags(BF_IGNORE_PP_CONTACT_INFO);
	body->SetFriction(0.9f);
	body->SetCollisionGroup(CG_DEBRIS);
	mNode->SetBodyAtCurrentPosition(body);

	mLiquidCastingInterval = Math::Rnd(Debris::LiquidCastingTime, 2 * Debris::LiquidCastingTime);

	body->SetPosition(position);
}

void Debris::Update()
{
	THROW_IF_EXPIRED(mLocation);

	// merge with terrain
	auto location = mLocation.lock();

	--mLifeTime;
	--mLiquidCastingInterval;

	if (mLiquidCastingInterval <= 0 && mLifeTime > 0)
	{
		if (mCastAcid || mCastBlood)
		{
			auto type = mCastBlood ? LiquidParticle::Type::Blood : LiquidParticle::Type::Acid;
			auto lqp = make_shared<LiquidParticle>(location, type, mNode->GetGlobalPosition(), Vec2f(0, 0));
			location->AddLiquidParticle(lqp);
			lqp->SetVelocity(mNode->GetBody()->GetVelocity());
			mLiquidCastingInterval = Math::Rnd(Debris::LiquidCastingTime, 2 * Debris::LiquidCastingTime);
		}
	}

	if (mLifeTime <= 0 && !mMerged)
	{
		const int maxAliveTime = 10 * 60; // in frames
		auto terrain = location->GetScene()->GetTerrain();

		// keep piece alive at least maxAliveTime
		bool contactedWithTerrain = mLifeTime < -maxAliveTime;

		// look for contact with terrain
		auto &body = mNode->GetBody();
		for (int i = 0; i < body->GetContactCount(); ++i)
		{
			if (body->GetContact(i).mCell)
			{
				contactedWithTerrain = true;
			}
		}

		if (contactedWithTerrain)
		{
			// prepare args for masking
			MaskOperation args;
			args.mFirstPassBitwiseOperation = BitwiseOp::Or;
			args.mFirstPassMask = PA_COLLIDABLE | PA_EDITABLE | PA_RAYTRACEABLE | PA_VISIBLE;
			args.mSecondPassBitwiseOperation = BitwiseOp::And;
			args.mSecondPassMask = ~(PA_LAVA | PA_ACID);
			args.mBorderMask = PA_NONE;
			args.mPosition = mNode->GetBody()->GetPosition();
			args.mBounds = Bounds::Rect;
			args.mBrushMode = BrushMode::Insert;

			if (mNode->GetTexture())
			{
				auto texture = mNode->GetTexture();
				auto tr = mNode->GetTextureRectangle();
				args.mRectSize.x = tr.width * texture->GetWidth();
				args.mRectSize.y = tr.height * texture->GetHeight();

				args.SetBrushImage(mNode->GetTexture());
				args.SetImageRect(sf::IntRect(static_cast<int>(tr.left * texture->GetWidth()),
					static_cast<int>(tr.top * texture->GetHeight()),
					static_cast<int>(tr.width * texture->GetWidth()),
					static_cast<int>(tr.height * texture->GetHeight())));
			}

			// do masking
			terrain->Mask(args);

			mMerged = true;
		}
	}
}

bool Debris::IsDead() const
{
	return mMerged;
}

void Debris::SetLocation(const shared_ptr<Location> &location)
{
	mLocation = location;
}

void Debris::Serialize(Serializer &sav)
{
	sav &mLocation;
	sav &mNode;
	sav &mLifeTime;
	sav &mMerged;
	sav &mCastAcid;
	sav &mCastBlood;
	sav &mLiquidCastingInterval;
}

shared_ptr<Sprite> Debris::GetNode()
{
	return mNode;
}

shared_ptr<Body> Debris::GetBody() const
{
	return mNode->GetBody();
}

void Debris::EnableBloodCasting(bool state)
{
	mCastBlood = state;
	mCastAcid = false;
}

void Debris::EnableAcidCasting(bool state)
{
	mCastAcid = state;
	mCastBlood = false;
}

void Debris::SetVelocity(const Vec2f &force)
{
	mNode->GetBody()->SetVelocity(force);
}
