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

#include "Item.h"
#include "Hitbox.h"

class Sprite;

class Shield : public Item
{
private:
	bool mRaised { false };
	// Main hitbox that catches all damage it can
	// Damage suppression is strictly defined by this and attack hitboxes intersection
	HitBox mHitbox;
public:
	Shield();
	Shield(const shared_ptr<Location> &location, ItemType type);
	void Raise(bool state);
	bool IsRaised() const;
	const HitBox & GetHitbox() const;
	virtual void Serialize(Serializer &sav) override final;	 
};
