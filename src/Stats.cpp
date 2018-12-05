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
#include "Stats.h"
#include "Serializer.h"
#include "Actor.h"
#include "Level.h"

inline float Percent(float percent)
{
	return percent / 100.0f;
}

void Stats::Recalc()
{
	// Calculate max health
	mMaxHealth.Set(Vitality.Get() * 48.0f);
	// Calculate fire dmg resist
	mFireDamageResist.Set(Math::Min(0.5f, Percent(1.0f + Vitality.Get() * 0.297f)));
	// Calculate physical dmg resist
	mPhysicalDamageResist.Set(Math::Min(0.5f, Percent(1.0f + 0.3f * Vitality.Get() + 0.12f * Endurance.Get())));
	// Calculate physical dmg resist
	mChemicalDamageResist.Set(Math::Min(0.5f, Percent(1.0f + 0.32f * Vitality.Get() + 0.12f * Endurance.Get())));
	// Calculate electrical dmg resist
	mElectricalDamageResist.Set(Math::Min(0.5f, Percent(1.0f + 0.34f * Vitality.Get() + 0.11f * Endurance.Get())));
	// Calculate max stamina
	mMaxStamina.Set(Endurance.Get() * 7.0f);
	// Calculate jump height
	mJumpVerticalSpeed.Set(5.5f + 0.2f * Dexterity.Get());
	if (mJumpVerticalSpeed.Get() > 10.0f)
	{
		mJumpVerticalSpeed.Set(10.0f);
	}
	// Calculate melee weapon swing speed
	mMeleeWeaponSwingBaseSpeed = Math::Min(4.25f + Dexterity.Get() * 0.1f, 10.0f);
	// Calculate move speed
	mMoveSpeed.Set(1.5f + 0.12f * Dexterity.Get() + 0.02f * Endurance.Get());
}

Stats::Stats(Actor *actor) : mActor(actor)
{
	Vitality.OnChange = [this]
	{
		Recalc();

		// Heal actor to max health (bonus)
		mHealth = mMaxHealth.Get();
	};

	Endurance.OnChange = [this]
	{
		Recalc();

		// Completely restore stamina (bonus)
		mStamina = mMaxStamina.Get();
	};

	Dexterity.OnChange = [this] 
	{ 
		Recalc(); 
	};

	Strenght.OnChange = [this]
	{
		Recalc();
	};

	Charisma.OnChange = [this]
	{
		Recalc();
	};

	Luck.OnChange = [this]
	{
		Recalc();
	};
	
	// Explicit call for the first time
	Vitality.OnChange();
	Endurance.OnChange();
	Dexterity.OnChange();
	Strenght.OnChange();
	Charisma.OnChange();
	Luck.OnChange();
}

int Stats::GetLevel() const
{
	return mLevel;
}

// returns count of free stat point
int Stats::GetStatPointCount() const
{
	return mFreePts;
}

int Stats::ExtractStatPoints(int howMuch)
{
	if (mFreePts >= howMuch)
	{
		int ret = mFreePts;
		mFreePts -= howMuch;
		return ret;
	}
	return 0;
}

void Stats::Serialize(Serializer &sav)
{
	sav &mExpirience;
	sav &mExpirienceNextLevel;
	sav &mFreePts;
	sav &mHealth;
	sav &mMaxHealth;
	sav &mStamina;
	sav &mMaxStamina;
	sav &mLevel;
	sav &Vitality;
	sav &Strenght;
	sav &Dexterity;
	sav &Endurance;
	sav &Luck;
	sav &Intelligence;
	sav &mPhysicalDamageResist;
	sav &mChemicalDamageResist;
	sav &mFireDamageResist;
	sav &mExpirienceGrowthFactor;
	sav &mExpirienceDrop;
	sav &mMoveSpeed;
	sav &mJumpVerticalSpeed;
	sav &mDead;
	sav &mHealTime;
	sav &mHealValue;
	sav &mStaminaRegenInterval;
	sav &mReputation;
}

void Stats::Damage(float howMuch)
{
	mHealth = Math::Clamp(mHealth - abs(howMuch), 0.0f, mMaxHealth.Get());

	if (mHealth <= 0 && !mDead)
	{
		mDead = true;
		mActor->OnDie();
	}
}

void Stats::Heal(float howMuch, int timeFrames)
{
	if (timeFrames)
	{
		mHealTime = timeFrames;
		mHealValue = howMuch / timeFrames;
	}
	else
	{
		// insta-heal
		mHealth = Math::Clamp(mHealth + abs(howMuch), 0.0f, mMaxHealth.Get());
	}
}

