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
#include "StatWindow.h"
#include "Level.h"
#include "Player.h"
#include "Stats.h"
#include "Canvas.h"
#include "Window.h"
#include "NumericUpDown.h"
#include "Tooltip.h"

int StatWindow::GetTotalPoints() const
{
	return static_cast<int>(mVitality->GetValue() + mStrength->GetValue() + mDexterity->GetValue() +
		mEndurance->GetValue() + mIntelligence->GetValue() +
		mCharisma->GetValue() + mLuck->GetValue());
}

shared_ptr<Player> StatWindow::GetPlayer() const
{
	auto &level = game->GetLevel();
	if (level)
	{
		return level->GetPlayer();
	}
	return nullptr;
}

StatWindow::StatWindow()
{
	mCanvas = game->CreateCanvas();

	mWindow = mCanvas->Create<Window>();
	mWindow->SetSize({ 600, 520 });
	mWindow->SetCaption(game->GetStr("stats"));
	mWindow->SetInCenter();

	auto &layout = mWindow->GetLayout();

	layout.ConfigureRow(Layout::TemplateRow, 2);

	mLevelLabel = mCanvas->Create<Label>();
	mLevelLabel->SetText(game->GetStr("level"));
	mWindow->AddChild(mLevelLabel);

	mLevel = mCanvas->Create<Label>();
	mWindow->AddChild(mLevel);

	mExpirienceLabel = mCanvas->Create<Label>();
	mExpirienceLabel->SetText(game->GetStr("expirience"));
	mWindow->AddChild(mExpirienceLabel);

	mExpirience = mCanvas->Create<Label>();
	mWindow->AddChild(mExpirience);

	mReputationLabel = mCanvas->Create<Label>();
	mReputationLabel->SetText(game->GetStr("reputation"));
	mReputationLabel->OnHover += [this] { mTooltip->Show(game->GetStr("reputationTooltip")); };
	mWindow->AddChild(mReputationLabel);

	mReputation = mCanvas->Create<Label>();
	mReputation->OnHover += [this] { mTooltip->Show(game->GetStr("reputationTooltip")); };
	mWindow->AddChild(mReputation);

	mHealthLabel = mCanvas->Create<Label>();
	mHealthLabel->SetText(game->GetStr("health"));
	mWindow->AddChild(mHealthLabel);

	mHealth = mCanvas->Create<Label>();
	mWindow->AddChild(mHealth);

	mStaminaLabel = mCanvas->Create<Label>();
	mStaminaLabel->SetText(game->GetStr("stamina"));
	mWindow->AddChild(mStaminaLabel);

	mStamina = mCanvas->Create<Label>();
	mWindow->AddChild(mStamina);

	mFireDamageResistLabel = mCanvas->Create<Label>();
	mFireDamageResistLabel->SetText(game->GetStr("fireDmgResist"));
	mFireDamageResistLabel->SetAutoHeight(true);
	mWindow->AddChild(mFireDamageResistLabel);

	mFireDamageResist = mCanvas->Create<Label>();
	mWindow->AddChild(mFireDamageResist);

	mPhysicalDamageResistLabel = mCanvas->Create<Label>();
	mPhysicalDamageResistLabel->SetText(game->GetStr("physDmgResist"));
	mPhysicalDamageResistLabel->SetAutoHeight(true);
	mWindow->AddChild(mPhysicalDamageResistLabel);

	mPhysicalDamageResist = mCanvas->Create<Label>();
	mWindow->AddChild(mPhysicalDamageResist);

	mChemicalDamageResistLabel = mCanvas->Create<Label>();
	mChemicalDamageResistLabel->SetText(game->GetStr("chemDmgResist"));
	mChemicalDamageResistLabel->SetAutoHeight(true);
	mWindow->AddChild(mChemicalDamageResistLabel);

	mChemicalDamageResist = mCanvas->Create<Label>();
	mWindow->AddChild(mChemicalDamageResist);

	mElectricalDamageResistLabel = mCanvas->Create<Label>();
	mElectricalDamageResistLabel->SetText(game->GetStr("electricalDmgResist"));
	mElectricalDamageResistLabel->SetAutoHeight(true);
	mWindow->AddChild(mElectricalDamageResistLabel);

	mElectricalDamageResist = mCanvas->Create<Label>();
	mWindow->AddChild(mElectricalDamageResist);


	mMeleeWeaponDamageCoefficient = mCanvas->Create<Label>();
	mMeleeWeaponDamageCoefficient->SetText(game->GetStr("meleeWeaponDmgCoeff"));
	mMeleeWeaponDamageCoefficient->SetAutoHeight(true);
	mWindow->AddChild(mMeleeWeaponDamageCoefficient);

	mMeleeWeaponDamageCoefficientLbl = mCanvas->Create<Label>();
	mWindow->AddChild(mMeleeWeaponDamageCoefficientLbl);


	mFreeStatPointsLabel = mCanvas->Create<Label>();
	mFreeStatPointsLabel->SetText(game->GetStr("freeStatPts"));
	mWindow->AddChild(mFreeStatPointsLabel);

	mFreeStatPoints = mCanvas->Create<Label>();
	mWindow->AddChild(mFreeStatPoints);

	layout.ConfigureRow(Layout::TemplateRow, 3);

	const float h = 25;

	auto increaseCondition = [this] { return mPoints > 0; };
	auto onIncrease = [this] { --mPoints; };
	auto onDecrease = [this] { ++mPoints; };

	// Vitality
	{
		mVitalityLabel = mCanvas->Create<Label>();
		mVitalityLabel->SetText(game->GetStr("vitality"));
		mVitalityLabel->SetSize({ 10, h });
		mVitalityLabel->OnHover += [this] { mTooltip->Show(game->GetStr("vitalityDesc")); };

		mWindow->AddChild(mVitalityLabel);

		mCurrentVitality = mCanvas->Create<Label>();
		mCurrentVitality->SetAlignment(TextAlignment::RightTop);
		mCurrentVitality->SetSize({ 10, h });
		mWindow->AddChild(mCurrentVitality);

		mVitality = mCanvas->Create<NumericUpDown>();
		mVitality->IncreaseCondition = increaseCondition;
		mVitality->OnIncrease += onIncrease;
		mVitality->OnDecrease += onDecrease;
		mVitality->SetStep(1);
		mVitality->SetMaxLimit(100);
		mVitality->SetSize({ 100, h });
		mWindow->AddChild(mVitality);
	}

	// Strength
	{
		mStrengthLabel = mCanvas->Create<Label>();
		mStrengthLabel->SetText(game->GetStr("strength"));
		mStrengthLabel->SetSize({ 10, h });
		mStrengthLabel->OnHover += [this] { mTooltip->Show(game->GetStr("strengthDesc")); };
		mWindow->AddChild(mStrengthLabel);

		mCurrentStrength = mCanvas->Create<Label>();
		mCurrentStrength->SetAlignment(TextAlignment::RightTop);
		mCurrentStrength->SetSize({ 10, h });
		mWindow->AddChild(mCurrentStrength);

		mStrength = mCanvas->Create<NumericUpDown>();
		mStrength->IncreaseCondition = increaseCondition;
		mStrength->OnIncrease += onIncrease;
		mStrength->OnDecrease += onDecrease;
		mStrength->SetStep(1);
		mStrength->SetMaxLimit(100);
		mStrength->SetSize({ 100, h });
		mWindow->AddChild(mStrength);
	}

	// Dexterity
	{
		mDexterityLabel = mCanvas->Create<Label>();
		mDexterityLabel->SetText(game->GetStr("dexterity"));
		mDexterityLabel->SetSize({ 10, h });
		mDexterityLabel->OnHover += [this] { mTooltip->Show(game->GetStr("dexterityDesc")); };
		mWindow->AddChild(mDexterityLabel);

		mCurrentDexterity = mCanvas->Create<Label>();
		mCurrentDexterity->SetSize({ 10, h });
		mCurrentDexterity->SetAlignment(TextAlignment::RightTop);
		mWindow->AddChild(mCurrentDexterity);

		mDexterity = mCanvas->Create<NumericUpDown>();
		mDexterity->IncreaseCondition = increaseCondition;
		mDexterity->OnIncrease += onIncrease;
		mDexterity->OnDecrease += onDecrease;
		mDexterity->SetStep(1);
		mDexterity->SetMaxLimit(100);
		mDexterity->SetSize({ 100, h });
		mWindow->AddChild(mDexterity);
	}

	// Endurance
	{
		mEnduranceLabel = mCanvas->Create<Label>();
		mEnduranceLabel->SetText(game->GetStr("endurance"));
		mEnduranceLabel->SetSize({ 10, h });
		mEnduranceLabel->OnHover += [this] { mTooltip->Show(game->GetStr("enduranceDesc")); };
		mWindow->AddChild(mEnduranceLabel);

		mCurrentEndurance = mCanvas->Create<Label>();
		mCurrentEndurance->SetSize({ 10, h });
		mCurrentEndurance->SetAlignment(TextAlignment::RightTop);
		mWindow->AddChild(mCurrentEndurance);

		mEndurance = mCanvas->Create<NumericUpDown>();
		mEndurance->IncreaseCondition = increaseCondition;
		mEndurance->OnIncrease += onIncrease;
		mEndurance->OnDecrease += onDecrease;
		mEndurance->SetStep(1);
		mEndurance->SetMaxLimit(100);
		mEndurance->SetSize({ 100, h });
		mWindow->AddChild(mEndurance);
	}

	// Intelligence
	{
		mIntelligenceLabel = mCanvas->Create<Label>();
		mIntelligenceLabel->SetText(game->GetStr("intelligence"));
		mIntelligenceLabel->SetSize({ 10, h });
		mIntelligenceLabel->OnHover += [this] { mTooltip->Show(game->GetStr("intelligenceDesc")); };
		mWindow->AddChild(mIntelligenceLabel);

		mCurrentIntelligence = mCanvas->Create<Label>();
		mCurrentIntelligence->SetSize({ 10, h });
		mCurrentIntelligence->SetAlignment(TextAlignment::RightTop);
		mWindow->AddChild(mCurrentIntelligence);

		mIntelligence = mCanvas->Create<NumericUpDown>();
		mIntelligence->IncreaseCondition = increaseCondition;
		mIntelligence->OnIncrease += onIncrease;
		mIntelligence->OnDecrease += onDecrease;
		mIntelligence->SetStep(1);
		mIntelligence->SetMaxLimit(100);
		mIntelligence->SetSize({ 100, h });
		mWindow->AddChild(mIntelligence);
	}

	// Charisma
	{
		mCharismaLabel = mCanvas->Create<Label>();
		mCharismaLabel->SetText(game->GetStr("charisma"));
		mCharismaLabel->SetSize({ 10, h });
		mCharismaLabel->OnHover += [this] { mTooltip->Show(game->GetStr("charismaDesc")); };
		mWindow->AddChild(mCharismaLabel);

		mCurrentCharisma = mCanvas->Create<Label>();
		mCurrentCharisma->SetSize({ 10, h });
		mCurrentCharisma->SetAlignment(TextAlignment::RightTop);
		mWindow->AddChild(mCurrentCharisma);

		mCharisma = mCanvas->Create<NumericUpDown>();
		mCharisma->IncreaseCondition = increaseCondition;
		mCharisma->OnIncrease += onIncrease;
		mCharisma->OnDecrease += onDecrease;
		mCharisma->SetStep(1);
		mCharisma->SetMaxLimit(100);
		mCharisma->SetSize({ 100, h });
		mWindow->AddChild(mCharisma);
	}

	// Luck
	{
		mLuckLabel = mCanvas->Create<Label>();
		mLuckLabel->SetText(game->GetStr("luck"));
		mLuckLabel->SetSize({ 10, h });
		mLuckLabel->OnHover += [this] { mTooltip->Show(game->GetStr("luckDesc")); };
		mWindow->AddChild(mLuckLabel);

		mCurrentLuck = mCanvas->Create<Label>();
		mCurrentLuck->SetSize({ 10, h });
		mCurrentLuck->SetAlignment(TextAlignment::RightTop);
		mWindow->AddChild(mCurrentLuck);

		mLuck = mCanvas->Create<NumericUpDown>();
		mLuck->IncreaseCondition = increaseCondition;
		mLuck->OnIncrease += onIncrease;
		mLuck->OnDecrease += onDecrease;
		mLuck->SetStep(1);
		mLuck->SetMaxLimit(100);
		mLuck->SetSize({ 100, h });
		mWindow->AddChild(mLuck);
	}

	layout.ConfigureRow(Layout::TemplateRow, 1);

	mConfirm = mCanvas->Create<Button>();
	mConfirm->SetSize({ 0, 25 });
	mConfirm->SetText(game->GetStr("confirm"));
	mConfirm->OnClick += [this]
	{
		auto player = GetPlayer();
		if (player)
		{
			auto &stats = player->GetStats();
			auto total = GetTotalPoints();
			stats->Vitality += static_cast<int>(mVitality->GetValue());
			stats->Strenght += static_cast<int>(mStrength->GetValue());
			stats->Dexterity += static_cast<int>(mDexterity->GetValue());
			stats->Endurance += static_cast<int>(mEndurance->GetValue());
			stats->Intelligence += static_cast<int>(mIntelligence->GetValue());
			stats->Charisma += static_cast<int>(mCharisma->GetValue());
			stats->Luck += static_cast<int>(mLuck->GetValue());
			stats->ExtractStatPoints(total);
			mVitality->SetValue(0);
			mStrength->SetValue(0);
			mDexterity->SetValue(0);
			mEndurance->SetValue(0);
			mIntelligence->SetValue(0);
			mCharisma->SetValue(0);
			mLuck->SetValue(0);
		}
	};

	mWindow->AddChild(mConfirm);

	mTooltip = mCanvas->Create<Tooltip>();
	mTooltip->SetSize({ 280, 180 });

	mCanvas->SetVisible(false);

	Log() << "Stat window successfully initialized!" << LineEnd;
}

