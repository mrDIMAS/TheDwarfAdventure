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
#include "Label.h"
#include "Canvas.h"
#include "ScrollableArea.h"

void Label::OnResize()
{
	mNeedUpdate = true;
}

void Label::DrawSelf()
{
	if (mFont)
	{
		DrawVertices(mFont->getTexture(mCharSize).getNativeHandle());
	}
}

void Label::CalcHeight()
{
	if (!mAutoHeight)
	{
		return;
	}

	if (!mFont)
	{
		return;
	}

	unordered_map<int, sf::Color> colorMap;
	sf::String purifiedString = GetPurifiedStringAndColorMap(colorMap);
	BreakOnLines(purifiedString);

	// calc total height for auto-height mode
	float totalSize = mLines.size() * mFont->getLineSpacing(mCharSize);
	if (totalSize != mSize.y)
	{
		mSize.y = totalSize;
		// call relayout if label attached to ScrollableArea
		if (auto scrArea = dynamic_pointer_cast<ScrollableArea>(mParent.lock()))
		{
			scrArea->ReLayout();
		}
	}
}

sf::String Label::GetPurifiedStringAndColorMap(unordered_map<int, sf::Color> &colorMap)
{
	// fill color map using special character sequencies inside source string
	// color -  ##(32-bit integer) - example #255
	//
	// probably this is not best and fast solution, but i'm out of time and have
	// to do this in such way
	// !!! WARNING: GRIM CODE AHEAD !!!
	sf::String purifiedString;
	int purePosition = 0;
	for (size_t i = 0; i < mText.getSize(); ++i)
	{
		unsigned int symbol = mText[i];
		// extract color code

		if (symbol == '$' && i + 1 < mText.getSize() && mText[i + 1] == '$')
		{
			i += 2;
			sf::String clrcode;
			while (i < mText.getSize())
			{
				if (mText[i] == '$')
				{
					break;
				}
				clrcode += mText[i];
				++i;
			}
			auto cc = clrcode.toAnsiString();
			unsigned int code;
			sscanf(cc.c_str(), "%X", &code);
			auto color = sf::Color(static_cast<sf::Uint32>(code));
			colorMap[purePosition] = color;
		}
		else
		{
			purifiedString += symbol;
			++purePosition;
		}
	}
	return purifiedString;
}

void Label::BreakOnLines(const sf::String &purifiedString)
{
	mLines.clear();

	// break on lines, align to left corner of rectangle [mWidth; mHeight]
	int lastNotAlpha = -1;
	float stringWidth = 0.0f;
	int prevSubStringLocation = 0;
	float y = 0.0f;

	float stepY = mFont->getLineSpacing(mCharSize);

	for (size_t i = 0; i < purifiedString.getSize(); ++i)
	{
		unsigned int symbol = purifiedString[i];

		auto &glyph = mFont->getGlyph(symbol, mCharSize, false);

		if (symbol > 0 && symbol < 255)
		{
			if (isdigit(symbol) || ispunct(symbol) || isspace(symbol))
			{
				lastNotAlpha = i;
			}
		}

		stringWidth += glyph.advance;
		if (stringWidth > mSize.x || i == (purifiedString.getSize() - 1) || symbol == '\n')
		{
			if (lastNotAlpha < 0 || i == (purifiedString.getSize() - 1))
			{
				const auto newLine =
					purifiedString.substring(prevSubStringLocation, i - prevSubStringLocation + 1);
				mLines.push_back(TextLine(0.0f, y, newLine));
				prevSubStringLocation = i;
			}
			else
			{
				const auto newLine = purifiedString.substring(
					prevSubStringLocation, lastNotAlpha - prevSubStringLocation + 1);
				mLines.push_back(TextLine(0.0f, y, newLine));
				i = lastNotAlpha + 1;
				prevSubStringLocation = i;
			}
			stringWidth = 0;
			lastNotAlpha = -1;
			y += stepY;
		}
	}
}

