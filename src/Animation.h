// Copyright (C) 2016 - 2019 Stepanov Dmitriy - All Rights Resconsterved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Stepanov Dmitriy (a.k.a mr.DIMAS a.k.a v1al) <d1maxa@yandex.ru>

#pragma once

#include "Event.h"
#include "Serializer.h"

// note: time events are non-serializeable, because they contains lambdas
// which you must restore after deserialization
//
// work principle: when animation time passes mTime of the event
// mAction() is called
struct TimeEvent
{
	using Func = std::function<void()>;
	float mTime { 0.0f };
	Func mAction;
	TimeEvent()
	{
	}
	TimeEvent(float time, const Func &action) : mTime(time), mAction(action)
	{
	}
};

class Animation
{
private:
	vector<shared_ptr<AnimationTrack>> mTrackList;
	vector<TimeEvent> mTimeEvents;
	string mName { "UnnamedAnimation" };
	bool mEnabled { false };
	bool mLooped { false };
	float mSpeed { 1.0f };
	float mLength { 0.0f };
	float mTimePosition { 0.0f };
	float mWeight { 1.0f };
	float mFadeStep { 0.0f };

public:
	void UpdateTracks();
	Animation();
	~Animation();
	void SetEnabled(bool state);
	bool IsEnabled() const;
	void SetLooped(bool state);
	bool IsLooped() const;
	void SetSpeed(float speed);
	float GetSpeed() const;
	void SetTimePosition(float time);
	float GetTimePosition() const;
	void ChangeTimePosition(float amount);
	void Rewind();
	void ClipLengthToLongestTrack();
	void FadeOut(float time);
	void FadeIn(float time);
	bool IsFading() const;
	// do not call it manually, this method will be called into Scene::Update
	void Update(float dt = 1.0f / 60.0f);
	shared_ptr<AnimationTrack> AddTrack();
	shared_ptr<AnimationTrack> FindTrack(const string &name);
	shared_ptr<AnimationTrack> FindAssociatedTrack(const shared_ptr<SceneNode> &node);
	const vector<shared_ptr<AnimationTrack>> &GetTrackList() const;
	void Serialize(Serializer &sav);
	void SetLength(float length);
	float GetLength() const;
	void ChangeLength(float value);
	void SetWeight(float weight);
	float GetWeight() const;
	void RemoveTrack(const shared_ptr<AnimationTrack> &track);
	shared_ptr<Animation> MakeCopy() const;
	void SetName(const string &name);
	const string &GetName() const;
	void ResetAssociatedNodesTransform();
	bool IsEnded() const;
	void AddTimeEvent(TimeEvent &&evt);	
	bool HasInfluence() const;
};
