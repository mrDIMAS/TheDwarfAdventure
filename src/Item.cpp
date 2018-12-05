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
#include "Item.h"
#include "Level.h"
#include "Renderer.h"
#include "ParticleSystem.h"
#include "Light.h"
#include "Sprite.h"
#include "Label.h"
#include "Location.h"

Item::Item() : mType("Unknown"), mDurability(0.0f)
{
}

Item::Item(const shared_ptr<Location> &location, const ItemType &type) : Item()
{
	mLocation = location;
	THROW_IF_NULL(location);
	const auto scene = location->GetScene();
	mType = type;
	const ItemDefinition& desc = game->GetItemDatabase()->Get(type);
	mDurability = desc.mDurability;
	if (!mNode)
	{
		if (desc.mClass == ItemClass::Weapon)
		{
			mNode = scene->CreateNode();
		}
		else
		{
			if (desc.mTexture)
			{
				auto s = scene->CreateSprite();
				float aspect = desc.mRect.width / desc.mRect.height;
				s->SetSize({ 13, 13 / aspect });
				s->SetTexture(desc.mTexture);
				s->SetOriginInCenter();
				auto t = s->GetTexture();
				s->SetTextureRectangle({ desc.mRect.left / t->GetWidth(), desc.mRect.top / t->GetHeight(),
					desc.mRect.width / t->GetWidth(), desc.mRect.height / t->GetHeight() });
				mNode = s;
			}
			else
			{
				auto sprite = scene->CreateSprite();
				sprite->SetSize({ 5, 5 });
				sprite->SetColor(Math::RndOpaqueColor(100, 255));
				sprite->SetOriginInCenter();
				mNode = sprite;
			}
		}
	}

	if (desc.mHasPhysicalBody)
	{
		if (mNode)
		{
			auto size = mNode->GetSize();
			auto avRadius = sqrt(size.x * size.x + size.y * size.y) / 2;
			auto body = location->GetScene()->GetSimulationIsland()->CreateParticle(avRadius);
			body->SetFriction(0.9f);
			mNode->SetBody(body);
		}
	}
}

Item::~Item()
{
	DBG_ECHO();
}

ItemType Item::GetType() const
{
	return mType;
}

shared_ptr<SceneNode> Item::GetNode() const
{
	return mNode;
}

void Item::Serialize(Serializer &sav)
{
	sav &mDurability;
	sav &mType;
	sav &mNode;
	sav &mModifierList;
	sav & mLocation;
}

void Item::Update()
{

}

void Item::SetPosition(const Vec2f &pos)
{
	mNode->SetLocalPosition(pos);
}

shared_ptr<Texture> Item::GetPictogram()
{
	if (auto s = dynamic_pointer_cast<Sprite>(mNode))
	{
		return s->GetTexture();
	}
	return nullptr;
}

sf::FloatRect Item::GetPictogramTexCoords() const
{
	if (auto s = dynamic_pointer_cast<Sprite>(mNode))
	{
		return s->GetTextureRectangle();
	}
	return { 0, 0, 1, 1 };
}

const ItemDefinition &Item::GetDefinition() const
{
	return game->GetItemDatabase()->Get(mType);
}

void Item::WearOut(float howMuch)
{
	mDurability -= howMuch;
	if (mDurability < 0)
	{
		mDurability = 0;
	}
}

bool Item::IsBroken() const
{
	return mDurability < 0.001f;
}

bool Item::IsBrandNew() const
{
	return mDurability == GetDefinition().mDurability;
}

float Item::GetCurrentDurability() const
{
	return mDurability;
}

float Item::GetDamageLevel() const
{
	return 100.0f * (1.0f - mDurability / GetDefinition().mDurability);
}

void Item::Repair()
{
	mDurability = GetDefinition().mDurability;
}

const map<ItemModifierType, float> &Item::GetModifierList() const
{
	return mModifierList;
}

float Item::GetModifiedValue(ItemModifierType type) const
{
	const auto &desc = GetDefinition();

	// select non-modified value
	float value = 0;
	switch (type)
	{
	case ItemModifierType::ChemicalDamageReduction:
		value = desc.mChemicalDamageReduction;
		break;
	case ItemModifierType::Damage:
		value = desc.mDamage;
		break;
	case ItemModifierType::Durability:
		value = desc.mDurability;
		break;
	case ItemModifierType::FireDamageReduction:
		value = desc.mFireDamageReduction;
		break;
	case ItemModifierType::PhysicalDamageReduction:
		value = desc.mPhysicalDamageReduction;
		break;
	case ItemModifierType::ShootDistance:
		value = desc.mShootDistance;
		break;
	case ItemModifierType::ShootPeriod:
		value = static_cast<float>(desc.mShootPeriod);
		break;
	case ItemModifierType::Stability:
		value = desc.mStability;
		break;
	default:
		value = 0;
	}

	// add modifiers
	auto iter = mModifierList.find(type);
	if (iter != mModifierList.end())
	{
		value *= 1.0f + iter->second;
	}

	return value;
}