Label::Label(const shared_ptr<Canvas> &scene)
	: Widget(scene), mCharSize(16), mAlignment(TextAlignment::LeftTop), mAutoHeight(false),
	mNeedUpdate(true), mLastFontTextureWidth(0), mLastFontTextureHeight(0)
{
	// TODO: change theme item
	mFont = scene->GetTheme()->GetFont(Theme::Item::Window);
	mSize = { 100, 20 };
	SetStencilWriteEnabled(false);
}

void Label::SetPosition(const Vec2f &p)
{
	Widget::SetPosition(p);
	mNeedUpdate = true;
}

void Label::LinkComponents()
{
}

void Label::OnGlobalPositionChanged()
{
	if (mPreviousGlobalPosition != mGlobalPosition)
	{
		mNeedUpdate = true;
	}
	mPreviousGlobalPosition = mGlobalPosition;
}

void Label::SetFont(const shared_ptr<sf::Font> &font)
{
	mFont = font;
	mNeedUpdate = true;
}

shared_ptr<sf::Font> Label::GetFont() const
{
	return mFont;
}

void Label::SetText(const string &textUTF8)
{
	mText = sf::String::fromUtf8(textUTF8.begin(), textUTF8.end());
	if (mAutoHeight)
	{
		CalcHeight();
	}
	mNeedUpdate = true;
}

void Label::AppendText(const string &textUTF8)
{
	mText += sf::String::fromUtf8(textUTF8.begin(), textUTF8.end());
	if (mAutoHeight)
	{
		CalcHeight();
	}
	mNeedUpdate = true;
}

sf::String Label::GetText() const
{
	return mText;
}

void Label::SetCharacterSize(int size)
{
	if (size != mCharSize)
	{
		mCharSize = size;
		mNeedUpdate = true;
	}
}

int Label::GetCharacterSize() const
{
	return mCharSize;
}

void Label::SetAlignment(const TextAlignment &al)
{
	if (al != mAlignment)
	{
		mAlignment = al;
		mNeedUpdate = true;
	}
}

TextAlignment Label::GetAlignment() const
{
	return mAlignment;
}

