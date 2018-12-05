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
#include "Hook.h"
#include "Level.h"
#include "Renderer.h"
#include "Input.h"
#include "Actor.h"
#include "PolyLine.h"
#include "Terrain.h"
#include "Stats.h"
#include "VisualEffect.h"
#include "ResourceDatabase.h"

void Hook::OnSerialize(Serializer &sav)
{
	sav &mHook;
	sav &mHookDestType;
	sav &mChainModel;
	sav &mChain;
	sav &mHookThrown;
}

void Hook::OnSetVisible(bool vis)
{
	mHook->SetVisible(vis);

	if (!vis)
	{
		mChainModel->SetVisible(false);
		ResetHook();
	}
}

void Hook::OnUpdate()
{
	THROW_IF_EXPIRED(mOwner);
	auto input = game->GetInput();

	const bool hookedToSomething = mHookDestType != HookDestType::None;
	mChainModel->SetVisible(hookedToSomething || mHookThrown);
	if (!mHookThrown && hookedToSomething && input->IsMouseButtonHit(sf::Mouse::Right))
	{
		ResetHook();
	}

	mChainModel->Clear();
	for (auto &p : mChain->mBodies)
	{
		mChainModel->AddPoint(p->GetPosition());
	}

	if (!mHookThrown && hookedToSomething)
	{
		if (mHookDestType == HookDestType::Terrain)
		{
			mChain->mBodies.front()->SetPosition(mHook->GetGlobalPosition());
		}
		if (mHookDestType == HookDestType::Actor)
		{
			mChain->mBodies.back()->SetPosition(GetShootPointGlobal());
		}
	}
	
	if (mHookThrown)
	{
		mChain->mBodies.back()->SetPosition(GetShootPointGlobal());
		mHook->SetLocalPosition(mChain->mBodies.front()->GetPosition());
		mChainScale -= 0.007f;
		if (mChainScale <= 0)
		{
			mHookThrown = false;
			ResetHook();
		}
		else
		{
			mChain->Scale(mChainScale);
		}
	}
}

void Hook::ResetHook()
{
	if (mHookDestType == HookDestType::Actor)
	{
		// detach actor body from chain
		mChain->RemoveFirstLink();
	}
	if (mHookDestType == HookDestType::Terrain)
	{
		// detach owner body from chain
		mChain->RemoveLastLink();
	}

	mHook->AttachTo(mModel);
	mHook->SetLocalPosition(mHook->GetSize().x * 1.55f, mModel->GetSize().y / 2);
	mHook->SetLocalRotation(0);
	mChain->SetEnabled(false);

	mHookDestType = HookDestType::None;
}

void Hook::StaticConnectTo(const shared_ptr<Body> &ownerBody, const Vec2f &from, const Vec2f &to)
{
	const float actorLinkLength = 37; // now its hardcoded, fix this
	const Vec2f dir = to - from;
	mHookDestType = HookDestType::Terrain;
	// align and position hook model
	mHook->Detach();
	mHook->SetLocalPosition(to);
	mHook->SetLocalRotation(Math::CartesianToPolar(dir).mAngle);
	// attach owner body to chain and align chain
	mChain->AddBodyToEnd(ownerBody, actorLinkLength);
	mChain->Align(to, ownerBody->GetPosition());
}

void Hook::DynamicConnectTo(const shared_ptr<Body> &ownerBody, const Vec2f &from,
	const shared_ptr<Actor> &targetActor, const Vec2f &connectPoint)
{
	const float actorLinkLength = 37; // now its hardcoded, fix this
	const Vec2f dir = targetActor->GetPosition() - from;
	mHookDestType = HookDestType::Actor;
	// align and position hook model
	mHook->AttachTo(targetActor->GetPivot());
	mHook->SetLocalPosition(connectPoint);
	mHook->SetLocalRotation(Math::CartesianToPolar(dir).mAngle);
	// attach owner body to chain and align chain
	mChain->AddBodyToBegin(targetActor->GetBody(), actorLinkLength);
	mChain->Align(targetActor->GetPosition(), ownerBody->GetPosition());
}

void Hook::ThrowHook(const Vec2f & direction, const Vec2f & destPosition)
{
	auto owner = mOwner.lock();
	mHook->Detach();
	mHook->SetLocalRotation(Math::CartesianToPolar(direction).mAngle);
	mHookThrown = true;
	mChainScale = 1.0f;
	mChain->Scale(mChainScale);
	mChain->SetEnabled(true);
	mChain->Align(destPosition, owner->GetPosition());
}

Hook::Hook()
{
}

