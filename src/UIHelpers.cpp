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
#include "UIHelpers.h"
#include "Game.h"

namespace ui
{
	static int mControlId;

	int GenPopupId()
	{
		static int id = 0;
		return id++;
	}

	void PrepareFrame()
	{
		mControlId = 0;
	}

	char * GenId()
	{
		static char idBuf[64];
		++mControlId;
		sprintf(idBuf, "##%d", mControlId);
		return idBuf;
	}

	void TextLabel(const string & name, const string & txt)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::Text(txt.c_str());

		ImGui::Separator();
		ImGui::NextColumn();
	}

	void IntField(const string & name, int & value, float step, int min, int max)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(ImGui::GetColumnWidth());
		ImGui::DragInt(GenId(), &value, step, min, max);
		ImGui::PopItemWidth();

		ImGui::Separator();
		ImGui::NextColumn();
	}

	void ByteField(const string & name, uint8_t & value, uint8_t step, uint8_t min, uint8_t max)
	{
		int integer = value;
		IntField(name, integer, step, min, max);
		value = static_cast<uint8_t>(integer);
	}

	void BoolField(const string & name, bool & value)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(ImGui::GetColumnWidth());
		ImGui::Checkbox(GenId(), &value);
		ImGui::PopItemWidth();

		ImGui::Separator();
		ImGui::NextColumn();
	}

	void FloatTableField(const string & name, float & value, float step, float min, float max)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(ImGui::GetColumnWidth());
		ImGui::DragFloat(GenId(), &value, step, min, max, "%.2f");
		ImGui::PopItemWidth();

		ImGui::Separator();
		ImGui::NextColumn();
	}
	
	void FloatRectField(const string & name, sf::FloatRect & value, float step, float min, float max)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(ImGui::GetColumnWidth() - 50);
		ImGui::BeginGroup();
		PushId();
		ImGui::DragFloat("Left", &value.left, step, min, max, "%.2f");
		ImGui::DragFloat("Top", &value.top, step, min, max, "%.2f");
		ImGui::DragFloat("Width", &value.width, step, min, max, "%.2f");
		ImGui::DragFloat("Height", &value.height, step, min, max, "%.2f");
		ImGui::PopID();
		ImGui::EndGroup();
		ImGui::PopItemWidth();

		ImGui::Separator();
		ImGui::NextColumn();
	}
	
	void Vector2TableField(const string & name, Vec2f & value, float step, float min, float max)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(ImGui::GetColumnWidth() - 35);
		ImGui::BeginGroup();
		PushId();
		ImGui::DragFloat("X", &value.x, step, min, max, "%.2f");
		ImGui::DragFloat("Y", &value.y, step, min, max, "%.2f");
		ImGui::PopID();
		ImGui::EndGroup();
		ImGui::PopItemWidth();

		ImGui::Separator();
		ImGui::NextColumn();
	}

	void PercentField(const string & name, float & value, float step)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();

		float percent = value * 100.0f;

		ImGui::PushItemWidth(ImGui::GetColumnWidth());
		if (ImGui::DragFloat(GenId(), &percent, step, 0, 100, "%.2f%%"))
		{
			value = percent / 100.0f;
		}
		ImGui::PopItemWidth();

		ImGui::Separator();
		ImGui::NextColumn();
	}

	void ColorTableField(const string & name, sf::Color & value)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();

		float colors[4] = {
			static_cast<float>(value.r / 255.0f) ,
			static_cast<float>(value.g / 255.0f),
			static_cast<float>(value.b / 255.0f),
			static_cast<float>(value.a / 255.0f)
		};

		ImGui::PushItemWidth(ImGui::GetColumnWidth());
		ImGui::ColorEdit4(GenId(), colors);
		
		value.r = static_cast<uint8_t>(colors[0] * 255);
		value.g = static_cast<uint8_t>(colors[1] * 255);
		value.b = static_cast<uint8_t>(colors[2] * 255);
		value.a = static_cast<uint8_t>(colors[3] * 255);
		
		ImGui::PopItemWidth();

		ImGui::Separator();
		ImGui::NextColumn();
	}

	void TranslatedStringField(const string & name, string & str)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();

		auto & loca = game->GetCurrentLocalization();

		int index = 0;
		for (auto & kv : loca)
		{
			if (kv.first == str)
			{
				break;
			}
			++index;
		}

		auto getter = [] (void* data, int index, const char** out) -> bool
		{
			auto loca = reinterpret_cast<unordered_map<string, string>*>(data);

			if (index >= 0 && index < (int)loca->size())
			{

				auto iter = loca->begin();
				std::advance(iter, index);

				*out = iter->first.c_str();

				return true;
			}

			return false;
		};

		ImGui::PushItemWidth(ImGui::GetColumnWidth());
		if (ImGui::Combo(GenId(), &index, getter, &loca, loca.size()))
		{
			auto iter = loca.begin();
			std::advance(iter, index);
			str = iter->first;
		}
		if (ImGui::IsItemHovered())
		{
			auto iter = loca.find(str);

			if (iter != loca.end())
			{
				ImGui::SetTooltip(iter->second.c_str());
			}
		}
		ImGui::PopItemWidth();

		ImGui::Separator();
		ImGui::NextColumn();
	}
	void PushId()
	{
		ImGui::PushID(++mControlId);
	}
}