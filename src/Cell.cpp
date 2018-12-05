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
#include "Cell.h"
#include "Terrain.h"
#include "Game.h"
#include "Texture.h"

sf::Color Cell::ColdColor = sf::Color(60, 30, 30);
sf::Color Cell::HotColor = sf::Color(255, 40, 40);

void Cell::Compress()
{
	if (!mCompressed)
	{
		// Try to compress
		const size_t totalBytesPerCell = PixelCount * sizeof(*mPixels);
		mz_ulong destSize = mz_compressBound(totalBytesPerCell);
		unsigned char* dest = reinterpret_cast<unsigned char*>(malloc(destSize));
		const unsigned char* src = reinterpret_cast<unsigned char*>(mPixels);
		if (mz_compress(dest, &destSize, src, totalBytesPerCell) != MZ_OK)
		{
			free(dest);
			throw Exception("Fatal: Unable to compress cell data.");
		}
		// Substitute old pixels with compressed data
		delete[] mPixels;
		mCompressedSize = destSize;
		mPixels = reinterpret_cast<CellPixel*>(dest);
		mCompressed = true;
	}
}

void Cell::Decompress()
{
	if (mCompressed)
	{
		// Try to decompress
		mz_ulong decompressedSize = PixelCount * sizeof(*mPixels);
		unsigned char* dest = reinterpret_cast<unsigned char*>(malloc(decompressedSize));
		const unsigned char* src = reinterpret_cast<unsigned char*>(mPixels);
		if (mz_uncompress(dest, &decompressedSize, src, mCompressedSize) != MZ_OK)
		{
			free(dest);
			throw Exception("Fatal: Unable to decompress cell data.");
		}
		// Substitute compressed pixels with decompressed data
		free(mPixels); // free here instead of delete[] because of malloc on Compress
		mPixels = reinterpret_cast<CellPixel*>(dest);
		// Recalculate grid pointers because we allocated new chunk of data
		CalculateImageGridPointers();
		mCompressedSize = 0;
		mCompressed = false;
	}
}

bool Cell::IsCompressed() const
{
	return mCompressed;
}

shared_ptr<Cell> Cell::MakeCompressedCopy()
{
	const shared_ptr<Cell> compressed = MakeCopy();
	compressed->Compress();
	return compressed;
}

// Rewritten SFML sf::Image::copy
void Cell::CopyPixels(sf::Color *dest, uint32_t destWidth, uint32_t destHeight,
	const sf::Color *source, int sourceWidth, int sourceHeight, uint32_t destX,
	uint32_t destY, const sf::IntRect &sourceRect, bool conversionCopy)
{
	sf::IntRect srcRect = sourceRect;
	if (srcRect.width == 0 || (srcRect.height == 0))
	{
		srcRect.left = 0;
		srcRect.top = 0;
		srcRect.width = sourceWidth;
		srcRect.height = sourceHeight;
	}
	else
	{
		if (srcRect.left < 0)
		{
			srcRect.left = 0;
		}
		if (srcRect.top < 0)
		{
			srcRect.top = 0;
		}
		if (srcRect.width > static_cast<int>(sourceWidth))
		{
			srcRect.width = sourceWidth;
		}
		if (srcRect.height > static_cast<int>(sourceHeight))
		{
			srcRect.height = sourceHeight;
		}
	}

	// Then find the valid bounds of the destination rectangle
	unsigned int width = srcRect.width;
	unsigned int height = srcRect.height;
	if (destX + width > destWidth)
		width = destWidth - destX;
	if (destY + height > destHeight)
		height = destHeight - destY;

	// Make sure the destination area is valid
	if ((width <= 0) || (height <= 0))
		return;

	// Precompute as much as possible
	int srcStride = sourceWidth * 4;
	int dstStride = destWidth * 4;
	const sf::Uint8 *srcPixels =
		reinterpret_cast<const sf::Uint8 *>(source) + (srcRect.left + srcRect.top * sourceWidth) * 4;
	sf::Uint8 *dstPixels = reinterpret_cast<sf::Uint8 *>(dest) + (destX + destY * destWidth) * 4;

	for (unsigned int i = 0; i < height; ++i)
	{
		for (unsigned int j = 0; j < width; ++j)
		{
			const sf::Uint8 *src = srcPixels + j * 4;
			sf::Uint8 *dst = dstPixels + j * 4;
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			if (conversionCopy)
			{
				dst[3] = (src[3] & PA_VISIBLE) ? 255 : 0;
			}
			else
			{
				dst[3] = PA_COLLIDABLE | PA_RAYTRACEABLE | PA_VISIBLE | PA_EDITABLE;
			}
		}

		srcPixels += srcStride;
		dstPixels += dstStride;
	}
}

