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

#define ENUM_EXPAND_AS_ENUM(a, b) a,
#define ENUM_EXPAND_AS_STRINGS(a, b) b,

#define ENUM_DECLARE(Name, EnumMacro) \
	class Name { \
	public: \
		enum Type { EnumMacro(ENUM_EXPAND_AS_ENUM) }; \
		static constexpr const char* Str[] = { EnumMacro(ENUM_EXPAND_AS_STRINGS) }; \
		static constexpr const size_t EntryCount = sizeof(Str) / sizeof(Str[0]); \
		Name() { } \
		Name(Type value) : mValue(value) { } \
		Name(const string& str) { \
            size_t i = 0; \
            while(i < EntryCount && str != Str[i]) ++i; \
		    if(i < EntryCount) mValue = static_cast<Type>(i); \
			else throw std::runtime_error("Unknown " #Name " enum element: " + str); \
		} \
		operator Type() const { return mValue; } \
		void Serialize(Serializer &sav) { sav &mValue; } \
        string ToString() const { return Str[mValue]; } \
	private: \
		Type mValue = static_cast<Type>(0); \
	}; \
	namespace std { \
		template <> struct hash<Name> { \
		std::size_t operator()(const Name &k) const { \
			return std::hash<int>()(k); \
		} \
	}; \
	}
