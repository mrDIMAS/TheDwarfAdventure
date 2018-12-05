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

//////////////////////////////////////////////////////////////////
// BitwiseOp enumeration
#define ENUM_ENTRIES(X) \
	X(Point, "Point") \
	X(Circle, "Circle") \
	X(Rect, "Rect") \
	X(Polygon, "Polygon")

ENUM_DECLARE(Bounds, ENUM_ENTRIES);

#undef ENUM_ENTRIES

//////////////////////////////////////////////////////////////////
// BitwiseOp enumeration
#define ENUM_ENTRIES(X) \
	X(And, "And") \
	X(Or, "Or") \
	X(Xor, "Xor")

ENUM_DECLARE(BitwiseOp, ENUM_ENTRIES);

#undef ENUM_ENTRIES

//////////////////////////////////////////////////////////////////
// BlendOperation enumeration
#define ENUM_ENTRIES(X) \
	X(Replace, "Replace")

ENUM_DECLARE(BlendOperation, ENUM_ENTRIES);

#undef ENUM_ENTRIES

//////////////////////////////////////////////////////////////////
// BrushMode enumeration
#define ENUM_ENTRIES(X) \
	X(Wrap, "Wrap") /* Wrap pixel coordinates into image bounds */ \
	X(WrapBounds, "WrapBounds") /* Offset pixel coords by bounds left corner and wrap into bounds */ \
	X(Insert, "Insert") /* Insert image into terrain */

ENUM_DECLARE(BrushMode, ENUM_ENTRIES);

#undef ENUM_ENTRIES

class MaskOperation
{
public:
	// left upper corner of bounds for rect
	// center - for circle
	Vec2f mPosition { 0, 0 };
	// width and height of rect bounds
	Vec2f mRectSize { 0, 0 };
	// border will be filled with this color
	sf::Color mBorderColor { 255, 255, 255 };
	// radius of circle if mBounds is 'MaskOperationBounds::Circle'
	float mRadius { 0 };
	float mBorderThickness { 0 };
	// any combination of PixelAttribute's
	int mFirstPassMask { 0 };
	int mSecondPassMask { 0 };
	int mBorderMask { 0 };
	// type of figure for masking
	Bounds mBounds { Bounds::Circle };
	BitwiseOp mFirstPassBitwiseOperation { BitwiseOp::And };
	BitwiseOp mSecondPassBitwiseOperation { BitwiseOp::Or };
	BitwiseOp mBorderBitwiseOperation { BitwiseOp::Or };
	BlendOperation mBlendOp { BlendOperation::Replace };
	BrushMode mBrushMode { BrushMode::Wrap };
	// true if operation performed from editor, level generator or smth.
	// if true - ignores Editable attribute of pixel
	bool mEditorAccess { false };
	bool mDoBitmaskOnlyOnVisiblePixelsOfReplaceImage { false };
	bool mDoBitmaskOnlyOnVisibleDestPixels { false };
	// true - for background editing
	bool mEditBackground { false };
	// background will be filled with this value if editing is enabled
	// 0 - means transparent background
	uint8_t mBackgroundValue { 0 };
	// if true - mask op can be done on pixels that have PA_METAL
	bool mMeltMetalPixels { false };
	// list of forbidden areas - no pixels will be modified inside these
	vector<sf::FloatRect> mForbiddenRects;
	vector<Math::CircleDesc> mForbiddenCircles;
	// mask value and op on intersection pixels (any forbidden bounds and mBounds)
	int mForbiddenBoundsIntersectionMask { 0 };
	BitwiseOp mForbiddenBoundsIntersectionBitwiseOperation { BitwiseOp::Or };
	bool mForbiddenBoundsEditBackground { false };
	int mForbiddenBoundsIntersectionBackgroundValue { 0 };
	bool mDoNotAddLavaPixelsToChillList { false };
	bool mDrawOnFogOfWar { false };
	sf::Color mFogOfWarBrush { 255, 255, 255, 255 };

	// will be called if pixel inside of bounds is modified
	function<void(const Vec2f &)> mPixelModifyCallback;
	function<void(const Vec2f &)> mPixelIgnoreCallback;
	void SetPolygon(const Math::Polygon & poly);
	void SetBrushImage(const shared_ptr<Texture> &tex);
	void SetImageRect(const sf::IntRect &r);
	void SetBounds(const sf::FloatRect & r);
	void SetBounds(const Math::Polygon &poly);
	void SetBounds(const Vec2f & position, float radius);
private:
	// pointer to image, using which mBounds will be filled
	// can be nullptr, in such way no pixels will be replaced
	shared_ptr<Texture> mReplaceImage { nullptr };
	sf::IntRect mImageRect { 0, 0, 0, 0 };
	// for fast access in Cell::Mask
	friend class Cell;
	// used together with mBounds == Bounds::Polygon
	Math::Polygon mPolygon;
	Vec2f mPolygonBoundsPosition;
	Vec2f mPolygonBoundsSize;
};