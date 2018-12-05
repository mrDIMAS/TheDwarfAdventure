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
#include "Player.h"
#include "Level.h"
#include "Game.h"
#include "Renderer.h"
#include "Input.h"
#include "Debris.h"
#include "Weapon.h"
#include "LiquidParticle.h"
#include "Chest.h"
#include "HUD.h"
#include "StatWindow.h"
#include "InventoryGUI.h"
#include "Dialog.h"
#include "Ladder.h"
#include "MotherCrystal.h"
#include "Menu.h"
#include "Remains.h"
#include "LootList.h"
#include "Hook.h"
#include "Stats.h"
#include "Terrain.h"
#include "Light.h"
#include "Inventory.h"
#include "Sound.h"
#include "Door.h"
#include "Monster.h"
#include "Shield.h"
#include "Animation.h"
#include "AnimationTrack.h"
#include "IK.h"
#include "NPC.h"
#include "PolyLine.h"
#include "InteractionMenu.h"
#include "Workbench.h"
#include "Bench.h"
#include "BookReader.h"
#include "Room.h"
#include "FogGate.h"
#include "LocationMap.h"
#include "Projectile.h"
#include "AttributeGrid.h"
#include "Arena.h"

void Player::UpdateMovement()
{
}

void Player::UpdateFogOfWar()
{
	shared_ptr<Terrain> terrain = mLocation.lock()->GetScene()->GetTerrain();

	AttributeGridLayer& genericFlagsLayer = terrain->GetAttributeGrid()->GetGenericFlagsLayer();

	static auto lastTime = chrono::high_resolution_clock::now();

	if (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - lastTime).count() >= 60)
	{
		genericFlagsLayer.Clear();
		
		lastTime = chrono::high_resolution_clock::now();
		
		const Vec2f playerPos = GetPosition();

		constexpr float revealRadius = 1000.0f;
		constexpr float revealStep = 20;
		constexpr int angleStep = 4;

		static_assert(360 % angleStep == 0, "Angle step invalid");

		for (float angle = 0; angle < 360; angle += angleStep)
		{
			const Vec2f normDir = {
				cos(Math::Deg2Rad(angle)),
				sin(Math::Deg2Rad(angle))
			};

			const Vec2f traceDirection = {
				normDir.x * revealRadius,
				normDir.y * revealRadius
			};
			
			const TerrainRayTraceResult rt = terrain->TraceRay(playerPos, traceDirection);
						
			const float maxDistance = rt.mIntersection ?
				Math::SqrDistance(playerPos, rt.mPosition) :
				Math::Sqr(revealRadius);

			const Vec2f step = {
				normDir.x * revealStep,
				normDir.y * revealStep
			};

			Vec2f revealPos { playerPos };

			for (;;)
			{
				terrain->RevealFogOfWarAt(revealPos, 5 * 60);
								
				genericFlagsLayer.Set(terrain->AbsoluteToNormalized(revealPos), AttributeGridLayer::Flags::Visible);

				revealPos += step;

				if (Math::SqrDistance(playerPos, revealPos) > maxDistance)
				{
					break;
				}
			}

			if (game->Debug.showPlayerFogOfWarRevealRays)
			{
				shared_ptr<PolyLine> line = terrain->GetScene()->CreatePolyLine();
				line->AddPoint(playerPos);
				line->AddPoint(revealPos);
				line->SetThickness(1);
				line->SetColor({ 0, 255, 0 });
				line->SetLifeTime(24);
			}
		}
	}
}

