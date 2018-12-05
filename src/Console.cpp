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
#include "Console.h"
#include "Game.h"
#include "Input.h"
#include "Item.h"
#include "Level.h"
#include "Player.h"
#include "Inventory.h"
#include "Stats.h"
#include "Level.h"
#include "Scene.h"
#include "Torch.h"
#include "Boss.h"
#include "Door.h"
#include "Chest.h"

void Console::PrintStats(StringBuilder & str, Stats * stats)
{
	str << "Level: " << stats->GetLevel() << LineEnd
		<< "Main stats"
		<< "\tVitality: " << stats->Vitality.Get() << LineEnd
		<< "\tStrenght: " << stats->Strenght.Get() << LineEnd
		<< "\tDexterity: " << stats->Dexterity.Get() << LineEnd
		<< "\tEndurance: " << stats->Endurance.Get() << LineEnd
		<< "\tIntelligence: " << stats->Intelligence.Get() << LineEnd
		<< "\tLuck: " << stats->Luck.Get() << LineEnd
		<< "\tCharisma: " << stats->Charisma.Get() << LineEnd;

}

Console::Console()
{
	for (auto &desc : game->GetItemDatabase()->GetRecords())
	{
		mGiveList.push_back(desc.second.GetTranslatedName());
	}

	Log() << "Debug menu successfully initialized!" << LineEnd;
}

Console::~Console()
{
}

