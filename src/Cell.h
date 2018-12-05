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

class Terrain;
class Body;
class Texture;

#include "Collision.h"
#include "CellPixel.h"
#include "MaskOperation.h"

// Compile-time function to calculate 2 ^ n
uint32_t constexpr CompileTimePow2(uint32_t exponent)
{
	return exponent == 0 ? 1 : 2 * CompileTimePow2(exponent - 1);
}

struct CellTraceResult
{
	Vec2f mPosition;        // world coordinates
	sf::Vector2i mRelative; // x, y index of pixel
	CellPixel mColor;
	CellTraceResult(const Vec2f &position, const sf::Vector2i &relative, const CellPixel &pix);
};

/*
 * Cell is square with pow2 side, this gives us some bit-magic tricks
 * when iterating through pixels, we can replace
 * 
 * for(int y = 0; y < Size; ++y) {
 *	 for(int x = 0; x < Size; ++x) {
 *	   int index = y * size + x;
 *	 }
 * }
 *
 *
 * With this
 *
 * for(unsigned int index = 0; index < Size * Size; ++index) {
 *	 unsigned int x = index & (Size - 1);
 *	 unsigned int y = index >> SizePow2;
 * }
 *
 *
 * this trick gives at least 30% boost
 *
 * each cell divided into more small pieces defining a grid
 * each sub-cell aligned in memory in linear order, this reduces number of
 * cache misses and improves performance as well
 *
 * here is some illustration
 *
 * Step one: divide bitmap using grid
 *
 * ******** | ******** | ******** |
 * ******** | ******** | ******** |
 * ******** | ******** | ******** |
 * ******** | ******** | ******** |
 * ******** | ******** | ******** |
 * --------------------------------
 * ******** | ******** | ******** |
 * ******** | ******** | ******** |
 * ******** | ******** | ******** |
 * ******** | ******** | ******** |
 * ******** | ******** | ******** |
 * --------------------------------
 * ******** | ******** | ******** |
 * ******** | ******** | ******** |
 * ******** | ******** | ******** |
 * ******** | ******** | ******** |
 * ******** | ******** | ******** |
 *
 * Step two: align each cell linearly

 * ********************************
 * **************** | *************
 * ********************************
 * *** | **************************
 * ********************** | *******
 * ********************************
 * ********* | ********************
 * **************************** | *
 * ********************************
 * *************** | **************
 * ********************************
 * ** | ***************************
 * ********************* | ********
 * ********************************
 * ********
 *
 * Now each grid cell pixels aligned linearly in memory
 * as you know (or not), cpu likes data in linear order,
 * cpu can fetch data in such order very fast, because data nicely fits into
 * cache line with such trick you can easily achieve up to 50% boost
 *
 * Alpha value of each pixel is bit set of attributes
 */
class Cell : public enable_shared_from_this<Cell>
{
public:
	using uint = unsigned int;
	// ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
	// ! WARNING: DO NOT TOUCH! YOU MUST BE COMPLETELY SURE WHAT ARE YOU DOING !
	// ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
	// Change only this, other values will be calculated automatically
	static constexpr uint SubCellSizePow2 = 4;
	static constexpr uint SubCellWrapMask = CompileTimePow2(SubCellSizePow2) - 1;
	static constexpr uint SubCellSize = CompileTimePow2(SubCellSizePow2);
	static constexpr float SubCellSizeF = static_cast<float>(SubCellSize);
	static constexpr float SubCellInvSize = 1.0f / SubCellSize;
	// 256x256 texture
	static constexpr uint Size = CompileTimePow2(8);
	static constexpr float SizeF = static_cast<float>(Size);
	static constexpr float InvSize = 1.0f / Size;
	static constexpr uint GridSize = Size / SubCellSize;
	static constexpr uint PixelCount = Size * Size;
	// with defaults gives 64x64 8-bit texture
	static constexpr uint BackgroundDownScale = 4;
	static constexpr uint BackgroundSize = Size / BackgroundDownScale;
	static constexpr uint BackgroundPixelCount = BackgroundSize * BackgroundSize;

	static sf::Color ColdColor;
	static sf::Color HotColor;

