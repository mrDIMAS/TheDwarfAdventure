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
#include "Config.h"
#include <iostream>
#include <exception>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <streambuf>

template <>
const string& Config::Get<const string&>(const string &varName) const
{
	return GetExisting(varName);
}

template <>
string Config::Get<string>(const string &varName) const
{
	return GetExisting(varName);
}

template <>
float Config::Get<float>(const string &varName) const
{
	return static_cast<float>(atof(GetExisting(varName).c_str()));
}

template <>
int Config::Get<int>(const string &varName) const
{
	return atoi(GetExisting(varName).c_str());
}

template <>
bool Config::Get<bool>(const string &varName) const
{
	return atoi(GetExisting(varName).c_str()) != 0;
}

template <>
unsigned int Config::Get<unsigned int>(const string &varName) const
{
	return static_cast<unsigned int>(atoi(GetExisting(varName).c_str()));
}

Config::Config()
{
}

Config::Config(const string &filename)
{
	Load(filename);
}

void Config::LoadString(const string &str)
{
	mValues.clear();
	bool equalFound = false;
	bool quoteLF = false, quoteRF = false;
	bool comment = false;
	unsigned int n = 0;
	string varName, var;
	unsigned int lineNumber = 0;
	while (true)
	{
		char symbol = str[n];
		if (symbol == '\n')
		{
			++lineNumber;
		}
		if (symbol == '#')
		{
			comment = true;
		}
		if (comment && symbol == '\n')
		{
			comment = false;
		}
		if (!comment)
		{
			if (symbol == ';')
			{
				if (!quoteRF)
				{
					throw Exception(StringBuilder("Syntax error when parsing ") << mFileName << ": forgot value closing quote at line " + to_string(lineNumber + 1));
				}
				quoteLF = false;
				quoteRF = false;
				equalFound = false;
				varName.clear();
				var.clear();
			};
			if (isalpha((unsigned char)symbol) || isdigit((unsigned char)symbol) || symbol == '_')
			{
				if (!equalFound)
				{
					varName.push_back(symbol);
				}
			}
			else
			{
				if (symbol == '=')
				{
					equalFound = true;
				}
				if (symbol == '"')
				{
					if (quoteLF == false)
					{
						quoteLF = true;
					}
					else
					{
						quoteRF = true;
					}
				}
			};
			if (quoteLF)
			{
				if (quoteRF)
				{
					mValues.insert(make_pair(varName, var));
				}
				else if (symbol != '"')
				{
					var.push_back(symbol);
				}
			};
		}
		n++;
		if (n >= str.size())
		{
			break;
		}
	};
}

void Config::Load(const string &fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		throw Exception("Unable to parse file " + fileName + ". File does not exist!");
	}
	std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	mFileName = fileName;
	LoadString(buffer);
}

void Config::Save(const string &fileName) const
{
	ofstream file(fileName);

	// Preserve order of values. This will make diffs much clear
	map<string, string> orderedValues;
	for (auto &keyVal : mValues)
	{
		orderedValues[keyVal.first] = keyVal.second;
	}

	for (auto &keyVal : orderedValues)
	{
		file << keyVal.first << " = " << "\"" << keyVal.second << "\";" << endl;
	}
}

void Config::Save() const
{
	Save(mFileName);
}

unordered_map<string, string> Config::GetValuesCopy() const
{
	return mValues;
}

unordered_map<string, string>& Config::GetMap()
{
	return mValues;
}

void Config::SetNumber(const string &varName, float value)
{
	GetExisting(varName) = to_string(value);
}

void Config::SetString(const string &varName, const string &value)
{
	GetExisting(varName) = value;
}

void Config::AddNumber(const string &newVarName, float value)
{
	mValues[newVarName] = to_string(value);
}

void Config::AddString(const string &newVarName, const string &value)
{
	mValues[newVarName] = value;
}

bool Config::IsEmpty() const
{
	return mValues.empty();
}

string Config::GetPath() const
{
	return mFileName;
}

void Config::SetNumber(const string &varName, int value)
{
	GetExisting(varName) = to_string(value);
}

void Config::AddNumber(const string &newVarName, int value)
{
	mValues[newVarName] = to_string(value);
}

void Config::SetBoolean(const string &varName, bool value)
{
	GetExisting(varName) = value ? "1" : "0";
}

void Config::AddBoolean(const string &varName, bool value)
{
	mValues[varName] = value ? "1" : "0";
}

string& Config::GetExisting(const string &varName)
{
	const auto iter = mValues.find(varName);
	if (iter == mValues.end())
	{
		throw Exception("Unable to find value '" + varName + "' in " + mFileName + "!");
	}
	return iter->second;
}

const string& Config::GetExisting(const string &varName) const
{
	const auto iter = mValues.find(varName);
	if (iter == mValues.end())
	{
		throw Exception("Unable to find value '" + varName + "' in " + mFileName + "!");
	}
	return iter->second;
}
