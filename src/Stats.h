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

class Serializer;
class Actor;
class Game;

// Class for modifiable character parameter
// T must be numeric value
// char, short, int, float, etc.
template <class T> class Parameter
{
private:
	T mValue { 0 };
	T mModifier { 0 };
	T mMin { std::numeric_limits<T>::min() };
	T mMax { std::numeric_limits<T>::max() };
public:
	function<void()> OnChange;
	explicit Parameter() 
	{
	}
	explicit Parameter(T value) : mValue(value)
	{
	}
	// returns Base
	T Raw() const
	{
		return mValue;
	}
	// returns Base + Modifier
	T Get() const
	{
		return mValue + mModifier;
	}
	void Set(T value)
	{
		mValue = Math::Clamp(value, mMin, mMax);
		if (OnChange)
		{
			OnChange();
		}
	}
	void Modify(T value)
	{
		mModifier += value;
		if (OnChange)
		{
			OnChange();
		}
	}
	void ResetModifier()
	{
		mModifier = 0;
		if (OnChange)
		{
			OnChange();
		}
	}
	void Serialize(Serializer &sav)
	{
		sav &mValue;
		sav &mModifier;
		sav &mMin;
		sav &mMax;
	}
	void operator+=(T value)
	{
		mValue = Math::Clamp(mValue + value, mMin, mMax);
		if (OnChange)
		{
			OnChange();
		}
	}
	void operator-=(T value)
	{
		mValue = Math::Clamp(mValue - value, mMin, mMax);
		if (OnChange)
		{
			OnChange();
		}
	}
	void SetMax(T value)
	{
		mMax = value;
	}
	void SetMin(T value)
	{
		mMin = value;
	}
};

using ParameterF = Parameter<float>;
using ParameterI = Parameter<int>;

template<typename T>
class DependentParameter
{
private:
		T mValue { 0 };
public:
	DependentParameter()
	{

	}

	DependentParameter(T value) : mValue(value)
	{

	}

	void SetValue(float value)
	{
		mValue = value;
	}
	 
	T GetValue() const
	{
		return mValue;
	}
};

using DependentParameterF = DependentParameter<float>;
using DependentParameterI = DependentParameter<int>;

class Stats
{
public:
	////////////////////////////////////////////////////////////////////////////////
	// Basic parameters                                                           //
	////////////////////////////////////////////////////////////////////////////////
	// Vitality defines actor max health points and max damage that he can stand
	ParameterI Vitality { 1 };
	// Strength defines actor's ability to use weapons, armors and etc.
	ParameterI Strenght { 1 };
	// Dexterity defines actor's ability to dodge damage
	// Increases movement speed and jump height
	ParameterI Dexterity { 1 };
	// Endurance defines actor's maximum stamina and physical damage resist
	ParameterI Endurance { 1 };
	// Intelligence defines actor's ability to use hi-tec weapons and etc.
	ParameterI Intelligence { 1 };
	// Luck gives actor a chance to not get damage and etc (full the list)
	ParameterI Luck { 1 };
	// Charisma defines prices balance on barter and reputation penalties
	ParameterI Charisma { 1 };

	//////////////////////////////////////////////////////////////////////////////////
	// Secondary parameters (based on basic parameters and does not set explicitly) //
	//////////////////////////////////////////////////////////////////////////////////
	// Vital parameters
	float mHealth { 0 };
	float mStamina { 0 };
	ParameterF mMaxHealth { 0 };
	ParameterF mMaxStamina { 0 };	
	// Resists
	ParameterF mPhysicalDamageResist { 0.1f }; // [0; 1]
	ParameterF mChemicalDamageResist { 0.05f }; // [0; 1]
	ParameterF mFireDamageResist { 0.05f }; // [0; 1]  
	ParameterF mElectricalDamageResist { 0.01f }; // [0; 1]
	// Movement
	ParameterF mMoveSpeed { 3.5f };

	ParameterF mJumpVerticalSpeed { 6.0f }; 
	// Expirience
	int mExpirienceDrop { 12 };

private:
	void Recalc();
	// Expirience
	int mExpirience { 0 };
	int mExpirienceNextLevel { 20 };
	float mExpirienceGrowthFactor { 1.15f };
	// Other

	bool mDead { false };
	int mLevel { 1 };
	int mFreePts { 0 };

	Actor *mActor { nullptr };

	int mHealTime { 0 };
	float mHealValue { 0 };

	int mStaminaRestorationTime { 0 };
	float mStaminaRestorationValue { 0 };

	int mStaminaRegenInterval { 0 };

	// < 0 - evil
	// > 0 - good
	float mReputation { 3 };

	// Speed of swing of current melee weapon, defined by character dexterity
	float mMeleeWeaponSwingBaseSpeed { 1 };
public:
	Stats(Actor *actor);
	int GetLevel() const;
	// Returns count of free stat point
	int GetStatPointCount() const;
	// Returns 1 if there is a free stat point, 0 - otherwise
	int ExtractStatPoints(int howMuch);
	void Serialize(Serializer &sav);
	// Do not use this directly! Use proxy method Damage inside Actor class
	void Damage(float howMuch);
	// timeFrames = 0 for instant heal
	void Heal(float howMuch, int timeFrames);
	void RestoreStamina(float howMuch, int timeFrames);
	float GetHealth() const;
	bool IsDead() const;
	float GetStamina() const;
	int GetExpirience() const;
	int GetExpirienceForNextLevel() const;
	int GetExpirienceDrop() const;
	void AddExpirience(int value);
	void Resurrect();
	bool ConsumeStamina(float howMuch);
	void Update(bool shieldRaised);
	bool IsHealing() const;
	// Returns probability to be hit in [0; 1] range
	float GetDodgeChance() const;
	void ChangeReputation(float value);
	float GetReputation() const;
	float GetHealValue() const;
	void SetLevel(int level);
	float GetMeleeWeaponSwingBaseSpeed() const;
	float GetMeleeWeaponDamageScaleCoefficient() const;
	float ReduceDamage(float count, const class DamageClass & damageClass) const;
};
