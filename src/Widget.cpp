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
#include "Widget.h"
#include "Label.h"
#include "Renderer.h"
#include "Input.h"
#include "Canvas.h"
#include "Game.h"

void Widget::PushQuad(const Vec2f &position, const Vec2f &size, const sf::FloatRect &texRect, const sf::Color &color)
{
	mVertices.emplace_back(Vec2f(position.x, position.y), color, Vec2f(texRect.left, texRect.top));
	mVertices.emplace_back(Vec2f(position.x + size.x, position.y), color, Vec2f(texRect.left + texRect.width, texRect.top));
	mVertices.emplace_back(Vec2f(position.x + size.x, position.y + size.y), color, Vec2f(texRect.left + texRect.width, texRect.top + texRect.height));
	mVertices.emplace_back(Vec2f(position.x, position.y + size.y), color, Vec2f(texRect.left, texRect.top + texRect.height));
}

void Widget::PushScaleableFrame(const FrameDesc &frame)
{
	const Vec2f p = mGlobalPosition + frame.mPosition;
	const Vec2f cs = frame.mCornerSize;
	const Vec2f bdx = { frame.mBorderSize.x, 0 };
	const Vec2f bdy = { 0, frame.mBorderSize.y };
	const Vec2f cdx = { cs.x, 0 };
	const Vec2f cdy = { 0, cs.y };
	const Vec2f bw = { frame.mSize.x - 2 * cs.x, bdy.y };
	const Vec2f bh = { bdx.x, frame.mSize.y - 2 * cs.y };
	const Vec2f w = { frame.mSize.x, 0 };
	const Vec2f h = { 0, frame.mSize.y };
	PushQuad(p, frame.mSize, frame.mBack, frame.mColor);
	PushQuad(p, cs, frame.mLeftTopCorner, frame.mColor);
	PushQuad(p + cdx, bw, frame.mTopBorder, frame.mColor);
	PushQuad(p + w - cdx, cs, frame.mRightTopCorner, frame.mColor);
	PushQuad(p + cdy, bh, frame.mLeftBorder, frame.mColor);
	PushQuad(p + h - cdy, cs, frame.mLeftBottomCorner, frame.mColor);
	PushQuad(p + h + cdx - bdy, bw, frame.mBottomBorder, frame.mColor);
	PushQuad(p + h + w - cs, cs, frame.mRightBottomCorner, frame.mColor);
	PushQuad(p + w + cdy - bdx, bh, frame.mRightBorder, frame.mColor);
}

void Widget::DrawVertices(GLuint textureID, GLenum mode)
{
	const auto renderer = game->GetRenderer();
	if (mVertices.size())
	{
		if (textureID)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
		}
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, renderer->GetDummyTexture()));
		}
		const char *data = reinterpret_cast<const char *>(&mVertices[0]);
		GLCall(glVertexPointer(2, GL_FLOAT, sizeof(sf::Vertex), data + offsetof(sf::Vertex, position)));
		GLCall(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(sf::Vertex), data + offsetof(sf::Vertex, color)));
		GLCall(glTexCoordPointer(2, GL_FLOAT, sizeof(sf::Vertex), data + offsetof(sf::Vertex, texCoords)));
		GLCall(glDrawArrays(mode, 0, mVertices.size()));
	}
}

void Widget::DrawQuad(const Vec2f &position, const Vec2f &size, const sf::FloatRect &texRect, GLuint textureID, const sf::Color &color)
{
	mVertices.clear();
	PushQuad(position, size, texRect, color);
	DrawVertices(textureID);
}

void Widget::OnResize()
{
}

void Widget::RemoveChild(const shared_ptr<Widget> &child)
{
	auto iter = remove(mChildren.begin(), mChildren.end(), child);
	if (iter != mChildren.end())
	{
		mChildren.erase(iter);
	}
}

void Widget::OnGlobalPositionChanged()
{

}

Vec2f Widget::CalculateGlobalPositionBackTrace() const
{
	if (mParent.expired())
	{
		mGlobalPosition = mPosition;
	}
	else
	{
		mGlobalPosition = mParent.lock()->CalculateGlobalPositionBackTrace() + mPosition;
	}

	return mGlobalPosition;
}

Widget::Widget(const shared_ptr<Canvas> &scene) : mCanvas(scene)
{
}

Widget::~Widget()
{
	DBG_ECHO();
}

void Widget::SetPosition(const Vec2f &p)
{
	mNeedUpdateTransform = true;
	mPosition = p;
}

Vec2f Widget::GetPosition() const
{
	return mPosition;
}

Vec2f Widget::GetGlobalPosition() const
{
	if (mNeedUpdateTransform)
	{
		CalculateGlobalPositionBackTrace();
	}
	return mGlobalPosition;
}

void Widget::SetSize(const Vec2f &s)
{
	mSize = s;
	OnResize();
}

Vec2f Widget::GetSize() const
{
	return mSize;
}

