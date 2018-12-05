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

class Editor;

class InteractionMode
{
public:
	InteractionMode(Editor* owner);

	// Use this method to draw and update UI on side settings panel
	virtual void DrawSettingsUI() = 0;

	// Use this method to draw and update UI on scene previewer
	virtual void DrawPreviewerOverlay() = 0;

	virtual void Update() = 0;

	bool TrayButton(const Vec2f& size);

	const vector<sf::Keyboard::Key>& GetShortcut() const;

	// Appends shortcut description into description of this mode
	void SetShortcut(const vector<sf::Keyboard::Key>& shortcut);
protected:
	string mText;
	string mDescription;
	Editor* mOwner;	
	vector<sf::Keyboard::Key> mShortcut;	
};