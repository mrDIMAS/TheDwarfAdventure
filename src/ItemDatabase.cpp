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
#include "ItemDatabase.h"
#include "Game.h"
#include "Scene.h"
#include "Weapon.h"
#include "Drill.h"
#include "PlasmaDrill.h"
#include "Hook.h"
#include "Shotgun.h"
#include "SoilGun.h"
#include "RocketLauncher.h"
#include "GrenadeLauncher.h"
#include "Shield.h"
#include "RailGun.h"
#include "Harpoon.h"
#include "RevealGun.h"
#include "AcidGun.h"
#include "Pickaxe.h"
#include "GravityGun.h"
#include "ForceGun.h"
#include "LightningGun.h"
#include "Torch.h"
#include "ConstructionGun.h"
#include "Renderer.h"
#include "JsonConverters.h"

void ItemDatabase::ParseDatabase()
{
	std::ifstream file(Path);
	nlohmann::json root;
	file >> root;

	for (auto entryIter = root.begin(); entryIter != root.end(); ++entryIter)
	{
		ItemDefinition entry;
		entry.mType = ItemType(entryIter.key());

		nlohmann::json& entryJson = entryIter.value();

		for (auto prop = entryJson.begin(); prop != entryJson.end(); ++prop)
		{
			const string name = prop.key();
			nlohmann::json value = prop.value();

			if (value.is_null())
			{
				continue;
			}
									
			if (name == ClassTag)
			{
				entry.mClass = ItemClass(value.get<string>());
			}
			else if (name == NameTag)
			{
				entry.mNameKey = value.get<string>();
			}
			else if (name == TextureTag)
			{
				entry.mTexture = game->GetRenderer()->RequestTexture(value.get<string>());
			}
			else if (name == DescriptionTag)
			{
				entry.mDescriptionKey = value.get<string>();
			}
			else if (name == BookTextTag)
			{
				entry.mBookTextKey = value.get<string>();
			}
			else if (name == BodyPartAffectorsTag)
			{
				for (auto bodyPartIter = value.begin(); bodyPartIter != value.end(); ++bodyPartIter)
				{
					nlohmann::json & bodyPartValue = bodyPartIter.value();

					BodyPartDefinition bpaDesc;
					bpaDesc.mTexturePath = bodyPartValue[BodyPartAffectorTextureTag].get<string>();
					auto rect = bodyPartValue[BodyPartAffectorRectTag];
					if (!rect.is_null())
					{
						bpaDesc.mRect = rect.get<sf::FloatRect>();
					}
					else
					{
						bpaDesc.mRect = { 0, 0, 1, 1 };
					}
					bpaDesc.mType = DwarfBodyPart(bodyPartIter.key());

					entry.mBodyPartAffectors.push_back(bpaDesc);
				}				
			}
			else if (name == TextureRectTag)
			{
				entry.mRect = value.get<sf::FloatRect>();
			}
			else if (name == PriceTag)
			{
				entry.mPrice = value.get<int>();
			}
			else if (name == WeightTag)
			{
				entry.mWeight = value.get<float>();
			}
			else if (name == DurabilityTag)
			{
				entry.mDurability = value.get<float>();
			}
			else if (name == DamageTag)
			{
				entry.mDamage = value[DamageCountTag].get<float>();
				entry.mDamageClass = DamageClass(value[DamageClassTag].get<string>());
				entry.mDamageType = value[DamageFlagsTag].get<int>();
			}
			else if (name == PhysDmgReductionPercentTag)
			{
				entry.mPhysicalDamageReduction = Math::Clamp(value.get<float>() / 100.0f, 0.0f, 1.0f);
			}
			else if (name == ChemDmgReductionPercentTag)
			{
				entry.mChemicalDamageReduction = Math::Clamp(value.get<float>() / 100.0f, 0.0f, 1.0f);
			}
			else if (name == FireDmgReductionPercentTag)
			{
				entry.mFireDamageReduction = Math::Clamp(value.get<float>() / 100.0f, 0.0f, 1.0f);
			}
			else if (name == ElecDmgReductionPercentTag)
			{
				entry.mElectricalDamageReduction = Math::Clamp(value.get<float>() / 100.0f, 0.0f, 1.0f);
			}
			else if (name == HealTag)
			{
				entry.mHeal = abs(value.get<float>());
			}
			else if (name == HealFramesTag)
			{
				entry.mHealFrames = abs(value.get<int>());
			}
			else if (name == StaminaRestorationTag)
			{
				entry.mStaminaRestoration = abs(value.get<float>());
			}
			else if (name == StaminaRestorationFramesTag)
			{
				entry.mStaminaRestorationFrames = abs(value.get<int>());
			}
			else if (name == ConsumableTag)
			{
				entry.mConsumable = ItemType(value.get<string>());
			}
			else if (name == BlueprintResultTag)
			{
				entry.mBlueprintResultItem = ItemType(value.get<string>());
			}
			else if (name == StabilityTag)
			{
				entry.mStability = Math::Clamp(value.get<float>() / 100.0f, 0.0f, 1.0f);
			}
			else if (name == ComposeItemsTag)
			{
				for (auto itemIter = value.begin(); itemIter != value.end(); ++itemIter)
				{
					ItemType type = ItemType(itemIter.key());

					int count = itemIter.value().get<int>();

					for (int i = 0; i < count; ++i)
					{
						entry.mComposeItems.push_back(type);
					}
				}
			}
			else if (name == DecomposeItemsTag)
			{
				for (auto itemIter = value.begin(); itemIter != value.end(); ++itemIter)
				{
					ItemType type = ItemType(itemIter.key());

					int count = itemIter.value().get<int>();

					for (int i = 0; i < count; ++i)
					{
						entry.mDecomposeItems.push_back(type);
					}
				}				
			}
			else if (name == RepairItemsTag)
			{
				for (auto damageAmountIter = value.begin(); damageAmountIter != value.end(); ++damageAmountIter)
				{
					ItemDamageClass dmgClassType = ItemDamageClass(damageAmountIter.key());
					nlohmann::json damageAmountJson = damageAmountIter.value();

					for (auto itemIter = damageAmountJson.begin(); itemIter != damageAmountJson.end(); ++itemIter)
					{
						ItemType itemType = ItemType(itemIter.key());

						int count = itemIter.value().get<int>();

						for (int i = 0; i < count; ++i)
						{
							entry.mRepairItems[dmgClassType].push_back(itemType);
						}
					}
				}
			}
			else if (name == ShootPeriodTag)
			{
				entry.mShootPeriod = abs(value.get<int>());
			}
			else if (name == HasPhysicalBodyTag)
			{
				entry.mHasPhysicalBody = value.get<bool>();
			}
			else if (name == ShootDistanceTag)
			{
				entry.mShootDistance = abs(value.get<float>());
			}
			else if (name == RequiredIntelligenceForCraftTag)
			{
				entry.mRequiredIntelligence = abs(value.get<int>());
			}
			else if (name == RequiredDexterityForUseTag)
			{
				entry.mRequiredDexterityForUse = abs(value.get<int>());
			}
			else if (name == RequiredStrengthForUseTag)
			{
				entry.mRequiredStrengthForUse = abs(value.get<int>());
			}
			else if (name == RequiredIntelligenceForUseTag)
			{
				entry.mRequiredIntelligenceForUse = abs(value.get<int>());
			}
			else if (name == IsMeleeTag)
			{
				entry.mIsMeleeWeapon = value.get<bool>();
			}
			else if (name == ThrowableTag)
			{
				entry.mIsThrowableProjectile = value.get<bool>();
			}
			else if (name == RarenessTag)
			{
				entry.mRareness = static_cast<unsigned char>(Math::Clamp(value.get<int>(), 0, 100));
			}
			else
			{
				throw Exception((string)"Illegal property defined - " + name);
			}
		}

		mBase[entry.mType] = entry;
	}
}

