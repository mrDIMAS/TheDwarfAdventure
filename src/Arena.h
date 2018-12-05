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

class Boss;
class FogGate;

// Represent arena for boss fight
class Arena
{
public:
	Arena();
	Arena(const shared_ptr<Location>& location);
	void AddFogGate(const shared_ptr<FogGate> &fogGate);
	void SetFightArea(const Math::Polygon &area);
	void AddBoss(const shared_ptr<Boss>& boss);
	void Update();
	void Serialize(Serializer& sav);
	shared_ptr<Location> GetLocation() const;
	bool Contains(const Vec2f& pt) const;
private:
	vector<weak_ptr<FogGate>> mControlledFogGates;
	Math::Polygon mFightArea;
	weak_ptr<Location> mLocation;
	vector<weak_ptr<Boss>> mBosses;
};