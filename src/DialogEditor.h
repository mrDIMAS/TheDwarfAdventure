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

#include "Color.h"

namespace ed
{
	enum class SocketDirection
	{
		In, Out
	};

	class Connection;

	class Socket
	{
	public:
		friend class GraphEditor;

		static constexpr Color SocketColor = { 255, 255, 255, 255 };

		Socket();
		virtual ~Socket();

		void SetValue(const string & value);
		const string & GetValue() const;

		void SetDirection(SocketDirection dir);
		SocketDirection GetDirection() const;

		void SetPosition(const Vec2f & pos);
		Vec2f GetPosition() const;

		virtual void Load(const nlohmann::json & json);
		virtual void Save(nlohmann::json & json);

		void ConnectTo(const shared_ptr<Socket> & socket);
	private:
		string mValue;
		Vec2f mPosition;
		SocketDirection mDirection { SocketDirection::In };
		weak_ptr<Socket> mConnectedTo;
	};
				
	class Connection
	{
	public:
		Connection();
		virtual ~Connection();

		void SetBegin(const weak_ptr<Socket> & begin);
		void SetEnd(const weak_ptr<Socket> & end);
		
		// Will swap begin/end if needed
		void FixDirection();

		virtual void Load(const nlohmann::json & json);
		virtual void Save(nlohmann::json & json);

		shared_ptr<Socket> GetBegin() const;
		shared_ptr<Socket> GetEnd() const;
	private:	

		// Always 'Out' socket
		weak_ptr<Socket> mBegin;
		// Always 'In' socket
		weak_ptr<Socket> mEnd;
	};

	class GraphNode
	{
	public:
		friend class GraphEditor;
		friend class Graph;

		static constexpr float Padding = 5;
		static constexpr float CaptionHeight = 25;
		static constexpr float MinWidth = 100;
		static constexpr float MinHeight = 140;
		static constexpr float IndicatorWidth = 5;
		static constexpr float SegmentHeight = 26;
		static constexpr float SocketRadius = 6;
		static constexpr Color BackgroundColor = { 70, 70, 70, 150 };
		static constexpr Color IndicatorColor = { 255, 255, 255, 255 };
		static constexpr Color SelectColor = { 0, 0, 0, 255 };

		GraphNode();
		virtual ~GraphNode();

		virtual void Load(const nlohmann::json & json);
		virtual void Save(nlohmann::json & json);
	private:
		Vec2f mPosition;
		Vec2f mSize;
		int mVisualId;
		bool mEditCaption { false };

		string mCaption { "Node" };
		vector<shared_ptr<Socket>> mSockets;
	};

	class Graph
	{
	public:
		friend class GraphEditor;

		Graph();
		virtual ~Graph();

		void Load(const string & file);
		void Save(const string & file);
		shared_ptr<GraphNode> CreateNode();
		void RemoveNode(const shared_ptr<GraphNode> & node);

		void AddConnection(const shared_ptr<Connection> & connection);
	private:
		vector<shared_ptr<GraphNode>> mNodes;
		vector<shared_ptr<Connection>> mConnections;
	};

	enum class GraphEditorState
	{
		Normal,
		CreatingConnection
	};

	class GraphEditor
	{
	public:
		static constexpr float GridStep = 20;
		const ImColor GridColor { ImColor(100, 100, 100, 30) };

		GraphEditor();
		virtual ~GraphEditor();

		void DrawAndUpdate();
	private:
		void DrawAndUpdateNodeSockets(float yStart, const shared_ptr<GraphNode> & node, SocketDirection dir);
		void DrawAndUpdateSocket(const shared_ptr<Socket> & socket, int index);
		void DrawConnection(const ImVec2 & p1, const ImVec2 & p2);
		void DrawGrid();
		ImDrawList* mDrawList { nullptr };
		bool mVisible { true };
		bool mOpenContextMenu { false };
		GraphEditorState mState { GraphEditorState::Normal };
		unique_ptr<Graph> mGraph;
		weak_ptr<GraphNode> mContextMenuNode;
		weak_ptr<Socket> mContextMenuSocket;
		Vec2f mScrolling;
		weak_ptr<GraphNode> mSelectedNode;
		weak_ptr<Socket> mHoveredSocket;
		shared_ptr<Connection> mActiveConnection;
	};

	class DialogEditor : public GraphEditor
	{

	};
}