	// some precomputed values for speed up things
	Vec2f mCenter; // = mPosition + Vec2f(Cell::Size, Cell::Size)
	float mRight;  // = mPosition.x + Cell::Size
	float mBottom; // = mPosition.y + Cell::Size

	// set to true in Mask op
	// renderer will set this flag to false
	bool mNeedUpdate;
	// Do not use these textures!
	// They only valid if cell is inside screen bounds otherwise they will be deleted
	GLuint mTextureId { 0 };
	GLuint mBackgroundMaskTextureId { 0 };

	Cell();
	Cell(const Vec2f &position);
	~Cell();

	// this method is "work-horse" for editing contents of a cell
	// by calling this method with appropriate arguments in MaskOperation
	// you can cut holes with different bounds, apply special flags to pixels
	// paste some image into cell with wrapping and so on
	void Mask(const MaskOperation &args);
	Vec2f GetPosition() const;
	void Update();
	void Fill(MaskOperation &args, const shared_ptr<Texture> &tex);
	void Serialize(Serializer &sav);
	// raw pointer to remove redundant overhead
	// 'inside' means that 'circle' is fully inside cell and we can skip some
	// checks
	void CircleCollision(Body *circle, bool inside);
	TerrainRayTraceResult RayIntersection(const Vec2f &origin, const Vec2f &direction);
	// fills output array with positions of each pixel that inside circle bounds
	// pixel will be put in outPositions only if it attributes compliant with
	// gatherMask: attrib & gatherMask == gatherMask -> put pixel
	void GatherPixelsInsideCircle(const Vec2f &circlePosition, float circleRadius, int dx, int dy,
		vector<CellTraceResult> &outPositions, uint8_t gatherMask = PA_VISIBLE);
	// fills output array with positions of each pixel that inside rect bounds
	// pixel will be put in outPositions only if it attributes compliant with
	// gatherMask: attrib & gatherMask == gatherMask -> put pixel
	// you should clear array before passing it this method
	void GatherPixelsInsideRect(const Vec2f &rectPosition, const Vec2f &rectSize, int dx, int dy,
		vector<CellTraceResult> &outPositions, uint8_t gatherMask = PA_VISIBLE);
	CellPixel *GetPixel(int x, int y);
	// coordinates given in cell pixel coordinates
	uint8_t * GetBackgroundMaskPixel(int x, int y);
	// Create copy of this cell
	// NOTE: Does not copies hot pixels!
	// Mostly used in editor
	shared_ptr<Cell> MakeCopy() const;
	// pixels arranged with a grid, each cell of grid aligned linear in memory.
	// to get pixels in standard order (2d image) we have to copy each grid cell
	// into buffer
	void TransferPixelsFromGridToLinearOrder(array<sf::Color, PixelCount> &buffer);
	const uint8_t *GetBackgroundPixels() const;
	bool IsIntersectsWithMaskOpBounds(const MaskOperation & args);
	void SetIndex(int i);
	int GetIndex() const;

	// Only for storage purposes! Engine fill throw exception if you try to send compressed
	// data to for rendering, collision detection, etc.
	// Very handy method for terrain paint commands in editor
	void Compress();
	void Decompress();
	bool IsCompressed() const;
	shared_ptr<Cell> MakeCompressedCopy();
private:
	CellPixel *mPixels;
	// mPixels divided into small pieces, with linear alignment in memory,
	// to reduce number of cache misses
	CellPixel *mImageGrid[GridSize][GridSize];
	vector<CellPixel *> mHotPixels;
	Vec2f mPosition;
	bool mCompressed { false };
	size_t mCompressedSize { 0 };
	// Index of cell in array where it is stored
	int mIndex { 0 };
	void CopyPixels(sf::Color *dest, uint32_t destWidth, uint32_t destHeight,
		const sf::Color *source, int sourceWidth, int sourceHeight, uint32_t destX,
		uint32_t destY, const sf::IntRect &sourceRect, bool conversionCopy);
	void AllocPixels();
	void CalculateImageGridPointers();
	// background mask is array of indices of textures in volume texture in
	// shader when pixel of the cell marked as invisible, shader will fetch
	// texture with index defined in mask and replace pixel of the cell with
	// pixel from texture used in rendering to make background more rich in
	// details usually background is four times smaller than size of the cell
	uint8_t *mBackgroundMask;
};
