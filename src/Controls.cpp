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
#include "Controls.h"
#include "Config.h"
#include "Game.h"
#include "Input.h"

void Controls::SetKeyDescription(const ControlKey & key, const string & desc)
{
	mKeys[static_cast<int>(key)].mDescription = desc;
}

Controls::Controls()
{
}

Controls::~Controls()
{
	WriteKeys();
	DBG_ECHO();
}

void Controls::SetKey(const ControlKey & k, sf::Keyboard::Key v)
{
	for (auto &existing : mKeys)
	{
		if (existing.mCode == v)
		{
			swap(existing, mKeys[(int)k]);
			break;
		}
	}
	mKeys[static_cast<int>(k)].mCode = v;
}

string Controls::GetKeyDescription(const ControlKey & k)
{
	return mKeys[static_cast<int>(k)].mDescription;
}

void Controls::WriteKeys()
{
	// write config
	Config keysConfig;
	keysConfig.AddNumber("MoveLeft", GetKey(ControlKey::MoveLeft));
	keysConfig.AddNumber("MoveRight", GetKey(ControlKey::MoveRight));
	keysConfig.AddNumber("ClimbUp", GetKey(ControlKey::ClimbUp));
	keysConfig.AddNumber("ClimbDown", GetKey(ControlKey::ClimbDown));
	keysConfig.AddNumber("Jump", GetKey(ControlKey::Jump));
	keysConfig.AddNumber("OpenInventory", GetKey(ControlKey::OpenInventory));
	keysConfig.AddNumber("OpenStats", GetKey(ControlKey::OpenStats));
	keysConfig.AddNumber("OpenJournal", GetKey(ControlKey::OpenJournal));
	keysConfig.AddNumber("OpenMap", GetKey(ControlKey::OpenMap));
	keysConfig.AddNumber("OpenEncyclopedia", GetKey(ControlKey::OpenEncyclopedia));
	keysConfig.AddNumber("Use", GetKey(ControlKey::Use));
	keysConfig.AddNumber("Back", GetKey(ControlKey::Back));
	keysConfig.AddNumber("Next", GetKey(ControlKey::Next));
	keysConfig.AddNumber("ZoomIn", GetKey(ControlKey::ZoomIn));
	keysConfig.AddNumber("ZoomOut", GetKey(ControlKey::ZoomOut));
	keysConfig.AddNumber("TakeScreenshot", GetKey(ControlKey::TakeScreenshot));
	keysConfig.AddNumber("QuickAccess0", GetKey(ControlKey::QuickAccess0));
	keysConfig.AddNumber("QuickAccess1", GetKey(ControlKey::QuickAccess1));
	keysConfig.AddNumber("QuickAccess2", GetKey(ControlKey::QuickAccess2));
	keysConfig.AddNumber("QuickAccess3", GetKey(ControlKey::QuickAccess3));
	keysConfig.AddNumber("QuickAccess4", GetKey(ControlKey::QuickAccess4));
	keysConfig.AddNumber("QuickAccess5", GetKey(ControlKey::QuickAccess5));
	keysConfig.AddNumber("QuickAccess6", GetKey(ControlKey::QuickAccess6));
	keysConfig.AddNumber("QuickAccess7", GetKey(ControlKey::QuickAccess7));
	keysConfig.AddNumber("QuickAccess8", GetKey(ControlKey::QuickAccess8));
	keysConfig.AddNumber("QuickAccess9", GetKey(ControlKey::QuickAccess9));
	keysConfig.Save(ConfigName);
}

