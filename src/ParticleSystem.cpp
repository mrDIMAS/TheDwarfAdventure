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
#include "ParticleSystem.h"
#include "Renderer.h"
#include "Scene.h"

void ParticleSystem::RespawnParticle(Particle &p)
{
	if (mBoundsType == BoundsType::Rect)
	{
		p.mPosition.x = Math::Rnd(mRect.left, mRect.left + mRect.width);
		p.mPosition.y = Math::Rnd(mRect.top, mRect.top + mRect.height);
	}
	else if (mBoundsType == BoundsType::Circle)
	{
		// use polar coordinates to randomly place particle in the circle bounds
		float r = Math::Rnd(0.0f, mRadius);
		float a = Math::Rnd(0.0f, 2.0f * Math::Pi);
		p.mPosition.x = r * cos(a);
		p.mPosition.y = r * sin(a);
	}
	p.mSpeed.x = Math::Rnd(mSpeedMin.x, mSpeedMax.x);
	p.mSpeed.y = Math::Rnd(mSpeedMin.y, mSpeedMax.y);
	p.mLifeTimeMax = Math::Rnd(mLifeTimeMin, mLifeTimeMax);
	p.mLifeTime = p.mLifeTimeMax;
	p.mColor.a = mFadeInParticles ? 0u
		: static_cast<uint8_t>(Math::Rnd(
			static_cast<int>(mAlphaMin), static_cast<int>(mAlphaMax)));
	p.mSize = mParticleSize;
	p.mFadeIn = mFadeInParticles;
}

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::ParticleSystem(const shared_ptr<Scene> &scene) : ParticleSystem()
{
	mName = StringBuilder("UnnamedParticleSystem") << scene->GetParticleSystemList().size();
	mScene = scene;
}

ParticleSystem::~ParticleSystem()
{
	DBG_ECHO();
}

void ParticleSystem::CreateParticles(int count)
{
	if (count < 0)
	{
		count = 0;
	}
	mParticles.clear();
	if (count > 0)
	{
		mParticles.resize(count);
		RespawnParticles();
	}
}

void ParticleSystem::SetTexture(const shared_ptr<Texture> &texture)
{
	mTexture = texture;
}

shared_ptr<Texture> ParticleSystem::GetTexture() const
{
	return mTexture;
}

void ParticleSystem::Update()
{
	static const Vec2f texCoordLeftTop = Vec2f(0, 0);
	static const Vec2f texCoordRightTop = Vec2f(1, 0);
	static const Vec2f texCoordRightBottom = Vec2f(1, 1);
	static const Vec2f texCoordLeftBottom = Vec2f(0, 1);

	mVertices.clear();
	for (auto &p : mParticles)
	{
		p.Update();
		const float fadeOutRate = 0.5f;
		const float fadeInRate = 10.0f;

		if (p.mFadeIn)
		{
			float newAlpha = p.mColor.a + fadeInRate;
			if (newAlpha >= 255.0f)
			{
				p.mColor.a = 255;
				p.mFadeIn = false;
			}
			else
			{
				p.mColor.a = static_cast<uint8_t>(newAlpha);
			}
		}
		else
		{
			bool insideBounds = (mBoundsType == BoundsType::Circle)
				? (Math::SqrLength(p.mPosition) >= mRadius * mRadius)
				: (mRect.contains(p.mPosition));

			// fade out
			if (!insideBounds || p.mLifeTime <= 0)
			{
				p.mColor.a = static_cast<uint8_t>(p.mColor.a * fadeOutRate);
			}
		}

		float t = p.mLifeTime / static_cast<float>(p.mLifeTimeMax);
		Math::LerpRGB(p.mColor, mColorBegin, mColorEnd, t);

		if (p.mColor.a > 0)
		{
			const sf::Vertex leftUpperCorner = { p.mPosition, p.mColor, texCoordLeftTop };
			const sf::Vertex rightUpperCorner = { p.mPosition + Vec2f(p.mSize, 0), p.mColor, texCoordRightTop };
			const sf::Vertex rightBottomCorner = { p.mPosition + Vec2f(p.mSize, p.mSize), p.mColor, texCoordRightBottom };
			const sf::Vertex leftBottomCorner = { p.mPosition + Vec2f(0, p.mSize), p.mColor, texCoordLeftBottom };

			mVertices.emplace_back(leftUpperCorner);
			mVertices.emplace_back(rightUpperCorner);
			mVertices.emplace_back(leftBottomCorner);

			mVertices.emplace_back(rightUpperCorner);
			mVertices.emplace_back(rightBottomCorner);
			mVertices.emplace_back(leftBottomCorner);
		}
		else
		{
			if (mAutoRespawn)
			{
				RespawnParticle(p);
			}
		}
	}
}