void Cell::AllocPixels()
{
	mPixels = new CellPixel[PixelCount];

	CalculateImageGridPointers();

	mBackgroundMask = new uint8_t[BackgroundPixelCount];
	for (uint32_t i = 0; i < BackgroundPixelCount; ++i)
	{
		mBackgroundMask[i] = 0; // default texture at layer 0
	}
}

void Cell::CalculateImageGridPointers()
{
	// acquire pointers for each subcell beginning
	for (uint32_t i = 0; i < GridSize; ++i)
	{
		for (uint32_t j = 0; j < GridSize; ++j)
		{
			// each subcell is continuous array of pixels, to minimize cache misses
			mImageGrid[i][j] = mPixels + (i * GridSize + j) * (int)(SubCellSize * SubCellSize);
		}
	}
}

void Cell::TransferPixelsFromGridToLinearOrder(array<sf::Color, PixelCount> &buffer)
{
	constexpr auto pixelCount = SubCellSize * SubCellSize;
	auto bufOrig = buffer.data();
	for (unsigned int i = 0; i < GridSize; ++i)
	{
		const auto yOffset = i * SubCellSize;
		for (unsigned int j = 0; j < GridSize; ++j)
		{
			const auto xOffset = j * SubCellSize;
			const auto pixels = mImageGrid[i][j];
			for (unsigned int counter = 0; counter < pixelCount; ++counter)
			{
				const unsigned int x = xOffset + (counter & SubCellWrapMask);
				const unsigned int y = yOffset + (counter >> SubCellSizePow2);
				const auto pix = pixels + counter;
				auto ptr = bufOrig + y * Size + x;
				ptr->r = pix->r;
				ptr->g = pix->g;
				ptr->b = pix->b;
				ptr->a = (pix->a & PA_VISIBLE) * 255;
			}
		}
	}
}

const uint8_t *Cell::GetBackgroundPixels() const
{
	return mBackgroundMask;
}

bool Cell::IsIntersectsWithMaskOpBounds(const MaskOperation & args)
{
	const float borderHalfThickness = args.mBorderThickness * 0.5f;
	const Vec2f borderOffset = Vec2f(borderHalfThickness, borderHalfThickness);

	if (args.mBounds == Bounds::Circle)
	{
		// check if circle does not intersects with cell's rectangle
		if (!Math::IsCircleSquareIntersection(mPosition, SizeF, args.mPosition, args.mRadius))
		{
			return false;
		}
		// check if circle + border does not intersects with cell's
		// rectangle
		if (!Math::IsCircleSquareIntersection(mPosition - borderOffset, SizeF, args.mPosition, args.mRadius + args.mBorderThickness))
		{
			return false;
		}
	}
	else if (args.mBounds == Bounds::Rect)
	{
		// check if rectangle intersects with cell's rectangle
		if (!Math::IsSquareRectIntersection(mPosition, SizeF, args.mPosition, args.mRectSize))
		{
			return false;
		}
	}
	else if (args.mBounds == Bounds::Polygon)
	{
		// check if polygon intersects with cell's rectangle
		if (!Math::IsSquareRectIntersection(mPosition, SizeF, args.mPolygonBoundsPosition, args.mPolygonBoundsSize))
		{
			return false;
		}
	}
	else if (args.mBounds == Bounds::Point)
	{
		if (!Math::IsPointInsideRectangle(args.mPosition, mPosition, { SizeF, SizeF }))
		{
			return false;
		}
	}

	return true;
}

void Cell::SetIndex(int i)
{
	mIndex = i;
}

int Cell::GetIndex() const
{
	return mIndex;
}

Cell::Cell()
    : mPixels(nullptr)
    , mNeedUpdate(true)
	, mImageGrid { { nullptr } }			
    , mRight(0)
    , mBackgroundMask(nullptr)		
    , mBottom(0)
    , mTextureId(0)	
{

}

Cell::Cell(const Vec2f &position)
	: mPosition(position)
	, mPixels(nullptr)
	, mImageGrid { { nullptr } }
	, mBackgroundMask(nullptr)
	, mNeedUpdate(true)
	, mTextureId(0)
{
	AllocPixels();
	mCenter = mPosition + Vec2f(sf::Vector2u(Cell::Size, Cell::Size) / 2u);
	mRight = mPosition.x + Cell::Size;
	mBottom = mPosition.y + Cell::Size;
}

