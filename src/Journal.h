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

class Canvas;
class Window;
class Table;
class ScrollableArea;
class Label;
class QuestStage;

// gui for quest handling
class Journal
{
private:
	string normalColor;
	string activeColor;
	string failedColor;
	string completeColor;
	shared_ptr<Canvas> mCanvas;
	shared_ptr<Window> mWindow;
	shared_ptr<Table> mQuestTable;
	shared_ptr<ScrollableArea> mCurrentQuestArea;
	vector<shared_ptr<Label>> mCurrentQuestMarks;
	void ClearQuestArea();
	void Clear();
	void OnRowClicked();
	void AddStageLabel(const string& prefix, const shared_ptr<QuestStage>& stage);
public:
	Journal();
	~Journal();
	void Update();
	bool IsVisible() const;
	void SetVisible(bool state);
	void SwitchVisible();
	void Sync();
};
