

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
#include "IK.h"
#include "Node.h"

#include "Sprite.h"

void GrabParentHierarchy(const shared_ptr<SceneNode> &node, vector<shared_ptr<SceneNode>> &nodes)
{
	nodes.push_back(node);
	if (auto parent = node->GetParent())
	{
		GrabParentHierarchy(parent, nodes);
	}
}

bool IKSolver::SolveChain(const vector<shared_ptr<SceneNode>> &nodes, const Vec2f &target)
{
	vector<Vec2f> scales;
	vector<shared_ptr<SceneNode>> allNodes;
	GrabParentHierarchy(nodes.back(), allNodes);
	for (auto &node : allNodes)
	{
		scales.push_back(node->GetLocalScale());
		node->SetLocalScale({ 1, 1 });
	}

	// simple CCD algorithm
	bool solutionFound = false;
	const int convergenceLimit = 5;
	for (int iteration = 0; iteration < convergenceLimit; ++iteration)
	{
		const auto &endNode = nodes.back();
		for (auto iter = nodes.rbegin(); iter != nodes.rend(); ++iter)
		{
			auto &node = *iter;
			float endNodeRotation;
			Math::DecomposeTransform(endNode->CalculateTransforms(), nullptr, nullptr, &endNodeRotation);
			const Vec2f endNodeLocalEnd = Math::RotateVector({ 0, endNode->GetSize().y }, endNodeRotation);
			const Vec2f endEffector = endNode->GetGlobalPosition() + endNodeLocalEnd;
			if (Math::SqrDistance(endEffector, target) > 0.0001f)
			{
				// calculate vectors
				const Vec2f jointPos = node->GetGlobalPosition();
				Vec2f fromJointToTarget = target - jointPos;
				Vec2f fromEndEffectorToJoint = endEffector - jointPos;
				float dAngle = Math::AngleBetweenVectors(fromEndEffectorToJoint, fromJointToTarget);
				node->SetLocalRotation(node->GetLocalRotation() + dAngle);
			}
			else
			{
				solutionFound = true;
				break;
			}
		}
		if (solutionFound)
		{
			break;
		}
	}
	for (size_t i = 0; i < scales.size(); ++i)
	{
		allNodes[i]->SetLocalScale(scales[i]);
	}
	return solutionFound;
}
