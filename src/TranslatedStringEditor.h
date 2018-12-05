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

namespace ed
{
	class TranslatedStringEditor
	{
	public:
		void DrawAndUpdate();
	private:
		bool mVisible { true };
		array<char, 32536> mEditBuffer { 0 };
		array<char, 255> mKeyName { 0 };
		int mSelectedIndex { -1 };

		// Uses current localization as reference and fixes other localizations
		// so they will contain all strings that are needed.
		// For example: russian localization contains 
		//   foo = "some string";
		//   foobar = "some string 2";
		// but english contains only 'foo' and 'foobaz', so this function will add
		// 'foobar' string to english localization and will remove 'foobaz'. 
		void SyncLocalizations();

		// Looks for strings in source files and removes unused from every localization
		// For example: in a localization we have 
		//   foo = "foobar";
		// but in code node such string "foobar", so entry will be deleted 		
		void RemoveUnusedEntries();
	};
}