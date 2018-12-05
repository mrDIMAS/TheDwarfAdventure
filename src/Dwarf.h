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

#include "Actor.h"

enum class MeleeAttackState
{
	Ready,
	Swing,
	Hit,
};

// represent generic dwarf character with animated body based on prefab
class Dwarf : public Actor
{
private:
	vector<string> mFootStepSoundList;
protected:
	array<shared_ptr<SceneNode>, 2> mWeaponMountPoints;
	shared_ptr<Sprite> mLeftUpperHand;
	shared_ptr<Sprite> mLeftLowerHand;
	shared_ptr<Sprite> mRightLowerHand;
	shared_ptr<Sprite> mRightUpperHand;
	shared_ptr<Sprite> mLeftUpperLeg;
	shared_ptr<Sprite> mRightUpperLeg;
	shared_ptr<Sprite> mLeftLowerLeg;
	shared_ptr<Sprite> mRightLowerLeg;
	shared_ptr<Sprite> mHead;
	shared_ptr<Sprite> mHair;
	shared_ptr<Sprite> mBeard;
	shared_ptr<Sprite> mBody;
	

	// When dwarf puts on helmet, texture of its hair replaced with texture of helmet
	// We have to store this texture to return it back when dwatf puts off helmet
	shared_ptr<Texture> mPreviousHairTexture;
	sf::FloatRect mPreviousHairTextureRect;

	shared_ptr<Animation> mWalkAnimation;
	shared_ptr<Animation> mStandAnimation;
	shared_ptr<Animation> mJumpAnimation;
	shared_ptr<Animation> mStunLockAnimation;

	// Attack state of each hand
	array<MeleeAttackState, 2> mMeleeAttackState { MeleeAttackState::Ready };

	void LoadAndPrepareModel();
	// aims weapon through rotation of left lower hand
	// also performs melee attacks when appropriate MeleeAttackState is set
	void AnimateHandsAndHeadAndUpdateWeapons(const Vec2f &aimDirection, bool xFlip);
	shared_ptr<Sprite> SelectBodyPart(const DwarfBodyPart & dbp);

	void EmitStepSound();
public:
	Dwarf();
	Dwarf(const shared_ptr<Location> &location);
	virtual ~Dwarf();
	virtual void Serialize(Serializer &sav) override;
	virtual void UseItem(const shared_ptr<Item> &item, const Hand & hand) override;
	virtual void UnequipItem(const shared_ptr<Item> &item) override;
	bool CanEquip(const shared_ptr<Item> & item) const;
};
