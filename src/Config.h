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

class Config
{
public:
	Config();
	Config(const string &filename);
	void LoadString(const string &str);
	void Load(const string &fileName);
	void Save(const string &fileName) const;
	void Save() const;
	bool IsEmpty() const;
	string GetPath() const;
	unordered_map<string, string> GetValuesCopy() const;
	unordered_map<string, string> & GetMap();
	void SetNumber(const string &varName, float value);
	void SetNumber(const string &varName, int value);
	void SetBoolean(const string &varName, bool value);
	void SetString(const string &varName, const string &value);
	void AddNumber(const string &newVarName, float value);
	void AddNumber(const string &newVarName, int value);
	void AddString(const string &newVarName, const string &value);
	void AddBoolean(const string &varName, bool value);

	template <class T> T Get(const string &varName) const;		

private:
	string mFileName;
	unordered_map<string, string> mValues;
	string& GetExisting(const string &varName);
	const string& GetExisting(const string &varName) const;
};

