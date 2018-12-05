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

class Player;
class Canvas;
class Window;
class Label;
class NumericUpDown;
class Button;
class Tooltip;

class StatWindow
{
private:
	shared_ptr<Canvas> mCanvas;
	shared_ptr<Window> mWindow;

	shared_ptr<Label> mLevel;
	shared_ptr<Label> mLevelLabel;

	shared_ptr<Label> mExpirience;
	shared_ptr<Label> mExpirienceLabel;

	shared_ptr<Label> mReputation;
	shared_ptr<Label> mReputationLabel;

	shared_ptr<Label> mHealth;
	shared_ptr<Label> mHealthLabel;

	shared_ptr<Label> mStamina;
	shared_ptr<Label> mStaminaLabel;

	shared_ptr<Label> mFreeStatPoints;
	shared_ptr<Label> mFreeStatPointsLabel;

	shared_ptr<Label> mFireDamageResist;
	shared_ptr<Label> mFireDamageResistLabel;

	shared_ptr<Label> mPhysicalDamageResist;
	shared_ptr<Label> mPhysicalDamageResistLabel;

	shared_ptr<Label> mChemicalDamageResist;
	shared_ptr<Label> mChemicalDamageResistLabel;

	shared_ptr<Label> mElectricalDamageResist;
	shared_ptr<Label> mElectricalDamageResistLabel;

	shared_ptr<Label> mMeleeWeaponDamageCoefficient;
	shared_ptr<Label> mMeleeWeaponDamageCoefficientLbl;

	shared_ptr<Label> mVitalityLabel;
	shared_ptr<Label> mCurrentVitality;
	shared_ptr<NumericUpDown> mVitality;

	shared_ptr<Label> mStrengthLabel;
	shared_ptr<Label> mCurrentStrength;
	shared_ptr<NumericUpDown> mStrength;

	shared_ptr<Label> mDexterityLabel;
	shared_ptr<Label> mCurrentDexterity;
	shared_ptr<NumericUpDown> mDexterity;

	shared_ptr<Label> mEnduranceLabel;
	shared_ptr<Label> mCurrentEndurance;
	shared_ptr<NumericUpDown> mEndurance;

	shared_ptr<Label> mIntelligenceLabel;
	shared_ptr<Label> mCurrentIntelligence;
	shared_ptr<NumericUpDown> mIntelligence;

	shared_ptr<Label> mCharismaLabel;
	shared_ptr<Label> mCurrentCharisma;
	shared_ptr<NumericUpDown> mCharisma;

	shared_ptr<Label> mLuckLabel;
	shared_ptr<Label> mCurrentLuck;
	shared_ptr<NumericUpDown> mLuck;

	shared_ptr<Button> mConfirm;
	shared_ptr<Tooltip> mTooltip;

	// new stat values
	int mPoints { 0 };
	int mVitalityPts { 0 };
	int mStrenghtPts { 0 };
	int mDexterityPts { 0 };
	int mEndurancePts { 0 };
	int mIntelligencePts { 0 };
	int mCharismaPts { 0 };
	int mLuckPts { 0 };

	int GetTotalPoints() const;
	shared_ptr<Player> GetPlayer() const;

public:
	StatWindow();
	~StatWindow();
	void SetVisible(bool state);
	bool IsVisible() const;
	void Update();
	void SwitchVisible();
};
