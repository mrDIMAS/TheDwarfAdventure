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

#include "Widget.h"
#include "Button.h"
#include "Label.h"

class NumericUpDown : public Widget
{
private:
	FrameDesc mFrame;
	shared_ptr<Button> mPrev;
	shared_ptr<Button> mNext;
	shared_ptr<Label> mTextField;
	float mMinLimit;
	float mMaxLimit;
	float mValue;
	float mStep;
	int mDecimalPlaces;
	string mAdditionalText;
	void OnResize() override final;
	virtual void DrawSelf() override final;

public:
	virtual void LinkComponents() override final;
	Event OnChange;
	Event OnIncrease;
	Event OnDecrease;
	function<bool()> IncreaseCondition;
	function<bool()> DecreaseCondition;
	NumericUpDown(const shared_ptr<Canvas> &scene);
	~NumericUpDown();
	void Update();
	void SetValue(float value);
	float GetValue() const;
	void SetMinLimit(float value);
	float GetMinLimit() const;
	void SetMaxLimit(float value);
	float GetMaxLimit() const;
	void SetStep(float step);
	float GetStep() const;
	void SetDecimalPlaces(int count);
	int GetDecimalPlaces() const;
	void SetAdditionalText(const string &str);
	string GetAdditionalText() const;
	virtual void SetEnabled(bool state) override final;
};
