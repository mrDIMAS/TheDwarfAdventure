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
#include "HUD.h"
#include "Renderer.h"
#include "InventoryGUI.h"
#include "StatWindow.h"
#include "Input.h"
#include "Journal.h"
#include "Item.h"
#include "Canvas.h"
#include "Image.h"
#include "Label.h"
#include "Bar.h"
#include "Button.h"
#include "Frame.h"
#include "Inventory.h"
#include "Player.h"
#include "Frame.h"
#include "Window.h"
#include "Dialog.h"
#include "BarterWindow.h"
#include "MapWindow.h"

HUD::HUD()
{
	mCanvas = game->CreateCanvas();
	auto theme = mCanvas->GetTheme();

	const Vec2f scrSize = game->VirtualScreen.getSize();
	const Vec2f cornerOffset = { 20, 20 };
	const Vec2f scrCenter = game->VirtualScreen.getCenter();

	// Create minimap
	mMinimapFrame = mCanvas->Create<Frame>();
	mMinimapFrame->SetSize({ 135, 135 });
	mMinimapFrame->SetPosition({ scrSize.x - cornerOffset.x - mMinimapFrame->GetSize().y, scrSize.y - mMinimapFrame->GetSize().y - cornerOffset.y });

	const Vec2f border = { 3, 3 };
	mMinimapImg = mCanvas->Create<Image>();
	mMinimapImg->SetPosition(mMinimapFrame->GetPosition() + border);
	mMinimapImg->SetSize(mMinimapFrame->GetSize() - 2.0f * border);

	mMinimapFogOfWar = mCanvas->Create<Image>();
	mMinimapFogOfWar->SetPosition(mMinimapFrame->GetPosition() + border);
	mMinimapFogOfWar->SetSize(mMinimapFrame->GetSize() - 2.0f * border);

	mPlayerMark = mCanvas->Create<Image>();
	mPlayerMark->SetColor({ 255, 255, 255 });
	mPlayerMark->SetSize({ 2, 2 });

	// Create notification label
	mNotificationLbl = mCanvas->Create<Label>();
	mNotificationLbl->SetPosition({ 10, game->VirtualScreen.getCenter().y });
	mNotificationLbl->SetSize({ 300, 32 });

	// Create health, stamina, expirience bars
	const Vec2f barSize = { 200, 20 };
	const float barSpacing = 3;

	mExpirienceBar = mCanvas->Create<Bar>();
	mExpirienceBar->SetSize(barSize);
	mExpirienceBar->SetBarColor(sf::Color(255, 127, 39));
	mExpirienceBar->SetPosition({ cornerOffset.x, scrSize.y - cornerOffset.y - barSize.y });

	mStatPointsAvail = mCanvas->Create<Label>();
	mStatPointsAvail->SetPosition(mExpirienceBar->GetGlobalPosition());
	mStatPointsAvail->SetSize(mExpirienceBar->GetSize());
	mStatPointsAvail->SetAlignment(TextAlignment::Center);

	mStaminaBar = mCanvas->Create<Bar>();
	mStaminaBar->SetSize(barSize);
	mStaminaBar->SetBarColor(sf::Color(32, 179, 47));
	mStaminaBar->SetPosition({ cornerOffset.x, scrSize.y - cornerOffset.y - (barSize.y + barSpacing) - barSize.y });

	mHealthBarCurrent = mCanvas->Create<Bar>();
	mHealthBarCurrent->SetBarColor(sf::Color(200, 0, 0));
	mHealthBarCurrent->SetSize(barSize);
	mHealthBarCurrent->SetPosition({ cornerOffset.x, scrSize.y - cornerOffset.y - 2 * (barSize.y + barSpacing) - barSize.y });

	mRareRedCrystals = mCanvas->Create<Label>();
	mRareRedCrystals->SetPosition(mHealthBarCurrent->GetGlobalPosition());
	mRareRedCrystals->SetSize(mHealthBarCurrent->GetSize());
	mRareRedCrystals->SetAlignment(TextAlignment::Center);
	
	// Create hand equipment slots
	const float yOffset = mHealthBarCurrent->GetPosition().y;
	const Vec2f handEquipSize = { 40, 40 };

	mHandEquipmentFrames[0] = mCanvas->Create<Frame>();
	mHandEquipmentFrames[0]->SetSize(handEquipSize);
	mHandEquipmentFrames[0]->SetPosition({ cornerOffset.x, yOffset - 2.5f * handEquipSize.y });

	mHandEquipmentImages[0] = mCanvas->Create<Image>();
	mHandEquipmentImages[0]->SetSize({ 34, 34 });
	mHandEquipmentImages[0]->SetPosition(mHandEquipmentFrames[0]->GetGlobalPosition() + Vec2f(3, 3));
	mHandEquipmentFrames[0]->AddChild(mHandEquipmentImages[0]);

	mHandEquipmentConsumables[0] = mCanvas->Create<Label>();
	mHandEquipmentConsumables[0]->SetPosition(mHandEquipmentFrames[0]->GetGlobalPosition() + Vec2f(mHandEquipmentFrames[0]->GetSize().x + 5, 10));
	mHandEquipmentConsumables[0]->SetSize({ 170, 60 });

	mHandEquipmentFrames[1] = mCanvas->Create<Frame>();
	mHandEquipmentFrames[1]->SetSize(handEquipSize);
	mHandEquipmentFrames[1]->SetPosition(mHandEquipmentFrames[0]->GetGlobalPosition() + Vec2f(0, handEquipSize.y + 5));

	mHandEquipmentImages[1] = mCanvas->Create<Image>();
	mHandEquipmentImages[1]->SetSize({ 34, 34 });
	mHandEquipmentImages[1]->SetPosition(mHandEquipmentFrames[1]->GetGlobalPosition() + Vec2f(3, 3));
	mHandEquipmentFrames[1]->AddChild(mHandEquipmentImages[1]);

	mHandEquipmentConsumables[1] = mCanvas->Create<Label>();
	mHandEquipmentConsumables[1]->SetPosition(mHandEquipmentFrames[1]->GetGlobalPosition() + Vec2f(mHandEquipmentFrames[1]->GetSize().x + 5, 10));
	mHandEquipmentConsumables[1]->SetSize({ 170, 60 });

	// Create action label above quick access panel
	mActionLbl = mCanvas->Create<Label>();
	mActionLbl->SetSize({ 400, 100 });
	mActionLbl->SetPosition({ (scrSize.x - mActionLbl->GetSize().x) / 2.0f, scrCenter.y + scrSize.y / 2 - mActionLbl->GetSize().y - 80 });
	mActionLbl->SetAlignment(TextAlignment::Center);

	// Create quick access panel
	const Vec2f slotSize = { 40, 40 };
	const float slotSpacing = 5;
	int n = 1;
	for (auto &slot : mQuickAccessSlots)
	{
		slot.mFrame = mCanvas->Create<Frame>();
		slot.mFrame->SetSize(slotSize);
		slot.mFrame->SetPosition({ 
			scrCenter.x + (n - 6) * (slotSize.x + slotSpacing),
			scrSize.y - cornerOffset.y - slotSize.y 
		});

		slot.mImage = mCanvas->Create<Image>();
		slot.mImage->SetPosition({ 3, 3 });
		slot.mImage->SetSize(slot.mFrame->GetSize() - Vec2f(6, 6));
		slot.mImage->SetLocalVisibility(false);
		slot.mFrame->AddChild(slot.mImage);

		slot.mKey = mCanvas->Create<Label>();
		slot.mKey->SetText(n == 10 ? "0" : to_string(n++));
		slot.mKey->SetPosition(slot.mFrame->GetSize() - Vec2f(14, 18));
		slot.mFrame->AddChild(slot.mKey);

		slot.mCount = mCanvas->Create<Label>();
		slot.mCount->SetText("");
		slot.mCount->SetPosition({ 2, 0 });
		slot.mFrame->AddChild(slot.mCount);
	}

	// Create menu access buttons
	mInventoryBtn = mCanvas->Create<Button>();
	mInventoryBtn->SetSize(slotSize);
	mInventoryBtn->SetPosition(mMinimapFrame->GetGlobalPosition() + Vec2f(0, -slotSpacing - slotSize.y));
	mInventoryBtn->SetImage(game->GetRenderer()->RequestTexture("data/textures/gui/inventory_icon.png"));
	mInventoryBtn->OnClick += [this]
	{
		game->GetStatWindow()->SetVisible(false);
		game->GetDialogWindow()->SetVisible(false);
		game->GetJournal()->SetVisible(false);
		game->GetBarterWindow()->SetVisible(false);
		game->GetMapWindow()->SetVisible(false);
		game->GetInventoryGUI()->SetVisible(true);
	};

	mStatsBtn = mCanvas->Create<Button>();
	mStatsBtn->OnClick += [this]
	{
		game->GetInventoryGUI()->SetVisible(false);
		game->GetJournal()->SetVisible(false);
		game->GetDialogWindow()->SetVisible(false);
		game->GetBarterWindow()->SetVisible(false);
		game->GetMapWindow()->SetVisible(false);
		game->GetStatWindow()->SetVisible(true);
	};
	mStatsBtn->SetSize(slotSize);
	mStatsBtn->SetPosition(mMinimapFrame->GetGlobalPosition() + Vec2f(slotSize.x + slotSpacing, -slotSpacing - slotSize.y));
	mStatsBtn->SetImage(game->GetRenderer()->RequestTexture("data/textures/gui/stats_icon.png"));

	mJournalBtn = mCanvas->Create<Button>();
	mJournalBtn->OnClick += [this]
	{
		game->GetInventoryGUI()->SetVisible(false);
		game->GetStatWindow()->SetVisible(false);
		game->GetDialogWindow()->SetVisible(false);
		game->GetBarterWindow()->SetVisible(false);
		game->GetMapWindow()->SetVisible(false);
		game->GetJournal()->SetVisible(true);
	};
	mJournalBtn->SetSize(slotSize);
	mJournalBtn->SetPosition(mMinimapFrame->GetGlobalPosition() + Vec2f(2*(slotSize.x + slotSpacing), -slotSpacing - slotSize.y));
	mJournalBtn->SetImage(game->GetRenderer()->RequestTexture("data/textures/gui/map_icon.png"));

	mDeathOverlay = mCanvas->Create<Image>();
	mDeathOverlay->SetSize(game->VirtualScreen.getSize());
	mDeathOverlay->SetTexture(game->GetRenderer()->RequestTexture("data/textures/gui/youdied_background.png"));
	mDeathOverlay->SetTextureRectangle({ 0, 0, 1, 1 });
	mDeathOverlay->SetColor({ 150, 0, 0, 220 });
	mDeathOverlay->SetLocalVisibility(false);

	mYouDiedLbl = mCanvas->Create<Label>();
	mYouDiedLbl->SetPosition({ 0, 0 });
	mYouDiedLbl->SetSize(game->VirtualScreen.getSize());
	mYouDiedLbl->SetAlignment(TextAlignment::Center);
	mYouDiedLbl->SetCharacterSize(52);
	mYouDiedLbl->SetText(game->GetStr("youDied"));
	mYouDiedLbl->SetColor({ 200, 0, 0 });
	mYouDiedLbl->SetLocalVisibility(false);

	mQuestionModalWindow = mCanvas->Create<Window>();
	mQuestionModalWindow->SetSize({ 400, 300 });
	mQuestionModalWindow->SetInCenter();
	mQuestionModalWindow->SetLocalVisibility(false);

	mQuestionModalWindowQuestion = mCanvas->Create<Label>();
	mQuestionModalWindowQuestion->SetAutoHeight(true);
	mQuestionModalWindow->AddChild(mQuestionModalWindowQuestion);

	mQuestionModalWindow->GetLayout().ConfigureRow(Layout::TemplateRow, 2);

	mQuestionModalWindowYes = mCanvas->Create<Button>();
	mQuestionModalWindowYes->SetText(game->GetStr("yes"));
	mQuestionModalWindowYes->SetSize({ 0, 30 });
	mQuestionModalWindow->AddChild(mQuestionModalWindowYes);

	mQuestionModalWindowNo = mCanvas->Create<Button>();
	mQuestionModalWindowNo->SetText(game->GetStr("no"));
	mQuestionModalWindowNo->SetSize({ 0, 30 });
	mQuestionModalWindow->AddChild(mQuestionModalWindowNo);
	
	mQuestCompleteLbl = mCanvas->Create<Label>();
	mQuestCompleteLbl->SetSize({ 400, 32 });
	mQuestCompleteLbl->SetAlignment(TextAlignment::Center);
	mQuestCompleteLbl->SetPosition({ game->VirtualScreen.getCenter().x - mQuestCompleteLbl->GetSize().x / 2,
		game->VirtualScreen.getCenter().y - 200 });

	mBossHealthBar = mCanvas->Create<Bar>();
	mBossHealthBar->SetSize({ game->VirtualScreen.getSize().x * 0.5f, 13 });
	mBossHealthBar->SetPosition({ 
		0.5f * (game->VirtualScreen.getSize().x - mBossHealthBar->GetSize().x),
		game->VirtualScreen.getSize().y - 100
	});
	mBossHealthBar->SetBarColor({ 200, 0, 0 });

	mBossName = mCanvas->Create<Label>();
	mBossName->SetPosition(
	{ mBossHealthBar->GetGlobalPosition().x, mBossHealthBar->GetGlobalPosition().y - 16 });
	mBossName->SetText("Golem");

	SetBossFightModeEnabled(false);

	Log() << "HUD successfully initialized!" << LineEnd;
}

