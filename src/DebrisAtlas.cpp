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
#include "DebrisAtlas.h"
#include "Texture.h"
#include "Level.h"
#include "Renderer.h"

void DebrisAtlas::CopyPixels(const sf::Color *source, int sourceWidth, int sourceHeight, unsigned int destX, unsigned int destY, const sf::IntRect &sourceRect)
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

	if (destX >= Size)
	{
		destX = Size - 1;
	}
	if (destY >= Size)
	{
		destY = Size - 1;
	}

	if (destX + width > Size)
	{
		width = Size - destX;
	}
	if (destY + height > Size)
	{
		height = Size - destY;
	}

	// Make sure the destination area is valid
	if (width == 0 || height == 0)
	{
		return;
	}

	if (destX + width >= Size || destY + height >= Size)
	{
		return;
	}

	// Precompute as much as possible
	int srcStride = sourceWidth * 4;
	int dstStride = Size * 4;
	const sf::Uint8 *srcPixels = reinterpret_cast<const sf::Uint8 *>(source) + (srcRect.left + srcRect.top * sourceWidth) * 4;
	sf::Uint8 *dstPixels = reinterpret_cast<sf::Uint8 *>(mTexture->GetPixels()) + (destX + destY * Size) * 4;

	for (unsigned int i = 0; i < height; ++i)
	{
		for (unsigned int j = 0; j < width; ++j)
		{
			const sf::Uint8 *src = srcPixels + j * 4;
			sf::Uint8 *dst = dstPixels + j * 4;
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			dst[3] = src[3];
		}
		srcPixels += srcStride;
		dstPixels += dstStride;
	}
}

DebrisAtlas::DebrisAtlas()
{
	mTexture = game->GetRenderer()->CreateTexture(Size, Size);
	for (int i = 0; i < PieceCount; ++i)
	{
		mPieces[i] = false;
	}
}

DebrisAtlas::~DebrisAtlas()
{
	DBG_ECHO();
}

shared_ptr<Texture> DebrisAtlas::GetTexture() const
{
	return mTexture;
}

// packs new piece into atlas and returns it's id
// this method expect that data pointer referred to memory block with size
// (Debris::Radius * 2) ^ 2
int DebrisAtlas::Pack(const sf::Color *source, int sourceWidth, int sourceHeight, int sourceX,
	int sourceY, sf::FloatRect &outTexCoords)
{
	// find free index
	int freeIndex = 0;
	for (int i = 0; i < PieceCount; ++i)
	{
		if (mPieces[i] == false)
		{
			freeIndex = i;
			break;
		}
	}

	// precompute some constants
	constexpr int k = Debris::Radius * 2;
	constexpr float pieceSize = static_cast<float>(Debris::Radius * 2);
	constexpr float invSize = 1.0f / static_cast<float>(Size);

	const int x = k * (freeIndex & WrapMask);
	const int y = k * (freeIndex >> WidthPiecesPow2);

	CopyPixels(source, sourceWidth, sourceHeight, x, y,
		sf::IntRect(sourceX, sourceY, Debris::Radius * 2, Debris::Radius * 2));

	// compute out texture coordinates
	outTexCoords.left = static_cast<float>(x) * invSize;
	outTexCoords.top = static_cast<float>(y) * invSize;
	outTexCoords.width = pieceSize * invSize;
	outTexCoords.height = outTexCoords.width;

	// mark piece as busy
	mPieces[freeIndex] = true;

	NeedUploadToGpu = true;

	return freeIndex;
}

// marks piece as free. id - value returned from Pack
void DebrisAtlas::Free(int id)
{
	if (id >= 0)
	{
		mPieces[id] = false;
	}
}

void DebrisAtlas::Serialize(Serializer &sav)
{
	sav &mTexture;
	// TODO: this can produce "version dependent" saves, which depends on
	// PieceCount
	for (int i = 0; i < PieceCount; ++i)
	{
		sav &mPieces[i];
	}
}
