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

// wrapper for sf::Sound
// actual need for this wrapper is that we have to serialize sounds
// and easily manipulate position of sound (i.e. attach sound to scene node)
class Sound
{
private:
	static constexpr float FadeRate = 0.05f;

	friend class Game;
	sf::Sound mSnd;
	shared_ptr<sf::SoundBuffer> mBuffer;
	// sound position will be set at global position of the node at each frame
	weak_ptr<SceneNode> mNode;
	float kVolume { 1.0f };
	float volume { 100 };
	bool mFadeIn { true };
public:
	Sound();
	// will automatically play sound
	Sound(const string &src);
	virtual ~Sound();
	void Serialize(Serializer &sav);
	bool IsPlaying() const;
	bool IsPaused() const;
	bool IsStopped() const;
	void Play();
	void PlayAt(const sf::Vector2f &position);
	void Pause();
	void Stop();
	Vec2f GetPosition() const;
	void SetPosition(const Vec2f &pos);
	void SetLoop(bool state);
	bool IsLoop() const;
	void SetVolume(float vol);
	float GetVolume() const;
	void Set2D(bool state);
	void AttachTo(const shared_ptr<SceneNode> &node);
	// only for scene-sounds
	void Update();
	void FadeIn();
	void FadeOut(bool instantly = false);
};