void HUD::Update()
{
	--mNextNotificationTimer;
	if (mNextNotificationTimer <= 0)
	{
		if (mNotificationList.size())
		{
			mNotificationList.erase(mNotificationList.begin());
			if (mNotificationList.size())
			{
				mNotificationLbl->SetText(*mNotificationList.begin());
			}
			else
			{
				mNotificationLbl->SetText(" ");
			}
			mNextNotificationTimer = NextNotificationTimeout;
		}
	}

	--mActionClearTimer;
	if (mActionClearTimer <= 0)
	{
		mActionLbl->SetText("");
	}

	auto &controls = game->GetControls();
	for (size_t i = 0; i < mQuickAccessSlots.size(); ++i)
	{
		const auto controlKey = static_cast<ControlKey::Type>(static_cast<int>(ControlKey::QuickAccess0) + i);
		mQuickAccessSlots[i].mKey->SetText(controls.GetKeyName(controlKey));
	}

	mQuestCompleteLbl->SetLocalVisibility(mQuestCompletedNotifyTime > 0);
	--mQuestCompletedNotifyTime;
}

void HUD::AddNotification(const string &notification)
{
	if (mNotificationList.size() == 0)
	{
		mNotificationLbl->SetText(notification);
	}
	mNotificationList.emplace_back(notification);
	mNextNotificationTimer = NextNotificationTimeout;
}

