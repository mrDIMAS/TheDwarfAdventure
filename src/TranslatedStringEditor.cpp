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
#include "TranslatedStringEditor.h"
#include "Game.h"

namespace ed
{
	void TranslatedStringEditor::DrawAndUpdate()
	{
		ImGui::Begin("Translated String Editor", &mVisible, ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					game->SaveLocalization();
				}

				if (ImGui::MenuItem("Close"))
				{
					mVisible = false;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Syncing"))
			{
				if (ImGui::MenuItem("Sync to Current"))
				{
					SyncLocalizations();
				}

				if (ImGui::MenuItem("Remove Unused Entries (check your diff after!)"))
				{
					RemoveUnusedEntries();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}


		unordered_map<string, string>& loca = game->GetCurrentLocalization();

		ImGui::Columns(2);

		ImGui::BeginChild("Key/Val", { ImGui::GetColumnWidth() - 15, 0 }, true);
		{
			ImGui::Columns(3, nullptr, false);

			ImGui::PushItemWidth(ImGui::GetColumnWidth());
			ImGui::InputText("##1", mKeyName.data(), mKeyName.size());
			ImGui::PopItemWidth();
			ImGui::NextColumn();
			if (ImGui::Button("Add", { ImGui::GetColumnWidth(), 0 }))
			{
				auto existing = loca.find(string(mKeyName.data()));
				if (existing == loca.end())
				{
					loca[mKeyName.data()] = "(empty)";
				}

				int index = 0;
				for (auto& kv : loca)
				{
					if (kv.first == mKeyName.data())
					{
						break;
					}
					++index;
				}

				mSelectedIndex = index;
			}
			ImGui::NextColumn();
			if (ImGui::Button("Remove Selected", { ImGui::GetColumnWidth(), 0 }))
			{
				if (mSelectedIndex >= 0 && mSelectedIndex < static_cast<int>(loca.size()))
				{
					auto iter = loca.begin();
					std::advance(iter, mSelectedIndex);
					loca.erase(iter);
				}
			}
			ImGui::Columns(1);
			// Table (Key, Value)
			ImGui::BeginChild("Table", { 0, 0 }, true);
			{
				int id = 0;

				ImGui::Columns(2);
				for (const auto & kv : loca)
				{
					ImGui::PushID(id);
					const bool highlight = id == mSelectedIndex;
					if (highlight)
					{
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.6f, 0.8f, 1));
					}
					if (ImGui::Button(kv.first.c_str(), { ImGui::GetColumnWidth(), 0 }))
					{
						mSelectedIndex = id;

						// Copy to edit buffer
						memcpy(mEditBuffer.data(), kv.second.c_str(), kv.second.size());
						mEditBuffer[kv.second.size()] = '\0';
					}
					if (highlight)
					{
						ImGui::PopStyleColor();
					}
					ImGui::PopID();
					ImGui::NextColumn();

					ImGui::Text("%.64s...", kv.second.c_str());
					ImGui::NextColumn();
					ImGui::Separator();
					++id;
				}
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();

		ImGui::NextColumn();
		// Editor
		ImGui::BeginChild("Editor", { ImGui::GetColumnWidth() - 15, 0 }, true);
		{
			if (mSelectedIndex >= 0 && mSelectedIndex < static_cast<int>(loca.size()))
			{
				auto iter = loca.begin();
				std::advance(iter, mSelectedIndex);
				ImGui::Columns(1);
				ImGui::Text("Editing: %s", iter->first.c_str());
				ImGui::InputTextMultiline("", mEditBuffer.data(), mEditBuffer.size(), { ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y - 30 });
				if (ImGui::Button("Apply", { ImGui::GetContentRegionAvailWidth(), 25 }))
				{
					// Copy string back
					iter->second = mEditBuffer.data();
				}
			}

		}
		ImGui::EndChild();

		ImGui::End();
	}

	void TranslatedStringEditor::SyncLocalizations()
	{
		const unordered_map<string, string>& currentLocalization = game->GetCurrentLocalization();

		for (auto &p : filesystem::recursive_directory_iterator("data/strings/"))
		{
			auto path = p.path();
						
			if (path != game->GetCurrentLocalizationConfig().GetPath() && path.has_extension())
			{
				auto ext = path.extension().string();
				if (ext == ".loca")
				{
					Config otherLocalizationConfig { path.string() };

					unordered_map<string, string>& otherLocalization = otherLocalizationConfig.GetMap();

					// First pass: look for missing entries in other localizations and add them
					for (const pair<string, string>& currentKeyValue : currentLocalization)
					{					
						if (otherLocalization.find(currentKeyValue.first) == otherLocalization.end())
						{
							otherLocalization[currentKeyValue.first] = currentKeyValue.second;
						}						
					}

					// Second pass: look for unused entries in other localizations and delete them
					unordered_map<string, string> result;
					for (const pair<string, string>& otherKeyValue : otherLocalization)
					{
						if (currentLocalization.find(otherKeyValue.first) != currentLocalization.end())
						{
							result[otherKeyValue.first] = otherKeyValue.second;
						}
					}

					// Swap localizations
					otherLocalization = result;

					// Save it back
					otherLocalizationConfig.Save();
				}
			}
		}
	}

	void TranslatedStringEditor::RemoveUnusedEntries()
	{
		vector<string> sources;

		// Load sources
		for (auto &p : filesystem::recursive_directory_iterator("../src/"))
		{
			auto path = p.path();

			if (path.has_extension())
			{
				auto ext = path.extension().string();
				if (ext == ".cpp" || ext == ".h")
				{
					std::ifstream file(path);
					sources.push_back(std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()));
				}
			}
		}

		// Load every json
		for (auto &p : filesystem::recursive_directory_iterator("data/"))
		{
			auto path = p.path();

			if (path.has_extension())
			{
				auto ext = path.extension().string();
				if (ext == ".json")
				{
					std::ifstream file(path);
					sources.push_back(std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()));
				}
			}
		}

		// Do a search from each localization
		for (auto &p : filesystem::recursive_directory_iterator("data/strings/"))
		{
			auto path = p.path();

			if (path.has_extension())
			{
				auto ext = path.extension().string();
				if (ext == ".loca")
				{
					Config localizationConfig { path.string() };

					unordered_map<string, string>& localization = localizationConfig.GetMap();

					vector<string> entriesForDelete;
					for (const pair<string, string>& keyValue : localization)
					{
						string decorated = '"' + keyValue.first + '"';

						bool found = false;
						for (const string& sourceFile : sources)
						{
							if (sourceFile.find(decorated) != string::npos)
							{
								found = true;
								break;
							}
						}

						if (!found)
						{
							entriesForDelete.push_back(keyValue.first);
						}
					}

					for (const string& delEntry : entriesForDelete)
					{
						localization.erase(delEntry);
					}

					localizationConfig.Save();
				}
			}
		}
	}
}