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

#define CONTROL_KEY_ENUM_ENTRIES(X) \
	X(MoveLeft, "MoveLeft") \
	X(MoveRight, "MoveRight") \
	X(ClimbUp, "ClimbUp") \
	X(ClimbDown, "ClimbDown") \
	X(Jump, "Jump") \
	X(Use, "Use") \
	X(Back, "Back") \
	X(Next, "Next") \
	X(OpenInventory, "OpenInventory") \
	X(OpenStats, "OpenStats") \
	X(OpenJournal, "OpenJournal") \
	X(OpenMap, "OpenMap") \
	X(OpenEncyclopedia, "OpenEncyclopedia") \
	X(TakeScreenshot, "TakeScreenshot") \
	X(ZoomIn, "ZoomIn") \
	X(ZoomOut, "ZoomOut") \
	X(QuickAccess0, "QuickAccess0") \
	X(QuickAccess1, "QuickAccess1") \
	X(QuickAccess2, "QuickAccess2") \
	X(QuickAccess3, "QuickAccess3") \
	X(QuickAccess4, "QuickAccess4") \
	X(QuickAccess5, "QuickAccess5") \
	X(QuickAccess6, "QuickAccess6") \
	X(QuickAccess7, "QuickAccess7") \
	X(QuickAccess8, "QuickAccess8") \
	X(QuickAccess9, "QuickAccess9") \
	X(Count, "Count")

ENUM_DECLARE(ControlKey, CONTROL_KEY_ENUM_ENTRIES)

#undef CONTROL_KEY_ENUM_ENTRIES

class Controls
{
private:
	struct Key
	{
		sf::Keyboard::Key mCode { sf::Keyboard::Unknown };
		string mDescription { "Unknown Key" };
	};
	array<Key, static_cast<int>(ControlKey::Count)> mKeys;
	void SetKeyDescription(const ControlKey &key, const string &desc);

public:
	static constexpr const char *ConfigName = "keys.cfg";
	Controls();
	~Controls();
	void SetKey(const ControlKey &k, sf::Keyboard::Key v);
	string GetKeyDescription(const ControlKey &k);
	void WriteKeys();
	void LoadDefaults();
	bool IsKeyHit(const ControlKey &k) const;
	bool IsKeyPressed(const ControlKey &k) const;
	string GetKeyName(const ControlKey &k) const;
	sf::Keyboard::Key GetKey(const ControlKey &k) const;
};
