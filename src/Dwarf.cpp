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

#include "Precompiled.h"
#include "Dwarf.h"
#include "Animation.h"
#include "AnimationTrack.h"
#include "Level.h"
#include "Item.h"
#include "Weapon.h"
#include "Renderer.h"
#include "Stats.h"

void Dwarf::LoadAndPrepareModel()
{
	auto location = mLocation.lock();
	auto &scene = location->GetScene();

	// Load prefab and reset some render flags
	auto prefab = game->LoadPrefab("data/prefabs/character.prefab");

	// Find model root
	mModelRoot = prefab->FindStrict<SceneNode>("Root");
	mModelRoot->AttachTo(mPivot);
	mModelRoot->SetLocalPosition({ 0, mPivot->GetBody()->GetRadius() });

	// Lind body parts
	mLeftUpperHand = prefab->FindStrict<Sprite>("LeftUpperArm");
	mLeftLowerHand = prefab->FindStrict<Sprite>("LeftLowerArm");
	mRightLowerHand = prefab->FindStrict<Sprite>("RightLowerArm");
	mRightUpperHand = prefab->FindStrict<Sprite>("RightUpperArm");
	mLeftUpperLeg = prefab->FindStrict<Sprite>("LeftUpperLeg");
	mRightUpperLeg = prefab->FindStrict<Sprite>("RightUpperLeg");
	mLeftLowerLeg = prefab->FindStrict<Sprite>("LeftLowerLeg");
	mRightLowerLeg = prefab->FindStrict<Sprite>("RightLowerLeg");
	mHead = prefab->FindStrict<Sprite>("Head");
	mHair = prefab->FindStrict<Sprite>("Hair");
	mBeard = prefab->FindStrict<Sprite>("Beard");
	mBody = prefab->FindStrict<Sprite>("Body");

	mPreviousHairTexture = mHair->GetTexture();
	mPreviousHairTextureRect = mHair->GetTextureRectangle();

	// create weapon mount point on the hand
	mWeaponMountPoints[0] = scene->CreateNode();
	mWeaponMountPoints[0]->SetLocalPosition(0, mLeftLowerHand->GetSize().y);
	mWeaponMountPoints[0]->AttachTo(mLeftLowerHand);

	mWeaponMountPoints[1] = scene->CreateNode();
	mWeaponMountPoints[1]->SetLocalPosition(0, mRightLowerHand->GetSize().y);
	mWeaponMountPoints[1]->AttachTo(mRightLowerHand);

	// copy animations
	mWalkAnimation = prefab->FindAnimationStrict("Walk");
	mJumpAnimation = prefab->FindAnimationStrict("JumpBegin");
	mStandAnimation = prefab->FindAnimationStrict("Stand");
	mStunLockAnimation = prefab->FindAnimationStrict("StunLock");
	mStunLockAnimation->SetLooped(false);

	mAnimations = prefab->GetAnimationList();
		
	scene->Merge(prefab);

	mFootStepSoundList.push_back("data/sounds/footsteps/step1.ogg");
	mFootStepSoundList.push_back("data/sounds/footsteps/step2.ogg");
	mFootStepSoundList.push_back("data/sounds/footsteps/step3.ogg");
	mFootStepSoundList.push_back("data/sounds/footsteps/step4.ogg");

	mWalkAnimation->AddTimeEvent(TimeEvent(0.2f, [this] { EmitStepSound(); }));
	mWalkAnimation->AddTimeEvent(TimeEvent(0.6f, [this] { EmitStepSound(); }));
}

