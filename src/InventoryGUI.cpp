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
#include "InventoryGUI.h"
#include "Layout.h"
#include "Level.h"
#include "Player.h"
#include "Input.h"
#include "Item.h"
#include "Window.h"
#include "Image.h"
#include "NumericUpDown.h"
#include "Table.h"
#include "Tooltip.h"
#include "Frame.h"
#include "Inventory.h"
#include "SlideList.h"
#include "Slot.h"

void InventoryGUI::ShowTooltipForItem(const string &preText, const shared_ptr<Item> &item, int count)
{
	if (item)
	{
		auto fmt = StringBuilder();
		auto &desc = item->GetDefinition();
		fmt << desc.GetDescription(preText, count) << LineEnd;
		float dmg = item->GetDamageLevel();
		fmt << game->GetStr("itemDamageLevel") << ": ";
		if (dmg < 33.0f)
		{
			fmt << ColorToStr({ 0, 255, 0 });
		}
		else if (dmg > 33.0f && dmg < 66.0f)
		{
			fmt << ColorToStr({ 255, 127, 39 });
		}
		else if (dmg > 66.0f)
		{
			fmt << ColorToStr({ 255, 0, 0 });
		}
		fmt << dmg << "%" << LineEnd;
		if (item->IsModified())
		{
			item->EnlistModifications(fmt);
		}
		mTooltip->Show(fmt);
	}
}

void InventoryGUI::UnequipItemByClick(const shared_ptr<Item> &item)
{
	if (game->GetInput()->IsMouseButtonHit(sf::Mouse::Left))
	{
		game->GetLevel()->GetPlayer()->UnequipItem(item);
		Sync();
	}
}

void InventoryGUI::ShowSlotTooltip(
	const shared_ptr<Item> &item, const string &preText, const string &emptyText)
{
	if (item)
	{
		ShowTooltipForItem(preText, item, 1);
		UnequipItemByClick(item);
	}
	else
	{
		mTooltip->Show(emptyText);
	}
}

void InventoryGUI::SetSlotItem(const shared_ptr<Slot> &slot, const shared_ptr<Item> &item)
{
	slot->SetItem(item);
	if (item)
	{
		slot->SetImage(item->GetPictogram(), item->GetPictogramTexCoords());
	}
	else
	{
		slot->SetImage(nullptr);
	}
}

shared_ptr<Slot> InventoryGUI::MakeSlot(const Vec2f &size, const Vec2f &position)
{
	const auto slot = mCanvas->Create<Slot>();
	slot->SetSize(size);
	slot->SetPosition(position);
	mWindow->AddChild(slot);
	return slot;
}

