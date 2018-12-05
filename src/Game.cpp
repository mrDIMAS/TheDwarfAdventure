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
#include "Game.h"
#include "Level.h"
#include "Scene.h"
#include "Canvas.h"
#include "Level.h"
#include "Collision.h"
#include "Renderer.h"
#include "Input.h"
#include "Editor.h"
#include "Menu.h"
#include "Serializer.h"
#include "Debris.h"
#include "Weapon.h"
#include "Chest.h"
#include "Label.h"
#include "Game.h"
#include "HUD.h"
#include "StatWindow.h"
#include "InventoryGUI.h"
#include "Hook.h"
#include "Drill.h"
#include "PlasmaDrill.h"
#include "Shotgun.h"
#include "GrenadeLauncher.h"
#include "RocketLauncher.h"
#include "Monster.h"
#include "Sound.h"
#include "SoilGun.h"
#include "Window.h"
#include "Dialog.h"
#include "QuestBase.h"
#include "MotherCrystal.h"
#include "Ladder.h"
#include "LootList.h"
#include "Remains.h"
#include "BarterWindow.h"
#include "Journal.h"
#include "FrameAnimation.h"
#include "ItemDatabase.h"
#include "Terrain.h"
#include "Projectile.h"
#include "LiquidParticle.h"
#include "Inventory.h"
#include "ParticleSystem.h"
#include "Light.h"
#include "PolyLine.h"
#include "Door.h"
#include "Harpoon.h"
#include "TextureStack.h"
#include "RevealGun.h"
#include "AcidGun.h"
#include "RailGun.h"
#include "GravityGun.h"
#include "Shield.h"
#include "Bench.h"
#include "WaterBlob.h"
#include "Animation.h"
#include "Pickaxe.h"
#include "AnimationTrack.h"
#include "Dwarf.h"
#include "NPC.h"
#include "BossGolem.h"
#include "AStar.h"
#include "Image.h"
#include "Console.h"
#include "InteractionMenu.h"
#include "Stats.h"
#include "Workbench.h"
#include "Smoke.h"
#include "PerlinNoise.h"
#include "Torch.h"
#include "ConstructionGun.h"
#include "MapWindow.h"
#include "BookReader.h"
#include "LightningGun.h"
#include "Room.h"
#include "FogGate.h"
#include "Encyclopedia.h"
#include "NameDatabase.h"
#include "LocationMap.h"
#include "ResourceDatabase.h"
#include "Asset.h"
#include "BossCrystal.h"
#include "WorldGenerator.h"
#include "TonnelVertex.h"
#include "Passage.h"
#include "Island.h"
#include "Cloud.h"
#include "FogOfWar.h"
#include "AttributeGrid.h"
#include "Arena.h"
#include "BossBeast.h"
#include "Guid.h"
#include "ForceGun.h"

Game *game = nullptr;

void Game::SaveSettings()
{
	Config cfg("default.cfg");
	auto rConfig = mRenderer->GetConfig();
	cfg.SetNumber("resolutionWidth", static_cast<float>(rConfig.mWidth));
	cfg.SetNumber("resolutionHeight", static_cast<float>(rConfig.mHeight));
	cfg.SetBoolean("fullscreen", rConfig.mFullscreen);
	cfg.Save();
}

void Game::UpdateSounds()
{
	for (auto it = mSoundList.begin(); it != mSoundList.end();)
	{
		auto &snd = *it;
		if (snd->IsStopped())
		{
			it = mSoundList.erase(it);
		}
		else
		{
			snd->Update();

			++it;
		}
	}
}

float Game::GetTimeSeconds()
{
	return std::chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - mStartTime).count() * 1e-9f;
}

