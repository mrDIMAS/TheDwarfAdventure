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
#include "Text.h"
#include "Game.h"
#include "Renderer.h"
#include "Scene.h"

Text::Text()
{
	mSize = { 100, 20 };	
}

Text::Text(const shared_ptr<Scene> &scene) : Text()
{
	mName = StringBuilder("UnnamedText") << scene->GetTextList().size();
	mScene = scene;		
}

Text::~Text()
{
	DBG_ECHO();
}

void Text::SetText(const string &textUTF8)
{
	mText = sf::String::fromUtf8(textUTF8.begin(), textUTF8.end());
	BreakOnLines();
}

sf::String Text::GetText() const
{
	return mText;
}

void Text::SetFont(const shared_ptr<sf::Font> &font)
{
	mFont = font;
	BreakOnLines();
}

shared_ptr<sf::Font> Text::GetFont() const
{
	return mFont;
}

void Text::SetColor(const sf::Color &color)
{
	mColor = color;
}

void Text::SetSize(float w, float h)
{
	SceneNode::SetSize(w, h);
	BreakOnLines();
}

void Text::SetSize(const Vec2f &size)
{
	SceneNode::SetSize(size);
	BreakOnLines();
}

sf::Color Text::GetColor() const
{
	return mColor;
}

void Text::SetCharacterSize(int size)
{
	mCharSize = size;
	BreakOnLines();
}

int Text::GetCharacterSize() const
{
	return mCharSize;
}

void Text::SetOutlineWidth(float outline)
{
	mOutline = outline;
}

float Text::GetOutlineWidth() const
{
	return mOutline;
}

void Text::BreakOnLines()
{
	lock_guard<recursive_mutex> guard(game->GetRenderer()->GetMutex());

	if (mFont)
	{
		mLines.clear();

		// break on lines, align to left corner of rectangle [mWidth; mHeight]
		int lastNotAlpha = -1;
		float stringWidth = 0.0f;
		int prevSubStringLocation = 0;
		float y = 0.0f;

		float stepY = mFont->getLineSpacing(mCharSize);
		float gWidth = 0;

		for (size_t i = 0; i < mText.getSize(); ++i)
		{
			unsigned int symbol = mText[i];
			auto &glyph = mFont->getGlyph(symbol, mCharSize, false);

			gWidth = glyph.advance;

			if (symbol > 0 && symbol < 255)
			{
				if (isdigit(symbol) || ispunct(symbol) || isspace(symbol))
				{
					lastNotAlpha = i;
				}
			}

			stringWidth += gWidth;
			if (stringWidth > mSize.x || i == (mText.getSize() - 1) || symbol == '\n')
			{
				if (lastNotAlpha < 0 || i == (mText.getSize() - 1))
				{
					mLines.push_back(TextLine(
						0.0f, y, mText.substring(prevSubStringLocation, i - prevSubStringLocation + 1)));
					prevSubStringLocation = i;
				}
				else
				{
					mLines.push_back(TextLine(0.0f, y,
						mText.substring(prevSubStringLocation, lastNotAlpha - prevSubStringLocation + 1)));
					i = lastNotAlpha + 1;
					prevSubStringLocation = i;
				}
				stringWidth = 0;
				lastNotAlpha = -1;
				y += stepY;
			}
		}

		// do alignment
		if (mAlignment == TextAlignment::Center)
		{
			for (auto &line : mLines)
			{
				float width = 0.0f;
				for (size_t i = 0; i < line.mSubstring.getSize(); ++i)
				{
					unsigned int symbol = line.mSubstring[i];
					width += mFont->getGlyph(symbol, mCharSize, false).advance;
				}
				line.mX = (mSize.x - width) * 0.5f;
				line.mY += (mSize.y - mLines.size() * stepY) * 0.5f;
			}
		}
		else if (mAlignment == TextAlignment::LeftCenter)
		{
			for (auto &line : mLines)
			{
				line.mY += (mSize.y - mLines.size() * stepY) * 0.5f;
			}
		}

		// fill vertices
		mVertices.clear();
		auto &fontTexture = mFont->getTexture(GetCharacterSize());
		Vec2f invAtlasSize = Vec2f(1.0f / fontTexture.getSize().x, 1.0f / fontTexture.getSize().y);
		Vec2f caret;
		sf::Uint32 prevChar = 0;
		for (auto &line : mLines)
		{
			caret.x = line.mX;
			caret.y = line.mY;
			for (auto &symbol : line.mSubstring)
			{
				auto &glyph = mFont->getGlyph(symbol, GetCharacterSize(), 0);

				caret.x += mFont->getKerning(prevChar, symbol, GetCharacterSize());

				prevChar = symbol;

				const float dy = stepY * 0.75f;

				Vec2f v0 = caret + Vec2f(glyph.bounds.left, dy + glyph.bounds.top);
				Vec2f v1 = caret + Vec2f(glyph.bounds.left + glyph.bounds.width, dy + glyph.bounds.top);
				Vec2f v2 = caret + Vec2f(glyph.bounds.left + glyph.bounds.width,
					dy + glyph.bounds.top + glyph.bounds.height);
				Vec2f v3 = caret + Vec2f(glyph.bounds.left, dy + glyph.bounds.top + glyph.bounds.height);

				Vec2f t0 =
					Vec2f(glyph.textureRect.left * invAtlasSize.x, glyph.textureRect.top * invAtlasSize.y);
				Vec2f t1 = Vec2f((glyph.textureRect.left + glyph.textureRect.width) * invAtlasSize.x,
					glyph.textureRect.top * invAtlasSize.y);
				Vec2f t2 = Vec2f((glyph.textureRect.left + glyph.textureRect.width) * invAtlasSize.x,
					(glyph.textureRect.top + glyph.textureRect.height) * invAtlasSize.y);
				Vec2f t3 = Vec2f((glyph.textureRect.left) * invAtlasSize.x,
					(glyph.textureRect.top + glyph.textureRect.height) * invAtlasSize.y);

				mVertices.emplace_back(v0, mColor, t0);
				mVertices.emplace_back(v1, mColor, t1);
				mVertices.emplace_back(v3, mColor, t3);

				mVertices.emplace_back(v1, mColor, t1);
				mVertices.emplace_back(v2, mColor, t2);
				mVertices.emplace_back(v3, mColor, t3);

				caret.x += glyph.advance;
			}
		}
	}
}

vector<TextLine> &Text::GetLines()
{
	return mLines;
}

vector<sf::Vertex> &Text::GetVertices()
{
	return mVertices;
}

void Text::SetAlignment(const TextAlignment &al)
{
	mAlignment = al;
	BreakOnLines();
}

TextAlignment Text::GetAlignment() const
{
	return mAlignment;
}

void Text::Serialize(Serializer &sav)
{
	SceneNode::Serialize(sav);

	string fontName = game->GetRenderer()->GetFontName(mFont);

	sav &fontName;
	sav &mText;
	sav &mColor;
	sav &mAlignment;
	sav &mCharSize;
	sav &mOutline;

	if (sav.IsLoading())
	{
		if (fontName != "<nullptr>")
		{
			mFont = game->GetRenderer()->GetFont(fontName);
		}

		BreakOnLines();
	}
}

NodeType Text::GetType() const
{
	return NodeType::Text;
}