InventoryGUI::InventoryGUI()
{
	mCanvas = game->CreateCanvas();

	mWindow = mCanvas->Create<Window>();
	mWindow->SetSize({ 700, 350 });
	mWindow->SetInCenter();
	mWindow->SetCaption(game->GetStr("inventory"));
	mWindow->SetAutoLayoutingEnabled(false);
	mWindow->AddHelpButton();
	mWindow->GetHelpButton()->OnHover += [this]
	{
		mTooltip->Show(game->GetStr("InventoryUI_Help"));
	};

	Layout leftGroup({ 10, 30 }, mWindow->GetSize().x - 220, 1, 5);

	/**
	 ** do not forget to add new filter element when adding new item class
	 **/
	pair<string, ItemClass> filterItems[] = {
		{ game->GetStr("all"), ItemClass::Unknown },
		{ game->GetStr("weapons"), ItemClass::Weapon },
		{ game->GetStr("shields"), ItemClass::Shield },
		{ game->GetStr("armors"), ItemClass::Armor },
		{ game->GetStr("helmets"), ItemClass::Helmet },
		{ game->GetStr("trousers"), ItemClass::Trousers },
		{ game->GetStr("gloves"), ItemClass::Gloves },
		{ game->GetStr("potions"), ItemClass::Potion },
		{ game->GetStr("rings"), ItemClass::Ring },
		{ game->GetStr("necklaces"), ItemClass::Necklace },
		{ game->GetStr("ore"), ItemClass::Ore },
		{ game->GetStr("metals"), ItemClass::Metal },
		{ game->GetStr("projectiles"), ItemClass::Projectile },
		{ game->GetStr("crystals"), ItemClass::Crystal },
		{ game->GetStr("blueprints"), ItemClass::Blueprint },
		{ game->GetStr("keys"), ItemClass::Key },
		{ game->GetStr("ingredient"), ItemClass::Ingredient },
		{ game->GetStr("toolsets"), ItemClass::Toolset },
		{ game->GetStr("books"), ItemClass::Book },
		{ game->GetStr("rubbish"), ItemClass::Rubbish },
	};

	mFilter = mCanvas->Create<SlideList>();
	mFilter->SetSize({ 0, 30 });
	for (auto item : filterItems)
	{
		mFilter->AddItem(
			StringBuilder() << game->GetStr("show") << item.first, make_shared<ItemClass>(item.second));
	}
	mFilter->SetCurrentItem(0);
	mFilter->OnCurrentItemChanged += [this] { Sync(); };
	mWindow->AddChild(mFilter);
	leftGroup.Pack(mFilter);

	mTable = mCanvas->Create<Table>();
	mTable->SetSize({ 0, 220 });
	mTable->AddColumn(game->GetStr("name"));
	mTable->OnRowHovered += [this]
	{
		THROW_IF_EXPIRED(mInventory);
		if (mTable->GetHoveredRowIndex() >= 0)
		{
			auto &group = mFilteredGroups.at(mTable->GetHoveredRowIndex());
			ShowTooltipForItem("", group->mItems[0], group->Count());
		}
	};
	mWindow->AddChild(mTable);
	leftGroup.Pack(mTable);

	leftGroup.ConfigureRow(Layout::TemplateRow, 3);

	mUseLeftHandBtn = mCanvas->Create<Button>();
	mUseLeftHandBtn->SetSize({ 0, 40 });
	mUseLeftHandBtn->SetText(game->GetStr("useLeftHand"));
	mUseLeftHandBtn->OnClick += [this]
	{
		THROW_IF_EXPIRED(mInventory);
		auto inv = mInventory.lock();
		THROW_IF_EXPIRED(inv->GetOwner());
		auto owner = inv->GetOwner().lock();
		if (mTable->GetSelectedRowIndex() >= 0)
		{
			auto &group = mFilteredGroups.at(mTable->GetSelectedRowIndex());
			owner->UseItem(group->mItems[0], Hand::Left);
			Sync();
		}
	};
	leftGroup.Pack(mUseLeftHandBtn);
	mWindow->AddChild(mUseLeftHandBtn);

	mUseRightHandBtn = mCanvas->Create<Button>();
	mUseRightHandBtn->SetSize({ 0, 40 });
	mUseRightHandBtn->SetText(game->GetStr("useRightHand"));
	mUseRightHandBtn->OnClick += [this]
	{
		THROW_IF_EXPIRED(mInventory);
		auto inv = mInventory.lock();
		THROW_IF_EXPIRED(inv->GetOwner());
		auto owner = inv->GetOwner().lock();
		if (mTable->GetSelectedRowIndex() >= 0)
		{
			auto &group = mFilteredGroups.at(mTable->GetSelectedRowIndex());
			owner->UseItem(group->mItems[0], Hand::Right);
			Sync();
		}
	};
	leftGroup.Pack(mUseRightHandBtn);
	mWindow->AddChild(mUseRightHandBtn);

	mThrowBtn = mCanvas->Create<Button>();
	mThrowBtn->SetText(game->GetStr("throw"));
	mThrowBtn->SetSize({ 0, 40 });
	mThrowBtn->OnClick += [this]
	{
		THROW_IF_EXPIRED(mInventory);
		auto inv = mInventory.lock();
		auto owner = inv->GetOwner().lock();
		if (mTable->GetSelectedRowIndex() >= 0)
		{
			auto &group = mFilteredGroups.at(mTable->GetSelectedRowIndex());
			owner->UnequipItem(group->mItems.front());
			inv->ThrowItem(group->mItems.front());
			Sync();
		}
	};
	leftGroup.Pack(mThrowBtn);
	mWindow->AddChild(mThrowBtn);

	Vec2f slotSize = { 45, 45 };
	float equipOffsetX = mWindow->GetSize().x - (300 - slotSize.x) / 2;

	mEquipmentLbl = mCanvas->Create<Label>();
	mEquipmentLbl->SetSize({ 220, 30 });
	mEquipmentLbl->SetPosition({ mWindow->GetSize().x - 220, 15 });
	mEquipmentLbl->SetText(game->GetStr("equipment"));
	mEquipmentLbl->SetAlignment(TextAlignment::Center);
	mWindow->AddChild(mEquipmentLbl);

	mHeadEquipment = MakeSlot(slotSize, { equipOffsetX, 45 });
	mHeadEquipment->OnHover +=
		[this] { ShowSlotTooltip(mHeadEquipment->GetItem<Item>(), "Helmet\n", "Helmet Slot"); };

	mArmsEquipment = MakeSlot(slotSize, { equipOffsetX + 60, 45 });
	mArmsEquipment->OnHover +=
		[this] { ShowSlotTooltip(mArmsEquipment->GetItem<Item>(), "Gloves\n", "Gloves Slot"); };

	mBodyEquipment = MakeSlot(slotSize, { equipOffsetX, 100 });
	mBodyEquipment->OnHover += [this]
	{
		ShowSlotTooltip(mBodyEquipment->GetItem<Item>(), "Chest Armor\n", "Chest Armor Slot");
	};

	mLegsEquipment = MakeSlot(slotSize, { equipOffsetX + 60, 100 });
	mLegsEquipment->OnHover +=
		[this] { ShowSlotTooltip(mLegsEquipment->GetItem<Item>(), "Trousers\n", "Trousers Slot"); };

	mLeftArmForefingerRing = MakeSlot(slotSize, { equipOffsetX, 155 });
	mLeftArmForefingerRing->OnHover += [this]
	{
		ShowSlotTooltip(mLeftArmForefingerRing->GetItem<Item>(), "Left Arm Forefinger Ring\n",
			"Left Arm Forefinger Ring Slot");
	};

	mLeftArmRingfingerRing = MakeSlot(slotSize, { equipOffsetX, 210 });
	mLeftArmRingfingerRing->OnHover += [this]
	{
		ShowSlotTooltip(mLeftArmRingfingerRing->GetItem<Item>(), "Left Arm Ringfinger Ring\n",
			"Left Arm Ringfinger Ring Slot");
	};

	mRightArmRingfingerRing = MakeSlot(slotSize, { equipOffsetX + 60, 155 });
	mRightArmRingfingerRing->OnHover += [this]
	{
		ShowSlotTooltip(mRightArmRingfingerRing->GetItem<Item>(), "Right Arm Ringfinger Ring\n",
			"Right Arm Ringfinger Ring Slot");
	};

	mRightArmForefingerRing = MakeSlot(slotSize, { equipOffsetX + 60, 210 });
	mRightArmForefingerRing->OnHover += [this]
	{
		ShowSlotTooltip(mRightArmForefingerRing->GetItem<Item>(), "Right Arm Forefinger Ring\n",
			"Right Arm Forefinger Ring Slot");
	};

	mHandSlots[0] = MakeSlot(slotSize, { equipOffsetX - 60, 45 });
	mHandSlots[0]->OnHover +=
		[this] { ShowSlotTooltip(mHandSlots[0]->GetItem<Item>(), "Left hand item\n", "Left Hand Slot"); };

	mHandEquipmentConsumablesSlots[0] = MakeSlot(slotSize, { equipOffsetX - 60, 100 });
	mHandEquipmentConsumablesSlots[0]->OnHover += [this]
	{
		ShowSlotTooltip(
			mHandEquipmentConsumablesSlots[0]->GetItem<Item>(), "Left Hand Equipment Consumables\n", "Left Hand Equipment Consumables Slot");
	};

	mHandSlots[1] = MakeSlot(slotSize, { equipOffsetX - 60, 155 });
	mHandSlots[1]->OnHover +=
		[this] { ShowSlotTooltip(mHandSlots[1]->GetItem<Item>(), "Right hand item\n", "Right Hand Slot"); };

	mHandEquipmentConsumablesSlots[1] = MakeSlot(slotSize, { equipOffsetX - 60, 210 });
	mHandEquipmentConsumablesSlots[1]->OnHover += [this]
	{
		ShowSlotTooltip(
			mHandEquipmentConsumablesSlots[1]->GetItem<Item>(), "Right Hand Equipment Consumables\n", "Right Hand Equipment Consumables Slot");
	};



	mMoney = mCanvas->Create<Label>();
	mMoney->SetPosition(
	{ mHandSlots[0]->GetPosition().x - 10, mLeftArmRingfingerRing->GetPosition().y + 60 });
	mMoney->SetSize({ 200, 30 });
	mWindow->AddChild(mMoney);

	mWeight = mCanvas->Create<Label>();
	mWeight->SetPosition(
	{ mHandSlots[0]->GetPosition().x - 10, mLeftArmRingfingerRing->GetPosition().y + 90 });
	mWeight->SetSize({ 200, 30 });
	mWindow->AddChild(mWeight);

	mTooltip = mCanvas->Create<Tooltip>();
	mTooltip->SetSize({ 350, 420 });

	mCanvas->SetVisible(false);

	Log() << "Inventory system successfully initialized!" << LineEnd;
}

