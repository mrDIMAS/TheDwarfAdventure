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

// Built-in editor based on imgui for fast development
// Game itself uses home-brewed gui for skinning support and other useful
// features

#include "Precompiled.h"
#include "Editor.h"
#include "Animation.h"
#include "AnimationTrack.h"
#include "Game.h"
#include "IK.h"
#include "Input.h"
#include "Level.h"
#include "Light.h"
#include "ParticleSystem.h"
#include "PerlinNoise.h"
#include "Player.h"
#include "PolyLine.h"
#include "Renderer.h"
#include "Scene.h"
#include "Serializer.h"
#include "Sprite.h"
#include "Terrain.h"
#include "Text.h"
#include "EditorCommand.h"
#include "CreateNodeCommand.h"
#include "CreateSpriteCommand.h"
#include "SetNodePositionCommand.h"
#include "SetNodeOriginCommand.h"
#include "SetNodeScaleCommand.h"
#include "SetNodeRotationCommand.h"
#include "SetSpriteTextureCommand.h"
#include "SetSpriteTextureRectangleCommand.h"
#include "SetSelectedCommand.h"
#include "SetSpriteSizeCommand.h"
#include "SetSpriteColorCommand.h"
#include "SetNodeNameCommand.h"
#include "SetTrackKeyframeCommand.h"
#include "CreateAnimationCommand.h"
#include "AddAnimationTrackCommand.h"
#include "Timeline.h"
#include "UIHelpers.h"
#include "PaintTerrainInteractionMode.h"
#include "SelectInteractionMode.h"
#include "ScaleInteractionMode.h"
#include "MoveInteractionMode.h"
#include "MoveOriginInteractionMode.h"
#include "RotateInteractionMode.h"
#include "WorldOutliner.h"

void Editor::QuestEditor()
{
	ImGui::Begin("Quest Editor");
	ImGui::End();
}

void Editor::DialogEditor()
{
	ImGui::Begin("Dialog Editor");
	ImGui::End();
}

void Editor::CreateInteractionModes()
{
	mInteractionModes.emplace_back(make_unique<SelectInteractionMode>(this));
	mInteractionModes.emplace_back(make_unique<MoveInteractionMode>(this));
	mInteractionModes.emplace_back(make_unique<ScaleInteractionMode>(this));
	mInteractionModes.emplace_back(make_unique<MoveOriginInteractionMode>(this));
	mInteractionModes.emplace_back(make_unique<RotateInteractionMode>(this));
	mInteractionModes.emplace_back(make_unique<PaintTerrainInteractionMode>(this));
}

