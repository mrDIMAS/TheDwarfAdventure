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

class Game;

class Input
{
private:
	static constexpr unsigned int KeyCount = 256;
	bool mKeyPressed[KeyCount] { false };
	bool mKeyHit[KeyCount] { false };
	bool mKeyReleased[KeyCount] { true };
	Vec2f mMousePosition;
	float mMouseWheelSpeed { 0 };
	bool mMouseButtonPressed[sf::Mouse::ButtonCount] { false };
	bool mMouseButtonHit[sf::Mouse::ButtonCount] { false };
	bool mMouseButtonUp[sf::Mouse::ButtonCount] { false };
	bool mMouseButtonReleased[sf::Mouse::ButtonCount] { true };
	vector<sf::Keyboard::Key> mKeysPressed;
	Vec2f mLastMousePosition;
	Vec2f mMouseSpeed;
	static const string mKeyNames[sf::Keyboard::KeyCount];

public:
	Input();
	~Input();
	void Listen(const sf::Event &event);
	float GetMouseWheelSpeed() const;
	bool IsKeyPressed(const sf::Keyboard::Key &key) const;
	bool IsKeyHit(const sf::Keyboard::Key &key) const;
	bool IsKeyReleased(const sf::Keyboard::Key &key) const;
	bool IsMouseButtonPressed(const sf::Mouse::Button &button) const;
	bool IsMouseButtonHit(const sf::Mouse::Button &button) const;
	bool IsMouseButtonUp(const sf::Mouse::Button &button) const;
	bool IsMouseButtonReleased(const sf::Mouse::Button &button) const;
	Vec2f GetMousePosition() const;
	void Flush();
	vector<sf::Keyboard::Key> GetPressedKeys() const;
	Vec2f GetMouseSpeed() const;
	const string& GetKeyName(const sf::Keyboard::Key &key) const;
};
