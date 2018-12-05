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
#include "Bench.h"
#include "Game.h"
#include "Canvas.h"
#include "Tooltip.h"
#include "Inventory.h"
#include "Label.h"
#include "Window.h"
#include "Button.h"
#include "Table.h"
#include "Player.h"
#include "Level.h"
#include "Inventory.h"
#include "Item.h"
#include "AcidGun.h"
#include "GrenadeLauncher.h"
#include "Drill.h"
#include "PlasmaDrill.h"
#include "Hook.h"
#include "GravityGun.h"
#include "RailGun.h"
#include "Harpoon.h"
#include "RevealGun.h"
#include "RocketLauncher.h"
#include "Shotgun.h"
#include "SoilGun.h"
#include "Stats.h"
#include "NumericUpDown.h"

void Bench::SetPage(Page page)
{
	mBlueprints.clear();
	mCraftComponents.clear();
	mDecomposableItems.clear();
	mItemPartsAfterDecomposition.clear();
	mItemWillBeDecomposed.reset();
	mDamagedItems.clear();
	mComponentsForRepair.clear();
	mItemForRepair.reset();
	mPossibleModItems.clear();
	mSelectionWindow->SetLocalVisibility(page == Page::Selector);
	mModifySelectorWindow->SetLocalVisibility(page == Page::ModifySelector);
	mCraftWindow->SetLocalVisibility(page == Page::Craft);
	mRepairWindow->SetLocalVisibility(page == Page::Repair);
	mDecomposeWindow->SetLocalVisibility(page == Page::Decompose);
	mModifyWindow->SetLocalVisibility(page == Page::Modify);
	mModifySelectorSelection->SetEnabled(false);
	if (page == Page::Craft)
	{
		SyncCraft();
	}
	if (page == Page::ModifySelector)
	{
		SyncModifySelector();
	}
	if (page == Page::Decompose)
	{
		SyncDecompose();
	}
	if (page == Page::Repair)
	{
		SyncRepair();
	}
	if (page != Page::Modify)
	{
		mModifiableItem.reset();
	}
	else
	{
		SyncModify();
	}
	mPage = page;
}

void Bench::CalcModCostAndPrint()
{
	auto fmt = StringBuilder();
	fmt << ColorToStr({ 0, 162, 232 }) << "*** " << game->GetStr("modificationCost") << " ***"
		<< ColorToStr({ 255, 255, 255 }) << LineEnd;
	mModifiableItem->EnlistModifications(fmt);
	mModCost->SetText(fmt);
}

void Bench::SyncCraft()
{
	auto inv = game->GetLevel()->GetPlayer()->GetInventory();
	mCraftItems->Clear();
	for (auto &group : inv->GetGroups())
	{
		auto &desc = group->mItems.at(0)->GetDefinition();
		if (desc.mClass == ItemClass::Blueprint)
		{
			mBlueprints.push_back(group);
			int row = mCraftItems->AddRow();
			dynamic_pointer_cast<Label>(mCraftItems->Get(0, row))
				->SetText(StringBuilder("x") << group->Count() << " - " << desc.GetTranslatedName());
		}
	}
}

void Bench::SyncRepair()
{
	auto inv = game->GetLevel()->GetPlayer()->GetInventory();
	mRepairItems->Clear();
	for (auto &group : inv->GetGroups())
	{
		for (auto &item : group->mItems)
		{
			if (!item->IsBrandNew())
			{
				mDamagedItems.push_back(item);
				int row = mRepairItems->AddRow();
				dynamic_pointer_cast<Label>(mRepairItems->Get(0, row))->SetText(item->GetDefinition().GetTranslatedName());
			}
		}
	}
}

void Bench::SyncModifySelector()
{
	auto inv = game->GetLevel()->GetPlayer()->GetInventory();
	ItemDatabase* db = game->GetItemDatabase();

	// we can modify only items with blueprints
	// for this, firstly we iterate thru inventory and look for blueprints
	// then for each blueprint we trying to find item group of with 'blueprint
	// result' type finally, we copying all items from such group (if we have one)
	// to mod list
	mPossibleModItems.clear();
	for (auto &group : inv->GetGroups())
	{
		auto &desc = db->Get(group->mType);
		// if this is blueprint
		if (desc.mClass == ItemClass::Blueprint)
		{
			// check what it gives
			auto &resultDesc = db->Get(desc.mBlueprintResultItem);
			// we can modify only these types of items (probably I will add new
			// classes in the future)
			if (resultDesc.mClass == ItemClass::Weapon || resultDesc.mClass == ItemClass::Armor ||
				resultDesc.mClass == ItemClass::Gloves || resultDesc.mClass == ItemClass::Helmet ||
				resultDesc.mClass == ItemClass::Trousers || resultDesc.mClass == ItemClass::Shield)
			{
				// try to find group
				auto modGroup = inv->FindGroup(desc.mBlueprintResultItem);
				if (modGroup)
				{
					// copy all items with such type to mod items
					for (auto &item : modGroup->mItems)
					{
						mPossibleModItems.push_back(item);
					}
				}
			}
		}
	}

	mModifySelectorItems->Clear();
	for (auto &modItem : mPossibleModItems)
	{
		auto &desc = modItem->GetDefinition();
		int row = mModifySelectorItems->AddRow();
		dynamic_pointer_cast<Label>(mModifySelectorItems->Get(0, row))->SetText(desc.GetTranslatedName());
	}
}