Game::Game()
	: mActive(true)
	, mStartTime(std::chrono::high_resolution_clock::now())
{
	game = this;

	Config cfg("default.cfg");
	int processorCount = max(1u, std::thread::hardware_concurrency() - 1);
	int threadCount = cfg.Get<int>("maxPhysicsThreadCount");
	if (threadCount > 0)
	{
		Log() << "Physics will try to use " << threadCount << " threads to do calculations" << LineEnd;
	}
	else
	{
		Log() << "Physics will use all physical cores (" << processorCount << ") of your processor to do calculations" << LineEnd;
	}
	string locPath = cfg.Get<string>("localizationPath");
	mLocalization.Load(locPath + "db.loca");
	mControls.LoadDefaults();
	Debug.useMultithreadedWorldGenerator = cfg.Get<bool>("useMultithreadedWorldGenerator");

	const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	RendererConfig rConfig;
	rConfig.mWidth = cfg.Get<int>("resolutionWidth");
	if (rConfig.mWidth == 0 || rConfig.mWidth > static_cast<int>(desktopMode.width))
	{
		rConfig.mWidth = desktopMode.width;
	}
	rConfig.mHeight = cfg.Get<int>("resolutionHeight");
	if (rConfig.mHeight == 0 || rConfig.mHeight > static_cast<int>(desktopMode.height))
	{
		rConfig.mHeight = desktopMode.height;
	}
	rConfig.mFullscreen = cfg.Get<bool>("fullscreen");
	rConfig.mVirtualScreenSize = cfg.Get<float>("virtualScreenSize");
	rConfig.mFrameRateLimit = cfg.Get<int>("frameRateLimit");
	rConfig.mFrameProfilerEnabled = cfg.Get<bool>("frameProfilerEnabled");
	rConfig.mBorderlessWindow = cfg.Get<bool>("borderlessWindow");
	mRenderer = make_unique<Renderer>(rConfig);

	mResourceDb = make_unique<ResourceDatabase>();
	mInput = make_unique<Input>();
	mPhysics = make_unique<Physics>(threadCount > 0 ? threadCount : processorCount);
	mEditor = make_unique<Editor>();
	mItemDatabase = make_unique<ItemDatabase>();
	mMenu = make_unique<Menu>();
	mHUD = make_unique<HUD>();
	mStatWindow = make_unique<StatWindow>();
	mInventoryGUI = make_unique<InventoryGUI>();
	mDialogWindow = make_unique<DialogGUI>();
	mLootWindow = make_unique<LootWindow>();
	mBarterWindow = make_unique<BarterWindow>();
	mJournal = make_unique<Journal>();
	mBench = make_unique<Bench>();
	mConsole = make_unique<Console>();
	mInteractionMenu = make_unique<InteractionMenu>();
	mMapWindow = make_unique<MapWindow>();
	mNameDatabase = make_unique<NameDatabase>(locPath);
	mBookReader = make_unique<BookReader>();	
	mEncyclopedia = make_unique<Encyclopedia>();

	mOverlay = CreateCanvas();
	mOverlay->SetLayer(Scene::TopmostLayer);

	mFPSText = mOverlay->Create<Label>();
	mFPSText->SetCharacterSize(12);
	mFPSText->SetText(" ");
	mFPSText->SetAlignment(TextAlignment::LeftTop);
	mFPSText->SetSize({ 240, 0 });
	mFPSText->SetAutoHeight(true);

	mEarlyBuild = mOverlay->Create<Label>();
	mEarlyBuild->SetCharacterSize(14);
	mEarlyBuild->SetAutoHeight(true);
	mEarlyBuild->SetText("Development Build v0.71");
	mEarlyBuild->SetColor({ 0, 255, 0, 180 });
	mEarlyBuild->SetAlignment(TextAlignment::RightTop);
	mEarlyBuild->SetSize({ 250, 20 });
	mEarlyBuild->SetPosition({ VirtualScreen.getSize().x - mEarlyBuild->GetSize().x,
		VirtualScreen.getSize().y - mEarlyBuild->GetSize().y });

	mMusic.openFromFile("data/music/misery.ogg");
	mMusic.play();
	mMusic.setLoop(true);
	mMusic.setVolume(10);
}

