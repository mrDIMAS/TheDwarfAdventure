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

enum PixelAttribute
{
	PA_NONE = 0,
	// 0 - invisible
	PA_VISIBLE = BIT(0),
	// 0 - ignored in collision
	PA_COLLIDABLE = BIT(1),
	// 0 - ignored in ray-tracing
	PA_RAYTRACEABLE = BIT(2),
	// super durable pixel, ignored in Mask method if 0
	PA_EDITABLE = BIT(3),
	// lava pixels doing damage, if pixel in first time becomes lava, then it
	// added to lavaPixels of a cell
	PA_LAVA = BIT(4),
	// acid pixels doing damage
	PA_ACID = BIT(5),
	// 1 - water will not drain thru pixel
	PA_HYDROPHOBIC = BIT(6),
	// pixel that can be destructed by powerful weapon only
	// (also behaves as PA_HYDROPHOBIC)
	PA_METAL = BIT(7),
};