void Editor::SceneEditor()
{
	ImGui::Begin("Scene Editor");

	auto renderer = game->GetRenderer();
	renderer->FrameBufferClearColor = { 220, 220, 220 };
	auto &view = mEditorScene->GetView();
	view.setSize(mEditorScene->GetFrameSize());
	view.zoom(mZoom);
	
	for (const shared_ptr<Sprite>& sprite : mEditorScene->GetSpriteList())
	{
		if (mUseLights)
		{
			sprite->ResetRenderFlags(RF_NO_LIGHTING);
			mEditorScene->SetLightingEnabled(true);
		}
		else
		{
			sprite->SetRenderFlags(RF_NO_LIGHTING);
		}
	}

	MainMenuBar();
	HandleShortcuts();

	ImGui::Columns(2);
	ImGui::Separator();

	ImGui::Columns();

	if (ImGui::TreeNode("Current Scene"))
	{
		ImGui::Columns(2);
		ImGui::Separator();

		// Scene
		{
			ImGui::Text("Scene");
			ImGui::NextColumn();

			if (ImGui::Button("Clear", { ImGui::GetColumnWidth(), 0 }))
			{
				mEditorScene->Clear();
			}
		}

		ImGui::NextColumn();
		ImGui::Separator();

		// Create
		{
			ImGui::Text("Create");
			ImGui::NextColumn();

			if (ImGui::Button("Empty Node", { ImGui::GetColumnWidth(), 0 }))
			{
				CreateNodeOfType(NodeType::Node);
			}
			if (ImGui::Button("Sprite", { ImGui::GetColumnWidth(), 0 }))
			{
				CreateNodeOfType(NodeType::Sprite);
			}
			if (ImGui::Button("Particle System", { ImGui::GetColumnWidth(), 0 }))
			{
				CreateNodeOfType(NodeType::ParticleSystem);
			}
			if (ImGui::Button("Light", { ImGui::GetColumnWidth(), 0 }))
			{
				CreateNodeOfType(NodeType::Light);
			}
			if (ImGui::Button("Text", { ImGui::GetColumnWidth(), 0 }))
			{
				CreateNodeOfType(NodeType::Text);
			}
		}

		// Selection
		if (mSelectedNode)
		{
			ImGui::NextColumn();
			ImGui::Separator();

			ImGui::Text("Selection");
			ImGui::NextColumn();

			if (ImGui::Button("Make Copy", { ImGui::GetColumnWidth(), 0 }))
			{
				if (auto sprite = dynamic_pointer_cast<Sprite>(mSelectedNode))
				{
					mSelectedNode = sprite->MakeCopy();
				}
				else
				{
					mSelectedNode = mSelectedNode->MakeCopy();
				}
			}
		}

		ImGui::NextColumn();
		ImGui::Separator();

		ui::BoolField("Use Light", mUseLights);

		// Fitting
		{
			ImGui::Text("Fitting");
			ImGui::NextColumn();

			ImGui::PushItemWidth(ImGui::GetColumnWidth());
			static float newSceneSize[2] = { 30, 42 };
			ImGui::DragFloat2("##1", newSceneSize);
			ImGui::PopItemWidth();

			if (ImGui::Button("Scale To Fit", { ImGui::GetColumnWidth(), 0 }))
			{
				mEditorScene->ScaleToFit(newSceneSize[0], newSceneSize[1]);
			}

			if (ImGui::Button("Set New Width", { ImGui::GetColumnWidth(), 0 }))
			{
				mEditorScene->UniformAxisScaleX(newSceneSize[0]);
			}

			if (ImGui::Button("Set New Height", { ImGui::GetColumnWidth(), 0 }))
			{
				mEditorScene->UniformAxisScaleY(newSceneSize[1]);
			}
		}

		ImGui::NextColumn();
		ImGui::Separator();

		// Render order
		{

			auto nodeGetter = [] (void *data, int idx, const char **out_text)
			{
				if (out_text)
				{
					*out_text = ((vector<shared_ptr<SceneNode>> *)data)->at(idx)->GetName().c_str();
				}
				return true;
			};


			ImGui::Text("Render Order");
			ImGui::NextColumn();

			mDrawableNodes.clear();
			mDrawableNodes.insert(mDrawableNodes.begin(), mEditorScene->GetSpriteList().begin(), mEditorScene->GetSpriteList().end());

			static int currentDrawableNode = 0;

			ImGui::PushItemWidth(ImGui::GetColumnWidth());
			ImGui::ListBox("##2", &currentDrawableNode, nodeGetter, &mDrawableNodes, mDrawableNodes.size(), 6);
			ImGui::PopItemWidth();

			if (ImGui::Button("Up", { ImGui::GetColumnWidth(), 0 }))
			{
				if (currentDrawableNode > 0)
				{
					mEditorScene->ChangeRenderOrder(mDrawableNodes[currentDrawableNode], -1);
					--currentDrawableNode;
				}
			}

			if (ImGui::Button("Down", { ImGui::GetColumnWidth(), 0 }))
			{
				if (static_cast<unsigned int>(currentDrawableNode) <
					mDrawableNodes.size() - 1)
				{
					mEditorScene->ChangeRenderOrder(mDrawableNodes[currentDrawableNode], 1);
					++currentDrawableNode;
				}
			}
		}

		ImGui::Columns(1);
		ImGui::Separator();

		// Action History
		if (ImGui::TreeNode("Action History"))
		{
			static int actionNum = 0;			
			auto getter = [] (void *data, int idx, const char **out_text)
			{
				if (out_text)
				{
					*out_text = ((ed::CommandStack::Stack*)data)->at(idx)->GetDefinition().c_str();
				}
				return true;
			};

			float btnWidth = ImGui::GetContentRegionAvailWidth();
			ImGui::PushItemWidth(btnWidth);
			ImGui::BeginGroup();
			ImGui::Text("Command Stack");
			ImGui::ListBox("##0", &actionNum, getter, (ed::CommandStack::Stack*)&mCommandStack.GetCommands(), mCommandStack.GetCommands().size(), 4);
			ImGui::EndGroup();
			ImGui::PopItemWidth();

			if (ImGui::Button("Undo", { btnWidth, 0 }))
			{
				mCommandStack.Undo();
			}
			ImGui::SameLine();
			if (ImGui::Button("Redo", { btnWidth, 0 }))
			{
				mCommandStack.Redo();
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	TerrainOptionsTab();

	if (mActiveMode)
	{
		mActiveMode->DrawSettingsUI();
	}

	ImGui::Separator();
	if (mSelectedNode)
	{
		if (ImGui::TreeNode("NodeOptions", "\"%s\" Node Properties", mSelectedNode->GetName().c_str()))
		{
			if (!mSelectedNode->IsEditingEnabled())
			{
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "Editing is not allowed!");
			}

			ImGui::Columns(2);
			ImGui::Separator();

			// Parent
			{
				ImGui::Text("Parent");
				ImGui::NextColumn();
				if (mSelectedNode->GetParent())
				{
					ImGui::Text("%s", mSelectedNode->GetParent()->GetName().c_str());
				}
				else
				{
					ImGui::Text("None");
				}
			}

			ImGui::NextColumn();
			ImGui::Separator();

			// Type
			{
				ImGui::Text("Type");
				ImGui::NextColumn();
				ImGui::Text("%s", typeid(*mSelectedNode.get()).name());
			}

			ImGui::NextColumn();
			ImGui::Separator();

			// Node name
			{
				static bool nameWasChanged = false;
				static string lastName;
				char buffer[1024];
				strcpy(buffer, mSelectedNode->GetName().c_str());
				ImGui::Text("Name");
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::GetColumnWidth());
				if (ImGui::InputText("##1", buffer, 1024))
				{
					if (mSelectedNode->IsEditingEnabled())
					{
						if (!nameWasChanged)
						{
							lastName = mSelectedNode->GetName();
							nameWasChanged = true;
						}
						mSelectedNode->SetName(buffer);
					}
				}
				ImGui::PopItemWidth();
				if (!ImGui::IsItemActive() && nameWasChanged)
				{
					mCommandStack.Do(make_shared<SetNodeNameCommand>(mSelectedNode, mSelectedNode->GetName(), lastName));
					nameWasChanged = false;
				}
			}

			ImGui::NextColumn();
			ImGui::Separator();

			// Node rotation
			{
				static bool rotationWasChanged = false;
				static float lastRotation = 0.0f;
				float rotation = mSelectedNode->GetLocalRotation();
				ImGui::Text("Local Rotation");
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::GetColumnWidth());
				if (ImGui::DragFloat("##2", &rotation))
				{
					if (mSelectedNode->IsEditingEnabled())
					{
						if (!rotationWasChanged)
						{
							lastRotation = mSelectedNode->GetLocalRotation();
							rotationWasChanged = true;
						}
						mSelectedNode->SetLocalRotation(rotation);
					}
				}
				ImGui::PopItemWidth();
				if (!ImGui::IsItemActive() && rotationWasChanged)
				{
					mCommandStack.Do(make_shared<SetNodeRotationCommand>(mSelectedNode, mSelectedNode->GetLocalRotation(), lastRotation));
					rotationWasChanged = false;
				}
			}

			ImGui::NextColumn();
			ImGui::Separator();

			// Node scale
			{
				static bool scaleWasChanged = false;
				static Vec2f lastScale;
				float localScale[2] = { mSelectedNode->GetLocalScale().x, mSelectedNode->GetLocalScale().y };
				ImGui::Text("Local Scale");
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::GetColumnWidth());
				if (ImGui::DragFloat2("##3", localScale, 0.05f))
				{
					if (mSelectedNode->IsEditingEnabled())
					{
						if (!scaleWasChanged)
						{
							lastScale = mSelectedNode->GetLocalScale();
							scaleWasChanged = true;
						}
						mSelectedNode->SetLocalScale(localScale[0], localScale[1]);
					}
				}
				ImGui::PopItemWidth();
				// commit changes after item becomes inactive (next frame, so control vars are static)
				if (!ImGui::IsItemActive() && scaleWasChanged)
				{
					mCommandStack.Do(make_shared<SetNodeScaleCommand>(mSelectedNode, mSelectedNode->GetLocalScale(), lastScale));
					scaleWasChanged = false;
				}
			}

			ImGui::NextColumn();
			ImGui::Separator();

			// Node origin
			{
				static bool originWasChanged = false;
				static Vec2f lastOrigin;
				float localOrigin[2] = { mSelectedNode->GetLocalOrigin().x, mSelectedNode->GetLocalOrigin().y };
				ImGui::Text("Local Origin");
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::GetColumnWidth());
				if (ImGui::DragFloat2("##4", localOrigin, 0.5))
				{
					if (mSelectedNode->IsEditingEnabled())
					{
						if (!originWasChanged)
						{
							lastOrigin = mSelectedNode->GetLocalOrigin();
							originWasChanged = true;
						}
						mSelectedNode->SetLocalOrigin(localOrigin[0], localOrigin[1]);
					}
				}
				ImGui::PopItemWidth();
				if (!ImGui::IsItemActive() && originWasChanged)
				{
					mCommandStack.Do(make_shared<SetNodeOriginCommand>(mSelectedNode, mSelectedNode->GetLocalOrigin(), lastOrigin));
					originWasChanged = false;
				}
			}

			ImGui::NextColumn();
			ImGui::Separator();

			// Node Position
			{
				static bool positionWasChanged = false;
				static Vec2f lastPosition;
				ImGui::Text("Local Position");
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::GetColumnWidth());
				if (mSelectedNode->GetBody())
				{
					auto &body = mSelectedNode->GetBody();
					float bodyPosition[2] = { body->GetPosition().x, body->GetPosition().y };

					if (ImGui::DragFloat2("##5", bodyPosition, 0.5))
					{
						if (mSelectedNode->IsEditingEnabled())
						{
							if (!positionWasChanged)
							{
								lastPosition = body->GetPosition();
								positionWasChanged = true;
							}
							body->SetPosition(Vec2f(bodyPosition[0], bodyPosition[1]));
						}
					}
				}
				else
				{
					float localPosition[2] = { mSelectedNode->GetLocalPosition().x, mSelectedNode->GetLocalPosition().y };
					if (ImGui::DragFloat2("##5", localPosition, 0.5))
					{
						if (mSelectedNode->IsEditingEnabled())
						{
							if (!positionWasChanged)
							{
								lastPosition = mSelectedNode->GetLocalPosition();
								positionWasChanged = true;
							}
							mSelectedNode->SetLocalPosition(localPosition[0], localPosition[1]);
						}
					}
				}
				ImGui::PopItemWidth();
				if (!ImGui::IsItemActive() && positionWasChanged)
				{
					mCommandStack.Do(make_shared<SetNodePositionCommand>(mSelectedNode, mSelectedNode->GetLocalPosition(), lastPosition));
					positionWasChanged = false;
				}
			}

			// Sprite properties
			{
				if (auto sprite = dynamic_pointer_cast<Sprite>(mSelectedNode))
				{
					ImGui::NextColumn();
					ImGui::Separator();

					// Size
					{
						static bool sizeWasChanged = false;
						static Vec2f lastSize;
						float size[2] = { sprite->GetSize().x, sprite->GetSize().y };
						ImGui::Text("Size");
						ImGui::NextColumn();
						ImGui::PushItemWidth(ImGui::GetColumnWidth());
						if (ImGui::DragFloat2("##6", size, 0.5))
						{
							if (mSelectedNode->IsEditingEnabled())
							{
								if (!sizeWasChanged)
								{
									lastSize = mSelectedNode->GetSize();
									sizeWasChanged = true;
								}
								sprite->SetSize(size[0], size[1]);
							}
						}
						ImGui::PopItemWidth();
						if (!ImGui::IsItemActive() && sizeWasChanged)
						{
							mCommandStack.Do(make_shared<SetSpriteSizeCommand>(sprite, sprite->GetSize(), lastSize));
							sizeWasChanged = false;
						}
					}

					ImGui::NextColumn();
					ImGui::Separator();

					// texture rect
					{
						static bool texRectWasChanged = false;
						static sf::FloatRect lastTexRect;
						sf::FloatRect r = sprite->GetTextureRectangle();
						ImGui::Text("Texture Rect");
						ImGui::NextColumn();
						ImGui::PushItemWidth(ImGui::GetColumnWidth());
						if (ImGui::DragFloat4("##7", (float *)&r, 0.0025f))
						{
							if (sprite->IsEditingEnabled())
							{
								if (!texRectWasChanged)
								{
									lastTexRect = sprite->GetTextureRectangle();
									texRectWasChanged = true;
								}
								sprite->SetTextureRectangle(r);
							}
						}
						ImGui::PopItemWidth();
						if (!ImGui::IsItemActive() && texRectWasChanged)
						{
							mCommandStack.Do(make_shared<SetSpriteTextureRectangleCommand>(sprite, sprite->GetTextureRectangle(), lastTexRect));
							texRectWasChanged = false;
						}
					}

					ImGui::NextColumn();
					ImGui::Separator();

					// Color
					{
						static bool colorWasChanged = false;
						static sf::Color lastColor;
						auto clr = sprite->GetColor();

						float color[4] = {
							clr.r / 255.0f,
							clr.g / 255.0f,
							clr.b / 255.0f,
							clr.a / 255.0f
						};

						ImGui::Text("Color");
						ImGui::NextColumn();
						ImGui::PushItemWidth(ImGui::GetColumnWidth());
						if (ImGui::ColorEdit4("##8", color))
						{
							if (mSelectedNode->IsEditingEnabled())
							{
								if (!colorWasChanged)
								{
									lastColor = sprite->GetColor();
									colorWasChanged = true;
								}


								sprite->SetColor(
									sf::Color(static_cast<uint8_t>(color[0] * 255),
										static_cast<uint8_t>(color[1] * 255),
										static_cast<uint8_t>(color[2] * 255),
										static_cast<uint8_t>(color[3] * 255)));
							}
						}
						ImGui::PopItemWidth();
						if (!ImGui::IsItemActive() && colorWasChanged)
						{
							mCommandStack.Do(make_shared<SetSpriteColorCommand>(sprite, sprite->GetColor(), lastColor));
							colorWasChanged = false;
						}
					}

					ImGui::NextColumn();
					ImGui::Separator();

					ImGui::Text("Applied Texture");
					ImGui::NextColumn();
					if (auto texture = sprite->GetTexture())
					{
						ImGui::Text("%s", texture->GetSource().c_str());
					}
					else
					{
						ImGui::Text("None");
					}

					ImGui::NextColumn();
					ImGui::Separator();
					// Textures
					{
						ScanTexturesFolder();
						auto getter = [] (void *data, int idx, const char **out_text)
						{
							if (out_text)
							{
								*out_text = ((vector<string> *)data)->at(idx).c_str();
							}
							return true;
						};
						static int currentTexture = 0;

						ImGui::Text("Textures");
						ImGui::NextColumn();
						ImGui::PushItemWidth(ImGui::GetColumnWidth());
						if (ImGui::ListBox("##9", &currentTexture, getter, &mTextureList, mTextureList.size()))
						{
							mCommandStack.Do(make_shared<SetSpriteTextureCommand>(sprite, renderer->RequestTexture(mTextureList[currentTexture]), sprite->GetTexture()));
						}
						ImGui::PopItemWidth();
					}

					ImGui::NextColumn();
					ImGui::Separator();

					ImGui::Text("Set Origin In");
					ImGui::NextColumn();

					if (ImGui::Button("Center", { ImGui::GetColumnWidth(), 0 }))
					{
						mCommandStack.Do(make_shared<SetNodeOriginCommand>(mSelectedNode, sprite->GetSize() / 2.0f, sprite->GetLocalOrigin()));
					}

					if (ImGui::Button("Center X", { ImGui::GetColumnWidth(), 0 }))
					{
						Vec2f newOrigin = { sprite->GetSize().x / 2.0f, sprite->GetLocalOrigin().y };
						mCommandStack.Do(make_shared<SetNodeOriginCommand>(mSelectedNode, newOrigin, sprite->GetLocalOrigin()));
					}

					if (ImGui::Button("Center Y", { ImGui::GetColumnWidth(), 0 }))
					{
						Vec2f newOrigin = { sprite->GetLocalOrigin().x, sprite->GetSize().y / 2.0f };
						mCommandStack.Do(make_shared<SetNodeOriginCommand>(mSelectedNode, newOrigin, sprite->GetLocalOrigin()));
					}
				}
			}

			// Particle System
			{
				if (auto particleSystem = dynamic_pointer_cast<ParticleSystem>(mSelectedNode))
				{
					ImGui::Separator();
					ImGui::NextColumn();

					// Particle count
					{
						ImGui::Text("Particle Count");
						ImGui::NextColumn();

						int particleCount = particleSystem->GetParticleCount();
						if (ImGui::DragInt("##100", &particleCount, 1, 1))
						{
							particleSystem->CreateParticles(particleCount);
						}
					}

					// Min speed
					{
						ImGui::Text("Min Speed");
						ImGui::NextColumn();

						Vec2f speedMin = particleSystem->GetMinSpeed();
						if (ImGui::DragFloat2("##101", &speedMin.x, 0.01f))
						{
							particleSystem->SetMinSpeed(speedMin);
						}
					}

					// Max speed
					{
						ImGui::Text("Max Speed");
						ImGui::NextColumn();

						Vec2f speedMax = particleSystem->GetMaxSpeed();
						if (ImGui::DragFloat2("##102", &speedMax.x, 0.01f))
						{
							particleSystem->SetMaxSpeed(speedMax);
						}
					}

					// Particle size
					{
						ImGui::Text("Particle Size");
						ImGui::NextColumn();

						float particleSize = particleSystem->GetParticleSize();
						if (ImGui::DragFloat("##103", &particleSize, 0.01f))
						{
							particleSystem->SetParticleSize(particleSize);
						}
					}
				}
			}

			// Light properties
			{
				if (auto light = dynamic_pointer_cast<Light>(mSelectedNode))
				{
					ImGui::Separator();
					ImGui::NextColumn();

					float radius = light->GetRadius();
					if (ImGui::DragFloat("Radius", &radius, 0.1f, 0, 2000))
					{
						light->SetRadius(radius);
					}
					auto byteColor = light->GetColor();
					float normalizedColor[4] = { byteColor.r / 255.0f, byteColor.g / 255.0f, byteColor.b / 255.0f, byteColor.a / 255.0f };
					if (ImGui::ColorEdit4("Color", normalizedColor))
					{
						light->SetColor(sf::Color(static_cast<uint8_t>(normalizedColor[0] * 255),
							static_cast<uint8_t>(normalizedColor[1] * 255),
							static_cast<uint8_t>(normalizedColor[2] * 255),
							static_cast<uint8_t>(normalizedColor[3] * 255)));
					}
				}
			}

			ImGui::Separator();
			ImGui::NextColumn();

			ImGui::Text("Animation Tracks");

			ImGui::NextColumn();
			if (ImGui::Button("Create New Track", { ImGui::GetColumnWidth(), 0 }))
			{
				auto prevTrack = mActiveAnimation->FindAssociatedTrack(mSelectedNode);
				if (prevTrack)
				{
					mActiveAnimation->RemoveTrack(prevTrack);
				}
				mActiveAnimation->AddTrack()->SetAnimatingNode(mSelectedNode);
			}

			ImGui::TreePop();
		}
	}

	ImGui::Separator();
	ImGui::Columns(1);

	SaveLoadSceneTab();

	ImGui::End();

	ImGui::Begin("Animation", 0);
	{
		static int currentAnimationIndex = 0;
		auto getter = [] (void *data, int idx, const char **out_text)
		{
			if (out_text)
			{
				*out_text = ((vector<shared_ptr<Animation>> *)data)->at(idx)->GetName().c_str();
			}
			return true;
		};

		ImGui::Columns(2);
		ImGui::Separator();

		// Control
		{
			ImGui::Text("Control");
			ImGui::NextColumn();

			if (ImGui::Button("New Animation", { ImGui::GetColumnWidth(), 0 }))
			{
				mCommandStack.Do(make_shared<CreateAnimationCommand>(mEditorScene));
				mActiveAnimation = mEditorScene->GetAnimationList().back();
			}

			if (ImGui::Button("Remove Animation", { ImGui::GetColumnWidth(), 0 }))
			{
				auto &list = mEditorScene->GetAnimationList();
				if (list.size())
				{
					list.erase(list.begin() + currentAnimationIndex);
					if (list.size())
					{
						mActiveAnimation = list.front();
						currentAnimationIndex = 0;
					}
				}
			}
		}

		ImGui::NextColumn();
		ImGui::Separator();

		// Animation list
		{
			ImGui::Text("Animation List");
			ImGui::NextColumn();

			ImGui::PushItemWidth(ImGui::GetColumnWidth());
			if (ImGui::ListBox("##1", &currentAnimationIndex, getter, &mEditorScene->GetAnimationList(), mEditorScene->GetAnimationList().size(), 3))
			{
				if (mActiveAnimation)
				{
					mActiveAnimation->SetEnabled(false);
				}
				mActiveAnimation = mEditorScene->GetAnimationList().at(currentAnimationIndex);
			}
			ImGui::PopItemWidth();
		}

		ImGui::NextColumn();
		ImGui::Separator();

		if (mActiveAnimation)
		{
			// Name
			{
				ImGui::Text("Name");
				ImGui::NextColumn();

				char buffer[512];
				strcpy(buffer, mActiveAnimation->GetName().c_str());

				ImGui::PushItemWidth(ImGui::GetColumnWidth());
				if (ImGui::InputText("Name", buffer, 512))
				{
					mActiveAnimation->SetName(buffer);
				}
				ImGui::PopItemWidth();
			}

			ImGui::NextColumn();
			ImGui::Separator();

			// Key frame control
			{
				ImGui::Text("Keyframe Control");
				ImGui::NextColumn();

				if (ImGui::Button("Set Keyframe", { ImGui::GetColumnWidth(), 0 }))
				{
					if (auto track = mActiveAnimation->FindAssociatedTrack(mSelectedNode))
					{
						auto command = make_shared<SetTrackKeyframeCommand>(
							track,
							mActiveAnimation->GetTimePosition(),
							mSelectedNode->GetLocalPosition(),
							mSelectedNode->GetLocalScale(),
							mSelectedNode->GetLocalRotation());

						mCommandStack.Do(command);
					}
				}
			}
		}

	}
	ImGui::End();

	SceneRenderWindow();

	if (mActiveAnimation)
	{
		mTimeline->DrawAndUpdate();
	}

	mWorldOutliner->DrawAndUpdate();
}