void Player::UpdateView(const shared_ptr<Location> &location)
{
	
	auto scene = location->GetScene();
	auto terrain = scene->GetTerrain();
	auto &view = scene->GetView();
	view.setSize({ game->VirtualScreen.getSize().x / mViewScale, game->VirtualScreen.getSize().y / mViewScale });
	view.setCenter(GetPosition());
	sf::Listener::setPosition(GetPosition().x, GetPosition().y, 10);
	// clamp view to terrain
	if (location->GetClass() != LocationClass::FlyingIsland)
	{
		if (terrain->GetWidthPixels() > view.getSize().x)
		{
			if (view.getCenter().x - view.getSize().x / 2 < 0)
			{
				view.setCenter(view.getSize().x / 2, view.getCenter().y);
			}
			if (view.getCenter().x + view.getSize().x / 2 >= terrain->GetWidthPixels())
			{
				view.setCenter(terrain->GetWidthPixels() - view.getSize().x / 2, view.getCenter().y);
			}
		}
		else
		{
			view.setCenter(terrain->GetWidthPixels() * 0.5f, view.getCenter().y);
		}
		if (terrain->GetHeightPixels() > view.getSize().y)
		{
			if (view.getCenter().y + view.getSize().y / 2 >= terrain->GetHeightPixels())
			{
				view.setCenter(view.getCenter().x, terrain->GetHeightPixels() - view.getSize().y / 2);
			}
			if (location->GetClass() == LocationClass::Underground)
			{
				if (view.getCenter().y - view.getSize().y / 2 < 0)
				{
					view.setCenter(view.getCenter().x, view.getSize().y / 2);
				}
			}
		}
		else
		{
			view.setCenter(view.getCenter().x, terrain->GetHeightPixels() * 0.5f);
		}
	}

	const Controls& controls = game->GetControls();
	
	if (controls.IsKeyPressed(ControlKey::ZoomIn))
	{
		mViewScale = Math::Clamp(mViewScale + 0.025f, mZoomMin, mZoomMax);
	}
	else if(controls.IsKeyPressed(ControlKey::ZoomOut))
	{
		mViewScale = Math::Clamp(mViewScale - 0.025f, mZoomMin, mZoomMax);
	}
}

void Player::InteractWithItems()
{
	game->GetInteractionMenu()->HideOption(InteractionType::GrabItem);
	for (auto &item : GetLocation()->GetItemList())
	{
		float distance = Math::Distance(item->GetNode()->GetGlobalPosition(), GetPosition());
		if (distance < 50.0f)
		{
			// delegate action to interaction menu 
			auto grabItem = [this, weakItem = weak_ptr<Item>(item)]
			{
				if (auto item = weakItem.lock())
				{
					auto scene = GetLocation()->GetScene();
					scene->EmitSound("data/sounds/item_pickup.ogg", item->GetNode()->GetGlobalPosition());
					AddItem(item);
				}
			};
			game->GetInteractionMenu()->SetOption(InteractionType::GrabItem, grabItem);
			break;
		}
	}
}

void Player::InteractWithLadders(const shared_ptr<Body> &body, const shared_ptr<Location> &location)
{
	for (auto &ladder : location->GetLadderList())
	{
		if (ladder->Intersects(GetPosition(), body->GetRadius()))
		{
			mOnLadder = true;
			body->SetGravity({ 0, 0 });
			body->SetVelocity({ body->GetVelocity().x, 0 });
			mLastFeetPosition = GetFeetPosition();
		}
	}
}

void Player::InteractWithActors()
{
	game->GetInteractionMenu()->HideOption(InteractionType::Talk);
	game->GetInteractionMenu()->HideOption(InteractionType::Apologize);
	for (auto &actor : GetLocation()->GetActorList())
	{
		if (actor != shared_from_this())
		{
			auto npc = dynamic_pointer_cast<NPC>(actor);
			if (npc && !npc->IsDead())
			{

				if (Math::Distance(npc->GetPosition(), GetPosition()) < 80)
				{
					if (npc->IsAggressiveToPlayer())
					{
						// delegate action to interaction menu
						auto apologize = [this, weakNPC = weak_ptr<NPC>(npc)]
						{
							if (const shared_ptr<NPC> npc = weakNPC.lock())
							{
								npc->BeginDialogOfType<ApologizeDialog>();								
							}
						};
						game->GetInteractionMenu()->SetOption(InteractionType::Apologize, apologize);
					}
					else
					{
						// delegate action to interaction menu
						auto talk = [this, weakNPC = weak_ptr<NPC>(npc)]
						{
							if (const shared_ptr<NPC> npc = weakNPC.lock())
							{
								npc->BeginDialog();
							}
						};
						game->GetInteractionMenu()->SetOption(InteractionType::Talk, talk);
					}
					break;
				}
			}
		}
	}
}