void Console::DrawUpdate()
{
	auto input = game->GetInput();
	if (input->IsKeyHit(sf::Keyboard::Tilde))
	{
		mActive = !mActive;
	}
	if (!mActive)
	{
		return;
	}
	ImGui::Begin("Debug Menu by v1al");
	ImGui::Text("=== Debug Menu v1.1 for The Dwarf Adventure ===");
	ImGui::Text("       !!!  Use with extreme caution !!!       ");
	ImGui::Text("Note: many function available only in game.");
	ImGui::Separator();
	ItemDatabase* db = game->GetItemDatabase();
	if (auto level = game->GetLevel())
	{
		if (auto player = level->GetPlayer())
		{
			auto inv = player->GetInventory();
			auto location = player->GetLocation();

			if (ImGui::CollapsingHeader("Give items to player"))
			{
				static int currentItem = 0;
				static int giveCount = 1;

				if (ImGui::Button("Give Surviver Item Set", { 300, 0 }))
				{
					for (int i = 0; i < 100; ++i)
					{
						inv->AddItem(db->Make(ItemType::Projectile_Rocket, location));
						inv->AddItem(db->Make(ItemType::Projectile_Grenade, location));
						inv->AddItem(db->Make(ItemType::Projectile_Shells, location));
						inv->AddItem(db->Make(ItemType::Projectile_Harpoon, location));
						inv->AddItem(db->Make(ItemType::Projectile_AcidBlob, location));
						inv->AddItem(db->Make(ItemType::Projectile_Rail, location));
						inv->AddItem(db->Make(ItemType::Projectile_Dynamite, location));
					}

					for (int i = 0; i < 20; ++i)
					{
						inv->AddItem(db->Make(ItemType::Metal_Copper, location));
						inv->AddItem(db->Make(ItemType::Metal_Gold, location));
						inv->AddItem(db->Make(ItemType::Metal_Iron, location));
						inv->AddItem(db->Make(ItemType::Metal_Platinum, location));
						inv->AddItem(db->Make(ItemType::Metal_Silver, location));
						inv->AddItem(db->Make(ItemType::Metal_Tin, location));
						inv->AddItem(db->Make(ItemType::Metal_Tungsten, location));

						inv->AddItem(db->Make(ItemType::Ingredient_Fur, location));
						inv->AddItem(db->Make(ItemType::Ingredient_GunPowder, location));
						inv->AddItem(db->Make(ItemType::Ingredient_Leather, location));
						inv->AddItem(db->Make(ItemType::Crystal_Polycrystal, location));
					}
					for (int i = 0; i < 20; ++i)
					{
						inv->AddItem(db->Make(ItemType::Potion_WeakHeal, location));
						inv->AddItem(db->Make(ItemType::Potion_MediumHeal, location));
						inv->AddItem(db->Make(ItemType::Potion_StrongHeal, location));
					}
					for (int i = 0; i < 10; ++i)
					{
						inv->AddItem(db->Make(ItemType::Crystal_SmallPower, location));
						inv->AddItem(db->Make(ItemType::Crystal_RareRed, location));
					}
					inv->AddItem(db->Make(ItemType::Armor_LightLeatherArmor, location));
					inv->AddItem(db->Make(ItemType::Helmet_IronHelmet, location));
					inv->AddItem(db->Make(ItemType::Gloves_LightLeatherGloves, location));
					inv->AddItem(db->Make(ItemType::Trousers_LightLeatherTrousers, location));

					inv->AddItem(db->Make(ItemType::Crystal_Homeward, location));
					inv->AddItem(db->Make(ItemType::Ring_RingOfLife, location));
					inv->AddItem(db->Make(ItemType::Key_Rusty, location));
					inv->AddItem(db->Make(ItemType::Shield_Steel, location));

					inv->AddItem(db->Make(ItemType::Blueprint_MetalCopper, location));
					inv->AddItem(db->Make(ItemType::Blueprint_MetalGold, location));
					inv->AddItem(db->Make(ItemType::Blueprint_MetalIron, location));
					inv->AddItem(db->Make(ItemType::Blueprint_MetalPlatinum, location));
					inv->AddItem(db->Make(ItemType::Blueprint_MetalSilver, location));
					inv->AddItem(db->Make(ItemType::Blueprint_MetalTin, location));
					inv->AddItem(db->Make(ItemType::Blueprint_MetalTungsten, location));
					inv->AddItem(db->Make(ItemType::Blueprint_Drill, location));
					inv->AddItem(db->Make(ItemType::Blueprint_PlasmaDrill, location));
					inv->AddItem(db->Make(ItemType::Blueprint_GrenadeLauncher, location));
					inv->AddItem(db->Make(ItemType::Blueprint_RocketLauncher, location));
					inv->AddItem(db->Make(ItemType::Blueprint_Railgun, location));

					inv->AddItem(db->Make(ItemType::Weapon_AcidGun, location));
					inv->AddItem(db->Make(ItemType::Weapon_Drill, location));
					inv->AddItem(db->Make(ItemType::Weapon_GrenadeLauncher, location));
					inv->AddItem(db->Make(ItemType::Weapon_Harpoon, location));
					inv->AddItem(db->Make(ItemType::Weapon_Hook, location));
					inv->AddItem(db->Make(ItemType::Weapon_PlasmaDrill, location));
					inv->AddItem(db->Make(ItemType::Weapon_RailGun, location));
					inv->AddItem(db->Make(ItemType::Weapon_RevealGun, location));
					inv->AddItem(db->Make(ItemType::Weapon_Shotgun, location));
					inv->AddItem(db->Make(ItemType::Weapon_SoilGun, location));
					inv->AddItem(db->Make(ItemType::Weapon_LightningGun, location));

					inv->AddItem(db->Make(ItemType::Weapon_Torch, location));

					player->AddItem(db->Make(ItemType::Book_DiscoveryOfArkalith, location));
					player->AddItem(db->Make(ItemType::Book_EpochBeforeArkalith, location));
					player->AddItem(db->Make(ItemType::Book_Seasons, location));

					auto rocketLauncher = db->Make(ItemType::Weapon_RocketLauncher, location);
					rocketLauncher->ModifyValue(ItemModifierType::Damage, 1.1f);
					rocketLauncher->ModifyValue(ItemModifierType::ShootDistance, 1.1f);
					rocketLauncher->ModifyValue(ItemModifierType::ShootPeriod, 1.1f);
					inv->AddItem(rocketLauncher);
					player->UseItem(rocketLauncher, Hand::Left);
				}
				if (ImGui::Button("Make Me Rich"))
				{
					inv->AddMoney(0xFFFF);
				}

				auto getter = [] (void *data, int idx, const char **out_text)
				{
					if (out_text)
					{
						*out_text = ((vector<string> *)data)->at(idx).c_str();
					}
					return true;
				};

				ImGui::PushItemWidth(300);
				ImGui::ListBox("Items", &currentItem, getter, &mGiveList, mGiveList.size(), 6);
				ImGui::PopItemWidth();
				if (ImGui::Button("Give", { 75, 0 }))
				{
					ItemType type;
					auto iter = db->GetRecords().begin();
					std::advance(iter, currentItem);
					for (int i = 0; i < giveCount; ++i)
					{
						player->AddItem(db->Make(iter->first, location));
					}
				}
				ImGui::SameLine();
				ImGui::PushItemWidth(136);
				ImGui::DragInt("##0", &giveCount, 1, 0, 999);
				ImGui::PopItemWidth();
			}

			if (ImGui::CollapsingHeader("Debug control"))
			{
				ImGui::Separator();
				ImGui::Checkbox("Show Hitboxes", &game->Debug.showHitboxes);
				ImGui::Checkbox("Dump actor explode texture to file", &game->Debug.dumpExplodeTexture);
				ImGui::Checkbox("Show paths", &game->Debug.showPaths);
				ImGui::Checkbox("Show walkable vertices", &game->Debug.showWalkableVertices);
				ImGui::Checkbox("Allow fast travel on undiscovered locations", &game->Debug.allowFastTravelOnUndiscoveredLocations);
				ImGui::Checkbox("Show bot vision triangle", &game->Debug.showBotVisionTriangle);
				ImGui::Checkbox("Verbose unstable zone analysis output", &game->Debug.verboseUnstableZoneAnalysisOutput);
				ImGui::Checkbox("Disable gravity", &game->Debug.gravityDisabled);
				ImGui::Checkbox("Terrain rendering disabled", &game->Debug.terrainRenderingDisabled);
				ImGui::Checkbox("Show player fog-of-war reveal rays", &game->Debug.showPlayerFogOfWarRevealRays);
				ImGui::Checkbox("Disable player-terrain collisions", &game->Debug.disablePlayerTerrainCollisions);
			}

			if (ImGui::CollapsingHeader("Inter-location doors"))
			{
				int id = 0;
				for (const shared_ptr<Door> & door : location->GetDoorList())
				{
					if (door->GetType() == DoorType::ToOtherScene)
					{
						if (const std::shared_ptr<Door> & destDoor = door->GetDestinationDoor())
						{
							std::string name = "To " + destDoor->GetLocation()->GetDefinition()->GetType().ToString();
							ImGui::PushID(id);
							if (ImGui::Button(name.c_str()))
							{
								level->TransferPlayerToLocation(door->GetLocation(), door->GetPosition());
							}
							ImGui::PopID();
						}
						else
						{
							ImGui::Text("Invalid door: dest location is missing!");
						}
					}
					++id;
				}
			}

			if (ImGui::CollapsingHeader("Player stats"))
			{
				if (ImGui::Button("+10 Reputation", { 145, 0 }))
				{
					player->GetStats()->ChangeReputation(+10);
				}
				ImGui::SameLine();
				if (ImGui::Button("-10 Reputation", { 145, 0 }))
				{
					player->GetStats()->ChangeReputation(-10);
				}
				if (ImGui::Button("Level Up"))
				{
					player->GetStats()->AddExpirience(player->GetStats()->GetExpirienceForNextLevel());
				}
				if (ImGui::Button("Die"))
				{
					Damage dmg;
					dmg.mCount = 1000000;
					dmg.mFlags = DMG_UNBLOCKABLE;
					dmg.mClass = DamageClass::Physical;
					player->ApplyDamage(dmg);
				}
			}

			if (ImGui::CollapsingHeader("Current location statistics"))
			{
				auto scene = location->GetScene();

				StringBuilder stat;
				stat << "Location statistics: " << LineEnd
					<< "\tActor count: " << location->GetActorList().size() << LineEnd
					<< "\tChest count: " << location->GetChestList().size() << LineEnd
					<< "\tDoors count: " << location->GetDoorList().size() << LineEnd
					<< "\tFog Gate count: " << location->GetFogGateList().size() << LineEnd
					<< "\tItem count: " << location->GetItemList().size() << LineEnd
					<< "\tLadder count: " << location->GetLadderList().size() << LineEnd
					<< "\tMother crystal count: " << location->GetMotherCrystalList().size() << LineEnd
					<< "\tRemains count: " << location->GetRemainsList().size() << LineEnd
					<< "\tRoom count: " << location->GetRoomList().size() << LineEnd
					<< "\tWorkbench count: " << location->GetWorkbenchList().size() << LineEnd;

				stat << "Scene statistics:" << LineEnd
					<< "\tAnimation count: " << scene->GetAnimationList().size() << LineEnd
					<< "\tLight count: " << scene->GetLightList().size() << LineEnd
					<< "\tNode count: " << scene->GetNodeList().size() << LineEnd
					<< "\tParticle system count: " << scene->GetParticleSystemList().size() << LineEnd
					<< "\tPolyline count: " << scene->GetPolyLineList().size() << LineEnd
					<< "\tSprite count: " << scene->GetSpriteList().size() << LineEnd
					<< "\tText count: " << scene->GetTextList().size() << LineEnd
					<< "\tWater blob count: " << scene->GetWaterBlobList().size() << LineEnd
					<< "\tSound channel count: " << scene->GetSoundList().size() << LineEnd;

				stat << "GUI statistics" << LineEnd
					<< "\tTotal Canvas count: " << game->GetTotalCanvasCount() << LineEnd
					<< "\tTotal Widget count: " << game->GetTotalWidgetCount() << LineEnd
					<< "\tVisible canvas count: " << game->GetVisibleCanvasCount() << LineEnd
					<< "\tVisible widget count: " << game->GetVisibleWidgetCount() << LineEnd;

				string s = stat;

				ImGui::Text(s.c_str());
			}

			if (ImGui::CollapsingHeader("Boss list"))
			{
				ImGui::Separator();
				mBossList.clear();
				for (auto & levelLocation : level->GetLocationList())
				{
					for (auto & actor : levelLocation->GetActorList())
					{
						if (auto boss = dynamic_pointer_cast<Boss>(actor))
						{
							mBossList.push_back(boss.get());
						}
					}
				}
				auto getter = [] (void *data, int idx, const char **out_text)
				{
					if (out_text)
					{
						*out_text = ((vector<Boss*>*)data)->at(idx)->GetName().c_str();
					}
					return true;
				};
				static int bossIndex = -1;
				ImGui::ListBox("Boss List", &bossIndex, getter, &mBossList, mBossList.size(), 6);
				Boss* boss;
				if (bossIndex >= 0 && bossIndex < static_cast<int>(mBossList.size()))
				{
					boss = mBossList.at(bossIndex);
				}
				else
				{
					boss = nullptr;
				}

				if (boss)
				{
					if (ImGui::Button("Kill boss"))
					{
						Damage fatalDamage;
						fatalDamage.mClass = DamageClass::Physical;
						fatalDamage.mFlags = DMG_UNBLOCKABLE;
						fatalDamage.mCount = 99999999.0f;
						boss->ApplyDamage(fatalDamage);
					}
					ImGui::SameLine();
					if (ImGui::Button("Teleport to boss (begin fight)"))
					{
						level->TransferPlayerToLocation(boss->GetLocation(), boss->GetPosition());
					}
				}
			}
			if (ImGui::CollapsingHeader("NPC list"))
			{
				ImGui::Separator();
				mNPCList.clear();
				for (auto & levelLocation : level->GetLocationList())
				{
					for (auto & actor : levelLocation->GetActorList())
					{
						if (auto npc = dynamic_pointer_cast<NPC>(actor))
						{
							mNPCList.push_back(npc.get());
						}
					}
				}
				auto getter = [] (void *data, int idx, const char **out_text)
				{
					if (out_text)
					{
						*out_text = ((vector<NPC*>*)data)->at(idx)->GetName().c_str();
					}
					return true;
				};
				static int npcIndex = -1;
				ImGui::ListBox("NPC List", &npcIndex, getter, &mNPCList, mNPCList.size(), 6);
				NPC* npc;
				if (npcIndex >= 0 && npcIndex < static_cast<int>(mNPCList.size()))
				{
					npc = mNPCList.at(npcIndex);
				}
				else
				{
					npc = nullptr;
				}

				if (npc)
				{
					StringBuilder info;
					PrintStats(info, npc->GetStats().get());
					string infoStr = info;
					ImGui::Text(infoStr.c_str());

					if (ImGui::Button("Kill NPC"))
					{
						Damage fatalDamage;
						fatalDamage.mClass = DamageClass::Physical;
						fatalDamage.mFlags = DMG_UNBLOCKABLE;
						fatalDamage.mCount = 666666.0f;
						npc->ApplyDamage(fatalDamage);
					}
					ImGui::SameLine();
					if (ImGui::Button("Teleport to NPC"))
					{
						level->TransferPlayerToLocation(npc->GetLocation(), npc->GetPosition());
					}
				}
			}

			if (ImGui::CollapsingHeader("Chest items statistics per location"))
			{
				using TypeCountMap = unordered_map<ItemType, int>;

				struct ItemStat
				{
					ItemType type;
					int count;
				};

				struct LocaStat
				{
					Location* loca;
					TypeCountMap mp;
					vector<ItemStat> items;
				};
				
				static vector<LocaStat> chestItemsPerLocation;
				chestItemsPerLocation.clear();
				for (const shared_ptr<Location>& loca : level->GetLocationList())
				{
					LocaStat stat;
					stat.loca = loca.get();
					for (const shared_ptr<Chest>& chest : loca->GetChestList())
					{
						for (const shared_ptr<ItemGroup>& group : chest->GetInventory()->GetGroups())
						{
							for (const shared_ptr<Item>& item : group->mItems)
							{
								stat.mp[item->GetType()]++;
							}
						}
					}
					chestItemsPerLocation.push_back(stat);
				}

				for (auto& stat : chestItemsPerLocation)
				{
					for (const auto& kv : stat.mp)
					{
						ItemStat item;
						item.type = kv.first;
						item.count = kv.second;
						stat.items.push_back(item);
					}

					sort(stat.items.begin(), stat.items.end(), [] (const ItemStat& a, const ItemStat& b)
					{
						return a.count < b.count;
					});
				}
												
				int id = 0;
				for (const auto& stat : chestItemsPerLocation)
				{			
					ImGui::PushID(++id);
					if (ImGui::CollapsingHeader(stat.loca->GetDefinition()->GetName().c_str()))
					{
						for (const auto& item : stat.items)
						{
							string str = item.type.ToString() + " - " + to_string(item.count);

							ImGui::Text(str.c_str());
						}
					}
					ImGui::PopID();
				}
			}
		}
	}
	ImGui::End();
}