Hook::Hook(const shared_ptr<Location> &location) : Weapon(location, ItemType::Weapon_Hook)
{
	const auto scene = location->GetScene();

	auto wpnTemplate = game->GetResourceDatabase()->GetRandom(WeaponModelTemplateType::Hook);
	auto hookPart = wpnTemplate->mParts[WeaponPartTemplateType::Hook];
	auto modelPart = wpnTemplate->mParts[WeaponPartTemplateType::Model];

	mHook = scene->CreateSprite();
	mHook->SetTexture(hookPart.mTexture);
	mHook->SetSize(hookPart.mSize);
	mHook->SetLocalOrigin(hookPart.mOrigin);

	auto hookEnd = scene->CreateNode();
	hookEnd->SetLocalPosition(0, 3.5f);
	hookEnd->AttachTo(mHook);

	mModel = scene->CreateSprite();
	mModel->AttachTo(mNode);
	mModel->SetTexture(modelPart.mTexture);
	mModel->SetSize(modelPart.mSize);
	mModel->SetLocalOrigin(modelPart.mOrigin);

	mHook->AttachTo(mModel);

	mShootPoint = { mModel->GetSize().x, mModel->GetLocalOrigin().y };

	mChainModel = scene->CreatePolyLine();
	mChainModel->SetVisible(false);
	mChainModel->SetThickness(5);
	mChainModel->SetColor({ 223, 184, 0 });

	mChain = scene->GetSimulationIsland()->CreateChain(5, mMaxRopeLength);

	ResetHook();
}

bool Hook::Shoot(const Vec2f &direction)
{
	THROW_IF_EXPIRED(mOwner);
	
	auto owner = mOwner.lock();

	// Unable to shoot while hook is outside of gun
	if (mHookThrown)
	{
		return true;
	}

	// Not enough stamina to shoot
	if (!owner->GetStats()->ConsumeStamina(30))
	{
		return true;
	}

	auto location = owner->GetLocation();
	auto scene = location->GetScene();
	auto terrain = location->GetScene()->GetTerrain();

	scene->AcceptNode(mHook);
	scene->AcceptNode(mChainModel);
	scene->GetSimulationIsland()->AcceptChain(mChain);

	if (mHookDestType == HookDestType::None && mShootTimeout <= 0)
	{
		const auto origin = GetShootPointGlobal();
		const auto rayDir = direction * mMaxRopeLength;

		// pick actor by ray
		Vec2f intPos;
		float actorSqrDistance;
		shared_ptr<Actor> closestActor = PickClosestActorByRay(origin, rayDir, &intPos, &actorSqrDistance);

		// and trace ray with terrain
		auto result = terrain->TraceRay(origin, rayDir);

		bool actorIsCloser = closestActor ? Math::Distance(origin, intPos) < Math::Distance(origin, result.mPosition) : false;

		if (actorIsCloser)
		{
			auto &desc = GetDefinition();

			Damage dmg;
			dmg.mWho = owner;
			dmg.mCount = GetModifiedValue(ItemModifierType::Damage);
			dmg.mClass = desc.mDamageClass;
			dmg.mFlags = (DamageType)desc.mDamageType;
			closestActor->ApplyDamage(dmg);

			if (closestActor->IsDead())
			{
				ThrowHook(rayDir, intPos);
			}
			else
			{
				// hook actor if alive after hook shot
				const Vec2f connectPtLocal = intPos - closestActor->GetPosition();
				DynamicConnectTo(owner->GetBody(), origin, closestActor, connectPtLocal);
			}
			mShootTimeout = GetDefinition().mShootPeriod;
			mChain->SetEnabled(true);
			scene->EmitSound("data/sounds/flesh_impact1.ogg", intPos);
		}
		else
		{
			if (result.mIntersection)
			{
				if (result.mPixel.a & PA_METAL)
				{
					VisualEffect::MakeSparks(scene, result.mPosition);
					ThrowHook(rayDir, result.mPosition);
					scene->EmitSound("data/sounds/metal_impact1.ogg", result.mPosition);
				}
				else
				{
					StaticConnectTo(owner->GetBody(), origin, result.mPosition);
					mShootTimeout = GetDefinition().mShootPeriod;
					mChain->SetEnabled(true);
					scene->EmitSound("data/sounds/pickaxe_impact3.ogg", result.mPosition);
				}
			}
			else
			{
				ThrowHook(rayDir, owner->GetPosition() + rayDir);
			}
		}
	}

	return true;
}

bool Hook::IsHooked() const
{
	return mHookDestType != HookDestType::None;
}

void Hook::ChangeChainLength(float howMuch)
{
	if (mChain->GetScale() > 0.05f && howMuch < 0)
	{
		mChain->Scale(mChain->GetScale() + howMuch);
	}
	if (mChain->GetScale() < 1.0f && howMuch > 0)
	{
		mChain->Scale(mChain->GetScale() + howMuch);
	}
}