void HUD::SetHealth(float health, float maxLimit, int rareRedCrystals)
{
	mHealthBarCurrent->SetValue(health);
	mHealthBarCurrent->SetMaxLimit(maxLimit);
	mRareRedCrystals->SetText('x' + to_string(rareRedCrystals));
}

void HUD::SetStamina(float stamina, float maxStamina)
{
	mStaminaBar->SetValue(stamina);
	mStaminaBar->SetMaxLimit(maxStamina);
}

void HUD::UpdateMinimap(const shared_ptr<Texture> &texture, const shared_ptr<Texture> &fogOfWar, const Vec2f &playerPosition, const Vec2f &worldSize)
{
	mMinimapFogOfWar->SetTexture(fogOfWar);
	mMinimapFogOfWar->SetTextureRectangle({ 0, 0, 1, 1 });

	mMinimapImg->SetTexture(texture);
	mMinimapImg->SetTextureRectangle({ 0, 0, 1, 1 });

	Vec2f relPos = { mMinimapImg->GetSize().x * (playerPosition.x / worldSize.x),
		mMinimapImg->GetSize().y * (playerPosition.y / worldSize.y) };
	mPlayerMark->SetPosition(mMinimapImg->GetGlobalPosition() + relPos);
}

void HUD::SetVisible(bool state)
{
	mCanvas->SetVisible(state);
}

