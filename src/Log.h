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

// Logging
constexpr char LineEnd = '\n';
class LogImpl
{
private:
	ofstream mFile;

public:
	LogImpl() : mFile("dwarf.log")
	{
		// Disable bufferization
		mFile.rdbuf()->pubsetbuf(nullptr, 0u);
	}

	~LogImpl()
	{
	}

	template <class T> inline LogImpl &operator<<(const T &value)
	{
		mFile << value;
		cout << value;
		return *this;
	}

	inline LogImpl &operator<<(const Vec2f &value)
	{
		mFile << value.x << "\t" << value.y;
		cout << value.x << "\t" << value.y;
		return *this;
	}
};

LogImpl &Log();