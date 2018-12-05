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

struct Particle
{
	Vec2f mPosition;
	Vec2f mSpeed;
	sf::Color mColor { 255, 255, 255, 0 };
	int mLifeTime { 120 };
	int mLifeTimeMax { mLifeTime };
	float mSize { 10 };
	bool mFadeIn { true };
	void Update();
	void Serialize(Serializer &sav);
};

class ParticleSystem : public SceneNode
{
public:
	enum class BoundsType : int { Circle, Rect };

private:
	BoundsType mBoundsType { BoundsType::Circle };
	vector<Particle> mParticles;
	vector<sf::Vertex> mVertices;
	shared_ptr<Texture> mTexture;
	Vec2f mSpeedMin { -1, -1 };
	Vec2f mSpeedMax { 1, 1 };
	float mRadius { 100 };  // BoundsType::Circle
	sf::FloatRect mRect; // BoundsType::Rect
	float mParticleSize { 3 };
	int mLifeTimeMin { 100 };
	int mLifeTimeMax { 220 };
	uint8_t mAlphaMin { 100 };
	uint8_t mAlphaMax { 255 };
	sf::Color mColorBegin { 255, 255, 255, 255 }; // alpha is not used
	sf::Color mColorEnd { 255, 255, 255, 255 };   // alpha is not used
	bool mAutoRespawn { true };
	bool mFadeInParticles { true };
	void RespawnParticle(Particle &p);

public:
	ParticleSystem();
	ParticleSystem(const shared_ptr<Scene> &scene);
	~ParticleSystem();
	void CreateParticles(int count);
	void SetTexture(const shared_ptr<Texture> &texture);
	shared_ptr<Texture> GetTexture() const;
	void Update();
	vector<sf::Vertex> &GetVertexArray();
	sf::Vertex *GetRawVertices();
	void SetRadius(float radius);
	float GetRadius() const;
	void SetBoundsType(const BoundsType &bounds);
	BoundsType GetBoundsType() const;
	void SetMaxSpeed(const Vec2f &speed);
	Vec2f GetMaxSpeed() const;
	Vec2f GetMinSpeed() const;
	void SetMinSpeed(const Vec2f &speed);
	void SetParticleSize(float size);
	float GetParticleSize() const;
	void RespawnParticles();
	int GetParticleCount() const;
	void SetRectangle(const sf::FloatRect &r);
	sf::FloatRect GetRectangle() const;
	virtual void Serialize(Serializer &sav) override;
	virtual bool IsInView(const sf::View &view) override final;
	void SetAutoRespawnEnabled(bool state);
	bool IsAutoRespawnEnabled() const;
	void SetParticleFadeInEffectEnabled(bool state);
	void SetParticleLifeTime(int min, int max);
	void SetBeginColor(const sf::Color &clr);
	void SetEndColor(const sf::Color &clr);
	sf::Color GetBeginColor() const;
	sf::Color GetEndColor() const;
	virtual NodeType GetType() const override;
};
