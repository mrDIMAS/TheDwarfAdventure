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
#include "Light.h"
#include "Scene.h"

Light::Light()
{
}

Light::Light(const shared_ptr<Scene> &scene)
{
	mName = StringBuilder("UnnamedLight") << scene->GetLightList().size();
	mScene = scene;
}

Light::~Light()
{
	DBG_ECHO();
}

void Light::SetRadius(float r)
{
	mRadius = r;
}

float Light::GetRadius() const
{
	return mRadius + mCurrentRadiusJitter;
}

sf::Color Light::GetColor() const
{
	return mColor;
}

void Light::SetColor(const sf::Color &color)
{
	mColor = color;
}

void Light::SetRadiusJitter(float jitter)
{
	mRadiusJitter = jitter;
}

float Light::GetRadiusJitter() const
{
	return mRadiusJitter;
}

void Light::Update()
{
	mRadiusJitterDest = Math::Rnd(-mRadiusJitter, mRadiusJitter);

	mCurrentRadiusJitter += (mRadiusJitterDest - mCurrentRadiusJitter) * 0.1f;
}

void Light::Serialize(Serializer &sav)
{
	SceneNode::Serialize(sav);

	sav &mRadius;
	sav &mColor;
	sav &mRadiusJitter;
}

NodeType Light::GetType() const
{
	return NodeType::Light;
}