void Player::InteractWithFogGates()
{
	game->GetInteractionMenu()->HideOption(InteractionType::UseFogGate);

	const shared_ptr<Location> location = GetLocation();
		
	for (const shared_ptr<Arena>& arena : location->GetArenaList())
	{
		if (arena->Contains(GetPosition()))
		{
			// Cannot interact with foggates while fighting with boss
			return;
		}
	}

	for (const shared_ptr<FogGate> &fogGate : location->GetFogGateList())
	{
		const sf::FloatRect bloatedRect = { 
			fogGate->mRect.left - 60, 
			fogGate->mRect.top - 60, 
			fogGate->mRect.width + 120, 
			fogGate->mRect.height + 120
		};

		if (bloatedRect.contains(GetPosition()))
		{
			if (fogGate->mType == FogType::BossBlock)
			{
				game->GetHUD()->SetNotification(game->GetStr("passageBlock"));
			}
			else
			{
				// delegate action to interaction menu
				auto enter = [this, weakFG = weak_ptr<FogGate>(fogGate)]
				{
					if (const shared_ptr<FogGate> fogGate = weakFG.lock())
					{
						SetPosition({ 
							fogGate->mRect.left + fogGate->mRect.width, 
							fogGate->mRect.top + fogGate->mRect.height / 2 
						});
					}
				};
				game->GetInteractionMenu()->SetOption(InteractionType::UseFogGate, enter);
				break;
			}
		}
	}
}

void Player::InteractWithRemains()
{
	game->GetInteractionMenu()->HideOption(InteractionType::PillageRemains);
	for (auto &remains : GetLocation()->GetRemainsList())
	{
		if (Math::Distance(remains->GetPosition(), GetPosition()) < 60)
		{
			// delegate action to interaction menu
			auto pillage = [this, weakRemains = weak_ptr<Remains>(remains)]
			{
				if (auto remains = weakRemains.lock())
				{
					game->GetLootWindow()->Open(remains->GetInventory());
				}
			};
			game->GetInteractionMenu()->SetOption(InteractionType::PillageRemains, pillage);
			break;
		}
	}
}

void Player::InteractWithChests()
{
	game->GetInteractionMenu()->HideOption(InteractionType::UseChest);
	for (auto &chest : GetLocation()->GetChestList())
	{
		auto position = chest->GetModel()->GetGlobalPosition();
		if (Math::Distance(position, GetPosition()) < 60)
		{
			// delegate action to interaction menu
			auto useChest = [this, weakChest = weak_ptr<Chest>(chest)]
			{
				if (auto chest = weakChest.lock())
				{
					game->GetLootWindow()->Open(chest->GetInventory());
				}
			};
			game->GetInteractionMenu()->SetOption(InteractionType::UseChest, useChest);
			break;
		}
	}
}

void Player::InteractWithMotherCrystals()
{
	game->GetInteractionMenu()->HideOption(InteractionType::UseMotherCrystal);
	for (auto &mc : GetLocation()->GetMotherCrystalList())
	{
		if (Math::Distance(mc->GetPosition(), GetPosition()) < 100)
		{

			// delegate action to interaction menu
			auto useMC = [this, weakMC = weak_ptr<MotherCrystal>(mc)]
			{
				if (auto mc = weakMC.lock())
				{
					if (mc->IsActive())
					{
						game->GetMenu()->SetVisible(true);
						game->GetMenu()->SetPage(MenuPage::SaveGame);
					}
					else
					{
						mc->Activate();
						mLastVisitedMotherCrystal = mc;
					}
				}
			};
			game->GetInteractionMenu()->SetOption(InteractionType::UseMotherCrystal, useMC);
			break;
		}
	}
}

