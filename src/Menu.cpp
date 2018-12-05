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
#include "Menu.h"
#include "Game.h"
#include "Editor.h"
#include "Renderer.h"
#include "Input.h"
#include "Layout.h"
#include "Level.h"
#include "Inventory.h"
#include "Player.h"
#include "Button.h"
#include "SlideList.h"
#include "ListBox.h"
#include "Window.h"
#include "NumericUpDown.h"
#include "Table.h"
#include "Checkbox.h"
#include "Tooltip.h"
#include "HUD.h"
#include "Dialog.h"
#include "MapWindow.h"
#include "WorldGenerator.h"

#define WORLD_SIZE_ENUM_ENTRIES(X) \
	X(VerySmall, "VerySmall") \
	X(Small, "Small") \
	X(Medium, "Medium") \
	X(Large, "Large") \
	X(VeryLarge, "VeryLarge") \
	X(Huge, "Huge")

ENUM_DECLARE(WorldSize, WORLD_SIZE_ENUM_ENTRIES);

#undef WORLD_SIZE_ENUM_ENTRIES

static string GetTimeAndDate(chrono::system_clock::time_point now)
{
	string timeAndDate;
	time_t tt = chrono::system_clock::to_time_t(now);
	tm localTime = *localtime(&tt);
	timeAndDate += to_string(localTime.tm_mday);
	timeAndDate += ' ';
	switch (localTime.tm_mon)
	{
	case 0:
		timeAndDate += "Jan";
		break;
	case 1:
		timeAndDate += "Feb";
		break;
	case 2:
		timeAndDate += "Mar";
		break;
	case 3:
		timeAndDate += "Apr";
		break;
	case 4:
		timeAndDate += "May";
		break;
	case 5:
		timeAndDate += "Jun";
		break;
	case 6:
		timeAndDate += "Jul";
		break;
	case 7:
		timeAndDate += "Aug";
		break;
	case 8:
		timeAndDate += "Sep";
		break;
	case 9:
		timeAndDate += "Oct";
		break;
	case 10:
		timeAndDate += "Nov";
		break;
	case 11:
		timeAndDate += "Dec";
		break;
	}
	timeAndDate += ' ';
	timeAndDate += to_string(localTime.tm_year + 1900);
	timeAndDate += '\n';
	if (localTime.tm_hour < 10)
	{
		timeAndDate += '0';
	}
	timeAndDate += to_string(localTime.tm_hour);
	timeAndDate += ':';
	if (localTime.tm_min < 10)
	{
		timeAndDate += '0';
	}
	timeAndDate += to_string(localTime.tm_min);
	timeAndDate += ':';
	if (localTime.tm_sec < 10)
	{
		timeAndDate += '0';
	}
	timeAndDate += to_string(localTime.tm_sec);
	return timeAndDate;
}

static vector<string> GetSaveFileList(const string &path)
{
	filesystem::path someDir(path);
	filesystem::directory_iterator end_iter;

	vector<string> saveFiles;
	if (filesystem::exists(someDir) && filesystem::is_directory(someDir))
	{
		for (filesystem::directory_iterator dir_iter(someDir); dir_iter != end_iter; ++dir_iter)
		{
			if (filesystem::is_regular_file(dir_iter->status()))
			{
				string name = dir_iter->path().generic_string();
				if (name.find(".dsv") != string::npos)
				{
					saveFiles.push_back(name);
				}
			}
		}
	}
	return saveFiles;
}

shared_ptr<PlayerPreset> Menu::GetSelectedPreset() const
{
	return static_pointer_cast<PlayerPreset>(mHeroClassSlt->GetCurrentItem());
}

