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

#include "Config.h"
#include "Cell.h"
#include "Keyframe.h"
#include "ItemDefinitionEditor.h"
#include "DialogEditor.h"
#include "TranslatedStringEditor.h"
#include "CommandStack.h"
#include "NodeType.h"

class InteractionMode;
class Animation;
class EditorCommand;

namespace ed
{
	class Timeline;
	class WorldOutliner;
}

class Editor
{
public:
	Editor();
	~Editor();
	void SetActive(bool active);
	bool IsActive() const;
	void DrawAndUpdate();
	void AddBrushTexture(const string &path);
	ed::CommandStack& GetCommandStack();
private:
	friend class ed::Timeline;
	friend class PaintTerrainInteractionMode;
	friend class SelectInteractionMode;
	friend class MoveInteractionMode;
	friend class ScaleInteractionMode;
	friend class MoveOriginInteractionMode;
	friend class RotateInteractionMode;
	friend class ed::WorldOutliner;

	unique_ptr<ed::Timeline> mTimeline;
	unique_ptr<ed::WorldOutliner> mWorldOutliner;

	static constexpr const char * OverwriteFilePopupName = "overwritePrefab";
	bool mActive { false };
	bool mDrawModeEnabled { false };

	shared_ptr<SceneNode> mSelectedNode;
	ed::CommandStack mCommandStack;
	shared_ptr<Scene> mEditorScene;
	vector<string> mTextureList;
	vector<string> mPrefabFileList;
	shared_ptr<Animation> mActiveAnimation;
	vector<shared_ptr<Sprite>> mDrawableNodes;
	vector<shared_ptr<SceneNode>> mTestIKChain;
	shared_ptr<Sprite> mIKTarget;
	sf::Color mOldFrameBufferClearColor;
	float mZoom { 1.0f };
	bool mUseLights { false };

	vector<unique_ptr<InteractionMode>> mInteractionModes;
	InteractionMode* mActiveMode { nullptr };

	ItemDefinitionEditor mItemDefinitionEditor;
	ed::DialogEditor mDialogEditor;
	ed::TranslatedStringEditor mTranslatedStringEditor;

	void CreateInteractionModes();
	void SceneEditor();
	void ScanTexturesFolder();
	void ScanPrefabsFolder();
	void RemoveKeyframeAtTime(vector<Keyframe> &keyframeList, float time);
	bool IsShortcutPressed(const vector<sf::Keyboard::Key> &keys);
	void QuestEditor();
	void DialogEditor();
	void OnSavePrefabClicked(const string & fileName);
	void OnLoadPrefabClicked(const string & fileName);
	void OverwritePrefabPopup(const string & fileName);
	void HandleShortcuts();
	void MainMenuBar();
	void DrawCoordinateGrid(const sf::View & view);
	Vec2f ScreenToWorld(const Vec2f & screen) const;
	void DrawSelectedNodeGizmo(const sf::View & view);
	void DrawSelectedNodeBounds(const sf::View & view, const ImColor & color);
	void FixKeyframesForSelectedNode(const shared_ptr<SceneNode> & parent);
	void TerrainOptionsTab();
	void DeleteTerrainPopupQuestion();
	void SceneRenderWindow();
	void SaveLoadSceneTab();
	bool IsPointInNodeBounds(const Vec2f& pointScreenCoords, const shared_ptr<SceneNode>& node);
	shared_ptr<SceneNode> CreateNodeOfType(const NodeType& type);
};