void Dwarf::AnimateHandsAndHeadAndUpdateWeapons(const Vec2f &aimDirection, bool xFlip)
{
	const float aimAngle = Math::CartesianToPolar(aimDirection).mAngle;
	// override left hand animation when carry weapon
	int handIndex = 0;
	for (auto & handEquipment : GetHandSlots())
	{
		auto upperHand = handIndex == 0 ? mLeftUpperHand : mRightUpperHand;
		auto lowerHand = handIndex == 0 ? mLeftLowerHand : mRightLowerHand;
		if (auto weapon = dynamic_pointer_cast<Weapon>(handEquipment))
		{
			// Update Weapon
			weapon->Update();
			weapon->GetNode()->AttachTo(mWeaponMountPoints[handIndex]);
			weapon->GetNode()->SetLocalPosition(0, 0);
			weapon->GetNode()->SetLocalRotation(90);


			// Animate hand 
			if (weapon->GetDefinition().mIsMeleeWeapon)
			{
				SetAnimationEnabledForNode(upperHand, false);
				upperHand->SetLocalRotation((xFlip ? -1 : 1) * (aimAngle - 90));
				if (mMeleeAttackState[handIndex] == MeleeAttackState::Swing)
				{
					if (lowerHand->GetLocalRotation() > -160)
					{
						lowerHand->SetLocalRotation(lowerHand->GetLocalRotation() - mStats->GetMeleeWeaponSwingBaseSpeed());
					}
					else
					{
						mMeleeAttackState[handIndex] = MeleeAttackState::Hit;
					}
				}
				else if (mMeleeAttackState[handIndex] == MeleeAttackState::Hit)
				{
					if (lowerHand->GetLocalRotation() < 0)
					{

						if (weapon->GetType() == ItemType::Weapon_Pickaxe)
						{
							if (!weapon->Shoot(aimDirection))
							{
								lowerHand->SetLocalRotation(0);
								mMeleeAttackState[handIndex] = MeleeAttackState::Ready;
							}
						}
						lowerHand->SetLocalRotation(lowerHand->GetLocalRotation() + 25);
						if (lowerHand->GetLocalRotation() >= 0)
						{
							if (weapon->GetType() != ItemType::Weapon_Pickaxe)
							{
								weapon->Shoot(aimDirection);
							}
							lowerHand->SetLocalRotation(0);
							mMeleeAttackState[handIndex] = MeleeAttackState::Ready;
						}
					}
				}
			}
			else
			{
				SetAnimationEnabledForNode(upperHand, false, false);
				SetAnimationEnabledForNode(lowerHand, false, false);
				upperHand->SetLocalRotation(0);
				lowerHand->SetLocalRotation((xFlip ? -1 : 1) * (aimAngle - 90));
			}
		}
		else
		{
			SetAnimationEnabledForNode(upperHand, true);
		}
		++handIndex;
	}

	// head animated manually, so animation for it is disabled
	SetAnimationEnabledForNode(mHead, false);
	const float headAngle = xFlip ? Math::SimplifyAngle(180 - aimAngle) : aimAngle;
	mHead->SetLocalRotation(Math::Clamp(headAngle, -20.0f, 20.0f));
}

shared_ptr<Sprite> Dwarf::SelectBodyPart(const DwarfBodyPart &dbp)
{
	switch (dbp)
	{
	case DwarfBodyPart::Body: return mBody;
	case DwarfBodyPart::LeftLowerArm: return mLeftLowerHand;
	case DwarfBodyPart::RightLowerArm: return mRightLowerHand;
	case DwarfBodyPart::LeftUpperArm: return mLeftUpperHand;
	case DwarfBodyPart::RightUpperArm: return mRightUpperHand;
	case DwarfBodyPart::LeftUpperLeg: return mLeftUpperLeg;
	case DwarfBodyPart::RightUpperLeg: return mRightUpperLeg;
	case DwarfBodyPart::LeftLowerLeg: return mLeftLowerLeg;
	case DwarfBodyPart::RightLowerLeg: return mRightLowerLeg;
	case DwarfBodyPart::Hair: return mHair;
    case DwarfBodyPart::Head: return mHead;
    case DwarfBodyPart::Beard: return mBeard;
    case DwarfBodyPart::Unknown: break;
	}
	return nullptr;
}

