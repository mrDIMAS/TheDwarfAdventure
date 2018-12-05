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

#include "Damage.h"
#include "ItemType.h"
#include "Bundle.h"

// do not forget to add new class also to inventory gui filter
#define ITEM_CLASS_ENUM_ENTRIES(X) \
	X(Unknown,"Unknown") \
	X(Rubbish,"Rubbish") \
	X(Ore,"Ore") \
	X(Metal,"Metal") \
	X(Weapon,"Weapon") \
	X(Projectile,"Projectile") \
	X(Potion,"Potion") \
	X(Crystal,"Crystal") \
	X(Blueprint,"Blueprint") \
	X(Armor,"Armor") \
	X(Helmet,"Helmet") \
	X(Trousers,"Trousers") \
	X(Gloves,"Gloves") \
	X(Ring,"Ring") \
	X(Shield,"Shield") \
	X(Key,"Key") \
	X(Necklace,"Necklace") \
	X(Ingredient,"Ingredient") \
	X(Toolset,"Toolset") \
	X(Book,"Book") \

ENUM_DECLARE(ItemClass, ITEM_CLASS_ENUM_ENTRIES)

#undef ITEM_CLASS_ENUM_ENTRIES

#define DWARF_BODY_PART_ENUM_ENTRIES(X) \
	X(Unknown, "Unknown") \
	X(Head, "Head") \
	X(Beard, "Beard") \
	X(Hair, "Hair") \
	X(Body, "Body") \
	X(LeftUpperArm, "LeftUpperArm") \
	X(RightUpperArm, "RightUpperArm") \
	X(LeftLowerArm, "LeftLowerArm") \
	X(RightLowerArm, "RightLowerArm") \
	X(LeftUpperLeg, "LeftUpperLeg") \
	X(RightUpperLeg, "RightUpperLeg") \
	X(LeftLowerLeg, "LeftLowerLeg") \
	X(RightLowerLeg, "RightLowerLeg")

ENUM_DECLARE(DwarfBodyPart, DWARF_BODY_PART_ENUM_ENTRIES);

#undef DWARF_BODY_PART_ENUM_ENTRIES


#define ITEM_DAMAGE_CLASS_ENUM_ENTRIES(X) \
	X(Low, "Low") \
	X(Medium, "Medium") \
	X(High, "High")

ENUM_DECLARE(ItemDamageClass, ITEM_DAMAGE_CLASS_ENUM_ENTRIES);

#undef ITEM_DAMAGE_CLASS_ENUM_ENTRIES

enum ItemDescFormatFlags
{
	IDFF_EXCLUDE_NAME = BIT(0),
	IDFF_EXCLUDE_COUNT = BIT(1),
	IDFF_EXCLUDE_DESCRIPTION = BIT(2),
	IDFF_EXCLUDE_DURABILITY = BIT(3),
	IDFF_EXCLUDE_WEIGHT = BIT(4),
	IDFF_EXCLUDE_TOTAL_WEIGHT = BIT(5),
	IDFF_EXCLUDE_DAMAGE = BIT(6)
};

struct BodyPartDefinition
{
	DwarfBodyPart mType { DwarfBodyPart::Unknown };
	string mTexturePath;
	sf::FloatRect mRect;
	BodyPartDefinition();
	BodyPartDefinition(DwarfBodyPart type, const string &tex, const sf::FloatRect texRect);
};

class ItemDefinition
{
public:
	static constexpr int MaxRarity = 100;

	ItemType mType { ItemType::Unknown };
	ItemClass mClass { ItemClass::Unknown };
	// Key in translated strings database
	string mNameKey;
	shared_ptr<Texture> mTexture;
	// Key in translated strings database
	string mDescriptionKey;
	// Key in translated strings database
	string mBookTextKey;
	// Textures which will be applied to appropriate body part when actor puts on this item
	vector<BodyPartDefinition> mBodyPartAffectors;
	sf::FloatRect mRect { 0, 0, 1, 1 };
	// for all classes: prices in stores
	int mPrice { 1 };
	// for all classes: weight, defines max load of an actor
	float mWeight { 0 };
	// for all classes: durability (real durability value is individual for every
	// item and stored in Item so this is max durability)
	float mDurability { 1 };
	// for weapon and projectiles: defines damage which will be applied to an
	// actor.
	float mDamage { 0 };
	// for weapon and projectiles: defines damage class which will be applied to
	// an actor
	DamageClass mDamageClass { DamageClass::Unknown };
	// for weapon and projectiles: defines damage type which will be applied to an
	// actor
	int mDamageType { DMG_STANDARD };
	// For armor, helmet, gloves, trousers, rings: defines physical damage
	// reduction. The value is in [0; 1] range
	float mPhysicalDamageReduction { 0 };
	// For armor, helmet, gloves, trousers, rings: defines chemical damage
	// reduction. The value is in [0; 1] range
	float mChemicalDamageReduction { 0 };
	// For armor, helmet, gloves, trousers, rings: defines fire damage reduction
	// The value is in [0; 1] range
	float mFireDamageReduction { 0 };
	// For armor, helmet, gloves, trousers, rings: defines electrical damage
	// reduction. The value is in [0; 1] range
	float mElectricalDamageReduction { 0 };
	// for potions: defines amount of health to be restored
	float mHeal { 0 };
	// for potions: defines time period in which mHeal value of health became
	// restored
	int mHealFrames { 0 };
	// for potions:
	float mStaminaRestoration { 0 };
	int mStaminaRestorationFrames { 0 };
	// for weapons: defines item that weapon uses to work
	ItemType mConsumable { "Unknown" };
	// for blueprints: defines item which can be created using blueprint
	ItemType mBlueprintResultItem { "Unknown" };
	// For shields: defines stamina consumption coefficient (higher stability >
	// lower stamina consumption) The value is in [0; 1] range
	float mStability { 0 };
	// items given to player when he decomposes item
	vector<ItemType> mDecomposeItems;
	// items required to make this item
	vector<ItemType> mComposeItems;
	// for weapons: defines speed of shooting (given in frames)
	int mShootPeriod { 30 };
	// for weapons: defines max shooting distance (for raycasting weapons)
	//     or max projectile flight distance (for launchers)
	float mShootDistance { 0 };
	// for craftable items
	int mRequiredIntelligence { 1 };
	// item usage
	int mRequiredStrengthForUse { 1 };
	int mRequiredDexterityForUse { 1 };
	int mRequiredIntelligenceForUse { 1 };
	// for weapons: indicates that a weapon is melee (pickaxe as example)
	bool mIsMeleeWeapon { false };
	bool mHasPhysicalBody { false };
	// list of items for repair
	// items with less damage class will be taken into account on Bench
	// (stacks together)
	unordered_map<ItemDamageClass, vector<ItemType>> mRepairItems;
	// for throwable projectiles (grenades etc.)
	bool mIsThrowableProjectile { false };
	// Item rareness in [0; 100] range
	int mRareness { 10 };
	// defaults
	ItemDefinition();
	// returns string with full description of an item
	// if count < 0 - such string wont be added to result string
	// flags (combination of ItemDescFormatFlags) allow to define what actually
	// need to be put in out string
	string GetDescription(const string &preText, int count, int flags = 0) const;
	// projectileDerived - tells formatter to add "(consumable)" after damage
	// fields
	//      this info tells player that weapon does no damage by itself, but
	//      damage added from consumable
	void GetDamageInfo(StringBuilder &str, float damage, DamageClass dmgClass, int dmgType, bool projectileDerived, int shootingPeriod) const;
	float ReduceDamage(float count, const DamageClass & damageClass) const;

