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

#include "Serializer.h"
#include "Event.h"

struct FrameListener
{
	int mColumn { 0 };
	int mRow { 0 };
	string mName;
	bool mDone { false };

	// !!! WARNING !!!
	// You have to manually restore Action after deserialization
	// Best solution is to make RestoreListeners method inside user class
	// and call it inside constructor and after deserialization
	// There is must be some workaround, but I can't find it :)
	// Serialization of functions is very mind-blowing stuff
	Event FunctionalNode;

	FrameListener();
	void Serialize(Serializer &sav);
};

struct AnimationSequence
{
	string mName { "<undefined>" };
	float mSpeed { 24 }; // in frames per second
	int mXOffset { 0 };
	int mYOffset { 0 };
	int mWidth { 1 };
	int mHeight { 1 };
	bool mLooped { true };
	vector<shared_ptr<FrameListener>> mFrameListeners;
	AnimationSequence();
	AnimationSequence(const string &name);
	void Serialize(Serializer &sav);
	void CheckFrameListeners(int column, int row);
	void ResetListeners();
	shared_ptr<FrameListener> GetFrameListener(const string &name) const;
};

class FrameAnimation
{
private:
	bool mEnabled { false };
	float mRow { 0 };
	float mColumn { 0 };
	int mWidthFrames { 1 };
	int mHeightFrames { 1 };
	sf::FloatRect mTextureCoordinates;
	string mSource;
	shared_ptr<AnimationSequence> mCurrentSeq;
	vector<shared_ptr<AnimationSequence>> mSequences;

public:
	FrameAnimation();
	void LoadFromFile(const string &filename);
	void SetSequence(const string &name);
	shared_ptr<AnimationSequence> GetSequence(const string &name) const;
	void SetEnabled(bool state);
	bool IsEnabled() const;
	float GetCurrentFrame() const;
	void SetWidthFrames(int count);
	void SetHeightFrames(int count);
	int GetWidthFrames() const;
	int GetHeightFrames() const;
	sf::FloatRect GetTextureCoordinates() const;
	void UpdateTexCoords();
	void Update(float dt = 1.0f / FIXED_FPS);
	void Serialize(Serializer &sav);
};