void Menu::LoadPlayerPresets(const string & presetsPath)
{
	// Collect preset files
	vector<string> presetFiles;
	if (filesystem::exists(presetsPath) && filesystem::is_directory(presetsPath))
	{
		for (filesystem::directory_iterator dirIter(presetsPath), endIter; dirIter != endIter; ++dirIter)
		{
			if (filesystem::is_regular_file(dirIter->status()))
			{
				const auto path = dirIter->path();
				if (path.extension().generic_string() == ".json")
				{
					presetFiles.push_back(path.generic_string());
				}
			}
		}
	}
	else
	{
		throw Exception("Player presets folder does not exists!");
	}

	Log() << "Found " << presetFiles.size() << " player preset files!" << LineEnd;

	// Load them
	for (auto & filename : presetFiles)
	{
		rapidjson::Document doc;
		LoadJSON(filename, doc);
		auto root = doc.GetObject();

		auto preset = make_shared<PlayerPreset>();

		// Read name
		auto statName = doc.FindMember("Name");
		if (statName != doc.MemberEnd())
		{
			if (statName->value.IsString())
			{
				preset->name = game->GetStr(statName->value.GetString());
			}
			else
			{
				throw Exception(filename + ". Preset name must be string!");
			}
		}

		// Read money
		auto money = doc.FindMember("Money");
		if (money != doc.MemberEnd())
		{
			if (money->value.IsInt())
			{
				preset->money = abs(money->value.GetInt());
			}
			else
			{
				throw Exception(filename + ". Money must be integer!");
			}
		}

		// Read priority
		auto priority = doc.FindMember("Priority");
		if (priority != doc.MemberEnd())
		{
			if (priority->value.IsInt())
			{
				preset->priority = priority->value.GetInt();
			}
			else
			{
				throw Exception(filename + ". Priority must be integer!");
			}
		}

		// Read stats
		auto stats = doc.FindMember("Stats");
		if (stats != doc.MemberEnd())
		{
			for (auto stat = stats->value.MemberBegin(); stat != stats->value.MemberEnd(); ++stat)
			{
				const string name = stat->name.GetString();

				if (stat->value.IsInt())
				{
					int value = abs(stat->value.GetInt());

					// Clamp stat on some reasonable value
					if (value > 30)
					{
						value = 30;
					}

					if (name == "vitality")
					{
						preset->vitality = value;
					}
					else if (name == "strength")
					{
						preset->strength = value;
					}
					else if (name == "vitality")
					{
						preset->vitality = value;
					}
					else if (name == "dexterity")
					{
						preset->dexterity = value;
					}
					else if (name == "endurance")
					{
						preset->endurance = value;
					}
					else if (name == "intelligence")
					{
						preset->intelligence = value;
					}
					else if (name == "charisma")
					{
						preset->charisma = value;
					}
					else if (name == "luck")
					{
						preset->luck = value;
					}
				}
				else
				{
					throw Exception(filename + ". Stat \"" + name + "\" value must be integer!");
				}
			}
		}

		// Read items
		auto items = doc.FindMember("Items");
		if (items != doc.MemberEnd())
		{
			for (auto item = items->value.MemberBegin(); item != items->value.MemberEnd(); ++item)
			{
				if (!item->value.IsInt())
				{
					throw Exception(filename + ". Item count must be integer!");
				}
				else
				{
					const string name = item->name.GetString();
					const int count = item->value.GetInt();

					preset->startItems.push_back(make_pair(ItemType(name), count));
				}
			}
		}

		// Add standard set of items to each preset
		preset->startItems.push_back({ ItemType::Weapon_Pickaxe, 1 });
		preset->startItems.push_back({ ItemType::Weapon_RevealGun, 1 });
		preset->startItems.push_back({ ItemType::Crystal_SmallPower, 3 });
		preset->startItems.push_back({ ItemType::Crystal_Homeward, 1 });
		preset->startItems.push_back({ ItemType::Weapon_Torch, 2 });
		preset->startItems.push_back({ ItemType::Weapon_ConstructionGun, 1 });
		preset->startItems.push_back({ ItemType::Crystal_RareRed, 3 });

		mPresets.push_back(preset);
	}

	// Sort presets by priority
	auto sortCriteria = [] (const shared_ptr<PlayerPreset>& left, const shared_ptr<PlayerPreset>& right)
	{
		return left->priority < right->priority;
	};

	sort(mPresets.begin(), mPresets.end(), sortCriteria);
}

void Menu::ShowTooltipForItem(ItemType item, int count)
{
	mTooltip->Show(game->GetItemDatabase()->Get(item).GetDescription("", count, IDFF_EXCLUDE_DURABILITY));
}

void Menu::Sync()
{
	shared_ptr<PlayerPreset> preset = GetSelectedPreset();

	mStartItemsTbl->Clear();
	for (auto &kv : preset->startItems)
	{
		auto row = mStartItemsTbl->AddRow();
		auto lbl = dynamic_pointer_cast<Label>(mStartItemsTbl->Get(0, row));
		lbl->SetText(StringBuilder() << "x" << kv.second << " - " << game->GetItemDatabase()->Get(kv.first).GetTranslatedName());
	}

	mVitalityTextLbl->SetText(to_string(preset->vitality));
	mStrengthTextLbl->SetText(to_string(preset->strength));
	mDexterityTextLbl->SetText(to_string(preset->dexterity));
	mEnduranceTextLbl->SetText(to_string(preset->endurance));
	mIntelligenceTextLbl->SetText(to_string(preset->intelligence));
	mCharismaTextLbl->SetText(to_string(preset->charisma));
	mLuckTextLbl->SetText(to_string(preset->luck));
	mMoneyCountLbl->SetText(to_string(preset->money));
}

void Menu::InvokeGenerationThread()
{
	LevelGeneratorParameters genParams;
	Math::SetRNGSeed(mSeed);

	struct WorldPreset
	{
		int minLocationWidth;
		int minLocationHeight;
		int sublocationCount;
	};

	unordered_map<WorldSize, WorldPreset> presets;

	// Load presets
	std::ifstream presetsFile("data/generators/world_presets.json");
	nlohmann::json jsonPresets;
	presetsFile >> jsonPresets;

	for (size_t entry = 0; entry < WorldSize::EntryCount; ++entry)
	{
		WorldSize worldSize = static_cast<WorldSize::Type>(entry);

		const auto jsonPreset = jsonPresets.find(WorldSize::Str[entry]);
		if (jsonPreset != jsonPresets.end())
		{
			WorldPreset preset;
			preset.minLocationWidth = (*jsonPreset)["minWidth"];
			preset.minLocationHeight = (*jsonPreset)["minHeight"];
			preset.sublocationCount = (*jsonPreset)["sublocationCount"];
			presets[worldSize] = preset;
		}
		else
		{
			throw Exception(string("World preset for ") + WorldSize::Str[entry] + " is missing!");
		}
	}

	const WorldSize worldSize = *static_pointer_cast<WorldSize>(mWorldSizeNud->GetCurrentItem());

	const WorldPreset worldPreset = presets[worldSize];

	genParams.mMinLocationWidthInCells = worldPreset.minLocationWidth;
	genParams.mMinLocationHeightInCells = worldPreset.minLocationHeight;
	genParams.mSublocationsCount = worldPreset.sublocationCount;

	if (mDifficultySld->GetCurrentItemIndex() == 0)
	{
		genParams.mDifficulty = Difficulty::Easy;
	}
	else if (mDifficultySld->GetCurrentItemIndex() == 1)
	{
		genParams.mDifficulty = Difficulty::Medium;
	}
	else if (mDifficultySld->GetCurrentItemIndex() == 2)
	{
		genParams.mDifficulty = Difficulty::Hard;
	}
	else if (mDifficultySld->GetCurrentItemIndex() == 3)
	{
		genParams.mDifficulty = Difficulty::Realistic;
	}

	genParams.mPlayerPreset = GetSelectedPreset();

	game->GetHUD()->Reset();

	mNewGameWnd->SetLocalVisibility(false);

	game->ScheduleGenerateWorld(genParams);
}