Game::~Game()
{
	SaveSettings();
	mMenu.reset();
	mEditor.reset();
	mLevel.reset();
	mSceneList.clear();
	mCanvasList.clear();
	mPhysics.reset();
	mInput.reset();
	mRenderer.reset();
	DBG_ECHO();
}

void Game::MainLoop()
{
	constexpr const double fixedTimeStep = 1.0f / FIXED_FPS;
	double gameClock = GetTime() - 2 * fixedTimeStep;
	double fpsTime = GetTime();
	double infoTime = GetTime();
	int fps = 0;
	int currentFPS = 0;

	while (mRenderer->IsActive() && mActive)
	{
		double logicTime = 0.0f;
		double dt = GetTime() - gameClock;

		while (dt >= fixedTimeStep)
		{
			// if inner loop takes too much time, skip it
			if (dt > 4 * fixedTimeStep)
			{
				gameClock = GetTime();
				break;
			}

			dt -= fixedTimeStep;
			gameClock += fixedTimeStep;

			// flush input before receiving new messages from OS
			// needed for correct work of IsXXXHit methods
			mInput->Flush();
			mRenderer->ProcessEvents();

			CheckWorldState();

			mRenderer->BeginImGuiRender();

			bool anyWindowOpen = mEditor->IsActive() || mMenu->IsVisible() ||
				mInventoryGUI->IsVisible() || mStatWindow->IsVisible() ||
				mDialogWindow->IsVisible() || mLootWindow->IsVisible() ||
				mBarterWindow->IsVisible() || mJournal->IsVisible() || mEncyclopedia->IsVisible() ||
				mHUD->IsInModalMode() || mMapWindow->IsVisible() || mBench->IsVisible();

			if (!mMenu->IsVisible())
			{
				// handle [Esc] key actions
				if (mInput->IsKeyHit(sf::Keyboard::Escape))
				{
					if (mStatWindow->IsVisible())
					{
						mStatWindow->SetVisible(false);
					}
					else if (mInventoryGUI->IsVisible())
					{
						mInventoryGUI->SetVisible(false);
					}
					else if (mDialogWindow->IsVisible())
					{
						mDialogWindow->SetVisible(false);
					}
					else if (mLootWindow->IsVisible())
					{
						mLootWindow->SetVisible(false);
					}
					else if (mMapWindow->IsVisible())
					{
						mMapWindow->SetVisible(false);
					}
					else if (mBarterWindow->IsVisible())
					{
						mBarterWindow->SetVisible(false);
					}
					else if (mJournal->IsVisible())
					{
						mJournal->SetVisible(false);
					}
					else if (mBench->IsVisible())
					{
						mBench->SetVisible(false);
					}
					else if (mBookReader->IsVisible())
					{
						mBookReader->SetVisible(false);
					}
					else if (mEncyclopedia->IsVisible())
					{
						mEncyclopedia->SetVisible(false);
					}
					else if (mEditor->IsActive())
					{
						mEditor->SetActive(false);
						mMenu->SetVisible(true);
					}
					else
					{
						mMenu->SetVisible(true);
						mEditor->SetActive(false);
					}
				}
				if (!mEditor->IsActive())
				{
					if (!mInventoryGUI->IsVisible() && !mDialogWindow->IsVisible() && !mLootWindow->IsVisible() &&
						!mBarterWindow->IsVisible() && !mJournal->IsVisible() && !mBench->IsVisible() &&
						!mMapWindow->IsVisible() && !mBookReader->IsVisible())
					{
						if (mControls.IsKeyHit(ControlKey::OpenStats))
						{
							mStatWindow->SwitchVisible();
						}
					}
					if (!mStatWindow->IsVisible() && !mDialogWindow->IsVisible() && !mLootWindow->IsVisible() &&
						!mBarterWindow->IsVisible() && !mJournal->IsVisible() && !mBench->IsVisible() &&
						!mMapWindow->IsVisible() && !mBookReader->IsVisible())
					{
						if (mControls.IsKeyHit(ControlKey::OpenInventory))
						{
							mInventoryGUI->SwitchVisible();
						}
					}
					if (!mStatWindow->IsVisible() && !mDialogWindow->IsVisible() && !mLootWindow->IsVisible() &&
						!mBarterWindow->IsVisible() && !mInventoryGUI->IsVisible() && !mBench->IsVisible() &&
						!mMapWindow->IsVisible() && !mBookReader->IsVisible())
					{
						if (mControls.IsKeyHit(ControlKey::OpenJournal))
						{
							mJournal->SwitchVisible();
						}
					}
					if (!mStatWindow->IsVisible() && !mDialogWindow->IsVisible() && !mLootWindow->IsVisible() &&
						!mBarterWindow->IsVisible() && !mJournal->IsVisible() && !mBench->IsVisible())
					{
						if (mControls.IsKeyHit(ControlKey::OpenMap) && !mBookReader->IsVisible())
						{
							mMapWindow->SwitchVisible();
						}
					}
				}
			}
			else
			{
				if (mInput->IsKeyHit(sf::Keyboard::Escape))
				{
					if (mLevel)
					{
						mMenu->SetVisible(false);
					}
				}
			}

			mHUD->SetVisible(!mMenu->IsVisible() && !mEditor->IsActive());

			if (!mMenu->IsVisible())
			{
				// update each scene
				for (const shared_ptr<Scene> & scene : mSceneList)
				{
					scene->Update();
				}
			}

			// update each canvas
			for (const shared_ptr<Canvas> & canvas : mCanvasList)
			{
				canvas->Update();
			}

			mMenu->Update();
			mDialogWindow->Update();
			mHUD->Update();
			mStatWindow->Update();
			mInventoryGUI->Update();
			mLootWindow->Update();
			mBarterWindow->Update();
			mJournal->Update();
			mBench->Update();
			mConsole->DrawUpdate();
			UpdateSounds();

			if (!anyWindowOpen && mLevel && mWorldState == WorldState::Generated)
			{
				// level update
				auto lastTime = chrono::high_resolution_clock::now();
				mLevel->Update();
				mInteractionMenu->Update();
				logicTime += chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - lastTime).count();
				// physics update
				mPhysics->Step((float)fixedTimeStep);
			}

			if (!mMenu->IsVisible())
			{
				// update each scene
				for (const shared_ptr<Scene> & scene : mSceneList)
				{
					scene->PostUpdate();
				}
			} 

			mEditor->DrawAndUpdate();
			mRenderer->EndImGuiRender();
		}

		mFPSText->SetLocalVisibility(!mEditor->IsActive());
		mRenderer->Render();

		// fps measurement
		++fps;
		if (GetTime() - fpsTime >= 1.0f)
		{
			currentFPS = fps;
			fps = 0;
			fpsTime = GetTime();
		}

		// print info each 500 ms
		if (GetTime() - infoTime >= 0.5f)
		{
			mFPSText->SetText(StringBuilder()
				<< "FPS: " << currentFPS << LineEnd
				<< "Frame: " << RoundAndConvert(mRenderer->mFrameTime / 1000.0f, 2) << " ms"
				<< LineEnd << "Logic: " << RoundAndConvert((float)logicTime / 1000.0f, 2) << " ms"
				<< LineEnd << "Particle-Terrain Collision: "
				<< RoundAndConvert(mPhysics->GetParticleBitmapSolveTime() / 1000.0f, 2)
				<< " ms" << LineEnd << "Particle-Particle Collision: "
				<< RoundAndConvert(mPhysics->GetParticleParticleSolveTime() / 1000.0f, 2)
				<< " ms" << LineEnd << "Physics Pre-pass: "
				<< RoundAndConvert(mPhysics->GetPrePassTime() / 1000.0f, 2) << " ms"
				<< LineEnd << "Total Physics Time: "
				<< RoundAndConvert(mPhysics->GetTotalSolveTime() / 1000.0f, 2) << " ms"
				<< LineEnd << "Physics Islands: " << mPhysics->GetIslandCount() << LineEnd
				<< "Total Physics Particles: " << mPhysics->GetParticleCount() << LineEnd
				<< "Draw Calls: " << mRenderer->GetDrawCallCount() << LineEnd
				<< "Up time: " << GetTimeSeconds() << " s\n");

			mPhysics->ResetPerformanceCounters();
			infoTime = GetTime();

			Log() << "FPS: " << currentFPS << LineEnd;
		}
	}
}

