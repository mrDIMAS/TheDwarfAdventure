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
#include "MapWindow.h"
#include "Canvas.h"
#include "Window.h"
#include "Image.h"
#include "Renderer.h"
#include "LineSet.h"
#include "Input.h"
#include "Door.h"
#include "Player.h"
#include "Tooltip.h"
#include "WorldGenerator.h"
#include "Location.h"

void MapWindow::ClearMap()
{
	mLocationLinks->Clear();
	mLocationMarks.clear();
}

Vec2f MapWindow::NormalizedCoordinatesToWindow(const Vec2f &normCoord)
{
	const auto size = mWindow->GetSize();
	return { mMapOrigin.x + normCoord.x * (size.x - 2 * mMapOrigin.x),
		mMapOrigin.y + normCoord.y * (size.y - 2 * mMapOrigin.y) };
}

void MapWindow::OnMarkHovered(LocationMark *mark)
{
	auto location = mark->GetLocation();

	// Disallow fast travel from current to current location
	if (location == GetPlayerLocation())
	{
		return;
	}

	if (location)
	{
		mTooltip->Show(mark->GetLocationName() + '\n' + game->GetStr("expectedLevel") + ": " + to_string(location->GetDefinition()->GetBaseLevel()));
	}

	if (game->GetInput()->IsMouseButtonHit(sf::Mouse::Left))
	{
		ResetMarksStateToNormal();
		mark->SetState(LocationMark::State::Selected);
		mDestLocation = mark->GetLocation();
		mJumpTo->SetEnabled(!mDestLocation.expired());
		if (auto destLocation = mDestLocation.lock())
		{
			const bool fastTravelAllowed = destLocation->Visited || game->Debug.allowFastTravelOnUndiscoveredLocations;
			mJumpTo->SetEnabled(fastTravelAllowed);
			if (fastTravelAllowed)
			{
				mDestination->SetText(game->GetStr("jumpToDestination") + mark->GetLocationName());
			}
			else
			{
				mDestination->SetText(game->GetStr("notVisited"));
			}
		}
	}
}

void MapWindow::DoFastTravel()
{
	if (mDestLocation.expired())
	{
		return;
	}
	const auto level = game->GetLevel();
	const auto location = mDestLocation.lock();
	Vec2f position;
	for (auto &door : location->GetDoorList())
	{
		if (door->GetType() == DoorType::ToOtherScene)
		{
			position = door->GetPosition();
			break;
		}
	}
	level->TransferPlayerToLocation(location, position);
	SetVisible(false);
}

void MapWindow::ResetMarksStateToNormal()
{
	const auto currentLocation = GetPlayerLocation();
	for (auto &otherMark : mLocationMarks)
	{
		if (otherMark->GetLocation() == currentLocation)
		{
			otherMark->SetState(LocationMark::State::PlayerInside);
		}
		else
		{
			otherMark->SetState(LocationMark::State::Normal);
		}
	}
}

shared_ptr<Location> MapWindow::GetPlayerLocation()
{
	const auto &level = game->GetLevel();
	return level ? level->GetPlayer()->GetLocation() : nullptr;
}

void MapWindow::MakeCanvas()
{
	mCanvas = game->CreateCanvas();
}

void MapWindow::MakeWindow()
{
	mWindow = mCanvas->Create<Window>();
	mWindow->SetCaption(game->GetStr("worldMap"));
	mWindow->SetAutoLayoutingEnabled(false);
	mWindow->SetSize({ 600, 420 });
	mWindow->SetInCenter();
}

void MapWindow::MakeDestinationLabel()
{
	mDestination = mCanvas->Create<Label>();
	mDestination->SetSize({ 400, 30 });
	mDestination->SetPosition({ 20, mWindow->GetSize().y - mDestination->GetSize().y * 1.5f });
	mWindow->AddChild(mDestination);
}

void MapWindow::MakeJumpToButton()
{
	mJumpTo = mCanvas->Create<Button>();
	mJumpTo->SetSize({ 150, 30 });
	mJumpTo->SetTextAlignment(TextAlignment::LeftCenter);
	mJumpTo->SetPosition({ mWindow->GetSize().x - mJumpTo->GetSize().x - 15,
		mWindow->GetSize().y - mJumpTo->GetSize().y * 1.5f });
	mJumpTo->SetText(game->GetStr("jumpTo"));
	mJumpTo->OnClick += [this] { DoFastTravel(); };
	mWindow->AddChild(mJumpTo);
}

void MapWindow::MakeLocationLinks()
{
	mLocationLinks = mCanvas->Create<LineSet>();
	mWindow->AddChild(mLocationLinks);
}

void MapWindow::MakeTooltip()
{
	mTooltip = mCanvas->Create<Tooltip>();
	mTooltip->SetSize({ 400, 280 });
}