void Dwarf::EmitStepSound()
{
	mLocation.lock()->GetScene()->EmitSound(Math::RandomElement(mFootStepSoundList), mPivot);
}

Dwarf::Dwarf()
{
}

Dwarf::Dwarf(const shared_ptr<Location> &location) : Actor(location, 20)
{
	LoadAndPrepareModel();
}

Dwarf::~Dwarf()
{
}

void Dwarf::Serialize(Serializer &sav)
{
	Actor::Serialize(sav);
	sav &mLeftUpperHand;
	sav &mLeftLowerHand;
	for (auto & wpnMntPt : mWeaponMountPoints)
	{
		sav &wpnMntPt;
	}
	sav &mHead;
	sav &mHair;
	sav &mBeard;
	sav &mWalkAnimation;
	sav &mStandAnimation;
	sav &mJumpAnimation;
	sav &mStunLockAnimation;
	for (auto & mas : mMeleeAttackState)
	{
		sav &mas;
	}
	sav &mBody;
	sav &mRightLowerHand;
	sav &mLeftUpperHand;
	sav &mRightUpperHand;
	sav &mLeftUpperLeg;
	sav &mRightUpperLeg;
	sav &mLeftLowerLeg;
	sav &mRightLowerLeg;
	sav &mPreviousHairTexture;
	sav &mPreviousHairTextureRect;
	sav &mFootStepSoundList;
}

void Dwarf::UseItem(const shared_ptr<Item> &item, const Hand & hand)
{
	Actor::UseItem(item, hand);

	const auto &desc = item->GetDefinition();
	for (const auto bpd : desc.mBodyPartAffectors)
	{
		if (shared_ptr<Sprite> bodyPart = SelectBodyPart(bpd.mType))
		{
			bodyPart->SetTexture(game->GetRenderer()->RequestTexture(bpd.mTexturePath));
			bodyPart->SetTextureRectangle(bpd.mRect);
		}
	}
}

void Dwarf::UnequipItem(const shared_ptr<Item> &item)
{
	if (!item)
	{
		return;
	}

	const auto &desc = item->GetDefinition();
	const auto &renderer = game->GetRenderer();

	if (desc.mClass == ItemClass::Armor)
	{
		mBody->SetTexture(renderer->RequestTexture("data/textures/body_parts/body_generic.png"));
	}
	if (desc.mClass == ItemClass::Helmet)
	{
		mHair->SetTexture(mPreviousHairTexture);
		mHair->SetTextureRectangle(mPreviousHairTextureRect);
	}
	if (desc.mClass == ItemClass::Trousers)
	{
		mLeftUpperLeg->SetTexture(renderer->RequestTexture("data/textures/body_parts/upperleg_generic.png"));
		mRightUpperLeg->SetTexture(renderer->RequestTexture("data/textures/body_parts/upperleg_generic.png"));
		mLeftLowerLeg->SetTexture(renderer->RequestTexture("data/textures/body_parts/lowerleg_generic.png"));
		mRightLowerLeg->SetTexture(renderer->RequestTexture("data/textures/body_parts/lowerleg_generic.png"));
	}
	if (desc.mClass == ItemClass::Gloves)
	{
		mLeftUpperHand->SetTexture(renderer->RequestTexture("data/textures/body_parts/upperarm_generic.png"));
		mRightUpperHand->SetTexture(renderer->RequestTexture("data/textures/body_parts/upperarm_generic.png"));
		mLeftLowerHand->SetTexture(renderer->RequestTexture("data/textures/body_parts/lowerarm_generic.png"));
		mRightLowerHand->SetTexture(renderer->RequestTexture("data/textures/body_parts/lowerarm_generic.png"));
	}

	Actor::UnequipItem(item);
}

bool Dwarf::CanEquip(const shared_ptr<Item> & item) const
{
	UNUSED(item);

	return true;
}