double Game::GetTime()
{
	const auto current = chrono::high_resolution_clock::now().time_since_epoch();
	return chrono::duration_cast<chrono::microseconds>(current).count() / 1000000.0;
}

void Game::SortScenes()
{
	lock_guard<recursive_mutex>(mRenderer->GetMutex());
	sort(mSceneList.begin(), mSceneList.end(), [] (const auto &left, const auto &right)
	{
		return left->GetLayer() < right->GetLayer();
	});
}

void Game::SortCanvases()
{
	sort(mCanvasList.begin(), mCanvasList.end(), [] (const auto &left, const auto &right)
	{
		return left->GetLayer() < right->GetLayer();
	});
}

shared_ptr<Canvas> Game::CreateCanvas()
{
	auto scene = make_shared<Canvas>();
	mCanvasList.push_back(scene);
	SortCanvases();
	return scene;
}

shared_ptr<Scene> Game::CreateScene()
{
	lock_guard<recursive_mutex>(mRenderer->GetMutex());
	auto scene = make_shared<Scene>();
	scene->CreateIsland();
	mSceneList.push_back(scene);
	SortScenes();
	return scene;
}

const vector<shared_ptr<Scene>>& Game::GetSceneList() const
{
	return mSceneList;
}

shared_ptr<Level> &Game::GetLevel()
{
	return mLevel;
}

