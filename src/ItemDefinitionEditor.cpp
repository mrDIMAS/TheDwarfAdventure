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
#include "ItemDefinitionEditor.h"
#include "ItemDatabase.h"
#include "Game.h"
#include "Texture.h"
#include "Renderer.h"
#include "UIHelpers.h"

ItemDefinitionEditor::ItemDefinitionEditor()
{

}

void ImageSelectorButton(ImageSelector& selector, string& textureName, const shared_ptr<Texture>& texture, const sf::FloatRect & rect)
{
	const float tw = texture ? static_cast<float>(texture->GetWidth()) : 20;
	const float th = texture ? static_cast<float>(texture->GetHeight()) : 20;

	ImTextureID id = texture ? (ImTextureID)texture->GetOpenGLTexture() : nullptr;

	bool open = ImGui::ImageButton(id, { tw, th }, { rect.left, rect.top }, { rect.left + rect.width, rect.top + rect.height }, 1);

	if (open)
	{
		selector.Open("data/textures/", [&textureName, &selector]
		{
			if (selector.GetSelectedTexture())
			{
				textureName = selector.GetSelectedTexture()->GetSource();
			}
		});
	}
}


void ItemDefinitionEditor::DrawAndUpdate()
{
	ImGui::Begin("Item Definition Editor", &mVisible, ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				game->GetItemDatabase()->Save();
			}

			if (ImGui::MenuItem("Close"))
			{
				mVisible = false;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ItemDatabase::DefinitionCollection& db = game->GetItemDatabase()->GetRecords();

	static int selectedItem = -1;
	ImGui::BeginChild("Header", { 300, 0 });
	{
		auto getter = [] (void* data, int index, const char** out) -> bool
		{
			auto collection = reinterpret_cast<ItemDatabase::DefinitionCollection*>(data);

			if (index >= 0 && index < (int)collection->size())
			{
				auto iter = collection->begin();
				std::advance(iter, index);

				*out = game->GetStr(iter->second.mNameKey).c_str();

				return true;
			}

			return false;
		};
		
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
		ImGui::ListBox("##1", &selectedItem, getter, &db, db.size(), 20);
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("Table", { 0, 0 });
	{
		if (selectedItem >= 0 && selectedItem < (int)db.size())
		{
			auto iter = db.begin();
			std::advance(iter, selectedItem);

			ItemDefinition & definition = iter->second;

			ImGui::Columns(2);

			// Common fields
			ui::TextLabel("Type", definition.mType.ToString());
			ui::TextLabel("Class", definition.mClass.ToString());
			ui::TranslatedStringField("Name", definition.mNameKey);
			ui::TranslatedStringField("Description", definition.mDescriptionKey);
			ImageField("Texture", definition.mTexture, definition.mRect);
			ui::IntField("Price", definition.mPrice, 1, 0);
			ui::FloatTableField("Weight", definition.mWeight, 0.01f, 0);
			ui::FloatTableField("Durability", definition.mDurability, 0.01f, 0);
			ui::IntField("Rareness", definition.mRareness, 1, 0, 100);
			ui::BoolField("Phys. Body", definition.mHasPhysicalBody);
			ui::IntField("Req. Strength", definition.mRequiredStrengthForUse, 1, 0);
			ui::IntField("Req. Dexterity", definition.mRequiredDexterityForUse, 1, 0);
			ui::IntField("Req. Intelligence", definition.mRequiredIntelligenceForUse, 1, 0);
			
			// Compose items
			{
				ImGui::Text("Compose items");
				ImGui::NextColumn();
				ImGui::BeginGroup();				
				if (ImGui::Button("Add"))
				{
					definition.mComposeItems.push_back(ItemType::Rubbish);
				}
				for (ItemType& type : definition.mComposeItems)
				{
					int index = type;			
					ImGui::PushItemWidth(ImGui::GetColumnWidth() - fieldSpacing);
					if (ImGui::Combo(ui::GenId(), &index, type.Str, type.EntryCount))
					{
						type = static_cast<ItemType::Type>(index);
					}
					ImGui::PopItemWidth();					
				}
				ImGui::EndGroup();
				ImGui::Separator();
				ImGui::NextColumn();
			}

			switch (definition.mClass)
			{
			case ItemClass::Armor:// Fallthru
			case ItemClass::Gloves:// Fallthru
			case ItemClass::Trousers:// Fallthru
			case ItemClass::Ring:// Fallthru
			{
				ui::PercentField("Phys. Dmg. Reduction", definition.mPhysicalDamageReduction, 0.01f);
				ui::PercentField("Chem. Dmg. Reduction", definition.mChemicalDamageReduction, 0.01f);
				ui::PercentField("Fire. Dmg. Reduction", definition.mFireDamageReduction, 0.01f);
				ui::PercentField("Elec. Dmg. Reduction", definition.mElectricalDamageReduction, 0.01f);

				// Body part affectors
				{
					ImGui::Text("Body part affectors");
					ImGui::NextColumn();
					ImGui::BeginGroup();
					for (BodyPartDefinition& desc : definition.mBodyPartAffectors)
					{
						ImGui::Text(desc.mType.ToString().c_str());												
						mPreviewTexture = game->GetRenderer()->RequestTexture(desc.mTexturePath);
						ImGui::SameLine();
						ImageSelectorButton(mImgSelector, desc.mTexturePath, mPreviewTexture, desc.mRect);
						ImGui::Separator();						
					}
					ImGui::EndGroup();
					ImGui::Separator();
					ImGui::NextColumn();
				}
				break;
			}
			case ItemClass::Weapon:
				ui::EnumTableField("Consumable", definition.mConsumable);
				ui::BoolField("Is Melee", definition.mIsMeleeWeapon);
				// Fallthru
			case ItemClass::Projectile:
			{
				ui::FloatTableField("Damage", definition.mDamage, 0.01f, 0);
				ui::EnumTableField("DamageClass", definition.mDamageClass);
				ui::BoolField("Is Throwable", definition.mIsThrowableProjectile);
				break;
			}
			case ItemClass::Potion:
			{
				ui::FloatTableField("Heal", definition.mHeal, 0.01f, 0);
				ui::IntField("Heal Time", definition.mHealFrames, 1, 0);
				ui::FloatTableField("Stamina Regen", definition.mStaminaRestoration, 0.01f, 0);
				ui::IntField("Stamina Regen Time", definition.mStaminaRestorationFrames, 1, 0);
				break;
			}
			case ItemClass::Blueprint:
			{
				ui::EnumTableField("Blueprint result", definition.mBlueprintResultItem);
				break;
			}
			case ItemClass::Book:
			{
				ui::TranslatedStringField("Book Text", definition.mBookTextKey);
				break;
			}		
			}

		
		}
	}

	ImGui::EndChild();

	ImGui::End();

	mImgSelector.DrawAndUpdate();
}

void ItemDefinitionEditor::ImageField(const string& name, shared_ptr<Texture>& texture, const sf::FloatRect & rect)
{
	ImGui::Text(name.c_str());
	ImGui::NextColumn();

	if (texture)
	{
		const float tw = static_cast<float>(texture->GetWidth());
		const float th = static_cast<float>(texture->GetHeight());

		const ImVec2 uv0 = {
			rect.left / tw,
			rect.top / th
		};
		const ImVec2 uv1 = {
			(rect.left + rect.width) / tw,
			(rect.top + rect.height) / th
		};

		bool open = ImGui::ImageButton((ImTextureID)texture->GetOpenGLTexture(), { rect.width, rect.height }, uv0, uv1, 1);

		if (open)
		{
			mImgSelector.Open("data/textures/", [this, &texture] { texture = mImgSelector.GetSelectedTexture(); });
		}
	}

	ImGui::Separator();
	ImGui::NextColumn();
}
