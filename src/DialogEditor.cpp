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
#include "DialogEditor.h"

ed::GraphEditor::GraphEditor()
{
	mGraph = make_unique<Graph>();
}

ed::GraphEditor::~GraphEditor()
{

}

void ed::GraphEditor::DrawAndUpdate()
{
	constexpr int BackgroundChannel = 0;
	constexpr int ForegroundChannel = 1;

	ImGui::Begin("Dialog Editor", &mVisible);

	ImGui::BeginChild("NodeList", { 200, 0 }, true, ImGuiWindowFlags_NoMove);

	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("ScrollingCanvas", { 0, 0 }, true, ImGuiWindowFlags_NoMove);

	mOpenContextMenu = ImGui::IsMouseReleased(1) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && !ImGui::IsAnyItemHovered();

	// Scrolling
	if (ImGui::IsMouseDown(2))
	{
		mScrolling = mScrolling + ImGui::GetIO().MouseDelta;
	}

	ImVec2 canvasPosition = ImGui::GetWindowPos();

	const ImVec2 offset = ImGui::GetCursorScreenPos();

	mDrawList = ImGui::GetWindowDrawList();

	DrawGrid();

	if (ImGui::IsMouseClicked(1))
	{
		mContextMenuNode.reset();
		mContextMenuSocket.reset();
	}

	if (ImGui::IsMouseDown(0))
	{
		if (mState == GraphEditorState::CreatingConnection)
		{
			ImVec2 p2 = ImGui::GetMousePos();
			ImVec2 p1 = mActiveConnection->GetBegin()->GetPosition();
			DrawConnection(p1, p2);
		}
	}
	else
	{
		if (mState == GraphEditorState::CreatingConnection)
		{
			const shared_ptr<Socket> beginSocket = mActiveConnection->GetBegin();

			const shared_ptr<Socket> endSocket = mHoveredSocket.lock();

			if (endSocket)
			{
				if (endSocket != beginSocket && endSocket->mDirection != beginSocket->mDirection)
				{
					mActiveConnection->SetEnd(endSocket);

					mActiveConnection->FixDirection();

					beginSocket->mConnectedTo = endSocket;
					endSocket->mConnectedTo = beginSocket;

					mGraph->mConnections.push_back(mActiveConnection);

					mActiveConnection.reset();
				}
			}
		}

		mState = GraphEditorState::Normal;
	}

	mHoveredSocket.reset();

	// Draw connections 
	for (const shared_ptr<Connection> & connection : mGraph->mConnections)
	{
		const shared_ptr<Socket> beginSocket = connection->GetBegin();
		const shared_ptr<Socket> endSocket = connection->GetEnd();

		if (beginSocket && endSocket)
		{
			DrawConnection(endSocket->GetPosition(), beginSocket->GetPosition());
		}
	}

	// Draw nodes
	shared_ptr<GraphNode> selectedNode = mSelectedNode.lock();

	for (const shared_ptr<GraphNode> & node : mGraph->mNodes)
	{
		ImGui::PushID(node->mVisualId);

		const ImVec2 rectMin = node->mPosition + offset + mScrolling;

		mDrawList->ChannelsSplit(2);

		mDrawList->ChannelsSetCurrent(ForegroundChannel);

		ImGui::SetCursorScreenPos({ GraphNode::IndicatorWidth + rectMin.x + GraphNode::Padding, rectMin.y + GraphNode::Padding });

		if (ImGui::IsMouseClicked(0))
		{
			node->mEditCaption = false;
		}

		if (node->mEditCaption)
		{
			ImGui::PushItemWidth(node->mSize.x);			
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0.1f));
			if (ImGui::InputText("##CaptionInputField", const_cast<char*>(node->mCaption.c_str()), node->mCaption.capacity(), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				node->mEditCaption = false;
			}
			ImGui::PopStyleColor();
			ImGui::PopItemWidth();
		}
		else
		{
			ImGui::Text(node->mCaption.c_str());
		}
		if (ImGui::IsItemClicked())
		{
			node->mEditCaption = true;
		}

		ImGui::SetCursorScreenPos({ GraphNode::IndicatorWidth + rectMin.x + GraphNode::Padding, rectMin.y + GraphNode::CaptionHeight + GraphNode::Padding });

		const float socketOriginY = ImGui::GetCursorScreenPos().y;

		ImGui::BeginGroup();
		DrawAndUpdateNodeSockets(socketOriginY, node, SocketDirection::In);
		ImGui::SameLine();
		DrawAndUpdateNodeSockets(socketOriginY, node, SocketDirection::Out);
		ImGui::EndGroup();

		node->mSize.x = std::max(ImGui::GetItemRectSize().x, GraphNode::MinWidth);
		node->mSize.y = std::max(ImGui::GetItemRectSize().y + GraphNode::CaptionHeight, GraphNode::MinHeight);

		node->mSize.x += 2 * GraphNode::Padding + GraphNode::IndicatorWidth;

		const ImVec2 rectMax = rectMin + node->mSize;

		mDrawList->ChannelsSetCurrent(BackgroundChannel);

		ImGui::SetCursorScreenPos(rectMin);
		ImGui::InvisibleButton("Node", node->mSize);

		if (ImGui::IsItemActive())
		{
			mSelectedNode = node;

			if (ImGui::IsMouseDragging())
			{
				node->mPosition = node->mPosition + ImGui::GetIO().MouseDelta;
			}
		}

		if (ImGui::IsMouseClicked(1) && ImGui::IsItemHovered())
		{
			mOpenContextMenu = true;

			mContextMenuNode = node;
		}

		bool showBorder = node == selectedNode;

		mDrawList->AddRectFilled(rectMin, { rectMin.x + GraphNode::IndicatorWidth, rectMax.y }, GraphNode::IndicatorColor);
		mDrawList->AddRectFilled({ rectMin.x + GraphNode::IndicatorWidth, rectMin.y }, rectMax, GraphNode::BackgroundColor);
		if (showBorder)
		{
			mDrawList->AddRect(rectMin, rectMax, GraphNode::SelectColor);
		}

		mDrawList->ChannelsMerge();

		ImGui::PopID();
	}

	if (mOpenContextMenu)
	{
		ImGui::OpenPopup("ContextMenu");

		mOpenContextMenu = false;
	}

	// Context menu	
	if (ImGui::BeginPopup("ContextMenu", ImGuiWindowFlags_NoTitleBar))
	{
		if (!mContextMenuNode.expired())
		{
			const shared_ptr<GraphNode> & node = mContextMenuNode.lock();

			if (ImGui::MenuItem("Remove Node"))
			{
				mGraph->RemoveNode(node);
			}
			if (ImGui::MenuItem("Add Output"))
			{
				const shared_ptr<Socket> socket = make_shared<Socket>();
				socket->SetDirection(SocketDirection::Out);
				socket->SetValue(string("Answer ") + to_string(node->mSockets.size()));
				node->mSockets.push_back(socket);				
			}
		}
		else
		{
			if (ImGui::MenuItem("Create Node Here"))
			{
				shared_ptr<GraphNode> node = mGraph->CreateNode();

				node->mPosition = ImGui::GetMousePos() - canvasPosition - mScrolling;
			}
		}

		if (!mContextMenuSocket.expired())
		{
			if (ImGui::MenuItem("Break Connection"))
			{
				const shared_ptr<Socket> & contextMenuSocket = mContextMenuSocket.lock();

				auto pred = [=] (const shared_ptr<Connection> & c) -> bool
				{
					bool match = c->GetBegin() == contextMenuSocket || c->GetEnd() == contextMenuSocket;

					if (match)
					{
						c->GetBegin()->ConnectTo(nullptr);
						c->GetEnd()->ConnectTo(nullptr);
					}

					return match;
				};

				mGraph->mConnections.erase(std::remove_if(mGraph->mConnections.begin(), mGraph->mConnections.end(), pred), mGraph->mConnections.end());
			}
		}

		ImGui::EndPopup();
	}

	ImGui::EndChild();
	ImGui::End();
}

