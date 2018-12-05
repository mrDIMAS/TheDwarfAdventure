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
#include "BehaviourTree.h"

namespace bt
{

Node::Node()
{
}

Node::~Node()
{
}

void Node::AddChild(const shared_ptr<Node>& child)
{
	mChildren.push_back(child);
}

ActionNode::ActionNode(const Func & doFunc) : mDo(doFunc)
{

}

Status ActionNode::Tick()
{
	return mDo();
}

ConditionNode::ConditionNode(const Func & doFunc) : mDo(doFunc)
{

}

Status ConditionNode::Tick()
{
	bool result = mDo();

	return result ? Status::Success : Status::Fail;
}

SelectorNode::SelectorNode()
{

}

Status SelectorNode::Tick()
{
	for (auto & child : mChildren)
	{
		if (child->Tick() == Status::Success)
		{
			return Status::Success;
		}
	}

	return Status::Fail;
}

SequenceNode::SequenceNode()
{
}

Status SequenceNode::Tick()
{
	for (auto & child : mChildren)
	{
		if (child->Tick() == Status::Fail)
		{
			return Status::Fail;
		}
	}

	return Status::Success;
}

ParallelNode::ParallelNode()
{

}

Status ParallelNode::Tick()
{
	for (auto & child : mChildren)
	{
		child->Tick();
	}

	return Status::Success;
}

Builder::Builder()
{

}

Builder & Builder::Sequence(const char * name)
{
	UNUSED(name);

	auto sequence = make_shared<SequenceNode>();

	if (!mActionStack.empty())
	{
		mActionStack.top()->AddChild(sequence);
	}

	mActionStack.push(sequence);

	return *this;
}

Builder & Builder::Selector(const char * name)
{
	UNUSED(name);

	auto selector = make_shared<SelectorNode>();

	if (!mActionStack.empty())
	{
		mActionStack.top()->AddChild(selector);
	}

	mActionStack.push(selector);

	return *this;
}

Builder & Builder::Do(const ActionNode::Func & func)
{
	if (mActionStack.empty())
	{
		throw Exception("BehaviourTreeBuilder: Action must be leaf node!");
	}

	mActionStack.top()->AddChild(make_shared<ActionNode>(func));

	return *this;
}

Builder & Builder::Condition(const ConditionNode::Func & func)
{
	if (mActionStack.empty())
	{
		throw Exception("BehaviourTreeBuilder: Condition must be leaf node!");
	}

	mActionStack.top()->AddChild(make_shared<ConditionNode>(func));

	return *this;
}

Builder & Builder::Parallel(const char * name)
{
	UNUSED(name);

	auto parallel = make_shared<ParallelNode>();

	if (!mActionStack.empty())
	{
		mActionStack.top()->AddChild(parallel);
	}

	mActionStack.push(parallel);

	return *this;
}

Builder & Builder::Inject(const shared_ptr<Node>& node)
{
	if (!mActionStack.empty())
	{
		mActionStack.top()->AddChild(node);
	}

	return *this;
}

Builder & Builder::Wait(WaitNode::State* state, int timeInFrames)
{
	if (mActionStack.empty())
	{
		throw Exception("BehaviourTreeBuilder: Wait must be leaf node!");
	}

	if (!mActionStack.empty())
	{
		state->mTimeToWait = timeInFrames;

		mActionStack.top()->AddChild(make_shared<WaitNode>(state));
	}

	return *this;
}

Builder & Builder::WaitAndDo(WaitAndDoNode::State* state, int waitTime, int doTime)
{
	if (mActionStack.empty())
	{
		throw Exception("BehaviourTreeBuilder: WaitAndDo must be leaf node!");
	}

	if (!mActionStack.empty())
	{
		state->mTimeToWait = waitTime;
		state->mTimeToDo = doTime;

		mActionStack.top()->AddChild(make_shared<WaitAndDoNode>(state));
	}

	return *this;
}

Builder & Builder::End()
{
	if (mActionStack.empty())
	{
		throw Exception("BehaviourTreeBuilder: Wrong number AddXXX/End pairs!");
	}
	mCurrentNode = mActionStack.top();
	mActionStack.pop();
	return *this;
}

shared_ptr<Node> Builder::Build()
{
	return mCurrentNode;
}

WaitNode::WaitNode(State* state) : mState(state)
{

}

Status WaitNode::Tick()
{
	++mState->mTime;

	if (mState->mTime >= mState->mTimeToWait)
	{
		mState->mTime = 0;
		return Status::Success;
	}

	return Status::Fail;
}

WaitAndDoNode::WaitAndDoNode(State* state) : mState(state)
{

}

Status WaitAndDoNode::Tick()
{
	--mState->mTime;

	if (mState->mWaiting)
	{
		if (mState->mTime <= 0)
		{
			mState->mTime = mState->mTimeToDo;

			mState->mWaiting = false;
		}
	}
	else
	{
		if (mState->mTime > 0)
		{
			return Status::Success;
		}
		else
		{
			mState->mTime = mState->mTimeToWait;

			mState->mWaiting = true;
		}
	}

	return Status::Fail;
}

}