void HUD::SetActionLabel(const sf::Keyboard::Key &key, const string &text)
{
	mActionLbl->SetText(StringBuilder("[") << game->GetInput()->GetKeyName(key) << "] -" << text);
	mActionClearTimer = 10;
}

void HUD::SetNotification(const string &message)
{
	mActionLbl->SetText(message);
	mActionClearTimer = 15;
}

void HUD::SetYouDiedVisible(bool state)
{
	mYouDiedLbl->SetLocalVisibility(state);
	mDeathOverlay->SetLocalVisibility(state);
}

void HUD::SetExpirience(int current, int nextLevel)
{
	mExpirienceBar->SetMaxLimit(static_cast<float>(nextLevel));
	mExpirienceBar->SetValue(static_cast<float>(current));
}

void HUD::SetQuickAccess(const array<weak_ptr<ItemGroup>, Player::QuickAccessSlotCount> &slots)
{
	int slotNum = 0;
	for (auto &playerSlot : slots)
	{
		if (auto g = playerSlot.lock())
		{
			mQuickAccessSlots[slotNum].mCount->SetText(StringBuilder("x") << g->Count());
			mQuickAccessSlots[slotNum].mCount->SetLocalVisibility(true);
			const auto &img = mQuickAccessSlots[slotNum].mImage;
			const auto &frame = mQuickAccessSlots[slotNum].mFrame;
			const auto pic = g->mItems[0]->GetPictogram();
			if (pic)
			{
				if (pic->GetWidth() > pic->GetHeight())
				{
					float w = frame->GetSize().x - 6;
					float k = w / pic->GetWidth();
					img->SetSize({ w, k * pic->GetHeight() });
				}
				else
				{
					float h = frame->GetSize().y - 6;
					float k = h / pic->GetHeight();
					img->SetSize({ k * pic->GetWidth(), h });
				}
				const Vec2f p {
					(frame->GetSize().x - img->GetSize().x) / 2,
					(frame->GetSize().y - img->GetSize().y) / 2
				};
				img->SetPosition(p);
			}
			else
			{
				// dummy
				img->SetSize({ 6, 6 });
				const Vec2f p {
					(mQuickAccessSlots[slotNum].mFrame->GetSize().x - img->GetSize().x) / 2,
					(mQuickAccessSlots[slotNum].mFrame->GetSize().y - img->GetSize().y) / 2
				};
				img->SetPosition(p);
			}
			img->SetLocalVisibility(true);
			img->SetTexture(pic);
			img->SetTextureRectangle(g->mItems[0]->GetPictogramTexCoords());
		}
		else
		{
			mQuickAccessSlots[slotNum].mImage->SetLocalVisibility(false);
			mQuickAccessSlots[slotNum].mCount->SetLocalVisibility(false);
		}
		++slotNum;
	}
}