void Player::InteractWithDoors()
{
	game->GetInteractionMenu()->HideOption(InteractionType::UseDoor);
	for (auto &door : GetLocation()->GetDoorList())
	{
		if (door->IsIntersect(GetPosition(), GetBody()->GetRadius()))
		{
			if (door->GetState() == DoorState::Locked)
			{
				game->GetHUD()->SetNotification(game->GetStr("doorLocked"));
			}

			// delegate action to interaction menu
			auto useDoor = [this, weakDoor = weak_ptr<Door>(door)]
			{
				if (auto door = weakDoor.lock())
				{
					auto location = GetLocation();
					if (door->GetType() == DoorType::ToOtherScene)
					{
						if (door->GetDestinationDoor())
						{
							auto level = location->GetLevel();
							for (auto handEqupment : GetHandSlots())
							{
								if (auto hook = dynamic_pointer_cast<Hook>(handEqupment))
								{
									hook->ResetHook();
								}
							}
							level->TransferPlayerToLocation(door->GetDestinationDoor()->GetLocation(), door->GetDestinationDoor()->GetPosition());
						}
					}
					else
					{
						if (door->GetState() == DoorState::Closed)
						{
							OpenDoor(door);
						}
						else if (door->GetState() == DoorState::Open)
						{
							CloseDoor(door); // todo: add lock parameter usage
						}
					}
				}
			};

			game->GetInteractionMenu()->SetOption(InteractionType::UseDoor, useDoor);
		}
	}
}

void Player::InteractWithRooms()
{
	for (auto &room : GetLocation()->GetRoomList())
	{
		if (room->IsEntranceContainsPoint(GetPosition()))
		{
			if (room->IsLocked())
			{
				const shared_ptr<Actor> inhabitant = room->GetMostPowerfulInhabitant();

				if (inhabitant)
				{
					// delegate action to interaction menu
					auto talk = [this, weakActor = weak_ptr<Actor>(inhabitant)]
					{
						if (auto actor = weakActor.lock())
						{
							if (auto npc = dynamic_pointer_cast<NPC>(actor))
							{
								npc->BeginDialog();
							}
						}
					};
					game->GetInteractionMenu()->SetOption(InteractionType::Talk, talk);
					break;
				}
			}
		}
	}
}

void Player::InteractWithBenches()
{
	game->GetInteractionMenu()->HideOption(InteractionType::UseBench);
	for (auto &bench : GetLocation()->GetWorkbenchList())
	{
		if (bench->GetBounds().contains(GetPosition()))
		{
			// delegate action to interaction menu
			auto useBench = [this, weakBench = weak_ptr<Workbench>(bench)] {
				if (auto bench = weakBench.lock())
				{
					game->GetBench()->SetVisible(true);
				}
			};
			game->GetInteractionMenu()->SetOption(InteractionType::UseBench, useBench);
			break;
		}
	}
}

void Player::UpdateHandEquipment(const Vec2f &aimDirection, const Controls &controls, Input *input)
{
	static vector<int> handIndices;
	handIndices.clear();

	if (input->IsMouseButtonPressed(sf::Mouse::Left))
	{
		handIndices.push_back(0);
	}
	if (input->IsMouseButtonPressed(sf::Mouse::Right))
	{
		handIndices.push_back(1);
	}

	HUD* hud = game->GetHUD();

	for (auto handIndex : handIndices)
	{
		if (auto wpn = dynamic_pointer_cast<Weapon>(GetHandEquipment(static_cast<Hand>(handIndex))))
		{
			if (!hud->IsAnythingHovered())
			{
				if (wpn->IsBroken())
				{
					hud->SetNotification(game->GetStr("weaponBroken"));
				}
				else
				{
					if (wpn->GetDefinition().mIsMeleeWeapon)
					{
						if (mMeleeAttackState[handIndex] == MeleeAttackState::Ready && mStats->ConsumeStamina(10))
						{
							mMeleeAttackState[handIndex] = MeleeAttackState::Swing;
						}
					}
					else
					{
						if (!wpn->Shoot(aimDirection))
						{
							hud->SetNotification(game->GetStr("outOfWpnConsumables"));
						}
					}
				}
			}
			if (!mOnLadder)
			{
				if (auto hook = dynamic_pointer_cast<Hook>(wpn))
				{
					if (controls.IsKeyPressed(ControlKey::ClimbUp))
					{
						hook->ChangeChainLength(-0.01f);
					}
					if (controls.IsKeyPressed(ControlKey::ClimbDown))
					{
						hook->ChangeChainLength(0.01f);
					}
				}
			}
		}
	}
}

