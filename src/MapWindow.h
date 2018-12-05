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

#include "Level.h"

#include "Location.h"

class LocationMark
{
public:
	enum class State
	{
		Normal,
		Selected,
		PlayerInside,
	};

private:
	shared_ptr<Label> mText;
	shared_ptr<Image> mImage;
	weak_ptr<Location> mLocation;
	string mLocationName;
	void MakeMarkImage(const shared_ptr<Canvas> &canvas, const shared_ptr<Window> &window, const Vec2f &position);
	void MakeLabel(
		const shared_ptr<Canvas> &canvas, const shared_ptr<Window> &window, const Vec2f &position, LocationType type);
	void NameLabel(LocationType type);

public:
	LocationMark(const shared_ptr<Canvas> &canvas, const shared_ptr<Window> &window, const Vec2f &position,
		const weak_ptr<Location> &location, LocationType type);
	~LocationMark();
	void AddOnHoverCallback(const function<void()> &func);
	shared_ptr<Location> GetLocation() const;
	const string &GetLocationName() const;
	void SetState(State state);
};

class MapWindow
{
private:
	shared_ptr<Canvas> mCanvas;
	shared_ptr<Window> mWindow;
	vector<unique_ptr<LocationMark>> mLocationMarks;
	shared_ptr<LineSet> mLocationLinks;
	shared_ptr<Label> mDestination;
	shared_ptr<Button> mJumpTo;
	shared_ptr<Tooltip> mTooltip;
	Vec2f mMapOrigin { 20, 40 };
	weak_ptr<Location> mDestLocation;

	void ClearMap();
	Vec2f NormalizedCoordinatesToWindow(const Vec2f &normCoord);
	void OnMarkHovered(LocationMark *mark);
	void DoFastTravel();
	void ResetMarksStateToNormal();
	shared_ptr<Location> GetPlayerLocation();
	// GUI Methods
	void MakeCanvas();
	void MakeWindow();
	void MakeDestinationLabel();
	void MakeJumpToButton();
	void MakeLocationLinks();
	void MakeTooltip();

public:
	MapWindow();
	void SyncWithWorld();
	void SetVisible(bool state);
	bool IsVisible() const;
	void SwitchVisible();
};