void Menu::FillResolutionList()
{
	mVideoModeList = sf::VideoMode::getFullscreenModes();
	auto eraseCriteria = [] (const sf::VideoMode &mode) -> bool { return mode.bitsPerPixel < 32; };
	mVideoModeList.erase(std::remove_if(mVideoModeList.begin(), mVideoModeList.end(), eraseCriteria), mVideoModeList.end());
	reverse(mVideoModeList.begin(), mVideoModeList.end());
}

void Menu::MakeEndGamePage()
{
	mEndGame.mWindow = mCanvas->Create<Window>();
	mEndGame.mWindow->SetSize({ 300, 400 });
	mEndGame.mWindow->SetInCenter();
	mEndGame.mWindow->SetLocalVisibility(false);

	mEndGame.mText = mCanvas->Create<Label>();
	mEndGame.mText->SetAlignment(TextAlignment::Center);
	mEndGame.mText->SetSize({ 0, 30 });
	mEndGame.mText->SetText("You died and have no rare red crystals. Game over.");
	mEndGame.mText->SetAutoHeight(true);
	mEndGame.mWindow->AddChild(mEndGame.mText);

	mEndGame.mLoadGameBtn = mCanvas->Create<Button>();
	mEndGame.mLoadGameBtn->SetSize({ 0, 30 });
	mEndGame.mLoadGameBtn->SetText("Load Game");
	mEndGame.mLoadGameBtn->OnClick += [this]
	{
		SetPage(MenuPage::LoadGame);
	};
	mEndGame.mWindow->AddChild(mEndGame.mLoadGameBtn);

	mEndGame.mToMainMenu = mCanvas->Create<Button>();
	mEndGame.mToMainMenu->SetSize({ 0, 30 });
	mEndGame.mToMainMenu->SetText("To Main Menu");
	mEndGame.mToMainMenu->OnClick += [this]
	{
		SetPage(MenuPage::Main);
	};
	mEndGame.mWindow->AddChild(mEndGame.mToMainMenu);

	mEndGame.mExitGameBtn = mCanvas->Create<Button>();
	mEndGame.mExitGameBtn->SetSize({ 0, 30 });
	mEndGame.mExitGameBtn->SetText("Exit Game");
	mEndGame.mExitGameBtn->OnClick += [this]
	{
		mExitBtn->OnClick();
	};
	mEndGame.mWindow->AddChild(mEndGame.mExitGameBtn);
}

