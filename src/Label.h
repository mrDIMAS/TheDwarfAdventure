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
#include "TextAlignment.h"
#include "TextLine.h"

class Label : public Widget
{
protected:
	vector<TextLine> mLines;
	int mCharSize;
	bool mAutoHeight;
	Vec2f mPreviousGlobalPosition;
	sf::String mText;
	shared_ptr<sf::Font> mFont;
	TextAlignment mAlignment;
	mutable bool mNeedUpdate;
	unsigned int mLastFontTextureWidth;
	unsigned int mLastFontTextureHeight;
	virtual void OnResize() override final;
	virtual void DrawSelf() override final;
	void CalcHeight();
	sf::String GetPurifiedStringAndColorMap(unordered_map<int, sf::Color> &colorMap);
	void BreakOnLines(const sf::String &purifiedString);

public:
	Label(const shared_ptr<Canvas> &scene);
	virtual void SetPosition(const Vec2f &p) override final;
	virtual void LinkComponents() override final;
	virtual void OnGlobalPositionChanged() override final;
	void SetFont(const shared_ptr<sf::Font> &font);
	shared_ptr<sf::Font> GetFont() const;
	void SetText(const string &textUTF8);
	void AppendText(const string &textUTF8);
	sf::String GetText() const;
	void SetCharacterSize(int size);
	int GetCharacterSize() const;
	void SetAlignment(const TextAlignment &al);
	TextAlignment GetAlignment() const;
	void Update() override final;
	bool IsAutoHeightEnabled() const;
	void SetAutoHeight(bool state);
};