void Player::UpdateCurrentShield(Input *input, bool flip)
{
	int handIndex = 0;
	for (auto & handEquipment : GetHandSlots())
	{
		if (handEquipment)
		{
			if (auto shield = dynamic_pointer_cast<Shield>(handEquipment))
			{
				auto key = handIndex == 0 ? sf::Mouse::Left : sf::Mouse::Right;

				shield->Raise(input->IsMouseButtonPressed(key) && !IsInStunLock());
				shield->GetNode()->SetLocalScale({ flip ? -1.0f : 1.0f, 1.0f });
				shield->GetNode()->SetLocalPosition({ flip ? -15.0f : 15.0f, 4.0f });
			}
		}
		++handIndex;
	}
}

void Player::OnDie()
{
	const auto location = GetLocation();
	const auto scene = location->GetScene();
	const auto rareRed = mInventory->FindGroup(ItemType::Crystal_RareRed);
	game->GetHUD()->SetBossFightModeEnabled(false);
	if (rareRed)
	{
		// if player got some rare red crystals, then he stays alive but one crystal
		// will be destroyed
		rareRed->RemoveCount(1);
		++mDeathCount;
	}
	else
	{
		game->GetMenu()->SetVisible(true);
		game->GetMenu()->SetPage(MenuPage::EndGame);

		game->ScheduleWorldDestruction();
	}
	for (auto & handEquipment : GetHandSlots())
	{
		if (auto hook = dynamic_pointer_cast<Hook>(handEquipment))
		{
			hook->ResetHook();
		}
	}
}
/*
// player have no rare red crystals, apply some penalties
if (game->GetDifficulty() == Difficulty::Easy) {
// death penalty is 10% of total money
mInventory->RemoveMoney(static_cast<int>(0.1f * mInventory->GetMoney()));
// no items lost
} else if (game->GetDifficulty() == Difficulty::Medium) {
// death penalty is 20% of total money
mInventory->RemoveMoney(static_cast<int>(0.2f * mInventory->GetMoney()));
// some items is lost
// move part of equipment to remains
const auto stuff = make_shared<Inventory>(shared_from_this());
// copy group ptrs
const auto playerItems = mInventory->GetGroups();
for (const auto &group : playerItems) {
auto iclass = group->GetAny()->GetDesc().mClass;
if (iclass == ItemClass::Blueprint || iclass == ItemClass::Crystal || iclass == ItemClass::Ingredient ||
iclass == ItemClass::Metal || iclass == ItemClass::Necklace || iclass == ItemClass::Ore || iclass == ItemClass::Ring) {
for (const auto &item : group->mItems) {
UnequipItem(item);
}
mInventory->MoveGroup(group, stuff);
}
}
location->AddRemains(make_shared<Remains>(scene, GetPosition(), stuff));
} else if (game->GetDifficulty() == Difficulty::Hard) {
// death penalty is 30% of total money
mInventory->RemoveMoney(static_cast<int>(0.3f * mInventory->GetMoney()));
// move part of equipment to remains
const auto stuff = make_shared<Inventory>(shared_from_this());
// copy group ptrs
const auto playerItems = mInventory->GetGroups();
for (const auto &group : playerItems) {
auto iclass = group->GetAny()->GetDesc().mClass;
if (iclass == ItemClass::Blueprint || iclass == ItemClass::Crystal || iclass == ItemClass::Ingredient ||
iclass == ItemClass::Metal || iclass == ItemClass::Necklace || iclass == ItemClass::Ore || iclass == ItemClass::Potion ||
iclass == ItemClass::Ring || iclass == ItemClass::Shield) {
for (const auto &item : group->mItems) {
UnequipItem(item);
}
mInventory->MoveGroup(group, stuff);
}
}
location->AddRemains(make_shared<Remains>(scene, GetPosition(), stuff));
} else if (game->GetDifficulty() == Difficulty::Realistic) {
*/