ItemDatabase::ItemDatabase()
{
	ParseDatabase();

	Log() << "Item database successfully loaded!" << LineEnd;
}

ItemDatabase::~ItemDatabase()
{
	DBG_ECHO();
}

const ItemDefinition &ItemDatabase::Get(const ItemType &type) const
{
	auto desc = mBase.find(type);
	if (desc == mBase.end())
	{
		throw Exception(StringBuilder("Item type ") << type << " is not found in database!");
	}
	return desc->second;
}

ItemDatabase::DefinitionCollection& ItemDatabase::GetRecords()
{
	return mBase;
}

shared_ptr<Item> ItemDatabase::Make(const ItemType & type, const shared_ptr<Location> & location)
{
	switch (type)
	{
	case ItemType::Unknown: throw Exception("Unable to create item with Unknown type!");
	case ItemType::Weapon_AcidGun: return make_shared<AcidGun>(location);
	case ItemType::Weapon_ConstructionGun: return make_shared<ConstructionGun>(location);
	case ItemType::Weapon_Drill: return make_shared<Drill>(location);
	case ItemType::Weapon_ForceGun: return make_shared<ForceGun>(location);
	case ItemType::Weapon_GravityGun: return make_shared<GravityGun>(location);
	case ItemType::Weapon_GrenadeLauncher: return make_shared<GrenadeLauncher>(location);
	case ItemType::Weapon_Harpoon: return make_shared<Harpoon>(location);
	case ItemType::Weapon_Hook: return make_shared<Hook>(location);
	case ItemType::Weapon_LightningGun: return make_shared<LightningGun>(location);
	case ItemType::Weapon_Pickaxe: return make_shared<Pickaxe>(location);
	case ItemType::Weapon_PlasmaDrill: return make_shared<PlasmaDrill>(location);
	case ItemType::Weapon_RailGun: return make_shared<RailGun>(location);
	case ItemType::Weapon_RevealGun: return make_shared<RevealGun>(location);
	case ItemType::Weapon_RocketLauncher: return make_shared<RocketLauncher>(location);
	case ItemType::Weapon_Shotgun: return make_shared<Shotgun>(location);
	case ItemType::Weapon_SoilGun: return make_shared<SoilGun>(location);
	case ItemType::Weapon_Torch: return make_shared<Torch>(location);
	case ItemType::Shield_Steel: return make_shared<Shield>(location, ItemType::Shield_Steel);
	default: return make_shared<Item>(location, type);
	}
}

