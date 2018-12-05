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

class PolyLine : public SceneNode
{
private:
	vector<Vec2f> mPoints;
	sf::Color mColor { 255, 127, 40 };
	float mThickness { 5 };

public:
	PolyLine();
	PolyLine(const shared_ptr<Scene> &scene);
	~PolyLine();
	void Clear();
	void AddPoint(const Vec2f &pt);
	vector<Vec2f> &GetPoints();
	void SetColor(const sf::Color &color);
	sf::Color GetColor() const;
	void Serialize(Serializer &sav) override;
	void SetThickness(float t);
	float GetThickness() const;
	void MakeCircle(const Vec2f &center, float radius);
	virtual NodeType GetType() const override;
};