InventoryGUI::~InventoryGUI()
{
	DBG_ECHO();
}

void InventoryGUI::Update()
{
	if (!mInventory.expired() && mCanvas->IsVisible())
	{
		if (mTable->GetSelectedRowIndex() >= 0)
		{
			auto &controls = game->GetControls();
			auto player = game->GetLevel()->GetPlayer();
			for (int i = 0; i < Player::QuickAccessSlotCount; ++i)
			{
				const auto key = static_cast<int>(ControlKey::QuickAccess0) + i;
				if (controls.IsKeyHit(static_cast<ControlKey::Type>(key)))
				{
					const auto group = mFilteredGroups.at(mTable->GetSelectedRowIndex());
					player->SetQuickAccess(group, i);
				}
			}
		}
	}
}

void InventoryGUI::SetVisible(bool state)
{
	if (!game->GetLevel())
	{
		return;
	}
	if (!game->GetLevel()->GetPlayer())
	{
		return;
	}
	mCanvas->SetVisible(state);
	if (state)
	{
		Sync();
	}
	else
	{
		mFilteredGroups.clear();
	}
}

bool InventoryGUI::IsVisible() const
{
	return mCanvas->IsVisible();
}

void InventoryGUI::SwitchVisible()
{
	SetVisible(!IsVisible());
}