Menu::Menu()
{
	auto renderer = game->GetRenderer();
	auto border = 40.0f;
	auto hSpacing = 3.0f;
	auto yOffset = 55.0f;
	auto windowSize = Vec2f(320, 420);
	auto buttonSize = Vec2f(windowSize.x - 2.0f * border, 35);

	LoadPlayerPresets("data/presets/");
	FillResolutionList();

	mCanvas = game->CreateCanvas();
	mCanvas->SetLayer(Scene::TopmostLayer - 1);

	mBackground = mCanvas->Create<Image>();
	mBackground->SetTexture(game->GetRenderer()->RequestTexture("data/textures/gui/menu_back.png"));
	mBackground->SetSize(game->VirtualScreen.getSize());

	auto theme = mCanvas->GetTheme();
	// Main Window
	{
		mMainWnd = mCanvas->Create<Window>();
		mMainWnd->SetSize(windowSize);
		mMainWnd->SetInCenter();
		mMainWnd->SetCaption("The Dwarf Adventure");

		mContinueBtn = mCanvas->Create<Button>();
		mContinueBtn->SetText(game->GetStr("continueButton"));
		mContinueBtn->SetSize(buttonSize);
		mContinueBtn->OnClick += [this] { SetVisible(false); };
		mMainWnd->AddChild(mContinueBtn);

		mNewGameBtn = mCanvas->Create<Button>();
		mNewGameBtn->SetText(game->GetStr("startButton"));
		mNewGameBtn->SetSize(buttonSize);
		mNewGameBtn->OnClick += [this] { SetPage(MenuPage::NewGame); };
		mMainWnd->AddChild(mNewGameBtn);

		mSaveGameBtn = mCanvas->Create<Button>();
		mSaveGameBtn->SetText(game->GetStr("saveButton"));
		mSaveGameBtn->SetSize(buttonSize);
		mSaveGameBtn->OnClick += [this] { SetPage(MenuPage::SaveGame); };
		mMainWnd->AddChild(mSaveGameBtn);

		mLoadGameBtn = mCanvas->Create<Button>();
		mLoadGameBtn->SetText(game->GetStr("loadButton"));
		mLoadGameBtn->SetSize(buttonSize);
		mLoadGameBtn->OnClick += [this]
		{
			SetPage(MenuPage::LoadGame);

		};
		mMainWnd->AddChild(mLoadGameBtn);

		mEditorBtn = mCanvas->Create<Button>();
		mEditorBtn->SetText(game->GetStr("editorButton"));
		mEditorBtn->SetSize(buttonSize);
#ifdef DEMO_VERSION
		mEditorBtn->SetEnabled(false);
		mEditorBtn->OnHover += [this] { mTooltip->Show("Not available in Demo!"); };
#else
		mEditorBtn->SetEnabled(true);
		mEditorBtn->OnClick += [this]
		{
			game->GetEditor()->SetActive(true);
			SetVisible(false);
		};
#endif
		mMainWnd->AddChild(mEditorBtn);

		mOptionsBtn = mCanvas->Create<Button>();
		mOptionsBtn->SetText(game->GetStr("optionsButton"));
		mOptionsBtn->SetSize(buttonSize);
		mOptionsBtn->OnClick += [this] { SetPage(MenuPage::Options); };
		mMainWnd->AddChild(mOptionsBtn);

		mAuthorsBtn = mCanvas->Create<Button>();
		mAuthorsBtn->SetText(game->GetStr("authorsButton"));
		mAuthorsBtn->SetSize(buttonSize);
		mAuthorsBtn->OnClick += [this] { SetPage(MenuPage::Authors); };
		mMainWnd->AddChild(mAuthorsBtn);

		mExitBtn = mCanvas->Create<Button>();
		mExitBtn->SetText(game->GetStr("exitButton"));
		mExitBtn->SetSize(buttonSize);
		mExitBtn->OnClick += [this] { game->Shutdown(); };
		mMainWnd->AddChild(mExitBtn);
	}

	// New Game Window
	{
		mNewGameWnd = mCanvas->Create<Window>();
		mNewGameWnd->SetSize({ 520, game->VirtualScreen.getSize().y - 40 });
		mNewGameWnd->SetInCenter();
		mNewGameWnd->SetCaption(game->GetStr("newGameTitle"));

		mNewGameWnd->GetLayout().ConfigureRow(Layout::TemplateRow, 2);

		mDAWGVersionLbl = mCanvas->Create<Label>();
		mDAWGVersionLbl->SetText(game->GetStr("dawgVersion"));
		mNewGameWnd->AddChild(mDAWGVersionLbl);

		mDAWGVersionNumberLbl = mCanvas->Create<Label>();
		mDAWGVersionNumberLbl->SetText(WorldGenerator::Version);
		mDAWGVersionNumberLbl->SetAlignment(TextAlignment::Center);
		mNewGameWnd->AddChild(mDAWGVersionNumberLbl);

		mSeedLbl = mCanvas->Create<Label>();
		mSeedLbl->SetText("Seed");
		mNewGameWnd->AddChild(mSeedLbl);

		mNewSeedBtn = mCanvas->Create<Button>();
		mNewSeedBtn->SetText(to_string(mSeed));
		mNewSeedBtn->SetSize({ 0, 25 });
		mNewSeedBtn->OnClick += [this]
		{
			mSeed = Math::Rnd(0, INT_MAX);
			mNewSeedBtn->SetText(to_string(mSeed));
		};
		mNewSeedBtn->OnHover += [this] { mTooltip->Show(game->GetStr("seedTooltip")); };
		mNewGameWnd->AddChild(mNewSeedBtn);

		mWorldSizeLbl = mCanvas->Create<Label>();
		mWorldSizeLbl->SetText(game->GetStr("worldSize"));
		mNewGameWnd->AddChild(mWorldSizeLbl);

		mWorldSizeNud = mCanvas->Create<SlideList>();
		mWorldSizeNud->SetSize({ 0, 30 });
		mWorldSizeNud->AddItem(game->GetStr("verySmallWorld"), make_shared<WorldSize>(WorldSize::VerySmall));
		mWorldSizeNud->AddItem(game->GetStr("smallWorld"), make_shared<WorldSize>(WorldSize::Small));
		mWorldSizeNud->AddItem(game->GetStr("mediumWorld"), make_shared<WorldSize>(WorldSize::Medium));
		mWorldSizeNud->AddItem(game->GetStr("largeWorld"), make_shared<WorldSize>(WorldSize::Large));
		mWorldSizeNud->AddItem(game->GetStr("veryLargeWorld"), make_shared<WorldSize>(WorldSize::VeryLarge));
		mWorldSizeNud->AddItem(game->GetStr("hugeWorld"), make_shared<WorldSize>(WorldSize::Huge));
		mWorldSizeNud->SetCurrentItem(0);
		mWorldSizeNud->OnHover += [this] { mTooltip->Show(game->GetStr("worldSizeTooltip")); };
		mNewGameWnd->AddChild(mWorldSizeNud);

		mDifficultyLbl = mCanvas->Create<Label>();
		mDifficultyLbl->SetText(game->GetStr("difficulty"));
		mNewGameWnd->AddChild(mDifficultyLbl);

		mDifficultySld = mCanvas->Create<SlideList>();
		mDifficultySld->AddItem(game->GetStr("easy"), nullptr);
		mDifficultySld->AddItem(game->GetStr("medium"), nullptr);
		mDifficultySld->AddItem(game->GetStr("hard"), nullptr);
		mDifficultySld->AddItem(game->GetStr("realistic"), nullptr);
		mDifficultySld->SetSize({ 100, 30 });
		mDifficultySld->OnCurrentItemChanged += [this] { Sync(); };
		mDifficultySld->SetCurrentItem(0);
		mDifficultySld->OnHover += [this]
		{
			if (mDifficultySld->GetCurrentItemIndex() == 0)
			{
				mTooltip->Show(game->GetStr("easyTooltip"));
			}
			else if (mDifficultySld->GetCurrentItemIndex() == 1)
			{
				mTooltip->Show(game->GetStr("mediumTooltip"));
			}
			else if (mDifficultySld->GetCurrentItemIndex() == 2)
			{
				mTooltip->Show(game->GetStr("hardTooltip"));
			}
			else if (mDifficultySld->GetCurrentItemIndex() == 3)
			{
				mTooltip->Show(game->GetStr("realisticTooltip"));
			}
		};
		mNewGameWnd->AddChild(mDifficultySld);

		mHeroClassLbl = mCanvas->Create<Label>();
		mHeroClassLbl->SetText(game->GetStr("heroClass"));
		mNewGameWnd->AddChild(mHeroClassLbl);

		mHeroClassSlt = mCanvas->Create<SlideList>();
		mHeroClassSlt->SetSize({ 0, 30 });

		for (auto & preset : mPresets)
		{
			mHeroClassSlt->AddItem(preset->name, preset);
		}

		mHeroClassSlt->SetCurrentItem(0);
		mHeroClassSlt->OnCurrentItemChanged += [this] { Sync(); };
		mHeroClassSlt->OnHover += [this] { mTooltip->Show(game->GetStr("heroClassDesc")); };
		mNewGameWnd->AddChild(mHeroClassSlt);

		mMoneyLbl = mCanvas->Create<Label>();
		mMoneyLbl->SetText(game->GetStr("startMoney"));
		mNewGameWnd->AddChild(mMoneyLbl);

		mMoneyCountLbl = mCanvas->Create<Label>();
		mMoneyCountLbl->SetAlignment(TextAlignment::Center);
		mNewGameWnd->AddChild(mMoneyCountLbl);

		mVitalityLbl = mCanvas->Create<Label>();
		mVitalityLbl->SetText(game->GetStr("vitality"));
		mVitalityLbl->OnHover += [this] { mTooltip->Show(game->GetStr("vitalityDesc")); };
		mNewGameWnd->AddChild(mVitalityLbl);

		mVitalityTextLbl = mCanvas->Create<Label>();
		mVitalityTextLbl->SetAlignment(TextAlignment::Center);
		mNewGameWnd->AddChild(mVitalityTextLbl);

		mStrengthLbl = mCanvas->Create<Label>();
		mStrengthLbl->SetText(game->GetStr("strength"));
		mStrengthLbl->OnHover += [this] { mTooltip->Show(game->GetStr("strengthDesc")); };
		mNewGameWnd->AddChild(mStrengthLbl);

		mStrengthTextLbl = mCanvas->Create<Label>();
		mStrengthTextLbl->SetAlignment(TextAlignment::Center);
		mNewGameWnd->AddChild(mStrengthTextLbl);

		mDexterityLbl = mCanvas->Create<Label>();
		mDexterityLbl->SetText(game->GetStr("dexterity"));
		mDexterityLbl->OnHover += [this] { mTooltip->Show(game->GetStr("dexterityDesc")); };
		mNewGameWnd->AddChild(mDexterityLbl);

		mDexterityTextLbl = mCanvas->Create<Label>();
		mDexterityTextLbl->SetAlignment(TextAlignment::Center);
		mNewGameWnd->AddChild(mDexterityTextLbl);

		mEnduranceLbl = mCanvas->Create<Label>();
		mEnduranceLbl->SetText(game->GetStr("endurance"));
		mEnduranceLbl->OnHover += [this] { mTooltip->Show(game->GetStr("enduranceDesc")); };
		mNewGameWnd->AddChild(mEnduranceLbl);

		mEnduranceTextLbl = mCanvas->Create<Label>();
		mEnduranceTextLbl->SetAlignment(TextAlignment::Center);
		mNewGameWnd->AddChild(mEnduranceTextLbl);

		mIntelligenceLbl = mCanvas->Create<Label>();
		mIntelligenceLbl->SetText(game->GetStr("intelligence"));
		mIntelligenceLbl->OnHover += [this] { mTooltip->Show(game->GetStr("intelligenceDesc")); };
		mNewGameWnd->AddChild(mIntelligenceLbl);

		mIntelligenceTextLbl = mCanvas->Create<Label>();
		mIntelligenceTextLbl->SetAlignment(TextAlignment::Center);
		mNewGameWnd->AddChild(mIntelligenceTextLbl);

		mCharismaLbl = mCanvas->Create<Label>();
		mCharismaLbl->SetText(game->GetStr("charisma"));
		mCharismaLbl->OnHover += [this] { mTooltip->Show(game->GetStr("charismaDesc")); };
		mNewGameWnd->AddChild(mCharismaLbl);

		mCharismaTextLbl = mCanvas->Create<Label>();
		mCharismaTextLbl->SetAlignment(TextAlignment::Center);
		mNewGameWnd->AddChild(mCharismaTextLbl);

		mLuckLbl = mCanvas->Create<Label>();
		mLuckLbl->SetText(game->GetStr("luck"));
		mLuckLbl->OnHover += [this] { mTooltip->Show(game->GetStr("luckDesc")); };
		mNewGameWnd->AddChild(mLuckLbl);

		mLuckTextLbl = mCanvas->Create<Label>();
		mLuckTextLbl->SetAlignment(TextAlignment::Center);
		mNewGameWnd->AddChild(mLuckTextLbl);

		mNewGameWnd->GetLayout().ConfigureRow(Layout::TemplateRow, 1);

		mStartItemsTbl = mCanvas->Create<Table>();
		mStartItemsTbl->AddColumn(game->GetStr("startItems"));
		mStartItemsTbl->SetSize({ 0, 200 });

		mStartItemsTbl->OnRowHovered += [this]
		{
			int index = mStartItemsTbl->GetHoveredRowIndex();
			if (index >= 0)
			{
				auto preset = GetSelectedPreset();

				ShowTooltipForItem(preset->startItems.at(index).first, preset->startItems.at(index).second);
			}
		};
		mNewGameWnd->AddChild(mStartItemsTbl);

		mMapSizeWarning = mCanvas->Create<Label>();
		mMapSizeWarning->SetText(" ");
		mMapSizeWarning->SetAlignment(TextAlignment::Center);
		mMapSizeWarning->SetAutoHeight(true);
		mNewGameWnd->AddChild(mMapSizeWarning);

		mNewGameWnd->GetLayout().ConfigureRow(Layout::TemplateRow, 2);

		mGenerateBtn = mCanvas->Create<Button>();
		mGenerateBtn->SetText(game->GetStr("generate"));
		mGenerateBtn->SetSize(buttonSize);
		mGenerateBtn->OnClick += [this] { InvokeGenerationThread(); };
		mNewGameWnd->AddChild(mGenerateBtn);

		mNewGameBackBtn = mCanvas->Create<Button>();
		mNewGameBackBtn->SetText(game->GetStr("back"));
		mNewGameBackBtn->SetSize(buttonSize);
		mNewGameBackBtn->OnClick += [this] { SetPage(MenuPage::Main); };
		mNewGameWnd->AddChild(mNewGameBackBtn);
	}

	// Options Window
	{
		mOptionsWnd = mCanvas->Create<Window>();
		mOptionsWnd->SetSize({ 400, 420 });
		mOptionsWnd->SetInCenter();
		mOptionsWnd->SetCaption(game->GetStr("optionsTitle"));

		mResolutionLbl = mCanvas->Create<Label>();
		mResolutionLbl->SetText(game->GetStr("resolution"));
		mResolutionLbl->SetAlignment(TextAlignment::Center);
		mOptionsWnd->AddChild(mResolutionLbl);

		mResolutionLst = mCanvas->Create<SlideList>();
		mResolutionLst->SetSize(buttonSize);
		for (auto &videoMode : mVideoModeList)
		{
			mResolutionLst->AddItem(StringBuilder() << videoMode.width << "x" << videoMode.height << "x"
				<< videoMode.bitsPerPixel,
				nullptr);
		}
		mResolutionLst->OnCurrentItemChanged += [this]
		{
			game->GetRenderer()->SetResolution(mVideoModeList[mResolutionLst->GetCurrentItemIndex()]);
		};
		mOptionsWnd->AddChild(mResolutionLst);

		mWindowModeLbl = mCanvas->Create<Label>();
		mWindowModeLbl->SetText(game->GetStr("windowMode"));
		mWindowModeLbl->SetAlignment(TextAlignment::Center);
		mOptionsWnd->AddChild(mWindowModeLbl);

		mWindowModeLst = mCanvas->Create<SlideList>();
		mWindowModeLst->SetSize(buttonSize);
		mWindowModeLst->AddItem(game->GetStr("windowed"), nullptr);
		mWindowModeLst->AddItem(game->GetStr("fullscreen"), nullptr);
		mWindowModeLst->SetCurrentItem(game->GetRenderer()->GetConfig().mFullscreen ? 1 : 0);
		mWindowModeLst->OnCurrentItemChanged += [this]
		{
			game->GetRenderer()->SetFullscreen(mWindowModeLst->GetCurrentItemIndex() == 0 ? false : true);
		};
		mOptionsWnd->AddChild(mWindowModeLst);

		mMusicVolumeLbl = mCanvas->Create<Label>();
		mMusicVolumeLbl->SetText(game->GetStr("musicVolume"));
		mMusicVolumeLbl->SetAlignment(TextAlignment::Center);
		mOptionsWnd->AddChild(mMusicVolumeLbl);

		mMusicVolumeNud = mCanvas->Create<NumericUpDown>();
		mMusicVolumeNud->SetMaxLimit(100);
		mMusicVolumeNud->SetSize(buttonSize);
		mMusicVolumeNud->SetValue(10);
		mMusicVolumeNud->SetStep(5);
		mMusicVolumeNud->OnChange += [this]
		{
			game->GetMusic().setVolume(mMusicVolumeNud->GetValue());
		};
		mOptionsWnd->AddChild(mMusicVolumeNud);

		mSoundVolumeLbl = mCanvas->Create<Label>();
		mSoundVolumeLbl->SetText(game->GetStr("soundVolume"));
		mSoundVolumeLbl->SetAlignment(TextAlignment::Center);
		mOptionsWnd->AddChild(mSoundVolumeLbl);

		mSoundVolumeNud = mCanvas->Create<NumericUpDown>();
		mSoundVolumeNud->SetMaxLimit(100);
		mSoundVolumeNud->SetSize(buttonSize);
		mSoundVolumeNud->SetValue(100);
		mSoundVolumeNud->SetStep(5);
		mSoundVolumeNud->OnChange += [this]
		{
			sf::Listener::setGlobalVolume(mSoundVolumeNud->GetValue());
		};
		mOptionsWnd->AddChild(mSoundVolumeNud);

		mShowPerformanceChk = mCanvas->Create<Checkbox>();
		mShowPerformanceChk->SetText(game->GetStr("perfWindow"));
		mShowPerformanceChk->OnCheckedChanged += [this]
		{
			game->GetOverlay()->SetVisible(mShowPerformanceChk->IsChecked());
		};
		mOptionsWnd->AddChild(mShowPerformanceChk);

		mControlsLbl = mCanvas->Create<Label>();
		mControlsLbl->SetText(game->GetStr("controls"));
		mControlsLbl->SetAlignment(TextAlignment::Center);
		mOptionsWnd->AddChild(mControlsLbl);

		mControlsLbx = mCanvas->Create<ListBox>();
		mControlsLbx->SetSize({ buttonSize.x, 150 });
		auto &controls = game->GetControls();
		for (int i = 0; i < (int)ControlKey::Count; ++i)
		{
			mControlsLbx->AddItem(StringBuilder() << controls.GetKeyDescription((ControlKey::Type)i) << " - "
				<< controls.GetKeyName((ControlKey::Type)i));
		}
		mControlsLbx->SetPosition({ border, yOffset + 100 + 2 * hSpacing });
		mControlsLbx->SetScrollTexture(renderer->RequestTexture("data/textures/gui/scrollbar.png"));
		mControlsLbx->OnSelectedItemChanged += [this] { mWaitKey = true; };
		mOptionsWnd->AddChild(mControlsLbx);

		mOptionsBackBtn = mCanvas->Create<Button>();
		mOptionsBackBtn->SetText(game->GetStr("back"));
		mOptionsBackBtn->SetSize(buttonSize);
		mOptionsBackBtn->OnClick += [this] { SetPage(MenuPage::Main); };
		mOptionsWnd->AddChild(mOptionsBackBtn);
	}

	// Authors
	{
		mAuthorsWnd = mCanvas->Create<Window>();
		mAuthorsWnd->SetSize(windowSize);
		mAuthorsWnd->SetInCenter();
		mAuthorsWnd->SetCaption(game->GetStr("authorsTitle"));

		mAuthorsLbl = mCanvas->Create<Label>();
		mAuthorsLbl->SetPosition({ border / 2, 40 });
		mAuthorsLbl->SetSize({ windowSize.x - border, windowSize.y - 150 });
		mAuthorsLbl->SetAutoHeight(true);
		mAuthorsLbl->SetText(game->GetStr("authors"));
		mAuthorsLbl->SetAlignment(TextAlignment::Center);
		mAuthorsWnd->AddChild(mAuthorsLbl);

		mAuthorsBackBtn = mCanvas->Create<Button>();
		mAuthorsBackBtn->SetPosition({ border, windowSize.y - buttonSize.y - 20 });
		mAuthorsBackBtn->SetText(game->GetStr("back"));
		mAuthorsBackBtn->SetSize(buttonSize);
		mAuthorsBackBtn->OnClick += [this] { SetPage(MenuPage::Main); };
		mAuthorsWnd->AddChild(mAuthorsBackBtn);
	}

	// Save
	{
		mSaveGameWnd = mCanvas->Create<Window>();
		mSaveGameWnd->SetSize(windowSize);
		mSaveGameWnd->SetInCenter();
		mSaveGameWnd->SetCaption(game->GetStr("saveTitle"));

		for (unsigned int i = 0; i < SaveLoadSlotCount; ++i)
		{
			mSaveGameWnd->GetLayout().PushRow(2);
			mSaveSlotBtn[i] = mCanvas->Create<Button>();
			mSaveSlotBtn[i]->SetSize({ 0, buttonSize.y });
			mSaveSlotBtn[i]->OnClick += [this, i]
			{
				string fn = StringBuilder("data/save/Slot ") << i << ".dsv";
				game->Save(fn);
				mSaveSlotDescLbl[i]->SetText(GetTimeAndDate(filesystem::last_write_time(fn)));
			};
			mSaveGameWnd->AddChild(mSaveSlotBtn[i]);

			mSaveSlotDescLbl[i] = mCanvas->Create<Label>();
			mSaveSlotDescLbl[i]->SetSize({ 0, buttonSize.y });
			mSaveSlotDescLbl[i]->SetAlignment(TextAlignment::Center);
			mSaveGameWnd->AddChild(mSaveSlotDescLbl[i]);
		}
		mSaveGameWnd->GetLayout().PushRow(1);
		mSaveGameBackBtn = mCanvas->Create<Button>();
		mSaveGameBackBtn->SetPosition({ border, windowSize.y - buttonSize.y - 20 });
		mSaveGameBackBtn->SetText(game->GetStr("back"));
		mSaveGameBackBtn->SetSize(buttonSize);
		mSaveGameBackBtn->OnClick += [this] { SetPage(MenuPage::Main); };
		mSaveGameWnd->AddChild(mSaveGameBackBtn);
	}

	// Load
	{
		mLoadGameWnd = mCanvas->Create<Window>();
		mLoadGameWnd->SetSize(windowSize);
		mLoadGameWnd->SetInCenter();
		mLoadGameWnd->SetCaption(game->GetStr("loadTitle"));

		for (unsigned int i = 0; i < SaveLoadSlotCount; ++i)
		{
			mLoadGameWnd->GetLayout().PushRow(2);
			mLoadSlotBtn[i] = mCanvas->Create<Button>();
			mLoadSlotBtn[i]->SetSize({ 0, buttonSize.y });
			mLoadSlotBtn[i]->OnClick += [this, i]
			{
				auto saveFiles = GetSaveFileList("data/save/");
				game->Load(saveFiles[i]);
			};
			mLoadGameWnd->AddChild(mLoadSlotBtn[i]);

			mLoadSlotDescLbl[i] = mCanvas->Create<Label>();
			mLoadSlotDescLbl[i]->SetSize({ 0, buttonSize.y });
			mLoadSlotDescLbl[i]->SetAlignment(TextAlignment::Center);
			mLoadGameWnd->AddChild(mLoadSlotDescLbl[i]);
		}

		mLoadGameWnd->GetLayout().PushRow(1);
		mLoadGameBackBtn = mCanvas->Create<Button>();
		mLoadGameBackBtn->SetPosition({ border, windowSize.y - buttonSize.y - 20 });
		mLoadGameBackBtn->SetText(game->GetStr("back"));
		mLoadGameBackBtn->SetSize(buttonSize);
		mLoadGameBackBtn->OnClick += [this] { SetPage(MenuPage::Main); };
		mLoadGameWnd->AddChild(mLoadGameBackBtn);
	}

	{
		mProgressWindow = mCanvas->Create<Window>();
		mProgressWindow->SetSize({ 500, 350 });
		mProgressWindow->SetInCenter();
		mProgressWindow->SetAutoLayoutingEnabled(false);
		mProgressWindow->SetLocalVisibility(false);

		Layout layout { { 20, 20 }, 460, 1, 5 };

		mProgressPleaseWaitLabel = mCanvas->Create<Label>();
		mProgressPleaseWaitLabel->SetAlignment(TextAlignment::Center);
		mProgressPleaseWaitLabel->SetSize({ 0, 30 });
		mProgressPleaseWaitLabel->SetText("Please wait while new world being generated...");
		layout.Pack(mProgressPleaseWaitLabel);
		mProgressWindow->AddChild(mProgressPleaseWaitLabel);

		mProgressLabel = mCanvas->Create<Label>();
		mProgressLabel->SetAutoHeight(true);
		mProgressLabel->SetAlignment(TextAlignment::Center);
		layout.Pack(mProgressLabel);
		mProgressWindow->AddChild(mProgressLabel);
	}

	MakeEndGamePage();

	mTooltip = mCanvas->Create<Tooltip>();
	mTooltip->SetSize({ 400, 280 });

	mDifficultySld->OnCurrentItemChanged();



	SetPage(MenuPage::Main);

	Sync();

	Log() << "Menu successfully initialized!" << LineEnd;
}