Physics *Game::GetPhysics()
{
	return mPhysics.get();
}

Renderer *Game::GetRenderer()
{
	return mRenderer.get();
}

Input * Game::GetInput()
{
	return mInput.get();
}

Editor * Game::GetEditor()
{
	return mEditor.get();
}

Menu * Game::GetMenu()
{
	return mMenu.get();
}

InventoryGUI* Game::GetInventoryGUI() const
{
	return mInventoryGUI.get();
}

DialogGUI* Game::GetDialogWindow() const
{
	return mDialogWindow.get();
}

StatWindow* Game::GetStatWindow() const
{
	return mStatWindow.get();
}

Encyclopedia * Game::GetEncyclopedia() const
{
	return mEncyclopedia.get();
}

InteractionMenu * Game::GetInteractionMenu() const
{
	return mInteractionMenu.get();
}

unordered_map<string, string>& Game::GetCurrentLocalization()
{
	return mLocalization.GetMap();
}

void Game::SaveLocalization()
{
	mLocalization.Save();
}

Config & Game::GetCurrentLocalizationConfig()
{
	return mLocalization;
}

HUD * Game::GetHUD()
{
	return mHUD.get();
}

string Game::GetLanguagePath() const
{
	return mLanguagePath;
}

void Game::Shutdown()
{
	mActive = false;
}

void Game::Save(const string &path)
{
	Serializer save(path, true);
	save &mSceneList;
	mPhysics->Serialize(save);
	save &mLevel;
}

void Game::Load(const string &path)
{
	mLevel.reset();
	mSceneList.clear();

	Serializer save(path, false);

	save &mSceneList;
	mPhysics->Serialize(save);
	save &mLevel;

	mWorldState = WorldState::Generated;
}

