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

#include "EditorCommand.h"

class SetSelectedCommand final : public EditorCommand
{
public:
	SetSelectedCommand(shared_ptr<SceneNode> *selector, const shared_ptr<SceneNode> &newSelected,
		const shared_ptr<SceneNode> &lastSelected);
	virtual void Execute() override;
	virtual void Undo() override;
    
private:
	// selector always alive, so there is pointer to shared pointer :)
	shared_ptr<SceneNode> *mSelector;
	weak_ptr<SceneNode> mLastSelected;
	weak_ptr<SceneNode> mNewSelected;
};
