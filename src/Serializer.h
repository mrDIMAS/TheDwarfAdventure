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

class Game;

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <array>
#include <functional>
#include <unordered_map>

using namespace std;

template <typename T> class HasSerializeMethod
{
	typedef char one;
	typedef long two;

	template <typename C> static one test(decltype(&C::Serialize));
	template <typename C> static two test(...);

public:
	enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

struct Constructor
{
	std::string mTypeIdName;
	std::function<shared_ptr<void>()> mMakeSharedFunc;
	std::function<void*()> mMakeUniqueFunc;
};

// Supports only zero-args constructors for simplicity
// Save files are cross-compiler compatible
//
// WARNING! DO NOT TOUCH SERIALIZER WITHOUT THE NEED!
class Serializer
{
private:
	static map<int32_t, Constructor> mConstructors;
	map<uint32_t, shared_ptr<void>> mSharedPointers;
	fstream mFile;
	bool mWrite;
	string mFileName;
	// version of archive
	uint32_t mVersion;

public:
	// Increase this number when changing any serialization method when game is released.
	static constexpr uint32_t CurrentVersion = 1;

	// Use this method to register type and it's constructor
	template <typename T> static void AddType()
	{
		int32_t id = mConstructors.size();
		mConstructors[id].mMakeSharedFunc = [] { return make_shared<T>(); };
		mConstructors[id].mMakeUniqueFunc = [] { return new T(); };
		mConstructors[id].mTypeIdName = typeid(T).name();
	}
	Serializer(const string &filename, bool write);
	~Serializer()
	{
	}
	bool IsLoading() const
	{
		return !mWrite;
	}
	bool IsSaving() const
	{
		return mWrite;
	}
	string GetFileName() const
	{
		return mFileName;
	}
	uint32_t GetVersion() const
	{
		return mVersion;
	}

	// Serialize generic type with Serialize method
	template <typename T>
	typename enable_if<HasSerializeMethod<T>::value, void>::type operator&(T &value)
	{
		value.Serialize(*this);
	}

	// Serialize generic type directly
	template <typename T>
	typename enable_if<!HasSerializeMethod<T>::value, void>::type operator&(T &value)
	{
		static_assert(!std::is_class<T>::value,
			"Unable to serialize class/struct! Serialize fields directly to "
			"prevent alignment issues");

		if (mWrite)
		{
			mFile.write((const char *)&value, sizeof(value));
			if (mFile.fail())
				throw Exception("Serializer: Unable to write " + to_string(sizeof(value)) + " bytes!");
		}
		else
		{
			mFile.read((char *)&value, sizeof(value));
			if (mFile.fail())
				throw Exception("Serializer: Unable to read " + to_string(sizeof(value)) + " bytes!");
		}
	}

	// Serialize vector
	template <typename T> void operator&(sf::Vector2<T> &v)
	{
		*this &v.x;
		*this &v.y;
	}
	// Serialize float rect
	void operator&(sf::FloatRect &r)
	{
		*this &r.left;
		*this &r.top;
		*this &r.width;
		*this &r.height;
	}
	// Serialize color
	void operator&(sf::Color &c)
	{
		*this &c.r;
		*this &c.g;
		*this &c.b;
		*this &c.a;
	}
	// Serialize UTF8 string
	void operator&(string &str)
	{
		static char zeroByte = '\0';
		if (mWrite)
		{
			for (auto symbol : str)
			{
				*this &symbol;
			}
			*this &zeroByte;
		}
		else
		{
			str.clear();
			while (!mFile.eof())
			{
				char symbol;
				*this &symbol;
				if (symbol == zeroByte)
				{
					break;
				}
				else
				{
					str.push_back(symbol);
				}
			}
		}
	}
	// Serialize UTF32 string
	void operator&(sf::String &str)
	{
		static sf::Uint32 zero = (sf::Uint32)'\0';
		if (mWrite)
		{
			for (auto symbol : str)
			{
				*this &symbol;
			}
			*this &zero;
		}
		else
		{
			str.clear();
			while (!mFile.eof())
			{
				char32_t symbol;
				*this &symbol;
				if (symbol == zero)
				{
					break;
				}
				else
				{
					str += (sf::Uint32)symbol;
				}
			}
		}
	}

	// intrusive Serialize method
	template <typename T>
	typename std::enable_if<HasSerializeMethod<T>::value>::type CallSerialize(shared_ptr<T> &ptr)
	{
		ptr->Serialize(*this);
	}

	// non-intrusive global Serialize function
	template <typename T>
	typename std::enable_if<!HasSerializeMethod<T>::value>::type CallSerialize(shared_ptr<T> &ptr)
	{
		::Serialize(ptr, *this);	
	}

	int32_t TypeIDByType(const std::type_info &ti)
	{
		for (auto &kv : mConstructors)
		{
			if (kv.second.mTypeIdName == ti.name())
			{
				return kv.first;
			}
		}
		throw Exception(string("Type name '") + ti.name() + "' wasn't registered!");
	}

	template<typename T>
	int32_t GetTypeID(const T * ptr)
	{
		if (ptr == nullptr)
		{
			// place-holder type id
			return TypeIDByType(typeid(T));
		}
		else
		{
			// real type id
			return TypeIDByType(typeid(*ptr));
		}
	}

	// Serialize shared pointer
	template <typename T> void operator&(shared_ptr<T> &ptr)
	{
		int32_t typeID;
		if (IsSaving())
		{
			typeID = GetTypeID(ptr.get());
		}

		// serialize type name first
		*this &typeID;

		if (mWrite)
		{
			// write pointer
			uint32_t intPtr = (uint32_t)ptr.get();
			mFile.write((const char *)&intPtr, sizeof(intPtr));
			// try to serialize object
			if (ptr)
			{
				auto existing = mSharedPointers.find(intPtr);
				// serialize object on first call
				if (existing == mSharedPointers.end())
				{
					// register ptr
					mSharedPointers[intPtr] = ptr;
					// serialize it
					CallSerialize(ptr);
				}
			}
		}
		else
		{
			// read pointer
			uint32_t intPtr;
			mFile.read((char *)&intPtr, sizeof(intPtr));
			if (intPtr != 0)
			{
				// find already created and deserialized object
				auto existing = mSharedPointers.find(intPtr);
				if (existing != mSharedPointers.end())
				{
					// found, return it
					ptr = static_pointer_cast<T>(existing->second);
				}
				else
				{
					// not found, try to call constructor from mConstructors map
					try
					{
						// create objects
						ptr = static_pointer_cast<T>(mConstructors[typeID].mMakeSharedFunc());
						mSharedPointers[intPtr] = ptr;
						// and deserialize it
						CallSerialize(ptr);
					}
					catch (std::bad_function_call &)
					{
						// fail
						throw Exception(StringBuilder("Unable to create object from unregistered type '") << typeid(T).name() << "'!");
					}
				}
			}
		}
	}

	// Serialize weak pointer
	template <typename T> void operator&(weak_ptr<T> &ptr)
	{
		if (mWrite)
		{
			shared_ptr<T> locked = ptr.lock();
			*this &locked;
		}
		else
		{
			shared_ptr<T> lockedWeak;
			*this &lockedWeak;
			ptr = lockedWeak;
		}
	}

	// Serialize vector
	template <typename T> void operator&(vector<T> &v)
	{
		auto size = v.size();
		*this &size;
		if (IsLoading())
			v.resize(size);
		for (size_t i = 0; i < size; ++i)
		{
			*this &v[i];
		}
	}

	template<typename T>
	void operator&(unique_ptr<T> & ptr)
	{
		int32_t typeID;
		if (IsSaving())
		{
			typeID = GetTypeID(ptr.get());
		}

		// serialize type name first
		*this &typeID;

		if (IsLoading())
		{
			// make new object when loading
			auto constructor = mConstructors.find(typeID);
			if (constructor != mConstructors.end())
			{
				ptr.reset(reinterpret_cast<T*>(constructor->second.mMakeUniqueFunc()));
			}
			else
			{
				throw Exception(StringBuilder("Unable to create object from unregistered type '") << typeid(T).name() << "'!");
			}
		}

		ptr->Serialize(*this);
	}

	// Serialize binary data
	// Warning: This method does NOT allocates any memory on deserialization
	template <typename T> void SerializeBinary(T *v, size_t size)
	{
		*this &size;
		if (mWrite)
		{
			mFile.write((const char *)v, size);
			if (mFile.fail())
			{
				throw Exception(StringBuilder("Serializer: Unable to write ") << size << " bytes!");
			}
		}
		else
		{
			mFile.read((char *)v, size);
			if (mFile.fail())
			{
				throw Exception(StringBuilder("Serializer: Unable to read ") << size << " bytes!");
			}
		}
	}

	// Serialize map
	template <typename K, typename V> void operator&(map<K, V> &m)
	{
		auto size = m.size();
		*this &size;
		if (IsSaving())
		{
			for (auto &kv : m)
			{
				K k = kv.first;
				*this &k;
				*this &kv.second;
			}
		}
		else
		{
			for (decltype(size) i = 0; i < size; ++i)
			{
				K k;
				*this &k;
				V v;
				*this &v;
				m[k] = v;
			}
		}
	}

	// Serialize unordered map
	template <typename K, typename V> void operator&(unordered_map<K, V> &m)
	{
		auto size = m.size();
		*this &size;
		if (IsSaving())
		{
			for (auto &kv : m)
			{
				K k = kv.first;
				*this &k;
				*this &kv.second;
			}
		}
		else
		{
			for (decltype(size) i = 0; i < size; ++i)
			{
				K k;
				*this &k;
				V v;
				*this &v;
				m[k] = v;
			}
		}
	}

};
