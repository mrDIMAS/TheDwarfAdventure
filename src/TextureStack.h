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

// Note: 0 index is reserved, indexation starts from 1
class TextureStack
{
private:
	friend class Renderer;
	static constexpr unsigned int MaxSize = UINT8_MAX; // do not exceed UINT8_MAX !!!
	vector<string> mSources;
	int mWidth { 0 };
	int mHeight { 0 };
	GLuint mVolumeTextureID { 0 };
	sf::Color *mPixels { nullptr };

public:
	// set this flag to true to force renderer to re-upload texture on gpu
	// after this, renderer will set this flag to false
	bool NeedUploadToGPU { true };
	TextureStack();
	TextureStack(int width, int height);
	~TextureStack();
	sf::Color * GetPixels();
	void Push(const string &file);
	void Serialize(Serializer &sav);
};