bool HUD::IsAnythingHovered() const
{
	// return mCanvas->IsAnythingHovered();
	return mInventoryBtn->IsHovered() || mStatsBtn->IsHovered() || mJournalBtn->IsHovered();
}

bool HUD::IsInModalMode() const
{
	return mQuestionModalWindow->IsGloballyVisible();
}

void HUD::AskQuestion(const string &question, function<void()> yesAction, function<void()> noAction)
{
	// hide inventory window first
	game->GetInventoryGUI()->SetVisible(false);

	mQuestionModalWindow->SetLocalVisibility(true);

	mQuestionModalWindowQuestion->SetText(question);

	mQuestionModalWindowYes->OnClick.Clear();
	mQuestionModalWindowYes->OnClick += yesAction;
	mQuestionModalWindowYes->OnClick += [this] { mQuestionModalWindow->SetLocalVisibility(false); };

	mQuestionModalWindowNo->OnClick.Clear();
	mQuestionModalWindowNo->OnClick += noAction;
	mQuestionModalWindowNo->OnClick += [this] { mQuestionModalWindow->SetLocalVisibility(false); };
}

void HUD::SetHandsEquipment(const array<shared_ptr<Item>, 2> &handsEqupment, const array<shared_ptr<Item>, 2> &consumables, const shared_ptr<Inventory> &inv)
{
	for (int handIndex = 0; handIndex < 2; ++handIndex)
	{
		auto & handEquipment = handsEqupment[handIndex];
		auto & handEquipmentImage = mHandEquipmentImages[handIndex];
		auto & handEquipmentFrame = mHandEquipmentFrames[handIndex];
		auto & consumable = consumables[handIndex];
		auto & handEquipmentConsumableLabel = mHandEquipmentConsumables[handIndex];
		if (handEquipment)
		{
			auto pic = handEquipment->GetPictogram();
			if (pic)
			{
				handEquipmentImage->SetTexture(pic);
				handEquipmentImage->SetTextureRectangle(handEquipment->GetPictogramTexCoords());
				if (pic->GetWidth() > pic->GetHeight())
				{
					float w = handEquipmentFrame->GetSize().x - 6;
					float k = w / pic->GetWidth();
					handEquipmentImage->SetSize({ w, k * pic->GetHeight() });
				}
				else
				{
					float h = handEquipmentFrame->GetSize().y - 6;
					float k = h / pic->GetHeight();
					handEquipmentImage->SetSize({ k * pic->GetWidth(), h });
				}
				handEquipmentImage->SetPosition({ (handEquipmentFrame->GetSize().x - handEquipmentImage->GetSize().x) / 2,
					(handEquipmentFrame->GetSize().y - handEquipmentImage->GetSize().y) / 2 });
			}
			handEquipmentImage->SetLocalVisibility(pic != nullptr);


			if (consumable)
			{
				bool consumablesMatch = consumable->GetDefinition().mType == handEquipment->GetDefinition().mConsumable;
				bool isCrystal = consumable->GetDefinition().mClass == ItemClass::Crystal;
				string count;
				auto group = inv->FindItemGroup(consumable->GetType());
				if (group)
				{
					count = to_string(group->Count());
				}

				StringBuilder msg;
				if (consumablesMatch)
				{
					msg << game->GetStr("weaponConsumablesCount") << ": x" << count;
					if (isCrystal)
					{
						msg << ": " << RoundAndConvert(consumable->GetCurrentDurability(), 1);
					}
				}
				else
				{
					msg << game->GetStr("noconsumables");
				}

				handEquipmentConsumableLabel->SetText(msg);
			}
			else
			{
				handEquipmentConsumableLabel->SetText(game->GetStr("noconsumables"));
			}
		}
		else
		{
			handEquipmentConsumableLabel->SetText(game->GetStr("noweapon"));
		}
	}
}

void HUD::Reset()
{
	for (auto &slot : mQuickAccessSlots)
	{
		slot.mImage->SetLocalVisibility(false);
		slot.mCount->SetLocalVisibility(false);
	}
}

void HUD::NotifyQuestCompletion(const string &questCaption)
{
	mQuestCompleteLbl->SetText(questCaption);
	mQuestCompletedNotifyTime = 120;
}

void HUD::SetBossFightModeEnabled(bool state)
{
	mBossName->SetLocalVisibility(state);
	mBossHealthBar->SetLocalVisibility(state);
}

void HUD::SetBossName(const string &name)
{
	mBossName->SetText(name);
}

void HUD::SetBossHealth(float health, float maxHealth)
{
	mBossHealthBar->SetMaxLimit(maxHealth);
	mBossHealthBar->SetValue(health);
}

void HUD::NotifyStatPointsAvailable(int count)
{
	mStatPointsAvail->SetText("x" + to_string(count));
}
