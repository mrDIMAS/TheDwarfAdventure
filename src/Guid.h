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

// UUID4
class Guid
{
public:
	// Creates empty {00000000-0000-0000-0000-000000000000} UUID
	Guid();
	// Creates guid using two 64-bit words
	Guid(uint64_t left, uint64_t right);
	string ToString(bool hyphenate = true) const;
	// Generates new UUID4 
	static Guid Generate();
	static bool TryParse(const std::string& str, Guid& out);
	bool operator == (const Guid& other) const;
	const uint64_t* GetData() const;
	void Serialize(Serializer& sav);

	static Guid Empty;
private:
	uint64_t mData[2];
	static std::mt19937_64 mEngine;
};

namespace std
{
	template <>
	struct hash<Guid>
	{
		std::size_t operator()(const Guid &k) const
		{
			return std::hash<uint64_t>()(k.GetData()[0] ^ k.GetData()[1]);
		}
	};
}