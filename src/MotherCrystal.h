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

class Sprite;
class Scene;

class MotherCrystal
{
private:
	shared_ptr<Sprite> mNode;
	shared_ptr<Light> mLight;
	Vec2f mPosition;
	Vec2f mOffset;
	bool mFloatUp { true };
	bool mActivated { false };
	static constexpr float MaxFloatOffset = 10.0f;
	weak_ptr<Location> mLocation;
public:
	static constexpr float Width = 32;
	static constexpr float Height = 64;
	MotherCrystal();
	MotherCrystal(const shared_ptr<Location> &location);
	~MotherCrystal();
	void SetPosition(const Vec2f &p);
	Vec2f GetPosition() const;
	Vec2f GetSize() const;
	void Serialize(Serializer &sav);
	void Activate();
	bool IsActive() const;
	shared_ptr<Scene> GetScene() const;
	void Update();
	shared_ptr<Location> GetLocation() const;
};
