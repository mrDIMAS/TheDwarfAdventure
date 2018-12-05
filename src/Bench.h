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

#include "ItemDatabase.h"

struct ItemGroup;

// Bench allows player to craft items, repair, decompose stuff, and modify
// weapons and armor
class Bench
{
private:
	enum class Page
	{
		Selector,
		Craft,
		Repair,
		Decompose,
		ModifySelector,
		Modify,
	};

	Page mPage;

	void SetPage(Page page);
	shared_ptr<Canvas> mCanvas;

	// selection window
	shared_ptr<Window> mSelectionWindow;
	shared_ptr<Button> mCraft;
	shared_ptr<Button> mRepair;
	shared_ptr<Button> mModify;
	shared_ptr<Button> mDecompose;

	// craft window
	shared_ptr<Window> mCraftWindow;
	shared_ptr<Table> mCraftItems;
	shared_ptr<Button> mCraftSelected;
	shared_ptr<Button> mCraftBack;
	vector<shared_ptr<ItemGroup>> mBlueprints;
	ItemType mCraftableItem { "Unknown" };
	unordered_map<ItemType, int> mCraftComponents;

	// repair window
	shared_ptr<Window> mRepairWindow;
	shared_ptr<Table> mRepairItems;
	shared_ptr<Button> mRepairSelected;
	shared_ptr<Button> mRepairAll;
	shared_ptr<Button> mRepairBack;
	unordered_map<ItemType, int> mComponentsForRepair;
	shared_ptr<Item> mItemForRepair;
	vector<shared_ptr<Item>> mDamagedItems;

	// modify selector window
	shared_ptr<Window> mModifySelectorWindow;
	shared_ptr<Table> mModifySelectorItems;
	shared_ptr<Button> mModifySelectorSelection;
	shared_ptr<Button> mModifySelectorBack;

	// modify window
	shared_ptr<Window> mModifyWindow;
	shared_ptr<Label> mModifiableItemInfo;
	shared_ptr<Label> mModDamageLbl;
	shared_ptr<Label> mModDamageValueLbl;
	shared_ptr<NumericUpDown> mModDamageNud;
	shared_ptr<Label> mModPhysicalDamageReductionLbl;
	shared_ptr<Label> mModPhysicalDamageReductionValueLbl;
	shared_ptr<NumericUpDown> mModPhysicalDamageReductionNud;
	shared_ptr<Label> mModChemicalDamageReductionLbl;
	shared_ptr<Label> mModChemicalDamageReductionValueLbl;
	shared_ptr<NumericUpDown> mModChemicalDamageReductionNud;
	shared_ptr<Label> mModFireDamageReductionLbl;
	shared_ptr<Label> mModFireDamageReductionValueLbl;
	shared_ptr<NumericUpDown> mModFireDamageReductionNud;
	shared_ptr<Label> mModDurabilityLbl;
	shared_ptr<Label> mModDurabilityValueLbl;
	shared_ptr<NumericUpDown> mModDurabilityNud;
	shared_ptr<Label> mModStabilityLbl;
	shared_ptr<Label> mModStabilityValueLbl;
	shared_ptr<NumericUpDown> mModStabilityNud;
	shared_ptr<Label> mModShootPeriodLbl;
	shared_ptr<Label> mModShootPeriodValueLbl;
	shared_ptr<NumericUpDown> mModShootPeriodNud;
	shared_ptr<Label> mModShootDistanceLbl;
	shared_ptr<Label> mModShootDistanceValueLbl;
	shared_ptr<NumericUpDown> mModShootDistanceNud;
	shared_ptr<Label> mModCost;
	shared_ptr<Button> mConfirmModification;
	shared_ptr<Button> mModifyBack;
	shared_ptr<Item> mModifiableItem;
	vector<shared_ptr<Item>> mPossibleModItems;
	void CalcModCostAndPrint();
	shared_ptr<Button> mClose;

	// decompose window
	shared_ptr<Window> mDecomposeWindow;
	shared_ptr<Table> mDecomposeItems;
	shared_ptr<Button> mDecomposeSelected;
	shared_ptr<Button> mDecomposeBack;
	vector<shared_ptr<ItemGroup>> mDecomposableItems;
	shared_ptr<Item> mItemWillBeDecomposed;
	unordered_map<ItemType, int> mItemPartsAfterDecomposition;

	shared_ptr<Tooltip> mTooltip;

	void SyncCraft();
	void SyncRepair();
	void SyncModifySelector();
	void SyncModify();
	void SyncDecompose();
	void ShowCraftTooltip(ItemType type, const shared_ptr<Inventory> &inv);
	void ShowDecomposeTooltip(ItemType type);
	void CraftSelectedRowClicked();
	void CraftSelectedClicked();
	void DecomposeItemsRowClicked();
	void ShowRepairTooltip(const shared_ptr<Item> &item);
	ItemDamageClass GetItemRepairComponents(
		const shared_ptr<Item> &item, unordered_map<ItemType, int> &components);
	bool CountNeededItems(
		const unordered_map<ItemType, int> &neededItems, unordered_map<ItemType, int> &whatGot);
	void RepairItemsOnRowClicked();
	void RepairSelectedClicked();

public:
	Bench();
	~Bench();
	// do nothing when called without generated level and if no player presented
	void SetVisible(bool state);
	bool IsVisible() const;
	void Update();
};