shared_ptr<sf::SoundBuffer> Game::GetSoundBuffer(const string &filename)
{
	auto existing = mSoundBufferList.find(filename);
	if (existing == mSoundBufferList.end())
	{
		auto buffer = make_shared<sf::SoundBuffer>();
		if (buffer->loadFromFile(filename))
		{
			Log() << "Sound buffer " << filename << " was successfully loaded and cached!" << LineEnd;
		}
		else
		{
			throw Exception(StringBuilder("Unable to load ") << filename << " sound!");
		}
		mSoundBufferList[filename] = buffer;
		return buffer;
	}
	else
	{
		return existing->second;
	}
}

string Game::GetSoundBufferFileName(const shared_ptr<sf::SoundBuffer> &buf)
{
	for (auto &pair : mSoundBufferList)
	{
		if (pair.second == buf)
		{
			return pair.first;
		}
	}
	return "";
}

void Game::RegisterSound(const string &name, const shared_ptr<sf::SoundBuffer> &buf)
{
	mSoundBufferList[name] = buf;
}

Controls &Game::GetControls()
{
	return mControls;
}

vector<shared_ptr<Canvas>> &Game::GetCanvasList()
{
	return mCanvasList;
}

void Game::RemoveScene(const shared_ptr<Scene> &scene)
{
	lock_guard<recursive_mutex>(mRenderer->GetMutex());
	mSceneList.erase(std::remove(mSceneList.begin(), mSceneList.end(), scene), mSceneList.end());
}

shared_ptr<Sound> Game::EmitSound2D(const string &buf)
{
	auto snd = make_shared<Sound>(buf);
	mSoundList.push_back(snd);
	snd->Set2D(true);
	return snd;
}

Canvas * Game::GetOverlay()
{
	return mOverlay.get();
}

const string& Game::GetStr(const string &name) const
{
	return mLocalization.Get<const string&>(name);
}

sf::Music &Game::GetMusic()
{
	return mMusic;
}

ItemDatabase * Game::GetItemDatabase()
{
	return mItemDatabase.get();
}

void Game::AddScene(const shared_ptr<Scene> &prefab)
{
	lock_guard<recursive_mutex>(mRenderer->GetMutex());
	mSceneList.push_back(prefab);
	SortScenes();
}

shared_ptr<Scene> Game::LoadPrefab(const string &path)
{
	lock_guard<recursive_mutex> guard(mRenderer->GetMutex());

	shared_ptr<Scene> prefab;
	Serializer sav { path, false };
	sav &prefab;

	AddScene(prefab);

	// after editor, prefab can have wrong options for the game
	// fix it
	for (auto &sprite : prefab->GetSpriteList())
	{
		sprite->ResetRenderFlags(RF_NO_LIGHTING);
	}
	for (auto &animation : prefab->GetAnimationList())
	{
		animation->SetEnabled(false);
		animation->SetWeight(0);
	}

	return prefab;
}

void Game::SavePrefab(const string &path, shared_ptr<Scene> &prefab)
{
	// prefab is a scene by default
	// so we can easily serialize it by standard serializer
	Serializer sav { path, true };
	sav &prefab;

	Asset asset;
	asset.AddScene(prefab);
	asset.Save(path + ".json");
}

Bench * Game::GetBench()  const
{
	return mBench.get();
}

Journal * Game::GetJournal() const
{
	return mJournal.get();
}

LootWindow * Game::GetLootWindow() const
{
	return mLootWindow.get();
}

BarterWindow* Game::GetBarterWindow() const
{
	return mBarterWindow.get();
}

MapWindow *Game::GetMapWindow() const
{
	return mMapWindow.get();
}

NameDatabase *Game::GetNameDatabase() const
{
	return mNameDatabase.get();
}

BookReader *Game::GetBookReader() const
{
	return mBookReader.get();
}

ResourceDatabase * Game::GetResourceDatabase() const
{
	return mResourceDb.get();
}

