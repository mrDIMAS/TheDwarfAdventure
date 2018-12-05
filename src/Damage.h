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

#define DAMAGE_CLASS_ENUM_ENTRIES(X) \
	X(Unknown, "Unknown") \
	X(Physical, "Physical") \
	X(Chemical, "Chemical") \
	X(Electrical, "Electrical") \
	X(Fire, "Fire") 

ENUM_DECLARE(DamageClass, DAMAGE_CLASS_ENUM_ENTRIES);

#undef DAMAGE_CLASS_ENUM_ENTRIES

enum DamageType : int32_t
{
	DMG_STANDARD = 0,
	// damage freely passed thru shield - mostly environment damage (lava, acid and etc.)
	DMG_UNBLOCKABLE = BIT(0),
};

struct Damage
{
	// Pointer to actor that applied damage
	shared_ptr<Actor> mWho;
	// Amount of damage (in hit points)
	float mCount { 0 };
	// Class (sort) of damage
	DamageClass mClass { DamageClass::Unknown };
	// Bitset of flags
	DamageType mFlags { DMG_STANDARD };
	// Pointer to hitbox that actually applied damage
	class HitBox * mHitbox { nullptr };
};