void ed::GraphEditor::DrawConnection(const ImVec2 & p1, const ImVec2 & p2)
{
	ImVec2 begin = p1;
	ImVec2 end = p2;

	float sign = 1.0f;

	if (begin.x < end.x)
	{
		std::swap(begin, end);

		sign = -1.0f;
	}

	const float arrowSize = 6;
	const float thickness = 2;

	const Vec2f cp1 = begin - ImVec2(50, 0);
	const Vec2f cp2 = end + ImVec2(50, 0);

	const Vec2f centerDelta = Math::CubicBezier(begin, cp1, cp2, end, 0.49f);
	const Vec2f center = Math::CubicBezier(begin, cp1, cp2, end, 0.5f);

	const Vec2f tangent = Math::Normalized(center - centerDelta);

	const Vec2f perp = Math::Normalized(Math::Perpendicular(tangent));
	
	const Vec2f tangentOffset = sign * tangent * arrowSize;

	const Vec2f left = center - arrowSize * perp;
	const Vec2f right = center + arrowSize * perp;
	
	const ImColor clr = ImColor(0, 200, 0);

	mDrawList->AddBezierCurve(begin, cp1, cp2, end, clr, thickness);

	mDrawList->AddLine(center, left + tangentOffset, clr, thickness);
	mDrawList->AddLine(center, right + tangentOffset, clr, thickness);
}