int Game::GetTotalCanvasCount() const
{
	return mCanvasList.size();
}

int Game::GetTotalWidgetCount() const
{
	int count = 0;
	for (auto &canvas : mCanvasList)
	{
		count += canvas->GetWidgetList().size();
	}
	return count;
}

int Game::GetVisibleCanvasCount() const
{
	int count = 0;
	for (auto &canvas : mCanvasList)
	{
		if (canvas->IsVisible())
		{
			++count;
		}
	}
	return count;
}

int Game::GetVisibleWidgetCount() const
{
	int count = 0;
	for (auto &canvas : mCanvasList)
	{
		if (canvas->IsVisible())
		{
			for (auto &widget : canvas->GetWidgetList())
			{
				if (widget->IsGloballyVisible())
				{
					++count;
				}
			}
		}
	}
	return count;
}

void Game::RaiseDialog(const shared_ptr<Dialog>& dialog)
{
	mDialogWindow->SetDialog(dialog);
}

void Game::CheckWorldState()
{
	if (mWorldState == WorldState::NeedsToBeDestroyed)
	{
		mPhysics->Clear();
		mLevel.reset();

		mWorldState = WorldState::Destroyed;
	}

	if (mWorldState == WorldState::NeedsToBeGenerated)
	{
		// Destroy first
		mPhysics->Clear();
		mLevel.reset();

		// Start new thread to generate level
		std::thread([this]
		{
			try
			{
				mLevel = mWorldGenerator->Generate();

				// Raise atomic flag that world is generated
				mWorldGenerationIsDone = true;
			}
			catch (std::exception &e)
			{
				ReportError(e.what());
			}
		}).detach();

		mMenu->ShowProgressWindow(true);

		mWorldState = WorldState::Generating;
	}

	if (mWorldState == WorldState::Generating)
	{
		// Make sure that generation thread done its job
		if (mWorldGenerationIsDone)
		{
			mWorldGenerator.reset();
            
            // transfer player to start location (player apartment)
            mLevel->TransferPlayerToLocation(mLevel->GetPlayer()->GetLocation(), mLevel->GetPlayer()->GetPosition());

			// Create autosave
			Save("data/save/autosave.dsv");
            
            // Show start screen and hide main menu
			RaiseDialog(make_shared<GameStoryDialog>(nullptr));

			mMenu->ShowProgressWindow(false);

			mMenu->SetVisible(false);

			mWorldState = WorldState::Generated;
		}
		else
		{
			mMenu->SetProgressMessage(mWorldGenerator->GetProgressMessage());
		}
	}
}

WorldState Game::GetWorldState() const
{
	return mWorldState;
}

void Game::ScheduleWorldDestruction()
{
	mWorldState = WorldState::NeedsToBeDestroyed;
}

void Game::ScheduleGenerateWorld(const LevelGeneratorParameters & parameters)
{
	if (mWorldState == WorldState::Destroyed || mWorldState == WorldState::Generated)
	{
		mWorldGenerator = make_unique<WorldGenerator>(parameters);
		mWorldState = WorldState::NeedsToBeGenerated;
		mWorldGenerationIsDone = false;
	}
	else
	{
		throw Exception("Trying to generate world while generation thread is active!");
	}
}

