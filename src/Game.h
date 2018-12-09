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
#include "Controls.h"

class Text;
class Renderer;
class Level;
class Physics;
class Input;
class Editor;
class Menu;
class Canvas;
class GameScene;
class Scene;
class Label;
class HUD;
class StatWindow;
class InventoryGUI;
class Sound;
class Texture;
class Window;
class DialogNode;
class DialogGUI;
class Dialog;
class LootWindow;
class BarterWindow;
class Journal;
class ItemDatabase;
class Bench;
class Console;
class InteractionMenu;
class Game;
class MapWindow;
class BookReader;
class Encyclopedia;
class NameDatabase;
class ResourceDatabase;
class DialogType;
struct LevelGeneratorParameters;
class WorldGenerator;

enum class Difficulty
{
	Easy,
	Medium,
	Hard,
	Realistic,
};

enum class WorldState
{
	// World does not exists
	Destroyed,
	// World needs to be destoyed
	NeedsToBeDestroyed,
	// World needs to be generated
	NeedsToBeGenerated,
	// World is generating in other thread
	Generating,
	// World is ready to update
	Generated,
};

class Game
{
public:
	Game();
	~Game();

	void MainLoop();
	double GetTime();
	void SortScenes();
	void SortCanvases();
	shared_ptr<Canvas> CreateCanvas();
	shared_ptr<Scene> CreateScene();
	const vector<shared_ptr<Scene>> &GetSceneList() const;
	shared_ptr<Level> &GetLevel();
	Physics *GetPhysics();
	Renderer *GetRenderer();
	Input * GetInput();
	Editor* GetEditor();
	Menu* GetMenu();

	HUD* GetHUD();
	string GetLanguagePath() const;
	void Shutdown();
	void Save(const string &path);
	void Load(const string &path);
	shared_ptr<sf::SoundBuffer> GetSoundBuffer(const string &filename);
	string GetSoundBufferFileName(const shared_ptr<sf::SoundBuffer> &buf);
	void RegisterSound(const string &name, const shared_ptr<sf::SoundBuffer> &buf);
	Controls &GetControls();
	vector<shared_ptr<Canvas>> &GetCanvasList();
	void RemoveScene(const shared_ptr<Scene> &scene);
	shared_ptr<Sound> EmitSound2D(const string &buf);
	Canvas* GetOverlay();
	const string& GetStr(const string &name) const;
	sf::Music &GetMusic();
	ItemDatabase* GetItemDatabase();
	void AddScene(const shared_ptr<Scene> &scene);
	shared_ptr<Scene> LoadPrefab(const string &path);
	void SavePrefab(const string &path, shared_ptr<Scene> &prefab);
	int GetTotalCanvasCount() const;
	int GetTotalWidgetCount() const;
	int GetVisibleCanvasCount() const;
	int GetVisibleWidgetCount() const;
	void RaiseDialog(const shared_ptr<Dialog>& dialog);
	WorldState GetWorldState() const;
	/// World will be destructed before next frame
	void ScheduleWorldDestruction();
	/// World will be generated before next frame
	void ScheduleGenerateWorld(const LevelGeneratorParameters & parameters);
	/// Returns time from game start
	float GetTimeSeconds();
	MapWindow *GetMapWindow() const;
	NameDatabase *GetNameDatabase() const;
	BookReader *GetBookReader() const;
	ResourceDatabase* GetResourceDatabase() const;
	Bench* GetBench() const;
	Journal* GetJournal() const;
	LootWindow* GetLootWindow() const;
	BarterWindow* GetBarterWindow() const;
	InventoryGUI* GetInventoryGUI() const;
	DialogGUI* GetDialogWindow() const;
	StatWindow* GetStatWindow() const;
	Encyclopedia * GetEncyclopedia() const;
	InteractionMenu* GetInteractionMenu() const;
	unordered_map<string, string> & GetCurrentLocalization();
	void SaveLocalization();
	Config& GetCurrentLocalizationConfig();

	// Built-in debug menu uses this data to control state of the game
	struct TDebug
	{
		bool showHitboxes { false };
		bool dumpExplodeTexture { false };
		bool showPaths { false };
		bool showWalkableVertices { false };
		bool allowFastTravelOnUndiscoveredLocations { false };
		bool showBotVisionTriangle { false };
		bool verboseUnstableZoneAnalysisOutput { false };
		bool gravityDisabled { false };
		bool terrainRenderingDisabled { false };
		bool useMultithreadedWorldGenerator { false };
		bool showPlayerFogOfWarRevealRays { false };
		bool disablePlayerTerrainCollisions { false };
		bool disableFogOfWar { false };
	} Debug;

	sf::View VirtualScreen = sf::View(sf::FloatRect(0, 0, 1000, 1000));

	// Main mutex used for thread syncing
	std::mutex SyncMutex;
private:
	shared_ptr<Label> mFPSText;
	shared_ptr<Label> mEarlyBuild;

	unique_ptr<Renderer> mRenderer;
	shared_ptr<Level> mLevel;
	unique_ptr<Physics> mPhysics;
	unique_ptr<Input> mInput;
	unique_ptr<Editor> mEditor;
	unique_ptr<Menu> mMenu;
	unique_ptr<HUD> mHUD;
	unique_ptr<InventoryGUI> mInventoryGUI;
	unique_ptr<StatWindow> mStatWindow;
	unique_ptr<DialogGUI> mDialogWindow;
	unique_ptr<LootWindow> mLootWindow;
	unique_ptr<BarterWindow> mBarterWindow;
	unique_ptr<Journal> mJournal;
	unique_ptr<ItemDatabase> mItemDatabase;
	unique_ptr<Bench> mBench;
	unique_ptr<Console> mConsole;
	unique_ptr<InteractionMenu> mInteractionMenu;
	unique_ptr<MapWindow> mMapWindow;
	unique_ptr<NameDatabase> mNameDatabase;
	unique_ptr<BookReader> mBookReader;
	unique_ptr<Encyclopedia> mEncyclopedia;
	unique_ptr<ResourceDatabase> mResourceDb;
	vector<shared_ptr<Scene>> mSceneList;
	vector<shared_ptr<Canvas>> mCanvasList;
	unordered_map<string, shared_ptr<sf::SoundBuffer>> mSoundBufferList;
	shared_ptr<Canvas> mOverlay;
	string mLanguagePath;
	Controls mControls;
	sf::Music mMusic;

	bool mActive;
	std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
	// map of all dialogs in the game

	Config mLocalization;
	void SaveSettings();
	// list of playing sounds (when sound becomes stopped - it will be deleted
	// from the list)
	vector<shared_ptr<Sound>> mSoundList;
	void UpdateSounds();

	WorldState mWorldState { WorldState::Destroyed };
	atomic<bool> mWorldGenerationIsDone { false };
	unique_ptr<WorldGenerator> mWorldGenerator;
	void CheckWorldState();
};