Cell::~Cell()
{
	delete[] mPixels;
	delete[] mBackgroundMask;
	DBG_ECHO();
}

void Cell::Mask(const MaskOperation &args)
{
	const float r2 = args.mRadius * args.mRadius;
	const float borderHalfThickness = args.mBorderThickness * 0.5f;
	const float br2 = (args.mRadius + borderHalfThickness) * (args.mRadius + borderHalfThickness);
	const Vec2f borderOffset = Vec2f(borderHalfThickness, borderHalfThickness);

	if (!IsIntersectsWithMaskOpBounds(args))
	{
		return;
	}

	mNeedUpdate = true;

	const float kCircleWScale = args.mImageRect.width / (2.0f * args.mRadius);
	const float kCircleHScale = args.mImageRect.height / (2.0f * args.mRadius);

	const float kRectWScale = args.mImageRect.width / args.mRectSize.x;
	const float kRectHScale = args.mImageRect.height / args.mRectSize.y;

	constexpr unsigned int subCellPixelCount = SubCellSize * SubCellSize;

	const Vec2f borderRect {
		args.mRectSize.x + args.mBorderThickness,
		args.mRectSize.y + args.mBorderThickness
	};

	for (unsigned int i = 0; i < GridSize; ++i)
	{
		for (unsigned int j = 0; j < GridSize; ++j)
		{
			const float xOffset = static_cast<float>(j * SubCellSize);
			const float yOffset = static_cast<float>(i * SubCellSize);

			const Vec2f subCellPosition = { mPosition.x + xOffset, mPosition.y + yOffset };

			// check if sub-cell inside figure bounds, or inside border
			if (args.mBounds == Bounds::Circle)
			{
				if (!Math::IsCircleSquareIntersection(subCellPosition, SubCellSizeF, args.mPosition, args.mRadius))
				{
					continue;
				}
				if (!Math::IsCircleSquareIntersection(subCellPosition - borderOffset, SubCellSizeF, args.mPosition, args.mRadius + borderHalfThickness))
				{
					continue;
				}
			}
			else if (args.mBounds == Bounds::Rect)
			{
				if (!Math::IsSquareRectIntersection(subCellPosition, SubCellSizeF, args.mPosition, args.mRectSize))
				{
					continue;
				}
			}
			else if (args.mBounds == Bounds::Polygon)
			{
				if (!Math::IsSquareRectIntersection(subCellPosition, SubCellSizeF, args.mPolygonBoundsPosition, args.mPolygonBoundsSize))
				{
					continue;
				}
			}
			else if (args.mBounds == Bounds::Point)
			{
				if (!Math::IsPointInsideRectangle(args.mPosition, subCellPosition, { SubCellSizeF, SubCellSizeF }))
				{
					continue;
				}
			}

			const auto pixels = mImageGrid[i][j];
			for (unsigned int counter = 0; counter < subCellPixelCount; ++counter)
			{
				// obtain coordinates using bitwise operations
				const unsigned int x = counter & Cell::SubCellWrapMask;
				const unsigned int y = counter >> Cell::SubCellSizePow2;

				auto isEditable = pixels[counter].a & PA_EDITABLE;

				// metal pixels can be edited only if PA_EDITABLE is set
				if ((pixels[counter].a & PA_METAL) && isEditable)
				{
					if (!args.mMeltMetalPixels)
					{
						isEditable = 0;
					}
				}

				// editor access overrides last restrictions
				if (args.mEditorAccess)
				{
					isEditable = true;
				}

				if (!isEditable)
				{
					continue;
				}

				const Vec2f pixelPosition {
					subCellPosition.x + static_cast<float>(x),
					subCellPosition.y + static_cast<float>(y)
				};

				bool isForbidden = false;

				// If pixel is inside any forbidden areas, skip it
				if (!args.mForbiddenRects.empty())
				{
					for (auto & forbiddenRect : args.mForbiddenRects)
					{
						if (forbiddenRect.contains(pixelPosition))
						{
							isForbidden = true;
							break;
						}
					}
				}

				if (!isForbidden)
				{
					if (!args.mForbiddenCircles.empty())
					{
						for (auto & forbiddenCircle : args.mForbiddenCircles)
						{
							const float sqrRadius = forbiddenCircle.mRadius * forbiddenCircle.mRadius;
							if (Math::IsPointInsideCircle(pixelPosition, forbiddenCircle.mPosition, sqrRadius))
							{
								isForbidden = true;
								break;
							}
						}
					}
				}

				bool pixelInside = false;
				bool pixelInsideBorder = false;

				// check if pixel inside figure bounds, or inside border
				if (args.mBounds == Bounds::Circle)
				{
					pixelInside = Math::IsPointInsideCircle(pixelPosition, args.mPosition, r2);
					pixelInsideBorder = Math::IsPointInsideCircle(pixelPosition, args.mPosition, br2);
				}
				else if (args.mBounds == Bounds::Rect)
				{
					pixelInside = Math::IsPointInsideRectangle(pixelPosition, args.mPosition, args.mRectSize);
					pixelInsideBorder = Math::IsPointInsideRectangle(pixelPosition, args.mPosition, borderRect);
				}
				else if (args.mBounds == Bounds::Polygon)
				{
					pixelInside = args.mPolygon.Contains(pixelPosition);
					pixelInsideBorder = false;
				}

				if (!pixelInside)
				{
					if (args.mPixelIgnoreCallback)
					{
						args.mPixelIgnoreCallback(pixelPosition);
					}
				}

				if (pixelInside)
				{
					// edit background if enabled
					if (args.mEditBackground)
					{
						int bx = (j * SubCellSize + x) / BackgroundDownScale;
						int by = (i * SubCellSize + y) / BackgroundDownScale;
						mBackgroundMask[by * BackgroundSize + bx] = args.mBackgroundValue;
					}

					if (isForbidden)
					{
						if (args.mPixelModifyCallback)
						{
							args.mPixelModifyCallback(pixelPosition);
						}

						if (args.mForbiddenBoundsIntersectionBitwiseOperation == BitwiseOp::And)
						{
							pixels[counter].a &= args.mForbiddenBoundsIntersectionMask;
						}
						else if (args.mForbiddenBoundsIntersectionBitwiseOperation == BitwiseOp::Or)
						{
							pixels[counter].a |= args.mForbiddenBoundsIntersectionMask;
						}
						else if (args.mForbiddenBoundsIntersectionBitwiseOperation == BitwiseOp::Xor)
						{
							pixels[counter].a ^= args.mForbiddenBoundsIntersectionMask;
						}

						if (args.mForbiddenBoundsEditBackground)
						{
							int bx = (j * SubCellSize + x) / BackgroundDownScale;
							int by = (i * SubCellSize + y) / BackgroundDownScale;
							mBackgroundMask[by * BackgroundSize + bx] = static_cast<uint8_t>(args.mForbiddenBoundsIntersectionBackgroundValue);
						}

						continue;
					}



					// do replacement (drawing using some image, with specified brush
					// bounds and blending options)
					int doBitmask = args.mDoBitmaskOnlyOnVisibleDestPixels ? (pixels[counter].a & PA_VISIBLE) : 1;
					if (args.mReplaceImage)
					{
						int rx = j * SubCellSize + x;
						int ry = i * SubCellSize + y;

						const Vec2f leftCorner = (args.mBounds == Bounds::Circle) ? (args.mPosition - Vec2f(args.mRadius, args.mRadius)) : args.mPosition;

						if (args.mBrushMode == BrushMode::Wrap)
						{
							// wrap coordinates
							rx = Math::Wrap(rx, args.mImageRect.left, args.mImageRect.left + args.mImageRect.width - 1);
							ry = Math::Wrap(ry, args.mImageRect.top, args.mImageRect.top + args.mImageRect.height - 1);
						}
						else if (args.mBrushMode == BrushMode::WrapBounds)
						{
							// translate pixel coordinates into bounding rect
							// we can do this, because if we are here - pixel inside bounds
							rx = static_cast<int>(pixelPosition.x - leftCorner.x);
							ry = static_cast<int>(pixelPosition.y - leftCorner.y);

							// scale coordinates to fit into bounds
							/*
							if ( args.mBounds == Bounds::Circle )
							{
								rx = static_cast<int>( rx * kCircleWScale );
								ry = static_cast<int>( ry * kCircleHScale );
							}
							else
							{
								rx = static_cast<int>( rx * kRectWScale );
								ry = static_cast<int>( ry * kRectHScale );
							}
							*/

							// wrap coordinates
							rx = Math::Wrap(rx, args.mImageRect.left, args.mImageRect.left + args.mImageRect.width - 1);
							ry = Math::Wrap(ry, args.mImageRect.top, args.mImageRect.top + args.mImageRect.height - 1);
						}
						else if (args.mBrushMode == BrushMode::Insert)
						{
							// translate pixel coordinates into bounding rect
							// we can do this, because if we are here - pixel inside bounds
							rx = static_cast<int>(pixelPosition.x - leftCorner.x);
							ry = static_cast<int>(pixelPosition.y - leftCorner.y);

							// scale coordinates to fit into bounds
							if (args.mBounds == Bounds::Circle)
							{
								rx = static_cast<int>(rx * kCircleWScale);
								ry = static_cast<int>(ry * kCircleHScale);
							}
							else
							{
								rx = static_cast<int>(rx * kRectWScale);
								ry = static_cast<int>(ry * kRectHScale);
							}

							// offset coordinates to image rect origin
							rx += args.mImageRect.left;
							ry += args.mImageRect.top;

							// value can be out of bounds because of rounding, fix it
							if (rx < 0)
							{
								rx = 0;
							}
							if (ry < 0)
							{
								ry = 0;
							}
							if (rx >= args.mReplaceImage->GetWidth())
							{
								rx = args.mReplaceImage->GetWidth() - 1;
							}
							if (ry >= args.mReplaceImage->GetHeight())
							{
								ry = args.mReplaceImage->GetHeight() - 1;
							}
						}

						auto replaceColor = args.mReplaceImage->GetPixels()[rx + ry * args.mReplaceImage->GetWidth()];

						if (args.mDoBitmaskOnlyOnVisiblePixelsOfReplaceImage)
						{
							if (replaceColor.a == 0)
							{
								doBitmask = 0;
							}
						}

						if (args.mBlendOp == BlendOperation::Replace)
						{
							// ignore transparent pixels
							if (replaceColor.a > 0)
							{
								pixels[counter].r = replaceColor.r;
								pixels[counter].g = replaceColor.g;
								pixels[counter].b = replaceColor.b;
								pixels[counter].a &= ~PA_LAVA;
							}
						}
					}

					if (doBitmask)
					{
						if (args.mPixelModifyCallback)
						{
							args.mPixelModifyCallback(pixelPosition);
						}

						int wasLava = pixels[counter].a & PA_LAVA;

						// first pass
						if (args.mFirstPassBitwiseOperation == BitwiseOp::And)
						{
							pixels[counter].a &= args.mFirstPassMask;
						}
						else if (args.mFirstPassBitwiseOperation == BitwiseOp::Or)
						{
							pixels[counter].a |= args.mFirstPassMask;
						}
						else if (args.mFirstPassBitwiseOperation == BitwiseOp::Xor)
						{
							pixels[counter].a ^= args.mFirstPassMask;
						}

						// second pass
						if (args.mSecondPassBitwiseOperation == BitwiseOp::And)
						{
							pixels[counter].a &= args.mSecondPassMask;
						}
						else if (args.mSecondPassBitwiseOperation == BitwiseOp::Or)
						{
							pixels[counter].a |= args.mSecondPassMask;
						}
						else if (args.mSecondPassBitwiseOperation == BitwiseOp::Xor)
						{
							pixels[counter].a ^= args.mSecondPassMask;
						}

						// hot pixels are stored in special array for further cooling
						if (!args.mDoNotAddLavaPixelsToChillList)
						{
							// If was not lava
							if (!wasLava)
							{
								// But become it
								if (pixels[counter].IsLava())
								{
									// Add to chill list
									mHotPixels.emplace_back(&pixels[counter]);
								}
							}
						}
					}
				}
				else if (pixelInsideBorder)
				{
					pixels[counter].r = args.mBorderColor.r;
					pixels[counter].g = args.mBorderColor.g;
					pixels[counter].b = args.mBorderColor.b;

					int wasLava = pixels[counter].a & PA_LAVA;

					if (args.mBorderBitwiseOperation == BitwiseOp::And)
					{
						pixels[counter].a &= args.mBorderMask;
					}
					else if (args.mBorderBitwiseOperation == BitwiseOp::Or)
					{
						pixels[counter].a |= args.mBorderMask;
					}
					else if (args.mBorderBitwiseOperation == BitwiseOp::Xor)
					{
						pixels[counter].a ^= args.mBorderMask;
					}

					// hot pixels are stored in special array for further cooling
					if (!args.mDoNotAddLavaPixelsToChillList)
					{
						// If was not lava
						if (!wasLava)
						{
							// But become it
							if (pixels[counter].IsLava())
							{
								// Add to chill list
								mHotPixels.emplace_back(&pixels[counter]);
							}
						}
					}
				}

			}
		}
	}
}

