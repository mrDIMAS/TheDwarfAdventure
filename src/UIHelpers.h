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

namespace ui
{
	struct BitFieldDefinition
	{
		string mName;
		int mPosition;

		BitFieldDefinition()
		{

		}

		BitFieldDefinition(const string& name, int pos)
			: mName(name)
			, mPosition(pos)
		{

		}
	};

	int GenPopupId();
	void PrepareFrame();
	char* GenId();
	void TextLabel(const string& name, const string& txt);

	void IntField(const string& name, int& value, float step, int min = 0, int max = 0);
	void ByteField(const string & name, uint8_t & value, uint8_t step, uint8_t min, uint8_t max);
	void BoolField(const string& name, bool& value);
	void FloatTableField(const string& name, float& value, float step, float min = 0, float max = 0);
	void PercentField(const string& name, float& value, float step);
	void ColorTableField(const string & name, sf::Color & value);
	void TranslatedStringField(const string& name, string & str);
	void PushId();
	void FloatRectField(const string & name, sf::FloatRect & value, float step, float min, float max);
	void Vector2TableField(const string & name, Vec2f & value, float step, float min, float max);

	template<unsigned int Size>
	void BitTableField(const string& name, int& field, BitFieldDefinition(&fieldDefinitions)[Size])
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();

		ImGui::BeginGroup();
		for (const BitFieldDefinition& fieldDef : fieldDefinitions)
		{
			bool checked = (field & fieldDef.mPosition) > 0;

			PushId();
			if (ImGui::Checkbox(fieldDef.mName.c_str(), &checked))
			{
				if (checked)
				{
					field |= fieldDef.mPosition;
				}
				else
				{
					field &= ~fieldDef.mPosition;
				}
			}
			ImGui::PopID();
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip(fieldDef.mName.c_str());
			}
		}
		ImGui::EndGroup();

		ImGui::Separator();
		ImGui::NextColumn();
	}

	template<class T>
	void EnumTableField(const string& name, T & enumeration)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();

		int index = enumeration;

		ImGui::PushItemWidth(ImGui::GetColumnWidth() - 5);
		if (ImGui::Combo(GenId(), &index, enumeration.Str, enumeration.EntryCount))
		{
			enumeration = static_cast<typename T::Type>(index);
		}
		ImGui::PopItemWidth();

		ImGui::Separator();
		ImGui::NextColumn();
	}
};