void Bench::SyncModify()
{
	THROW_IF_NULL(mModifiableItem);

	auto &desc = mModifiableItem->GetDefinition();

	auto fmt = StringBuilder();

	fmt << ColorToStr({ 0, 162, 232 }) << "*** " << game->GetStr("modifyingItem") << " ***"
		<< ColorToStr({ 255, 255, 255 }) << LineEnd;
	fmt << desc.GetDescription("", -1,
		IDFF_EXCLUDE_COUNT | IDFF_EXCLUDE_TOTAL_WEIGHT | IDFF_EXCLUDE_DAMAGE |
		IDFF_EXCLUDE_DESCRIPTION)
		<< LineEnd;
	fmt << ColorToStr({ 0, 162, 232 }) << "*** " << game->GetStr("appliableModifications") << " ***"
		<< ColorToStr({ 255, 255, 255 });
	mModifiableItemInfo->SetText(fmt);

	if (desc.mClass == ItemClass::Weapon)
	{
		mModDamageValueLbl->SetText(
			RoundAndConvert(mModifiableItem->GetModifiedValue(ItemModifierType::Damage), 1) + " + ");
		mModDamageNud->SetEnabled(true);

		mModShootPeriodValueLbl->SetText(
			RoundAndConvert(mModifiableItem->GetModifiedValue(ItemModifierType::ShootPeriod), 1) +
			" + ");
		mModShootPeriodNud->SetEnabled(true);

		mModShootDistanceValueLbl->SetText(
			RoundAndConvert(mModifiableItem->GetModifiedValue(ItemModifierType::ShootDistance), 1) +
			" + ");
		mModShootDistanceNud->SetEnabled(true);

		// disable not appliable values
		mModStabilityNud->SetEnabled(false);
		mModStabilityValueLbl->SetText(game->GetStr("notAppliable"));
		mModPhysicalDamageReductionNud->SetEnabled(false);
		mModPhysicalDamageReductionValueLbl->SetText(game->GetStr("notAppliable"));
		mModChemicalDamageReductionNud->SetEnabled(false);
		mModChemicalDamageReductionValueLbl->SetText(game->GetStr("notAppliable"));
		mModFireDamageReductionNud->SetEnabled(false);
		mModFireDamageReductionValueLbl->SetText(game->GetStr("notAppliable"));
	}
	else if (desc.mClass == ItemClass::Armor || desc.mClass == ItemClass::Gloves ||
		desc.mClass == ItemClass::Helmet || desc.mClass == ItemClass::Trousers ||
		desc.mClass == ItemClass::Shield)
	{
		mModPhysicalDamageReductionValueLbl->SetText(
			RoundAndConvert(
				mModifiableItem->GetModifiedValue(ItemModifierType::PhysicalDamageReduction), 1) +
			" + ");
		mModPhysicalDamageReductionNud->SetEnabled(true);

		mModChemicalDamageReductionValueLbl->SetText(
			RoundAndConvert(
				mModifiableItem->GetModifiedValue(ItemModifierType::ChemicalDamageReduction), 1) +
			" + ");
		mModChemicalDamageReductionNud->SetEnabled(true);

		mModFireDamageReductionValueLbl->SetText(
			RoundAndConvert(
				mModifiableItem->GetModifiedValue(ItemModifierType::FireDamageReduction), 1) +
			" + ");
		mModFireDamageReductionNud->SetEnabled(true);

		if (desc.mClass == ItemClass::Shield)
		{
			mModStabilityValueLbl->SetText(
				RoundAndConvert(mModifiableItem->GetModifiedValue(ItemModifierType::Stability), 1) +
				" + ");
			mModStabilityNud->SetEnabled(true);
		}
		else
		{
			mModStabilityNud->SetEnabled(false);
		}

		// disable not appliable values
		mModDamageNud->SetEnabled(false);
		mModDamageValueLbl->SetText(game->GetStr("notAppliable"));
		mModShootPeriodNud->SetEnabled(false);
		mModShootPeriodValueLbl->SetText(game->GetStr("notAppliable"));
		mModShootDistanceNud->SetEnabled(false);
		mModShootDistanceValueLbl->SetText(game->GetStr("notAppliable"));
	}

	// common
	mModDurabilityValueLbl->SetText(
		RoundAndConvert(mModifiableItem->GetModifiedValue(ItemModifierType::Durability), 1) + " + ");
}

void Bench::SyncDecompose()
{
	auto inv = game->GetLevel()->GetPlayer()->GetInventory();
	mDecomposeItems->Clear();
	for (auto &group : inv->GetGroups())
	{
		auto &desc = group->mItems.at(0)->GetDefinition();
		if (desc.mDecomposeItems.size())
		{
			mDecomposableItems.push_back(group);
			int row = mDecomposeItems->AddRow();
			dynamic_pointer_cast<Label>(mDecomposeItems->Get(0, row))
				->SetText(StringBuilder("x") << group->Count() << " - " << desc.GetTranslatedName());
		}
	}
}

