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
#include "Workbench.h"
#include "Scene.h"
#include "Sprite.h"
#include "Renderer.h"

Workbench::Workbench()
{
}

Workbench::Workbench(const shared_ptr<Scene> &scene, const sf::FloatRect &bounds, const shared_ptr<Texture> &tex)
{
	mSprite = scene->CreateSprite();
	mSprite->SetTexture(tex);
	mSprite->SetTextureRectangle({ 0, 0, 1, 1 });
	mSprite->SetLocalPosition(bounds.left, bounds.top);
	mSprite->SetSize(bounds.width, bounds.height);
}

Workbench::~Workbench()
{
}

sf::FloatRect Workbench::GetBounds() const
{
	auto p = mSprite->GetGlobalPosition();
	auto s = mSprite->GetSize();
	return { p.x, p.y, s.x, s.y };
}

void Workbench::Serialize(Serializer &sav)
{
	sav &mSprite;
}