void ed::GraphEditor::DrawGrid()
{
	const float w = ImGui::GetWindowWidth();
	const float h = ImGui::GetWindowHeight();

	const ImVec2 windowPos = ImGui::GetCursorScreenPos();

	for (float x = fmodf(mScrolling.x, GridStep); x < w; x += GridStep)
	{
		mDrawList->AddLine(ImVec2(x, 0.0f) + windowPos, ImVec2(x, h) + windowPos, GridColor);
	}

	for (float y = fmodf(mScrolling.y, GridStep); y < h; y += GridStep)
	{
		mDrawList->AddLine(ImVec2(0.0f, y) + windowPos, ImVec2(w, y) + windowPos, GridColor);
	}
}

void ed::GraphEditor::DrawAndUpdateNodeSockets(float yStart, const shared_ptr<GraphNode>& node, SocketDirection dir)
{
	ImGui::BeginGroup();
	float socketY = yStart;
	for (size_t i = 0; i < node->mSockets.size(); ++i)
	{
		if (node->mSockets[i]->GetDirection() == dir)
		{
			DrawAndUpdateSocket(node->mSockets[i], i);
			socketY += GraphNode::SegmentHeight;
		}
	}
	ImGui::EndGroup();
}

void ed::GraphEditor::DrawAndUpdateSocket(const shared_ptr<Socket>& socket, int index)
{	
	ImGui::PushID(index);
	ImGui::BeginGroup();

	ImVec2 textSize = ImGui::CalcTextSize(socket->GetValue().c_str());


	const ImVec2 socketBounds = { GraphNode::SocketRadius * 2, GraphNode::SocketRadius * 2 };
	const ImVec2 socketOffset = ImVec2(GraphNode::SocketRadius, textSize.y * 0.5f);

	ImVec2 socketPos;
	bool socketHovered = false;
	bool socketPressed = false;

	if (socket->GetDirection() == SocketDirection::In)
	{
		socketPos = ImGui::GetCursorScreenPos();
		ImGui::InvisibleButton("InputSocket", socketBounds);
		socketHovered |= ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
		ImGui::SameLine();
	}

	ImGui::Text(socket->GetValue().c_str());

	if (socket->GetDirection() == SocketDirection::Out)
	{
		ImGui::SameLine();
		socketPos = ImGui::GetCursorScreenPos();
		ImGui::InvisibleButton("OutputSocket", socketBounds);
		socketHovered |= ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
	}

	socket->SetPosition(socketPos + socketOffset);

	if (socketHovered)
	{
		mHoveredSocket = socket;

		if (ImGui::IsMouseDown(1))
		{
			mOpenContextMenu = true;

			mContextMenuSocket = socket;
		}
	}

	socketPressed = socketHovered && ImGui::IsMouseDown(0);

	const bool socketActivated = socketPressed || !socket->mConnectedTo.expired();

	if (socketActivated)
	{
		mDrawList->AddCircleFilled(socket->GetPosition(), GraphNode::SocketRadius - 3, Socket::SocketColor);
	}

	mDrawList->AddCircle(socket->GetPosition(), GraphNode::SocketRadius, Socket::SocketColor);

	if (mState != GraphEditorState::CreatingConnection)
	{
		const bool connectionAllowed = (socket->GetDirection() == SocketDirection::In) || (socket->GetDirection() == SocketDirection::Out && socket->mConnectedTo.expired());

		if (socketPressed && connectionAllowed)
		{
			mState = GraphEditorState::CreatingConnection;

			mActiveConnection = make_shared<Connection>();
			mActiveConnection->SetBegin(socket);
		}
	}

	ImGui::EndGroup();
	ImGui::PopID();
}