MapWindow::MapWindow()
{
	MakeCanvas();
	MakeWindow();
	MakeLocationLinks();
	MakeDestinationLabel();
	MakeJumpToButton();
	MakeTooltip();
	SetVisible(false);
	Log() << "World map successfully initialized!" << LineEnd;
}

void MapWindow::SyncWithWorld()
{
	ClearMap();
	
	for (const shared_ptr<Location> & location : game->GetLevel()->GetLocationList())
	{
		const Vec2f position = NormalizedCoordinatesToWindow(location->GetDefinition()->GetPosition());

		unique_ptr<LocationMark> mark = make_unique<LocationMark>(mCanvas, mWindow, position, location, location->GetDefinition()->GetType());
		mark->AddOnHoverCallback([this, rawMark = mark.get()] { OnMarkHovered(rawMark); });
		mLocationMarks.push_back(move(mark));

		for (const weak_ptr<LocationDefinition> &weakNeighbourLocation : location->GetDefinition()->GetLinkedLocations())
		{
			const auto windowPos = mWindow->GetGlobalPosition();
			const auto begin = windowPos + position;
			const auto end = windowPos + NormalizedCoordinatesToWindow(weakNeighbourLocation.lock()->GetPosition());
			mLocationLinks->Add(begin, end, { 128, 128, 128 });
		}
	}
}

void MapWindow::SetVisible(bool state)
{
	if (state)
	{
		game->GetMapWindow()->SyncWithWorld();
	}
	mCanvas->SetVisible(state);
	mDestination->SetText(game->GetStr("jumpToDestination") + game->GetStr("destNotSelected"));
	mJumpTo->SetEnabled(false);

	ResetMarksStateToNormal();
}

bool MapWindow::IsVisible() const
{
	return mCanvas->IsVisible();
}

void MapWindow::SwitchVisible()
{
	SetVisible(!IsVisible());
}

void LocationMark::MakeMarkImage(
	const shared_ptr<Canvas> &canvas, const shared_ptr<Window> &window, const Vec2f &position)
{
	mImage = canvas->Create<Image>();
	mImage->SetTexture(game->GetRenderer()->RequestTexture("data/textures/gui/mark.png"));
	mImage->SetSize({ 15, 15 });
	mImage->SetPosition(position - mImage->GetSize() * 0.5f);
	window->AddChild(mImage);
}

void LocationMark::MakeLabel(
	const shared_ptr<Canvas> &canvas, const shared_ptr<Window> &window, const Vec2f &position, LocationType type)
{
	mText = canvas->Create<Label>();
	mText->SetSize({ 140, 20 });
	mText->SetAlignment(TextAlignment::Center);
	const float yOffset = mText->GetSize().y + mImage->GetSize().y * 0.5f;
	mText->SetPosition(position - Vec2f(mText->GetSize().x * 0.5f, yOffset));
	window->AddChild(mText);

	NameLabel(type);
}

LocationMark::LocationMark(const shared_ptr<Canvas> &canvas, const shared_ptr<Window> &window, const Vec2f &position,
	const weak_ptr<Location> &location, LocationType type)
	: mLocation(location)
{
	MakeMarkImage(canvas, window, position);
	MakeLabel(canvas, window, position, type);
}

LocationMark::~LocationMark()
{
	mImage->RemoveFromCanvas();
	mText->RemoveFromCanvas();
}

void LocationMark::NameLabel(LocationType type)
{
	switch (type)
	{
	case LocationType::UndergroundSettlement:
		mLocationName = game->GetStr("apartment");
		break;
	case LocationType::Bank:
		mLocationName = game->GetStr("bank");
		break;
	case LocationType::Caves:
		mLocationName = game->GetStr("caves");
		break;
	case LocationType::Hub:
		mLocationName = game->GetStr("hub");
		break;
	case LocationType::NaturalCaves:
		mLocationName = game->GetStr("naturalCaves");
		break;
	case LocationType::FlyingIsland:
		mLocationName = game->GetStr("flyingIslands");
		break;
    case LocationType::Settlement:
        mLocationName = "Settlement";
        break;
    case LocationType::Unknown:
        mLocationName = "Unknown";
        break;
	}
	mText->SetText(mLocationName);
}

void LocationMark::AddOnHoverCallback(const function<void()> &func)
{
	mImage->OnHover += func;
}

shared_ptr<Location> LocationMark::GetLocation() const
{
	return mLocation.lock();
}

const string &LocationMark::GetLocationName() const
{
	return mLocationName;
}

void LocationMark::SetState(State state)
{
	switch (state)
	{
	case State::Normal:
		mImage->SetColor({ 255, 255, 255 });
		break;
	case State::Selected:
		mImage->SetColor({ 0, 200, 0 });
		break;
	case State::PlayerInside:
		mImage->SetColor({ 200, 0, 0 });
		break;
	}
}
