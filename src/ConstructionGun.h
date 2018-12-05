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

#include "Weapon.h"
#include "Cell.h"

class ConstructionGun : public Weapon
{
private:
public:
	virtual void OnUpdate() override final;
	virtual void OnSerialize(Serializer &sav) override final;
	shared_ptr<Sprite> mBlockBounds;
	vector<CellTraceResult> mGatheredPixels;
	bool mValidPlace { false };
	virtual void OnSetVisible(bool vis) override final;

public:
	ConstructionGun();
	ConstructionGun(const shared_ptr<Location> &location);
	~ConstructionGun();
	virtual bool Shoot(const Vec2f &direction) override final;
};