Menu::~Menu()
{
	DBG_ECHO();
}

void Menu::SetVisible(bool state)
{
	mCanvas->SetVisible(state);
	SetPage(MenuPage::Main);
}

bool Menu::IsVisible() const
{
	return mCanvas->IsVisible();
}

void Menu::SetPage(const MenuPage &page)
{
	mPage = page;
	mMainWnd->SetLocalVisibility(mPage == MenuPage::Main);
	mOptionsWnd->SetLocalVisibility(mPage == MenuPage::Options);
	mAuthorsWnd->SetLocalVisibility(mPage == MenuPage::Authors);
	mSaveGameWnd->SetLocalVisibility(mPage == MenuPage::SaveGame);
	mLoadGameWnd->SetLocalVisibility(mPage == MenuPage::LoadGame);
	mNewGameWnd->SetLocalVisibility(mPage == MenuPage::NewGame);
	mEndGame.mWindow->SetLocalVisibility(mPage == MenuPage::EndGame);
	if (page == MenuPage::SaveGame)
	{
		auto saveFiles = GetSaveFileList("data/save/");
		for (unsigned int i = 0; i < SaveLoadSlotCount; ++i)
		{
			mSaveSlotBtn[i]->SetText(StringBuilder("Slot ") << i);
			if (i < saveFiles.size())
			{
				mSaveSlotDescLbl[i]->SetText(GetTimeAndDate(filesystem::last_write_time(saveFiles[i])));
			}
			else
			{
				mSaveSlotDescLbl[i]->SetText(game->GetStr("empty"));
			}
		}
	}
	if (page == MenuPage::LoadGame)
	{
		auto saveFiles = GetSaveFileList("data/save/");
		for (unsigned int i = 0; i < SaveLoadSlotCount; ++i)
		{
			if (i < saveFiles.size()) 
			{
				mLoadSlotBtn[i]->SetText(StringBuilder("Slot ") << i);
				mLoadSlotBtn[i]->SetEnabled(true);
				mLoadSlotDescLbl[i]->SetText(GetTimeAndDate(filesystem::last_write_time(saveFiles[i])));
			}
			else
			{
				mLoadSlotBtn[i]->SetText(StringBuilder("Empty") << i);
				mLoadSlotBtn[i]->SetEnabled(false);
				mLoadSlotDescLbl[i]->SetText("Empty");
			}
		}
	}
}

