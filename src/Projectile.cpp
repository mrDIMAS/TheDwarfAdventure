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
#include "Projectile.h"
#include "Level.h"
#include "Renderer.h"
#include "Actor.h"
#include "Debris.h"
#include "DebrisAtlas.h"
#include "Light.h"
#include "Terrain.h"
#include "LiquidParticle.h"
#include "Sprite.h"
#include "Sound.h"
#include "VisualEffect.h"
#include "Smoke.h"

void Projectile::Explode()
{
	auto scene = mModel->GetScene();
	auto terrain = scene->GetTerrain();
	auto level = game->GetLevel();
	auto location = mLocation.lock();
	auto &debrisAtlas = level->GetDebrisAtlas();
	auto &desc = game->GetItemDatabase()->Get(mType);
	// create debris first

	// Pixel can become debris only if it has this attributes
	const uint8_t gatherMask = PA_VISIBLE | PA_EDITABLE;
	static array<sf::Color, Cell::PixelCount> tempBuffer;
	auto position = mModel->GetGlobalPosition();
	vector<CellTraceResult> intersectedPixelsPositions;
	array<Cell *, 9> cells;
	const int cellCount = terrain->GetCellsNear(mModel->GetGlobalPosition(), mDamageRadius, cells);
	for (int i = 0; i < cellCount; ++i)
	{
		if (cells[i])
		{
			constexpr int gatherStep = 4;
			cells[i]->TransferPixelsFromGridToLinearOrder(tempBuffer);
			intersectedPixelsPositions.clear();
			cells[i]->GatherPixelsInsideCircle(
				position, mDamageRadius, gatherStep, gatherStep, intersectedPixelsPositions, gatherMask);
			for (auto &rt : intersectedPixelsPositions)
			{
				sf::FloatRect texCoords;
				const int pieceIndex = debrisAtlas->Pack(
					tempBuffer.data(), Cell::Size, Cell::Size, rt.mRelative.x, rt.mRelative.y, texCoords);
				const auto debris = make_shared<Debris>(
					location, rt.mPosition, debrisAtlas->GetTexture(), texCoords, pieceIndex);
				location->AddDebris(debris);
			}
		}
	}

	terrain->MeltCicle(mModel->GetGlobalPosition(), mDamageRadius, sf::Color(255, 0, 0));

	// handling splash damage is slightly complicated
	// we should check direct visibility of actor from explosion point and
	// intersection of explosion blob bounds with actor
	for (auto &actor : location->GetActorList())
	{
		// firstly check if any hitbox is intersected by explosion circle
		if (actor->IsAnyHitBoxIntersected(position, mDamageRadius))
		{
			// adjust this to make ray tracing more accurate
			const int numRays = 10;
			const auto dir = Math::Normalized(actor->GetPosition() - position);
			const auto perp = Math::Perpendicular(dir);
			auto p1 = actor->GetPosition() - perp * mDamageRadius;
			auto p2 = position - perp * mDamageRadius;
			const auto d = perp * 2.0f * mDamageRadius / static_cast<float>(numRays);
			int intCount = 0;
			for (int i = 0; i < numRays; ++i)
			{
				const auto traceResult = terrain->TraceRay(p2, mDamageRadius * Math::Normalized(p1 - p2));
				if (traceResult.mIntersection)
				{
					++intCount;
				}
				p1 += d;
				p2 += d;
			}
			// more rays that intersected terrain -> less damage
			const float kDamage = 1.0f - intCount / static_cast<float>(numRays);
			const float damage = (desc.mDamage * (1 + mDamageMod)) * kDamage;
			// and now apply damage to the actor
			Damage dmg;
			dmg.mWho = mOwner.lock();
			dmg.mCount = damage;
			dmg.mClass = desc.mDamageClass;
			dmg.mFlags = (DamageType)desc.mDamageType;
			actor->ApplyDamage(dmg);
			const float r = actor->GetBody()->GetRadius();
			// apply force to actor (rocket jumping)
			actor->AddForce(30000.0f * dir);
			// create blood
			for (int i = 0; i < 25; ++i)
			{
				const Vec2f bloodPosition =
					actor->GetPosition() + Vec2f(Math::Rnd(-r, r), Math::Rnd(-r, r));
				location->AddLiquidParticle(make_shared<LiquidParticle>(location, LiquidParticle::Type::Blood, bloodPosition, 1400.5f * dir));
			}
		}
	}
	VisualEffect::MakeExplosion(scene, mModel->GetGlobalPosition());
	// break unstable zones
	const Vec2f zoneSize = { 256, 256 };
	location->AnalyzeAndBreakUnstableZones(mModel->GetGlobalPosition() - zoneSize * 0.5f, zoneSize);
}

