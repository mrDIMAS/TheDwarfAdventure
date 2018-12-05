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

#include "ItemDatabase.h"

class SceneNode;
class Scene;
class Texture;

enum class ItemModifierType
{
	Unknown,
	Damage,
	PhysicalDamageReduction,
	ChemicalDamageReduction,
	FireDamageReduction,
	Durability,
	Stability,
	ShootPeriod,
	ShootDistance,
};

class Item : public enable_shared_from_this<Item>
{
protected:
	weak_ptr<Location> mLocation;
    ItemType mType;
	float mDurability;
	shared_ptr<SceneNode> mNode;

	// All modifications given in percents of base value defined in ItemDatabase
	map<ItemModifierType, float> mModifierList;

public:
	static constexpr float MaxDamageMod = 2.0f;
	static constexpr float MaxPhysicalDamageReductionMod = 2.0f;
	static constexpr float MaxChemicalDamageReductionMod = 2.0f;
	static constexpr float MaxFireDamageReductionMod = 2.0f;
	static constexpr float MaxDurabilityMod = 2.0f;
	static constexpr float MaxStabilityMod = 2.0f;
	static constexpr float MaxShootPeriodMod = 2.0f;
	static constexpr float MaxShootDistanceMod = 3.0f;
	Item();
	Item(const shared_ptr<Location> &location, const ItemType &type);
	virtual ~Item();
	ItemType GetType() const;
	shared_ptr<SceneNode> GetNode() const;
	virtual void Serialize(Serializer &sav);
	void Update();
	void SetPosition(const Vec2f &pos);
	virtual shared_ptr<Texture> GetPictogram();
	virtual sf::FloatRect GetPictogramTexCoords() const;
	const ItemDefinition &GetDefinition() const;
	void WearOut(float howMuch);
	bool IsBroken() const;
	bool IsBrandNew() const;
	float GetCurrentDurability() const;
	// Out value in percent (degree of wear)
	float GetDamageLevel() const;
	void Repair();
	const map<ItemModifierType, float> &GetModifierList() const;
	float GetModifiedValue(ItemModifierType type) const;
	float GetModifier(ItemModifierType type) const;
	void ModifyValue(ItemModifierType type, float value);
	void EnlistModifications(StringBuilder &fmt);
	bool IsModified() const;
	void SetLocation(const shared_ptr<Location> & location);
	shared_ptr<Location> GetLocation() const;
	// called when putting item into inventory
	void OnPut();
	// called when item extracted from inventory
	void OnPullOut();
};
