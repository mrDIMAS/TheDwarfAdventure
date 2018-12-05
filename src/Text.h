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

#include "Node.h"
#include "TextAlignment.h"
#include "TextLine.h"

class Text : public SceneNode
{
private:
	vector<TextLine> mLines;
	sf::String mText;
	shared_ptr<sf::Font> mFont;
	sf::Color mColor { 255, 255, 255 };
	TextAlignment mAlignment { TextAlignment::LeftTop };
	int mCharSize { 16 };
	float mOutline { 0 };
	vector<sf::Vertex> mVertices;

public:
	void BreakOnLines();
	Text();
	Text(const shared_ptr<Scene> &scene);
	virtual ~Text();
	void SetText(const string &textUTF8);
	sf::String GetText() const;
	void SetFont(const shared_ptr<sf::Font> &font);
	shared_ptr<sf::Font> GetFont() const;
	void SetColor(const sf::Color &color);
	void SetSize(float w, float h) override;
	void SetSize(const Vec2f &size) override;
	sf::Color GetColor() const;
	void SetCharacterSize(int size);
	int GetCharacterSize() const;
	void SetOutlineWidth(float outline);
	float GetOutlineWidth() const;
	vector<TextLine> &GetLines();
	vector<sf::Vertex> &GetVertices();
	void SetAlignment(const TextAlignment &al);
	TextAlignment GetAlignment() const;
	virtual void Serialize(Serializer &sav) override;
	virtual NodeType GetType() const override;
};