Projectile::Projectile()
{
}

Projectile::Projectile(const shared_ptr<Location> &location, const shared_ptr<Actor> &owner, ItemType type,
	const Vec2f &position, const Vec2f &velocity, float damageMod) : mLocation(location)
{
	const auto scene = location->GetScene();
	mOwner = owner;
	mType = type;
	mModel = scene->CreateSprite();
	mDirection = Math::Normalized(velocity);
	mSpeed = 1.0f;
	mDamageMod = damageMod;
	const auto renderer = game->GetRenderer();
	if (mType == ItemType::Projectile_Grenade)
	{
		auto body = scene->GetSimulationIsland()->CreateParticle(6.0f);
		body->SetCollisionGroup(CG_PROJECTILE);
		mModel->SetBodyAtCurrentPosition(body);
		mModel->SetTexture(renderer->RequestTexture("data/textures/weapons/grenade.png"));
		mModel->SetSize(8.0f, 8.0f);
		mDamageRadius = 50.0f;
		mFlags = PRF_EXPLOSIVE;
	}
	else if (mType == ItemType::Projectile_Rocket)
	{
		auto body = scene->GetSimulationIsland()->CreateParticle(6.0f);
		body->SetCollisionGroup(CG_PROJECTILE);
		mModel->SetBodyAtCurrentPosition(body);
		mModel->SetTexture(renderer->RequestTexture("data/textures/weapons/rocket.png"));
		mModel->SetSize(19.0f, 7.0f);
		mModel->SetLocalRotation(Math::CartesianToPolar(velocity).mAngle);
		mDamageRadius = 50.0f;
		mSpeed = 10.0f;
		// load sounds
		mIdleSound = scene->EmitSound("data/sounds/rocket_loop.ogg", mModel);
		mIdleSound->SetLoop(true);
		auto fire = scene->CreateSprite();
		fire->SetSize(12, 7);
		fire->SetLocalPosition(-fire->GetSize().x, 0);
		fire->SetTexture(renderer->RequestTexture("data/textures/weapons/rocketFire.png"));
		fire->GetAnimation().LoadFromFile("data/animations/rocket_fire.fra");
		fire->GetAnimation().SetEnabled(true);
		fire->AttachTo(mModel);
		// create light
		auto light = scene->CreateLight();
		light->SetRadius(25);
		light->SetColor({ 204, 236, 244 });
		light->AttachTo(fire);
		mFlags = PRF_FLYING | PRF_EXPLOSIVE | PRF_DIE_ON_TERRAIN_IMPACT;
	}
	else if (mType == ItemType::Projectile_Harpoon)
	{
		auto body = scene->GetSimulationIsland()->CreateParticle(6.0f);
		body->SetCollisionGroup(CG_PROJECTILE);
		mModel->SetBodyAtCurrentPosition(body);
		mModel->SetTexture(renderer->RequestTexture("data/textures/weapons/harpoon.png"));
		mModel->SetSize(22.0f, 5.0f);
		mDamageRadius = 50.0f;
		mSpeed = 10.0f;
		mModel->SetLocalRotation(Math::CartesianToPolar(velocity).mAngle);
		// create light
		auto light = scene->CreateLight();
		light->SetRadius(25);
		light->SetColor({ 204, 236, 244 });
		light->AttachTo(mModel);
		mFlags = PRF_FLYING | PRF_ATTACH_MODEL_ON_IMPACT_POINT | PRF_IMPACT_DAMAGE | PRF_DIE_ON_TERRAIN_IMPACT;
	}
	else if (mType == ItemType::Projectile_AcidBlob)
	{
		auto body = scene->GetSimulationIsland()->CreateParticle(6.0f);
		body->SetCollisionGroup(CG_PROJECTILE);
		mModel->SetBodyAtCurrentPosition(body);
		mModel->SetTexture(renderer->RequestTexture("data/textures/weapons/acid_blob.png"));
		mModel->SetSize(10, 10);
		mDamageRadius = 50.0f;
		mSpeed = 10.0f;
		// create light
		auto light = scene->CreateLight();
		light->SetRadius(25);
		light->SetColor({ 188, 255, 109 });
		light->AttachTo(mModel);
		light->SetLocalPosition(mModel->GetSize() * 0.5f);
		mFlags = PRF_DIE_ON_TERRAIN_IMPACT;
	}
	else if (mType == ItemType::Projectile_Fireball)
	{
		auto body = scene->GetSimulationIsland()->CreateParticle(15.0f);
		body->SetCollisionGroup(CG_PROJECTILE);
		mModel->SetBodyAtCurrentPosition(body);
		mModel->SetTexture(renderer->RequestTexture("data/textures/weapons/fireball.png"));
		mModel->SetSize(30, 30);
		mModel->SetOriginInCenter();
		mDamageRadius = 50.0f;
		mSpeed = 5.5f;
		// create light
		auto light = scene->CreateLight();
		light->SetRadius(50);
		light->SetColor({ 255, 127, 39 });
		light->AttachTo(mModel);
		light->SetLocalPosition(mModel->GetSize() * 0.5f);
		mFlags = PRF_FLYING | PRF_EXPLOSIVE | PRF_DIE_ON_TERRAIN_IMPACT;
	}
	else if (mType == ItemType::Projectile_Dynamite)
	{
		auto body = scene->GetSimulationIsland()->CreateParticle(5.0f);
		body->SetCollisionGroup(CG_PROJECTILE);
		body->SetFriction(1.0f);
		mModel->SetBodyAtCurrentPosition(body);
		mModel->SetTexture(renderer->RequestTexture("data/textures/weapons/dynamite.png"));
		mModel->SetSize(4, 16);
		mModel->SetOriginInCenter();
		mDamageRadius = 40.0f;
		mSpeed = 5.5f;
		// create light
		auto light = scene->CreateLight();
		light->SetRadius(50);
		light->SetColor({ 255, 127, 39 });
		light->AttachTo(mModel);
		light->SetLocalPosition(mModel->GetSize() * 0.5f);
		mFlags = PRF_EXPLOSIVE;
	}
	if (mModel->GetBody())
	{
		auto body = mModel->GetBody();
		body->SetPosition(position);
		body->SetVelocity(velocity);
		body->SetCollisionGroup(CG_PROJECTILE);
		body->SetCollisionMask(CG_PROJECTILE);
	}
}

