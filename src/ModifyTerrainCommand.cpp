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
#include "ModifyTerrainCommand.h"
#include "Cell.h"
#include "Terrain.h"

ModifyTerrainCommand::ModifyTerrainCommand(const shared_ptr<Terrain>& terrain, const vector<shared_ptr<Cell>>& currentState, const vector<shared_ptr<Cell>>& oldState)
{
	mDesc = "Modify terrain";
	mTerrain = terrain;	
	mCurrentState = currentState;
	mOldState = oldState;

	// HACK!
	Swap();
}

void ModifyTerrainCommand::Execute()
{
	Swap();
}

void ModifyTerrainCommand::Undo()
{
	Swap();
}

void ModifyTerrainCommand::Swap()
{
	const vector<shared_ptr<Cell>> temp = mCurrentState;
	mCurrentState = mOldState;
	mOldState = temp;

	// Put cells of current state back to terrain
	if (const shared_ptr<Terrain> terrain = mTerrain.lock())
	{
		for (const shared_ptr<Cell>& cell : mCurrentState)
		{
			const shared_ptr<Cell>& decompressedCopy = cell->MakeCopy();
			decompressedCopy->Decompress();
			terrain->SetCellAt(decompressedCopy, decompressedCopy->GetIndex());
		}
	}
}