void Player::ChangeReputation(float repChange)
{
	Actor::ChangeReputation(repChange);
	if (repChange < 0)
	{
		game->GetHUD()->AddNotification(
			StringBuilder() << game->GetStr("reputationDecreased") << abs(repChange));
	}
	else
	{
		game->GetHUD()->AddNotification(
			StringBuilder() << game->GetStr("reputationIncreased") << abs(repChange));
	}
}

void Player::AddItem(const shared_ptr<Item> &item)
{
	Actor::AddItem(item);

	game->GetHUD()->AddNotification(StringBuilder() << game->GetStr("received") << item->GetDefinition().GetTranslatedName());
}

void Player::UpdateQuickAccess(const Controls &controls)
{
	for (int i = 0; i < static_cast<int>(mQuickAccessGroups.size()); ++i)
	{
		if (controls.IsKeyHit(static_cast<ControlKey::Type>(static_cast<int>(ControlKey::QuickAccess0) + i)))
		{
			if (!mQuickAccessGroups[i].expired())
			{
				UseItem(mQuickAccessGroups[i].lock()->mItems[0], Hand::Left);
			}
		}
	}

	game->GetHUD()->SetQuickAccess(mQuickAccessGroups);
}

void Player::Respawn()
{
	if (mDeathCount == 1)
	{
		game->RaiseDialog(make_shared<FirstDeathDialog>(nullptr));
	}

	game->GetHUD()->SetYouDiedVisible(false);
	mRespawnTime = 180;
	mStats->Resurrect();
	const auto location = GetLocation();
	const auto level = location->GetLevel();
	shared_ptr<MotherCrystal> crystal = mLastVisitedMotherCrystal;
	// if we have last visited crystal, resurrect player near it
	if (crystal)
	{
		// last visited crystal can be on other location, so we must check it
		if (crystal->GetScene() != location->GetScene())
		{
			level->TransferPlayerToLocation(crystal->GetLocation(), crystal->GetPosition());
		}
		else
		{
			SetPosition(crystal->GetPosition());
		}
	}
	else
	{
		// we have no last visited crystal, look for nearest crystal on current
		// location for resurrection
		if (location->GetMotherCrystalList().size())
		{
			crystal = location->GetMotherCrystalList().front();
			for (auto &otherCrystal : location->GetMotherCrystalList())
			{
				if (Math::Distance(GetPosition(), otherCrystal->GetPosition()) <
					Math::Distance(GetPosition(), crystal->GetPosition()))
				{
					if (otherCrystal->IsActive())
					{
						crystal = otherCrystal;
					}
				}
			}
		}
		if (crystal)
		{
			SetPosition(crystal->GetPosition());
		}
		else
		{
			// special case: no mother crystals at all, resurrect at left upper corner
			// of the location
			SetPosition({ 20, 20 });
		}
	}
	mModelRoot->SetLocalRotation(0);
}

Player::Player()
{
}

Player::Player(const shared_ptr<Location> &location) : Dwarf(location)
{
	mLight = location->GetScene()->CreateLight();
	mLight->AttachTo(mPivot);
	//mLight->SetColor({ 255, 255, 255 });
	mLight->SetColor({ 80, 80, 80 });
	mLight->SetRadius(80);
	
	mHealthBar->SetVisible(false);

	mViewTriangle.SetFov(60);
	mViewTriangle.SetViewDistance(200);
}

Player::~Player()
{
	mPivot->RemoveFromScene();
	// mModel->RemoveFromScene();
	mLight->RemoveFromScene();
	DBG_ECHO();
}