void Bench::ShowCraftTooltip(ItemType type, const shared_ptr<Inventory> &inv)
{
	const sf::Color lightBlue = { 0, 162, 232 };
	const sf::Color white = { 255, 255, 255 };
	const sf::Color orange = { 255, 127, 39 };
	const sf::Color green = { 0, 255, 0 };
	ItemDatabase* db = game->GetItemDatabase();
	auto &stats = inv->GetOwner().lock()->GetStats();
	auto &blueprintDesc = db->Get(type);
	if (blueprintDesc.mClass == ItemClass::Blueprint)
	{
		auto &craftResultDesc = db->Get(blueprintDesc.mBlueprintResultItem);
		auto fmt = StringBuilder();
		fmt << blueprintDesc.GetDescription("", -1,
			IDFF_EXCLUDE_COUNT | IDFF_EXCLUDE_WEIGHT | IDFF_EXCLUDE_TOTAL_WEIGHT |
			IDFF_EXCLUDE_DESCRIPTION);
		fmt << ColorToStr(lightBlue) << "*** " << game->GetStr("willBeCreated") << " ***"
			<< ColorToStr(white) << LineEnd;
		fmt << craftResultDesc.GetDescription(
			"", -1, IDFF_EXCLUDE_COUNT | IDFF_EXCLUDE_DESCRIPTION | IDFF_EXCLUDE_TOTAL_WEIGHT);
		fmt << ColorToStr(lightBlue) << "*** " << game->GetStr("needResources") << " ***"
			<< ColorToStr(white) << LineEnd;
		unordered_map<ItemType, int> components;
		for (auto &comp : craftResultDesc.mComposeItems)
		{
			components[comp]++;
		}
		for (auto &kv : components)
		{
			auto group = inv->FindGroup(kv.first);
			auto &desc = db->Get(kv.first);
			int count = (group ? group->Count() : 0);
			fmt << ColorToStr(white) << desc.GetTranslatedName() << " - ";
			if (count < kv.second)
			{
				fmt << ColorToStr(orange);
			}
			else
			{
				fmt << ColorToStr(green);
			}
			fmt << game->GetStr("needItems") << ": " << kv.second << "; " << game->GetStr("gotItems")
				<< ": " << count << LineEnd;
		}
		fmt << game->GetStr("needIntelligence") << ": ";
		if (craftResultDesc.mRequiredIntelligence < stats->Intelligence.Get())
		{
			fmt << ColorToStr(green);
		}
		else
		{
			fmt << ColorToStr(orange);
		}
		fmt << craftResultDesc.mRequiredIntelligence << "; " << game->GetStr("youHaveSkill") << ": "
			<< stats->Intelligence.Get() << LineEnd;

		mTooltip->Show(fmt);
	}
	else
	{
		// should there be thrown an exception?
	}
}

void Bench::ShowDecomposeTooltip(ItemType type)
{
	const sf::Color lightBlue = { 0, 162, 232 };
	const sf::Color white = { 255, 255, 255 };
	ItemDatabase* db = game->GetItemDatabase();
	auto &decomposableDesc = db->Get(type);
	auto fmt = StringBuilder();
	fmt << ColorToStr(lightBlue) << "*** " << game->GetStr("willBeDecomposed") << " ***"
		<< ColorToStr(white) << LineEnd;
	fmt << decomposableDesc.GetDescription(
		"", -1, IDFF_EXCLUDE_COUNT | IDFF_EXCLUDE_DESCRIPTION | IDFF_EXCLUDE_TOTAL_WEIGHT);
	fmt << ColorToStr(lightBlue) << "*** " << game->GetStr("decomposeComponents") << " ***"
		<< ColorToStr(white) << LineEnd;
	unordered_map<ItemType, int> components;
	for (auto &comp : decomposableDesc.mDecomposeItems)
	{
		components[comp]++;
	}
	for (auto &kv : components)
	{
		fmt << "x" << kv.second << " - " << db->Get(kv.first).GetTranslatedName() << LineEnd;
	}
	mTooltip->Show(fmt);
}

void Bench::CraftSelectedRowClicked()
{
	if (mCraftItems->GetSelectedRowIndex() >= 0)
	{
		auto player = game->GetLevel()->GetPlayer();
		auto inv = player->GetInventory();
		auto &stats = player->GetStats();
		auto &blueprintGroup = mBlueprints.at(mCraftItems->GetSelectedRowIndex());
		ItemDatabase* db = game->GetItemDatabase();
		auto &blueprintDesc = db->Get(blueprintGroup->mType);
		if (blueprintDesc.mClass == ItemClass::Blueprint)
		{
			mCraftableItem = blueprintDesc.mBlueprintResultItem;
			auto &craftResultDesc = db->Get(mCraftableItem);
			mCraftComponents.clear();
			for (auto &comp : craftResultDesc.mComposeItems)
			{
				mCraftComponents[comp]++;
			}
			bool enoughComponents = true;
			for (auto &kv : mCraftComponents)
			{
				auto group = inv->FindGroup(kv.first);
				int count = (group ? group->Count() : 0);
				if (count < kv.second)
				{
					enoughComponents = false;
					break;
				}
			}
			bool skillMatch = craftResultDesc.mRequiredIntelligence <= stats->Intelligence.Get();
			mCraftSelected->SetEnabled(enoughComponents && skillMatch);
			mCraftSelected->OnHover.Clear();
			if (!enoughComponents || !skillMatch)
			{
				auto fmt = StringBuilder();
				if (!enoughComponents)
				{
					fmt << game->GetStr("notEnoughComponents") << LineEnd;
				}
				if (!skillMatch)
				{
					fmt << game->GetStr("skillDoesntMatch") << LineEnd;
				}
				string str = fmt;
				mCraftSelected->OnHover += [this, str] { mTooltip->Show(str); };
			}
		}
	}
}

void Bench::CraftSelectedClicked()
{
	ItemDatabase* db = game->GetItemDatabase();
	auto player = game->GetLevel()->GetPlayer();
	auto inv = player->GetInventory();

	inv->AddItem(db->Make(mCraftableItem, player->GetLocation()));

	// remove components from inventory
	for (auto &kv : mCraftComponents)
	{
		inv->FindGroup(kv.first)->RemoveCount(kv.second);
	}
}

void Bench::DecomposeItemsRowClicked()
{
	ItemDatabase* db = game->GetItemDatabase();
	mItemPartsAfterDecomposition.clear();
	mItemWillBeDecomposed =
		mDecomposableItems.at(mDecomposeItems->GetSelectedRowIndex())->mItems.at(0);
	auto &decomposableDesc = db->Get(mItemWillBeDecomposed->GetType());
	for (auto &comp : decomposableDesc.mDecomposeItems)
	{
		mItemPartsAfterDecomposition[comp]++;
	}
}

