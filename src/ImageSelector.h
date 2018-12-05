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

class ImageSelector
{
public:
	void Open(const string & startDir, const function<void()> & onApply, const function<void()> & onClose = nullptr);
	void DrawAndUpdate();
	shared_ptr<Texture> GetSelectedTexture() const;
	void EnlistFiles(const string & from);
private:
	using FileEntry = pair<filesystem::path, string>;
	using FileList = vector<FileEntry>;

	string mCurrentPath;
	stack<string> mPathStack;
	int mSelectedIndex { 0 };
	FileList mFiles;
	shared_ptr<Texture> mSelectedTexture;
	sf::FloatRect mRect;
	bool mOpen { false };
	function<void()> mOnApply;
	function<void()> mOnClose;
	Vec2f mMinRect;
	Vec2f mMaxRect { 100, 100 };
	Vec2f* mDragPoint;
	Vec2f mClickPos;
	Vec2f mInitialPos;

	static constexpr const char* Id = "Image Selector Popup";
};