Vec2f Cell::GetPosition() const
{
	return mPosition;
}

void Cell::Update()
{
	const float chillSpeed = 0.015f;
	const int delta = 10;
	if (mHotPixels.size())
	{
		// TODO: this should be optimized using circular buffer
		for (auto iter = mHotPixels.begin(); iter != mHotPixels.end();)
		{
			auto hotPixel = *iter;
			// pixel can be modified in many places, so we should check if this pixel
			// still lava and if not, remove it from array
			int isLava = hotPixel->a & PA_LAVA;
			if (abs(ColdColor.r - hotPixel->r) < delta || abs(ColdColor.g - hotPixel->g) < delta || abs(ColdColor.b - hotPixel->b) < delta || !isLava)
			{
				hotPixel->a &= ~PA_LAVA;
				iter = mHotPixels.erase(iter);
			}
			else
			{
				const auto dr = (ColdColor.r - hotPixel->r);
				hotPixel->r = static_cast<uint8_t>(hotPixel->r + dr * chillSpeed);
				const auto dg = (ColdColor.g - hotPixel->g);
				hotPixel->g = static_cast<uint8_t>(hotPixel->g + dg * chillSpeed);
				const auto db = (ColdColor.b - hotPixel->b);
				hotPixel->b = static_cast<uint8_t>(hotPixel->b + db * chillSpeed);
				mNeedUpdate = true;
				++iter;
			}
		}
	}
}

