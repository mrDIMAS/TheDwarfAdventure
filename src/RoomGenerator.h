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

#include "Bundle.h"
#include "Room.h"
#include "LocationGenerator.h"

class RoomGenerator : public LocationGeneratorModule
{
public:
	sf::FloatRect mBounds;
	sf::FloatRect mEntranceBounds;

	RoomGenerator(LocationGenerator * parentGenerator);
	virtual void Generate() override;
	void AllowInhabitants(bool state);
private:
	bool mAllowInhabitants { true };
	float mWallThickness { 8 };
	float mAverageInhabitantHeight { 60 };
	float mLadderWidth { 40 };
	Bundle<RoomType> mPossibleRoomTypes = {
		{ 0.33f, RoomType::UndergroundApartment },
		{ 0.33f, RoomType::Forge },
		{ 0.33f, RoomType::Store }
	};
};
