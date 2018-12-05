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

enum class PassageType
{
	Horizontal,
	Vertical,
};

class Passage
{
public:
	Passage();
	Passage(const sf::FloatRect & bounds, const PassageType & type);
	
	void AddLink(const shared_ptr<Passage> & passage);
	const vector<weak_ptr<Passage>> & GetLinkList();
		
	void AddSpacerBounds(const sf::FloatRect & bounds);
	const vector<sf::FloatRect> & GetSpacerBounds() const;

	void AddDecorationBounds(const sf::FloatRect & bounds);
	const vector<sf::FloatRect> & GetDecorationBounds() const;

	sf::FloatRect GetBounds() const;
	Vec2f GetPosition() const;
	Vec2f GetSize() const;
	PassageType GetType() const;

	bool Intersects(const sf::FloatRect & bounds);

	void Serialize(Serializer & sav);
private:
	PassageType mType { PassageType::Horizontal };
	vector<weak_ptr<Passage>> mLinkList;
	vector<sf::FloatRect> mDecorationBounds;
	vector<sf::FloatRect> mSpacerBounds;
	sf::FloatRect mBounds;
};