void Cell::Fill(MaskOperation &args, const shared_ptr<Texture> &tex)
{
	args.mPosition = mPosition;
	args.mRectSize = { SizeF, SizeF };
	args.mBounds = Bounds::Rect;
	args.mFirstPassBitwiseOperation = BitwiseOp::Or;
	args.SetBrushImage(tex);
	Mask(args);
}

void Cell::Serialize(Serializer &sav)
{
	if (sav.IsLoading())
	{
		AllocPixels();
	}
	sav &mPosition;
	sav &mCenter;
	sav &mRight;
	sav &mBottom;
	sav.SerializeBinary(mPixels, PixelCount * sizeof(*mPixels));
	sav.SerializeBinary(mBackgroundMask, BackgroundPixelCount * sizeof(*mBackgroundMask));
}

// Correct handling collision with bitmap is pain in the ass
// this method uses few tricks to avoid unstable behaviour of
// colliding particles, but of course this is isn't enough
//
// Finally make it works correctly!
void Cell::CircleCollision(Body *circle, bool inside)
{
	// count of pixels, that was inside circle, see explanation at the end of
	// the method
	unsigned int insideCount = 0;
	constexpr unsigned int pixelCount = SubCellSize * SubCellSize;	
	if (inside ||
		Math::IsCircleSquareIntersection(mPosition, SizeF, circle->GetPosition(), circle->mRadius))
	{
		for (unsigned int i = 0; i < GridSize; ++i)
		{
			for (unsigned int j = 0; j < GridSize; ++j)
			{
				const auto xOffset = static_cast<float>(j * SubCellSize);
				const auto yOffset = static_cast<float>(i * SubCellSize);
				const Vec2f subCellPosition = { mPosition.x + xOffset, mPosition.y + yOffset };
				if (Math::IsCircleSquareIntersection(
					subCellPosition, SubCellSizeF, circle->GetPosition(), circle->mRadius))
				{
					const auto pixels = mImageGrid[i][j];
					for (unsigned int counter = 0; counter < pixelCount; ++counter)
					{
						if (pixels[counter].a & PA_COLLIDABLE)
						{
							// obtain coordinates (this trick is well explained in Cell.h)
							const unsigned int x = counter & SubCellWrapMask;
							const unsigned int y = counter >> SubCellSizePow2;
							const Vec2f pixelPosition = { subCellPosition.x + x, subCellPosition.y + y };
							const Vec2f offset = circle->GetPosition() - pixelPosition;
							const float d2 = offset.x * offset.x + offset.y * offset.y;
							if (d2 <= circle->mSqrRadius)
							{
								++insideCount;

								// do collision response
								const float len = sqrtf(d2);
								const Vec2f dirNorm = offset / len;

								if (circle->mFlags & BF_RESPONSE_ENABLED)
								{
									// this helps to avoid "sucking" particle deeper inside
									// terrain
									if (Math::Dot(dirNorm, circle->GetVelocity()) < 0)
									{
										circle->Offset(dirNorm * (circle->mRadius - len));
									}
								}

								// write contact info if needed
								if ((circle->mFlags & BF_IGNORE_PT_CONTACT_INFO) == 0)
								{
									auto contact = circle->AddContact();
									if (contact)
									{
										contact->mCell = this;
										contact->mPixelCoord.x = j * SubCellSize + x;
										contact->mPixelCoord.y = i * SubCellSize + y;
										contact->mCollidedWith = nullptr;
										contact->mPosition = pixelPosition;
										contact->mNormal = dirNorm;
										contact->mPixel = pixels[counter];
									}
								}
							}
						}
					}
				}
			}
		}
	}
	// check new position for validity
	// new position is invalid if count of pixels that was inside circle exceed
	// half of the circle area this means that collision response pushed circle
	// deeply into collidable area of the cell,  instead of pushing it into free
	// area. this can happen when circle moving too fast, or when on stage
	// circle-circle collision, circle was pushed into collidable area of a cell
	// btw, this technique is a sort of hack. very useful hack
	if (insideCount >= circle->GetArea() * 0.5f)
	{
		if (circle->mFlags & BF_RESPONSE_ENABLED)
		{
			circle->SetPosition(circle->mRecoveryPosition);
		}
	}
}

