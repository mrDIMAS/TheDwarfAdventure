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

#include "Debris.h"

class Texture;

// represent atlas for packing debris textures from terrain cells
class DebrisAtlas
{
public:
	// better do not touch these!
	// Size MUST be power of two
	static constexpr int Size = 1024;
	static constexpr int WidthPiecesPow2 = 8;
	static constexpr int WidthPieces = Size / (Debris::Radius * 2);
	static constexpr int WrapMask = WidthPieces - 1;
	static constexpr int PieceCount = WidthPieces * WidthPieces;

private:
	shared_ptr<Texture> mTexture;
	bool mPieces[PieceCount];
	void CopyPixels(const sf::Color *source, int sourceWidth, int sourceHeight, unsigned int destX, unsigned int destY, const sf::IntRect &sourceRect);

public:
	bool NeedUploadToGpu = true;

	DebrisAtlas();
	~DebrisAtlas();
	shared_ptr<Texture> GetTexture() const;
	// packs new piece into atlas and returns it's id
	// this method expect that data pointer referred to memory block with size
	// (Debris::Radius * 2) ^ 2
	int Pack(const sf::Color *source, int sourceWidth, int sourceHeight, int sourceX, int sourceY,
		sf::FloatRect &outTexCoords);
	// marks piece as free. id - value returned from Pack
	// ids < 0 - ignored
	void Free(int id);
	void Serialize(Serializer &sav);
};
