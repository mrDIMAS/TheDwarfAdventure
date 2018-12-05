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

using json = nlohmann::json;

// Asset is a human-readable form of a game resource
// It will be converted to a set of game entities on loading
// Loading is extremely flexible, errors in file ignored but message is logged
class Asset
{
private:
	json mRoot;
	vector<shared_ptr<Scene>> mSceneList;
	bool GetVec2(json & node, const string & name, Vec2f & v) const;
	bool GetString(json & node, const string & name, string & s) const;
	bool GetBoolean(json & node, const string & name, bool & b) const;
	bool GetColor(json & node, const string & name, sf::Color & clr) const;
	template<class T>
	bool GetNumber(json & node, const string & name, T & v) const;
	template<class T>
	bool GetRect(json & node, const string & name, sf::Rect<T> & r) const;
public:
	Asset();
	virtual ~Asset();
	void AddScene(const shared_ptr<Scene> & scene);
	void Save(const string &path);
	void Load(const string &path);
};

template<class T>
bool Asset::GetNumber(json & node, const string & name, T & v) const
{
	try
	{
		auto record = node.find(name);

		if (record != node.end())
		{
			if (record->is_number())
			{
				v = static_cast<T>(static_cast<double>(*record));

				return true;
			}
			else
			{
				Log() << name << " must be number!" << LineEnd;
			}
		}
		else
		{
			Log() << "Unable to find node with name " << name << LineEnd;
		}
	}
	catch (json::exception & e)
	{
		Log() << "JSON error occured: " << e.what() << LineEnd;
	}

	return false;
}

template<class T>
bool Asset::GetRect(json & node, const string & name, sf::Rect<T>& r) const
{
	try
	{
		auto record = node.find(name);

		if (record != node.end())
		{
			if (record->is_array())
			{
				if (record->size() >= 2)
				{
					r.left = record->at(0);
					r.top = record->at(1);
					r.width = record->at(2);
					r.height = record->at(3);

					return true;
				}
				else
				{
					Log() << "Rect must be array of four numbers!" << LineEnd;
				}
			}
			else
			{
				Log() << name << " must be array!" << LineEnd;
			}
		}
		else
		{
			Log() << "Unable to find node with name " << name << LineEnd;
		}
	}
	catch (json::exception & e)
	{
		Log() << "JSON error occured: " << e.what() << LineEnd;
	}

	return false;
}
