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
#include "Sprite.h"
#include "Renderer.h"
#include "Debris.h"
#include "Level.h"

Sprite::Sprite()
{
	mSize = { 10, 10 };
}

Sprite::Sprite(const shared_ptr<Scene> &scene) : Sprite()
{
	mName = StringBuilder("UnnamedSprite") << scene->GetSpriteList().size();
	mScene = scene;
}

void Sprite::SetTexture(const shared_ptr<Texture> &texture)
{
	mTexture = texture;
}

shared_ptr<Texture> Sprite::GetTexture()
{
	return mTexture;
}

Vec2f Sprite::GetSize() const
{
	return mSize;
}

Sprite::~Sprite()
{
	DBG_ECHO();
}

void Sprite::SetTextureRectangle(const sf::FloatRect &rect)
{
	mTextureRect = rect;
}

sf::FloatRect Sprite::GetTextureRectangle() const
{
	return mTextureRect;
}

bool Sprite::IsInView(const sf::View &view)
{
	return Math::IsRectRectIntersection(
		GetGlobalPosition(), GetSize(), view.getCenter() - view.getSize() * 0.5f, view.getSize());
}

void Sprite::SetColor(const sf::Color &color)
{
	mColor = color;
}

sf::Color Sprite::GetColor() const
{
	return mColor;
}

int Sprite::GetRenderFlags() const
{
	return mRenderFlags;
}

void Sprite::SetRenderFlags(int flags)
{
	mRenderFlags |= flags;
}

void Sprite::ResetRenderFlags(int flags)
{
	mRenderFlags &= ~flags;
}

int Sprite::GetRenderOrder() const
{
	return mRenderOrder;
}

void Sprite::Serialize(Serializer &sav)
{
	SceneNode::Serialize(sav);
	sav &mTextureRect;
	sav &mColor;
	sav &mRenderFlags;
	sav &mTexture;
	sav &mBlowable;

	sav &mRenderOrder;

	mAnimation.Serialize(sav);
}

void Sprite::SetBlowable(bool state)
{
	mBlowable = state;
}

bool Sprite::IsBlowable() const
{
	return mBlowable;
}

void Sprite::SetOriginInCenter()
{
	SetLocalOrigin(mSize.x * 0.5f, mSize.y * 0.5f);
}

void Sprite::SetOriginInCenterY()
{
	SetLocalOrigin(mLocalOrigin.x, mSize.y * 0.5f);
}

void Sprite::SetOriginInCenterX()
{
	SetLocalOrigin(mSize.x * 0.5f, mLocalOrigin.y);
}

FrameAnimation &Sprite::GetAnimation()
{
	return mAnimation;
}

void Sprite::UpdateFrameAnimation()
{
	if (mAnimation.IsEnabled())
	{
		mAnimation.Update();
		mTextureRect = mAnimation.GetTextureCoordinates();
	}
}

void Sprite::SetDecalTexture(const shared_ptr<Texture> &decal)
{
	mDecalTexture = decal;
}

void Sprite::SetDecalTextureRectangle(const sf::FloatRect &r)
{
	mDecalTextureRect = r;
}

sf::FloatRect Sprite::GetDecalTextureRectangle() const
{
	return mDecalTextureRect;
}

shared_ptr<Texture> Sprite::GetDecalTexture() const
{
	return mDecalTexture;
}

shared_ptr<Sprite> Sprite::MakeCopy() const
{
	shared_ptr<Sprite> copy = make_shared<Sprite>();
	copy->mLocalOrigin = mLocalOrigin;
	copy->mLocalPosition = mLocalPosition;
	copy->mLocalScale = mLocalScale;
	copy->mLocalTransform = mLocalTransform;
	copy->mGlobalTransform = mGlobalTransform;
	copy->mSize = mSize;
	copy->mLocalRotation = mLocalRotation;
	copy->mScene = mScene;
	copy->mName = mName;
	copy->mVisible = mVisible;
	copy->mTrulyVisible = mTrulyVisible;
	copy->mEditable = mEditable;
	copy->mLifeTime = mLifeTime;
	copy->mAnimation = mAnimation;
	copy->mTexture = mTexture;
	copy->mTextureRect = mTextureRect;
	copy->mDecalTexture = mDecalTexture;
	copy->mDecalTextureRect = mDecalTextureRect;
	copy->mColor = mColor;
	copy->mRenderFlags = mRenderFlags;
	copy->mBlowable = mBlowable;
	GetScene()->AddNode(copy);
	// todo: add body copying
	return copy;
}

void Sprite::FlipX(bool state)
{
	mFlipX = state;
}

bool Sprite::IsXFlipped() const
{
	return mFlipX;
}

void Sprite::FlipY(bool state)
{
	mFlipY = state;
}

bool Sprite::IsYFlipped() const
{
	return mFlipY;
}

// rate < 1 - fade out
// rate > 1 - fade in

void Sprite::Fade(float rate)
{
	mColor.a = static_cast<uint8_t>(mColor.a * rate);
}

NodeType Sprite::GetType() const
{
	return NodeType::Sprite;
}