void Bench::ShowRepairTooltip(const shared_ptr<Item> &item)
{
	ItemDatabase* db = game->GetItemDatabase();
	auto &desc = item->GetDefinition();
	auto fmt = StringBuilder();

	unordered_map<ItemType, int> needComponents, playerStuff;
	ItemDamageClass dmgClass = GetItemRepairComponents(item, needComponents);
	CountNeededItems(needComponents, playerStuff);
	fmt << desc.GetTranslatedName() << LineEnd;
	fmt << game->GetStr("itemDamageLevel") << ": ";
	if (dmgClass == ItemDamageClass::Low)
	{
		fmt << ColorToStr({ 0, 255, 0 });
	}
	else if (dmgClass == ItemDamageClass::Medium)
	{
		fmt << ColorToStr({ 255, 127, 39 });
	}
	else if (dmgClass == ItemDamageClass::High)
	{
		fmt << ColorToStr({ 255, 0, 0 });
	}
	fmt << RoundAndConvert(item->GetDamageLevel(), 1) << "%";
	if (dmgClass == ItemDamageClass::Low)
	{
		fmt << " (" << game->GetStr("lowItemDamage") << ")";
	}
	else if (dmgClass == ItemDamageClass::Medium)
	{
		fmt << " (" << game->GetStr("mediumItemDamage") << ")";
	}
	else if (dmgClass == ItemDamageClass::High)
	{
		fmt << " (" << game->GetStr("highItemDamage") << ")";
	}
	fmt << LineEnd;
	fmt << ColorToStr({ 0, 162, 232 }) << "*** " << game->GetStr("itemRepairComponents") << " ***"
		<< LineEnd;
	for (auto &kv : needComponents)
	{
		fmt << ColorToStr({ 255, 255, 255 }) << db->Get(kv.first).GetTranslatedName() << " - "
			<< game->GetStr("needItems") << ": ";
		if (kv.second > playerStuff[kv.first])
		{
			fmt << ColorToStr({ 255, 127, 39 });
		}
		else
		{
			fmt << ColorToStr({ 0, 255, 0 });
		}
		fmt << kv.second << "; " << game->GetStr("gotItems") << ": " << playerStuff[kv.first]
			<< LineEnd;
	}
	mTooltip->Show(fmt);
}

ItemDamageClass Bench::GetItemRepairComponents(const shared_ptr<Item> & item, unordered_map<ItemType, int>& components)
{
	auto &desc = item->GetDefinition();
	auto dmg = item->GetDamageLevel();
	ItemDamageClass dmgClass = ItemDamageClass::Low;
	if (dmg > 33.0f && dmg < 66.0f)
	{
		dmgClass = ItemDamageClass::Medium;
	}
	else if (dmg > 66.0f)
	{
		dmgClass = ItemDamageClass::High;
	}
	components.clear();
	for (int i = 0; i <= static_cast<int>(dmgClass); ++i)
	{
		if (!desc.mRepairItems.empty())
		{
			const auto &comps = desc.mRepairItems.at(static_cast<ItemDamageClass::Type>(i));
			for (auto &c : comps)
			{
				components[c]++;
			}
		}
	}
	return dmgClass;
}

bool Bench::CountNeededItems(
	const unordered_map<ItemType, int> &neededItems, unordered_map<ItemType, int> &whatGot)
{
	whatGot.clear();
	auto inv = game->GetLevel()->GetPlayer()->GetInventory();
	bool enoughComponents = true;
	for (auto &kv : neededItems)
	{
		auto group = inv->FindGroup(kv.first);
		if (!group)
		{
			whatGot[kv.first] = 0;
			enoughComponents = false;
			break;
		}
		else
		{
			whatGot[kv.first] = group->Count();
			if (group->Count() < kv.second)
			{
				enoughComponents = false;
				break;
			}
		}
	}
	return enoughComponents;
}

void Bench::RepairItemsOnRowClicked()
{
	auto inv = game->GetLevel()->GetPlayer()->GetInventory();
	mItemForRepair = mDamagedItems.at(mRepairItems->GetSelectedRowIndex());
	GetItemRepairComponents(mItemForRepair, mComponentsForRepair);
	bool enoughComponents = true;
	for (auto &kv : mComponentsForRepair)
	{
		auto group = inv->FindGroup(kv.first);
		if (!group)
		{
			enoughComponents = false;
			break;
		}
		else
		{
			if (group->Count() < kv.second)
			{
				enoughComponents = false;
				break;
			}
		}
	}
	mRepairSelected->OnHover.Clear();
	mRepairSelected->SetEnabled(enoughComponents);
	if (!enoughComponents)
	{
		mRepairSelected->OnHover +=
			[this] { mTooltip->Show(game->GetStr("notEnoughComponentsRepair")); };
	}
}

void Bench::RepairSelectedClicked()
{
	auto inv = game->GetLevel()->GetPlayer()->GetInventory();
	for (auto &kv : mComponentsForRepair)
	{
		auto group = inv->FindGroup(kv.first);
		if (group)
		{
			group->RemoveCount(kv.second);
		}
	}
	mItemForRepair->Repair();
	mComponentsForRepair.clear();
	mItemForRepair.reset();
	mRepairSelected->SetEnabled(false);
}

