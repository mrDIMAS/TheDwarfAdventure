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
#include "FrameAnimation.h"

FrameAnimation::FrameAnimation()
{
}

void FrameAnimation::LoadFromFile(const string &path)
{
    string normalizedPath = path;
    for(auto& c : normalizedPath) 
    {
        if(c == '\\') 
        {
            c = '/';
        }
    }
    
	try
	{
		ifstream f(normalizedPath);
        
        if(!f.is_open()) 
        {
            throw Exception("Unable to open frame animation file: " + normalizedPath);
        }
        
		shared_ptr<AnimationSequence> currentSeq;
		while (!f.eof())
		{
			string rawLine;
			getline(f, rawLine);
            
            string line;
            for(char c : rawLine) 
            {                
                if(!iscntrl(c)) 
                {
                    line.push_back(c);
                }
            }

			// skip comments
			if (line.size() > 1)
			{
				if (line.at(0) == '\\' && line.at(1) == '\\')
				{
					continue;
				}
			}

			// tokenize line
			vector<string> tokens;
			const char *delim = " ";
			char *token = strtok(const_cast<char *>(line.c_str()), delim);
			while (token)
			{
				tokens.push_back(token);
				token = strtok(nullptr, delim);
			}

			if (tokens.size() < 2)
			{
				continue;
			}

			// interpret tokens
			if (tokens.at(0) == "#version")
			{
				// todo - add versioning
			}
			else if (tokens.at(0) == "#frameswidth")
			{
				mWidthFrames = atoi(tokens.at(1).c_str());
			}
			else if (tokens.at(0) == "#framesheight")
			{
				mHeightFrames = atoi(tokens.at(1).c_str());
			}
			else if (tokens.at(0) == "#seq")
			{
				currentSeq = make_shared<AnimationSequence>(tokens.at(1));
				mSequences.push_back(currentSeq);
			}
			else if (tokens.at(0) == "#xoffset")
			{
				if (!currentSeq)
					throw Exception("unable to set xoffset. no active seq! did you "
						"forgot to use #seq name?");
				currentSeq->mXOffset = atoi(tokens.at(1).c_str());
			}
			else if (tokens.at(0) == "#yoffset")
			{
				if (!currentSeq)
					throw Exception("unable to set yoffset. no active seq! did you "
						"forgot to use #seq name?");
				currentSeq->mYOffset = atoi(tokens.at(1).c_str());
			}
			else if (tokens.at(0) == "#width")
			{
				if (!currentSeq)
					throw Exception("unable to set width. no active seq! did you "
						"forgot to use #seq name?");
				currentSeq->mWidth = atoi(tokens.at(1).c_str());
			}
			else if (tokens.at(0) == "#height")
			{
				if (!currentSeq)
					throw Exception("unable to set height. no active seq! did you "
						"forgot to use #seq name?");
				currentSeq->mHeight = atoi(tokens.at(1).c_str());
			}
			else if (tokens.at(0) == "#speed")
			{
				if (!currentSeq)
					throw Exception("unable to set speed. no active seq! did you "
						"forgot to use #seq name?");
				currentSeq->mSpeed = static_cast<float>(atof(tokens.at(1).c_str()));
			}
			else if (tokens.at(0) == "#loop")
			{
				if (!currentSeq)
					throw Exception("unable to set loop. no active seq! did you "
						"forgot to use #seq name?");
				currentSeq->mLooped = atoi(tokens.at(1).c_str()) != 0;
			}
			else if (tokens.at(0) == "#listener")
			{
				if (!currentSeq)
					throw Exception("unable to add listener. no active seq! did you "
						"forgot to use #seq name?");
				auto listener = make_shared<FrameListener>();
				listener->mColumn = atoi(tokens.at(1).c_str());
				listener->mRow = atoi(tokens.at(2).c_str());
				listener->mName = tokens.at(3);
				currentSeq->mFrameListeners.push_back(listener);
			}
		}
		if (mSequences.empty())
		{
			throw Exception("Invalid anim file! No sequences found!");
		}
		mSource = normalizedPath;
		mCurrentSeq = mSequences.front();
	}
	catch (std::exception &e)
	{
		throw Exception(StringBuilder("Error occured when parsing animation file ")
			<< normalizedPath << ". Reason:\n"
			<< e.what());
	}
}