void Stats::RestoreStamina(float howMuch, int timeFrames)
{
	if (timeFrames)
	{
		mStaminaRestorationTime = timeFrames;
		mStaminaRestorationValue = howMuch / timeFrames;
	}
	else
	{
		// insta-rest
		mStamina = Math::Clamp(mStamina + abs(howMuch), 0.0f, mMaxStamina.Get());
	}
}

float Stats::GetHealth() const
{
	return mHealth;
}

bool Stats::IsDead() const
{
	return mHealth <= 0.0f;
}

float Stats::GetStamina() const
{
	return mStamina;
}

int Stats::GetExpirience() const
{
	return mExpirience;
}

int Stats::GetExpirienceForNextLevel() const
{
	return mExpirienceNextLevel;
}

int Stats::GetExpirienceDrop() const
{
	return mExpirienceDrop;
}

void Stats::AddExpirience(int value)
{
	mExpirience += abs(value);
	if (mExpirience >= mExpirienceNextLevel)
	{
		mActor->GetLocation()->ReportMessage(
			game->GetStr("newLevel"), mActor->GetPosition() + Vec2f(-30, -30), sf::Color(0, 255, 0));
	}
	else
	{
		mActor->GetLocation()->ReportMessage(StringBuilder("+") << value << " exp",
			mActor->GetPosition() + Vec2f(-30, -30), sf::Color(0, 255, 0));
	}
	// level up
	while (mExpirience >= mExpirienceNextLevel)
	{
		mExpirience -= mExpirienceNextLevel;
		mExpirienceNextLevel = static_cast<int>(mExpirienceNextLevel * mExpirienceGrowthFactor);
		++mFreePts;
		++mLevel;
	}
}

void Stats::Resurrect()
{
	mHealth = mMaxHealth.Get();
	mDead = false;
}

bool Stats::ConsumeStamina(float howMuch)
{
	howMuch = abs(howMuch);
	if (mStamina - howMuch >= 0)
	{
		mStamina -= howMuch;
		mStaminaRegenInterval = 60;
		return true;
	}
	return false;
}

void Stats::Update(bool shieldRaised)
{
	// Health
	if (mHealTime > 0)
	{
		mHealth += mHealValue;
		--mHealTime;
	}

	// Stamina restoration
	if (mStaminaRestorationTime > 0)
	{
		mStamina += mStaminaRestorationValue;
		--mStaminaRestorationTime;
	}

	// Stamina
	if (mStaminaRegenInterval <= 0)
	{
		if (shieldRaised)
		{
			mStamina += 0.05f;
		}
		else
		{
			mStamina += 0.2f;
		}
	}
	if (mStamina >= mMaxStamina.Get())
	{
		mStamina = mMaxStamina.Get();
	}
	--mStaminaRegenInterval;
}

bool Stats::IsHealing() const
{
	return mHealTime > 0;
}

float Stats::GetDodgeChance() const
{
	float chance = Dexterity.Get() * 0.005f + Luck.Get() * 0.0025f;
	return Math::Clamp(chance, 0.0f, 0.6f);
}

void Stats::ChangeReputation(float value)
{
	mReputation += value;
}

float Stats::GetReputation() const
{
	return mReputation;
}

float Stats::GetHealValue() const
{
	return mHealValue * mHealTime;
}

void Stats::SetLevel(int level)
{
	mLevel = level;
}

float Stats::GetMeleeWeaponSwingBaseSpeed() const
{
	return mMeleeWeaponSwingBaseSpeed;
}

float Stats::GetMeleeWeaponDamageScaleCoefficient() const
{
	return 1.0f + Strenght.Get() * 0.08f + Dexterity.Get() * 0.04f;
}

float Stats::ReduceDamage(float count, const DamageClass & damageClass) const
{
	switch (damageClass)
	{
	case DamageClass::Physical:
		count *= 1.0f - mPhysicalDamageResist.Get();
		break;
	case DamageClass::Fire:
		count *= 1.0f - mFireDamageResist.Get();
		break;
	case DamageClass::Chemical:
		count *= 1.0f - mChemicalDamageResist.Get();
		break;
	case DamageClass::Electrical:
		count *= 1.0f - mElectricalDamageResist.Get();
		break;
    case DamageClass::Unknown:
        break;
	}

	return count;
}