	string GetTranslatedName() const;
	string GetTranslatedDescription() const;
	string GetTranslatedBookText() const;
};

class ItemDatabase
{
public:
	using DefinitionCollection = unordered_map<ItemType, ItemDefinition>;

	static constexpr const char* ClassTag = "class";
	static constexpr const char* NameTag = "name";
	static constexpr const char* TextureTag = "texture";
	static constexpr const char* DescriptionTag = "description";
	static constexpr const char* BookTextTag = "bookText";
	static constexpr const char* BodyPartAffectorsTag = "bodyPartAffectors";
	static constexpr const char* TextureRectTag = "textureRect";
	static constexpr const char* PriceTag = "price";
	static constexpr const char* WeightTag = "weight";
	static constexpr const char* DurabilityTag = "durability";
	static constexpr const char* DamageTag = "damage";
	static constexpr const char* PhysDmgReductionPercentTag = "physDmgReductionPercent";
	static constexpr const char* ChemDmgReductionPercentTag = "chemDmgReductionPercent";
	static constexpr const char* FireDmgReductionPercentTag = "fireDmgReductionPercent";
	static constexpr const char* ElecDmgReductionPercentTag = "elecDmgReductionPercent";
	static constexpr const char* HealTag = "heal";
	static constexpr const char* HealFramesTag = "healFrames";
	static constexpr const char* StaminaRestorationTag = "staminaRestoration";
	static constexpr const char* StaminaRestorationFramesTag = "staminaRestorationFrames";
	static constexpr const char* ConsumableTag = "consumable";
	static constexpr const char* BlueprintResultTag = "blueprintResult";
	static constexpr const char* StabilityTag = "stability";
	static constexpr const char* ComposeItemsTag = "composeItems";
	static constexpr const char* DecomposeItemsTag = "decomposeItems";
	static constexpr const char* RepairItemsTag = "repairItems";
	static constexpr const char* ShootPeriodTag = "shootPeriod";
	static constexpr const char* HasPhysicalBodyTag = "hasPhysicalBody";
	static constexpr const char* ShootDistanceTag = "shootDistance";
	static constexpr const char* RequiredIntelligenceForCraftTag = "requiredIntelligenceForCraft";
	static constexpr const char* RequiredDexterityForUseTag = "requiredDexterityForUse";
	static constexpr const char* RequiredStrengthForUseTag = "requiredStrengthForUse";
	static constexpr const char* RequiredIntelligenceForUseTag = "requiredIntelligenceForUse";
	static constexpr const char* IsMeleeTag = "isMelee";
	static constexpr const char* ThrowableTag = "throwable";
	static constexpr const char* RarenessTag = "rareness";

	static constexpr const char* DamageClassTag = "class";
	static constexpr const char* DamageCountTag = "count";
	static constexpr const char* DamageFlagsTag = "flags";

	static constexpr const char* BodyPartAffectorRectTag = "rect";
	static constexpr const char* BodyPartAffectorTextureTag = "texture";

	static constexpr const char* Path = "data/items/db.json";

	ItemDatabase();
	~ItemDatabase();
	const ItemDefinition &Get(const ItemType &type) const;
	DefinitionCollection &GetRecords();
	shared_ptr<Item> Make(const ItemType & type, const shared_ptr<Location> & location);
	Bundle<ItemType> MakeBundleOfClassByRarity(const ItemClass& itemClass, int maxRarity = ItemDefinition::MaxRarity);
	void Save();

private:
	DefinitionCollection mBase;
	void ParseDatabase();
};
