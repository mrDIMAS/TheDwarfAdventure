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
#include "VisualEffect.h"
#include "Scene.h"
#include "Light.h"
#include "Renderer.h"
#include "ParticleSystem.h"

namespace VisualEffect
{
	shared_ptr<Sprite> MakeExplosion(const shared_ptr<Scene> &scene, const Vec2f &position, const Vec2f &size)
	{
		auto explosion = scene->CreateSprite();
		explosion->SetSize(size);
		explosion->SetOriginInCenter();
		explosion->SetTexture(game->GetRenderer()->RequestTexture("data/textures/effects/explosion.png"));
		explosion->GetAnimation().LoadFromFile("data/animations/explosion.fra");
		explosion->GetAnimation().SetEnabled(true);
		explosion->GetAnimation().UpdateTexCoords();
		explosion->SetLifeTime(static_cast<int>(10.0f * (FIXED_FPS / 40.0f)));
		explosion->SetLocalPosition(position);
		explosion->SetLocalRotation(Math::Rnd(-180.0f, 180.0f));

		auto light = scene->CreateLight();
		light->AttachTo(explosion);
		light->SetRadius(size.x);

		// boom!
		scene->EmitSound("data/sounds/explosion.ogg", explosion);

		return explosion;
	}
	shared_ptr<ParticleSystem> MakeSparks(const shared_ptr<Scene> &scene, const Vec2f &position)
	{
		auto ps = scene->CreateParticleSystem();
		ps->SetBoundsType(ParticleSystem::BoundsType::Circle);
		ps->SetRadius(2);
		ps->SetAutoRespawnEnabled(false);
		ps->SetMinSpeed({ -2, -2 });
		ps->SetMaxSpeed({ 2, 2 });
		ps->SetLocalPosition(position);
		ps->SetParticleSize(2);
		ps->SetParticleFadeInEffectEnabled(false);
		ps->SetParticleLifeTime(4, 6);
		ps->SetBeginColor({ 255, 255, 0 });
		ps->SetEndColor({ 150, 150, 0 });
		ps->CreateParticles(32);
		ps->SetLifeTime(60);
		return ps;
	}
} // namespace VisualEffect
