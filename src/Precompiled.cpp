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

namespace Math
{
	std::mt19937 gRandomNumbersGenerator;
};

void LoadJSON(const string & filename, rapidjson::Document & doc)
{
	using namespace rapidjson;
	std::string buffer;
	{
		ifstream file(filename);
		buffer = string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	}
	ifstream ifs(filename);
	IStreamWrapper isw(ifs);
	auto &parseResult = doc.ParseStream(isw);
	if (parseResult.HasParseError())
	{
		int offset = doc.GetErrorOffset();
		string str = buffer.substr(offset > 0 ? offset - 1 : offset, 200);
		throw Exception(string("Error occured when parsing ") + filename + " file\n" +
			GetParseError_En(doc.GetParseError()) + " - offset " + to_string(offset) + ". Fragment: " + str);
	}
}

string RoundAndConvert(float n, unsigned int decimalPlaces)
{
	char buffer[128];
	sprintf(buffer, "%.*f", decimalPlaces, n);
	return buffer;
}

void RaiseMessageAndDie(const string & message) noexcept
{
#ifdef _WIN32
	MessageBoxA(0, message.c_str(), "Exception Caught!", MB_ICONERROR);
#endif
	std::terminate();
}

void ReportError(const string & message)
{
	Log() << "ERROR: Exception caught: " << message << LineEnd;
#ifdef _WIN32
	MessageBoxA(0, message.c_str(), "Exception Caught!", MB_ICONERROR);
#endif
}