void InventoryGUI::SetInventory(const shared_ptr<Inventory> &inv)
{
	mInventory = inv;
}

void InventoryGUI::Sync()
{
	THROW_IF_EXPIRED(mInventory);
	auto inv = mInventory.lock();
	auto player = dynamic_pointer_cast<Player>(inv->GetOwner().lock());
	THROW_IF_NULL(player);
	const auto &groups = inv->GetGroups();
	float scrollBar = mTable->GetScrollBarPosition();
	mTable->Clear();
	mFilteredGroups.clear();
	const auto filterItem = *static_pointer_cast<ItemClass>(mFilter->GetCurrentItem());
	for (auto &group : groups)
	{
		const auto &desc = group->GetAny()->GetDefinition();
		if (desc.mClass == filterItem || filterItem == ItemClass::Unknown)
		{
			const int row = mTable->AddRow();
			// Also filter groups
			mFilteredGroups.push_back(group);
			dynamic_pointer_cast<Label>(mTable->Get(0, row))
				->SetText(StringBuilder("x") << group->Count() << " - " << desc.GetTranslatedName());
		}
	}
	mTable->SetScrollBarPosition(scrollBar);

	// Update slots
	SetSlotItem(mHandSlots[0], player->GetHandEquipment(Hand::Left));
	SetSlotItem(mHandSlots[1], player->GetHandEquipment(Hand::Right));
	SetSlotItem(mHandEquipmentConsumablesSlots[0], player->GetHandEquipmentConsumable(Hand::Left));
	SetSlotItem(mHandEquipmentConsumablesSlots[1], player->GetHandEquipmentConsumable(Hand::Right));
	SetSlotItem(mHeadEquipment, player->GetHelmet());
	SetSlotItem(mBodyEquipment, player->GetChestArmor());
	SetSlotItem(mArmsEquipment, player->GetGloves());
	SetSlotItem(mLegsEquipment, player->GetTrousers());
	SetSlotItem(mLeftArmForefingerRing, player->GetRings()[0]);
	SetSlotItem(mLeftArmRingfingerRing, player->GetRings()[1]);
	SetSlotItem(mRightArmForefingerRing, player->GetRings()[2]);
	SetSlotItem(mRightArmRingfingerRing, player->GetRings()[3]);

	mMoney->SetText(StringBuilder() << game->GetStr("money") << inv->GetMoney());
	mWeight->SetText(
		StringBuilder() << game->GetStr("stuffWeight") << RoundAndConvert(inv->GetTotalWeight(), 1));
}

EquipmentSlot::EquipmentSlot(const shared_ptr<Canvas> &canvas, const Vec2f &size)
{
	mFrame = canvas->Create<Frame>();
	mFrame->SetSize(size);
	mImage = canvas->Create<Image>();
	mImage->SetPosition({ 3, 3 });
	mImage->SetSize(size - Vec2f(6, 6));
	mImage->SetLocalVisibility(false);
	mFrame->AddChild(mImage);
}

void EquipmentSlot::SetItem(const shared_ptr<Item> &item)
{
	if (item)
	{
		auto pic = item->GetPictogram();
		if (pic)
		{
			if (pic->GetWidth() > pic->GetHeight())
			{
				float w = mFrame->GetSize().x - 6;
				float k = w / pic->GetWidth();
				mImage->SetSize({ w, k * pic->GetHeight() });
			}
			else
			{
				float h = mFrame->GetSize().y - 6;
				float k = h / pic->GetHeight();
				mImage->SetSize({ k * pic->GetWidth(), h });
			}
			mImage->SetPosition({ (mFrame->GetSize().x - mImage->GetSize().x) / 2,
				(mFrame->GetSize().y - mImage->GetSize().y) / 2 });
		}
		mImage->SetLocalVisibility(true);
		mImage->SetTexture(pic);
		mImage->SetTextureRectangle(item->GetPictogramTexCoords());
	}
	else
	{
		mImage->SetLocalVisibility(false);
	}
	mItem = item;
}
