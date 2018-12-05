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
#include "Sound.h"
#include "Game.h"
#include "Node.h"

void Serialize(shared_ptr<sf::SoundBuffer> &buf, Serializer &sav)
{
	string src;
	if (sav.IsSaving())
	{
		src = game->GetSoundBufferFileName(buf);
	}
	sav &src;
	if (sav.IsLoading())
	{
		buf->loadFromFile(src);
		game->RegisterSound(src, buf);
	}
}

Sound::Sound()
{
}

Sound::Sound(const string &src)
{
	mBuffer = game->GetSoundBuffer(src);
	mSnd.setBuffer(*mBuffer.get());
	mSnd.setAttenuation(0.005f);
	mSnd.play();
}

Sound::~Sound()
{
	DBG_ECHO();
}

void Sound::Serialize(Serializer &sav)
{
	game = game;

	// buffer
	sav &mBuffer;
	if (sav.IsLoading())
	{
		mSnd.setBuffer(*mBuffer.get());
	}

	// volume
	float vol = mSnd.getVolume();
	sav &vol;
	mSnd.setVolume(vol);

	// status
	int status = mSnd.getStatus();
	sav &status;
	if (sav.IsLoading())
	{
		if (status == sf::SoundSource::Status::Paused)
		{
			mSnd.pause();
		}
		else if (status == sf::SoundSource::Status::Playing)
		{
			mSnd.play();
		}
		else if (status == sf::SoundSource::Status::Stopped)
		{
			mSnd.stop(); // is it really need?
		}
	}

	// playback position
	float posSec = mSnd.getPlayingOffset().asSeconds();
	sav &posSec;
	if (sav.IsLoading())
	{
		mSnd.setPlayingOffset(sf::seconds(posSec));
	}

	// position
	Vec2f pos = { mSnd.getPosition().x, mSnd.getPosition().y };
	sav &pos;
	mSnd.setPosition(pos.x, pos.y, 0);

	// pitch
	float pitch = mSnd.getPitch();
	sav &pitch;
	mSnd.setPitch(pitch);

	sav &mFadeIn;
}

bool Sound::IsPlaying() const
{
	return mSnd.getStatus() == sf::SoundSource::Status::Playing;
}

bool Sound::IsPaused() const
{
	return mSnd.getStatus() == sf::SoundSource::Status::Paused;
}

bool Sound::IsStopped() const
{
	return mSnd.getStatus() == sf::SoundSource::Status::Stopped;
}

void Sound::Play()
{
	mSnd.play();
}

void Sound::PlayAt(const sf::Vector2f &position)
{
	mSnd.play();
	SetPosition(position);
}

void Sound::Pause()
{
	mSnd.pause();
}

void Sound::Stop()
{
	mSnd.stop();
}

Vec2f Sound::GetPosition() const
{
	sf::Vector3f pos3d = mSnd.getPosition();

	return { pos3d.x, pos3d.y };
}

void Sound::SetPosition(const Vec2f &pos)
{
	mSnd.setPosition(pos.x, pos.y, 0);
}

void Sound::SetLoop(bool state)
{
	mSnd.setLoop(state);
}

bool Sound::IsLoop() const
{
	return mSnd.getLoop();
}

void Sound::SetVolume(float vol)
{
	mSnd.setVolume(vol);
	volume = vol;
}

float Sound::GetVolume() const
{
	return mSnd.getVolume();
}

void Sound::Set2D(bool state)
{
	mSnd.setRelativeToListener(state);
}

void Sound::AttachTo(const shared_ptr<SceneNode> &node)
{
	mNode = node;
}

void Sound::Update()
{
	if (!mNode.expired())
	{
		auto node = mNode.lock();
		SetPosition(node->GetGlobalPosition());
	}

	if (mFadeIn)
	{
		// fade in
		kVolume = Math::Min(kVolume + FadeRate, 1.0f);
	}
	else
	{
		// fade out
		kVolume = Math::Max(kVolume - FadeRate, 0.0f);
	}

	mSnd.setVolume(kVolume * volume);
}

void Sound::FadeIn()
{
	mFadeIn = true;
}

void Sound::FadeOut(bool instantly)
{
	if (instantly)
	{
		kVolume = 0.0f;
		mSnd.setVolume(0.0f);
	}
	mFadeIn = false;
}

