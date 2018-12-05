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

#include "PixelAttribute.h"

struct CellPixel
{
	unsigned char r { 255 };
	unsigned char g { 255 };
	unsigned char b { 255 };
	unsigned short a { PA_VISIBLE | PA_COLLIDABLE | PA_RAYTRACEABLE | PA_EDITABLE };

	void SetColor(unsigned char red, unsigned char green, unsigned char blue)
	{
		r = red;
		g = green;
		b = blue;
	}

	void SetAttrib(unsigned char attrib)
	{
		a = attrib;
	}

	bool IsVisible() const
	{
		return (a & PA_VISIBLE) != 0;
	}

	bool IsCollidable() const
	{
		return (a & PA_COLLIDABLE) != 0;
	}

	bool IsRaytraceable() const
	{
		return (a & PA_COLLIDABLE) != 0;
	}

	bool IsEditable() const
	{
		return (a & PA_EDITABLE) != 0;
	}

	bool IsLava() const
	{
		return (a & PA_LAVA) != 0;
	}

	bool IsAcid() const
	{
		return (a & PA_ACID) != 0;
	}

	bool IsHydrophobic() const
	{
		return (a & PA_HYDROPHOBIC) != 0;
	}

	bool IsMetal() const
	{
		return (a & PA_METAL) != 0;
	}
};