TerrainRayTraceResult Cell::RayIntersection(const Vec2f &origin, const Vec2f &direction)
{
	float sqrClosestDist = FLT_MAX;
	TerrainRayTraceResult result;
	float t0, t1;
	if (!Math::IsRayIntersectRect(
		origin, direction, mPosition, mPosition + Vec2f(SizeF, SizeF), t0, t1))
	{
		return result;
	}
	const Vec2f staticOffset = { SubCellSizeF, SubCellSizeF };
	for (unsigned int i = 0; i < GridSize; ++i)
	{
		for (unsigned int j = 0; j < GridSize; ++j)
		{
			const Vec2f offset = {
				static_cast<float>(j * SubCellSize), static_cast<float>(i * SubCellSize) };
			const Vec2f subCellPosition = mPosition + offset;

			if (!Math::IsRayIntersectRect(
				origin, direction, subCellPosition, subCellPosition + staticOffset, t0, t1))
			{
				continue;
			}

			const auto pixels = mImageGrid[i][j];

			Vec2f p0 = origin + t0 * direction - subCellPosition;
			const Vec2f p1 = origin + t1 * direction - subCellPosition;
			Vec2f dp = p1 - p0;
			const auto distance = Math::Length(dp);
			const auto pixelCount = static_cast<unsigned int>(distance + 1.0f);
			dp *= 1.0f / distance;

			for (unsigned int k = 0; k < pixelCount; ++k)
			{
				const auto x = static_cast<unsigned int>(p0.x);
				const auto y = static_cast<unsigned int>(p0.y);

				if (x >= SubCellSize || y >= SubCellSize)
				{
					p0 += dp;
					continue;
				}

				const auto &pixel = pixels[y * SubCellSize + x];

				if (pixel.a & PA_RAYTRACEABLE)
				{
					result.mIntersection = true;
					const Vec2f newIntersection = subCellPosition + p0;
					const auto sqrDistance = Math::SqrLength(newIntersection - origin);
					if (sqrDistance < sqrClosestDist)
					{
						sqrClosestDist = sqrDistance;
						result.mPosition = newIntersection;
						result.mCell = shared_from_this();
						result.mPixel = pixel;
						result.mPixelCoord = {
							static_cast<int>(j * SubCellSize + x), static_cast<int>(i * SubCellSize + y) };
					}
				}

				p0 += dp;
			}
		}
	}
	return result;
}