StatWindow::~StatWindow()
{
	DBG_ECHO();
}

void StatWindow::SetVisible(bool state)
{
	if (!game->GetLevel())
	{
		return;
	}
	if (!game->GetLevel()->GetPlayer())
	{
		return;
	}
	if (state)
	{
		auto &stats = GetPlayer()->GetStats();
		mPoints = stats->GetStatPointCount();

		mVitalityPts = stats->Vitality.Get();
		mStrenghtPts = stats->Strenght.Get();
		mDexterityPts = stats->Dexterity.Get();
		mEndurancePts = stats->Endurance.Get();
		mIntelligencePts = stats->Intelligence.Get();
		mCharismaPts = stats->Charisma.Get();
		mLuckPts = stats->Luck.Get();

		mVitality->SetValue(0);
		mStrength->SetValue(0);
		mDexterity->SetValue(0);
		mEndurance->SetValue(0);
		mIntelligence->SetValue(0);
		mCharisma->SetValue(0);
		mLuck->SetValue(0);
	}
	mCanvas->SetVisible(state);
}

bool StatWindow::IsVisible() const
{
	return mCanvas->IsVisible();
}

void StatWindow::Update()
{
	auto player = GetPlayer();
	if (player)
	{
		auto &stats = player->GetStats();
		mLevel->SetText(to_string(stats->GetLevel()));
		mHealth->SetText(StringBuilder() << stats->GetHealth() << "/" << stats->mMaxHealth.Get());
		mStamina->SetText(StringBuilder() << stats->GetStamina() << "/" << stats->mMaxStamina.Get());
		mExpirience->SetText(
			StringBuilder() << stats->GetExpirience() << "/" << stats->GetExpirienceForNextLevel());
		mReputation->SetText(StringBuilder() << (stats->GetReputation() >= 0 ? "+" : "")
			<< RoundAndConvert(stats->GetReputation(), 1));

		mFireDamageResist->SetText(
			StringBuilder() << RoundAndConvert(stats->mFireDamageResist.Get() * 100.0f, 1) << "%"
			<< " + "
			<< RoundAndConvert(
				player->GetEquipmentDamageReduction(DamageClass::Fire) * 100.0f, 1)
			<< "%" << game->GetStr("modEquipment"));

		mPhysicalDamageResist->SetText(
			StringBuilder() << RoundAndConvert(stats->mPhysicalDamageResist.Get() * 100.0f, 1) << "%"
			<< " + "
			<< RoundAndConvert(
				player->GetEquipmentDamageReduction(DamageClass::Physical) * 100.0f,
				1)
			<< "%" << game->GetStr("modEquipment"));

		mChemicalDamageResist->SetText(
			StringBuilder() << RoundAndConvert(stats->mChemicalDamageResist.Get() * 100.0f, 1) << "%"
			<< " + "
			<< RoundAndConvert(
				player->GetEquipmentDamageReduction(DamageClass::Chemical) * 100.0f,
				1)
			<< "%" << game->GetStr("modEquipment"));

		mElectricalDamageResist->SetText(
			StringBuilder()
			<< RoundAndConvert(stats->mElectricalDamageResist.Get() * 100.0f, 1) << "%"
			<< " + "
			<< RoundAndConvert(player->GetEquipmentDamageReduction(DamageClass::Electrical) * 100.0f, 1)
			<< "%" << game->GetStr("modEquipment"));

		mMeleeWeaponDamageCoefficientLbl->SetText(RoundAndConvert(stats->GetMeleeWeaponDamageScaleCoefficient(), 1));
		mFreeStatPoints->SetText(StringBuilder() << mPoints);
		mCurrentVitality->SetText(StringBuilder() << stats->Vitality.Get() << " + ");
		mCurrentStrength->SetText(StringBuilder() << stats->Strenght.Get() << " + ");
		mCurrentDexterity->SetText(StringBuilder() << stats->Dexterity.Get() << " + ");
		mCurrentEndurance->SetText(StringBuilder() << stats->Endurance.Get() << " + ");
		mCurrentIntelligence->SetText(StringBuilder() << stats->Intelligence.Get() << " + ");
		mCurrentCharisma->SetText(StringBuilder() << stats->Charisma.Get() << " + ");
		mCurrentLuck->SetText(StringBuilder() << stats->Luck.Get() << " + ");
	}
}

void StatWindow::SwitchVisible()
{
	SetVisible(!IsVisible());
}
