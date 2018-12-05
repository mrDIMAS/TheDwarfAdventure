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

#include "Event.h"

class Texture;
class Canvas;

struct FrameDesc
{
	Vec2f mPosition;
	Vec2f mSize;
	sf::FloatRect mBack;
	sf::FloatRect mLeftTopCorner;
	sf::FloatRect mTopBorder;
	sf::FloatRect mRightTopCorner;
	sf::FloatRect mLeftBorder;
	sf::FloatRect mLeftBottomCorner;
	sf::FloatRect mBottomBorder;
	sf::FloatRect mRightBottomCorner;
	sf::FloatRect mRightBorder;
	Vec2f mCornerSize;
	Vec2f mBorderSize;
	sf::Color mColor;
	FrameDesc() : mColor(255, 255, 255)
	{
	}
};

class Widget : public enable_shared_from_this<Widget>
{
protected:
	friend class Canvas;
	weak_ptr<Canvas> mCanvas;
	Vec2f mPosition;
	mutable Vec2f mGlobalPosition;
	Vec2f mSize;
	sf::Color mColor { 255, 255, 255 };
	shared_ptr<Texture> mTexture;
	sf::FloatRect mTextureRect { 0, 0, 1, 1 };
	weak_ptr<Widget> mParent;
	vector<shared_ptr<Widget>> mChildren;
	vector<sf::Vertex> mVertices;
	int mTabIndex { -1 };
	bool mEnabled { true };
	// Do not takes hierarchy into account
	bool mLocalVisibility { true }; 
	// Takes hierarchy into account
	bool mGlobalVisibility { true }; 
	bool mStencilWriteEnabled { true };
	bool mWantsBeDeleted { false };
	bool mNeedUpdateTransform { true };
	// Used to trigger OnMouseEnter event only once
	// Will be set to "false" when mouse leaves widget bounds
	bool mAlreadyHovered { false };
	// Indicates that widget is hovered by mouse
	// Hierarchy is taken into account: if parent invisible or not hovered, a widget cant be hovered too
	bool mHovered { false };
	void PushQuad(const Vec2f &position, const Vec2f &size, const sf::FloatRect &texRect, const sf::Color &color = sf::Color(255, 255, 255));
	void DrawQuad(const Vec2f &position, const Vec2f &size, const sf::FloatRect &texRect, GLuint textureID, const sf::Color &color = sf::Color(255, 255, 255, 255));
	void PushScaleableFrame(const FrameDesc &frame);
	void DrawVertices(GLuint textureID, GLenum mode = GL_QUADS);
	virtual void OnResize();
	virtual void DrawSelf() = 0;
	void RemoveChild(const shared_ptr<Widget> &child);
	// Called from UpdateHierarchy
	virtual void OnGlobalPositionChanged();
	// Performs back trace. Kinda slow. Used to retrieve global position when scene graph is not updated
	Vec2f CalculateGlobalPositionBackTrace() const;
public:
	// Called when mouse is inside widget bounds (each frame)
	Event OnHover; 
	// Called when mouse enters widget bounds
	Event OnMouseEnter;
	// Use this to attach widgets to each other after creation
	virtual void LinkComponents() = 0;
	// Called before Update
	virtual void EarlyUpdate();
	virtual void Update() = 0;
	Widget(const shared_ptr<Canvas> &scene);
	virtual ~Widget();
	virtual void SetPosition(const Vec2f &p);
	Vec2f GetPosition() const;
	Vec2f GetGlobalPosition() const;
	void SetSize(const Vec2f &s);
	virtual Vec2f GetSize() const;
	virtual void AddChild(const shared_ptr<Widget> &child);
	void SetLocalVisibility(bool state);
	bool IsGloballyVisible() const;
	virtual void SetEnabled(bool state);
	virtual bool IsEnabled() const;
	virtual void SetTexture(const shared_ptr<Texture> &texture);
	void SetTextureRectangle(const sf::FloatRect &rect);
	shared_ptr<Canvas> GetCanvas() const;
	void Draw(int nesting, int &drawCallCounter);
	void SetColor(const sf::Color &color);
	sf::Color GetColor() const;
	void Move(const Vec2f &speed);
	shared_ptr<Widget> GetParent();
	bool IsHovered() const;
	virtual void RemoveAllChildren();
	void SetStencilWriteEnabled(bool state);
	bool IsStencilWriteEnabled() const;
	void RemoveFromCanvas();
	void SetTabIndex(int index);
	int GetTabIndex() const;
	void SetAlpha(uint8_t a);
	uint8_t GetAlpha() const;
	void FadeOut(float k);
	bool IsLocalVisible() const;
	void Detach();
	bool HasParent() const;
	bool Contains(const Vec2f & pt) const;
	bool Intersects(Widget * other) const;	
	
	// Must be called from root node in hierarchy with (0; 0)
	// Updates global position of each node in hierarchy
	// Updates global visibility flag based on hierarchy and its nodes
	// Updates global hovered flag
	void UpdateHierarchy(Widget * parent, bool isCanvasVisible, const Vec2f & virtualScreenMousePosition);
};