vector<sf::Vertex> &ParticleSystem::GetVertexArray()
{
	return mVertices;
}

sf::Vertex *ParticleSystem::GetRawVertices()
{
	return &mVertices[0];
}

void ParticleSystem::SetRadius(float radius)
{
	mRadius = radius;
}

float ParticleSystem::GetRadius() const
{
	return mRadius;
}

void ParticleSystem::SetBoundsType(const BoundsType &bounds)
{
	mBoundsType = bounds;
}

ParticleSystem::BoundsType ParticleSystem::GetBoundsType() const
{
	return mBoundsType;
}

void ParticleSystem::SetMaxSpeed(const Vec2f &speed)
{
	mSpeedMax = speed;
}

Vec2f ParticleSystem::GetMaxSpeed() const
{
	return mSpeedMax;
}

Vec2f ParticleSystem::GetMinSpeed() const
{
	return mSpeedMin;
}

void ParticleSystem::SetMinSpeed(const Vec2f &speed)
{
	mSpeedMin = speed;
}

void ParticleSystem::SetParticleSize(float size)
{
	mParticleSize = size;
}

float ParticleSystem::GetParticleSize() const
{
	return mParticleSize;
}

void ParticleSystem::RespawnParticles()
{
	for (auto &p : mParticles)
	{
		RespawnParticle(p);
	}
}

int ParticleSystem::GetParticleCount() const
{
	return mParticles.size();
}

void ParticleSystem::SetRectangle(const sf::FloatRect &r)
{
	mRect = r;
}

sf::FloatRect ParticleSystem::GetRectangle() const
{
	return mRect;
}

void ParticleSystem::Serialize(Serializer &sav)
{
	SceneNode::Serialize(sav);

	sav &mBoundsType;
	sav &mSpeedMax;
	sav &mSpeedMin;
	sav &mRadius;
	sav &mRect;
	sav &mParticleSize;
	sav &mLifeTimeMin;
	sav &mLifeTimeMax;
	sav &mAlphaMin;
	sav &mAlphaMax;
	sav &mTexture;
	sav &mFadeInParticles;
	sav &mAutoRespawn;
	sav &mColorBegin;
	sav &mColorEnd;

	int particleCount = mParticles.size();
	sav &particleCount;

	if (sav.IsLoading())
	{
		mParticles.resize(particleCount);
	}

	for (auto &p : mParticles)
	{
		p.Serialize(sav);
	}
}

bool ParticleSystem::IsInView(const sf::View &view)
{
	if (mBoundsType == BoundsType::Rect)
	{
		return Math::IsRectRectIntersection(GetGlobalPosition(), { mRect.width, mRect.height },
			view.getCenter() - view.getSize() * 0.5f, view.getSize());
	}
	return Math::IsCircleRectIntersection(
		view.getCenter() - view.getSize() * 0.5f, view.getSize(), GetGlobalPosition(), mRadius);
}

void ParticleSystem::SetAutoRespawnEnabled(bool state)
{
	mAutoRespawn = state;
}

bool ParticleSystem::IsAutoRespawnEnabled() const
{
	return mAutoRespawn;
}

void ParticleSystem::SetParticleFadeInEffectEnabled(bool state)
{
	mFadeInParticles = state;
}

void ParticleSystem::SetParticleLifeTime(int min, int max)
{
	mLifeTimeMin = min;
	mLifeTimeMax = max;
	if (mLifeTimeMin > mLifeTimeMax)
	{
		swap(mLifeTimeMin, mLifeTimeMax);
	}
}

void ParticleSystem::SetBeginColor(const sf::Color &clr)
{
	mColorBegin = clr;
}

void ParticleSystem::SetEndColor(const sf::Color &clr)
{
	mColorEnd = clr;
}

sf::Color ParticleSystem::GetBeginColor() const
{
	return mColorBegin;
}

sf::Color ParticleSystem::GetEndColor() const
{
	return mColorEnd;
}

NodeType ParticleSystem::GetType() const
{
	return NodeType::ParticleSystem;
}

void Particle::Update()
{
	mPosition += mSpeed;

	if (!mFadeIn)
	{
		--mLifeTime;
	}
}

void Particle::Serialize(Serializer &sav)
{
	sav &mPosition;
	sav &mSpeed;
	sav &mColor;
	sav &mLifeTime;
	sav &mSize;
	sav &mFadeIn;
}
