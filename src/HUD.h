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

#include "Player.h"

class Inventory;
class Item;

struct QuickAccessSlot
{
	shared_ptr<Frame> mFrame;
	shared_ptr<Image> mImage;
	shared_ptr<Label> mKey;
	shared_ptr<Label> mCount;
};

class HUD
{
private:
	shared_ptr<Canvas> mCanvas;
	shared_ptr<Frame> mMinimapFrame;
	shared_ptr<Image> mMinimapFogOfWar;
	shared_ptr<Image> mMinimapImg;
	shared_ptr<Label> mNotificationLbl;
	shared_ptr<Label> mQuestCompleteLbl;
	shared_ptr<Label> mYouDiedLbl;
	shared_ptr<Bar> mHealthBarCurrent;
	shared_ptr<Bar> mStaminaBar;
	shared_ptr<Bar> mExpirienceBar;
	shared_ptr<Label> mActionLbl;
	shared_ptr<Label> mRareRedCrystals;
	shared_ptr<Button> mInventoryBtn;
	shared_ptr<Button> mStatsBtn;
	shared_ptr<Button> mJournalBtn;

	array<shared_ptr<Frame>, 2> mHandEquipmentFrames;
	array<shared_ptr<Image>, 2> mHandEquipmentImages;
	array<shared_ptr<Label>, 2> mHandEquipmentConsumables;
	shared_ptr<Image> mPlayerMark;
	shared_ptr<Label> mShieldDesc;
	shared_ptr<Image> mDeathOverlay;
	shared_ptr<Window> mQuestionModalWindow;
	shared_ptr<Label> mStatPointsAvail;
	shared_ptr<Label> mQuestionModalWindowQuestion;
	shared_ptr<Button> mQuestionModalWindowYes;
	shared_ptr<Button> mQuestionModalWindowNo;
	shared_ptr<Label> mBossName;
	shared_ptr<Bar> mBossHealthBar;
	array<QuickAccessSlot, Player::QuickAccessSlotCount> mQuickAccessSlots;
	vector<string> mNotificationList;
	int mNextNotificationTimer { 0 };
	int mQuestCompletedNotifyTime { 0 };
	int mActionClearTimer { 0 };                             // in frames
	static constexpr int NextNotificationTimeout = 60; // in frames
public:
	HUD();
	void Update();
	void AddNotification(const string &notification);
	void SetHealth(float health, float maxLimit, int rareRedCrystals);
	void SetStamina(float stamina, float maxStamina);
	void UpdateMinimap(const shared_ptr<Texture> &texture, const shared_ptr<Texture> &fogOfWar, const Vec2f &playerPosition, const Vec2f &worldSize);
	void SetVisible(bool state);
	void SetActionLabel(const sf::Keyboard::Key &key, const string &text);
	void SetNotification(const string &message);
	void SetYouDiedVisible(bool state);
	void SetExpirience(int current, int nextLevel);
	void SetQuickAccess(const array<weak_ptr<ItemGroup>, Player::QuickAccessSlotCount> &slots);
	bool IsAnythingHovered() const;
	bool IsInModalMode() const;
	void AskQuestion(const string &question, function<void()> yesAction, function<void()> noAction);
	void SetHandsEquipment(const array<shared_ptr<Item>, 2> &handsEqupment, const array<shared_ptr<Item>, 2> &consumables, const shared_ptr<Inventory> &inv);
	void Reset();
	void NotifyQuestCompletion(const string &questCaption);
	void SetBossFightModeEnabled(bool state);
	void SetBossName(const string &name);
	void SetBossHealth(float health, float maxHealth);
	void NotifyStatPointsAvailable(int count);
};