void Player::Think()
{
	THROW_IF_EXPIRED(mLocation);

	auto location = mLocation.lock();
	auto &scene = location->GetScene();
	auto &terrain = scene->GetTerrain();
	auto input = game->GetInput();
	auto &controls = game->GetControls();
	HUD* hud = game->GetHUD();
	auto inventory = GetInventory();
	auto renderer = game->GetRenderer();

	if (!terrain)
	{
		return;
	}

	auto body = GetBody();

	if (game->Debug.disablePlayerTerrainCollisions)
	{
		body->ResetFlags(BF_RESPONSE_ENABLED);
	}
	else
	{
		body->SetFlags(BF_RESPONSE_ENABLED);
	}

	if (IsDead())
	{
		hud->SetYouDiedVisible(true);

		--mRespawnTime;

		if (mRespawnTime <= 0)
		{
			Respawn();
		}
		else
		{
			// lock player movement until dead
			body->SetVelocity({ 0, 0 });
			mModelRoot->SetLocalRotation(90);

			// disable all animations until dead
			for (auto &animation : mAnimations)
			{
				animation->SetEnabled(false);
			}
		}
	}
	else
	{
		auto vel = body->GetVelocity();
		if (!IsInStunLock())
		{
			if (controls.IsKeyPressed(ControlKey::MoveRight))
			{
				body->SetVelocity({ mStats->mMoveSpeed.Get(), vel.y });
			}
			if (controls.IsKeyPressed(ControlKey::MoveLeft))
			{
				body->SetVelocity({ -mStats->mMoveSpeed.Get(), vel.y });
			}
		}

		static bool jumped = false;
		if (IsGroundContact())
		{
			jumped = false;
			body->SetGravity({ 0, 0 });
			if (controls.IsKeyHit(ControlKey::Jump) && mStats->ConsumeStamina(10) && !IsInStunLock())
			{
				body->SetVelocity(Vec2f(body->GetVelocity().x, -mStats->mJumpVerticalSpeed.Get()));

				SetAnimation(mJumpAnimation);
				mJumpAnimation->SetLooped(false);
				mJumpAnimation->SetTimePosition(0.0f);
				mJumpAnimation->SetSpeed(2);

				jumped = true;
			}
		}
		else
		{
			body->SetGravity({ Body::DefaultGravityX, Body::DefaultGravityY });
		}

		// Overwrite gravity for debug purposes
		if (game->Debug.gravityDisabled)
		{
			body->SetGravity({ 0, 0 });
		}

		vel = body->GetVelocity();

		if (IsInStunLock())
		{
			SetAnimation(mStunLockAnimation);
		}
		else
		{
			mStunLockAnimation->Rewind();
			mStunLockAnimation->SetWeight(0);
			if (controls.IsKeyPressed(ControlKey::MoveRight) ||
				controls.IsKeyPressed(ControlKey::MoveLeft))
			{
				if (!jumped)
				{
					SetAnimation(mWalkAnimation);
				}
			}
			else
			{
				body->SetVelocity({ 0, vel.y });
				if (!jumped)
				{
					SetAnimation(mStandAnimation);
				}
			}
		}

		UpdateView(location);

		UpdateFogOfWar();



		auto mousePos = input->GetMousePosition();
		auto center = renderer->GetWindow().mapCoordsToPixel(mPivot->GetGlobalPosition(), scene->GetView());
		bool flipModel = mousePos.x < static_cast<float>(center.x);
		auto aimDirection = Math::Normalized(mousePos - Vec2f(center));
		if (!IsInStunLock())
		{
			mModelRoot->SetLocalScale(flipModel ? -1.0f : 1.0f, 1.0f);
		}

		if (!IsInStunLock())
		{
			for (auto & handEquipment : GetHandSlots())
			{
				if (auto weapon = dynamic_pointer_cast<Weapon>(handEquipment))
				{
					weapon->OrientCrosshair(aimDirection);
				}
			}
			AnimateHandsAndHeadAndUpdateWeapons(aimDirection, flipModel);
			UpdateHandEquipment(aimDirection, controls, input);
		}
		UpdateCurrentShield(input, flipModel);
		hud->SetHandsEquipment(mHandSlots, mHandEquipmentConsumables, mInventory);
		auto rareRed = mInventory->FindGroup(ItemType::Crystal_RareRed);
		hud->SetHealth(mStats->GetHealth(), mStats->mMaxHealth.Get(), rareRed ? rareRed->Count() : 0);
		hud->SetStamina(mStats->GetStamina(), mStats->mMaxStamina.Get());
		hud->UpdateMinimap(location->GetMap()->GetTexture(), terrain->GetFogOfWar()->GetTexture(), GetPosition(), terrain->GetSizePixels());
		hud->SetExpirience(mStats->GetExpirience(), mStats->GetExpirienceForNextLevel());
		hud->NotifyStatPointsAvailable(mStats->GetStatPointCount());

		mOnLadder = false;
		if (!IsInStunLock())
		{
			InteractWithLadders(body, location);
		}

		if (mOnLadder || game->Debug.gravityDisabled)
		{
			if (controls.IsKeyPressed(ControlKey::ClimbUp))
			{
				body->SetVelocity({ body->GetVelocity().x, -3 });
			}
			if (controls.IsKeyPressed(ControlKey::ClimbDown))
			{
				body->SetVelocity({ body->GetVelocity().x, 3 });
			}
		}

		InteractWithItems();
		InteractWithRemains();
		InteractWithChests();
		InteractWithDoors();
		InteractWithActors();
		InteractWithMotherCrystals();
		InteractWithFogGates();
		InteractWithRooms();
		InteractWithBenches();
		UpdateQuickAccess(controls);
	}
}