Projectile::~Projectile()
{
	if (!(mFlags & PRF_ATTACH_MODEL_ON_IMPACT_POINT))
	{
		mModel->RemoveFromScene();
	}
	DBG_ECHO();
}

void Projectile::Update()
{
	auto scene = mModel->GetScene();
	auto terrain = scene->GetTerrain();
	auto level = game->GetLevel();
	auto location = mLocation.lock();
	auto owner = mOwner.lock();
	if (mIdleSound)
	{
		mIdleSound->SetPosition(mModel->GetGlobalPosition());
	}
	if (mLifeTime > 0)
	{
		if (auto body = mModel->GetBody())
		{
			const auto bodyPos = body->GetPosition();
			const auto bodyRad = body->GetRadius();
			for (auto &actor : location->GetActorList())
			{
				// Self-damage is disabled
				if (actor == owner)
				{
					continue;
				}
				for (auto & hitbox : actor->GetHitBoxes())
				{
					if (hitbox.IsIntersects(bodyPos, bodyRad))
					{
						mLifeTime = 0;
						if (mFlags & PRF_ATTACH_MODEL_ON_IMPACT_POINT)
						{
							auto bodypart = hitbox.GetRelatedSprite();
							if (!bodypart)
							{
								continue;
							}
							mModel->RemoveBody();
							auto relativeTransform = bodypart->GetGlobalTransform().getInverse() * mModel->GetGlobalTransform();
							Vec2f p, s;
							float r;
							Math::DecomposeTransform(relativeTransform, &p, &s, &r);
							mModel->SetLocalScale(s);
							mModel->SetLocalPosition(p);
							mModel->SetLocalRotation(r);
							mModel->AttachTo(bodypart);
							// force update transforms 
							mModel->CalculateTransforms();
							mModel->SetLifeTime(300);
						}
						if (mFlags & PRF_IMPACT_DAMAGE)
						{
							auto &desc = game->GetItemDatabase()->Get(mType);
							Damage dmg;
							dmg.mWho = mOwner.lock();
							dmg.mCount = desc.mDamage * (1 + mDamageMod);
							dmg.mClass = desc.mDamageClass;
							dmg.mFlags = (DamageType)desc.mDamageType;
							actor->ApplyDamage(dmg);
						}
						break;
					}
				}
			}
			if (mFlags & PRF_DIE_ON_TERRAIN_IMPACT)
			{
				for (int i = 0; i < body->GetContactCount(); ++i)
				{
					if (body->GetContact(i).mCell)
					{
						mLifeTime = 0;
						break;
					}
				}
			}
			if (mFlags & PRF_FLYING)
			{
				body->SetVelocity(mDirection * mSpeed);
				if (mType == ItemType::Projectile_Rocket)
				{
					const auto smoke = make_shared<Smoke>(scene, mModel->GetGlobalPosition());
					location->AddSmoke(smoke);
					mSmokeTimer = 6;
				}
			}
		}
		--mSmokeTimer;
	}
	if (mLifeTime <= 0 && !mDead)
	{
		if (mIdleSound)
		{
			mIdleSound->Stop();
		}
		// explode projectile
		if (mFlags & PRF_EXPLOSIVE)
		{
			Explode();
		}
		// add special effects
		if (mType == ItemType::Projectile_AcidBlob)
		{
			// create acid particles
			for (int i = 0; i < 70; ++i)
			{
				const float angle = Math::Rnd(0.0f, 2.0f * Math::Pi);
				const Vec2f direction = { cos(angle), sin(angle) };
				const float force = Math::Rnd(10000.0f, 15000.0f);
				location->AddLiquidParticle(make_shared<LiquidParticle>(location, LiquidParticle::Type::Acid,
					mModel->GetGlobalPosition(), force * direction));
			}
		}
		if (!(mFlags & PRF_ATTACH_MODEL_ON_IMPACT_POINT))
		{
			mModel->SetVisible(false);
			mModel->SetLifeTime(60);
		}
		mModel->RemoveBody();
		mDead = true;
	}
	--mLifeTime;
}

bool Projectile::IsDead() const
{
	return mLifeTime <= 0 && mDead;
}

void Projectile::Serialize(Serializer &sav)
{
	sav &mModel;
	sav &mLifeTime;
	sav &mType;
	sav &mOwner;
	sav &mDead;
	sav &mDamageRadius;
	sav &mDirection;
	sav &mSpeed;
	sav &mIdleSound;
	sav &mFlags;
	sav &mSmokeTimer;
	sav &mLocation;
}