void Cell::GatherPixelsInsideCircle(const Vec2f &circlePosition, float circleRadius, int dx, int dy,
	vector<CellTraceResult> &outPositions, uint8_t gatherMask)
{
	if (!Math::IsCircleSquareIntersection(mPosition, SizeF, circlePosition, circleRadius))
	{
		return;
	}

	const float sqrRadius = circleRadius * circleRadius;
	const auto pixelCount = SubCellSize * SubCellSize;

	for (unsigned int i = 0; i < GridSize; ++i)
	{
		for (unsigned int j = 0; j < GridSize; ++j)
		{
			const Vec2f offset = {
				static_cast<float>(j * SubCellSize), static_cast<float>(i * SubCellSize) };

			const Vec2f subCellPosition = mPosition + offset;
			if (!Math::IsCircleSquareIntersection(
				subCellPosition, SubCellSizeF, circlePosition, circleRadius))
			{
				continue;
			}

			const auto pixels = mImageGrid[i][j];
			for (unsigned int counter = 0; counter < pixelCount; ++counter)
			{
				if ((pixels[counter].a & gatherMask) == gatherMask)
				{
					// obtain coordinates (this trick is well explained in Cell.h)
					const unsigned int x = counter & SubCellWrapMask;
					const unsigned int y = counter >> SubCellSizePow2;
					const Vec2f pixPos = { subCellPosition.x + x, subCellPosition.y + y };
					const Vec2i pixRelPos = {
						static_cast<int>(j * SubCellSize + x), static_cast<int>(i * SubCellSize + y) };
					if (Math::SqrDistance(pixPos, circlePosition) <= sqrRadius)
					{
						if ((pixRelPos.x % dx == 0) && (pixRelPos.y % dy == 0))
						{
							outPositions.emplace_back(pixPos, pixRelPos, pixels[counter]);
						}
					}
				}
			}
		}
	}
}

