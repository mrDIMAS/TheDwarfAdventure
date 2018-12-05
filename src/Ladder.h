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

class Scene;
class Sprite;

class Ladder
{
private:
	shared_ptr<Sprite> mNode;

public:
	Ladder();
	Ladder(const shared_ptr<Scene> &scene);
	~Ladder();
	void SetPosition(const Vec2f &p);
	void SetSize(const Vec2f &s);
	void SetBounds(const sf::FloatRect & bounds);
	sf::FloatRect GetBounds() const;
	bool Intersects(const Vec2f &p, float radius);
	void Serialize(Serializer &sav);
	Vec2f GetPosition() const;
};
