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
#include "Serializer.h"

map<int32_t, Constructor> Serializer::mConstructors;

Serializer::Serializer(const string &filename, bool write)
	: mWrite(write), mVersion(CurrentVersion)
{
	mFile.open(filename, ios_base::binary | (write ? ios_base::out : ios_base::in));
	if (!mFile.is_open())
	{
		throw Exception("Unable to read " + filename);
	}

	mFileName = filename;

	// when we creating new archive, we put current version in beginning
	if (write)
	{
		mFile.write((const char *)&CurrentVersion, sizeof(CurrentVersion));
	}
	else
	{
		// when we loading existing archive, we read version into mVersion
		// to let user compare versions and make a decision
		mFile.read((char *)&mVersion, sizeof(mVersion));

		if (mVersion != CurrentVersion)
		{
			Log() << "WARNING: Version of file '" << filename << "' is " << mVersion
				<< ", but game was compiled with serializer version " << CurrentVersion << ". "
				<< "Now game will try to deserialize state, but without any "
				"guarantees."
				<< LineEnd;
		}
	}
}