void Editor::ScanTexturesFolder()
{
	// update texture list every 300 ms
	static chrono::steady_clock::time_point lastTime;
	auto now = chrono::steady_clock::now();
	if (chrono::duration_cast<chrono::milliseconds>(now - lastTime).count() > 300)
	{
		mTextureList.clear();
		std::string path = "data/textures/anim/";
		for (auto &p : filesystem::recursive_directory_iterator(path))
		{
			if (p.path().has_extension())
			{
				auto ext = p.path().extension().string();
				if (ext == ".png" || ext == ".jpg" || ext == ".bmp" || ext == ".tga")
				{
					mTextureList.push_back(p.path().string());
				}
			}
		}
		lastTime = now;
	}
}

void Editor::ScanPrefabsFolder()
{
	// update prefab list every 300 ms
	static chrono::steady_clock::time_point lastTime;
	auto now = chrono::steady_clock::now();
	if (chrono::duration_cast<chrono::milliseconds>(now - lastTime).count() > 300)
	{
		mPrefabFileList.clear();
		std::string path = "data/prefabs/";
		for (auto &p : filesystem::recursive_directory_iterator(path))
		{
			auto otherPath = p.path();
			if (otherPath.has_extension())
			{
				auto ext = otherPath.extension().string();
				if (ext == ".prefab")
				{
					mPrefabFileList.push_back(otherPath.string());
				}
			}
		}
		lastTime = now;
	}
}

