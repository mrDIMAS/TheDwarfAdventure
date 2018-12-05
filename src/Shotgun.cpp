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
#include "Shotgun.h"
#include "Level.h"
#include "Renderer.h"
#include "Actor.h"
#include "Inventory.h"
#include "LiquidParticle.h"
#include "Terrain.h"
#include "Sound.h"
#include "Debris.h"
#include "Smoke.h"
#include "ResourceDatabase.h"

void Shotgun::OnUpdate()
{
	mShotFlame->SetVisible(mShootTimeout > mShootPeriod - 4);
	for (int i = 0; i < ShotTrailCount; ++i)
	{
		mShotTrails[i]->SetVisible(mShootTimeout > mShootPeriod - 2);
	}
}

void Shotgun::OnSerialize(Serializer &sav)
{
	sav &mShotFlame;
	for (int i = 0; i < ShotTrailCount; ++i)
	{
		sav &mShotTrails[i];
	}
	sav & mShootTimeout;
	mShootPeriod = GetDefinition().mShootPeriod;
}

void Shotgun::OnSetVisible(bool vis)
{
	for (auto &trail : mShotTrails)
	{
		trail->SetVisible(vis);
	}
}

Shotgun::Shotgun()
{
}

Shotgun::Shotgun(const shared_ptr<Location> &location) : Weapon(location, ItemType::Weapon_Shotgun)
{
	const auto scene = location->GetScene();

	const auto wpnTemplate = game->GetResourceDatabase()->GetRandom(WeaponModelTemplateType::Shotgun);

	const auto trailPart = wpnTemplate->mParts[WeaponPartTemplateType::ShotTrail];
	for (int i = 0; i < ShotTrailCount; ++i)
	{
		mShotTrails[i] = scene->CreateSprite();
		mShotTrails[i]->SetTexture(trailPart.mTexture);
	}

	const auto modelPart = wpnTemplate->mParts[WeaponPartTemplateType::Model];
	mModel = scene->CreateSprite();
	mModel->AttachTo(mNode);
	mModel->SetTexture(modelPart.mTexture);
	mModel->SetSize(modelPart.mSize);
	mModel->SetLocalOrigin(modelPart.mOrigin);

	mShootPoint = wpnTemplate->mShootPoint;

	const auto flamePart = wpnTemplate->mParts[WeaponPartTemplateType::ShotFlame];
	mShotFlame = scene->CreateSprite();
	mShotFlame->SetSize(flamePart.mSize);
	mShotFlame->SetLocalOrigin(flamePart.mOrigin);
	mShotFlame->SetTexture(flamePart.mTexture);

	mShootPeriod = GetDefinition().mShootPeriod;
}

Shotgun::~Shotgun()
{
	DBG_ECHO();
}

bool Shotgun::Shoot(const Vec2f &direction)
{
	THROW_IF_EXPIRED(mOwner);
	auto owner = mOwner.lock();
	auto location = owner->GetLocation();
	auto scene = mModel->GetScene();
	auto terrain = scene->GetTerrain();

	bool success = true;
	if (mShootTimeout <= 0)
	{
		if (auto consumable = ExtractConsumable(ItemType::Projectile_Shells))
		{
			WearOut(0.1f);

			mShootTimeout = GetDefinition().mShootPeriod;

			scene->AcceptNode(mShotFlame);

			mShotFlame->SetLocalPosition(GetShootPointGlobal());
			mShotFlame->SetLocalRotation(Math::CartesianToPolar(direction).mAngle);

			scene->EmitSound("data/sounds/shotgun_shot.ogg", mModel);

			auto &desc = GetDefinition();
			for (int i = 0; i < ShotTrailCount; ++i)
			{
				const auto origin = GetShootPointGlobal();
				const auto randomDir = Vec2f(Math::Rnd(-0.25f, 0.25f), Math::Rnd(-0.25f, 0.25f));
				const auto rayDir = Math::Normalized(direction + randomDir) * desc.mShootDistance;

				const auto smokeOffset = Vec2f(Math::Rnd(-7.0f, 7.0f), Math::Rnd(-7.0f, 7.0f));
				location->AddSmoke(make_shared<Smoke>(scene, origin + smokeOffset));

				scene->AcceptNode(mShotTrails[i]);
				mShotTrails[i]->SetLocalPosition(origin);
				mShotTrails[i]->SetLocalRotation(Math::CartesianToPolar(rayDir).mAngle);

				// pick actor by ray
				Vec2f intPos;
				float actorSqrDistance;
				shared_ptr<Actor> closestActor = PickClosestActorByRay(origin, rayDir, &intPos, &actorSqrDistance);

				// and trace ray with terrain
				auto result = terrain->TraceRay(origin, rayDir);

				float terrainSqrDistance = Math::SqrDistance(origin, result.mPosition);

				// if actor closer than terrain
				if (actorSqrDistance < terrainSqrDistance)
				{
					if (actorSqrDistance <= Math::Sqr(desc.mShootDistance))
					{
						location->AddLiquidParticle(make_shared<LiquidParticle>(location, LiquidParticle::Type::Blood, intPos, 2.5f * Math::Normalized(rayDir)));
						auto &consDesc = consumable->GetDefinition();
						Damage dmg;
						dmg.mWho = owner;
						dmg.mCount = (1.0f + GetModifier(ItemModifierType::Damage)) * consDesc.mDamage;
						dmg.mClass = consDesc.mDamageClass;
						dmg.mFlags = (DamageType)consDesc.mDamageType;
						closestActor->ApplyDamage(dmg);
					}

					mShotTrails[i]->SetSize({ sqrt(actorSqrDistance), 1 });
				}
				else
				{
					if (result.mIntersection && result.mCell)
					{
						Vec2i dPix;
						dPix.x = static_cast<int>(Debris::Radius * Math::Sign(rayDir.x));
						dPix.y = static_cast<int>(Debris::Radius * Math::Sign(rayDir.y));

						for (int k = 0; k < 3; ++k)
						{
							Vec2f randomOffset = { Math::Rnd(-3.0f, 3.0f), Math::Rnd(-3.0f, 3.0f) };
							AddDebrisInIntersection(result.mCell, result.mPixelCoord + dPix, result.mPosition + randomOffset);
						}
						terrain->DrillCicle(result.mPosition, 8);

						mShotTrails[i]->SetSize({ sqrt(terrainSqrDistance), 1 });
					}
				}
			}
		}
		else
		{
			success = false;
		}
	}
	return success;
}