Bundle<ItemType> ItemDatabase::MakeBundleOfClassByRarity(const ItemClass & itemClass, int maxRarity)
{
	Bundle<ItemType> bundle;
	for (const auto& kv : mBase)
	{
		const auto& definition = kv.second;

		if (definition.mClass == itemClass)
		{
			if (definition.mRareness <= maxRarity)
			{
				bundle.Add(BundleItem<ItemType>(static_cast<float>(101 - definition.mRareness), definition.mType));
			}
		}
	}
	return bundle;
}

void ItemDatabase::Save()
{
	nlohmann::json root;

	for (auto & kv : mBase)
	{
		ItemDefinition & definition = kv.second;

		nlohmann::json def;

		def[ClassTag] = definition.mClass.ToString();
		def[NameTag] = definition.mNameKey;
		def[TextureTag] = definition.mTexture ? definition.mTexture->GetSource() : "";
		def[DescriptionTag] = definition.mDescriptionKey;
		def[BookTextTag] = definition.mBookTextKey;
		{
			nlohmann::json bodyPartAffectors;
			for (auto & bodyPartAffector : definition.mBodyPartAffectors)
			{
				nlohmann::json affector;
				affector[BodyPartAffectorRectTag] = bodyPartAffector.mRect;
				affector[BodyPartAffectorTextureTag] = bodyPartAffector.mTexturePath;
				bodyPartAffectors[bodyPartAffector.mType.ToString()] = affector;
			}
			def[BodyPartAffectorsTag] = bodyPartAffectors;
		}
		def[TextureRectTag] = definition.mRect;
		def[PriceTag] = definition.mPrice;
		def[WeightTag] = definition.mWeight;
		def[DurabilityTag] = definition.mDurability;
		{
			nlohmann::json dmg;
			dmg[DamageCountTag] = definition.mDamage;
			dmg[DamageClassTag] = definition.mDamageClass.ToString();
			dmg[DamageFlagsTag] = definition.mDamageType;
			def[DamageTag] = dmg;
		}
		def[PhysDmgReductionPercentTag] = definition.mPhysicalDamageReduction * 100.0f;
		def[ChemDmgReductionPercentTag] = definition.mChemicalDamageReduction * 100.0f;
		def[FireDmgReductionPercentTag] = definition.mFireDamageReduction * 100.0f;
		def[ElecDmgReductionPercentTag] = definition.mElectricalDamageReduction * 100.0f;
		def[HealTag] = definition.mHeal;
		def[HealFramesTag] = definition.mHealFrames;
		def[StaminaRestorationTag] = definition.mStaminaRestoration;
		def[StaminaRestorationFramesTag] = definition.mStaminaRestorationFrames;
		def[ConsumableTag] = definition.mConsumable.ToString();
		def[BlueprintResultTag] = definition.mBlueprintResultItem.ToString();
		def[StabilityTag] = definition.mStability;
		{
			nlohmann::json decomposeItems;

			unordered_map<ItemType, int> components;
			for (auto &comp : definition.mDecomposeItems)
			{
				components[comp]++;
			}

			for (auto & componentCountPair : components)
			{
				nlohmann::json item;
				decomposeItems[componentCountPair.first.ToString()] = componentCountPair.second;
			}

			def[DecomposeItemsTag] = decomposeItems;
		}
		{
			nlohmann::json composeItems;

			unordered_map<ItemType, int> components;
			for (auto &comp : definition.mComposeItems)
			{
				components[comp]++;
			}

			for (auto & componentCountPair : components)
			{
				nlohmann::json item;
				composeItems[componentCountPair.first.ToString()] = componentCountPair.second;
			}

			def[ComposeItemsTag] = composeItems;
		}
		def[ShootPeriodTag] = definition.mShootPeriod;
		def[ShootDistanceTag] = definition.mShootDistance;
		def[RequiredIntelligenceForCraftTag] = definition.mRequiredIntelligence;
		def[RequiredStrengthForUseTag] = definition.mRequiredStrengthForUse;
		def[RequiredDexterityForUseTag] = definition.mRequiredDexterityForUse;
		def[RequiredIntelligenceForUseTag] = definition.mRequiredIntelligenceForUse;
		def[IsMeleeTag] = definition.mIsMeleeWeapon;
		def[HasPhysicalBodyTag] = definition.mHasPhysicalBody;
		{
			nlohmann::json repairItems;
			for (auto & damageClassRepairItemsPair : definition.mRepairItems)
			{
				vector<ItemType> & itemList = damageClassRepairItemsPair.second;

				unordered_map<ItemType, int> components;
				for (auto &comp : itemList)
				{
					components[comp]++;
				}
				
				nlohmann::json damageType;

				for (auto & componentCountPair : components)
				{
					nlohmann::json item;
					damageType[componentCountPair.first.ToString()] = componentCountPair.second;
				}

				repairItems[damageClassRepairItemsPair.first.ToString()] = damageType;
			}
			def[RepairItemsTag] = repairItems;
		}
		def[ThrowableTag] = definition.mIsThrowableProjectile;
		def[RarenessTag] = definition.mRareness;

		root[definition.mType.ToString()] = def;
	}

	std::ofstream of(Path);
	of << setw(2) << root << endl;
}