float Item::GetModifier(ItemModifierType type) const
{
	auto iter = mModifierList.find(type);
	if (iter != mModifierList.end())
	{
		return iter->second;
	}
	return 0.0f;
}

void Item::ModifyValue(ItemModifierType type, float value)
{
	auto iter = mModifierList.find(type);
	if (iter == mModifierList.end())
	{
		mModifierList.insert(make_pair(type, value));
	}
	else
	{
		iter->second += value;
	}
}

void Item::EnlistModifications(StringBuilder &fmt)
{
	float damageMod = GetModifier(ItemModifierType::Damage);
	float chemDmgRedMod = GetModifier(ItemModifierType::ChemicalDamageReduction);
	float physDmgRedMod = GetModifier(ItemModifierType::PhysicalDamageReduction);
	float fireDmgRedMod = GetModifier(ItemModifierType::FireDamageReduction);
	float durabilityMod = GetModifier(ItemModifierType::Durability);
	float shootDistMod = GetModifier(ItemModifierType::ShootDistance);
	float shootPeriodMod = GetModifier(ItemModifierType::ShootPeriod);
	float stabilityMod = GetModifier(ItemModifierType::Stability);

	// print installed modifications
	fmt << ColorToStr({ 0, 162, 232 }) << "*** " << game->GetStr("installedModifications") << " ***"
		<< ColorToStr({ 255, 255, 255 }) << LineEnd;
	if (damageMod != 0.0f)
	{
		fmt << game->GetStr("modDamage") << ": +" << RoundAndConvert(damageMod * 100.0f, 1) << "%"
			<< LineEnd;
	}
	if (chemDmgRedMod != 0.0f)
	{
		fmt << game->GetStr("modChemicalDamageReduction") << ": +"
			<< RoundAndConvert(chemDmgRedMod * 100.0f, 1) << "%" << LineEnd;
	}
	if (physDmgRedMod != 0.0f)
	{
		fmt << game->GetStr("modPhysicalDamageReduction") << ": +"
			<< RoundAndConvert(physDmgRedMod * 100.0f, 1) << "%" << LineEnd;
	}
	if (fireDmgRedMod != 0.0f)
	{
		fmt << game->GetStr("modFireDamageReduction") << ": +"
			<< RoundAndConvert(fireDmgRedMod * 100.0f, 1) << "%" << LineEnd;
	}
	if (durabilityMod != 0.0f)
	{
		fmt << game->GetStr("modDurability") << ": +" << RoundAndConvert(durabilityMod * 100.0f, 1)
			<< "%" << LineEnd;
	}
	if (shootDistMod != 0.0f)
	{
		fmt << game->GetStr("modShootDistance") << ": +" << RoundAndConvert(shootDistMod * 100.0f, 1)
			<< "%" << LineEnd;
	}
	if (shootPeriodMod != 0.0f)
	{
		fmt << game->GetStr("modShootPeriod") << ": +" << RoundAndConvert(shootPeriodMod * 100.0f, 1)
			<< "%" << LineEnd;
	}
	if (stabilityMod != 0.0f)
	{
		fmt << game->GetStr("modStability") << ": +" << RoundAndConvert(stabilityMod * 100.0f, 1) << "%"
			<< LineEnd;
	}
}

bool Item::IsModified() const
{
	return mModifierList.size() > 0;
}

void Item::SetLocation(const shared_ptr<Location> & location)
{
	mLocation = location;
	location->GetScene()->AcceptNode(mNode);
}

shared_ptr<Location> Item::GetLocation() const
{
	return mLocation.lock();
}

// called when putting item into inventory

void Item::OnPut()
{
	if (auto body = mNode->GetBody())
	{
		body->ResetFlags(BF_ENABLED);
	}
}

// called when item extracted from inventory

void Item::OnPullOut()
{
	if (auto body = mNode->GetBody())
	{
		body->SetFlags(BF_ENABLED);
	}
}
