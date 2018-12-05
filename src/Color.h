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

#include <SFML/Graphics.hpp>

// Compile-time color
struct Color
{
	unsigned char r { 255 };
	unsigned char g { 255 };
	unsigned char b { 255 };
	unsigned char a { 255 };

	constexpr Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) 
		: r(r)
		, g(g)
		, b(b)
		, a(a) 
	{ 

	}

	constexpr Color()
	{

	}

	operator sf::Color() const
	{
		return { r, g, b, a };
	}

	operator ImU32() const
	{
		return ImColor(r, g, b, a);
	}
};
