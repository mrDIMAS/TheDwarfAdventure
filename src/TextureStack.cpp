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
#include "TextureStack.h"
#include "stb_image.h"
#include "Serializer.h"

TextureStack::TextureStack()
{
}

TextureStack::TextureStack(int width, int height) : TextureStack()
{
	mWidth = width;
	mHeight = height;
	Push("data/textures/dummy.png");
}

TextureStack::~TextureStack()
{
	free(mPixels); // free cuz realloc into Push
}

sf::Color * TextureStack::GetPixels()
{
	return mPixels;
}

void TextureStack::Push(const string &file)
{
	if (mSources.size() >= MaxSize - 1)
	{
		throw Exception("TextureStack - Max size exceeded!");
	}
	// load texture to RAM
	int channels = 0, width = 0, height = 0;
	auto srcPixels = reinterpret_cast<sf::Color*>(stbi_load(file.c_str(), &width, &height, &channels, STBI_rgb_alpha));
	if (!srcPixels)
	{
		throw Exception(StringBuilder("TextureStack::Push - file does not exist or corrupted or unsupported: ") << file);
	}
	if (width != mWidth || height != mHeight)
	{
		throw Exception(StringBuilder("TextureStack can be filled only with textures with defined size: ")
			<< mWidth << "x" << mHeight << ". Passed size: " << width << "x" << height);
	}
	// texture match, stack it
	mSources.push_back(file);
	int pixelCount = mWidth * mHeight;
	int layerSize = pixelCount * sizeof(mPixels[0]);
	int newSize = layerSize * mSources.size();
	auto newPixels = reinterpret_cast<sf::Color*>(realloc(mPixels, newSize));
	if (!newPixels)
	{
		throw Exception("TextureStack - Realloc failed!");
	}
	mPixels = newPixels;
	sf::Color *destPos = mPixels + (pixelCount * (mSources.size() - 1));
	memcpy(destPos, srcPixels, layerSize);
	stbi_image_free(srcPixels);
	NeedUploadToGPU = true;
}

void TextureStack::Serialize(Serializer &sav)
{
	sav &mWidth;
	sav &mHeight;
	sav &mSources;
	if (sav.IsLoading())
	{
		// reload each source on load
		auto sources = mSources;
		mSources.clear(); // HAAAAAAAAAAAAAAAAAAAAAX!!!! cuz Push will push new src
						  // into mSources
		for (auto &src : sources)
		{
			Push(src);
		}
	}
}
