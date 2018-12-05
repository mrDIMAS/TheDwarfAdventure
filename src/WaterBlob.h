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

#include "Collision.h"

class Scene;
class Body;
class Texture;

enum class WaterBlobInitialState
{
	Rect,
	Circle,
};

// fully experimental stuff
// le grand FPS-dropper
class WaterBlob
{
private:
	vector<shared_ptr<Body>> mParticleList;
	queue<shared_ptr<Body>> mResurrectionQueue;
	shared_ptr<Texture> mTexture;
	weak_ptr<Scene> mScene;
	sf::Color mColor { 0, 182, 252, 240 };
	vector<Vec2f> mSourcePoints;
	int mResurrectionPtNum { 0 };
	bool mEnabled { true };
	void ResurrectParticle(const shared_ptr<Body> & p);
	shared_ptr<Sound> mSound;
	Vec2f mPosition;
public:
	static constexpr float ParticleRadius = 2.0f;
	WaterBlob();
	WaterBlob(const shared_ptr<Scene> &scene, const Vec2f &position, WaterBlobInitialState state, float radius,const Vec2f &size);
	void SetTexture(const shared_ptr<Texture> &texture);
	shared_ptr<Texture> GetTexture() const;
	void SetColor(const sf::Color &color);
	sf::Color GetColor() const;
	void Update();
	void EmitQuads(vector<sf::Vertex> &v);
	bool IsDrained() const;
	void Serialize(Serializer &sav);
	// source point used for resurrecting dead particles at new position
	// using this technique we can simulate water drops and water falls
	// note: if no source point was added, resurrection will not work
	void AddSourcePoint(const Vec2f &p);
	void FillCircleWithSourcePoints(const Vec2f & p, float r, float xStep, float yStep);
	void ClearSourcePoints();
	void SetEnabled(bool state);
	bool IsEnabled() const;
};