Bench::Bench() : mPage(Page::Selector)
{
	mCanvas = game->CreateCanvas();

	// selection window
	{
		mSelectionWindow = mCanvas->Create<Window>();
		mSelectionWindow->SetSize({ 450, 220 });
		mSelectionWindow->SetInCenter();
		mSelectionWindow->SetCaption(game->GetStr("bench"));
		mSelectionWindow->SetAutoLayoutingEnabled(false);

		Layout layout({ 10, 50 }, 430, 2, 5);

		mCraft = mCanvas->Create<Button>();
		mCraft->SetSize({ 0, 60 });
		mCraft->SetText(game->GetStr("craft"));
		mCraft->OnClick += [this] { SetPage(Page::Craft); };
		mCraft->OnHover += [this] { mTooltip->Show(game->GetStr("craftTooltip")); };
		mSelectionWindow->AddChild(mCraft);
		layout.Pack(mCraft);

		mRepair = mCanvas->Create<Button>();
		mRepair->SetSize({ 0, 60 });
		mRepair->SetText(game->GetStr("repair"));
		mRepair->OnClick += [this] { SetPage(Page::Repair); };
		mRepair->OnHover += [this] { mTooltip->Show(game->GetStr("repairTooltip")); };
		mSelectionWindow->AddChild(mRepair);
		layout.Pack(mRepair);

		mModify = mCanvas->Create<Button>();
		mModify->SetSize({ 0, 60 });
		mModify->SetText(game->GetStr("modify"));
		mModify->OnHover += [this] { mTooltip->Show(game->GetStr("modifyTooltip")); };
		mModify->OnClick += [this] { SetPage(Page::ModifySelector); };
		mSelectionWindow->AddChild(mModify);
		layout.Pack(mModify);

		mDecompose = mCanvas->Create<Button>();
		mDecompose->SetSize({ 0, 60 });
		mDecompose->SetText(game->GetStr("decompose"));
		mDecompose->OnHover += [this] { mTooltip->Show(game->GetStr("decomposeTooltip")); };
		mDecompose->OnClick += [this] { SetPage(Page::Decompose); };
		mSelectionWindow->AddChild(mDecompose);
		layout.Pack(mDecompose);

		mClose = mCanvas->Create<Button>();
		mClose->SetSize({ 0, 30 });
		mClose->SetText(game->GetStr("leaveBench"));
		mClose->OnClick += [this] { SetVisible(false); };
		layout.ConfigureRow(Layout::TemplateRow, 1);
		mSelectionWindow->AddChild(mClose);
		layout.Pack(mClose);
	}

	// craft window
	{
		mCraftWindow = mCanvas->Create<Window>();
		mCraftWindow->SetSize({ 450, 400 });
		mCraftWindow->SetInCenter();
		mCraftWindow->SetCaption(game->GetStr("craftWnd"));
		mCraftWindow->SetAutoLayoutingEnabled(false);

		Layout layout({ 10, 50 }, 430, 1, 5);

		mCraftItems = mCanvas->Create<Table>();
		mCraftItems->SetSize({ 0, 290 });
		mCraftItems->AddColumn(game->GetStr("blueprints"));
		mCraftItems->OnRowHovered += [this]
		{
			if (mCraftItems->GetHoveredRowIndex() >= 0)
			{
				auto &group = mBlueprints.at(mCraftItems->GetHoveredRowIndex());
				ShowCraftTooltip(group->mType, game->GetLevel()->GetPlayer()->GetInventory());
			}
		};
		mCraftItems->OnRowClicked += [this] { CraftSelectedRowClicked(); };
		mCraftWindow->AddChild(mCraftItems);
		layout.Pack(mCraftItems);

		layout.ConfigureRow(Layout::TemplateRow, 2);

		mCraftSelected = mCanvas->Create<Button>();
		mCraftSelected->SetText(game->GetStr("craftSelected"));
		mCraftSelected->SetSize({ 0, 30 });
		mCraftSelected->SetEnabled(false);
		mCraftSelected->OnClick += [this] { CraftSelectedClicked(); };
		mCraftWindow->AddChild(mCraftSelected);
		layout.Pack(mCraftSelected);

		mCraftBack = mCanvas->Create<Button>();
		mCraftBack->SetText(game->GetStr("back"));
		mCraftBack->SetSize({ 0, 30 });
		mCraftBack->OnClick += [this] { SetPage(Page::Selector); };
		mCraftWindow->AddChild(mCraftBack);
		layout.Pack(mCraftBack);
	}

	// repair window
	{
		mRepairWindow = mCanvas->Create<Window>();
		mRepairWindow->SetSize({ 450, 400 });
		mRepairWindow->SetInCenter();
		mRepairWindow->SetCaption(game->GetStr("repairWnd"));
		mRepairWindow->SetAutoLayoutingEnabled(false);

		Layout layout({ 10, 50 }, 430, 1, 5);

		mRepairItems = mCanvas->Create<Table>();
		mRepairItems->SetSize({ 0, 290 });
		mRepairItems->AddColumn(game->GetStr("needToBeRepaired"));
		mRepairItems->OnRowHovered += [this]
		{
			if (mRepairItems->GetHoveredRowIndex() >= 0)
			{
				ShowRepairTooltip(mDamagedItems.at(mRepairItems->GetHoveredRowIndex()));
			}
		};
		mRepairItems->OnRowClicked += [this] { RepairItemsOnRowClicked(); };
		mRepairWindow->AddChild(mRepairItems);
		layout.Pack(mRepairItems);

		layout.ConfigureRow(Layout::TemplateRow, 3);

		mRepairSelected = mCanvas->Create<Button>();
		mRepairSelected->SetText(game->GetStr("repairSelected"));
		mRepairSelected->SetSize({ 0, 30 });
		mRepairSelected->SetEnabled(false);
		mRepairSelected->OnClick += [this] { RepairSelectedClicked(); };
		mRepairWindow->AddChild(mRepairSelected);
		layout.Pack(mRepairSelected);

		mRepairAll = mCanvas->Create<Button>();
		mRepairAll->SetText(game->GetStr("repairAll"));
		mRepairAll->SetSize({ 0, 30 });
		mRepairAll->SetEnabled(false);
		mRepairAll->OnHover += [this]
		{

		};
		mRepairAll->OnClick += [this]
		{

		};
		mRepairWindow->AddChild(mRepairAll);
		layout.Pack(mRepairAll);

		mRepairBack = mCanvas->Create<Button>();
		mRepairBack->SetText(game->GetStr("back"));
		mRepairBack->SetSize({ 0, 30 });
		mRepairBack->OnClick += [this] { SetPage(Page::Selector); };
		mRepairWindow->AddChild(mRepairBack);
		layout.Pack(mRepairBack);
	}

	// modify selector window
	{
		mModifySelectorWindow = mCanvas->Create<Window>();
		mModifySelectorWindow->SetSize({ 450, 400 });
		mModifySelectorWindow->SetInCenter();
		mModifySelectorWindow->SetCaption(game->GetStr("modifyWnd"));
		mModifySelectorWindow->SetAutoLayoutingEnabled(false);

		Layout layout({ 10, 50 }, 430, 1, 5);

		mModifySelectorItems = mCanvas->Create<Table>();
		mModifySelectorItems->SetSize({ 0, 290 });
		mModifySelectorItems->AddColumn(game->GetStr("canBeModified"));
		mModifySelectorItems->OnRowHovered += [this]
		{

		};
		mModifySelectorItems->OnRowClicked += [this]
		{
			if (mModifySelectorItems->GetSelectedRowIndex() >= 0)
			{
				mModifiableItem = mPossibleModItems.at(mModifySelectorItems->GetSelectedRowIndex());
				mModifySelectorSelection->SetEnabled(true);
			}
		};
		mModifySelectorWindow->AddChild(mModifySelectorItems);
		layout.Pack(mModifySelectorItems);

		layout.ConfigureRow(Layout::TemplateRow, 2);

		mModifySelectorSelection = mCanvas->Create<Button>();
		mModifySelectorSelection->SetText(game->GetStr("beginModification"));
		mModifySelectorSelection->SetSize({ 0, 40 });
		mModifySelectorSelection->SetEnabled(false);
		mModifySelectorSelection->OnClick += [this] { SetPage(Page::Modify); };
		mModifySelectorWindow->AddChild(mModifySelectorSelection);
		layout.Pack(mModifySelectorSelection);

		mModifySelectorBack = mCanvas->Create<Button>();
		mModifySelectorBack->SetText(game->GetStr("back"));
		mModifySelectorBack->SetSize({ 0, 40 });
		mModifySelectorBack->OnClick += [this] { SetPage(Page::Selector); };
		mModifySelectorWindow->AddChild(mModifySelectorBack);
		layout.Pack(mModifySelectorBack);
	}

	{
		mModifyWindow = mCanvas->Create<Window>();
		mModifyWindow->SetSize({ 600, 400 });
		mModifyWindow->SetInCenter();
		mModifyWindow->SetCaption(game->GetStr("modifyWnd"));

		mModifiableItemInfo = mCanvas->Create<Label>();
		mModifiableItemInfo->SetAlignment(TextAlignment::Center);
		mModifiableItemInfo->SetAutoHeight(true);
		mModifyWindow->AddChild(mModifiableItemInfo);

		mModifyWindow->GetLayout().ConfigureRow(Layout::TemplateRow, 3);

		mModDamageLbl = mCanvas->Create<Label>();
		mModDamageLbl->SetSize({ 0, 30 });
		mModDamageLbl->SetText(game->GetStr("modDamage"));
		mModifyWindow->AddChild(mModDamageLbl);
		mModDamageValueLbl = mCanvas->Create<Label>();
		mModDamageValueLbl->SetAlignment(TextAlignment::Center);
		mModDamageValueLbl->SetSize({ 0, 30 });
		mModifyWindow->AddChild(mModDamageValueLbl);
		mModDamageNud = mCanvas->Create<NumericUpDown>();
		mModDamageNud->SetSize({ 0, 30 });
		mModDamageNud->SetMinLimit(0);
		mModDamageNud->SetMaxLimit(Item::MaxDamageMod * 100.0f);
		mModDamageNud->SetStep(5);
		mModDamageNud->SetAdditionalText("%");
		mModDamageNud->OnChange += [this] { CalcModCostAndPrint(); };
		mModifyWindow->AddChild(mModDamageNud);

		mModPhysicalDamageReductionLbl = mCanvas->Create<Label>();
		mModPhysicalDamageReductionLbl->SetSize({ 0, 30 });
		mModPhysicalDamageReductionLbl->SetText(game->GetStr("modPhysicalDamageReduction"));
		mModifyWindow->AddChild(mModPhysicalDamageReductionLbl);
		mModPhysicalDamageReductionValueLbl = mCanvas->Create<Label>();
		mModPhysicalDamageReductionValueLbl->SetAlignment(TextAlignment::Center);
		mModPhysicalDamageReductionValueLbl->SetSize({ 0, 30 });
		mModifyWindow->AddChild(mModPhysicalDamageReductionValueLbl);
		mModPhysicalDamageReductionNud = mCanvas->Create<NumericUpDown>();
		mModPhysicalDamageReductionNud->SetSize({ 0, 30 });
		mModPhysicalDamageReductionNud->SetMinLimit(0);
		mModPhysicalDamageReductionNud->SetMaxLimit(Item::MaxPhysicalDamageReductionMod * 100.0f);
		mModPhysicalDamageReductionNud->SetStep(1);
		mModPhysicalDamageReductionNud->SetAdditionalText("%");
		mModPhysicalDamageReductionNud->OnChange += [this] { CalcModCostAndPrint(); };
		mModifyWindow->AddChild(mModPhysicalDamageReductionNud);

		mModChemicalDamageReductionLbl = mCanvas->Create<Label>();
		mModChemicalDamageReductionLbl->SetSize({ 0, 30 });
		mModChemicalDamageReductionLbl->SetText(game->GetStr("modChemicalDamageReduction"));
		mModifyWindow->AddChild(mModChemicalDamageReductionLbl);
		mModChemicalDamageReductionValueLbl = mCanvas->Create<Label>();
		mModChemicalDamageReductionValueLbl->SetAlignment(TextAlignment::Center);
		mModChemicalDamageReductionValueLbl->SetSize({ 0, 30 });
		mModifyWindow->AddChild(mModChemicalDamageReductionValueLbl);
		mModChemicalDamageReductionNud = mCanvas->Create<NumericUpDown>();
		mModChemicalDamageReductionNud->SetSize({ 0, 30 });
		mModChemicalDamageReductionNud->SetMinLimit(0);
		mModChemicalDamageReductionNud->SetMaxLimit(Item::MaxChemicalDamageReductionMod * 100.0f);
		mModChemicalDamageReductionNud->SetStep(1);
		mModChemicalDamageReductionNud->SetAdditionalText("%");
		mModChemicalDamageReductionNud->OnChange += [this] { CalcModCostAndPrint(); };
		mModifyWindow->AddChild(mModChemicalDamageReductionNud);

		mModFireDamageReductionLbl = mCanvas->Create<Label>();
		mModFireDamageReductionLbl->SetSize({ 0, 30 });
		mModFireDamageReductionLbl->SetText(game->GetStr("modFireDamageReduction"));
		mModifyWindow->AddChild(mModFireDamageReductionLbl);
		mModFireDamageReductionValueLbl = mCanvas->Create<Label>();
		mModFireDamageReductionValueLbl->SetAlignment(TextAlignment::Center);
		mModFireDamageReductionValueLbl->SetSize({ 0, 30 });
		mModifyWindow->AddChild(mModFireDamageReductionValueLbl);
		mModFireDamageReductionNud = mCanvas->Create<NumericUpDown>();
		mModFireDamageReductionNud->SetSize({ 0, 30 });
		mModFireDamageReductionNud->SetMinLimit(0);
		mModFireDamageReductionNud->SetMaxLimit(Item::MaxFireDamageReductionMod * 100.0f);
		mModFireDamageReductionNud->SetStep(1);
		mModFireDamageReductionNud->SetAdditionalText("%");
		mModFireDamageReductionNud->OnChange += [this] { CalcModCostAndPrint(); };
		mModifyWindow->AddChild(mModFireDamageReductionNud);

		mModDurabilityLbl = mCanvas->Create<Label>();
		mModDurabilityLbl->SetSize({ 0, 30 });
		mModDurabilityLbl->SetText(game->GetStr("modDurability"));
		mModifyWindow->AddChild(mModDurabilityLbl);
		mModDurabilityValueLbl = mCanvas->Create<Label>();
		mModDurabilityValueLbl->SetAlignment(TextAlignment::Center);
		mModDurabilityValueLbl->SetSize({ 0, 30 });
		mModifyWindow->AddChild(mModDurabilityValueLbl);
		mModDurabilityNud = mCanvas->Create<NumericUpDown>();
		mModDurabilityNud->SetSize({ 0, 30 });
		mModDurabilityNud->SetMinLimit(0);
		mModDurabilityNud->SetMaxLimit(Item::MaxDurabilityMod * 100.0f);
		mModDurabilityNud->SetStep(1);
		mModDurabilityNud->SetAdditionalText("%");
		mModDurabilityNud->OnChange += [this] { CalcModCostAndPrint(); };
		mModifyWindow->AddChild(mModDurabilityNud);

		mModStabilityLbl = mCanvas->Create<Label>();
		mModStabilityLbl->SetSize({ 0, 30 });
		mModStabilityLbl->SetText(game->GetStr("modStability"));
		mModifyWindow->AddChild(mModStabilityLbl);
		mModStabilityValueLbl = mCanvas->Create<Label>();
		mModStabilityValueLbl->SetAlignment(TextAlignment::Center);
		mModStabilityValueLbl->SetSize({ 0, 30 });
		mModifyWindow->AddChild(mModStabilityValueLbl);
		mModStabilityNud = mCanvas->Create<NumericUpDown>();
		mModStabilityNud->SetSize({ 0, 30 });
		mModStabilityNud->SetMinLimit(0);
		mModStabilityNud->SetMaxLimit(Item::MaxStabilityMod * 100.0f);
		mModStabilityNud->SetStep(1);
		mModStabilityNud->SetAdditionalText("%");
		mModStabilityNud->OnChange += [this] { CalcModCostAndPrint(); };
		mModifyWindow->AddChild(mModStabilityNud);

		mModShootPeriodLbl = mCanvas->Create<Label>();
		mModShootPeriodLbl->SetSize({ 0, 30 });
		mModShootPeriodLbl->SetText(game->GetStr("modShootPeriod"));
		mModifyWindow->AddChild(mModShootPeriodLbl);
		mModShootPeriodValueLbl = mCanvas->Create<Label>();
		mModShootPeriodValueLbl->SetAlignment(TextAlignment::Center);
		mModShootPeriodValueLbl->SetSize({ 0, 30 });
		mModifyWindow->AddChild(mModShootPeriodValueLbl);
		mModShootPeriodNud = mCanvas->Create<NumericUpDown>();
		mModShootPeriodNud->SetSize({ 0, 30 });
		mModShootPeriodNud->SetMinLimit(0);
		mModShootPeriodNud->SetMaxLimit(Item::MaxShootPeriodMod * 100.0f);
		mModShootPeriodNud->SetStep(1);
		mModShootPeriodNud->SetAdditionalText("%");
		mModShootPeriodNud->OnChange += [this] { CalcModCostAndPrint(); };
		mModifyWindow->AddChild(mModShootPeriodNud);

		mModShootDistanceLbl = mCanvas->Create<Label>();
		mModShootDistanceLbl->SetSize({ 0, 30 });
		mModShootDistanceLbl->SetText(game->GetStr("modShootDistance"));
		mModifyWindow->AddChild(mModShootDistanceLbl);
		mModShootDistanceValueLbl = mCanvas->Create<Label>();
		mModShootDistanceValueLbl->SetAlignment(TextAlignment::Center);
		mModShootDistanceValueLbl->SetSize({ 0, 30 });
		mModifyWindow->AddChild(mModShootDistanceValueLbl);
		mModShootDistanceNud = mCanvas->Create<NumericUpDown>();
		mModShootDistanceNud->SetSize({ 0, 30 });
		mModShootDistanceNud->SetMinLimit(0);
		mModShootDistanceNud->SetMaxLimit(Item::MaxShootDistanceMod * 100.0f);
		mModShootDistanceNud->SetStep(1);
		mModShootDistanceNud->SetAdditionalText("%");
		mModShootDistanceNud->OnChange += [this] { CalcModCostAndPrint(); };
		mModifyWindow->AddChild(mModShootDistanceNud);

		mModifyWindow->GetLayout().ConfigureRow(Layout::TemplateRow, 1);

		mModCost = mCanvas->Create<Label>();
		mModCost->SetAlignment(TextAlignment::CenterTop);
		mModCost->SetAutoHeight(true);
		mModifyWindow->AddChild(mModCost);

		mModifyWindow->GetLayout().ConfigureRow(Layout::TemplateRow, 2);

		mConfirmModification = mCanvas->Create<Button>();
		mConfirmModification->SetText(game->GetStr("confirmModification"));
		mConfirmModification->SetSize({ 0, 40 });
		mConfirmModification->OnClick += [this] { SetPage(Page::ModifySelector); };
		mModifyWindow->AddChild(mConfirmModification);

		mModifyBack = mCanvas->Create<Button>();
		mModifyBack->SetText(game->GetStr("back"));
		mModifyBack->SetSize({ 0, 40 });
		mModifyBack->OnClick += [this] { SetPage(Page::ModifySelector); };
		mModifyWindow->AddChild(mModifyBack);
	}

	// decompose window
	{
		mDecomposeWindow = mCanvas->Create<Window>();
		mDecomposeWindow->SetSize({ 450, 400 });
		mDecomposeWindow->SetInCenter();
		mDecomposeWindow->SetCaption(game->GetStr("decomposeWnd"));
		mDecomposeWindow->SetAutoLayoutingEnabled(false);

		Layout layout({ 10, 50 }, 430, 1, 5);

		mDecomposeItems = mCanvas->Create<Table>();
		mDecomposeItems->SetSize({ 0, 290 });
		mDecomposeItems->AddColumn(game->GetStr("canBeDecomposed"));
		mDecomposeItems->OnRowHovered += [this]
		{
			if (mDecomposeItems->GetHoveredRowIndex() >= 0)
			{
				auto &group = mDecomposableItems.at(mDecomposeItems->GetHoveredRowIndex());
				ShowDecomposeTooltip(group->mType);
			}
		};
		mDecomposeItems->OnRowClicked += [this]
		{
			mDecomposeSelected->SetEnabled(true);
			DecomposeItemsRowClicked();
		};
		mDecomposeWindow->AddChild(mDecomposeItems);
		layout.Pack(mDecomposeItems);

		layout.ConfigureRow(Layout::TemplateRow, 2);

		mDecomposeSelected = mCanvas->Create<Button>();
		mDecomposeSelected->SetText(game->GetStr("decomposeSelected"));
		mDecomposeSelected->SetSize({ 0, 30 });
		mDecomposeSelected->OnClick += [this]
		{
			auto player = game->GetLevel()->GetPlayer();
			auto inv = player->GetInventory();
			for (auto &kv : mItemPartsAfterDecomposition)
			{
				for (int n = 0; n < kv.second; ++n)
				{
					inv->AddItem(game->GetItemDatabase()->Make(kv.first, player->GetLocation()));
				}
			}
			mItemPartsAfterDecomposition.clear();
			inv->RemoveItem(mItemWillBeDecomposed);
			mItemWillBeDecomposed.reset();
			SyncDecompose();
			mDecomposeSelected->SetEnabled(false);
		};
		mDecomposeSelected->SetEnabled(false);
		mDecomposeWindow->AddChild(mDecomposeSelected);
		layout.Pack(mDecomposeSelected);

		mDecomposeBack = mCanvas->Create<Button>();
		mDecomposeBack->SetText(game->GetStr("back"));
		mDecomposeBack->SetSize({ 0, 30 });
		mDecomposeBack->OnClick += [this] { SetPage(Page::Selector); };
		mDecomposeWindow->AddChild(mDecomposeBack);
		layout.Pack(mDecomposeBack);
	}

	mTooltip = mCanvas->Create<Tooltip>();
	mTooltip->SetSize({ 450, 0 });

	SetPage(Page::Selector);
	mCanvas->SetVisible(false);

	Log() << "Bench successfully initialized!" << LineEnd;
}

Bench::~Bench()
{
}

void Bench::SetVisible(bool state)
{
	if (!game->GetLevel())
	{
		return;
	}
	if (!game->GetLevel()->GetPlayer())
	{
		return;
	}
	SetPage(Page::Selector);
	mCanvas->SetVisible(state);
}

bool Bench::IsVisible() const
{
	return mCanvas->IsVisible();
}

void Bench::Update()
{
	if (mPage == Page::Craft)
	{
	}
}