void Widget::AddChild(const shared_ptr<Widget> &child)
{
	if (child.get() == this)
	{
		return;
	}

	child->Detach();

	mChildren.push_back(child);
	child->mParent = shared_from_this();
}

void Widget::SetLocalVisibility(bool state)
{
	mLocalVisibility = state;
}

bool Widget::IsGloballyVisible() const
{
	return mGlobalVisibility;
}

void Widget::SetEnabled(bool state)
{
	mEnabled = state;
}

bool Widget::IsEnabled() const
{
	return mEnabled;
}

void Widget::SetTexture(const shared_ptr<Texture> &texture)
{
	mTexture = texture;
}

void Widget::SetTextureRectangle(const sf::FloatRect &rect)
{
	mTextureRect = rect;
}

shared_ptr<Canvas> Widget::GetCanvas() const
{
	return mCanvas.lock();
}

void Widget::Draw(int nesting, int &drawCallCounter)
{
	if (!IsGloballyVisible())
	{
		return;
	}

	// Clear stencil when rendering root widget to prevent content "leakage" when drawing	
	if (nesting == 0)
	{
		GLCall(glClear(GL_STENCIL_BUFFER_BIT));
	}

	if (HasParent())
	{
		// Precise clipping to parent area
		GLCall(glStencilFunc(GL_LEQUAL, nesting, 0xFF));
		GLCall(glStencilOp(GL_KEEP, GL_KEEP, mStencilWriteEnabled ? GL_INCR : GL_KEEP));
	}
	else
	{
		// Do stamp in stencil buffer for root node
		// This will prevent rendering child outside of its parent bounds
		GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
		GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
	}

	DrawSelf();
	++drawCallCounter;

	int childNesting = mStencilWriteEnabled ? nesting + 1 : nesting;

	for (auto &child : mChildren)
	{
		child->Draw(childNesting, drawCallCounter);
	}
}

void Widget::EarlyUpdate()
{
}

void Widget::SetColor(const sf::Color &color)
{
	mColor = color;
}

sf::Color Widget::GetColor() const
{
	return mColor;
}

void Widget::Move(const Vec2f &speed)
{
	SetPosition(mPosition + speed);
}

shared_ptr<Widget> Widget::GetParent()
{
	return mParent.lock();
}

bool Widget::IsHovered() const
{
	return mHovered;
}

void Widget::RemoveAllChildren()
{
	mChildren.clear();
}

void Widget::SetStencilWriteEnabled(bool state)
{
	mStencilWriteEnabled = state;
}

bool Widget::IsStencilWriteEnabled() const
{
	return mStencilWriteEnabled;
}

void Widget::RemoveFromCanvas()
{
	mWantsBeDeleted = true;
	Detach();

	/*
	if (mCanvas.expired())
	{
		return;
	}

	Detach();

	mCanvas.lock()->Remove(shared_from_this());
	*/
}

void Widget::SetTabIndex(int index)
{
	mTabIndex = index;
}

int Widget::GetTabIndex() const
{
	return mTabIndex;
}

void Widget::SetAlpha(uint8_t a)
{
	mColor.a = a;
}

uint8_t Widget::GetAlpha() const
{
	return mColor.a;
}

void Widget::FadeOut(float k)
{
	k = Math::Clamp(k, 0.0f, 1.0f);
	mColor.a = static_cast<uint8_t>(mColor.a * k);
}

bool Widget::IsLocalVisible() const
{
	return mLocalVisibility;
}

void Widget::Detach()
{
	if (HasParent())
	{
		mParent.lock()->RemoveChild(shared_from_this());
		mParent.reset();
	}
}

bool Widget::HasParent() const
{
	return !mParent.expired();
}

bool Widget::Contains(const Vec2f & pt) const
{
	return Math::IsPointInsideRectangle(pt, mGlobalPosition, mSize);
}

bool Widget::Intersects(Widget * other) const
{
	return Math::IsRectRectIntersection(mGlobalPosition, mSize, other->mGlobalPosition, other->mSize);
}



void Widget::UpdateHierarchy(Widget * parent, bool isCanvasVisible, const Vec2f & virtualScreenMousePosition)
{
	//if (!mNeedUpdateTransform)
	//{
	//	return;
	//}

	mGlobalVisibility = mLocalVisibility && isCanvasVisible;
	mGlobalPosition = parent ? mPosition + parent->mGlobalPosition : mPosition;

	if (parent)
	{
		mGlobalVisibility &= parent->mGlobalVisibility && Intersects(parent);
	}

	mHovered = mGlobalVisibility && Contains(virtualScreenMousePosition);

	if (parent)
	{
		mHovered &= parent->mHovered;
	}

	mNeedUpdateTransform = false;

	OnGlobalPositionChanged();

	// Update children
	for (auto & child : mChildren)
	{
		child->UpdateHierarchy(this, isCanvasVisible, virtualScreenMousePosition);
	}
}
