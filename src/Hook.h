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

enum class HookDestType
{
	None,
	Terrain,
	Actor,
};

class Hook : public Weapon
{
private:
	shared_ptr<Sprite> mHook;
	shared_ptr<PolyLine> mChainModel;
	shared_ptr<Chain> mChain;
	HookDestType mHookDestType { HookDestType::None };
	float mMaxRopeLength { 250 };

	virtual void OnSerialize(Serializer &sav) override final;
	virtual void OnSetVisible(bool vis) override final;
	virtual void OnUpdate() override final;

	void StaticConnectTo(const shared_ptr<Body> &ownerBody, const Vec2f &from, const Vec2f &to);
	void DynamicConnectTo(const shared_ptr<Body> &ownerBody, const Vec2f &from, const shared_ptr<Actor> &targetActor,
		const Vec2f &connectPoint);

	bool mHookThrown { false };
	float mChainScale { 0.0f };
	void ThrowHook(const Vec2f & direction, const Vec2f & destPosition
	);
public:
	Hook();
	Hook(const shared_ptr<Location> &location);
	virtual bool Shoot(const Vec2f &direction) override final;
	bool IsHooked() const;
	void ChangeChainLength(float howMuch);
	void ResetHook();
};