void Controls::LoadDefaults()
{
	SetKeyDescription(ControlKey::MoveLeft, game->GetStr("moveLeft"));
	SetKeyDescription(ControlKey::MoveRight, game->GetStr("moveRight"));
	SetKeyDescription(ControlKey::ClimbUp, game->GetStr("climbUp"));
	SetKeyDescription(ControlKey::ClimbDown, game->GetStr("climbDown"));
	SetKeyDescription(ControlKey::Jump, game->GetStr("jump"));
	SetKeyDescription(ControlKey::OpenInventory, game->GetStr("inventory"));
	SetKeyDescription(ControlKey::OpenStats, game->GetStr("stats"));
	SetKeyDescription(ControlKey::OpenJournal, game->GetStr("journal"));
	SetKeyDescription(ControlKey::OpenMap, game->GetStr("worldMap"));
	SetKeyDescription(ControlKey::OpenEncyclopedia, game->GetStr("openEncyclopedia"));
	SetKeyDescription(ControlKey::Use, game->GetStr("use"));
	SetKeyDescription(ControlKey::Back, game->GetStr("prev"));
	SetKeyDescription(ControlKey::Next, game->GetStr("next"));
	SetKeyDescription(ControlKey::ZoomIn, game->GetStr("zoomIn"));
	SetKeyDescription(ControlKey::ZoomOut, game->GetStr("zoomOut"));
	SetKeyDescription(ControlKey::TakeScreenshot, game->GetStr("screenShot"));
	SetKeyDescription(ControlKey::QuickAccess0, game->GetStr("quickAccess0"));
	SetKeyDescription(ControlKey::QuickAccess1, game->GetStr("quickAccess1"));
	SetKeyDescription(ControlKey::QuickAccess2, game->GetStr("quickAccess2"));
	SetKeyDescription(ControlKey::QuickAccess3, game->GetStr("quickAccess3"));
	SetKeyDescription(ControlKey::QuickAccess4, game->GetStr("quickAccess4"));
	SetKeyDescription(ControlKey::QuickAccess5, game->GetStr("quickAccess5"));
	SetKeyDescription(ControlKey::QuickAccess6, game->GetStr("quickAccess6"));
	SetKeyDescription(ControlKey::QuickAccess7, game->GetStr("quickAccess7"));
	SetKeyDescription(ControlKey::QuickAccess8, game->GetStr("quickAccess8"));
	SetKeyDescription(ControlKey::QuickAccess9, game->GetStr("quickAccess9"));

	if (ifstream(ConfigName).is_open())
	{
		// load config
		Config keysConfig;
		keysConfig.Load(ConfigName);
		auto keyFromConfig = [&keysConfig] (const string &name)
		{
			return static_cast<sf::Keyboard::Key>(keysConfig.Get<int>(name));
		};
		SetKey(ControlKey::MoveLeft, keyFromConfig("MoveLeft"));
		SetKey(ControlKey::MoveRight, keyFromConfig("MoveRight"));
		SetKey(ControlKey::ClimbUp, keyFromConfig("ClimbUp"));
		SetKey(ControlKey::ClimbDown, keyFromConfig("ClimbDown"));
		SetKey(ControlKey::Jump, keyFromConfig("Jump"));
		SetKey(ControlKey::OpenInventory, keyFromConfig("OpenInventory"));
		SetKey(ControlKey::OpenStats, keyFromConfig("OpenStats"));
		SetKey(ControlKey::OpenJournal, keyFromConfig("OpenJournal"));
		SetKey(ControlKey::OpenMap, keyFromConfig("OpenMap"));
		SetKey(ControlKey::OpenEncyclopedia, keyFromConfig("OpenEncyclopedia"));
		SetKey(ControlKey::Use, keyFromConfig("Use"));
		SetKey(ControlKey::Back, keyFromConfig("Back"));
		SetKey(ControlKey::Next, keyFromConfig("Next"));
		SetKey(ControlKey::TakeScreenshot, keyFromConfig("TakeScreenshot"));
		SetKey(ControlKey::ZoomIn, keyFromConfig("ZoomIn"));
		SetKey(ControlKey::ZoomOut, keyFromConfig("ZoomOut"));
		SetKey(ControlKey::QuickAccess0, keyFromConfig("QuickAccess0"));
		SetKey(ControlKey::QuickAccess1, keyFromConfig("QuickAccess1"));
		SetKey(ControlKey::QuickAccess2, keyFromConfig("QuickAccess2"));
		SetKey(ControlKey::QuickAccess3, keyFromConfig("QuickAccess3"));
		SetKey(ControlKey::QuickAccess4, keyFromConfig("QuickAccess4"));
		SetKey(ControlKey::QuickAccess5, keyFromConfig("QuickAccess5"));
		SetKey(ControlKey::QuickAccess6, keyFromConfig("QuickAccess6"));
		SetKey(ControlKey::QuickAccess7, keyFromConfig("QuickAccess7"));
		SetKey(ControlKey::QuickAccess8, keyFromConfig("QuickAccess8"));
		SetKey(ControlKey::QuickAccess9, keyFromConfig("QuickAccess9"));
	}
	else
	{
		// set defaults
		SetKey(ControlKey::MoveLeft, sf::Keyboard::A);
		SetKey(ControlKey::MoveRight, sf::Keyboard::D);
		SetKey(ControlKey::ClimbUp, sf::Keyboard::W);
		SetKey(ControlKey::ClimbDown, sf::Keyboard::S);
		SetKey(ControlKey::Jump, sf::Keyboard::Space);
		SetKey(ControlKey::OpenInventory, sf::Keyboard::I);
		SetKey(ControlKey::OpenStats, sf::Keyboard::Tab);
		SetKey(ControlKey::OpenJournal, sf::Keyboard::J);
		SetKey(ControlKey::OpenMap, sf::Keyboard::M);
		SetKey(ControlKey::OpenEncyclopedia, sf::Keyboard::B);
		SetKey(ControlKey::Use, sf::Keyboard::E);
		SetKey(ControlKey::Back, sf::Keyboard::Q);
		SetKey(ControlKey::Next, sf::Keyboard::R);
		SetKey(ControlKey::ZoomIn, sf::Keyboard::X);
		SetKey(ControlKey::ZoomOut, sf::Keyboard::Z);
		SetKey(ControlKey::TakeScreenshot, sf::Keyboard::F12);
		SetKey(ControlKey::QuickAccess0, sf::Keyboard::Num1);
		SetKey(ControlKey::QuickAccess1, sf::Keyboard::Num2);
		SetKey(ControlKey::QuickAccess2, sf::Keyboard::Num3);
		SetKey(ControlKey::QuickAccess3, sf::Keyboard::Num4);
		SetKey(ControlKey::QuickAccess4, sf::Keyboard::Num5);
		SetKey(ControlKey::QuickAccess5, sf::Keyboard::Num6);
		SetKey(ControlKey::QuickAccess6, sf::Keyboard::Num7);
		SetKey(ControlKey::QuickAccess7, sf::Keyboard::Num8);
		SetKey(ControlKey::QuickAccess8, sf::Keyboard::Num9);
		SetKey(ControlKey::QuickAccess9, sf::Keyboard::Num0);
		WriteKeys();
	}
}

bool Controls::IsKeyHit(const ControlKey & k) const
{
	return game->GetInput()->IsKeyHit(GetKey(k));
}

bool Controls::IsKeyPressed(const ControlKey & k) const
{
	return game->GetInput()->IsKeyPressed(GetKey(k));
}

string Controls::GetKeyName(const ControlKey & k) const
{
	return game->GetInput()->GetKeyName(GetKey(k));
}

sf::Keyboard::Key Controls::GetKey(const ControlKey & k) const
{
	return mKeys[static_cast<int>(k)].mCode;
}
