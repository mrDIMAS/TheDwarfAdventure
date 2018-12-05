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
#include "FogOfWar.h"
#include "Texture.h"
#include "Renderer.h"
#include "Game.h"

FogOfWar::FogOfWar()
{

}

FogOfWar::FogOfWar(const Vec2i & size)
{
	// Create opaque black texture
	mTexture = game->GetRenderer()->CreateTexture(size.x, size.y, sf::Color(0, 0, 0, 255));

	// Turn on bilinear interpolation to make fog of war smooth
	mTexture->SetSmooth(true);

	mTimedPixels.resize(size.x * size.y);
}

FogOfWar::~FogOfWar()
{

}

void FogOfWar::RevealAt(const Vec2f & pointNormalizedCoords, unsigned int time)
{
	static const Vec2i offsets[] = {
		{ 1, 0 }, // Right
		{ -1, 0 }, // Left
		{ 0, 1 }, // Bottom
		{ 0, -1 } // Top
	};

	PixelPosition pixPos = NormalizedToPixelPosition(pointNormalizedCoords);

	RevealPixel(pixPos, time);

	// Reveal additional pixels 
	for (auto & offset : offsets)
	{
		RevealPixel({ pixPos.x + offset.x, pixPos.y + offset.y }, time);
	}
}

FogOfWarProbeResult FogOfWar::Probe(const Vec2f & pointNormalizedCoords)
{
	PixelPosition pixPos = NormalizedToPixelPosition(pointNormalizedCoords);

	const unsigned int index = PixelPositionToIndex(pixPos);

	FogOfWarProbeResult out;

	out.mColor = mTexture->GetPixels()[index];
	out.mTime = mTimedPixels[index].mTimer;

	return out;
}

void FogOfWar::Update()
{
	unsigned int index = 0;

	sf::Color * const pixels = mTexture->GetPixels();

	for (TimedPixel & timedPixel : mTimedPixels)
	{
		if (timedPixel.mTimer > 0)
		{
			--timedPixel.mTimer;
		}
		else
		{
			// Fade out when time is over
			unsigned int newAlpha = pixels[index].a + 1;

			if (newAlpha <= 255)
			{
				mIsDirty = true;

				pixels[index].a = static_cast<unsigned char>(newAlpha);
			}
		}
		++index;
	}

	if (mIsDirty)
	{
		game->GetRenderer()->ScheduleTextureGPUUpload(mTexture);

		mIsDirty = false;
	}
}

void FogOfWar::Serialize(Serializer & sav)
{
	sav & mTexture;
	sav & mTimedPixels;
}

shared_ptr<Texture> FogOfWar::GetTexture() const
{
	return mTexture;
}

unsigned int FogOfWar::GetWidth() const
{
	return static_cast<unsigned int>(mTexture->GetWidth());
}

unsigned int FogOfWar::GetHeight() const
{
	return static_cast<unsigned int>(mTexture->GetHeight());
}

unsigned int FogOfWar::GetPixelCount() const
{
	return GetWidth() * GetHeight();
}

PixelPosition FogOfWar::NormalizedToPixelPosition(const Vec2f & pointNormalizedCoords)
{
	unsigned int x = static_cast<unsigned int>(pointNormalizedCoords.x * mTexture->GetWidth());
	unsigned int y = static_cast<unsigned int>(pointNormalizedCoords.y * mTexture->GetHeight());

	unsigned int w = static_cast<unsigned int>(mTexture->GetWidth());
	unsigned int h = static_cast<unsigned int>(mTexture->GetHeight());

	if (x >= w)
	{
		x = w - 1;
	}
	if (y >= h)
	{
		y = h - 1;
	}

	return { x, y };
}

unsigned int FogOfWar::PixelPositionToIndex(const PixelPosition & pixPos) const
{
	unsigned int index = pixPos.y * GetWidth() + pixPos.x;
	unsigned int pixelCount = GetPixelCount();

	if (index >= pixelCount)
	{
		index = pixelCount - 1;
	}

	return index;
}

void FogOfWar::RevealPixel(const PixelPosition & pixPos, unsigned int time)
{
	const unsigned int index = PixelPositionToIndex(pixPos);

	if (time > mTimedPixels[index].mTimer)
	{
		mTimedPixels[index].mTimer = time;
	}

	mTexture->GetPixels()[index].a = 0;

	mIsDirty = true;
}

FogOfWar::TimedPixel::TimedPixel()
{

}

FogOfWar::TimedPixel::TimedPixel(unsigned int timer) : mTimer(timer)
{

}

void FogOfWar::TimedPixel::Serialize(Serializer & sav)
{
	sav & mTimer;
}
