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

#define BTFUNC(func) [this] { return func(); }

namespace bt
{

enum class Status
{
	Fail,
	Success,
};

class Node
{
protected:
	vector<shared_ptr<Node>> mChildren;
public:
	Node();
	virtual ~Node();
	void AddChild(const shared_ptr<Node> & child);
	virtual Status Tick() = 0;
};

class ActionNode : public Node
{
public:
	using Func = function<Status()>;
private:
	Func mDo;
public:
	ActionNode(const Func & doFunc);
	virtual Status Tick() override;
};

class ConditionNode : public Node
{
public:
	using Func = function<bool()>;
private:
	Func mDo;
public:
	ConditionNode(const Func & doFunc);
	virtual Status Tick() override;
};

class SelectorNode : public Node
{
public:
	SelectorNode();
	virtual Status Tick() override;
};

class SequenceNode : public Node
{
public:
	SequenceNode();
	virtual Status Tick() override;
};

class ParallelNode : public Node
{
public:
	ParallelNode();
	virtual Status Tick() override;
};

class WaitNode : public Node
{
public:
	struct State
	{
		int mTimeToWait { 0 };
		int mTime { 0 };

		void Serialize(Serializer & sav)
		{
			sav & mTimeToWait;
			sav & mTime;
		}
	};
private:
	State* mState { nullptr };
public:
	WaitNode(State* state);
	virtual Status Tick() override;
};

class WaitAndDoNode : public Node
{
public:
	// Main reason why state is separated out of node is that user needs to serialize state of node.
	// So state will be contained in client class and this node will use just pointer to it.
	// Internally user can serialize state as he wants. Behaviour tree is reconstructed on client's 
	// class deserialization and on this stage all closures will be reassigned correctly (we have to 
	// do this because we can't serialize closures).
	struct State
	{
		int mTimeToWait { 0 };
		int mTimeToDo { 0 };
		int mTime { 0 };
		bool mWaiting { true };

		void Serialize(Serializer & sav)
		{
			sav & mTimeToWait;
			sav & mTimeToDo;
			sav & mTime;
			sav & mWaiting;
		}
	};
private:
	State* mState { nullptr };
public:
	WaitAndDoNode(State* state);
	virtual Status Tick() override;
};

class Builder
{
private:
	shared_ptr<Node> mCurrentNode;
	stack<shared_ptr<Node>> mActionStack;
public:
	Builder();
	// Name not used, but you can mark nodes using it
	Builder & Sequence(const char * name = nullptr);
	Builder & Selector(const char * name = nullptr);
	Builder & Do(const ActionNode::Func & func);
	Builder & Condition(const ConditionNode::Func & func);
	Builder & Parallel(const char * name = nullptr);
	Builder & Inject(const shared_ptr<Node> & node);
	Builder & Wait(WaitNode::State* state, int timeInFrames);
	Builder & WaitAndDo(WaitAndDoNode::State* state, int waitTime, int doTime);
	Builder & End();
	shared_ptr<Node> Build();
};
}