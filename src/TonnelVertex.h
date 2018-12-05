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

class TonnelVertex
{
public:
	TonnelVertex();
	TonnelVertex(const Vec2f &position);
	Vec2f GetPosition() const;
	void SetPosition(const Vec2f & position);
	void LinkWith(const shared_ptr<TonnelVertex> & vertex);
	const vector<shared_ptr<TonnelVertex>> & GetEdgeVertices() const;
	void Serialize(Serializer & sav);
private:
	Vec2f mPosition;
	// list of vertices linked with this vertex
	vector<shared_ptr<TonnelVertex>> mEdgeVertices;
};
