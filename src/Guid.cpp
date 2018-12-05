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
#include "Guid.h"

Guid Guid::Empty;

namespace
{
	bool HexToDec(char symbol, uint8_t& outByte)
	{
		bool result = false;

		if (symbol >= '0' && symbol <= '9')
		{
			outByte = symbol - '0';

			result = true;
		}
		else if (symbol >= 'a' && symbol <= 'f')
		{
			outByte = 10 + symbol - 'a';

			result = true;
		}
		else if (symbol >= 'A' && symbol <= 'F')
		{
			outByte = 10 + symbol - 'A';

			result = true;
		}

		return result;
	}
}

std::mt19937_64 Guid::mEngine;

Guid::Guid()
	: mData { 0 }
{

}

Guid::Guid(uint64_t left, uint64_t right)
	: mData{ left, right }
{

}

string Guid::ToString(bool hyphenate) const
{
	static constexpr const char* symbols = "0123456789abcdef";
	string out;
	const uint8_t* c = reinterpret_cast<const uint8_t*>(mData);
	for (int i = 0; i < sizeof(mData); ++i)
	{
		uint8_t b = c[i];
		out.push_back(symbols[b & 0x0F]);
		out.push_back(symbols[(b >> 4) & 0x0F]);
		if (hyphenate && (i == 3 || i == 5 || i == 7 || i == 9))
		{
			out.push_back('-');
		}
	}
	return out;
}

Guid Guid::Generate()
{
	std::uniform_int_distribution<uint64_t> dist(1, UINT64_MAX);

	const uint64_t left = dist(mEngine);
	const uint64_t right = dist(mEngine);

	return { left, right };
}

bool Guid::operator == (const Guid& other) const
{
	return mData[0] == other.mData[0] && mData[1] == other.mData[1];
}

const uint64_t * Guid::GetData() const
{
	return mData;
}

void Guid::Serialize(Serializer & sav)
{
	sav &mData[0];
	sav &mData[1];
}

bool Guid::TryParse(const std::string & str, Guid & out)
{
	const size_t size = str.size();

	if (size != 36 && size != 32)
	{
		return false;
	}

	uint8_t* data = reinterpret_cast<uint8_t*>(out.mData);

	const bool hyphenated = size == 36;
	int hyphenCount = 0;
	for (size_t i = 0; i < size; )
	{
		if (hyphenated && str[i] == '-')
		{
			++i;
			++hyphenCount;
			continue;
		}

		const char a = str[i++];
		const char b = str[i++];

		uint8_t first_half, second_half;
		if (HexToDec(a, first_half) && HexToDec(b, second_half))
		{
			*data = (second_half << 4) | first_half;
			++data;
		}
		else
		{
			return false;
		}
	}

	if (hyphenCount != 4)
	{
		return false;
	}

	return true;
}