void Player::Serialize(Serializer &sav)
{
	Dwarf::Serialize(sav);

	sav & mLight;

	sav &mRespawnTime;
	sav &mDeathCount;
	sav &mKillCount;
	sav &mLastVisitedMotherCrystal;

	for (auto &slot : mQuickAccessGroups)
	{
		sav &slot;
	}

	// link inventory with gui
	if (sav.IsLoading())
	{
		game->GetInventoryGUI()->SetInventory(mInventory);
	}
}

void Player::SetQuickAccess(const weak_ptr<ItemGroup> &group, int slotNum)
{
	if (static_cast<unsigned int>(slotNum) >= mQuickAccessGroups.size())
	{
		return;
	}
	THROW_IF_EXPIRED(mLocation);
	HUD* hud = game->GetHUD();
	for (auto &weakGroup : mQuickAccessGroups)
	{
		if (weakGroup.lock() == group.lock())
		{
			weakGroup.reset();
		}
	}
	mQuickAccessGroups[slotNum] = group;
	hud->SetQuickAccess(mQuickAccessGroups);
}

void Player::CreateInventory()
{
	THROW_IF_EXPIRED(mLocation);
	if (!mInventory)
	{
		// create inventory and link it with gui
		mInventory = make_shared<Inventory>(shared_from_this());
		game->GetInventoryGUI()->SetInventory(mInventory);
	}
}

void Player::UseItem(const shared_ptr<Item> &item, const Hand & hand)
{
	if (item)
	{
		const ItemDefinition & desc = item->GetDefinition();

		Dwarf::UseItem(item, hand);

		THROW_IF_EXPIRED(mLocation);

		if (item->GetType() == ItemType::Crystal_Homeward)
		{
			auto yes = [this]
			{
				if (mLastVisitedMotherCrystal)
				{
					SetPosition(mLastVisitedMotherCrystal->GetPosition());
				}
				mInventory->RemoveItem(mInventory->FindGroup(ItemType::Crystal_Homeward)->mItems.front());
			};
			game->GetHUD()->AskQuestion(game->GetStr("useHomewardCrystal"), yes, nullptr);
		}
		else if (desc.mClass == ItemClass::Book)
		{
			// Since book can be readed only from inventory, we must close inventory before open book reader
			game->GetInventoryGUI()->SetVisible(false);

			const auto br = game->GetBookReader();
			br->SetVisible(true);
			br->SetText(desc.GetTranslatedName(), desc.GetTranslatedBookText());
		}
		else if (desc.mClass == ItemClass::Projectile)
		{
			if (desc.mIsThrowableProjectile)
			{
				mInventory->RemoveItem(item);

				auto location = GetLocation();

				auto mousePos = game->GetInput()->GetMousePosition();
				auto center = game->GetRenderer()->GetWindow().mapCoordsToPixel(mPivot->GetGlobalPosition(), mPivot->GetScene()->GetView());
				auto aimDirection = Math::Normalized(mousePos - Vec2f(center));

				location->AddProjectile(make_shared<Projectile>(
					location,
					shared_from_this(),
					item->GetType(),
					GetPosition(),
					aimDirection * 25.0f,
					0.0f)
				);
			}
		}
	}
}

void Player::AddKill()
{
	++mKillCount;
}

int Player::GetKillCount() const
{
	return mKillCount;
}