void Menu::Update()
{
	auto level = game->GetLevel();
	auto input = game->GetInput();
	auto &controls = game->GetControls();
	mContinueBtn->SetEnabled(level != nullptr);
	mSaveGameBtn->SetEnabled(level != nullptr);
	mBackground->SetLocalVisibility(level == nullptr);

	--mWaitKeyBlinkTimer;
	if (mWaitKey)
	{
		auto pressedKeys = input->GetPressedKeys();
		auto selected = mControlsLbx->GetSelectedItemNumber();
		auto ck = (ControlKey::Type)selected;
		string item = controls.GetKeyDescription(ck) + " - ";
		if (pressedKeys.size())
		{
			if (pressedKeys[0] != sf::Keyboard::Escape)
			{
				item += input->GetKeyName(pressedKeys[0]);
				controls.SetKey(ck, pressedKeys[0]);
				mControlsLbx->ResetSelection();
			}
			else
			{
				item += controls.GetKeyName(ck);
			}
			mWaitKey = false;
		}
		else
		{
			if (mWaitKeyBlinkTimer > 20 && mWaitKeyBlinkTimer < 40)
			{
				item += " [ ]";
			}
			else if (mWaitKeyBlinkTimer > 0 && mWaitKeyBlinkTimer < 20)
			{
				item += "[   ]";
			}
			else if (mWaitKeyBlinkTimer < 0)
			{
				mWaitKeyBlinkTimer = 40;
			}
		}

		for (int i = 0; i < (int)ControlKey::Count; ++i)
		{
			mControlsLbx->SetItem(i,
				controls.GetKeyDescription((ControlKey::Type)i) + " - " + controls.GetKeyName((ControlKey::Type)i));
		}
		mControlsLbx->SetItem(selected, item);
	}
}

void Menu::SetProgressMessage(const string & message)
{
	mProgressLabel->SetText(message);
}

void Menu::ShowProgressWindow(bool state)
{
	mProgressWindow->SetLocalVisibility(state);
}