void Label::Update()
{
	const auto &texture = mFont->getTexture(mCharSize);

	// font texture have changed
	if (mLastFontTextureWidth != texture.getSize().x ||
		mLastFontTextureHeight != texture.getSize().y)
	{
		mNeedUpdate = true;
		mLastFontTextureWidth = texture.getSize().x;
		mLastFontTextureHeight = texture.getSize().y;
	}

	if (mNeedUpdate)
	{
		mNeedUpdate = false;

		if (!mFont)
		{
			return;
		}

		mLines.clear();

		float stepY = mFont->getLineSpacing(mCharSize);

		unordered_map<int, sf::Color> colorMap;
		sf::String purifiedString = GetPurifiedStringAndColorMap(colorMap);
		BreakOnLines(purifiedString);

		// calc total height for auto-height mode
		if (mAutoHeight)
		{
			float totalSize = mLines.size() * mFont->getLineSpacing(mCharSize);
			if (totalSize != mSize.y)
			{
				mSize.y = totalSize;
				// call relayout if label attached to ScrollableArea
				auto scrArea = dynamic_pointer_cast<ScrollableArea>(mParent.lock());
				if (scrArea)
				{
					scrArea->ReLayout();
				}
			}
		}

		// calc lines widths
		for (auto &line : mLines)
		{
			line.mWidth = 0.0f;
			for (size_t i = 0; i < line.mSubstring.getSize(); ++i)
			{
				unsigned int symbol = line.mSubstring[i];
				line.mWidth += mFont->getGlyph(symbol, mCharSize, false).advance;
			}
		}

		// do alignment
		for (auto &line : mLines)
		{
			if (mAlignment == TextAlignment::Center)
			{
				line.mX = (mSize.x - line.mWidth) * 0.5f;
				line.mY += (mSize.y - mLines.size() * stepY) * 0.5f;
			}
			else if (mAlignment == TextAlignment::CenterTop)
			{
				line.mX = (mSize.x - line.mWidth) * 0.5f;
			}
			else if (mAlignment == TextAlignment::LeftCenter)
			{
				line.mY += (mSize.y - mLines.size() * stepY) * 0.5f;
			}
			else if (mAlignment == TextAlignment::RightTop)
			{
				line.mX = mSize.x - line.mWidth;
			}
			else if (mAlignment == TextAlignment::RightCenter)
			{
				line.mX = mSize.x - line.mWidth;
				line.mY += (mSize.y - mLines.size() * stepY) * 0.5f;
			}
		}

		// fill vertices
		mVertices.clear();

		const Vec2f invAtlasSize = { 1.0f / texture.getSize().x, 1.0f / texture.getSize().y };
		Vec2f caret;
		sf::Uint32 prevChar = 0;
		int charCounter = 0;
		auto color = mColor;
		for (auto &line : mLines)
		{
			caret.x = line.mX;
			caret.y = line.mY;
			caret += mGlobalPosition;

			for (size_t i = 0; i < line.mSubstring.getSize(); ++i)
			{
				unsigned int symbol = line.mSubstring[i];

				auto colorKeyIter = colorMap.find(charCounter);
				if (colorKeyIter != colorMap.end())
				{
					color = colorKeyIter->second;
				}

				auto &glyph = mFont->getGlyph(symbol, GetCharacterSize(), 0);

				caret.x += mFont->getKerning(prevChar, symbol, GetCharacterSize());

				prevChar = symbol;

				const float dy = stepY * 0.75f;

				// Left Top
				const Vec2f leftTop = { caret.x + glyph.bounds.left, caret.y + dy + glyph.bounds.top };

				const Vec2f leftTopTexCoord = {
					glyph.textureRect.left * invAtlasSize.x, glyph.textureRect.top * invAtlasSize.y };

				mVertices.emplace_back(leftTop, color, leftTopTexCoord);

				// Right Top
				const Vec2f rightTop = {
					caret.x + glyph.bounds.left + glyph.bounds.width, caret.y + dy + glyph.bounds.top };

				const Vec2f rightTopTexCoord = {
					(glyph.textureRect.left + glyph.textureRect.width) * invAtlasSize.x,
					glyph.textureRect.top * invAtlasSize.y };

				mVertices.emplace_back(rightTop, color, rightTopTexCoord);

				// Right Bottom
				const Vec2f rightBottom = { caret.x + glyph.bounds.left + glyph.bounds.width,
					caret.y + dy + glyph.bounds.top + glyph.bounds.height };

				const Vec2f rightBottomTexCoord = {
					(glyph.textureRect.left + glyph.textureRect.width) * invAtlasSize.x,
					(glyph.textureRect.top + glyph.textureRect.height) * invAtlasSize.y };

				mVertices.emplace_back(rightBottom, color, rightBottomTexCoord);

				// Left Bottom
				const Vec2f leftBottom = {
					caret.x + glyph.bounds.left, caret.y + dy + glyph.bounds.top + glyph.bounds.height };

				const Vec2f leftBottomTexCoord = { glyph.textureRect.left * invAtlasSize.x,
					(glyph.textureRect.top + glyph.textureRect.height) * invAtlasSize.y };

				mVertices.emplace_back(leftBottom, color, leftBottomTexCoord);

				caret.x += glyph.advance;

				++charCounter;
			}
		}
	}
}

bool Label::IsAutoHeightEnabled() const
{
	return mAutoHeight;
}

void Label::SetAutoHeight(bool state)
{
	mAutoHeight = state;
	mNeedUpdate = true;
}

string ColorToStr(sf::Color clr)
{
	static char hex[128];
	sprintf(hex, "%X", clr.toInteger());
	return string("$$") + hex + "$";
}
