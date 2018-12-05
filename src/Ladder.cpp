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
#include "Ladder.h"
#include "Sprite.h"
#include "Scene.h"
#include "Renderer.h"

Ladder::Ladder()
{
}

Ladder::Ladder(const shared_ptr<Scene> &scene)
{
	mNode = scene->CreateSprite();

	auto tex = game->GetRenderer()->RequestTexture("data/textures/objects/ladder.png");
	tex->SetRepeated(true);
	mNode->SetTexture(tex);
}

Ladder::~Ladder()
{
	DBG_ECHO();
}

void Ladder::SetPosition(const Vec2f &p)
{
	mNode->SetLocalPosition(p);
}

void Ladder::SetSize(const Vec2f &s)
{
	mNode->SetSize(s);
	mNode->SetTextureRectangle({ 0, 0, 1, s.y / mNode->GetTexture()->GetHeight() });
}

void Ladder::SetBounds(const sf::FloatRect & bounds)
{
	SetPosition({ bounds.left, bounds.top });
	SetSize({ bounds.width, bounds.height });
}

sf::FloatRect Ladder::GetBounds() const
{
	return { mNode->GetGlobalPosition(), mNode->GetSize() };
}

bool Ladder::Intersects(const Vec2f &p, float radius)
{
	return Math::IsCircleRectIntersection(mNode->GetGlobalPosition(), mNode->GetSize(), p, radius);
}

void Ladder::Serialize(Serializer &sav)
{
	sav &mNode;
}

Vec2f Ladder::GetPosition() const
{
	return mNode->GetGlobalPosition();
}
