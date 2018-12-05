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

#define ITEM_TYPE_ENUM_ENTRIES(X) \
	X(Unknown, "Unknown") \
	X(Rubbish, "Rubbish") \
	X(Ore_Platinum, "Ore_Platinum") \
	X(Ore_Gold, "Ore_Gold") \
	X(Ore_Silver, "Ore_Silver") \
	X(Ore_Copper, "Ore_Copper") \
	X(Ore_Tungsten, "Ore_Tungsten") \
	X(Ore_Tin, "Ore_Tin") \
	X(Ore_Iron, "Ore_Iron") \
	X(Metal_Platinum, "Metal_Platinum") \
	X(Metal_Gold, "Metal_Gold") \
	X(Metal_Silver, "Metal_Silver") \
	X(Metal_Copper, "Metal_Copper") \
	X(Metal_Tungsten, "Metal_Tungsten") \
	X(Metal_Tin, "Metal_Tin") \
	X(Metal_Iron, "Metal_Iron") \
	X(Weapon_Drill, "Weapon_Drill") \
	X(Weapon_PlasmaDrill, "Weapon_PlasmaDrill") \
	X(Weapon_Hook, "Weapon_Hook") \
	X(Weapon_Shotgun, "Weapon_Shotgun") \
	X(Weapon_GrenadeLauncher, "Weapon_GrenadeLauncher") \
	X(Weapon_SoilGun, "Weapon_SoilGun") \
	X(Weapon_RocketLauncher, "Weapon_RocketLauncher") \
	X(Weapon_Harpoon, "Weapon_Harpoon") \
	X(Weapon_RevealGun, "Weapon_RevealGun") \
	X(Weapon_AcidGun, "Weapon_AcidGun") \
	X(Weapon_RailGun, "Weapon_RailGun") \
	X(Weapon_Pickaxe, "Weapon_Pickaxe") \
	X(Weapon_GravityGun, "Weapon_GravityGun") \
	X(Weapon_ForceGun, "Weapon_ForceGun") \
	X(Weapon_LightningGun, "Weapon_LightningGun") \
	X(Weapon_Torch, "Weapon_Torch") \
	X(Weapon_ConstructionGun, "Weapon_ConstructionGun") \
	X(Projectile_Rocket, "Projectile_Rocket") \
	X(Projectile_Grenade, "Projectile_Grenade") \
	X(Projectile_Shells, "Projectile_Shells") \
	X(Projectile_Harpoon, "Projectile_Harpoon") \
	X(Projectile_AcidBlob, "Projectile_AcidBlob") \
	X(Projectile_Rail, "Projectile_Rail") \
	X(Projectile_Fireball, "Projectile_Fireball") \
	X(Projectile_ConstructionBlock, "Projectile_ConstructionBlock") \
	X(Projectile_Dynamite, "Projectile_Dynamite") \
	X(Crystal_SmallPower, "Crystal_SmallPower") \
	X(Crystal_RareRed, "Crystal_RareRed") \
	X(Crystal_Homeward, "Crystal_Homeward") \
	X(Crystal_Polycrystal, "Crystal_Polycrystal") \
	X(Potion_WeakHeal, "Potion_WeakHeal") \
	X(Potion_MediumHeal, "Potion_MediumHeal") \
	X(Potion_StrongHeal, "Potion_StrongHeal") \
	X(Potion_WeakStaminaRestoration, "Potion_WeakStaminaRestoration") \
	X(Potion_MediumStaminaRestoration, "Potion_MediumStaminaRestoration") \
	X(Potion_StrongStaminaRestoration, "Potion_StrongStaminaRestoration") \
	X(Armor_LightLeatherArmor, "Armor_LightLeatherArmor") \
	X(Armor_LeatherArmor, "Armor_LeatherArmor") \
	X(Armor_SteelArmor, "Armor_SteelArmor") \
	X(Helmet_IronHelmet, "Helmet_IronHelmet") \
	X(Helmet_SteelHelmet, "Helmet_SteelHelmet") \
	X(Trousers_LightLeatherTrousers, "Trousers_LightLeatherTrousers") \
	X(Trousers_LeatherTrousers, "Trousers_LeatherTrousers") \
	X(Trousers_SteelTrousers, "Trousers_SteelTrousers") \
	X(Gloves_LightLeatherGloves, "Gloves_LightLeatherGloves") \
	X(Gloves_LeatherGloves, "Gloves_LeatherGloves") \
	X(Gloves_SteelGloves, "Gloves_SteelGloves") \
	X(Ring_RingOfLife, "Ring_RingOfLife") \
	X(Ring_RingOfFireResistance, "Ring_RingOfFireResistance") \
	X(Ring_RingWithGreenGemstone, "Ring_RingWithGreenGemstone") \
	X(Ring_RingOfSteelProtection, "Ring_RingOfSteelProtection") \
	X(Key_Rusty, "Key_Rusty") \
	X(Shield_Steel, "Shield_Steel") \
	X(Blueprint_MetalIron, "Blueprint_MetalIron") \
	X(Blueprint_MetalPlatinum, "Blueprint_MetalPlatinum") \
	X(Blueprint_MetalGold, "Blueprint_MetalGold") \
	X(Blueprint_MetalSilver, "Blueprint_MetalSilver") \
	X(Blueprint_MetalCopper, "Blueprint_MetalCopper") \
	X(Blueprint_MetalTungsten, "Blueprint_MetalTungsten") \
	X(Blueprint_MetalTin, "Blueprint_MetalTin") \
	X(Blueprint_Drill, "Blueprint_Drill") \
	X(Blueprint_PlasmaDrill, "Blueprint_PlasmaDrill") \
	X(Blueprint_RocketLauncher, "Blueprint_RocketLauncher") \
	X(Blueprint_GrenadeLauncher, "Blueprint_GrenadeLauncher") \
	X(Blueprint_Railgun, "Blueprint_Railgun") \
	X(Blueprint_Hook, "Blueprint_Hook") \
	X(Blueprint_Shotgun, "Blueprint_Shotgun") \
	X(Blueprint_Harpoon, "Blueprint_Harpoon") \
	X(Blueprint_RevealGun, "Blueprint_RevealGun") \
	X(Blueprint_AcidGun, "Blueprint_AcidGun") \
	X(Blueprint_Pickaxe, "Blueprint_Pickaxe") \
	X(Blueprint_GravityGun, "Blueprint_GravityGun") \
	X(Blueprint_ForceGun, "Blueprint_ForceGun") \
	X(Blueprint_LightningGun, "Blueprint_LightningGun") \
	X(Blueprint_Rocket, "Blueprint_Rocket") \
	X(Blueprint_Grenade, "Blueprint_Grenade") \
	X(Blueprint_Shells, "Blueprint_Shells") \
	X(Blueprint_HarpoonProj, "Blueprint_HarpoonProj") \
	X(Blueprint_AcidBlob, "Blueprint_AcidBlob") \
	X(Blueprint_Rail, "Blueprint_Rail") \
	X(Toolset_Small, "Toolset_Small") \
	X(Toolset_HeavyDuty, "Toolset_HeavyDuty") \
	X(Toolset_Tungsten, "Toolset_Tungsten") \
	X(Ingredient_GunPowder, "Ingredient_GunPowder") \
	X(Ingredient_Leather, "Ingredient_Leather") \
	X(Ingredient_Fur, "Ingredient_Fur") \
	X(Ingredient_Wood, "Ingredient_Wood") \
	X(Ingredient_Fabric, "Ingredient_Fabric") \
	X(Book_EpochBeforeArkalith, "Book_EpochBeforeArkalith") \
	X(Book_DiscoveryOfArkalith, "Book_DiscoveryOfArkalith") \
	X(Book_Seasons, "Book_Seasons") 

ENUM_DECLARE(ItemType, ITEM_TYPE_ENUM_ENTRIES);

#undef ITEM_TYPE_ENUM_ENTRIES