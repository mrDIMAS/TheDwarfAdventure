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
#include "MaskOperation.h"
#include "Texture.h"

void MaskOperation::SetPolygon(const Math::Polygon & poly)
{
	mPolygon = poly;

	Vec2f min = { FLT_MAX, FLT_MAX };
	Vec2f max = { -FLT_MAX, -FLT_MAX };
	for (auto & p : mPolygon.GetPoints())
	{
		if (p.x > max.x)
		{
			max.x = p.x;
		}
		if (p.y > max.y)
		{
			max.y = p.y;
		}

		if (p.x < min.x)
		{
			min.x = p.x;
		}
		if (p.y < min.y)
		{
			min.y = p.y;
		}
	}

	mPolygonBoundsPosition = min;
	mPolygonBoundsSize = max - min;
	mBounds = Bounds::Polygon;
}

void MaskOperation::SetBrushImage(const shared_ptr<Texture> & tex)
{
	mReplaceImage = tex;
	if (mReplaceImage)
	{
		mImageRect = { 0, 0, mReplaceImage->GetWidth(), mReplaceImage->GetHeight() };
	}
}

void MaskOperation::SetImageRect(const sf::IntRect & r)
{
	if (mReplaceImage)
	{
		mImageRect = r;

		if (mImageRect.left < 0)
		{
			mImageRect.left = 0;
		}
		if (mImageRect.top < 0)
		{
			mImageRect.top = 0;
		}

		if (mImageRect.left >= mReplaceImage->GetWidth())
		{
			mImageRect.left = mReplaceImage->GetWidth() - 1;
		}
		if (mImageRect.top >= mReplaceImage->GetHeight())
		{
			mImageRect.top = mReplaceImage->GetHeight() - 1;
		}

		if (mImageRect.left + mImageRect.width > mReplaceImage->GetWidth())
		{
			mImageRect.width = mReplaceImage->GetWidth() - mImageRect.left;
		}
		if (mImageRect.top + mImageRect.height > mReplaceImage->GetHeight())
		{
			mImageRect.height = mReplaceImage->GetHeight() - mImageRect.top;
		}
	}
}

void MaskOperation::SetBounds(const sf::FloatRect & r)
{
	mBounds = Bounds::Rect;
	mPosition = { r.left, r.top };
	mRectSize = { r.width, r.height };
}

void MaskOperation::SetBounds(const Math::Polygon & poly)
{
	SetPolygon(poly);
}

void MaskOperation::SetBounds(const Vec2f & position, float radius)
{
	mPosition = position;
	mBounds = Bounds::Circle;
	mRadius = radius;
}