ed::Graph::Graph()
{

}

ed::Graph::~Graph()
{

}

void ed::Graph::Load(const string & file)
{
	UNUSED(file);
}

void ed::Graph::Save(const string & file)
{
	UNUSED(file);
}

shared_ptr<ed::GraphNode> ed::Graph::CreateNode()
{
	auto node = make_shared<GraphNode>();
	node->mVisualId = mNodes.size();
	mNodes.push_back(node);
	return node;
}

void ed::Graph::RemoveNode(const shared_ptr<GraphNode>& node)
{
	mNodes.erase(remove(mNodes.begin(), mNodes.end(), node), mNodes.end());
}

void ed::Graph::AddConnection(const shared_ptr<Connection>& connection)
{
	mConnections.push_back(connection);
}

ed::GraphNode::GraphNode()
{
	for (int i = 0; i < 1; ++i)
	{
		const shared_ptr<Socket> socket = make_shared<Socket>();
		socket->SetDirection(SocketDirection::In);
		socket->SetValue("Question");
		mSockets.push_back(socket);
	}

	for (int i = 0; i < 3; ++i)
	{
		const shared_ptr<Socket> socket = make_shared<Socket>();
		socket->SetDirection(SocketDirection::Out);
		socket->SetValue(string("Answer ") + to_string(i));
		mSockets.push_back(socket);
	}
}

ed::GraphNode::~GraphNode() { }

void ed::GraphNode::Load(const nlohmann::json & json)
{
	UNUSED(json);
}

void ed::GraphNode::Save(nlohmann::json & json)
{
	UNUSED(json);
}


ed::Socket::Socket()
{

}

ed::Socket::~Socket()
{

}

void ed::Socket::SetValue(const string & value)
{
	mValue = value;
}

const string & ed::Socket::GetValue() const
{
	return mValue;
}

void ed::Socket::SetDirection(SocketDirection dir)
{
	mDirection = dir;
}

ed::SocketDirection ed::Socket::GetDirection() const
{
	return mDirection;
}

void ed::Socket::SetPosition(const Vec2f & pos)
{
	mPosition = pos;
}

Vec2f ed::Socket::GetPosition() const
{
	return mPosition;
}

void ed::Socket::Load(const nlohmann::json & json)
{
	UNUSED(json);
}

void ed::Socket::Save(nlohmann::json & json)
{
	UNUSED(json);
}

void ed::Socket::ConnectTo(const shared_ptr<Socket>& socket)
{
	mConnectedTo = socket;
}

ed::Connection::Connection()
{

}

ed::Connection::~Connection()
{

}

void ed::Connection::SetBegin(const weak_ptr<Socket>& begin)
{
	mBegin = begin;
}

void ed::Connection::SetEnd(const weak_ptr<Socket>& end)
{
	mEnd = end;
}

void ed::Connection::Load(const nlohmann::json & json)
{
	UNUSED(json);
}

void ed::Connection::Save(nlohmann::json & json)
{
	UNUSED(json);
}

shared_ptr<ed::Socket> ed::Connection::GetBegin() const
{
	return mBegin.lock();
}

shared_ptr<ed::Socket> ed::Connection::GetEnd() const
{
	return mEnd.lock();
}

void ed::Connection::FixDirection()
{
	bool needSwap = false;

	if (const shared_ptr<Socket> socket = GetBegin())
	{
		if (socket->GetDirection() != SocketDirection::Out)
		{
			needSwap = true;
		}
	}

	if (const shared_ptr<Socket> socket = GetEnd())
	{
		if (socket->GetDirection() != SocketDirection::In)
		{
			needSwap = true;
		}
	}

	if (needSwap)
	{
		std::swap(mBegin, mEnd);
	}
}