int main(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	// register types for serializer
	// WARNING: for backward compatibility add new types ONLY in back of the list
	Serializer::AddType<Scene>();
	Serializer::AddType<SceneNode>();
	Serializer::AddType<Sprite>();
	Serializer::AddType<Light>();
	Serializer::AddType<ParticleSystem>();
	Serializer::AddType<PolyLine>();
	Serializer::AddType<Text>();
	Serializer::AddType<Body>();
	Serializer::AddType<Chain>();
	Serializer::AddType<Link>();
	Serializer::AddType<Cell>();
	Serializer::AddType<Terrain>();
	Serializer::AddType<Debris>();
	Serializer::AddType<Player>();
	Serializer::AddType<Creature>();
	Serializer::AddType<Hook>();
	Serializer::AddType<Drill>();
	Serializer::AddType<PlasmaDrill>();
	Serializer::AddType<Shotgun>();
	Serializer::AddType<GrenadeLauncher>();
	Serializer::AddType<RocketLauncher>();
	Serializer::AddType<SoilGun>();
	Serializer::AddType<Projectile>();
	Serializer::AddType<Level>();
	Serializer::AddType<Actor>();
	Serializer::AddType<Texture>();
	Serializer::AddType<Item>();
	Serializer::AddType<Inventory>();
	Serializer::AddType<int>(); // placeholder for backward compat (can be replaced by any other type)
	Serializer::AddType<ItemGroup>();
	Serializer::AddType<LiquidParticle>();
	Serializer::AddType<sf::SoundBuffer>();
	Serializer::AddType<Sound>();
	Serializer::AddType<Chest>();
	Serializer::AddType<BackgroundLayer>();
	Serializer::AddType<Cloud>();
	Serializer::AddType<MotherCrystal>();
	Serializer::AddType<Ladder>();
	Serializer::AddType<FogGate>();
	Serializer::AddType<Remains>();
	Serializer::AddType<AnimationSequence>();
	Serializer::AddType<FrameListener>();
	Serializer::AddType<SimulationIsland>();
	Serializer::AddType<Door>();
	Serializer::AddType<Harpoon>();
	Serializer::AddType<Location>();
	Serializer::AddType<TextureStack>();
	Serializer::AddType<RevealGun>();
	Serializer::AddType<AcidGun>();
	Serializer::AddType<RailGun>();
	Serializer::AddType<GravityGun>();
	Serializer::AddType<WaterBlob>();
	Serializer::AddType<Shield>();
	Serializer::AddType<Room>();
	Serializer::AddType<Animation>();
	Serializer::AddType<AnimationTrack>();
	Serializer::AddType<Pickaxe>();
	Serializer::AddType<Dwarf>();
	Serializer::AddType<NPC>();
	Serializer::AddType<BossGolem>();
	Serializer::AddType<Workbench>();
	Serializer::AddType<Smoke>();
	Serializer::AddType<Torch>();
	Serializer::AddType<ConstructionGun>();
	Serializer::AddType<LightningGun>();
	Serializer::AddType<QuestStage>();
	Serializer::AddType<LocationMap>();
	Serializer::AddType<BossCrystal>();
	Serializer::AddType<FindItemQuest>();
	Serializer::AddType<KillQuest>();
	Serializer::AddType<Passage>();
	Serializer::AddType<Island>();
	Serializer::AddType<TonnelVertex>();
	Serializer::AddType<FogOfWar>();
	Serializer::AddType<AttributeGrid>();
	Serializer::AddType<Arena>();
	Serializer::AddType<BossBeast>();
	Serializer::AddType<LocationDefinition>();
	Serializer::AddType<ForceGun>();

	Serializer::AddType<DialogNode>();
	Serializer::AddType<Phrase>();
	Serializer::AddType<KickOutOfRoomDialog>();
	Serializer::AddType<IllegalZoneDialog>();
	Serializer::AddType<ApologizeDialog>();
	Serializer::AddType<MerchantTalkDialog>();
	Serializer::AddType<LocalTalkDialog>();
	Serializer::AddType<BadReputationDialog>();
	Serializer::AddType<FirstWarningDialog>();
	Serializer::AddType<FirstDeathDialog>();
	Serializer::AddType<ThiefTalkDialog>();
	Serializer::AddType<BlacksmithDialog>();
	Serializer::AddType<GameStoryDialog>();		
		

	// game code
	try
	{
		Game instance;
		instance.MainLoop();
	}
	catch (std::exception &e)
	{
		ReportError(e.what());

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

