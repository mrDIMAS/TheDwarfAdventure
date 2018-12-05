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
#include "RailGun.h"
#include "Sprite.h"
#include "Actor.h"
#include "Projectile.h"
#include "Level.h"
#include "Terrain.h"
#include "Renderer.h"

void RailGun::OnUpdate()
{
	auto clr = mBeam->GetColor();
	mBeam->SetColor({ clr.r, clr.g, clr.b, static_cast<uint8_t>(clr.a * 0.95f) });
}

void RailGun::OnSerialize(Serializer &sav)
{
	sav &mBeam;
}

RailGun::RailGun()
{
}

RailGun::RailGun(const shared_ptr<Location> &location) : Weapon(location, ItemType::Weapon_RailGun)
{
	const auto scene = location->GetScene();

	mModel = scene->CreateSprite();
	mModel->AttachTo(mNode);
	mModel->SetTexture(game->GetRenderer()->RequestTexture("data/textures/weapons/rail_gun.png"));
	mModel->SetSize(34, 12);
	mModel->SetLocalOrigin(2, 6);

	mBeam = scene->CreateSprite();
	mBeam->SetTexture(game->GetRenderer()->RequestTexture("data/textures/weapons/rail_beam.png"));
	mBeam->SetSize({ 1, 5 });
	mBeam->SetOriginInCenter();

	mShootPoint = { 33, 6 };
}

RailGun::~RailGun()
{
}

bool RailGun::Shoot(const Vec2f &direction)
{
	THROW_IF_EXPIRED(mOwner);
	auto owner = mOwner.lock();
	auto location = owner->GetLocation();
	auto scene = mModel->GetScene();
	auto terrain = scene->GetTerrain();
	bool success = true;

	if (mShootTimeout <= 0)
	{
		if (auto consumable = ExtractConsumable(ItemType::Projectile_Rail))
		{
			WearOut(2.0);
			auto &consumableDesc = consumable->GetDefinition();
			auto &selfDesc = GetDefinition();
			const auto origin = GetShootPointGlobal();
			auto closestResult = terrain->TraceRay(origin, direction * selfDesc.mShootDistance);
			if (closestResult.mIntersection)
			{
				const float radius = 22;
				closestResult.mPosition -= direction * 2.0f * radius;

				MaskOperation args;
				args.mBorderColor = sf::Color(255, 0, 0);
				args.mBorderThickness = 5;
				args.mBounds = Bounds::Circle;
				args.mFirstPassBitwiseOperation = BitwiseOp::And;
				args.mFirstPassMask = ~(PA_VISIBLE | PA_RAYTRACEABLE | PA_COLLIDABLE | PA_LAVA);
				args.mBorderMask = PA_LAVA;
				args.mBorderBitwiseOperation = BitwiseOp::Or;
				auto end = closestResult.mPosition + direction * selfDesc.mShootDistance;
				terrain->DrawLine(closestResult.mPosition, end, radius * 2, args);

				mBeam->SetLocalPosition(origin);
				mBeam->SetLocalRotation(Math::CartesianToPolar(direction).mAngle);
				mBeam->SetColor({ 255, 255, 255, 255 });
				float len = Math::Length(end - origin);
				mBeam->SetSize({ len, 5 });
			}

			vector<RayCircleIntersectionResult> bodies;
			scene->GetSimulationIsland()->RayCircleCast(
				origin, direction * selfDesc.mShootDistance, bodies);
			for (auto &body : bodies)
			{
				for (auto &actor : location->GetActorList())
				{
					if (actor != owner)
					{
						if (actor->GetBody() == body.mBody)
						{
							Damage dmg;
							dmg.mWho = owner;
							dmg.mCount = (1.0f + GetModifier(ItemModifierType::Damage)) * consumableDesc.mDamage;
							dmg.mClass = consumableDesc.mDamageClass;
							dmg.mFlags = (DamageType)consumableDesc.mDamageType;
							actor->ApplyDamage(dmg);
						}
					}
				}
			}

			mShootTimeout = GetDefinition().mShootPeriod;
		}
		else
		{
			success = false;
		}
	}
	return success;
}
