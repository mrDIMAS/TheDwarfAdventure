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

struct FogOfWarProbeResult
{
	sf::Color mColor;
	unsigned int mTime;
};

struct PixelPosition
{
	unsigned int x;
	unsigned int y;
};

class FogOfWar
{
public:
	static constexpr unsigned int InfiniteTime = UINT_MAX;

	FogOfWar();
	FogOfWar(const Vec2i & size);
	~FogOfWar();
	void RevealAt(const Vec2f & pointNormalizedCoords, unsigned int time);
	FogOfWarProbeResult Probe(const Vec2f &pointNormalizedCoords);
	void Update();
	void Serialize(Serializer & sav);
	shared_ptr<Texture> GetTexture() const;
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	unsigned int GetPixelCount() const;
private:
	PixelPosition NormalizedToPixelPosition(const Vec2f &pointNormalizedCoords);

	// Dedicated struct to hold times for each pixel of fog of war
	// When time reach zero, pixel becomes opaque hiding stuff below it
	struct TimedPixel
	{
		// Setting this time to InfiniteTime (2^32) will give you 828 days before it reach zero
		// (of course if you don't change FixedFPS which by default is 60 Hz)
		unsigned int mTimer { 0 };

		TimedPixel();
		TimedPixel(unsigned int timer);

		void Serialize(Serializer & sav);
	};

	vector<TimedPixel> mTimedPixels;
	shared_ptr<Texture> mTexture;
	bool mIsDirty { true };

	unsigned int PixelPositionToIndex(const PixelPosition & pixPos) const;
	void RevealPixel(const PixelPosition & pixPos, unsigned int time);
};