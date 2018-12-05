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

#pragma once

class HitBox
{
private:
	weak_ptr<Sprite> mRelatedSprite;
	Math::RotatedRectangle mRect;
public:
	HitBox();
	HitBox(const shared_ptr<Sprite> &node);
	void Serialize(Serializer &sav);	
	shared_ptr<Sprite> GetRelatedSprite() const;
	bool RelatesTo(const shared_ptr<SceneNode> & sprite);
	// Hitbox-Hitbox intersection test
	bool IsIntersects(const HitBox &other) const;
	// Hitbox-Circle intersection test
	bool IsIntersects(const Vec2f &p, float r) const;
	// Hitbox-Rectangle intersection test
	bool IsIntersects(const Math::RotatedRectangle &r) const;
	// Hitbox-Line intersection test (also can give closest (to 'a') intersection point)
	bool IsIntersects(const Vec2f &a, const Vec2f &b, Vec2f * intPt) const;
	const Math::RotatedRectangle & GetRectangle() const;
	void SetRectange(const Math::RotatedRectangle & rect);
	void Sync();
};