void FrameAnimation::SetSequence(const string &name)
{
	if (mCurrentSeq)
	{
		if (mCurrentSeq->mName == name)
		{
			return;
		}
	}

	for (auto &seq : mSequences)
	{
		if (seq->mName == name)
		{
			mCurrentSeq = seq;

			mColumn = static_cast<float>(seq->mXOffset);
			mRow = static_cast<float>(seq->mYOffset);

			return;
		}
	}

	throw Exception(StringBuilder("There is no animation sequence with name ") << name << "!");
}

shared_ptr<AnimationSequence> FrameAnimation::GetSequence(const string &name) const
{
	for (auto &seq : mSequences)
	{
		if (seq->mName == name)
		{
			return seq;
		}
	}
	return nullptr;
}

void FrameAnimation::SetEnabled(bool state)
{
	mEnabled = state;
}

bool FrameAnimation::IsEnabled() const
{
	return mEnabled;
}

float FrameAnimation::GetCurrentFrame() const
{
	return mRow * mWidthFrames + mColumn;
}

void FrameAnimation::SetWidthFrames(int count)
{
	mWidthFrames = count;
}

void FrameAnimation::SetHeightFrames(int count)
{
	mHeightFrames = count;
}

int FrameAnimation::GetWidthFrames() const
{
	return mWidthFrames;
}

int FrameAnimation::GetHeightFrames() const
{
	return mHeightFrames;
}

sf::FloatRect FrameAnimation::GetTextureCoordinates() const
{
	return mTextureCoordinates;
}

void FrameAnimation::UpdateTexCoords()
{
	// round values to make edgy change of frames
	int row = static_cast<int>(floor(mRow));
	int column = static_cast<int>(floor(mColumn));

	// translate values into [0;1]
	mTextureCoordinates.left = column / static_cast<float>(mWidthFrames);
	mTextureCoordinates.top = row / static_cast<float>(mHeightFrames);
	mTextureCoordinates.width = 1.0f / static_cast<float>(mWidthFrames);
	mTextureCoordinates.height = 1.0f / static_cast<float>(mHeightFrames);
}

void FrameAnimation::Update(float dt)
{
	if (mEnabled && mCurrentSeq)
	{
		mColumn += dt * mCurrentSeq->mSpeed;
		mCurrentSeq->CheckFrameListeners(static_cast<int>(mColumn), static_cast<int>(mRow));
		if (mColumn >= mCurrentSeq->mXOffset + mCurrentSeq->mWidth)
		{
			mColumn = static_cast<float>(mCurrentSeq->mXOffset);
			++mRow;
			if (mRow >= mCurrentSeq->mYOffset + mCurrentSeq->mHeight)
			{
				mEnabled = mCurrentSeq->mLooped;
				mRow = static_cast<float>(mCurrentSeq->mYOffset);
				mColumn = static_cast<float>(mCurrentSeq->mXOffset);
				mCurrentSeq->ResetListeners();
			}
		}
		UpdateTexCoords();
	}
}

void FrameAnimation::Serialize(Serializer &sav)
{
	sav &mSource;
	sav &mSequences;
	sav &mCurrentSeq;
	sav &mEnabled;
	sav &mRow;
	sav &mColumn;
	sav &mWidthFrames;
	sav &mHeightFrames;
	sav &mTextureCoordinates;
}

AnimationSequence::AnimationSequence()
{
}

AnimationSequence::AnimationSequence(const string &name) : mName(name)
{
}

void AnimationSequence::Serialize(Serializer &sav)
{
	sav &mName;
	sav &mSpeed;
	sav &mXOffset;
	sav &mWidth;
	sav &mHeight;
	sav &mLooped;
	sav &mFrameListeners;
}

void AnimationSequence::CheckFrameListeners(int column, int row)
{
	for (auto &listener : mFrameListeners)
	{
		if (!listener->mDone)
		{
			if (column == listener->mColumn && row == listener->mRow)
			{
				listener->FunctionalNode();
				listener->mDone = true;
			}
		}
	}
}

void AnimationSequence::ResetListeners()
{
	for (auto &listener : mFrameListeners)
	{
		listener->mDone = false;
	}
}

shared_ptr<FrameListener> AnimationSequence::GetFrameListener(const string &name) const
{
	for (auto &listener : mFrameListeners)
	{
		if (listener->mName == name)
		{
			return listener;
		}
	}
	return nullptr;
}

FrameListener::FrameListener()
{
}

void FrameListener::Serialize(Serializer &sav)
{
	sav &mColumn;
	sav &mRow;
	sav &mName;
	sav &mDone;
}
