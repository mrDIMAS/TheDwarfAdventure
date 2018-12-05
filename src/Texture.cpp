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
#include "Texture.h"

// On Windows we use static libraries for SMFL which are already
// contains compiled version of stb_image
#ifndef _MSC_VER
#  define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"
#include "Game.h"
#include "Serializer.h"
#include "Renderer.h"

// trying to load texture from file to RAM, does not load texture on GPU
// use Renderer::ScheduleTextureGPUUpload to push texture to upload queue
void Texture::LoadPixelsFromFile(const string &path, const sf::Color &mask)
{
	string normalizedPath = path;
	for (auto& c : normalizedPath)
	{
		if (c == '\\')
		{
			c = '/';
		}
	}

	// load texture to RAM
	int channels = 0;
	mPixels = (sf::Color *)stbi_load(normalizedPath.c_str(), &mWidth, &mHeight, &channels, STBI_rgb_alpha);

	if (mPixels)
	{
		// apply color mask
		for (int i = 0; i < mWidth * mHeight; ++i)
		{
			if (mPixels[i].r == mask.r && mPixels[i].g == mask.g && mPixels[i].b == mask.b)
			{
				mPixels[i].a = 0;
			}
		}

		mSource = normalizedPath;
	}
	else
	{
		throw Exception(StringBuilder("Unable to load ") << normalizedPath << " texture!");
	}
}

void Texture::AllocPixels(int w, int h)
{
	mWidth = w;
	mHeight = h;
	mPixels = reinterpret_cast<sf::Color *>(malloc(mWidth * mHeight * sizeof(sf::Color)));
}

sf::Color *Texture::GetPixels()
{
	return mPixels;
}

Texture::Texture()
{
}

Texture::~Texture()
{
	if (game->GetRenderer())
	{
		game->GetRenderer()->ScheduleTextureGPUDeletion(mGLTexture);
	}

	stbi_image_free(mPixels);
	DBG_ECHO();
}

void Texture::Serialize(Serializer &sav)
{
	sav &mSource;
	sav &mWidth;
	sav &mHeight;
	sav &mSmooth;
	sav &mRepeated;

	auto renderer = game->GetRenderer();
	if (mSource.size())
	{
		if (sav.IsLoading())
		{
			// texture was loaded from other file, reload it on loading time
			LoadPixelsFromFile(mSource); // TODO: Fix masking, need to save it, cuz it will be lost on loading
			// register texture in renderer
			renderer->RegisterTexture(shared_from_this());
		}
	}
	else
	{
		// texture was created in runtime, dump\read it's pixels to\from file
		if (sav.IsLoading())
		{
			AllocPixels(mWidth, mHeight);
		}
		sav.SerializeBinary(mPixels, mWidth * mHeight * sizeof(mPixels[0]));
	}
	if (sav.IsLoading())
	{
		renderer->ScheduleTextureGPUUpload(shared_from_this());
	}
}

Vec2f Texture::GetSize() const
{
	return Vec2f(static_cast<float>(mWidth), static_cast<float>(mHeight));
}

void Texture::PushToUpdateQueue()
{
	game->GetRenderer()->ScheduleTextureGPUUpload(shared_from_this());
}

void Texture::DrawRect(const sf::FloatRect &r, const sf::Color &clr)
{
	sf::IntRect ir(r);
	for (int y = 0; y < mHeight; ++y)
	{
		for (int x = 0; x < mWidth; ++x)
		{
			if (ir.contains(x, y))
			{
				mPixels[y * mWidth + x] = clr;
			}
		}
	}
}

void Texture::DrawCircle(const Vec2f &p, float radius, const sf::Color &clr)
{
	sf::Vector2i ip(p);
	int r = static_cast<int>(radius);
	for (int y = 0; y < mHeight; ++y)
	{
		for (int x = 0; x < mWidth; ++x)
		{
			int dx = ip.x - x;
			int dy = ip.y - y;
			if (dx * dx + dy * dy <= r)
			{
				mPixels[y * mWidth + x] = clr;
			}
		}
	}
}

void Texture::DrawPixel(const Vec2f & p, const sf::Color & clr)
{
	auto x = static_cast<unsigned int>(p.x);
	auto y = static_cast<unsigned int>(p.y);
	if (x < static_cast<unsigned int>(mWidth) && y < static_cast<unsigned int>(mHeight))
	{
		mPixels[y * mWidth + x] = clr;
	}
}

// input rect should be in normalized coordinates [0; 1]

shared_ptr<Texture> Texture::ExtractPiece(const sf::FloatRect &r)
{
	int srcX = Math::Clamp(static_cast<int>(r.left * mWidth), 0, mWidth);
	int srcY = Math::Clamp(static_cast<int>(r.top * mHeight), 0, mHeight);

	int destWidth = Math::Clamp(static_cast<int>(r.width * mWidth), 0, mWidth);
	int destHeight = Math::Clamp(static_cast<int>(r.height * mHeight), 0, mHeight);

	auto texture = make_shared<Texture>();
	texture->AllocPixels(destWidth, destHeight);

	// stupid solution
	for (int destY = 0; destY < destHeight; ++destY)
	{
		for (int destX = 0; destX < destWidth; ++destX)
		{
			texture->mPixels[destY * destWidth + destX] = mPixels[(srcY + destY) * mWidth + srcX + destX];
		}
	}

	return texture;
}

int Texture::GetWidth() const
{
	return mWidth;
}

int Texture::GetHeight() const
{
	return mHeight;
}

string Texture::GetSource() const
{
	return mSource;
}

GLuint Texture::GetOpenGLTexture() const
{
	return mGLTexture;
}

void Texture::SetRepeated(bool state)
{
	if (state != mRepeated)
	{
		mRepeated = state;
		PushToUpdateQueue();
	}
}

bool Texture::IsRepeated() const
{
	return mRepeated;
}

void Texture::SetSmooth(bool state)
{
	if (state != mSmooth)
	{
		mSmooth = state;
		PushToUpdateQueue();
	}
}

bool Texture::IsSmooth() const
{
	return mSmooth;
}

float Texture::GetAspectRatio() const
{
	return mWidth / static_cast<float>(mHeight);
}

