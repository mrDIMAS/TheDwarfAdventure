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
#include "WaterBlob.h"
#include "Node.h"
#include "Scene.h"
#include "Texture.h"
#include "Cell.h"
#include "Scene.h"
#include "Sound.h"

void WaterBlob::ResurrectParticle(const shared_ptr<Body> & p)
{
	mResurrectionQueue.push(p);
	p->ResetFlags(BF_ENABLED);
	p->ClearContacts();
}

WaterBlob::WaterBlob()
{
}

WaterBlob::WaterBlob(const shared_ptr<Scene> &scene, const Vec2f &position, WaterBlobInitialState state, float radius, const Vec2f &size)
	: WaterBlob()
{
	mPosition = position;
	mScene = scene;
	auto island = scene->GetSimulationIsland();
	const float psize = ParticleRadius * 2;
	if (state == WaterBlobInitialState::Circle)
	{
		for (float y = position.y; y < position.y + size.y; y += psize)
		{
			for (float x = position.x; x < position.x + size.x; x += psize)
			{
				if (Math::IsCircleCircleIntersection(position, radius, { x, y }, ParticleRadius))
				{
					auto p = island->CreateParticle(ParticleRadius);
					p->SetPosition({ x, y });
					mParticleList.push_back(p);
				}
			}
		}
	}
	else if (state == WaterBlobInitialState::Rect)
	{
		for (float y = position.y; y < position.y + size.y; y += psize)
		{
			for (float x = position.x; x < position.x + size.x; x += psize)
			{
				auto p = island->CreateParticle(ParticleRadius);
				p->SetPosition({ x, y });
				mParticleList.push_back(p);
			}
		}
	}
	for (auto &p : mParticleList)
	{
		p->SetFriction(0.95f);
		p->SetFlags(BF_WATER | BF_IGNORE_PP_CONTACT_INFO);
		p->SetCollisionGroup(CG_WATER);
	}
}

void WaterBlob::SetTexture(const shared_ptr<Texture> &texture)
{
	mTexture = texture;
}

shared_ptr<Texture> WaterBlob::GetTexture() const
{
	return mTexture;
}

void WaterBlob::SetColor(const sf::Color &color)
{
	mColor = color;
}

sf::Color WaterBlob::GetColor() const
{
	return mColor;
}

void WaterBlob::Update()
{
	for (auto iter = mParticleList.begin(); iter != mParticleList.end();)
	{
		auto &p = *iter;

		// If the water particle have touched non-hydrophobic pixel, particle will die if no source points available
		bool isHydrophobicTouch = true;
		const int contactCount = p->GetContactCount();
		for (int i = 0; i < contactCount; ++i)
		{
			auto contact = p->GetContact(i);
			if (contact.mCell)
			{
				if (!(contact.mPixel.a & PA_HYDROPHOBIC) || !(contact.mPixel.a & PA_METAL))
				{
					isHydrophobicTouch = false;
					break;
				}
			}
		}

		if (isHydrophobicTouch)
		{
			++iter;
		}
		else
		{
			if (mSourcePoints.empty())
			{
				p->RemoveFromWorld();
				iter = mParticleList.erase(iter);
			}
			else
			{
				ResurrectParticle(p);
				++iter;
			}
		}
	}

	if (mResurrectionQueue.size())
	{
		for (auto & srcPt : mSourcePoints)
		{
			bool isVacant = true;

			for (auto & particle : mParticleList)
			{
				if (Math::IsCircleCircleIntersection(particle->GetPosition(), particle->GetRadius(), srcPt, particle->GetRadius()))
				{
					isVacant = false;
					break;
				}
			}

			if (isVacant)
			{
				Vec2f randomDir = { Math::Rnd(-1.0f, 1.0f), Math::Rnd(-1.0f, 0.0f) };
				
				auto p = mResurrectionQueue.front();
				p->SetPosition(srcPt);
				p->SetFlags(BF_ENABLED);
				p->AddForce(randomDir * 1000.0f);
				mResurrectionQueue.pop();

				if (mResurrectionQueue.empty())
				{
					break;
				}
			}
		}
	}

	if (!mSound || mSound->IsStopped())
	{
		mSound = mScene.lock()->EmitSound("data/sounds/water_sewer.ogg", mPosition);
	}
}

void WaterBlob::EmitQuads(vector<sf::Vertex> &v)
{
	sf::Vertex leftUpperCorner;
	leftUpperCorner.texCoords = { 0, 0 };
	leftUpperCorner.color = mColor;

	sf::Vertex rightUpperCorner;
	rightUpperCorner.texCoords = { 1, 0 };
	rightUpperCorner.color = mColor;

	sf::Vertex rightBottomCorner;
	rightBottomCorner.texCoords = { 1, 1 };
	rightBottomCorner.color = mColor;

	sf::Vertex leftBottomCorner;
	leftBottomCorner.texCoords = { 0, 1 };
	leftBottomCorner.color = mColor;

	v.clear();
	for (auto &p : mParticleList)
	{
		if (!p->IsFlagsSet(BF_ENABLED))
		{
			continue;
		}

		auto pos = p->GetPosition();
		auto r = p->GetRadius();

		leftUpperCorner.position = pos - Vec2f(r, r);
		rightUpperCorner.position = pos + Vec2f(r, -r);
		rightBottomCorner.position = pos + Vec2f(r, r);
		leftBottomCorner.position = pos + Vec2f(-r, r);

		// Triangle 1
		v.emplace_back(leftUpperCorner);
		v.emplace_back(rightUpperCorner);
		v.emplace_back(leftBottomCorner);

		// Triangle 2
		v.emplace_back(rightUpperCorner);
		v.emplace_back(rightBottomCorner);
		v.emplace_back(leftBottomCorner);
	}
}

bool WaterBlob::IsDrained() const
{
	return mParticleList.empty();
}

void WaterBlob::Serialize(Serializer &sav)
{
	sav &mParticleList;
	sav &mTexture;
	sav &mScene;
	sav &mColor;
	sav &mSourcePoints;
	sav &mEnabled;
	sav &mPosition;

	if (sav.IsLoading())
	{
		// Restore resurrection queue on loading
		for (auto & p : mParticleList)
		{
			if (!p->IsFlagsSet(BF_ENABLED))
			{
				mResurrectionQueue.push(p);
			}
		}
	}
}

void WaterBlob::AddSourcePoint(const Vec2f &p)
{
	mSourcePoints.push_back(p);
}

void WaterBlob::FillCircleWithSourcePoints(const Vec2f & p, float r, float xStep, float yStep)
{
	const float halfR = r * 0.5f;

	for (float x = -halfR; x < halfR; x += xStep)
	{
		for (float y = -halfR; y < halfR; y += yStep)
		{
			const Vec2f pt = p + Vec2f(x, y);

			if (Math::IsPointInsideCircle(pt, p, r * r))
			{
				AddSourcePoint(pt);
			}
		}
	}
}

void WaterBlob::ClearSourcePoints()
{
	mSourcePoints.clear();
}

void WaterBlob::SetEnabled(bool state)
{
	if (state != mEnabled)
	{
		if (state)
		{
			for (auto &p : mParticleList)
			{
				p->SetFlags(BF_ENABLED);
			}
		}
		else
		{
			for (auto &p : mParticleList)
			{
				p->ResetFlags(BF_ENABLED);
			}
		}
		mEnabled = state;
	}
}

bool WaterBlob::IsEnabled() const
{
	return mEnabled;
}
