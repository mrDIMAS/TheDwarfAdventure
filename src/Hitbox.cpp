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
#include "Hitbox.h"
#include "Sprite.h"

HitBox::HitBox()
{
}

HitBox::HitBox(const shared_ptr<Sprite> & node) : mRelatedSprite(node)
{
	Sync();
}

void HitBox::Serialize(Serializer & sav)
{
	sav &mRelatedSprite;
	sav &mRect;
}

bool HitBox::IsIntersects(const HitBox & other) const
{
	return mRect.IsIntersectOther(other.mRect);
}

shared_ptr<Sprite> HitBox::GetRelatedSprite() const
{
	return mRelatedSprite.lock();
}

bool HitBox::RelatesTo(const shared_ptr<SceneNode> & sprite)
{
	return GetRelatedSprite() == sprite;
}

bool HitBox::IsIntersects(const Vec2f & p, float r) const
{
	return mRect.IsIntersectCircle(p, r);
}

bool HitBox::IsIntersects(const Math::RotatedRectangle & r) const
{
	return mRect.IsIntersectOther(r);
}

bool HitBox::IsIntersects(const Vec2f & a, const Vec2f & b, Vec2f * intPt) const
{
	return mRect.IsIntersectLine(a, b, intPt);
}

const Math::RotatedRectangle & HitBox::GetRectangle() const
{
	return mRect;
}

void HitBox::SetRectange(const Math::RotatedRectangle & rect)
{
	mRect = rect;
}

void HitBox::Sync()
{
	if (auto node = mRelatedSprite.lock())
	{
		mRect = Math::RotatedRectangle(node->GetSize(), node->GetGlobalTransform());
	}
}

