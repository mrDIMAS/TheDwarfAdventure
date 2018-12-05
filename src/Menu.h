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

#include "Config.h"
#include "ItemDatabase.h"

class Game;
class Button;
class Window;
class ListBox;
class Window;
class Canvas;
class NumericUpDown;
class Table;
class Checkbox;
class Tooltip;
class Label;
class SlideList;
class WorldGenerator;

enum class MenuPage
{
	Main,
	NewGame,
	Options,
	SaveGame,
	LoadGame,
	Authors,
	EndGame,
};

class Menu
{
private:
	bool mWaitKey { false };
	int mWaitKeyBlinkTimer { 0 };
	int mSeed { 228 };
	MenuPage mPage { MenuPage::Main };

	vector<sf::VideoMode> mVideoModeList;

	shared_ptr<Canvas> mCanvas;

	shared_ptr<Window> mMainWnd;
	shared_ptr<Button> mContinueBtn;
	shared_ptr<Button> mNewGameBtn;
	shared_ptr<Button> mSaveGameBtn;
	shared_ptr<Button> mLoadGameBtn;
	shared_ptr<Button> mEditorBtn;
	shared_ptr<Button> mOptionsBtn;
	shared_ptr<Button> mAuthorsBtn;
	shared_ptr<Button> mExitBtn;

	shared_ptr<Window> mNewGameWnd;
	shared_ptr<Label> mSeedLbl;
	shared_ptr<Button> mNewSeedBtn;
	shared_ptr<SlideList> mWorldSizeNud;
	shared_ptr<Label> mWorldSizeLbl;
	shared_ptr<SlideList> mDifficultySld;
	shared_ptr<Label> mDifficultyLbl;
	shared_ptr<Label> mMapSizeWarning;
	shared_ptr<Label> mDAWGVersionLbl;
	shared_ptr<Label> mDAWGVersionNumberLbl;
	shared_ptr<Label> mMoneyLbl;
	shared_ptr<Label> mMoneyCountLbl;
	shared_ptr<Label> mHeroClassLbl;
	shared_ptr<SlideList> mHeroClassSlt;
	shared_ptr<Image> mBackground;

	bool mLevelGenerated = false;

	shared_ptr<Label> mVitalityLbl;
	shared_ptr<Label> mVitalityTextLbl;
	shared_ptr<Label> mStrengthLbl;
	shared_ptr<Label> mStrengthTextLbl;
	shared_ptr<Label> mDexterityLbl;
	shared_ptr<Label> mDexterityTextLbl;
	shared_ptr<Label> mEnduranceLbl;
	shared_ptr<Label> mEnduranceTextLbl;
	shared_ptr<Label> mIntelligenceLbl;
	shared_ptr<Label> mIntelligenceTextLbl;
	shared_ptr<Label> mCharismaLbl;
	shared_ptr<Label> mCharismaTextLbl;
	shared_ptr<Label> mLuckLbl;
	shared_ptr<Label> mLuckTextLbl;

	shared_ptr<Table> mStartItemsTbl;
	shared_ptr<Button> mGenerateBtn;
	shared_ptr<Button> mNewGameBackBtn;

	static constexpr unsigned int SaveLoadSlotCount = 8;

	shared_ptr<Window> mSaveGameWnd;
	shared_ptr<Button> mSaveGameBackBtn;
	array<shared_ptr<Button>, SaveLoadSlotCount> mSaveSlotBtn;
	array<shared_ptr<Label>, SaveLoadSlotCount> mSaveSlotDescLbl;

	shared_ptr<Window> mLoadGameWnd;
	shared_ptr<Button> mLoadGameBackBtn;
	array<shared_ptr<Button>, SaveLoadSlotCount> mLoadSlotBtn;
	array<shared_ptr<Label>, SaveLoadSlotCount> mLoadSlotDescLbl;

	shared_ptr<Window> mProgressWindow;
	shared_ptr<Label> mProgressPleaseWaitLabel;
	shared_ptr<Label> mProgressLabel;

	shared_ptr<Window> mOptionsWnd;
	shared_ptr<Button> mOptionsBackBtn;
	shared_ptr<Label> mResolutionLbl;
	shared_ptr<SlideList> mResolutionLst;
	shared_ptr<Label> mWindowModeLbl;
	shared_ptr<SlideList> mWindowModeLst;
	shared_ptr<Label> mControlsLbl;
	shared_ptr<ListBox> mControlsLbx;
	shared_ptr<Label> mMusicVolumeLbl;
	shared_ptr<NumericUpDown> mMusicVolumeNud;
	shared_ptr<Label> mSoundVolumeLbl;
	shared_ptr<NumericUpDown> mSoundVolumeNud;
	shared_ptr<Checkbox> mShowPerformanceChk;

	shared_ptr<Window> mAuthorsWnd;
	shared_ptr<Button> mAuthorsBackBtn;
	shared_ptr<Label> mAuthorsLbl;

	struct
	{
		shared_ptr<Window> mWindow;
		shared_ptr<Label> mText;
		shared_ptr<Button> mLoadGameBtn;
		shared_ptr<Button> mToMainMenu;
		shared_ptr<Button> mExitGameBtn;
	} mEndGame;

	shared_ptr<struct PlayerPreset> GetSelectedPreset() const;
	shared_ptr<Tooltip> mTooltip;
	vector<shared_ptr<struct PlayerPreset>> mPresets;
	void LoadPlayerPresets(const string & presetsPath);
	void ShowTooltipForItem(ItemType item, int count);
	void Sync();
	void InvokeGenerationThread();
	void FillResolutionList();
	void MakeEndGamePage();
public:
	Menu();
	~Menu();
	void SetVisible(bool state);
	bool IsVisible() const;
	void SetPage(const MenuPage &page);
	void Update();
	void SetProgressMessage(const string & message);
	void ShowProgressWindow(bool state);
};