ItemDefinition::ItemDefinition()
{
}

string ItemDefinition::GetDescription(const string &preText, int count, int flags) const
{
	auto str = StringBuilder();
	str << preText;

	if (!(flags & IDFF_EXCLUDE_NAME))
	{
		str << GetTranslatedName() << LineEnd;
	}

	if (!(flags & IDFF_EXCLUDE_COUNT) || count >= 0)
	{
		str << game->GetStr("count") << count << LineEnd;
	}

	str << game->GetStr("price") << mPrice << LineEnd;

	if (!(flags & IDFF_EXCLUDE_WEIGHT))
	{
		str << game->GetStr("weight") << mWeight << LineEnd;
	}
	if (!(flags & IDFF_EXCLUDE_TOTAL_WEIGHT))
	{
		str << game->GetStr("totalWeight") << count * mWeight << LineEnd;
	}

	if (!(flags & IDFF_EXCLUDE_DURABILITY))
	{
		str << game->GetStr("durability") << mDurability << LineEnd;
	}

	if (mClass == ItemClass::Ring || mClass == ItemClass::Armor || mClass == ItemClass::Gloves || mClass == ItemClass::Helmet ||
		mClass == ItemClass::Trousers || mClass == ItemClass::Shield)
	{
		str << game->GetStr("physDmgResist") << ": " << mPhysicalDamageReduction * 100.0f << "%"
			<< LineEnd;
		str << game->GetStr("fireDmgResist") << ": " << mFireDamageReduction * 100.0f << "%" << LineEnd;
		str << game->GetStr("chemDmgResist") << ": " << mChemicalDamageReduction * 100.0f << "%"
			<< LineEnd;
	}
	if (mClass == ItemClass::Potion)
	{
		if (mType == ItemType::Potion_MediumHeal || mType == ItemType::Potion_StrongHeal ||
			mType == ItemType::Potion_WeakHeal)
		{
			str << game->GetStr("heal") << ": " << mHeal;
			if (mHealFrames > 0)
			{
				str << " " << game->GetStr("during") << " " << mHealFrames / 60 << " "
					<< game->GetStr("seconds");
			}
			else
			{
				str << " " << game->GetStr("oneMoment");
			}
			str << LineEnd;
		}

		if (mType == ItemType::Potion_WeakStaminaRestoration || mType == ItemType::Potion_MediumStaminaRestoration ||
			mType == ItemType::Potion_StrongStaminaRestoration)
		{
			str << game->GetStr("restoreStamina") << ": " << mStaminaRestoration;
			if (mStaminaRestorationFrames > 0)
			{
				str << " " << game->GetStr("during") << " " << mStaminaRestorationFrames / 60 << " "
					<< game->GetStr("seconds");
			}
			else
			{
				str << " " << game->GetStr("oneMoment");
			}
			str << LineEnd;
		}
	}

	if (mClass == ItemClass::Shield)
	{
		str << game->GetStr("stability") << ": " << mStability * 100.0f << "%" << LineEnd;
	}
	if (!(flags & IDFF_EXCLUDE_DAMAGE))
	{
		if (mClass == ItemClass::Projectile || mClass == ItemClass::Weapon)
		{
			bool filled = false;
			if (mClass == ItemClass::Weapon)
			{
				str << game->GetStr("shootingSpeed") << ": ";
				if (mShootPeriod > 0)
				{
					str << FIXED_FPS / mShootPeriod << " " << game->GetStr("shotPerSec") << LineEnd;
				}
				else
				{
					str << game->GetStr("continuous") << LineEnd;
				}
				str << game->GetStr("shootDistance") << ": " << mShootDistance << LineEnd;
				if (mDamageClass == DamageClass::Unknown)
				{
					if (mConsumable != ItemType::Unknown)
					{
						// damage info derived from projectile
						auto &projDesc = game->GetItemDatabase()->Get(mConsumable);
						GetDamageInfo(str, projDesc.mDamage, projDesc.mDamageClass, projDesc.mDamageType, true,
							mShootPeriod);
					}
					filled = true;
				}
			}
			if (!filled)
			{
				GetDamageInfo(str, mDamage, mDamageClass, mDamageType, false, mShootPeriod);
			}
		}
	}
	if (mClass == ItemClass::Weapon)
	{
		if (mConsumable != ItemType::Unknown)
		{
			str << game->GetStr("projectiles") << ": " << game->GetItemDatabase()->Get(mConsumable).GetTranslatedName() << LineEnd;
		}
	}
	if (mClass == ItemClass::Blueprint)
	{
		if (mBlueprintResultItem != ItemType::Unknown)
		{
			str << game->GetStr("blueprintResult") << ": "
				<< game->GetItemDatabase()->Get(mBlueprintResultItem).GetTranslatedName() << LineEnd;
		}
	}
	if (!(flags & IDFF_EXCLUDE_DESCRIPTION))
	{
		str << game->GetStr("description") << GetTranslatedDescription() << LineEnd;
	}

	if (mClass == ItemClass::Weapon)
	{
		str << ColorToStr({ 0, 162, 232 }) << "*** " << game->GetStr("requiredSkills") << " ***" << ColorToStr({ 255, 255, 255 }) << LineEnd;

		str << game->GetStr("requiredStrength") << ": " << mRequiredStrengthForUse << LineEnd;
		str << game->GetStr("requiredDexterity") << ": " << mRequiredDexterityForUse << LineEnd;
		str << game->GetStr("requiredIntelligence") << ": " << mRequiredIntelligenceForUse << LineEnd;
	}

	return str;
}