void Cell::GatherPixelsInsideRect(const Vec2f &rectPosition, const Vec2f &rectSize, int dx, int dy,
	vector<CellTraceResult> &outPositions, uint8_t gatherMask)
{
	if (!Math::IsSquareRectIntersection(mPosition, SizeF, rectPosition, rectSize))
	{
		return;
	}

	constexpr unsigned int pixelCount = SubCellSize * SubCellSize;

	for (unsigned int i = 0; i < GridSize; ++i)
	{
		for (unsigned int j = 0; j < GridSize; ++j)
		{
			const Vec2f offset = { j * SubCellSizeF, i * SubCellSizeF };
			const Vec2f subCellPosition = mPosition + offset;
			if (!Math::IsSquareRectIntersection(subCellPosition, SubCellSizeF, rectPosition, rectSize))
			{
				continue;
			}
			const auto pixels = mImageGrid[i][j];
			for (unsigned int counter = 0; counter < pixelCount; ++counter)
			{
				if ((pixels[counter].a & gatherMask) == gatherMask)
				{
					// obtain coordinates (this trick is well explained in Cell.h)
					const unsigned int x = counter & SubCellWrapMask;
					const unsigned int y = counter >> SubCellSizePow2;
					const Vec2f pixPos = { subCellPosition.x + x, subCellPosition.y + y };
					const Vec2i pixRelPos = {
						static_cast<int>(j * SubCellSize + x),
						static_cast<int>(i * SubCellSize + y)
					};
					if (Math::IsPointInsideRectangle(pixPos, rectPosition, rectSize))
					{
						if ((pixRelPos.x % dx == 0) && (pixRelPos.y % dy == 0))
						{
							outPositions.emplace_back(pixPos, pixRelPos, pixels[counter]);
						}
					}
				}
			}
		}
	}
}

CellPixel * Cell::GetPixel(int x, int y)
{
	constexpr int subCellVolume = (int)(SubCellSize * SubCellSize);

	const int cx = x >> SubCellSizePow2;
	const int cy = y >> SubCellSizePow2;

	auto subCellBegin = mPixels + (cy * GridSize + cx) * subCellVolume;

	const int cbx = x - cx * GridSize;
	const int cby = y - cy * GridSize;

	return subCellBegin + cby * GridSize + cbx;
}

// coordinates given in cell pixel coordinates

uint8_t * Cell::GetBackgroundMaskPixel(int x, int y)
{
	x /= BackgroundDownScale;
	y /= BackgroundDownScale;

	return mBackgroundMask + y * BackgroundSize + x;
}

shared_ptr<Cell> Cell::MakeCopy() const
{
	shared_ptr<Cell> copy = make_shared<Cell>(mPosition);

	copy->mIndex = mIndex;
	copy->mCompressed = mCompressed;
	copy->mCompressedSize = mCompressedSize;

	memcpy(copy->mPixels, mPixels, PixelCount * sizeof(*mPixels));
	memcpy(copy->mBackgroundMask, mBackgroundMask, BackgroundPixelCount * sizeof(*mBackgroundMask));

	return copy;
}

CellTraceResult::CellTraceResult(const Vec2f &position, const sf::Vector2i &relative, const CellPixel &pix)
	: mPosition(position)
	, mRelative(relative)
	, mColor(pix)
{
}
