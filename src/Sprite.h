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
#include "FrameAnimation.h"

enum RenderFlags { RF_NO_LIGHTING = BIT(0) };

class Sprite : public SceneNode
{
private:
	// ************************************************************************
	// WARNING: WHEN ADDING NEW FIELD, DO NOT FORGET TO ADD IT TO 'MakeCopy'  *
	// ************************************************************************
	shared_ptr<Texture> mTexture;
	sf::FloatRect mTextureRect { 0, 0, 1, 1 };
	// decal texture used for effects
	shared_ptr<Texture> mDecalTexture;
	sf::FloatRect mDecalTextureRect { 0, 0, 1, 1 };
	sf::Color mColor { 255, 255, 255 };
	int mRenderFlags { 0 };
	bool mBlowable { false };
	FrameAnimation mAnimation;
	bool mFlipX { false };
	bool mFlipY { false };
	friend class Scene;
	int mRenderOrder { 0 };

public:
	Sprite();
	Sprite(const shared_ptr<Scene> &scene);
	virtual ~Sprite();
	void SetTexture(const shared_ptr<Texture> &texture);
	shared_ptr<Texture> GetTexture();
	virtual Vec2f GetSize() const override final;
	void SetTextureRectangle(const sf::FloatRect &rect);
	sf::FloatRect GetTextureRectangle() const;
	virtual bool IsInView(const sf::View &view) override;
	void SetColor(const sf::Color &color);
	sf::Color GetColor() const;
	int GetRenderFlags() const;
	void SetRenderFlags(int flags);
	void ResetRenderFlags(int flags);
	int GetRenderOrder() const;
	virtual void Serialize(Serializer &sav) override;
	void SetBlowable(bool state);
	bool IsBlowable() const;
	void SetOriginInCenter();
	void SetOriginInCenterY();
	void SetOriginInCenterX();

	FrameAnimation &GetAnimation();
	void UpdateFrameAnimation();
	void SetDecalTexture(const shared_ptr<Texture> &decal);
	void SetDecalTextureRectangle(const sf::FloatRect &r);
	sf::FloatRect GetDecalTextureRectangle() const;
	shared_ptr<Texture> GetDecalTexture() const;
	shared_ptr<Sprite> MakeCopy() const;
	void FlipX(bool state = true);
	bool IsXFlipped() const;
	void FlipY(bool state = true);
	bool IsYFlipped() const;
	// rate < 1 - fade out
	// rate > 1 - fade in
	void Fade(float rate);
	virtual NodeType GetType() const override;
};