void Editor::RemoveKeyframeAtTime(vector<Keyframe> &keyframeList, float time)
{
	for (auto iter = keyframeList.begin(); iter != keyframeList.end();)
	{
		if (Math::RoughlyEqual(iter->GetTime(), time))
		{
			iter = keyframeList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

bool Editor::IsShortcutPressed(const vector<sf::Keyboard::Key> &keys)
{
	if (keys.empty())
	{
		return false;
	}

	auto input = game->GetInput();
	size_t pressedCount = 0;
	for (size_t i = 0; i < keys.size() - 1; ++i)
	{
		if (input->IsKeyPressed(keys[i]))
		{
			++pressedCount;
		}
	}
	if (input->IsKeyHit(keys.back()))
	{
		++pressedCount;
	}
	return pressedCount == keys.size();
}

void Editor::OnSavePrefabClicked(const string & fileName)
{
	bool needOverwrite = false;
	for (auto &prefabName : mPrefabFileList)
	{
		if (prefabName == fileName)
		{
			needOverwrite = true;
		}
	}
	if (needOverwrite)
	{
		ImGui::OpenPopup(OverwriteFilePopupName);
	}
	else
	{
		game->SavePrefab(fileName, mEditorScene);
	}
}

void Editor::OnLoadPrefabClicked(const string & fileName)
{
	// clean up
	mSelectedNode.reset();
	game->RemoveScene(mEditorScene);

	mEditorScene = game->LoadPrefab(fileName);
	mEditorScene->SetRenderingMode(SceneRenderingMode::ToTexture);
	mEditorScene->GetView().setCenter({ 0, 0 });

	mCommandStack.Clear();

	if (mEditorScene->GetAnimationList().size())
	{
		mActiveAnimation = mEditorScene->GetAnimationList().front();
	}
}

void Editor::OverwritePrefabPopup(const string & fileName)
{
	if (ImGui::BeginPopup(OverwriteFilePopupName))
	{
		ImGui::Text(("Overwrite '" + string(fileName) + "' prefab?").c_str());
		if (ImGui::Button("Yes"))
		{
			game->SavePrefab(fileName, mEditorScene);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("No"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void Editor::HandleShortcuts()
{
	if (!ImGui::IsAnyItemActive())
	{
		if (IsShortcutPressed({ sf::Keyboard::LControl, sf::Keyboard::Z }))
		{
			mCommandStack.Undo();
		}
		else if (IsShortcutPressed({ sf::Keyboard::LControl, sf::Keyboard::Y }))
		{
			mCommandStack.Redo();
		}
		else if (IsShortcutPressed({ sf::Keyboard::K }))
		{
			if (auto track = mActiveAnimation->FindAssociatedTrack(mSelectedNode))
			{
				auto cmd = make_shared<SetTrackKeyframeCommand>(
					track,
					mActiveAnimation->GetTimePosition(),
					mSelectedNode->GetLocalPosition(),
					mSelectedNode->GetLocalScale(),
					mSelectedNode->GetLocalRotation());

				mCommandStack.Do(cmd);
			}
		}
		else
		{
			for (const unique_ptr<InteractionMode>& mode : mInteractionModes)
			{
				if (IsShortcutPressed(mode->GetShortcut()))
				{
					mActiveMode = mode.get();

					break;
				}
			}
		}
	}
}

void Editor::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit", "ESCAPE", false))
			{

			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z", false, mCommandStack.CanUndo()))
			{
				mCommandStack.Undo();
			}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, mCommandStack.CanRedo()))
			{
				mCommandStack.Redo();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Editor::DrawCoordinateGrid(const sf::View & view)
{
	auto drawList = ImGui::GetWindowDrawList();

	auto viewport = game->GetRenderer()->GetWindow().getViewport(view);

	auto color = ImColor(120, 120, 120, 40);
	const float maxLength = 99999.0f;
	const float meshStep = 30.0f;

	// draw origin lines
	auto origin = Math::MapWorldToScreen({ 0, 0 }, view.getTransform(), viewport);
	drawList->AddLine(origin, { maxLength, origin.y }, color, 2.0f);
	drawList->AddLine(origin, { -maxLength, origin.y }, color, 2.0f);
	drawList->AddLine(origin, { origin.x, -maxLength }, color, 2.0f);
	drawList->AddLine(origin, { origin.x, maxLength }, color, 2.0f);

	// draw minor lines
	auto contentRegion = ImGui::GetWindowContentRegionMax();
	for (float y = 0; y < contentRegion.y; y += meshStep)
	{
		auto begin = Math::MapWorldToScreen({ -contentRegion.x, y }, view.getTransform(), viewport);
		auto end = Math::MapWorldToScreen({ contentRegion.x, y }, view.getTransform(), viewport);
		drawList->AddLine(begin, end, color);
	}
	for (float y = 0; y > -contentRegion.y; y -= meshStep)
	{
		auto begin = Math::MapWorldToScreen({ -contentRegion.x, y }, view.getTransform(), viewport);
		auto end = Math::MapWorldToScreen({ contentRegion.x, y }, view.getTransform(), viewport);
		drawList->AddLine(begin, end, color);
	}
	for (float x = 0; x < contentRegion.x; x += meshStep)
	{
		auto begin = Math::MapWorldToScreen({ x, -contentRegion.y }, view.getTransform(), viewport);
		auto end = Math::MapWorldToScreen({ x, contentRegion.y }, view.getTransform(), viewport);
		drawList->AddLine(begin, end, color);
	}
	for (float x = 0; x > -contentRegion.x; x -= meshStep)
	{
		auto begin = Math::MapWorldToScreen({ x, -contentRegion.y }, view.getTransform(), viewport);
		auto end = Math::MapWorldToScreen({ x, contentRegion.y }, view.getTransform(), viewport);
		drawList->AddLine(begin, end, color);
	}

}

Vec2f Editor::ScreenToWorld(const Vec2f & screen) const
{
	const sf::View& view = mEditorScene->GetView();
	return game->GetRenderer()->GetWindow().mapPixelToCoords(Vec2i(screen), view);
}

void Editor::DrawSelectedNodeGizmo(const sf::View & view)
{
	auto viewport = game->GetRenderer()->GetWindow().getViewport(view);
	auto worldTransform = mSelectedNode->GetGlobalTransform();
	auto drawList = ImGui::GetWindowDrawList();

	// Gizmo
	Vec2f screenOrigin = Math::MapWorldToScreen(mSelectedNode->GetLocalOrigin(), worldTransform, view.getTransform(), viewport);
	Vec2f up = Math::MapWorldToScreen(mSelectedNode->GetLocalOrigin() + Vec2f(0, 40), worldTransform, view.getTransform(), viewport);
	Vec2f right = Math::MapWorldToScreen(mSelectedNode->GetLocalOrigin() + Vec2f(40, 0), worldTransform, view.getTransform(), viewport);
	drawList->AddLine(screenOrigin, up, ImColor(0, 200, 0));
	drawList->AddRectFilled({ up.x - 3, up.y - 3 }, { up.x + 3, up.y + 3 }, ImColor(0, 200, 0));
	drawList->AddLine(screenOrigin, right, ImColor(0, 0, 200));
	drawList->AddRectFilled({ right.x - 3, right.y - 3 }, { right.x + 3, right.y + 3 }, ImColor(0, 0, 200));
}

void Editor::DrawSelectedNodeBounds(const sf::View & view, const ImColor & color)
{
	auto viewport = game->GetRenderer()->GetWindow().getViewport(view);
	auto worldTransform = mSelectedNode->GetGlobalTransform();
	auto drawList = ImGui::GetWindowDrawList();

	Math::RotatedRectangle bounds = {
		mSelectedNode->GetSize(),
		worldTransform,
		view.getTransform(),
		viewport
	};

	drawList->AddLine(bounds.points[0], bounds.points[1], color);
	drawList->AddLine(bounds.points[1], bounds.points[2], color);
	drawList->AddLine(bounds.points[2], bounds.points[3], color);
	drawList->AddLine(bounds.points[3], bounds.points[0], color);
}

void Editor::FixKeyframesForSelectedNode(const shared_ptr<SceneNode> & parent)
{
	for (auto &anim : mEditorScene->GetAnimationList())
	{
		// we have to correct keyframes when attaching node
		if (auto track = anim->FindAssociatedTrack(mSelectedNode))
		{
			for (auto &keyframe : track->GetKeyframeList())
			{
				// compose keyframe transform
				auto keyframeTransform = Math::ComposeTransform(
					keyframe.GetPosition(), { 0, 0 }, keyframe.GetScale(),
					keyframe.GetRotation());

				// compute relative keyframe transform to new parent
				auto relativeTransform = parent->GetGlobalTransform().getInverse() * keyframeTransform;

				// decompose it
				float rot;
				Vec2f pos, scl;
				Math::DecomposeTransform(relativeTransform, &pos, &scl, &rot);

				// and write back to keyframe
				keyframe.SetPosition(pos);
				keyframe.SetScale(scl);
				keyframe.SetRotation(rot);
			}
		}
	}
}

void Editor::TerrainOptionsTab()
{
	shared_ptr<Terrain> terrain = mEditorScene->GetTerrain();

	if (ImGui::TreeNode("Terrain Options"))
	{
		static Vec2i terrainSize = { 4, 4 };

		if (terrain)
		{
			terrainSize = terrain->GetSize();
		}

		ImGui::Columns(2);
		ImGui::Separator();

		ui::IntField("Width", terrainSize.x, 1, 4, 40);
		ui::IntField("Height", terrainSize.y, 1, 4, 40);

		// Memory
		{
			const int memoryMb = (terrainSize.x * terrainSize.y * (int)Cell::Size * (int)Cell::Size * 4) / (1024 * 1024);
			ui::TextLabel("Memory Consumption, Mb", to_string(memoryMb));
		}

		ImGui::Columns(1);

		// Create/Delete
		{
			const float size = ImGui::GetContentRegionAvailWidth() * 0.5f;

			if (ImGui::Button("Create", { size, 0 }))
			{
				terrain = mEditorScene->CreateTerrain(terrainSize, 64);
			}

			ImGui::SameLine();

			if (ImGui::Button("Delete", { size, 0 }))
			{
				mEditorScene->DeleteTerrain();
			}
		}

		if (terrain)
		{
			terrain->SetFogOfWarEnabled(false);
		}

		ImGui::TreePop();
	}
}

void Editor::DeleteTerrainPopupQuestion()
{
	if (ImGui::BeginPopup("DeleteTerrainPopup"))
	{
		ImGui::EndPopup();
	}
}

void Editor::SceneRenderWindow()
{
	auto input = game->GetInput();
	auto &view = mEditorScene->GetView();

	ImGui::Begin("Previewer");

	ImGui::BeginChild("SideBar", { 30, 0 });
	{
		for (const unique_ptr<InteractionMode>& mode : mInteractionModes)
		{
			bool highlight = mActiveMode == mode.get();

			if (highlight)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, (ImU32)ImColor(150, 150, 150));
			}
			if (mode->TrayButton({ 30, 30 }))
			{
				mActiveMode = mode.get();
			}
			if (highlight)
			{
				ImGui::PopStyleColor();
			}
		}
		ImGui::EndChild();
	}

	ImGui::SameLine();

	ImGui::BeginChild("RenderFrame", { 0, 0 }, true, ImGuiWindowFlags_NoMove);
	{
		if (ImGui::BeginPopupContextWindow("PreviewerContextMenu"))
		{
			ImGui::Text("Create:");
			for (unsigned int i = 0; i < NodeType::EntryCount; ++i)
			{
				if (ImGui::Button(NodeType::Str[i], { 140, 0 }))
				{
					const shared_ptr<SceneNode> node = CreateNodeOfType(static_cast<NodeType::Type>(i));

					node->SetLocalPosition(ScreenToWorld(ImGui::GetWindowPos()));

					ImGui::CloseCurrentPopup();
				}
			}

			if (mSelectedNode)
			{
				ImGui::Separator();
				ImGui::Text(mSelectedNode->GetName().c_str());

				if (ImGui::Button("Delete", { 140, 0 }))
				{
					mSelectedNode->RemoveFromScene();

					mSelectedNode.reset();

					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
		{
			mZoom = Math::Clamp(mZoom + 0.075f * input->GetMouseWheelSpeed(), 0.1f, 5.0f);
			if (input->IsMouseButtonPressed(sf::Mouse::Middle))
			{
				view.move(-input->GetMouseSpeed() * mZoom);
			}

			if (mActiveMode)
			{
				mActiveMode->Update();
			}
		}

		const ImVec2 frameSize = ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin();

		const sf::Vector2u wndSize = game->GetRenderer()->GetWindow().getSize();

		const sf::FloatRect viewport = {
			(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x) / wndSize.x,
			(ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y) / wndSize.y,
			frameSize.x / wndSize.x,
			frameSize.y / wndSize.y,
		};
		view.setViewport(viewport);

		mEditorScene->SetFrameSize(frameSize);

		// Draw rendered scene
		ImGui::Image((ImTextureID)mEditorScene->GetFrame()->GetOpenGLTexture(), frameSize, { -1, 1 }, { 0, 0 });

		DrawCoordinateGrid(view);

		if (mActiveMode)
		{
			mActiveMode->DrawPreviewerOverlay();
		}

		// Draw selected node bounds and gizmo
		if (mSelectedNode)
		{
			ImColor selectedColor = ImColor(1.0f, 0.0f, 0.0f);
			if (input->IsMouseButtonPressed(sf::Mouse::Left))
			{
				selectedColor = ImColor(0.0f, 1.0f, 0.0f);
			}

			DrawSelectedNodeBounds(view, selectedColor);
			DrawSelectedNodeGizmo(view);
		}

		ImGui::EndChild();
	}

	ImGui::End();
}

void Editor::SaveLoadSceneTab()
{
	if (ImGui::TreeNode("Save/Load Prefab"))
	{
		ImGui::Columns(2);
		ImGui::Separator();

		ScanPrefabsFolder();

		static char fileName[FILENAME_MAX] = { 0 };
		static int currentPrefab = 0;
		ImGui::Text("File Name");
		ImGui::NextColumn();
		ImGui::PushItemWidth(ImGui::GetColumnWidth());
		ImGui::InputText("##10", fileName, FILENAME_MAX);
		ImGui::PopItemWidth();

		if (ImGui::Button("Load", { ImGui::GetColumnWidth(), 0 }))
		{
			OnLoadPrefabClicked(fileName);
		}

		ImGui::NextColumn();
		ImGui::Separator();

		ImGui::Text("Prefabs");
		ImGui::NextColumn();
		ImGui::PushItemWidth(ImGui::GetColumnWidth());
		auto getter = [] (void *data, int idx, const char **out_text)
		{
			if (out_text)
			{
				*out_text = ((vector<string> *)data)->at(idx).c_str();
			}
			return true;
		};
		if (ImGui::ListBox("##11", &currentPrefab, getter, &mPrefabFileList, mPrefabFileList.size()))
		{
			strcpy(fileName, mPrefabFileList[currentPrefab].c_str());
		}
		ImGui::PopItemWidth();

		ImGui::NextColumn();
		ImGui::Separator();

		ImGui::Text("Current Prefab");
		ImGui::NextColumn();

		if (ImGui::Button("Save", { ImGui::GetColumnWidth(), 0 }))
		{
			OnSavePrefabClicked(fileName);
		}

		OverwritePrefabPopup(fileName);

		ImGui::TreePop();
	}
}

bool Editor::IsPointInNodeBounds(const Vec2f & pointScreenCoords, const shared_ptr<SceneNode>& node)
{
	const sf::View& view = mEditorScene->GetView();
	const sf::IntRect viewport = game->GetRenderer()->GetWindow().getViewport(view);

	const Math::RotatedRectangle bounds = {
		node->GetSize(),
		node->GetGlobalTransform(),
		view.getTransform(), viewport
	};

	return bounds.IsPointInside(pointScreenCoords);
}

shared_ptr<SceneNode> Editor::CreateNodeOfType(const NodeType & type)
{
	auto createNodeCommand = make_shared<CreateNodeCommand>(mEditorScene, type);
	mCommandStack.Do(createNodeCommand);
	mSelectedNode = createNodeCommand->GetResult();
	if (mActiveAnimation)
	{
		auto track = mActiveAnimation->AddTrack();
		track->SetAnimatingNode(mSelectedNode);
	}
	return createNodeCommand->GetResult();
}

Editor::Editor()
{
	mTimeline = make_unique<ed::Timeline>(this);
	mWorldOutliner = make_unique<ed::WorldOutliner>(this);
	Log() << "Editor successfully initialized!" << LineEnd;
}

Editor::~Editor()
{
	DBG_ECHO();
}

void Editor::SetActive(bool active)
{
	mActive = active;
	if (active)
	{
		mOldFrameBufferClearColor = game->GetRenderer()->FrameBufferClearColor;

		mEditorScene = game->CreateScene();
		mEditorScene->SetRenderingMode(SceneRenderingMode::ToTexture);
		// reset timeline
		mActiveAnimation = make_shared<Animation>();
		mEditorScene->AddAnimation(mActiveAnimation);

		// set view in center
		auto &view = mEditorScene->GetView();
		view.setCenter({ 0, 0 });
		view.setSize({ 1280, 720 });

		CreateInteractionModes();
	}
	else
	{
		game->RemoveScene(mEditorScene);
		mEditorScene.reset();
		mInteractionModes.clear();
		mCommandStack.Clear();
		mActiveMode = nullptr;
		game->GetRenderer()->FrameBufferClearColor = mOldFrameBufferClearColor;
	}
}

bool Editor::IsActive() const
{
	return mActive;
}

void Editor::DrawAndUpdate()
{
	#ifndef DEMO_VERSION
	if (!mActive)
	{
		return;
	}
	mItemDefinitionEditor.DrawAndUpdate();
	mDialogEditor.DrawAndUpdate();
	mTranslatedStringEditor.DrawAndUpdate();
	SceneEditor();
	#endif
}

void Editor::AddBrushTexture(const string &path)
{
	UNUSED(path);
}

ed::CommandStack & Editor::GetCommandStack()
{
	return mCommandStack;
}