void ItemDefinition::GetDamageInfo(StringBuilder &str, float damage, DamageClass dmgClass, int dmgType, bool projectileDerived, int shootingPeriod) const
{
	if (shootingPeriod > 0)
	{
		str << game->GetStr("damage") << ": " << damage;
	}
	else
	{
		str << game->GetStr("damage") << ": " << damage * FIXED_FPS << " " << game->GetStr("dmgPerSec");
	}

	if (projectileDerived)
	{
		str << "(" << game->GetStr("consumable") << ")";
	}
	str << LineEnd;
	str << game->GetStr("damageClass") << ": ";
	if (dmgClass == DamageClass::Physical)
	{
		str << game->GetStr("physicalDmg");
		if (projectileDerived)
		{
			str << "(" << game->GetStr("consumable") << ")";
		}
	}
	else if (dmgClass == DamageClass::Chemical)
	{
		str << game->GetStr("chemicalDmg");
		if (projectileDerived)
		{
			str << "(" << game->GetStr("consumable") << ")";
		}
	}
	else if (dmgClass == DamageClass::Fire)
	{
		str << game->GetStr("fireDmg");
		if (projectileDerived)
		{
			str << "(" << game->GetStr("consumable") << ")";
		}
	}
	str << LineEnd;
	str << game->GetStr("damageType") << ": ";
	if (dmgType == 0)
	{
		str << game->GetStr("standardDmg");
	}
	if (dmgType & DMG_UNBLOCKABLE)
	{
		str << game->GetStr("unblockableDmg");
	}
	if (projectileDerived)
	{
		str << "(" << game->GetStr("consumable") << ")";
	}
	str << LineEnd;
}

float ItemDefinition::ReduceDamage(float count, const DamageClass & damageClass) const
{
	switch (damageClass)
	{
	case DamageClass::Physical:
		count *= 1.0f - mPhysicalDamageReduction;
		break;
	case DamageClass::Fire:
		count *= 1.0f - mFireDamageReduction;
		break;
	case DamageClass::Chemical:
		count *= 1.0f - mChemicalDamageReduction;
		break;
	case DamageClass::Electrical:
		count *= 1.0f - mElectricalDamageReduction;
		break;
	}

	return count;
}

string ItemDefinition::GetTranslatedName() const
{
	return game->GetStr(mNameKey);
}

string ItemDefinition::GetTranslatedDescription() const
{
	return game->GetStr(mDescriptionKey);
}

string ItemDefinition::GetTranslatedBookText() const
{
	return game->GetStr(mBookTextKey);
}

BodyPartDefinition::BodyPartDefinition()
{
}

BodyPartDefinition::BodyPartDefinition(DwarfBodyPart type, const string & tex, const sf::FloatRect texRect)
	: mType(type)
	, mTexturePath(tex)
	, mRect(texRect)
{
}

