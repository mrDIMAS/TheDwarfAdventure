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
#include "ConstructionGun.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sprite.h"
#include "Actor.h"
#include "Level.h"
#include "Terrain.h"
#include "Input.h"

void ConstructionGun::OnUpdate()
{
	THROW_IF_EXPIRED(mOwner);
	auto owner = mOwner.lock();
	auto location = owner->GetLocation();
	auto scene = location->GetScene();

	scene->AcceptNode(mBlockBounds);

	mGatheredPixels.clear();
	scene->GetTerrain()->GatherPixelsInsideRect(
		mBlockBounds->GetGlobalPosition(), mBlockBounds->GetSize(), 1, 1, mGatheredPixels, 0);

	mValidPlace = true;
	for (const auto &pix : mGatheredPixels)
	{
		auto attrib = pix.mColor.a;

		if (!(attrib & PA_EDITABLE))
		{
			mValidPlace = false;
			break;
		}

		if (attrib & PA_VISIBLE)
		{
			mValidPlace = false;
			break;
		}
	}

	if (mValidPlace)
	{
		mBlockBounds->SetColor({ 0, 255, 0 });
	}
	else
	{
		mBlockBounds->SetColor({ 255, 0, 0 });
	}

	const auto &desc = GetDefinition();
	const auto origin = GetShootPointGlobal();
	auto destPosToOrigin = scene->GetMousePosition() - origin;
	float k = Math::Min(Math::Length(destPosToOrigin), desc.mShootDistance);
	mBlockBounds->SetLocalPosition(origin + k * Math::Normalized(destPosToOrigin));
}

void ConstructionGun::OnSerialize(Serializer &sav)
{
	sav &mBlockBounds;
}

void ConstructionGun::OnSetVisible(bool vis)
{
	mBlockBounds->SetVisible(vis);
}

ConstructionGun::ConstructionGun()
{
}

ConstructionGun::ConstructionGun(const shared_ptr<Location> &location) : Weapon(location, ItemType::Weapon_ConstructionGun)
{
	const auto renderer = game->GetRenderer();
	const auto scene = location->GetScene();

	mModel = scene->CreateSprite();
	mModel->AttachTo(mNode);
	mModel->SetTexture(renderer->RequestTexture("data/textures/weapons/construction_gun.png"));
	mModel->SetSize(30, 12);
	mModel->SetLocalOrigin(5, 6);

	mBlockBounds = scene->CreateSprite();
	mBlockBounds->SetTexture(renderer->RequestTexture("data/textures/objects/block_bounds.png"));
	mBlockBounds->SetSize({ 27, 27 });
	mBlockBounds->SetRenderFlags(RF_NO_LIGHTING);
	mBlockBounds->SetOriginInCenter();

	mShootPoint = { 30, 4 };

	mShowCrosshair = false;
}

ConstructionGun::~ConstructionGun()
{
}

bool ConstructionGun::Shoot(const Vec2f &direction)
{
	UNUSED(direction);
	THROW_IF_EXPIRED(mOwner);
	auto owner = mOwner.lock();
	auto location = owner->GetLocation();
	auto scene = location->GetScene();
	const auto &desc = GetDefinition();

	bool success = true;
	if (mShootTimeout <= 0)
	{
		if (mValidPlace)
		{
			if (auto consumable = ExtractConsumable(desc.mConsumable))
			{
				MaskOperation args;
				args.mBounds = Bounds::Rect;
				args.mPosition = mBlockBounds->GetGlobalPosition();
				args.mRectSize = mBlockBounds->GetSize();
				args.mFirstPassBitwiseOperation = BitwiseOp::Or;
				args.mFirstPassMask = PA_VISIBLE | PA_COLLIDABLE | PA_EDITABLE | PA_RAYTRACEABLE;
				args.SetBrushImage(
					game->GetRenderer()->RequestTexture("data/textures/terrain_textures/soil_1.png"));
				scene->GetTerrain()->Mask(args);

				scene->EmitSound("data/sounds/soil_block_placement.ogg", mBlockBounds);
			}
			else
			{
				success = false;
			}
		}
	}
	return success;
}
