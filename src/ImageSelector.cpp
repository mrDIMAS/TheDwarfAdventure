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
#include "ImageSelector.h"
#include "Renderer.h"
#include "Game.h"

void ImageSelector::Open(const string & startDir, const function<void()>& onApply, const function<void()>& onClose)
{
	mOnApply = onApply;
	mOnClose = onClose;

	mOpen = true;

	mCurrentPath = startDir;

	while (!mPathStack.empty())
	{
		mPathStack.pop();
	}

	EnlistFiles(mCurrentPath);

	mSelectedIndex = 0;

	ImGui::OpenPopup(Id);
}

void ImageSelector::DrawAndUpdate()
{
	if (!mOpen)
	{
		return;
	}

	if (ImGui::Begin(Id, &mOpen, ImGuiWindowFlags_NoDocking))
	{
		ImGui::SetWindowFocus();

		ImGui::Columns(2, nullptr, false);

		ImGui::SetColumnWidth(-1, 60);
		if (ImGui::Button("< Back", { ImGui::GetColumnWidth(), 0 }))
		{
			if (!mPathStack.empty())
			{
				mCurrentPath = mPathStack.top();

				EnlistFiles(mCurrentPath);

				mPathStack.pop();
			}
		}

		ImGui::NextColumn();

		ImGui::PushItemWidth(ImGui::GetColumnWidth());
		ImGui::InputText("", (char*)mCurrentPath.c_str(), mCurrentPath.size(), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();

		ImGui::Columns(1);

		const auto getter = [] (void *data, int index, const char** out) -> bool
		{
			FileList* fileList = (FileList*)data;

			*out = fileList->at(index).second.c_str();

			return true;
		};

		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
		if (ImGui::ListBox("Files", &mSelectedIndex, getter, &mFiles, mFiles.size(), 10))
		{
			pair<filesystem::path, string> & entry = mFiles.at(mSelectedIndex);

			if (mFiles.at(mSelectedIndex).first.has_extension())
			{
				mSelectedTexture = game->GetRenderer()->RequestTexture(entry.first.string());
			}
			else
			{
				mPathStack.push(mCurrentPath);

				mCurrentPath = entry.first.string();

				EnlistFiles(mCurrentPath);
			}
		}
		ImGui::PopItemWidth();

		ImGui::Columns(2, nullptr, false);

		if (ImGui::Button("Apply", { ImGui::GetColumnWidth(), 0 }))
		{
			if (mOnApply)
			{
				mOnApply();
			}

			mOpen = false;
		}

		ImGui::NextColumn();

		if (ImGui::Button("Cancel", { ImGui::GetColumnWidth(), 0 }))
		{
			if (mOnClose)
			{
				mOnClose();
			}

			mOpen = false;
		}

		ImGui::Columns(1);

		ImGui::BeginChild("__ImgSel_Image", { 0, 0 }, true, ImGuiWindowFlags_AlwaysHorizontalScrollbar);
		if (mSelectedTexture)
		{
			ImGui::InvisibleButton("##1", { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });

			const Vec2f offset = { 8, 8 };

			ImGui::SetCursorScreenPos(ImGui::GetWindowPos() + offset);
			ImGui::Image((ImTextureID)mSelectedTexture->GetOpenGLTexture(), mSelectedTexture->GetSize());


			const Vec2f origin = ImGui::GetWindowPos() + offset;

			ImDrawList* dl = ImGui::GetWindowDrawList();

			// Draw bounds
			{
				const Vec2f screenMin = origin;
				const Vec2f screenMax = origin + mSelectedTexture->GetSize();

				dl->AddRect(screenMin, screenMax, ImColor(0, 255, 0, 120));
			}

			// Draw rect selector
			{
				const Vec2f cornerSize = { 6, 6 };
				const Vec2f halfCornerSize = cornerSize * 0.5f;
				const Vec2f screenMin = mMinRect + origin;
				const Vec2f screenMax = mMaxRect + origin;


				dl->AddRect(screenMin, screenMax, ImColor(255, 0, 0, 120));

				const Vec2f minDragRectMin = screenMin - halfCornerSize;
				const Vec2f minDragRectMax = screenMin + halfCornerSize;
				dl->AddRect(minDragRectMin, minDragRectMax, ImColor(255, 255, 0));
				if (Math::IsPointInsideRectangle(ImGui::GetCursorScreenPos(), minDragRectMin, cornerSize))
				{
					mDragPoint = &mMinRect;
					mClickPos = ImGui::GetCursorScreenPos();
					mInitialPos = mMinRect;
				}

				const Vec2f maxDragRectMin = screenMax - halfCornerSize;
				const Vec2f maxDragRectMax = screenMax + halfCornerSize;
				dl->AddRect(maxDragRectMin, maxDragRectMax, ImColor(255, 255, 0));
				if (Math::IsPointInsideRectangle(ImGui::GetCursorScreenPos(), maxDragRectMin, cornerSize))
				{
					mDragPoint = &mMaxRect;
					mClickPos = ImGui::GetCursorScreenPos();
					mInitialPos = mMaxRect;
				}

				if (mDragPoint)
				{
					if (ImGui::IsMouseDown(0))
					{
						*mDragPoint = mInitialPos + (ImGui::GetCursorScreenPos() - mClickPos);
					}
					else
					{
						mDragPoint = nullptr;
					}
				}
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

shared_ptr<Texture> ImageSelector::GetSelectedTexture() const
{
	return mSelectedTexture;
}

void ImageSelector::EnlistFiles(const string & from)
{
	mFiles.clear();

	for (auto &p : filesystem::directory_iterator(from))
	{
		auto & path = p.path();

		if (path.has_filename() && path.has_extension())
		{
			string ext = path.extension().string();

			if (ext == ".jpg" || ext == ".png" || ext == ".bmp")
			{
				mFiles.push_back(make_pair(path, path.filename().u8string()));
			}
		}
		else
		{
			mFiles.push_back(make_pair(path, "[DIR] " + path.filename().u8string()));
		}
	}

	// Make sure that folders will be first in the list
	const auto sorter = [] (const FileEntry& entry)
	{
		return !(entry.first.has_filename() && entry.first.has_extension());
	};

	std::partition(mFiles.begin(), mFiles.end(), sorter);
}
