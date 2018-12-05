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
#include "SetSpriteTextureCommand.h"
#include "Sprite.h"

SetSpriteTextureCommand::SetSpriteTextureCommand(const shared_ptr<Sprite> & sprite, const shared_ptr<Texture> & newTexture, const shared_ptr<Texture> & lastTexture)
	: mSprite(sprite), mTexture(newTexture), mLastTexture(lastTexture)
{
	mDesc = "SetTexture";
}

void SetSpriteTextureCommand::Execute()
{
	if (auto sprite = mSprite.lock())
	{
		sprite->SetTexture(mTexture);
	}
}

void SetSpriteTextureCommand::Undo()
{
	if (auto sprite = mSprite.lock())
	{
		sprite->SetTexture(mLastTexture);
	}
}
