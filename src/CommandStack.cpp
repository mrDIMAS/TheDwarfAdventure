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
#include "CommandStack.h"
#include "EditorCommand.h"

namespace ed
{
	CommandStack::CommandStack()
	{

	}

	CommandStack::~CommandStack()
	{

	}

	void CommandStack::Do(const shared_ptr<EditorCommand>& command)
	{
		for (int i = static_cast<int>(mCommands.size()) - 1; i >= mTop; --i)
		{
			mCommands.erase(mCommands.begin() + i);
		}

		++mTop;
		command->Execute();		
		mCommands.push_back(command);
	}

	void CommandStack::Undo()
	{
		if (CanUndo())
		{
			mCommands[--mTop]->Undo();
		}
	}

	void CommandStack::Redo()
	{
		if (CanRedo())
		{
			mCommands[mTop++]->Execute();
		}
	}

	void CommandStack::Clear()
	{
		mCommands.clear();
		mTop = 0;
	}

	bool CommandStack::CanRedo() const
	{
		return mTop < static_cast<int>(mCommands.size());
	}

	bool CommandStack::CanUndo() const
	{
		return mTop > 0;
	}
	const CommandStack::Stack& CommandStack::GetCommands() const
	{
		return mCommands;
	}
}