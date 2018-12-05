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
#include "Scene.h"

class Light : public SceneNode
{
private:
	float mRadius { 200 };
	sf::Color mColor { 255, 141, 66 };
	float mRadiusJitter { 0 }; // > 0 - for torch
	// jitter interpolation
	float mCurrentRadiusJitter { 0 };
	float mRadiusJitterDest { 0 };

public:
	Light();
	Light(const shared_ptr<Scene> &scene);
	virtual ~Light();
	void SetRadius(float r);
	float GetRadius() const;
	sf::Color GetColor() const;
	void SetColor(const sf::Color &color);
	void SetRadiusJitter(float jitter);
	float GetRadiusJitter() const;
	void Update();
	virtual void Serialize(Serializer &sav) override;
	virtual NodeType GetType() const override;
};
