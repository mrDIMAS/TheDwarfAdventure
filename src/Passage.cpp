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
#include "Passage.h"

Passage::Passage()
{

}

Passage::Passage(const sf::FloatRect & bounds, const PassageType & type)
	: mBounds(bounds)
	, mType(type)
{

}

Vec2f Passage::GetPosition() const
{
	return { mBounds.left, mBounds.top };
}

Vec2f Passage::GetSize() const
{
	return { mBounds.width, mBounds.height };
}

PassageType Passage::GetType() const
{
	return mType;
}

bool Passage::Intersects(const sf::FloatRect & bounds)
{
	return mBounds.intersects(bounds);
}

void Passage::AddLink(const shared_ptr<Passage>& passage)
{
	mLinkList.push_back(passage);
}

const vector<weak_ptr<Passage>> & Passage::GetLinkList()
{
	return mLinkList;
}

void Passage::AddSpacerBounds(const sf::FloatRect & bounds)
{
	mSpacerBounds.push_back(bounds);
}

const vector<sf::FloatRect>& Passage::GetSpacerBounds() const
{
	return mSpacerBounds;
}

void Passage::AddDecorationBounds(const sf::FloatRect & bounds)
{
	mDecorationBounds.push_back(bounds);
}

const vector<sf::FloatRect>& Passage::GetDecorationBounds() const
{
	return mDecorationBounds;
}

sf::FloatRect Passage::GetBounds() const
{
	return mBounds;
}

void Passage::Serialize(Serializer & sav)
{
	sav & mLinkList;
	sav & mBounds;
	sav & mDecorationBounds;
	sav & mSpacerBounds;
	sav & mType;
}
