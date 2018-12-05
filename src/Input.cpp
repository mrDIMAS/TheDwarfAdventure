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
#include "Input.h"

const string Input::mKeyNames[sf::Keyboard::KeyCount] = {
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Escape",
	"LControl",
	"LShift",
	"LAlt",
	"LSystem",
	"RControl",
	"RShift",
	"RAlt",
	"RSystem",
	"Menu",
	"LBracket",
	"RBracket",
	"SemiColon",
	"Comma",
	"Period",
	"Quote",
	"Slash",
	"BackSlash",
	"Tilde",
	"Equal",
	"Dash",
	"Space",
	"Return",
	"BackSpace",
	"Tab",
	"PageUp",
	"PageDown",
	"End",
	"Home",
	"Insert",
	"Delete",
	"Add",
	"Subtract",
	"Multiply",
	"Divide",
	"Left",
	"Right",
	"Up",
	"Down",
	"Numpad0",
	"Numpad1",
	"Numpad2",
	"Numpad3",
	"Numpad4",
	"Numpad5",
	"Numpad6",
	"Numpad7",
	"Numpad8",
	"Numpad9",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
	"Pause",
};

Input::Input()
{
	Log() << "Input successfully initialized!" << LineEnd;
}

Input::~Input()
{
	DBG_ECHO();
}

void Input::Listen(const sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (mKeyPressed[event.key.code])
		{
			mKeyHit[event.key.code] = false;
		}
		else
		{
			mKeyHit[event.key.code] = true;
		}
		mKeyPressed[event.key.code] = true;
		mKeyReleased[event.key.code] = false;
		mKeysPressed.push_back(event.key.code);
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		mKeyPressed[event.key.code] = false;
		mKeyReleased[event.key.code] = true;
		mKeyHit[event.key.code] = false;
	}
	else if (event.type == sf::Event::MouseMoved)
	{
		mMousePosition.x = static_cast<float>(event.mouseMove.x);
		mMousePosition.y = static_cast<float>(event.mouseMove.y);
		mMouseSpeed = mMousePosition - mLastMousePosition;
		mLastMousePosition = mMousePosition;
	}
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		if (mMouseButtonPressed[event.mouseButton.button])
		{
			mMouseButtonHit[event.mouseButton.button] = false;
		}
		else
		{
			mMouseButtonHit[event.mouseButton.button] = true;
		}
		mMouseButtonPressed[event.mouseButton.button] = true;
		mMouseButtonReleased[event.mouseButton.button] = false;
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		mMouseButtonPressed[event.mouseButton.button] = false;
		mMouseButtonReleased[event.mouseButton.button] = true;
		mMouseButtonUp[event.mouseButton.button] = true;
	}
	else if (event.type == sf::Event::MouseWheelScrolled)
	{
		mMouseWheelSpeed = event.mouseWheelScroll.delta;
	}
}

float Input::GetMouseWheelSpeed() const
{
	return mMouseWheelSpeed;
}

bool Input::IsKeyPressed(const sf::Keyboard::Key &key) const
{
	return mKeyPressed[key];
}

bool Input::IsKeyHit(const sf::Keyboard::Key &key) const
{
	return mKeyHit[key];
}

bool Input::IsKeyReleased(const sf::Keyboard::Key &key) const
{
	return mKeyReleased[key];
}

bool Input::IsMouseButtonPressed(const sf::Mouse::Button &button) const
{
	return mMouseButtonPressed[button];
}

bool Input::IsMouseButtonHit(const sf::Mouse::Button &button) const
{
	return mMouseButtonHit[button];
}

bool Input::IsMouseButtonUp(const sf::Mouse::Button &button) const
{
	return mMouseButtonUp[button];
}

bool Input::IsMouseButtonReleased(const sf::Mouse::Button &button) const
{
	return mMouseButtonReleased[button];
}

Vec2f Input::GetMousePosition() const
{
	return mMousePosition;
}

void Input::Flush()
{
	mKeysPressed.clear();
	for (unsigned int i = 0; i < KeyCount; ++i)
	{
		mKeyHit[i] = false;
	}
	for (int i = 0; i < sf::Mouse::ButtonCount; ++i)
	{
		mMouseButtonHit[i] = false;
		mMouseButtonUp[i] = false;
	}
	mMouseWheelSpeed = 0;
	mMouseSpeed = { 0, 0 };
}

vector<sf::Keyboard::Key> Input::GetPressedKeys() const
{
	return mKeysPressed;
}

Vec2f Input::GetMouseSpeed() const
{
	return mMouseSpeed;
}

const string& Input::GetKeyName(const sf::Keyboard::Key &key) const
{
	if (key >= sf::Keyboard::KeyCount || key < 0)
	{
		throw Exception(StringBuilder("Key ") << key << " has no name!");
	}
	return mKeyNames[key];
}
