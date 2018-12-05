##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=DwarfDig
ConfigurationName      :=Release
WorkspacePath          :=/home/dmitry/TheDwarfAdventure/codeliteproj
ProjectPath            :=/home/dmitry/TheDwarfAdventure/codeliteproj
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=dmitry
Date                   :=05/12/18
CodeLitePath           :=/home/dmitry/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=../bin/DwarfDig
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="DwarfDig.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            := -pg  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)./ $(IncludeSwitch)../src/ $(IncludeSwitch)../src/imgui/ $(IncludeSwitch)../external/SFML/include $(IncludeSwitch)../external/ 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)sfml-graphics $(LibrarySwitch)sfml-window $(LibrarySwitch)sfml-system $(LibrarySwitch)sfml-audio $(LibrarySwitch)GL $(LibrarySwitch)GLEW $(LibrarySwitch)pthread $(LibrarySwitch)stdc++fs $(LibrarySwitch)stdc++ 
ArLibs                 :=  "sfml-graphics" "sfml-window" "sfml-system" "sfml-audio" "GL" "GLEW" "pthread" "stdc++fs" "stdc++" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -pg -O2 -Wall -std=c++17 -Wno-format-security -Wno-class-memaccess -Wno-misleading-indentation  $(Preprocessors)
CFLAGS   :=  -pg -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_src_Table.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Game.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_UIHelpers.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Projectile.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_CreateNodeCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Menu.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_RoomGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Dialog.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Harpoon.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_BookReader.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_Player.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Router.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Smoke.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Keyframe.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Sound.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_SpinLock.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_CommandStack.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_TonnelVertex.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Config.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Animation.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_Hook.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_TextureStack.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Checkbox.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Event.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_RevealGun.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_BossAreaGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_WaterBlob.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_FlyingIslandsGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_JsonConverters.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Theme.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_Hitbox.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Text.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Frame.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_HUD.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_LineSet.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Stats.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_BossCrystal.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_SetNodeScaleCommand.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/up_src_FogGate.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_SetNodeNameCommand.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_Remains.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Renderer.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_EditorCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Precompiled.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_MoveInteractionMode.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_VisualEffect.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Level.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ArbitraryRectangle.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_BarterWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_AddAnimationTrackCommand.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_SetSpriteTextureCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_AttachCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_IK.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Selection.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Node.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Dwarf.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_GrenadeLauncher.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Debris.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_RailGun.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Scene.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_Terrain.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_BehaviourTree.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_AStar.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Reward.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Texture.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_QuestBase.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Button.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Editor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_InventoryGUI.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_MaskOperation.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_Console.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_MapWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_SetSelectedCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Bench.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Exception.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Cell.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Light.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Cloud.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_PolyLine.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_NameDatabase.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_SetNodePositionCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ParticleSystem.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_NPC.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_LiquidParticle.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_HouseGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Layout.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_MonsterGenerator.cpp$(ObjectSuffix) 

Objects2=$(IntermediateDirectory)/up_src_Drill.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_LootList.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Sprite.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_Image.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Island.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_SetNodeOriginCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_GravityGun.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_BossBeast.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Input.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_SetSpriteTextureRectangleCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ItemDefinitionEditor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_BossGolem.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_NaturalCaveGenerator.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_LocationMap.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ChestGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ImageSelector.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_LocationGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Ladder.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Room.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_WorldOutliner.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Chest.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_InteractionMenu.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Slot.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_LocationType.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_FrameAnimation.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_SetSpriteColorCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ViewTriangle.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Item.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_AttributeGrid.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Pickaxe.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Asset.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Timeline.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_SelectInteractionMode.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_Door.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_DebrisAtlas.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Tooltip.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_NumericUpDown.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Passage.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_WorldGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Monster.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_InteractionMode.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_CellPixel.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_AcidGun.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_TranslatedStringEditor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_CaveGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_SetTrackKeyframeCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_SettlementGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_UndergroundSettlementGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_BankGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_PerlinNoise.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Canvas.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_MoveOriginInteractionMode.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_SoilGun.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_CreateSpriteCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_PaintTerrainInteractionMode.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_PlasmaDrill.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Controls.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_LightningGun.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Shield.cpp$(ObjectSuffix) 

Objects3=$(IntermediateDirectory)/up_src_StatWindow.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_DialogEditor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Guid.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Command.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_Widget.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_SetSpriteSizeCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_CreateAnimationCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_RotateInteractionMode.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ModifyTerrainCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Trigger.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_WarehouseGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ForceGun.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_PassageGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Location.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_ResourceDatabase.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_SlideList.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Window.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Label.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_QuadTree.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Weapon.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Inventory.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Damage.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ConstructionGun.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_AnimationTrack.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_Shotgun.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Serializer.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_MotherCrystal.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_TextLine.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_HubGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Encyclopedia.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_RocketLauncher.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Journal.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Workbench.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Bar.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_Collision.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ScaleInteractionMode.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ScrollableArea.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Boss.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_SetNodeRotationCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Torch.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Actor.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Arena.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_Log.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_FogOfWar.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_src_ItemDatabase.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_ListBox.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_imgui_imgui_widgets.cpp$(ObjectSuffix) 

Objects4=$(IntermediateDirectory)/up_src_imgui_imgui.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_imgui_imgui_draw.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_imgui_imgui_demo.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_src_miniz_miniz.c$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) $(Objects2) $(Objects3) $(Objects4) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	@echo $(Objects2) >> $(ObjectsFileList)
	@echo $(Objects3) >> $(ObjectsFileList)
	@echo $(Objects4) >> $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


$(IntermediateDirectory)/.d:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:

# PreCompiled Header
../src/Precompiled.h.gch: ../src/Precompiled.h
	$(CXX) $(SourceSwitch) ../src/Precompiled.h $(PCHCompileFlags) $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/up_src_Table.cpp$(ObjectSuffix): ../src/Table.cpp $(IntermediateDirectory)/up_src_Table.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Table.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Table.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Table.cpp$(DependSuffix): ../src/Table.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Table.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Table.cpp$(DependSuffix) -MM ../src/Table.cpp

$(IntermediateDirectory)/up_src_Table.cpp$(PreprocessSuffix): ../src/Table.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Table.cpp$(PreprocessSuffix) ../src/Table.cpp

$(IntermediateDirectory)/up_src_Game.cpp$(ObjectSuffix): ../src/Game.cpp $(IntermediateDirectory)/up_src_Game.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Game.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Game.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Game.cpp$(DependSuffix): ../src/Game.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Game.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Game.cpp$(DependSuffix) -MM ../src/Game.cpp

$(IntermediateDirectory)/up_src_Game.cpp$(PreprocessSuffix): ../src/Game.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Game.cpp$(PreprocessSuffix) ../src/Game.cpp

$(IntermediateDirectory)/up_src_UIHelpers.cpp$(ObjectSuffix): ../src/UIHelpers.cpp $(IntermediateDirectory)/up_src_UIHelpers.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/UIHelpers.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_UIHelpers.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_UIHelpers.cpp$(DependSuffix): ../src/UIHelpers.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_UIHelpers.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_UIHelpers.cpp$(DependSuffix) -MM ../src/UIHelpers.cpp

$(IntermediateDirectory)/up_src_UIHelpers.cpp$(PreprocessSuffix): ../src/UIHelpers.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_UIHelpers.cpp$(PreprocessSuffix) ../src/UIHelpers.cpp

$(IntermediateDirectory)/up_src_Projectile.cpp$(ObjectSuffix): ../src/Projectile.cpp $(IntermediateDirectory)/up_src_Projectile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Projectile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Projectile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Projectile.cpp$(DependSuffix): ../src/Projectile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Projectile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Projectile.cpp$(DependSuffix) -MM ../src/Projectile.cpp

$(IntermediateDirectory)/up_src_Projectile.cpp$(PreprocessSuffix): ../src/Projectile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Projectile.cpp$(PreprocessSuffix) ../src/Projectile.cpp

$(IntermediateDirectory)/up_src_CreateNodeCommand.cpp$(ObjectSuffix): ../src/CreateNodeCommand.cpp $(IntermediateDirectory)/up_src_CreateNodeCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/CreateNodeCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_CreateNodeCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_CreateNodeCommand.cpp$(DependSuffix): ../src/CreateNodeCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_CreateNodeCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_CreateNodeCommand.cpp$(DependSuffix) -MM ../src/CreateNodeCommand.cpp

$(IntermediateDirectory)/up_src_CreateNodeCommand.cpp$(PreprocessSuffix): ../src/CreateNodeCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_CreateNodeCommand.cpp$(PreprocessSuffix) ../src/CreateNodeCommand.cpp

$(IntermediateDirectory)/up_src_Menu.cpp$(ObjectSuffix): ../src/Menu.cpp $(IntermediateDirectory)/up_src_Menu.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Menu.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Menu.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Menu.cpp$(DependSuffix): ../src/Menu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Menu.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Menu.cpp$(DependSuffix) -MM ../src/Menu.cpp

$(IntermediateDirectory)/up_src_Menu.cpp$(PreprocessSuffix): ../src/Menu.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Menu.cpp$(PreprocessSuffix) ../src/Menu.cpp

$(IntermediateDirectory)/up_src_RoomGenerator.cpp$(ObjectSuffix): ../src/RoomGenerator.cpp $(IntermediateDirectory)/up_src_RoomGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/RoomGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_RoomGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_RoomGenerator.cpp$(DependSuffix): ../src/RoomGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_RoomGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_RoomGenerator.cpp$(DependSuffix) -MM ../src/RoomGenerator.cpp

$(IntermediateDirectory)/up_src_RoomGenerator.cpp$(PreprocessSuffix): ../src/RoomGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_RoomGenerator.cpp$(PreprocessSuffix) ../src/RoomGenerator.cpp

$(IntermediateDirectory)/up_src_Dialog.cpp$(ObjectSuffix): ../src/Dialog.cpp $(IntermediateDirectory)/up_src_Dialog.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Dialog.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Dialog.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Dialog.cpp$(DependSuffix): ../src/Dialog.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Dialog.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Dialog.cpp$(DependSuffix) -MM ../src/Dialog.cpp

$(IntermediateDirectory)/up_src_Dialog.cpp$(PreprocessSuffix): ../src/Dialog.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Dialog.cpp$(PreprocessSuffix) ../src/Dialog.cpp

$(IntermediateDirectory)/up_src_Harpoon.cpp$(ObjectSuffix): ../src/Harpoon.cpp $(IntermediateDirectory)/up_src_Harpoon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Harpoon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Harpoon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Harpoon.cpp$(DependSuffix): ../src/Harpoon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Harpoon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Harpoon.cpp$(DependSuffix) -MM ../src/Harpoon.cpp

$(IntermediateDirectory)/up_src_Harpoon.cpp$(PreprocessSuffix): ../src/Harpoon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Harpoon.cpp$(PreprocessSuffix) ../src/Harpoon.cpp

$(IntermediateDirectory)/up_src_BookReader.cpp$(ObjectSuffix): ../src/BookReader.cpp $(IntermediateDirectory)/up_src_BookReader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/BookReader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_BookReader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_BookReader.cpp$(DependSuffix): ../src/BookReader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_BookReader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_BookReader.cpp$(DependSuffix) -MM ../src/BookReader.cpp

$(IntermediateDirectory)/up_src_BookReader.cpp$(PreprocessSuffix): ../src/BookReader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_BookReader.cpp$(PreprocessSuffix) ../src/BookReader.cpp

$(IntermediateDirectory)/up_src_Player.cpp$(ObjectSuffix): ../src/Player.cpp $(IntermediateDirectory)/up_src_Player.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Player.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Player.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Player.cpp$(DependSuffix): ../src/Player.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Player.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Player.cpp$(DependSuffix) -MM ../src/Player.cpp

$(IntermediateDirectory)/up_src_Player.cpp$(PreprocessSuffix): ../src/Player.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Player.cpp$(PreprocessSuffix) ../src/Player.cpp

$(IntermediateDirectory)/up_src_Router.cpp$(ObjectSuffix): ../src/Router.cpp $(IntermediateDirectory)/up_src_Router.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Router.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Router.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Router.cpp$(DependSuffix): ../src/Router.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Router.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Router.cpp$(DependSuffix) -MM ../src/Router.cpp

$(IntermediateDirectory)/up_src_Router.cpp$(PreprocessSuffix): ../src/Router.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Router.cpp$(PreprocessSuffix) ../src/Router.cpp

$(IntermediateDirectory)/up_src_Smoke.cpp$(ObjectSuffix): ../src/Smoke.cpp $(IntermediateDirectory)/up_src_Smoke.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Smoke.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Smoke.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Smoke.cpp$(DependSuffix): ../src/Smoke.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Smoke.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Smoke.cpp$(DependSuffix) -MM ../src/Smoke.cpp

$(IntermediateDirectory)/up_src_Smoke.cpp$(PreprocessSuffix): ../src/Smoke.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Smoke.cpp$(PreprocessSuffix) ../src/Smoke.cpp

$(IntermediateDirectory)/up_src_Keyframe.cpp$(ObjectSuffix): ../src/Keyframe.cpp $(IntermediateDirectory)/up_src_Keyframe.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Keyframe.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Keyframe.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Keyframe.cpp$(DependSuffix): ../src/Keyframe.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Keyframe.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Keyframe.cpp$(DependSuffix) -MM ../src/Keyframe.cpp

$(IntermediateDirectory)/up_src_Keyframe.cpp$(PreprocessSuffix): ../src/Keyframe.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Keyframe.cpp$(PreprocessSuffix) ../src/Keyframe.cpp

$(IntermediateDirectory)/up_src_Sound.cpp$(ObjectSuffix): ../src/Sound.cpp $(IntermediateDirectory)/up_src_Sound.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Sound.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Sound.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Sound.cpp$(DependSuffix): ../src/Sound.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Sound.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Sound.cpp$(DependSuffix) -MM ../src/Sound.cpp

$(IntermediateDirectory)/up_src_Sound.cpp$(PreprocessSuffix): ../src/Sound.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Sound.cpp$(PreprocessSuffix) ../src/Sound.cpp

$(IntermediateDirectory)/up_src_SpinLock.cpp$(ObjectSuffix): ../src/SpinLock.cpp $(IntermediateDirectory)/up_src_SpinLock.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SpinLock.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SpinLock.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SpinLock.cpp$(DependSuffix): ../src/SpinLock.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SpinLock.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SpinLock.cpp$(DependSuffix) -MM ../src/SpinLock.cpp

$(IntermediateDirectory)/up_src_SpinLock.cpp$(PreprocessSuffix): ../src/SpinLock.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SpinLock.cpp$(PreprocessSuffix) ../src/SpinLock.cpp

$(IntermediateDirectory)/up_src_CommandStack.cpp$(ObjectSuffix): ../src/CommandStack.cpp $(IntermediateDirectory)/up_src_CommandStack.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/CommandStack.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_CommandStack.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_CommandStack.cpp$(DependSuffix): ../src/CommandStack.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_CommandStack.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_CommandStack.cpp$(DependSuffix) -MM ../src/CommandStack.cpp

$(IntermediateDirectory)/up_src_CommandStack.cpp$(PreprocessSuffix): ../src/CommandStack.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_CommandStack.cpp$(PreprocessSuffix) ../src/CommandStack.cpp

$(IntermediateDirectory)/up_src_TonnelVertex.cpp$(ObjectSuffix): ../src/TonnelVertex.cpp $(IntermediateDirectory)/up_src_TonnelVertex.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/TonnelVertex.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_TonnelVertex.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_TonnelVertex.cpp$(DependSuffix): ../src/TonnelVertex.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_TonnelVertex.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_TonnelVertex.cpp$(DependSuffix) -MM ../src/TonnelVertex.cpp

$(IntermediateDirectory)/up_src_TonnelVertex.cpp$(PreprocessSuffix): ../src/TonnelVertex.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_TonnelVertex.cpp$(PreprocessSuffix) ../src/TonnelVertex.cpp

$(IntermediateDirectory)/up_src_Config.cpp$(ObjectSuffix): ../src/Config.cpp $(IntermediateDirectory)/up_src_Config.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Config.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Config.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Config.cpp$(DependSuffix): ../src/Config.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Config.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Config.cpp$(DependSuffix) -MM ../src/Config.cpp

$(IntermediateDirectory)/up_src_Config.cpp$(PreprocessSuffix): ../src/Config.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Config.cpp$(PreprocessSuffix) ../src/Config.cpp

$(IntermediateDirectory)/up_src_Animation.cpp$(ObjectSuffix): ../src/Animation.cpp $(IntermediateDirectory)/up_src_Animation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Animation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Animation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Animation.cpp$(DependSuffix): ../src/Animation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Animation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Animation.cpp$(DependSuffix) -MM ../src/Animation.cpp

$(IntermediateDirectory)/up_src_Animation.cpp$(PreprocessSuffix): ../src/Animation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Animation.cpp$(PreprocessSuffix) ../src/Animation.cpp

$(IntermediateDirectory)/up_src_Hook.cpp$(ObjectSuffix): ../src/Hook.cpp $(IntermediateDirectory)/up_src_Hook.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Hook.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Hook.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Hook.cpp$(DependSuffix): ../src/Hook.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Hook.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Hook.cpp$(DependSuffix) -MM ../src/Hook.cpp

$(IntermediateDirectory)/up_src_Hook.cpp$(PreprocessSuffix): ../src/Hook.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Hook.cpp$(PreprocessSuffix) ../src/Hook.cpp

$(IntermediateDirectory)/up_src_TextureStack.cpp$(ObjectSuffix): ../src/TextureStack.cpp $(IntermediateDirectory)/up_src_TextureStack.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/TextureStack.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_TextureStack.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_TextureStack.cpp$(DependSuffix): ../src/TextureStack.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_TextureStack.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_TextureStack.cpp$(DependSuffix) -MM ../src/TextureStack.cpp

$(IntermediateDirectory)/up_src_TextureStack.cpp$(PreprocessSuffix): ../src/TextureStack.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_TextureStack.cpp$(PreprocessSuffix) ../src/TextureStack.cpp

$(IntermediateDirectory)/up_src_Checkbox.cpp$(ObjectSuffix): ../src/Checkbox.cpp $(IntermediateDirectory)/up_src_Checkbox.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Checkbox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Checkbox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Checkbox.cpp$(DependSuffix): ../src/Checkbox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Checkbox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Checkbox.cpp$(DependSuffix) -MM ../src/Checkbox.cpp

$(IntermediateDirectory)/up_src_Checkbox.cpp$(PreprocessSuffix): ../src/Checkbox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Checkbox.cpp$(PreprocessSuffix) ../src/Checkbox.cpp

$(IntermediateDirectory)/up_src_Event.cpp$(ObjectSuffix): ../src/Event.cpp $(IntermediateDirectory)/up_src_Event.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Event.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Event.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Event.cpp$(DependSuffix): ../src/Event.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Event.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Event.cpp$(DependSuffix) -MM ../src/Event.cpp

$(IntermediateDirectory)/up_src_Event.cpp$(PreprocessSuffix): ../src/Event.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Event.cpp$(PreprocessSuffix) ../src/Event.cpp

$(IntermediateDirectory)/up_src_RevealGun.cpp$(ObjectSuffix): ../src/RevealGun.cpp $(IntermediateDirectory)/up_src_RevealGun.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/RevealGun.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_RevealGun.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_RevealGun.cpp$(DependSuffix): ../src/RevealGun.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_RevealGun.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_RevealGun.cpp$(DependSuffix) -MM ../src/RevealGun.cpp

$(IntermediateDirectory)/up_src_RevealGun.cpp$(PreprocessSuffix): ../src/RevealGun.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_RevealGun.cpp$(PreprocessSuffix) ../src/RevealGun.cpp

$(IntermediateDirectory)/up_src_BossAreaGenerator.cpp$(ObjectSuffix): ../src/BossAreaGenerator.cpp $(IntermediateDirectory)/up_src_BossAreaGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/BossAreaGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_BossAreaGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_BossAreaGenerator.cpp$(DependSuffix): ../src/BossAreaGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_BossAreaGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_BossAreaGenerator.cpp$(DependSuffix) -MM ../src/BossAreaGenerator.cpp

$(IntermediateDirectory)/up_src_BossAreaGenerator.cpp$(PreprocessSuffix): ../src/BossAreaGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_BossAreaGenerator.cpp$(PreprocessSuffix) ../src/BossAreaGenerator.cpp

$(IntermediateDirectory)/up_src_WaterBlob.cpp$(ObjectSuffix): ../src/WaterBlob.cpp $(IntermediateDirectory)/up_src_WaterBlob.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/WaterBlob.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_WaterBlob.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_WaterBlob.cpp$(DependSuffix): ../src/WaterBlob.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_WaterBlob.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_WaterBlob.cpp$(DependSuffix) -MM ../src/WaterBlob.cpp

$(IntermediateDirectory)/up_src_WaterBlob.cpp$(PreprocessSuffix): ../src/WaterBlob.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_WaterBlob.cpp$(PreprocessSuffix) ../src/WaterBlob.cpp

$(IntermediateDirectory)/up_src_FlyingIslandsGenerator.cpp$(ObjectSuffix): ../src/FlyingIslandsGenerator.cpp $(IntermediateDirectory)/up_src_FlyingIslandsGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/FlyingIslandsGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_FlyingIslandsGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_FlyingIslandsGenerator.cpp$(DependSuffix): ../src/FlyingIslandsGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_FlyingIslandsGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_FlyingIslandsGenerator.cpp$(DependSuffix) -MM ../src/FlyingIslandsGenerator.cpp

$(IntermediateDirectory)/up_src_FlyingIslandsGenerator.cpp$(PreprocessSuffix): ../src/FlyingIslandsGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_FlyingIslandsGenerator.cpp$(PreprocessSuffix) ../src/FlyingIslandsGenerator.cpp

$(IntermediateDirectory)/up_src_JsonConverters.cpp$(ObjectSuffix): ../src/JsonConverters.cpp $(IntermediateDirectory)/up_src_JsonConverters.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/JsonConverters.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_JsonConverters.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_JsonConverters.cpp$(DependSuffix): ../src/JsonConverters.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_JsonConverters.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_JsonConverters.cpp$(DependSuffix) -MM ../src/JsonConverters.cpp

$(IntermediateDirectory)/up_src_JsonConverters.cpp$(PreprocessSuffix): ../src/JsonConverters.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_JsonConverters.cpp$(PreprocessSuffix) ../src/JsonConverters.cpp

$(IntermediateDirectory)/up_src_Theme.cpp$(ObjectSuffix): ../src/Theme.cpp $(IntermediateDirectory)/up_src_Theme.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Theme.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Theme.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Theme.cpp$(DependSuffix): ../src/Theme.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Theme.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Theme.cpp$(DependSuffix) -MM ../src/Theme.cpp

$(IntermediateDirectory)/up_src_Theme.cpp$(PreprocessSuffix): ../src/Theme.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Theme.cpp$(PreprocessSuffix) ../src/Theme.cpp

$(IntermediateDirectory)/up_src_Hitbox.cpp$(ObjectSuffix): ../src/Hitbox.cpp $(IntermediateDirectory)/up_src_Hitbox.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Hitbox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Hitbox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Hitbox.cpp$(DependSuffix): ../src/Hitbox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Hitbox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Hitbox.cpp$(DependSuffix) -MM ../src/Hitbox.cpp

$(IntermediateDirectory)/up_src_Hitbox.cpp$(PreprocessSuffix): ../src/Hitbox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Hitbox.cpp$(PreprocessSuffix) ../src/Hitbox.cpp

$(IntermediateDirectory)/up_src_Text.cpp$(ObjectSuffix): ../src/Text.cpp $(IntermediateDirectory)/up_src_Text.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Text.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Text.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Text.cpp$(DependSuffix): ../src/Text.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Text.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Text.cpp$(DependSuffix) -MM ../src/Text.cpp

$(IntermediateDirectory)/up_src_Text.cpp$(PreprocessSuffix): ../src/Text.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Text.cpp$(PreprocessSuffix) ../src/Text.cpp

$(IntermediateDirectory)/up_src_Frame.cpp$(ObjectSuffix): ../src/Frame.cpp $(IntermediateDirectory)/up_src_Frame.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Frame.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Frame.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Frame.cpp$(DependSuffix): ../src/Frame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Frame.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Frame.cpp$(DependSuffix) -MM ../src/Frame.cpp

$(IntermediateDirectory)/up_src_Frame.cpp$(PreprocessSuffix): ../src/Frame.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Frame.cpp$(PreprocessSuffix) ../src/Frame.cpp

$(IntermediateDirectory)/up_src_HUD.cpp$(ObjectSuffix): ../src/HUD.cpp $(IntermediateDirectory)/up_src_HUD.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/HUD.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_HUD.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_HUD.cpp$(DependSuffix): ../src/HUD.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_HUD.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_HUD.cpp$(DependSuffix) -MM ../src/HUD.cpp

$(IntermediateDirectory)/up_src_HUD.cpp$(PreprocessSuffix): ../src/HUD.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_HUD.cpp$(PreprocessSuffix) ../src/HUD.cpp

$(IntermediateDirectory)/up_src_LineSet.cpp$(ObjectSuffix): ../src/LineSet.cpp $(IntermediateDirectory)/up_src_LineSet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/LineSet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_LineSet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_LineSet.cpp$(DependSuffix): ../src/LineSet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_LineSet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_LineSet.cpp$(DependSuffix) -MM ../src/LineSet.cpp

$(IntermediateDirectory)/up_src_LineSet.cpp$(PreprocessSuffix): ../src/LineSet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_LineSet.cpp$(PreprocessSuffix) ../src/LineSet.cpp

$(IntermediateDirectory)/up_src_Stats.cpp$(ObjectSuffix): ../src/Stats.cpp $(IntermediateDirectory)/up_src_Stats.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Stats.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Stats.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Stats.cpp$(DependSuffix): ../src/Stats.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Stats.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Stats.cpp$(DependSuffix) -MM ../src/Stats.cpp

$(IntermediateDirectory)/up_src_Stats.cpp$(PreprocessSuffix): ../src/Stats.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Stats.cpp$(PreprocessSuffix) ../src/Stats.cpp

$(IntermediateDirectory)/up_src_BossCrystal.cpp$(ObjectSuffix): ../src/BossCrystal.cpp $(IntermediateDirectory)/up_src_BossCrystal.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/BossCrystal.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_BossCrystal.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_BossCrystal.cpp$(DependSuffix): ../src/BossCrystal.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_BossCrystal.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_BossCrystal.cpp$(DependSuffix) -MM ../src/BossCrystal.cpp

$(IntermediateDirectory)/up_src_BossCrystal.cpp$(PreprocessSuffix): ../src/BossCrystal.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_BossCrystal.cpp$(PreprocessSuffix) ../src/BossCrystal.cpp

$(IntermediateDirectory)/up_src_SetNodeScaleCommand.cpp$(ObjectSuffix): ../src/SetNodeScaleCommand.cpp $(IntermediateDirectory)/up_src_SetNodeScaleCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SetNodeScaleCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SetNodeScaleCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SetNodeScaleCommand.cpp$(DependSuffix): ../src/SetNodeScaleCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SetNodeScaleCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SetNodeScaleCommand.cpp$(DependSuffix) -MM ../src/SetNodeScaleCommand.cpp

$(IntermediateDirectory)/up_src_SetNodeScaleCommand.cpp$(PreprocessSuffix): ../src/SetNodeScaleCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SetNodeScaleCommand.cpp$(PreprocessSuffix) ../src/SetNodeScaleCommand.cpp

$(IntermediateDirectory)/up_src_FogGate.cpp$(ObjectSuffix): ../src/FogGate.cpp $(IntermediateDirectory)/up_src_FogGate.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/FogGate.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_FogGate.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_FogGate.cpp$(DependSuffix): ../src/FogGate.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_FogGate.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_FogGate.cpp$(DependSuffix) -MM ../src/FogGate.cpp

$(IntermediateDirectory)/up_src_FogGate.cpp$(PreprocessSuffix): ../src/FogGate.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_FogGate.cpp$(PreprocessSuffix) ../src/FogGate.cpp

$(IntermediateDirectory)/up_src_SetNodeNameCommand.cpp$(ObjectSuffix): ../src/SetNodeNameCommand.cpp $(IntermediateDirectory)/up_src_SetNodeNameCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SetNodeNameCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SetNodeNameCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SetNodeNameCommand.cpp$(DependSuffix): ../src/SetNodeNameCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SetNodeNameCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SetNodeNameCommand.cpp$(DependSuffix) -MM ../src/SetNodeNameCommand.cpp

$(IntermediateDirectory)/up_src_SetNodeNameCommand.cpp$(PreprocessSuffix): ../src/SetNodeNameCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SetNodeNameCommand.cpp$(PreprocessSuffix) ../src/SetNodeNameCommand.cpp

$(IntermediateDirectory)/up_src_Remains.cpp$(ObjectSuffix): ../src/Remains.cpp $(IntermediateDirectory)/up_src_Remains.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Remains.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Remains.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Remains.cpp$(DependSuffix): ../src/Remains.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Remains.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Remains.cpp$(DependSuffix) -MM ../src/Remains.cpp

$(IntermediateDirectory)/up_src_Remains.cpp$(PreprocessSuffix): ../src/Remains.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Remains.cpp$(PreprocessSuffix) ../src/Remains.cpp

$(IntermediateDirectory)/up_src_Renderer.cpp$(ObjectSuffix): ../src/Renderer.cpp $(IntermediateDirectory)/up_src_Renderer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Renderer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Renderer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Renderer.cpp$(DependSuffix): ../src/Renderer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Renderer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Renderer.cpp$(DependSuffix) -MM ../src/Renderer.cpp

$(IntermediateDirectory)/up_src_Renderer.cpp$(PreprocessSuffix): ../src/Renderer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Renderer.cpp$(PreprocessSuffix) ../src/Renderer.cpp

$(IntermediateDirectory)/up_src_EditorCommand.cpp$(ObjectSuffix): ../src/EditorCommand.cpp $(IntermediateDirectory)/up_src_EditorCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/EditorCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_EditorCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_EditorCommand.cpp$(DependSuffix): ../src/EditorCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_EditorCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_EditorCommand.cpp$(DependSuffix) -MM ../src/EditorCommand.cpp

$(IntermediateDirectory)/up_src_EditorCommand.cpp$(PreprocessSuffix): ../src/EditorCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_EditorCommand.cpp$(PreprocessSuffix) ../src/EditorCommand.cpp

$(IntermediateDirectory)/up_src_Precompiled.cpp$(ObjectSuffix): ../src/Precompiled.cpp $(IntermediateDirectory)/up_src_Precompiled.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Precompiled.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Precompiled.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Precompiled.cpp$(DependSuffix): ../src/Precompiled.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Precompiled.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Precompiled.cpp$(DependSuffix) -MM ../src/Precompiled.cpp

$(IntermediateDirectory)/up_src_Precompiled.cpp$(PreprocessSuffix): ../src/Precompiled.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Precompiled.cpp$(PreprocessSuffix) ../src/Precompiled.cpp

$(IntermediateDirectory)/up_src_MoveInteractionMode.cpp$(ObjectSuffix): ../src/MoveInteractionMode.cpp $(IntermediateDirectory)/up_src_MoveInteractionMode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/MoveInteractionMode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_MoveInteractionMode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_MoveInteractionMode.cpp$(DependSuffix): ../src/MoveInteractionMode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_MoveInteractionMode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_MoveInteractionMode.cpp$(DependSuffix) -MM ../src/MoveInteractionMode.cpp

$(IntermediateDirectory)/up_src_MoveInteractionMode.cpp$(PreprocessSuffix): ../src/MoveInteractionMode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_MoveInteractionMode.cpp$(PreprocessSuffix) ../src/MoveInteractionMode.cpp

$(IntermediateDirectory)/up_src_VisualEffect.cpp$(ObjectSuffix): ../src/VisualEffect.cpp $(IntermediateDirectory)/up_src_VisualEffect.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/VisualEffect.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_VisualEffect.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_VisualEffect.cpp$(DependSuffix): ../src/VisualEffect.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_VisualEffect.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_VisualEffect.cpp$(DependSuffix) -MM ../src/VisualEffect.cpp

$(IntermediateDirectory)/up_src_VisualEffect.cpp$(PreprocessSuffix): ../src/VisualEffect.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_VisualEffect.cpp$(PreprocessSuffix) ../src/VisualEffect.cpp

$(IntermediateDirectory)/up_src_Level.cpp$(ObjectSuffix): ../src/Level.cpp $(IntermediateDirectory)/up_src_Level.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Level.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Level.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Level.cpp$(DependSuffix): ../src/Level.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Level.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Level.cpp$(DependSuffix) -MM ../src/Level.cpp

$(IntermediateDirectory)/up_src_Level.cpp$(PreprocessSuffix): ../src/Level.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Level.cpp$(PreprocessSuffix) ../src/Level.cpp

$(IntermediateDirectory)/up_src_ArbitraryRectangle.cpp$(ObjectSuffix): ../src/ArbitraryRectangle.cpp $(IntermediateDirectory)/up_src_ArbitraryRectangle.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/ArbitraryRectangle.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ArbitraryRectangle.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ArbitraryRectangle.cpp$(DependSuffix): ../src/ArbitraryRectangle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ArbitraryRectangle.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ArbitraryRectangle.cpp$(DependSuffix) -MM ../src/ArbitraryRectangle.cpp

$(IntermediateDirectory)/up_src_ArbitraryRectangle.cpp$(PreprocessSuffix): ../src/ArbitraryRectangle.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ArbitraryRectangle.cpp$(PreprocessSuffix) ../src/ArbitraryRectangle.cpp

$(IntermediateDirectory)/up_src_BarterWindow.cpp$(ObjectSuffix): ../src/BarterWindow.cpp $(IntermediateDirectory)/up_src_BarterWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/BarterWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_BarterWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_BarterWindow.cpp$(DependSuffix): ../src/BarterWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_BarterWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_BarterWindow.cpp$(DependSuffix) -MM ../src/BarterWindow.cpp

$(IntermediateDirectory)/up_src_BarterWindow.cpp$(PreprocessSuffix): ../src/BarterWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_BarterWindow.cpp$(PreprocessSuffix) ../src/BarterWindow.cpp

$(IntermediateDirectory)/up_src_AddAnimationTrackCommand.cpp$(ObjectSuffix): ../src/AddAnimationTrackCommand.cpp $(IntermediateDirectory)/up_src_AddAnimationTrackCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/AddAnimationTrackCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_AddAnimationTrackCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_AddAnimationTrackCommand.cpp$(DependSuffix): ../src/AddAnimationTrackCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_AddAnimationTrackCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_AddAnimationTrackCommand.cpp$(DependSuffix) -MM ../src/AddAnimationTrackCommand.cpp

$(IntermediateDirectory)/up_src_AddAnimationTrackCommand.cpp$(PreprocessSuffix): ../src/AddAnimationTrackCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_AddAnimationTrackCommand.cpp$(PreprocessSuffix) ../src/AddAnimationTrackCommand.cpp

$(IntermediateDirectory)/up_src_SetSpriteTextureCommand.cpp$(ObjectSuffix): ../src/SetSpriteTextureCommand.cpp $(IntermediateDirectory)/up_src_SetSpriteTextureCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SetSpriteTextureCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SetSpriteTextureCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SetSpriteTextureCommand.cpp$(DependSuffix): ../src/SetSpriteTextureCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SetSpriteTextureCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SetSpriteTextureCommand.cpp$(DependSuffix) -MM ../src/SetSpriteTextureCommand.cpp

$(IntermediateDirectory)/up_src_SetSpriteTextureCommand.cpp$(PreprocessSuffix): ../src/SetSpriteTextureCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SetSpriteTextureCommand.cpp$(PreprocessSuffix) ../src/SetSpriteTextureCommand.cpp

$(IntermediateDirectory)/up_src_AttachCommand.cpp$(ObjectSuffix): ../src/AttachCommand.cpp $(IntermediateDirectory)/up_src_AttachCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/AttachCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_AttachCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_AttachCommand.cpp$(DependSuffix): ../src/AttachCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_AttachCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_AttachCommand.cpp$(DependSuffix) -MM ../src/AttachCommand.cpp

$(IntermediateDirectory)/up_src_AttachCommand.cpp$(PreprocessSuffix): ../src/AttachCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_AttachCommand.cpp$(PreprocessSuffix) ../src/AttachCommand.cpp

$(IntermediateDirectory)/up_src_IK.cpp$(ObjectSuffix): ../src/IK.cpp $(IntermediateDirectory)/up_src_IK.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/IK.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_IK.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_IK.cpp$(DependSuffix): ../src/IK.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_IK.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_IK.cpp$(DependSuffix) -MM ../src/IK.cpp

$(IntermediateDirectory)/up_src_IK.cpp$(PreprocessSuffix): ../src/IK.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_IK.cpp$(PreprocessSuffix) ../src/IK.cpp

$(IntermediateDirectory)/up_src_Selection.cpp$(ObjectSuffix): ../src/Selection.cpp $(IntermediateDirectory)/up_src_Selection.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Selection.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Selection.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Selection.cpp$(DependSuffix): ../src/Selection.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Selection.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Selection.cpp$(DependSuffix) -MM ../src/Selection.cpp

$(IntermediateDirectory)/up_src_Selection.cpp$(PreprocessSuffix): ../src/Selection.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Selection.cpp$(PreprocessSuffix) ../src/Selection.cpp

$(IntermediateDirectory)/up_src_Node.cpp$(ObjectSuffix): ../src/Node.cpp $(IntermediateDirectory)/up_src_Node.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Node.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Node.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Node.cpp$(DependSuffix): ../src/Node.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Node.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Node.cpp$(DependSuffix) -MM ../src/Node.cpp

$(IntermediateDirectory)/up_src_Node.cpp$(PreprocessSuffix): ../src/Node.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Node.cpp$(PreprocessSuffix) ../src/Node.cpp

$(IntermediateDirectory)/up_src_Dwarf.cpp$(ObjectSuffix): ../src/Dwarf.cpp $(IntermediateDirectory)/up_src_Dwarf.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Dwarf.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Dwarf.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Dwarf.cpp$(DependSuffix): ../src/Dwarf.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Dwarf.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Dwarf.cpp$(DependSuffix) -MM ../src/Dwarf.cpp

$(IntermediateDirectory)/up_src_Dwarf.cpp$(PreprocessSuffix): ../src/Dwarf.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Dwarf.cpp$(PreprocessSuffix) ../src/Dwarf.cpp

$(IntermediateDirectory)/up_src_GrenadeLauncher.cpp$(ObjectSuffix): ../src/GrenadeLauncher.cpp $(IntermediateDirectory)/up_src_GrenadeLauncher.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/GrenadeLauncher.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_GrenadeLauncher.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_GrenadeLauncher.cpp$(DependSuffix): ../src/GrenadeLauncher.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_GrenadeLauncher.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_GrenadeLauncher.cpp$(DependSuffix) -MM ../src/GrenadeLauncher.cpp

$(IntermediateDirectory)/up_src_GrenadeLauncher.cpp$(PreprocessSuffix): ../src/GrenadeLauncher.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_GrenadeLauncher.cpp$(PreprocessSuffix) ../src/GrenadeLauncher.cpp

$(IntermediateDirectory)/up_src_Debris.cpp$(ObjectSuffix): ../src/Debris.cpp $(IntermediateDirectory)/up_src_Debris.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Debris.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Debris.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Debris.cpp$(DependSuffix): ../src/Debris.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Debris.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Debris.cpp$(DependSuffix) -MM ../src/Debris.cpp

$(IntermediateDirectory)/up_src_Debris.cpp$(PreprocessSuffix): ../src/Debris.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Debris.cpp$(PreprocessSuffix) ../src/Debris.cpp

$(IntermediateDirectory)/up_src_RailGun.cpp$(ObjectSuffix): ../src/RailGun.cpp $(IntermediateDirectory)/up_src_RailGun.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/RailGun.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_RailGun.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_RailGun.cpp$(DependSuffix): ../src/RailGun.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_RailGun.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_RailGun.cpp$(DependSuffix) -MM ../src/RailGun.cpp

$(IntermediateDirectory)/up_src_RailGun.cpp$(PreprocessSuffix): ../src/RailGun.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_RailGun.cpp$(PreprocessSuffix) ../src/RailGun.cpp

$(IntermediateDirectory)/up_src_Scene.cpp$(ObjectSuffix): ../src/Scene.cpp $(IntermediateDirectory)/up_src_Scene.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Scene.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Scene.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Scene.cpp$(DependSuffix): ../src/Scene.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Scene.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Scene.cpp$(DependSuffix) -MM ../src/Scene.cpp

$(IntermediateDirectory)/up_src_Scene.cpp$(PreprocessSuffix): ../src/Scene.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Scene.cpp$(PreprocessSuffix) ../src/Scene.cpp

$(IntermediateDirectory)/up_src_Terrain.cpp$(ObjectSuffix): ../src/Terrain.cpp $(IntermediateDirectory)/up_src_Terrain.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Terrain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Terrain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Terrain.cpp$(DependSuffix): ../src/Terrain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Terrain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Terrain.cpp$(DependSuffix) -MM ../src/Terrain.cpp

$(IntermediateDirectory)/up_src_Terrain.cpp$(PreprocessSuffix): ../src/Terrain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Terrain.cpp$(PreprocessSuffix) ../src/Terrain.cpp

$(IntermediateDirectory)/up_src_BehaviourTree.cpp$(ObjectSuffix): ../src/BehaviourTree.cpp $(IntermediateDirectory)/up_src_BehaviourTree.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/BehaviourTree.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_BehaviourTree.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_BehaviourTree.cpp$(DependSuffix): ../src/BehaviourTree.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_BehaviourTree.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_BehaviourTree.cpp$(DependSuffix) -MM ../src/BehaviourTree.cpp

$(IntermediateDirectory)/up_src_BehaviourTree.cpp$(PreprocessSuffix): ../src/BehaviourTree.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_BehaviourTree.cpp$(PreprocessSuffix) ../src/BehaviourTree.cpp

$(IntermediateDirectory)/up_src_AStar.cpp$(ObjectSuffix): ../src/AStar.cpp $(IntermediateDirectory)/up_src_AStar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/AStar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_AStar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_AStar.cpp$(DependSuffix): ../src/AStar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_AStar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_AStar.cpp$(DependSuffix) -MM ../src/AStar.cpp

$(IntermediateDirectory)/up_src_AStar.cpp$(PreprocessSuffix): ../src/AStar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_AStar.cpp$(PreprocessSuffix) ../src/AStar.cpp

$(IntermediateDirectory)/up_src_Reward.cpp$(ObjectSuffix): ../src/Reward.cpp $(IntermediateDirectory)/up_src_Reward.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Reward.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Reward.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Reward.cpp$(DependSuffix): ../src/Reward.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Reward.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Reward.cpp$(DependSuffix) -MM ../src/Reward.cpp

$(IntermediateDirectory)/up_src_Reward.cpp$(PreprocessSuffix): ../src/Reward.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Reward.cpp$(PreprocessSuffix) ../src/Reward.cpp

$(IntermediateDirectory)/up_src_Texture.cpp$(ObjectSuffix): ../src/Texture.cpp $(IntermediateDirectory)/up_src_Texture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Texture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Texture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Texture.cpp$(DependSuffix): ../src/Texture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Texture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Texture.cpp$(DependSuffix) -MM ../src/Texture.cpp

$(IntermediateDirectory)/up_src_Texture.cpp$(PreprocessSuffix): ../src/Texture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Texture.cpp$(PreprocessSuffix) ../src/Texture.cpp

$(IntermediateDirectory)/up_src_QuestBase.cpp$(ObjectSuffix): ../src/QuestBase.cpp $(IntermediateDirectory)/up_src_QuestBase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/QuestBase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_QuestBase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_QuestBase.cpp$(DependSuffix): ../src/QuestBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_QuestBase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_QuestBase.cpp$(DependSuffix) -MM ../src/QuestBase.cpp

$(IntermediateDirectory)/up_src_QuestBase.cpp$(PreprocessSuffix): ../src/QuestBase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_QuestBase.cpp$(PreprocessSuffix) ../src/QuestBase.cpp

$(IntermediateDirectory)/up_src_Button.cpp$(ObjectSuffix): ../src/Button.cpp $(IntermediateDirectory)/up_src_Button.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Button.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Button.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Button.cpp$(DependSuffix): ../src/Button.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Button.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Button.cpp$(DependSuffix) -MM ../src/Button.cpp

$(IntermediateDirectory)/up_src_Button.cpp$(PreprocessSuffix): ../src/Button.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Button.cpp$(PreprocessSuffix) ../src/Button.cpp

$(IntermediateDirectory)/up_src_Editor.cpp$(ObjectSuffix): ../src/Editor.cpp $(IntermediateDirectory)/up_src_Editor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Editor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Editor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Editor.cpp$(DependSuffix): ../src/Editor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Editor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Editor.cpp$(DependSuffix) -MM ../src/Editor.cpp

$(IntermediateDirectory)/up_src_Editor.cpp$(PreprocessSuffix): ../src/Editor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Editor.cpp$(PreprocessSuffix) ../src/Editor.cpp

$(IntermediateDirectory)/up_src_InventoryGUI.cpp$(ObjectSuffix): ../src/InventoryGUI.cpp $(IntermediateDirectory)/up_src_InventoryGUI.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/InventoryGUI.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_InventoryGUI.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_InventoryGUI.cpp$(DependSuffix): ../src/InventoryGUI.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_InventoryGUI.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_InventoryGUI.cpp$(DependSuffix) -MM ../src/InventoryGUI.cpp

$(IntermediateDirectory)/up_src_InventoryGUI.cpp$(PreprocessSuffix): ../src/InventoryGUI.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_InventoryGUI.cpp$(PreprocessSuffix) ../src/InventoryGUI.cpp

$(IntermediateDirectory)/up_src_MaskOperation.cpp$(ObjectSuffix): ../src/MaskOperation.cpp $(IntermediateDirectory)/up_src_MaskOperation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/MaskOperation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_MaskOperation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_MaskOperation.cpp$(DependSuffix): ../src/MaskOperation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_MaskOperation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_MaskOperation.cpp$(DependSuffix) -MM ../src/MaskOperation.cpp

$(IntermediateDirectory)/up_src_MaskOperation.cpp$(PreprocessSuffix): ../src/MaskOperation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_MaskOperation.cpp$(PreprocessSuffix) ../src/MaskOperation.cpp

$(IntermediateDirectory)/up_src_Console.cpp$(ObjectSuffix): ../src/Console.cpp $(IntermediateDirectory)/up_src_Console.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Console.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Console.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Console.cpp$(DependSuffix): ../src/Console.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Console.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Console.cpp$(DependSuffix) -MM ../src/Console.cpp

$(IntermediateDirectory)/up_src_Console.cpp$(PreprocessSuffix): ../src/Console.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Console.cpp$(PreprocessSuffix) ../src/Console.cpp

$(IntermediateDirectory)/up_src_MapWindow.cpp$(ObjectSuffix): ../src/MapWindow.cpp $(IntermediateDirectory)/up_src_MapWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/MapWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_MapWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_MapWindow.cpp$(DependSuffix): ../src/MapWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_MapWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_MapWindow.cpp$(DependSuffix) -MM ../src/MapWindow.cpp

$(IntermediateDirectory)/up_src_MapWindow.cpp$(PreprocessSuffix): ../src/MapWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_MapWindow.cpp$(PreprocessSuffix) ../src/MapWindow.cpp

$(IntermediateDirectory)/up_src_SetSelectedCommand.cpp$(ObjectSuffix): ../src/SetSelectedCommand.cpp $(IntermediateDirectory)/up_src_SetSelectedCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SetSelectedCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SetSelectedCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SetSelectedCommand.cpp$(DependSuffix): ../src/SetSelectedCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SetSelectedCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SetSelectedCommand.cpp$(DependSuffix) -MM ../src/SetSelectedCommand.cpp

$(IntermediateDirectory)/up_src_SetSelectedCommand.cpp$(PreprocessSuffix): ../src/SetSelectedCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SetSelectedCommand.cpp$(PreprocessSuffix) ../src/SetSelectedCommand.cpp

$(IntermediateDirectory)/up_src_Bench.cpp$(ObjectSuffix): ../src/Bench.cpp $(IntermediateDirectory)/up_src_Bench.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Bench.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Bench.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Bench.cpp$(DependSuffix): ../src/Bench.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Bench.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Bench.cpp$(DependSuffix) -MM ../src/Bench.cpp

$(IntermediateDirectory)/up_src_Bench.cpp$(PreprocessSuffix): ../src/Bench.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Bench.cpp$(PreprocessSuffix) ../src/Bench.cpp

$(IntermediateDirectory)/up_src_Exception.cpp$(ObjectSuffix): ../src/Exception.cpp $(IntermediateDirectory)/up_src_Exception.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Exception.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Exception.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Exception.cpp$(DependSuffix): ../src/Exception.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Exception.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Exception.cpp$(DependSuffix) -MM ../src/Exception.cpp

$(IntermediateDirectory)/up_src_Exception.cpp$(PreprocessSuffix): ../src/Exception.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Exception.cpp$(PreprocessSuffix) ../src/Exception.cpp

$(IntermediateDirectory)/up_src_Cell.cpp$(ObjectSuffix): ../src/Cell.cpp $(IntermediateDirectory)/up_src_Cell.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Cell.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Cell.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Cell.cpp$(DependSuffix): ../src/Cell.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Cell.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Cell.cpp$(DependSuffix) -MM ../src/Cell.cpp

$(IntermediateDirectory)/up_src_Cell.cpp$(PreprocessSuffix): ../src/Cell.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Cell.cpp$(PreprocessSuffix) ../src/Cell.cpp

$(IntermediateDirectory)/up_src_Light.cpp$(ObjectSuffix): ../src/Light.cpp $(IntermediateDirectory)/up_src_Light.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Light.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Light.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Light.cpp$(DependSuffix): ../src/Light.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Light.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Light.cpp$(DependSuffix) -MM ../src/Light.cpp

$(IntermediateDirectory)/up_src_Light.cpp$(PreprocessSuffix): ../src/Light.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Light.cpp$(PreprocessSuffix) ../src/Light.cpp

$(IntermediateDirectory)/up_src_Cloud.cpp$(ObjectSuffix): ../src/Cloud.cpp $(IntermediateDirectory)/up_src_Cloud.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Cloud.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Cloud.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Cloud.cpp$(DependSuffix): ../src/Cloud.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Cloud.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Cloud.cpp$(DependSuffix) -MM ../src/Cloud.cpp

$(IntermediateDirectory)/up_src_Cloud.cpp$(PreprocessSuffix): ../src/Cloud.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Cloud.cpp$(PreprocessSuffix) ../src/Cloud.cpp

$(IntermediateDirectory)/up_src_PolyLine.cpp$(ObjectSuffix): ../src/PolyLine.cpp $(IntermediateDirectory)/up_src_PolyLine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/PolyLine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_PolyLine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_PolyLine.cpp$(DependSuffix): ../src/PolyLine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_PolyLine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_PolyLine.cpp$(DependSuffix) -MM ../src/PolyLine.cpp

$(IntermediateDirectory)/up_src_PolyLine.cpp$(PreprocessSuffix): ../src/PolyLine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_PolyLine.cpp$(PreprocessSuffix) ../src/PolyLine.cpp

$(IntermediateDirectory)/up_src_NameDatabase.cpp$(ObjectSuffix): ../src/NameDatabase.cpp $(IntermediateDirectory)/up_src_NameDatabase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/NameDatabase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_NameDatabase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_NameDatabase.cpp$(DependSuffix): ../src/NameDatabase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_NameDatabase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_NameDatabase.cpp$(DependSuffix) -MM ../src/NameDatabase.cpp

$(IntermediateDirectory)/up_src_NameDatabase.cpp$(PreprocessSuffix): ../src/NameDatabase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_NameDatabase.cpp$(PreprocessSuffix) ../src/NameDatabase.cpp

$(IntermediateDirectory)/up_src_SetNodePositionCommand.cpp$(ObjectSuffix): ../src/SetNodePositionCommand.cpp $(IntermediateDirectory)/up_src_SetNodePositionCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SetNodePositionCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SetNodePositionCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SetNodePositionCommand.cpp$(DependSuffix): ../src/SetNodePositionCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SetNodePositionCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SetNodePositionCommand.cpp$(DependSuffix) -MM ../src/SetNodePositionCommand.cpp

$(IntermediateDirectory)/up_src_SetNodePositionCommand.cpp$(PreprocessSuffix): ../src/SetNodePositionCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SetNodePositionCommand.cpp$(PreprocessSuffix) ../src/SetNodePositionCommand.cpp

$(IntermediateDirectory)/up_src_ParticleSystem.cpp$(ObjectSuffix): ../src/ParticleSystem.cpp $(IntermediateDirectory)/up_src_ParticleSystem.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/ParticleSystem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ParticleSystem.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ParticleSystem.cpp$(DependSuffix): ../src/ParticleSystem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ParticleSystem.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ParticleSystem.cpp$(DependSuffix) -MM ../src/ParticleSystem.cpp

$(IntermediateDirectory)/up_src_ParticleSystem.cpp$(PreprocessSuffix): ../src/ParticleSystem.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ParticleSystem.cpp$(PreprocessSuffix) ../src/ParticleSystem.cpp

$(IntermediateDirectory)/up_src_NPC.cpp$(ObjectSuffix): ../src/NPC.cpp $(IntermediateDirectory)/up_src_NPC.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/NPC.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_NPC.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_NPC.cpp$(DependSuffix): ../src/NPC.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_NPC.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_NPC.cpp$(DependSuffix) -MM ../src/NPC.cpp

$(IntermediateDirectory)/up_src_NPC.cpp$(PreprocessSuffix): ../src/NPC.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_NPC.cpp$(PreprocessSuffix) ../src/NPC.cpp

$(IntermediateDirectory)/up_src_LiquidParticle.cpp$(ObjectSuffix): ../src/LiquidParticle.cpp $(IntermediateDirectory)/up_src_LiquidParticle.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/LiquidParticle.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_LiquidParticle.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_LiquidParticle.cpp$(DependSuffix): ../src/LiquidParticle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_LiquidParticle.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_LiquidParticle.cpp$(DependSuffix) -MM ../src/LiquidParticle.cpp

$(IntermediateDirectory)/up_src_LiquidParticle.cpp$(PreprocessSuffix): ../src/LiquidParticle.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_LiquidParticle.cpp$(PreprocessSuffix) ../src/LiquidParticle.cpp

$(IntermediateDirectory)/up_src_HouseGenerator.cpp$(ObjectSuffix): ../src/HouseGenerator.cpp $(IntermediateDirectory)/up_src_HouseGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/HouseGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_HouseGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_HouseGenerator.cpp$(DependSuffix): ../src/HouseGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_HouseGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_HouseGenerator.cpp$(DependSuffix) -MM ../src/HouseGenerator.cpp

$(IntermediateDirectory)/up_src_HouseGenerator.cpp$(PreprocessSuffix): ../src/HouseGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_HouseGenerator.cpp$(PreprocessSuffix) ../src/HouseGenerator.cpp

$(IntermediateDirectory)/up_src_Layout.cpp$(ObjectSuffix): ../src/Layout.cpp $(IntermediateDirectory)/up_src_Layout.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Layout.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Layout.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Layout.cpp$(DependSuffix): ../src/Layout.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Layout.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Layout.cpp$(DependSuffix) -MM ../src/Layout.cpp

$(IntermediateDirectory)/up_src_Layout.cpp$(PreprocessSuffix): ../src/Layout.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Layout.cpp$(PreprocessSuffix) ../src/Layout.cpp

$(IntermediateDirectory)/up_src_MonsterGenerator.cpp$(ObjectSuffix): ../src/MonsterGenerator.cpp $(IntermediateDirectory)/up_src_MonsterGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/MonsterGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_MonsterGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_MonsterGenerator.cpp$(DependSuffix): ../src/MonsterGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_MonsterGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_MonsterGenerator.cpp$(DependSuffix) -MM ../src/MonsterGenerator.cpp

$(IntermediateDirectory)/up_src_MonsterGenerator.cpp$(PreprocessSuffix): ../src/MonsterGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_MonsterGenerator.cpp$(PreprocessSuffix) ../src/MonsterGenerator.cpp

$(IntermediateDirectory)/up_src_Drill.cpp$(ObjectSuffix): ../src/Drill.cpp $(IntermediateDirectory)/up_src_Drill.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Drill.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Drill.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Drill.cpp$(DependSuffix): ../src/Drill.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Drill.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Drill.cpp$(DependSuffix) -MM ../src/Drill.cpp

$(IntermediateDirectory)/up_src_Drill.cpp$(PreprocessSuffix): ../src/Drill.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Drill.cpp$(PreprocessSuffix) ../src/Drill.cpp

$(IntermediateDirectory)/up_src_LootList.cpp$(ObjectSuffix): ../src/LootList.cpp $(IntermediateDirectory)/up_src_LootList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/LootList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_LootList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_LootList.cpp$(DependSuffix): ../src/LootList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_LootList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_LootList.cpp$(DependSuffix) -MM ../src/LootList.cpp

$(IntermediateDirectory)/up_src_LootList.cpp$(PreprocessSuffix): ../src/LootList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_LootList.cpp$(PreprocessSuffix) ../src/LootList.cpp

$(IntermediateDirectory)/up_src_Sprite.cpp$(ObjectSuffix): ../src/Sprite.cpp $(IntermediateDirectory)/up_src_Sprite.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Sprite.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Sprite.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Sprite.cpp$(DependSuffix): ../src/Sprite.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Sprite.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Sprite.cpp$(DependSuffix) -MM ../src/Sprite.cpp

$(IntermediateDirectory)/up_src_Sprite.cpp$(PreprocessSuffix): ../src/Sprite.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Sprite.cpp$(PreprocessSuffix) ../src/Sprite.cpp

$(IntermediateDirectory)/up_src_Image.cpp$(ObjectSuffix): ../src/Image.cpp $(IntermediateDirectory)/up_src_Image.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Image.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Image.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Image.cpp$(DependSuffix): ../src/Image.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Image.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Image.cpp$(DependSuffix) -MM ../src/Image.cpp

$(IntermediateDirectory)/up_src_Image.cpp$(PreprocessSuffix): ../src/Image.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Image.cpp$(PreprocessSuffix) ../src/Image.cpp

$(IntermediateDirectory)/up_src_Island.cpp$(ObjectSuffix): ../src/Island.cpp $(IntermediateDirectory)/up_src_Island.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Island.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Island.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Island.cpp$(DependSuffix): ../src/Island.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Island.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Island.cpp$(DependSuffix) -MM ../src/Island.cpp

$(IntermediateDirectory)/up_src_Island.cpp$(PreprocessSuffix): ../src/Island.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Island.cpp$(PreprocessSuffix) ../src/Island.cpp

$(IntermediateDirectory)/up_src_SetNodeOriginCommand.cpp$(ObjectSuffix): ../src/SetNodeOriginCommand.cpp $(IntermediateDirectory)/up_src_SetNodeOriginCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SetNodeOriginCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SetNodeOriginCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SetNodeOriginCommand.cpp$(DependSuffix): ../src/SetNodeOriginCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SetNodeOriginCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SetNodeOriginCommand.cpp$(DependSuffix) -MM ../src/SetNodeOriginCommand.cpp

$(IntermediateDirectory)/up_src_SetNodeOriginCommand.cpp$(PreprocessSuffix): ../src/SetNodeOriginCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SetNodeOriginCommand.cpp$(PreprocessSuffix) ../src/SetNodeOriginCommand.cpp

$(IntermediateDirectory)/up_src_GravityGun.cpp$(ObjectSuffix): ../src/GravityGun.cpp $(IntermediateDirectory)/up_src_GravityGun.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/GravityGun.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_GravityGun.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_GravityGun.cpp$(DependSuffix): ../src/GravityGun.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_GravityGun.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_GravityGun.cpp$(DependSuffix) -MM ../src/GravityGun.cpp

$(IntermediateDirectory)/up_src_GravityGun.cpp$(PreprocessSuffix): ../src/GravityGun.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_GravityGun.cpp$(PreprocessSuffix) ../src/GravityGun.cpp

$(IntermediateDirectory)/up_src_BossBeast.cpp$(ObjectSuffix): ../src/BossBeast.cpp $(IntermediateDirectory)/up_src_BossBeast.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/BossBeast.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_BossBeast.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_BossBeast.cpp$(DependSuffix): ../src/BossBeast.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_BossBeast.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_BossBeast.cpp$(DependSuffix) -MM ../src/BossBeast.cpp

$(IntermediateDirectory)/up_src_BossBeast.cpp$(PreprocessSuffix): ../src/BossBeast.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_BossBeast.cpp$(PreprocessSuffix) ../src/BossBeast.cpp

$(IntermediateDirectory)/up_src_Input.cpp$(ObjectSuffix): ../src/Input.cpp $(IntermediateDirectory)/up_src_Input.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Input.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Input.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Input.cpp$(DependSuffix): ../src/Input.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Input.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Input.cpp$(DependSuffix) -MM ../src/Input.cpp

$(IntermediateDirectory)/up_src_Input.cpp$(PreprocessSuffix): ../src/Input.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Input.cpp$(PreprocessSuffix) ../src/Input.cpp

$(IntermediateDirectory)/up_src_SetSpriteTextureRectangleCommand.cpp$(ObjectSuffix): ../src/SetSpriteTextureRectangleCommand.cpp $(IntermediateDirectory)/up_src_SetSpriteTextureRectangleCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SetSpriteTextureRectangleCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SetSpriteTextureRectangleCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SetSpriteTextureRectangleCommand.cpp$(DependSuffix): ../src/SetSpriteTextureRectangleCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SetSpriteTextureRectangleCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SetSpriteTextureRectangleCommand.cpp$(DependSuffix) -MM ../src/SetSpriteTextureRectangleCommand.cpp

$(IntermediateDirectory)/up_src_SetSpriteTextureRectangleCommand.cpp$(PreprocessSuffix): ../src/SetSpriteTextureRectangleCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SetSpriteTextureRectangleCommand.cpp$(PreprocessSuffix) ../src/SetSpriteTextureRectangleCommand.cpp

$(IntermediateDirectory)/up_src_ItemDefinitionEditor.cpp$(ObjectSuffix): ../src/ItemDefinitionEditor.cpp $(IntermediateDirectory)/up_src_ItemDefinitionEditor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/ItemDefinitionEditor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ItemDefinitionEditor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ItemDefinitionEditor.cpp$(DependSuffix): ../src/ItemDefinitionEditor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ItemDefinitionEditor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ItemDefinitionEditor.cpp$(DependSuffix) -MM ../src/ItemDefinitionEditor.cpp

$(IntermediateDirectory)/up_src_ItemDefinitionEditor.cpp$(PreprocessSuffix): ../src/ItemDefinitionEditor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ItemDefinitionEditor.cpp$(PreprocessSuffix) ../src/ItemDefinitionEditor.cpp

$(IntermediateDirectory)/up_src_BossGolem.cpp$(ObjectSuffix): ../src/BossGolem.cpp $(IntermediateDirectory)/up_src_BossGolem.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/BossGolem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_BossGolem.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_BossGolem.cpp$(DependSuffix): ../src/BossGolem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_BossGolem.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_BossGolem.cpp$(DependSuffix) -MM ../src/BossGolem.cpp

$(IntermediateDirectory)/up_src_BossGolem.cpp$(PreprocessSuffix): ../src/BossGolem.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_BossGolem.cpp$(PreprocessSuffix) ../src/BossGolem.cpp

$(IntermediateDirectory)/up_src_NaturalCaveGenerator.cpp$(ObjectSuffix): ../src/NaturalCaveGenerator.cpp $(IntermediateDirectory)/up_src_NaturalCaveGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/NaturalCaveGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_NaturalCaveGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_NaturalCaveGenerator.cpp$(DependSuffix): ../src/NaturalCaveGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_NaturalCaveGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_NaturalCaveGenerator.cpp$(DependSuffix) -MM ../src/NaturalCaveGenerator.cpp

$(IntermediateDirectory)/up_src_NaturalCaveGenerator.cpp$(PreprocessSuffix): ../src/NaturalCaveGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_NaturalCaveGenerator.cpp$(PreprocessSuffix) ../src/NaturalCaveGenerator.cpp

$(IntermediateDirectory)/up_src_LocationMap.cpp$(ObjectSuffix): ../src/LocationMap.cpp $(IntermediateDirectory)/up_src_LocationMap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/LocationMap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_LocationMap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_LocationMap.cpp$(DependSuffix): ../src/LocationMap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_LocationMap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_LocationMap.cpp$(DependSuffix) -MM ../src/LocationMap.cpp

$(IntermediateDirectory)/up_src_LocationMap.cpp$(PreprocessSuffix): ../src/LocationMap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_LocationMap.cpp$(PreprocessSuffix) ../src/LocationMap.cpp

$(IntermediateDirectory)/up_src_ChestGenerator.cpp$(ObjectSuffix): ../src/ChestGenerator.cpp $(IntermediateDirectory)/up_src_ChestGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/ChestGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ChestGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ChestGenerator.cpp$(DependSuffix): ../src/ChestGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ChestGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ChestGenerator.cpp$(DependSuffix) -MM ../src/ChestGenerator.cpp

$(IntermediateDirectory)/up_src_ChestGenerator.cpp$(PreprocessSuffix): ../src/ChestGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ChestGenerator.cpp$(PreprocessSuffix) ../src/ChestGenerator.cpp

$(IntermediateDirectory)/up_src_ImageSelector.cpp$(ObjectSuffix): ../src/ImageSelector.cpp $(IntermediateDirectory)/up_src_ImageSelector.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/ImageSelector.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ImageSelector.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ImageSelector.cpp$(DependSuffix): ../src/ImageSelector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ImageSelector.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ImageSelector.cpp$(DependSuffix) -MM ../src/ImageSelector.cpp

$(IntermediateDirectory)/up_src_ImageSelector.cpp$(PreprocessSuffix): ../src/ImageSelector.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ImageSelector.cpp$(PreprocessSuffix) ../src/ImageSelector.cpp

$(IntermediateDirectory)/up_src_LocationGenerator.cpp$(ObjectSuffix): ../src/LocationGenerator.cpp $(IntermediateDirectory)/up_src_LocationGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/LocationGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_LocationGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_LocationGenerator.cpp$(DependSuffix): ../src/LocationGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_LocationGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_LocationGenerator.cpp$(DependSuffix) -MM ../src/LocationGenerator.cpp

$(IntermediateDirectory)/up_src_LocationGenerator.cpp$(PreprocessSuffix): ../src/LocationGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_LocationGenerator.cpp$(PreprocessSuffix) ../src/LocationGenerator.cpp

$(IntermediateDirectory)/up_src_Ladder.cpp$(ObjectSuffix): ../src/Ladder.cpp $(IntermediateDirectory)/up_src_Ladder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Ladder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Ladder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Ladder.cpp$(DependSuffix): ../src/Ladder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Ladder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Ladder.cpp$(DependSuffix) -MM ../src/Ladder.cpp

$(IntermediateDirectory)/up_src_Ladder.cpp$(PreprocessSuffix): ../src/Ladder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Ladder.cpp$(PreprocessSuffix) ../src/Ladder.cpp

$(IntermediateDirectory)/up_src_Room.cpp$(ObjectSuffix): ../src/Room.cpp $(IntermediateDirectory)/up_src_Room.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Room.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Room.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Room.cpp$(DependSuffix): ../src/Room.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Room.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Room.cpp$(DependSuffix) -MM ../src/Room.cpp

$(IntermediateDirectory)/up_src_Room.cpp$(PreprocessSuffix): ../src/Room.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Room.cpp$(PreprocessSuffix) ../src/Room.cpp

$(IntermediateDirectory)/up_src_WorldOutliner.cpp$(ObjectSuffix): ../src/WorldOutliner.cpp $(IntermediateDirectory)/up_src_WorldOutliner.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/WorldOutliner.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_WorldOutliner.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_WorldOutliner.cpp$(DependSuffix): ../src/WorldOutliner.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_WorldOutliner.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_WorldOutliner.cpp$(DependSuffix) -MM ../src/WorldOutliner.cpp

$(IntermediateDirectory)/up_src_WorldOutliner.cpp$(PreprocessSuffix): ../src/WorldOutliner.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_WorldOutliner.cpp$(PreprocessSuffix) ../src/WorldOutliner.cpp

$(IntermediateDirectory)/up_src_Chest.cpp$(ObjectSuffix): ../src/Chest.cpp $(IntermediateDirectory)/up_src_Chest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Chest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Chest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Chest.cpp$(DependSuffix): ../src/Chest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Chest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Chest.cpp$(DependSuffix) -MM ../src/Chest.cpp

$(IntermediateDirectory)/up_src_Chest.cpp$(PreprocessSuffix): ../src/Chest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Chest.cpp$(PreprocessSuffix) ../src/Chest.cpp

$(IntermediateDirectory)/up_src_InteractionMenu.cpp$(ObjectSuffix): ../src/InteractionMenu.cpp $(IntermediateDirectory)/up_src_InteractionMenu.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/InteractionMenu.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_InteractionMenu.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_InteractionMenu.cpp$(DependSuffix): ../src/InteractionMenu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_InteractionMenu.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_InteractionMenu.cpp$(DependSuffix) -MM ../src/InteractionMenu.cpp

$(IntermediateDirectory)/up_src_InteractionMenu.cpp$(PreprocessSuffix): ../src/InteractionMenu.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_InteractionMenu.cpp$(PreprocessSuffix) ../src/InteractionMenu.cpp

$(IntermediateDirectory)/up_src_Slot.cpp$(ObjectSuffix): ../src/Slot.cpp $(IntermediateDirectory)/up_src_Slot.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Slot.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Slot.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Slot.cpp$(DependSuffix): ../src/Slot.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Slot.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Slot.cpp$(DependSuffix) -MM ../src/Slot.cpp

$(IntermediateDirectory)/up_src_Slot.cpp$(PreprocessSuffix): ../src/Slot.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Slot.cpp$(PreprocessSuffix) ../src/Slot.cpp

$(IntermediateDirectory)/up_src_LocationType.cpp$(ObjectSuffix): ../src/LocationType.cpp $(IntermediateDirectory)/up_src_LocationType.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/LocationType.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_LocationType.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_LocationType.cpp$(DependSuffix): ../src/LocationType.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_LocationType.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_LocationType.cpp$(DependSuffix) -MM ../src/LocationType.cpp

$(IntermediateDirectory)/up_src_LocationType.cpp$(PreprocessSuffix): ../src/LocationType.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_LocationType.cpp$(PreprocessSuffix) ../src/LocationType.cpp

$(IntermediateDirectory)/up_src_FrameAnimation.cpp$(ObjectSuffix): ../src/FrameAnimation.cpp $(IntermediateDirectory)/up_src_FrameAnimation.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/FrameAnimation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_FrameAnimation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_FrameAnimation.cpp$(DependSuffix): ../src/FrameAnimation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_FrameAnimation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_FrameAnimation.cpp$(DependSuffix) -MM ../src/FrameAnimation.cpp

$(IntermediateDirectory)/up_src_FrameAnimation.cpp$(PreprocessSuffix): ../src/FrameAnimation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_FrameAnimation.cpp$(PreprocessSuffix) ../src/FrameAnimation.cpp

$(IntermediateDirectory)/up_src_SetSpriteColorCommand.cpp$(ObjectSuffix): ../src/SetSpriteColorCommand.cpp $(IntermediateDirectory)/up_src_SetSpriteColorCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SetSpriteColorCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SetSpriteColorCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SetSpriteColorCommand.cpp$(DependSuffix): ../src/SetSpriteColorCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SetSpriteColorCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SetSpriteColorCommand.cpp$(DependSuffix) -MM ../src/SetSpriteColorCommand.cpp

$(IntermediateDirectory)/up_src_SetSpriteColorCommand.cpp$(PreprocessSuffix): ../src/SetSpriteColorCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SetSpriteColorCommand.cpp$(PreprocessSuffix) ../src/SetSpriteColorCommand.cpp

$(IntermediateDirectory)/up_src_ViewTriangle.cpp$(ObjectSuffix): ../src/ViewTriangle.cpp $(IntermediateDirectory)/up_src_ViewTriangle.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/ViewTriangle.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ViewTriangle.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ViewTriangle.cpp$(DependSuffix): ../src/ViewTriangle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ViewTriangle.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ViewTriangle.cpp$(DependSuffix) -MM ../src/ViewTriangle.cpp

$(IntermediateDirectory)/up_src_ViewTriangle.cpp$(PreprocessSuffix): ../src/ViewTriangle.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ViewTriangle.cpp$(PreprocessSuffix) ../src/ViewTriangle.cpp

$(IntermediateDirectory)/up_src_Item.cpp$(ObjectSuffix): ../src/Item.cpp $(IntermediateDirectory)/up_src_Item.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Item.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Item.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Item.cpp$(DependSuffix): ../src/Item.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Item.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Item.cpp$(DependSuffix) -MM ../src/Item.cpp

$(IntermediateDirectory)/up_src_Item.cpp$(PreprocessSuffix): ../src/Item.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Item.cpp$(PreprocessSuffix) ../src/Item.cpp

$(IntermediateDirectory)/up_src_AttributeGrid.cpp$(ObjectSuffix): ../src/AttributeGrid.cpp $(IntermediateDirectory)/up_src_AttributeGrid.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/AttributeGrid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_AttributeGrid.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_AttributeGrid.cpp$(DependSuffix): ../src/AttributeGrid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_AttributeGrid.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_AttributeGrid.cpp$(DependSuffix) -MM ../src/AttributeGrid.cpp

$(IntermediateDirectory)/up_src_AttributeGrid.cpp$(PreprocessSuffix): ../src/AttributeGrid.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_AttributeGrid.cpp$(PreprocessSuffix) ../src/AttributeGrid.cpp

$(IntermediateDirectory)/up_src_Pickaxe.cpp$(ObjectSuffix): ../src/Pickaxe.cpp $(IntermediateDirectory)/up_src_Pickaxe.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Pickaxe.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Pickaxe.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Pickaxe.cpp$(DependSuffix): ../src/Pickaxe.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Pickaxe.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Pickaxe.cpp$(DependSuffix) -MM ../src/Pickaxe.cpp

$(IntermediateDirectory)/up_src_Pickaxe.cpp$(PreprocessSuffix): ../src/Pickaxe.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Pickaxe.cpp$(PreprocessSuffix) ../src/Pickaxe.cpp

$(IntermediateDirectory)/up_src_Asset.cpp$(ObjectSuffix): ../src/Asset.cpp $(IntermediateDirectory)/up_src_Asset.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Asset.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Asset.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Asset.cpp$(DependSuffix): ../src/Asset.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Asset.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Asset.cpp$(DependSuffix) -MM ../src/Asset.cpp

$(IntermediateDirectory)/up_src_Asset.cpp$(PreprocessSuffix): ../src/Asset.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Asset.cpp$(PreprocessSuffix) ../src/Asset.cpp

$(IntermediateDirectory)/up_src_Timeline.cpp$(ObjectSuffix): ../src/Timeline.cpp $(IntermediateDirectory)/up_src_Timeline.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Timeline.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Timeline.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Timeline.cpp$(DependSuffix): ../src/Timeline.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Timeline.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Timeline.cpp$(DependSuffix) -MM ../src/Timeline.cpp

$(IntermediateDirectory)/up_src_Timeline.cpp$(PreprocessSuffix): ../src/Timeline.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Timeline.cpp$(PreprocessSuffix) ../src/Timeline.cpp

$(IntermediateDirectory)/up_src_SelectInteractionMode.cpp$(ObjectSuffix): ../src/SelectInteractionMode.cpp $(IntermediateDirectory)/up_src_SelectInteractionMode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SelectInteractionMode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SelectInteractionMode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SelectInteractionMode.cpp$(DependSuffix): ../src/SelectInteractionMode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SelectInteractionMode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SelectInteractionMode.cpp$(DependSuffix) -MM ../src/SelectInteractionMode.cpp

$(IntermediateDirectory)/up_src_SelectInteractionMode.cpp$(PreprocessSuffix): ../src/SelectInteractionMode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SelectInteractionMode.cpp$(PreprocessSuffix) ../src/SelectInteractionMode.cpp

$(IntermediateDirectory)/up_src_Door.cpp$(ObjectSuffix): ../src/Door.cpp $(IntermediateDirectory)/up_src_Door.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Door.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Door.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Door.cpp$(DependSuffix): ../src/Door.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Door.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Door.cpp$(DependSuffix) -MM ../src/Door.cpp

$(IntermediateDirectory)/up_src_Door.cpp$(PreprocessSuffix): ../src/Door.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Door.cpp$(PreprocessSuffix) ../src/Door.cpp

$(IntermediateDirectory)/up_src_DebrisAtlas.cpp$(ObjectSuffix): ../src/DebrisAtlas.cpp $(IntermediateDirectory)/up_src_DebrisAtlas.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/DebrisAtlas.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_DebrisAtlas.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_DebrisAtlas.cpp$(DependSuffix): ../src/DebrisAtlas.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_DebrisAtlas.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_DebrisAtlas.cpp$(DependSuffix) -MM ../src/DebrisAtlas.cpp

$(IntermediateDirectory)/up_src_DebrisAtlas.cpp$(PreprocessSuffix): ../src/DebrisAtlas.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_DebrisAtlas.cpp$(PreprocessSuffix) ../src/DebrisAtlas.cpp

$(IntermediateDirectory)/up_src_Tooltip.cpp$(ObjectSuffix): ../src/Tooltip.cpp $(IntermediateDirectory)/up_src_Tooltip.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Tooltip.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Tooltip.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Tooltip.cpp$(DependSuffix): ../src/Tooltip.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Tooltip.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Tooltip.cpp$(DependSuffix) -MM ../src/Tooltip.cpp

$(IntermediateDirectory)/up_src_Tooltip.cpp$(PreprocessSuffix): ../src/Tooltip.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Tooltip.cpp$(PreprocessSuffix) ../src/Tooltip.cpp

$(IntermediateDirectory)/up_src_NumericUpDown.cpp$(ObjectSuffix): ../src/NumericUpDown.cpp $(IntermediateDirectory)/up_src_NumericUpDown.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/NumericUpDown.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_NumericUpDown.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_NumericUpDown.cpp$(DependSuffix): ../src/NumericUpDown.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_NumericUpDown.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_NumericUpDown.cpp$(DependSuffix) -MM ../src/NumericUpDown.cpp

$(IntermediateDirectory)/up_src_NumericUpDown.cpp$(PreprocessSuffix): ../src/NumericUpDown.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_NumericUpDown.cpp$(PreprocessSuffix) ../src/NumericUpDown.cpp

$(IntermediateDirectory)/up_src_Passage.cpp$(ObjectSuffix): ../src/Passage.cpp $(IntermediateDirectory)/up_src_Passage.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Passage.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Passage.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Passage.cpp$(DependSuffix): ../src/Passage.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Passage.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Passage.cpp$(DependSuffix) -MM ../src/Passage.cpp

$(IntermediateDirectory)/up_src_Passage.cpp$(PreprocessSuffix): ../src/Passage.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Passage.cpp$(PreprocessSuffix) ../src/Passage.cpp

$(IntermediateDirectory)/up_src_WorldGenerator.cpp$(ObjectSuffix): ../src/WorldGenerator.cpp $(IntermediateDirectory)/up_src_WorldGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/WorldGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_WorldGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_WorldGenerator.cpp$(DependSuffix): ../src/WorldGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_WorldGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_WorldGenerator.cpp$(DependSuffix) -MM ../src/WorldGenerator.cpp

$(IntermediateDirectory)/up_src_WorldGenerator.cpp$(PreprocessSuffix): ../src/WorldGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_WorldGenerator.cpp$(PreprocessSuffix) ../src/WorldGenerator.cpp

$(IntermediateDirectory)/up_src_Monster.cpp$(ObjectSuffix): ../src/Monster.cpp $(IntermediateDirectory)/up_src_Monster.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Monster.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Monster.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Monster.cpp$(DependSuffix): ../src/Monster.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Monster.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Monster.cpp$(DependSuffix) -MM ../src/Monster.cpp

$(IntermediateDirectory)/up_src_Monster.cpp$(PreprocessSuffix): ../src/Monster.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Monster.cpp$(PreprocessSuffix) ../src/Monster.cpp

$(IntermediateDirectory)/up_src_InteractionMode.cpp$(ObjectSuffix): ../src/InteractionMode.cpp $(IntermediateDirectory)/up_src_InteractionMode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/InteractionMode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_InteractionMode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_InteractionMode.cpp$(DependSuffix): ../src/InteractionMode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_InteractionMode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_InteractionMode.cpp$(DependSuffix) -MM ../src/InteractionMode.cpp

$(IntermediateDirectory)/up_src_InteractionMode.cpp$(PreprocessSuffix): ../src/InteractionMode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_InteractionMode.cpp$(PreprocessSuffix) ../src/InteractionMode.cpp

$(IntermediateDirectory)/up_src_CellPixel.cpp$(ObjectSuffix): ../src/CellPixel.cpp $(IntermediateDirectory)/up_src_CellPixel.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/CellPixel.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_CellPixel.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_CellPixel.cpp$(DependSuffix): ../src/CellPixel.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_CellPixel.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_CellPixel.cpp$(DependSuffix) -MM ../src/CellPixel.cpp

$(IntermediateDirectory)/up_src_CellPixel.cpp$(PreprocessSuffix): ../src/CellPixel.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_CellPixel.cpp$(PreprocessSuffix) ../src/CellPixel.cpp

$(IntermediateDirectory)/up_src_AcidGun.cpp$(ObjectSuffix): ../src/AcidGun.cpp $(IntermediateDirectory)/up_src_AcidGun.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/AcidGun.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_AcidGun.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_AcidGun.cpp$(DependSuffix): ../src/AcidGun.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_AcidGun.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_AcidGun.cpp$(DependSuffix) -MM ../src/AcidGun.cpp

$(IntermediateDirectory)/up_src_AcidGun.cpp$(PreprocessSuffix): ../src/AcidGun.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_AcidGun.cpp$(PreprocessSuffix) ../src/AcidGun.cpp

$(IntermediateDirectory)/up_src_TranslatedStringEditor.cpp$(ObjectSuffix): ../src/TranslatedStringEditor.cpp $(IntermediateDirectory)/up_src_TranslatedStringEditor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/TranslatedStringEditor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_TranslatedStringEditor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_TranslatedStringEditor.cpp$(DependSuffix): ../src/TranslatedStringEditor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_TranslatedStringEditor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_TranslatedStringEditor.cpp$(DependSuffix) -MM ../src/TranslatedStringEditor.cpp

$(IntermediateDirectory)/up_src_TranslatedStringEditor.cpp$(PreprocessSuffix): ../src/TranslatedStringEditor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_TranslatedStringEditor.cpp$(PreprocessSuffix) ../src/TranslatedStringEditor.cpp

$(IntermediateDirectory)/up_src_CaveGenerator.cpp$(ObjectSuffix): ../src/CaveGenerator.cpp $(IntermediateDirectory)/up_src_CaveGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/CaveGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_CaveGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_CaveGenerator.cpp$(DependSuffix): ../src/CaveGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_CaveGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_CaveGenerator.cpp$(DependSuffix) -MM ../src/CaveGenerator.cpp

$(IntermediateDirectory)/up_src_CaveGenerator.cpp$(PreprocessSuffix): ../src/CaveGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_CaveGenerator.cpp$(PreprocessSuffix) ../src/CaveGenerator.cpp

$(IntermediateDirectory)/up_src_SetTrackKeyframeCommand.cpp$(ObjectSuffix): ../src/SetTrackKeyframeCommand.cpp $(IntermediateDirectory)/up_src_SetTrackKeyframeCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SetTrackKeyframeCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SetTrackKeyframeCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SetTrackKeyframeCommand.cpp$(DependSuffix): ../src/SetTrackKeyframeCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SetTrackKeyframeCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SetTrackKeyframeCommand.cpp$(DependSuffix) -MM ../src/SetTrackKeyframeCommand.cpp

$(IntermediateDirectory)/up_src_SetTrackKeyframeCommand.cpp$(PreprocessSuffix): ../src/SetTrackKeyframeCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SetTrackKeyframeCommand.cpp$(PreprocessSuffix) ../src/SetTrackKeyframeCommand.cpp

$(IntermediateDirectory)/up_src_SettlementGenerator.cpp$(ObjectSuffix): ../src/SettlementGenerator.cpp $(IntermediateDirectory)/up_src_SettlementGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SettlementGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SettlementGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SettlementGenerator.cpp$(DependSuffix): ../src/SettlementGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SettlementGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SettlementGenerator.cpp$(DependSuffix) -MM ../src/SettlementGenerator.cpp

$(IntermediateDirectory)/up_src_SettlementGenerator.cpp$(PreprocessSuffix): ../src/SettlementGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SettlementGenerator.cpp$(PreprocessSuffix) ../src/SettlementGenerator.cpp

$(IntermediateDirectory)/up_src_UndergroundSettlementGenerator.cpp$(ObjectSuffix): ../src/UndergroundSettlementGenerator.cpp $(IntermediateDirectory)/up_src_UndergroundSettlementGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/UndergroundSettlementGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_UndergroundSettlementGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_UndergroundSettlementGenerator.cpp$(DependSuffix): ../src/UndergroundSettlementGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_UndergroundSettlementGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_UndergroundSettlementGenerator.cpp$(DependSuffix) -MM ../src/UndergroundSettlementGenerator.cpp

$(IntermediateDirectory)/up_src_UndergroundSettlementGenerator.cpp$(PreprocessSuffix): ../src/UndergroundSettlementGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_UndergroundSettlementGenerator.cpp$(PreprocessSuffix) ../src/UndergroundSettlementGenerator.cpp

$(IntermediateDirectory)/up_src_BankGenerator.cpp$(ObjectSuffix): ../src/BankGenerator.cpp $(IntermediateDirectory)/up_src_BankGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/BankGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_BankGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_BankGenerator.cpp$(DependSuffix): ../src/BankGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_BankGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_BankGenerator.cpp$(DependSuffix) -MM ../src/BankGenerator.cpp

$(IntermediateDirectory)/up_src_BankGenerator.cpp$(PreprocessSuffix): ../src/BankGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_BankGenerator.cpp$(PreprocessSuffix) ../src/BankGenerator.cpp

$(IntermediateDirectory)/up_src_PerlinNoise.cpp$(ObjectSuffix): ../src/PerlinNoise.cpp $(IntermediateDirectory)/up_src_PerlinNoise.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/PerlinNoise.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_PerlinNoise.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_PerlinNoise.cpp$(DependSuffix): ../src/PerlinNoise.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_PerlinNoise.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_PerlinNoise.cpp$(DependSuffix) -MM ../src/PerlinNoise.cpp

$(IntermediateDirectory)/up_src_PerlinNoise.cpp$(PreprocessSuffix): ../src/PerlinNoise.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_PerlinNoise.cpp$(PreprocessSuffix) ../src/PerlinNoise.cpp

$(IntermediateDirectory)/up_src_Canvas.cpp$(ObjectSuffix): ../src/Canvas.cpp $(IntermediateDirectory)/up_src_Canvas.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Canvas.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Canvas.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Canvas.cpp$(DependSuffix): ../src/Canvas.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Canvas.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Canvas.cpp$(DependSuffix) -MM ../src/Canvas.cpp

$(IntermediateDirectory)/up_src_Canvas.cpp$(PreprocessSuffix): ../src/Canvas.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Canvas.cpp$(PreprocessSuffix) ../src/Canvas.cpp

$(IntermediateDirectory)/up_src_MoveOriginInteractionMode.cpp$(ObjectSuffix): ../src/MoveOriginInteractionMode.cpp $(IntermediateDirectory)/up_src_MoveOriginInteractionMode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/MoveOriginInteractionMode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_MoveOriginInteractionMode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_MoveOriginInteractionMode.cpp$(DependSuffix): ../src/MoveOriginInteractionMode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_MoveOriginInteractionMode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_MoveOriginInteractionMode.cpp$(DependSuffix) -MM ../src/MoveOriginInteractionMode.cpp

$(IntermediateDirectory)/up_src_MoveOriginInteractionMode.cpp$(PreprocessSuffix): ../src/MoveOriginInteractionMode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_MoveOriginInteractionMode.cpp$(PreprocessSuffix) ../src/MoveOriginInteractionMode.cpp

$(IntermediateDirectory)/up_src_SoilGun.cpp$(ObjectSuffix): ../src/SoilGun.cpp $(IntermediateDirectory)/up_src_SoilGun.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SoilGun.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SoilGun.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SoilGun.cpp$(DependSuffix): ../src/SoilGun.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SoilGun.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SoilGun.cpp$(DependSuffix) -MM ../src/SoilGun.cpp

$(IntermediateDirectory)/up_src_SoilGun.cpp$(PreprocessSuffix): ../src/SoilGun.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SoilGun.cpp$(PreprocessSuffix) ../src/SoilGun.cpp

$(IntermediateDirectory)/up_src_CreateSpriteCommand.cpp$(ObjectSuffix): ../src/CreateSpriteCommand.cpp $(IntermediateDirectory)/up_src_CreateSpriteCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/CreateSpriteCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_CreateSpriteCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_CreateSpriteCommand.cpp$(DependSuffix): ../src/CreateSpriteCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_CreateSpriteCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_CreateSpriteCommand.cpp$(DependSuffix) -MM ../src/CreateSpriteCommand.cpp

$(IntermediateDirectory)/up_src_CreateSpriteCommand.cpp$(PreprocessSuffix): ../src/CreateSpriteCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_CreateSpriteCommand.cpp$(PreprocessSuffix) ../src/CreateSpriteCommand.cpp

$(IntermediateDirectory)/up_src_PaintTerrainInteractionMode.cpp$(ObjectSuffix): ../src/PaintTerrainInteractionMode.cpp $(IntermediateDirectory)/up_src_PaintTerrainInteractionMode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/PaintTerrainInteractionMode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_PaintTerrainInteractionMode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_PaintTerrainInteractionMode.cpp$(DependSuffix): ../src/PaintTerrainInteractionMode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_PaintTerrainInteractionMode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_PaintTerrainInteractionMode.cpp$(DependSuffix) -MM ../src/PaintTerrainInteractionMode.cpp

$(IntermediateDirectory)/up_src_PaintTerrainInteractionMode.cpp$(PreprocessSuffix): ../src/PaintTerrainInteractionMode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_PaintTerrainInteractionMode.cpp$(PreprocessSuffix) ../src/PaintTerrainInteractionMode.cpp

$(IntermediateDirectory)/up_src_PlasmaDrill.cpp$(ObjectSuffix): ../src/PlasmaDrill.cpp $(IntermediateDirectory)/up_src_PlasmaDrill.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/PlasmaDrill.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_PlasmaDrill.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_PlasmaDrill.cpp$(DependSuffix): ../src/PlasmaDrill.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_PlasmaDrill.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_PlasmaDrill.cpp$(DependSuffix) -MM ../src/PlasmaDrill.cpp

$(IntermediateDirectory)/up_src_PlasmaDrill.cpp$(PreprocessSuffix): ../src/PlasmaDrill.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_PlasmaDrill.cpp$(PreprocessSuffix) ../src/PlasmaDrill.cpp

$(IntermediateDirectory)/up_src_Controls.cpp$(ObjectSuffix): ../src/Controls.cpp $(IntermediateDirectory)/up_src_Controls.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Controls.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Controls.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Controls.cpp$(DependSuffix): ../src/Controls.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Controls.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Controls.cpp$(DependSuffix) -MM ../src/Controls.cpp

$(IntermediateDirectory)/up_src_Controls.cpp$(PreprocessSuffix): ../src/Controls.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Controls.cpp$(PreprocessSuffix) ../src/Controls.cpp

$(IntermediateDirectory)/up_src_LightningGun.cpp$(ObjectSuffix): ../src/LightningGun.cpp $(IntermediateDirectory)/up_src_LightningGun.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/LightningGun.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_LightningGun.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_LightningGun.cpp$(DependSuffix): ../src/LightningGun.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_LightningGun.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_LightningGun.cpp$(DependSuffix) -MM ../src/LightningGun.cpp

$(IntermediateDirectory)/up_src_LightningGun.cpp$(PreprocessSuffix): ../src/LightningGun.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_LightningGun.cpp$(PreprocessSuffix) ../src/LightningGun.cpp

$(IntermediateDirectory)/up_src_Shield.cpp$(ObjectSuffix): ../src/Shield.cpp $(IntermediateDirectory)/up_src_Shield.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Shield.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Shield.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Shield.cpp$(DependSuffix): ../src/Shield.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Shield.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Shield.cpp$(DependSuffix) -MM ../src/Shield.cpp

$(IntermediateDirectory)/up_src_Shield.cpp$(PreprocessSuffix): ../src/Shield.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Shield.cpp$(PreprocessSuffix) ../src/Shield.cpp

$(IntermediateDirectory)/up_src_StatWindow.cpp$(ObjectSuffix): ../src/StatWindow.cpp $(IntermediateDirectory)/up_src_StatWindow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/StatWindow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_StatWindow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_StatWindow.cpp$(DependSuffix): ../src/StatWindow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_StatWindow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_StatWindow.cpp$(DependSuffix) -MM ../src/StatWindow.cpp

$(IntermediateDirectory)/up_src_StatWindow.cpp$(PreprocessSuffix): ../src/StatWindow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_StatWindow.cpp$(PreprocessSuffix) ../src/StatWindow.cpp

$(IntermediateDirectory)/up_src_DialogEditor.cpp$(ObjectSuffix): ../src/DialogEditor.cpp $(IntermediateDirectory)/up_src_DialogEditor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/DialogEditor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_DialogEditor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_DialogEditor.cpp$(DependSuffix): ../src/DialogEditor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_DialogEditor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_DialogEditor.cpp$(DependSuffix) -MM ../src/DialogEditor.cpp

$(IntermediateDirectory)/up_src_DialogEditor.cpp$(PreprocessSuffix): ../src/DialogEditor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_DialogEditor.cpp$(PreprocessSuffix) ../src/DialogEditor.cpp

$(IntermediateDirectory)/up_src_Guid.cpp$(ObjectSuffix): ../src/Guid.cpp $(IntermediateDirectory)/up_src_Guid.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Guid.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Guid.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Guid.cpp$(DependSuffix): ../src/Guid.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Guid.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Guid.cpp$(DependSuffix) -MM ../src/Guid.cpp

$(IntermediateDirectory)/up_src_Guid.cpp$(PreprocessSuffix): ../src/Guid.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Guid.cpp$(PreprocessSuffix) ../src/Guid.cpp

$(IntermediateDirectory)/up_src_Command.cpp$(ObjectSuffix): ../src/Command.cpp $(IntermediateDirectory)/up_src_Command.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Command.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Command.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Command.cpp$(DependSuffix): ../src/Command.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Command.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Command.cpp$(DependSuffix) -MM ../src/Command.cpp

$(IntermediateDirectory)/up_src_Command.cpp$(PreprocessSuffix): ../src/Command.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Command.cpp$(PreprocessSuffix) ../src/Command.cpp

$(IntermediateDirectory)/up_src_Widget.cpp$(ObjectSuffix): ../src/Widget.cpp $(IntermediateDirectory)/up_src_Widget.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Widget.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Widget.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Widget.cpp$(DependSuffix): ../src/Widget.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Widget.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Widget.cpp$(DependSuffix) -MM ../src/Widget.cpp

$(IntermediateDirectory)/up_src_Widget.cpp$(PreprocessSuffix): ../src/Widget.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Widget.cpp$(PreprocessSuffix) ../src/Widget.cpp

$(IntermediateDirectory)/up_src_SetSpriteSizeCommand.cpp$(ObjectSuffix): ../src/SetSpriteSizeCommand.cpp $(IntermediateDirectory)/up_src_SetSpriteSizeCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SetSpriteSizeCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SetSpriteSizeCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SetSpriteSizeCommand.cpp$(DependSuffix): ../src/SetSpriteSizeCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SetSpriteSizeCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SetSpriteSizeCommand.cpp$(DependSuffix) -MM ../src/SetSpriteSizeCommand.cpp

$(IntermediateDirectory)/up_src_SetSpriteSizeCommand.cpp$(PreprocessSuffix): ../src/SetSpriteSizeCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SetSpriteSizeCommand.cpp$(PreprocessSuffix) ../src/SetSpriteSizeCommand.cpp

$(IntermediateDirectory)/up_src_CreateAnimationCommand.cpp$(ObjectSuffix): ../src/CreateAnimationCommand.cpp $(IntermediateDirectory)/up_src_CreateAnimationCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/CreateAnimationCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_CreateAnimationCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_CreateAnimationCommand.cpp$(DependSuffix): ../src/CreateAnimationCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_CreateAnimationCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_CreateAnimationCommand.cpp$(DependSuffix) -MM ../src/CreateAnimationCommand.cpp

$(IntermediateDirectory)/up_src_CreateAnimationCommand.cpp$(PreprocessSuffix): ../src/CreateAnimationCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_CreateAnimationCommand.cpp$(PreprocessSuffix) ../src/CreateAnimationCommand.cpp

$(IntermediateDirectory)/up_src_RotateInteractionMode.cpp$(ObjectSuffix): ../src/RotateInteractionMode.cpp $(IntermediateDirectory)/up_src_RotateInteractionMode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/RotateInteractionMode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_RotateInteractionMode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_RotateInteractionMode.cpp$(DependSuffix): ../src/RotateInteractionMode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_RotateInteractionMode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_RotateInteractionMode.cpp$(DependSuffix) -MM ../src/RotateInteractionMode.cpp

$(IntermediateDirectory)/up_src_RotateInteractionMode.cpp$(PreprocessSuffix): ../src/RotateInteractionMode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_RotateInteractionMode.cpp$(PreprocessSuffix) ../src/RotateInteractionMode.cpp

$(IntermediateDirectory)/up_src_ModifyTerrainCommand.cpp$(ObjectSuffix): ../src/ModifyTerrainCommand.cpp $(IntermediateDirectory)/up_src_ModifyTerrainCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/ModifyTerrainCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ModifyTerrainCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ModifyTerrainCommand.cpp$(DependSuffix): ../src/ModifyTerrainCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ModifyTerrainCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ModifyTerrainCommand.cpp$(DependSuffix) -MM ../src/ModifyTerrainCommand.cpp

$(IntermediateDirectory)/up_src_ModifyTerrainCommand.cpp$(PreprocessSuffix): ../src/ModifyTerrainCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ModifyTerrainCommand.cpp$(PreprocessSuffix) ../src/ModifyTerrainCommand.cpp

$(IntermediateDirectory)/up_src_Trigger.cpp$(ObjectSuffix): ../src/Trigger.cpp $(IntermediateDirectory)/up_src_Trigger.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Trigger.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Trigger.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Trigger.cpp$(DependSuffix): ../src/Trigger.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Trigger.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Trigger.cpp$(DependSuffix) -MM ../src/Trigger.cpp

$(IntermediateDirectory)/up_src_Trigger.cpp$(PreprocessSuffix): ../src/Trigger.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Trigger.cpp$(PreprocessSuffix) ../src/Trigger.cpp

$(IntermediateDirectory)/up_src_WarehouseGenerator.cpp$(ObjectSuffix): ../src/WarehouseGenerator.cpp $(IntermediateDirectory)/up_src_WarehouseGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/WarehouseGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_WarehouseGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_WarehouseGenerator.cpp$(DependSuffix): ../src/WarehouseGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_WarehouseGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_WarehouseGenerator.cpp$(DependSuffix) -MM ../src/WarehouseGenerator.cpp

$(IntermediateDirectory)/up_src_WarehouseGenerator.cpp$(PreprocessSuffix): ../src/WarehouseGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_WarehouseGenerator.cpp$(PreprocessSuffix) ../src/WarehouseGenerator.cpp

$(IntermediateDirectory)/up_src_ForceGun.cpp$(ObjectSuffix): ../src/ForceGun.cpp $(IntermediateDirectory)/up_src_ForceGun.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/ForceGun.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ForceGun.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ForceGun.cpp$(DependSuffix): ../src/ForceGun.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ForceGun.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ForceGun.cpp$(DependSuffix) -MM ../src/ForceGun.cpp

$(IntermediateDirectory)/up_src_ForceGun.cpp$(PreprocessSuffix): ../src/ForceGun.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ForceGun.cpp$(PreprocessSuffix) ../src/ForceGun.cpp

$(IntermediateDirectory)/up_src_PassageGenerator.cpp$(ObjectSuffix): ../src/PassageGenerator.cpp $(IntermediateDirectory)/up_src_PassageGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/PassageGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_PassageGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_PassageGenerator.cpp$(DependSuffix): ../src/PassageGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_PassageGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_PassageGenerator.cpp$(DependSuffix) -MM ../src/PassageGenerator.cpp

$(IntermediateDirectory)/up_src_PassageGenerator.cpp$(PreprocessSuffix): ../src/PassageGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_PassageGenerator.cpp$(PreprocessSuffix) ../src/PassageGenerator.cpp

$(IntermediateDirectory)/up_src_Location.cpp$(ObjectSuffix): ../src/Location.cpp $(IntermediateDirectory)/up_src_Location.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Location.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Location.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Location.cpp$(DependSuffix): ../src/Location.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Location.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Location.cpp$(DependSuffix) -MM ../src/Location.cpp

$(IntermediateDirectory)/up_src_Location.cpp$(PreprocessSuffix): ../src/Location.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Location.cpp$(PreprocessSuffix) ../src/Location.cpp

$(IntermediateDirectory)/up_src_ResourceDatabase.cpp$(ObjectSuffix): ../src/ResourceDatabase.cpp $(IntermediateDirectory)/up_src_ResourceDatabase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/ResourceDatabase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ResourceDatabase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ResourceDatabase.cpp$(DependSuffix): ../src/ResourceDatabase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ResourceDatabase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ResourceDatabase.cpp$(DependSuffix) -MM ../src/ResourceDatabase.cpp

$(IntermediateDirectory)/up_src_ResourceDatabase.cpp$(PreprocessSuffix): ../src/ResourceDatabase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ResourceDatabase.cpp$(PreprocessSuffix) ../src/ResourceDatabase.cpp

$(IntermediateDirectory)/up_src_SlideList.cpp$(ObjectSuffix): ../src/SlideList.cpp $(IntermediateDirectory)/up_src_SlideList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SlideList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SlideList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SlideList.cpp$(DependSuffix): ../src/SlideList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SlideList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SlideList.cpp$(DependSuffix) -MM ../src/SlideList.cpp

$(IntermediateDirectory)/up_src_SlideList.cpp$(PreprocessSuffix): ../src/SlideList.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SlideList.cpp$(PreprocessSuffix) ../src/SlideList.cpp

$(IntermediateDirectory)/up_src_Window.cpp$(ObjectSuffix): ../src/Window.cpp $(IntermediateDirectory)/up_src_Window.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Window.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Window.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Window.cpp$(DependSuffix): ../src/Window.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Window.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Window.cpp$(DependSuffix) -MM ../src/Window.cpp

$(IntermediateDirectory)/up_src_Window.cpp$(PreprocessSuffix): ../src/Window.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Window.cpp$(PreprocessSuffix) ../src/Window.cpp

$(IntermediateDirectory)/up_src_Label.cpp$(ObjectSuffix): ../src/Label.cpp $(IntermediateDirectory)/up_src_Label.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Label.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Label.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Label.cpp$(DependSuffix): ../src/Label.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Label.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Label.cpp$(DependSuffix) -MM ../src/Label.cpp

$(IntermediateDirectory)/up_src_Label.cpp$(PreprocessSuffix): ../src/Label.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Label.cpp$(PreprocessSuffix) ../src/Label.cpp

$(IntermediateDirectory)/up_src_QuadTree.cpp$(ObjectSuffix): ../src/QuadTree.cpp $(IntermediateDirectory)/up_src_QuadTree.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/QuadTree.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_QuadTree.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_QuadTree.cpp$(DependSuffix): ../src/QuadTree.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_QuadTree.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_QuadTree.cpp$(DependSuffix) -MM ../src/QuadTree.cpp

$(IntermediateDirectory)/up_src_QuadTree.cpp$(PreprocessSuffix): ../src/QuadTree.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_QuadTree.cpp$(PreprocessSuffix) ../src/QuadTree.cpp

$(IntermediateDirectory)/up_src_Weapon.cpp$(ObjectSuffix): ../src/Weapon.cpp $(IntermediateDirectory)/up_src_Weapon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Weapon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Weapon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Weapon.cpp$(DependSuffix): ../src/Weapon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Weapon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Weapon.cpp$(DependSuffix) -MM ../src/Weapon.cpp

$(IntermediateDirectory)/up_src_Weapon.cpp$(PreprocessSuffix): ../src/Weapon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Weapon.cpp$(PreprocessSuffix) ../src/Weapon.cpp

$(IntermediateDirectory)/up_src_Inventory.cpp$(ObjectSuffix): ../src/Inventory.cpp $(IntermediateDirectory)/up_src_Inventory.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Inventory.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Inventory.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Inventory.cpp$(DependSuffix): ../src/Inventory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Inventory.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Inventory.cpp$(DependSuffix) -MM ../src/Inventory.cpp

$(IntermediateDirectory)/up_src_Inventory.cpp$(PreprocessSuffix): ../src/Inventory.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Inventory.cpp$(PreprocessSuffix) ../src/Inventory.cpp

$(IntermediateDirectory)/up_src_Damage.cpp$(ObjectSuffix): ../src/Damage.cpp $(IntermediateDirectory)/up_src_Damage.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Damage.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Damage.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Damage.cpp$(DependSuffix): ../src/Damage.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Damage.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Damage.cpp$(DependSuffix) -MM ../src/Damage.cpp

$(IntermediateDirectory)/up_src_Damage.cpp$(PreprocessSuffix): ../src/Damage.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Damage.cpp$(PreprocessSuffix) ../src/Damage.cpp

$(IntermediateDirectory)/up_src_ConstructionGun.cpp$(ObjectSuffix): ../src/ConstructionGun.cpp $(IntermediateDirectory)/up_src_ConstructionGun.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/ConstructionGun.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ConstructionGun.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ConstructionGun.cpp$(DependSuffix): ../src/ConstructionGun.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ConstructionGun.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ConstructionGun.cpp$(DependSuffix) -MM ../src/ConstructionGun.cpp

$(IntermediateDirectory)/up_src_ConstructionGun.cpp$(PreprocessSuffix): ../src/ConstructionGun.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ConstructionGun.cpp$(PreprocessSuffix) ../src/ConstructionGun.cpp

$(IntermediateDirectory)/up_src_AnimationTrack.cpp$(ObjectSuffix): ../src/AnimationTrack.cpp $(IntermediateDirectory)/up_src_AnimationTrack.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/AnimationTrack.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_AnimationTrack.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_AnimationTrack.cpp$(DependSuffix): ../src/AnimationTrack.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_AnimationTrack.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_AnimationTrack.cpp$(DependSuffix) -MM ../src/AnimationTrack.cpp

$(IntermediateDirectory)/up_src_AnimationTrack.cpp$(PreprocessSuffix): ../src/AnimationTrack.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_AnimationTrack.cpp$(PreprocessSuffix) ../src/AnimationTrack.cpp

$(IntermediateDirectory)/up_src_Shotgun.cpp$(ObjectSuffix): ../src/Shotgun.cpp $(IntermediateDirectory)/up_src_Shotgun.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Shotgun.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Shotgun.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Shotgun.cpp$(DependSuffix): ../src/Shotgun.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Shotgun.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Shotgun.cpp$(DependSuffix) -MM ../src/Shotgun.cpp

$(IntermediateDirectory)/up_src_Shotgun.cpp$(PreprocessSuffix): ../src/Shotgun.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Shotgun.cpp$(PreprocessSuffix) ../src/Shotgun.cpp

$(IntermediateDirectory)/up_src_Serializer.cpp$(ObjectSuffix): ../src/Serializer.cpp $(IntermediateDirectory)/up_src_Serializer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Serializer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Serializer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Serializer.cpp$(DependSuffix): ../src/Serializer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Serializer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Serializer.cpp$(DependSuffix) -MM ../src/Serializer.cpp

$(IntermediateDirectory)/up_src_Serializer.cpp$(PreprocessSuffix): ../src/Serializer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Serializer.cpp$(PreprocessSuffix) ../src/Serializer.cpp

$(IntermediateDirectory)/up_src_MotherCrystal.cpp$(ObjectSuffix): ../src/MotherCrystal.cpp $(IntermediateDirectory)/up_src_MotherCrystal.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/MotherCrystal.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_MotherCrystal.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_MotherCrystal.cpp$(DependSuffix): ../src/MotherCrystal.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_MotherCrystal.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_MotherCrystal.cpp$(DependSuffix) -MM ../src/MotherCrystal.cpp

$(IntermediateDirectory)/up_src_MotherCrystal.cpp$(PreprocessSuffix): ../src/MotherCrystal.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_MotherCrystal.cpp$(PreprocessSuffix) ../src/MotherCrystal.cpp

$(IntermediateDirectory)/up_src_TextLine.cpp$(ObjectSuffix): ../src/TextLine.cpp $(IntermediateDirectory)/up_src_TextLine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/TextLine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_TextLine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_TextLine.cpp$(DependSuffix): ../src/TextLine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_TextLine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_TextLine.cpp$(DependSuffix) -MM ../src/TextLine.cpp

$(IntermediateDirectory)/up_src_TextLine.cpp$(PreprocessSuffix): ../src/TextLine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_TextLine.cpp$(PreprocessSuffix) ../src/TextLine.cpp

$(IntermediateDirectory)/up_src_HubGenerator.cpp$(ObjectSuffix): ../src/HubGenerator.cpp $(IntermediateDirectory)/up_src_HubGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/HubGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_HubGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_HubGenerator.cpp$(DependSuffix): ../src/HubGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_HubGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_HubGenerator.cpp$(DependSuffix) -MM ../src/HubGenerator.cpp

$(IntermediateDirectory)/up_src_HubGenerator.cpp$(PreprocessSuffix): ../src/HubGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_HubGenerator.cpp$(PreprocessSuffix) ../src/HubGenerator.cpp

$(IntermediateDirectory)/up_src_Encyclopedia.cpp$(ObjectSuffix): ../src/Encyclopedia.cpp $(IntermediateDirectory)/up_src_Encyclopedia.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Encyclopedia.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Encyclopedia.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Encyclopedia.cpp$(DependSuffix): ../src/Encyclopedia.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Encyclopedia.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Encyclopedia.cpp$(DependSuffix) -MM ../src/Encyclopedia.cpp

$(IntermediateDirectory)/up_src_Encyclopedia.cpp$(PreprocessSuffix): ../src/Encyclopedia.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Encyclopedia.cpp$(PreprocessSuffix) ../src/Encyclopedia.cpp

$(IntermediateDirectory)/up_src_RocketLauncher.cpp$(ObjectSuffix): ../src/RocketLauncher.cpp $(IntermediateDirectory)/up_src_RocketLauncher.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/RocketLauncher.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_RocketLauncher.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_RocketLauncher.cpp$(DependSuffix): ../src/RocketLauncher.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_RocketLauncher.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_RocketLauncher.cpp$(DependSuffix) -MM ../src/RocketLauncher.cpp

$(IntermediateDirectory)/up_src_RocketLauncher.cpp$(PreprocessSuffix): ../src/RocketLauncher.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_RocketLauncher.cpp$(PreprocessSuffix) ../src/RocketLauncher.cpp

$(IntermediateDirectory)/up_src_Journal.cpp$(ObjectSuffix): ../src/Journal.cpp $(IntermediateDirectory)/up_src_Journal.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Journal.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Journal.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Journal.cpp$(DependSuffix): ../src/Journal.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Journal.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Journal.cpp$(DependSuffix) -MM ../src/Journal.cpp

$(IntermediateDirectory)/up_src_Journal.cpp$(PreprocessSuffix): ../src/Journal.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Journal.cpp$(PreprocessSuffix) ../src/Journal.cpp

$(IntermediateDirectory)/up_src_Workbench.cpp$(ObjectSuffix): ../src/Workbench.cpp $(IntermediateDirectory)/up_src_Workbench.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Workbench.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Workbench.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Workbench.cpp$(DependSuffix): ../src/Workbench.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Workbench.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Workbench.cpp$(DependSuffix) -MM ../src/Workbench.cpp

$(IntermediateDirectory)/up_src_Workbench.cpp$(PreprocessSuffix): ../src/Workbench.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Workbench.cpp$(PreprocessSuffix) ../src/Workbench.cpp

$(IntermediateDirectory)/up_src_Bar.cpp$(ObjectSuffix): ../src/Bar.cpp $(IntermediateDirectory)/up_src_Bar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Bar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Bar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Bar.cpp$(DependSuffix): ../src/Bar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Bar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Bar.cpp$(DependSuffix) -MM ../src/Bar.cpp

$(IntermediateDirectory)/up_src_Bar.cpp$(PreprocessSuffix): ../src/Bar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Bar.cpp$(PreprocessSuffix) ../src/Bar.cpp

$(IntermediateDirectory)/up_src_Collision.cpp$(ObjectSuffix): ../src/Collision.cpp $(IntermediateDirectory)/up_src_Collision.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Collision.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Collision.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Collision.cpp$(DependSuffix): ../src/Collision.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Collision.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Collision.cpp$(DependSuffix) -MM ../src/Collision.cpp

$(IntermediateDirectory)/up_src_Collision.cpp$(PreprocessSuffix): ../src/Collision.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Collision.cpp$(PreprocessSuffix) ../src/Collision.cpp

$(IntermediateDirectory)/up_src_ScaleInteractionMode.cpp$(ObjectSuffix): ../src/ScaleInteractionMode.cpp $(IntermediateDirectory)/up_src_ScaleInteractionMode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/ScaleInteractionMode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ScaleInteractionMode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ScaleInteractionMode.cpp$(DependSuffix): ../src/ScaleInteractionMode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ScaleInteractionMode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ScaleInteractionMode.cpp$(DependSuffix) -MM ../src/ScaleInteractionMode.cpp

$(IntermediateDirectory)/up_src_ScaleInteractionMode.cpp$(PreprocessSuffix): ../src/ScaleInteractionMode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ScaleInteractionMode.cpp$(PreprocessSuffix) ../src/ScaleInteractionMode.cpp

$(IntermediateDirectory)/up_src_ScrollableArea.cpp$(ObjectSuffix): ../src/ScrollableArea.cpp $(IntermediateDirectory)/up_src_ScrollableArea.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/ScrollableArea.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ScrollableArea.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ScrollableArea.cpp$(DependSuffix): ../src/ScrollableArea.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ScrollableArea.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ScrollableArea.cpp$(DependSuffix) -MM ../src/ScrollableArea.cpp

$(IntermediateDirectory)/up_src_ScrollableArea.cpp$(PreprocessSuffix): ../src/ScrollableArea.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ScrollableArea.cpp$(PreprocessSuffix) ../src/ScrollableArea.cpp

$(IntermediateDirectory)/up_src_Boss.cpp$(ObjectSuffix): ../src/Boss.cpp $(IntermediateDirectory)/up_src_Boss.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Boss.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Boss.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Boss.cpp$(DependSuffix): ../src/Boss.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Boss.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Boss.cpp$(DependSuffix) -MM ../src/Boss.cpp

$(IntermediateDirectory)/up_src_Boss.cpp$(PreprocessSuffix): ../src/Boss.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Boss.cpp$(PreprocessSuffix) ../src/Boss.cpp

$(IntermediateDirectory)/up_src_SetNodeRotationCommand.cpp$(ObjectSuffix): ../src/SetNodeRotationCommand.cpp $(IntermediateDirectory)/up_src_SetNodeRotationCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/SetNodeRotationCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_SetNodeRotationCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_SetNodeRotationCommand.cpp$(DependSuffix): ../src/SetNodeRotationCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_SetNodeRotationCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_SetNodeRotationCommand.cpp$(DependSuffix) -MM ../src/SetNodeRotationCommand.cpp

$(IntermediateDirectory)/up_src_SetNodeRotationCommand.cpp$(PreprocessSuffix): ../src/SetNodeRotationCommand.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_SetNodeRotationCommand.cpp$(PreprocessSuffix) ../src/SetNodeRotationCommand.cpp

$(IntermediateDirectory)/up_src_Torch.cpp$(ObjectSuffix): ../src/Torch.cpp $(IntermediateDirectory)/up_src_Torch.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Torch.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Torch.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Torch.cpp$(DependSuffix): ../src/Torch.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Torch.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Torch.cpp$(DependSuffix) -MM ../src/Torch.cpp

$(IntermediateDirectory)/up_src_Torch.cpp$(PreprocessSuffix): ../src/Torch.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Torch.cpp$(PreprocessSuffix) ../src/Torch.cpp

$(IntermediateDirectory)/up_src_Actor.cpp$(ObjectSuffix): ../src/Actor.cpp $(IntermediateDirectory)/up_src_Actor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Actor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Actor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Actor.cpp$(DependSuffix): ../src/Actor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Actor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Actor.cpp$(DependSuffix) -MM ../src/Actor.cpp

$(IntermediateDirectory)/up_src_Actor.cpp$(PreprocessSuffix): ../src/Actor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Actor.cpp$(PreprocessSuffix) ../src/Actor.cpp

$(IntermediateDirectory)/up_src_Arena.cpp$(ObjectSuffix): ../src/Arena.cpp $(IntermediateDirectory)/up_src_Arena.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Arena.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Arena.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Arena.cpp$(DependSuffix): ../src/Arena.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Arena.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Arena.cpp$(DependSuffix) -MM ../src/Arena.cpp

$(IntermediateDirectory)/up_src_Arena.cpp$(PreprocessSuffix): ../src/Arena.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Arena.cpp$(PreprocessSuffix) ../src/Arena.cpp

$(IntermediateDirectory)/up_src_Log.cpp$(ObjectSuffix): ../src/Log.cpp $(IntermediateDirectory)/up_src_Log.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/Log.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_Log.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_Log.cpp$(DependSuffix): ../src/Log.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_Log.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_Log.cpp$(DependSuffix) -MM ../src/Log.cpp

$(IntermediateDirectory)/up_src_Log.cpp$(PreprocessSuffix): ../src/Log.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_Log.cpp$(PreprocessSuffix) ../src/Log.cpp

$(IntermediateDirectory)/up_src_FogOfWar.cpp$(ObjectSuffix): ../src/FogOfWar.cpp $(IntermediateDirectory)/up_src_FogOfWar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/FogOfWar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_FogOfWar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_FogOfWar.cpp$(DependSuffix): ../src/FogOfWar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_FogOfWar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_FogOfWar.cpp$(DependSuffix) -MM ../src/FogOfWar.cpp

$(IntermediateDirectory)/up_src_FogOfWar.cpp$(PreprocessSuffix): ../src/FogOfWar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_FogOfWar.cpp$(PreprocessSuffix) ../src/FogOfWar.cpp

$(IntermediateDirectory)/up_src_ItemDatabase.cpp$(ObjectSuffix): ../src/ItemDatabase.cpp $(IntermediateDirectory)/up_src_ItemDatabase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/ItemDatabase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ItemDatabase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ItemDatabase.cpp$(DependSuffix): ../src/ItemDatabase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ItemDatabase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ItemDatabase.cpp$(DependSuffix) -MM ../src/ItemDatabase.cpp

$(IntermediateDirectory)/up_src_ItemDatabase.cpp$(PreprocessSuffix): ../src/ItemDatabase.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ItemDatabase.cpp$(PreprocessSuffix) ../src/ItemDatabase.cpp

$(IntermediateDirectory)/up_src_ListBox.cpp$(ObjectSuffix): ../src/ListBox.cpp $(IntermediateDirectory)/up_src_ListBox.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/ListBox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_ListBox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_ListBox.cpp$(DependSuffix): ../src/ListBox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_ListBox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_ListBox.cpp$(DependSuffix) -MM ../src/ListBox.cpp

$(IntermediateDirectory)/up_src_ListBox.cpp$(PreprocessSuffix): ../src/ListBox.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_ListBox.cpp$(PreprocessSuffix) ../src/ListBox.cpp

$(IntermediateDirectory)/up_src_imgui_imgui_widgets.cpp$(ObjectSuffix): ../src/imgui/imgui_widgets.cpp $(IntermediateDirectory)/up_src_imgui_imgui_widgets.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/imgui/imgui_widgets.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_imgui_imgui_widgets.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_imgui_imgui_widgets.cpp$(DependSuffix): ../src/imgui/imgui_widgets.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_imgui_imgui_widgets.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_imgui_imgui_widgets.cpp$(DependSuffix) -MM ../src/imgui/imgui_widgets.cpp

$(IntermediateDirectory)/up_src_imgui_imgui_widgets.cpp$(PreprocessSuffix): ../src/imgui/imgui_widgets.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_imgui_imgui_widgets.cpp$(PreprocessSuffix) ../src/imgui/imgui_widgets.cpp

$(IntermediateDirectory)/up_src_imgui_imgui.cpp$(ObjectSuffix): ../src/imgui/imgui.cpp $(IntermediateDirectory)/up_src_imgui_imgui.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/imgui/imgui.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_imgui_imgui.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_imgui_imgui.cpp$(DependSuffix): ../src/imgui/imgui.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_imgui_imgui.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_imgui_imgui.cpp$(DependSuffix) -MM ../src/imgui/imgui.cpp

$(IntermediateDirectory)/up_src_imgui_imgui.cpp$(PreprocessSuffix): ../src/imgui/imgui.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_imgui_imgui.cpp$(PreprocessSuffix) ../src/imgui/imgui.cpp

$(IntermediateDirectory)/up_src_imgui_imgui_draw.cpp$(ObjectSuffix): ../src/imgui/imgui_draw.cpp $(IntermediateDirectory)/up_src_imgui_imgui_draw.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/imgui/imgui_draw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_imgui_imgui_draw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_imgui_imgui_draw.cpp$(DependSuffix): ../src/imgui/imgui_draw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_imgui_imgui_draw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_imgui_imgui_draw.cpp$(DependSuffix) -MM ../src/imgui/imgui_draw.cpp

$(IntermediateDirectory)/up_src_imgui_imgui_draw.cpp$(PreprocessSuffix): ../src/imgui/imgui_draw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_imgui_imgui_draw.cpp$(PreprocessSuffix) ../src/imgui/imgui_draw.cpp

$(IntermediateDirectory)/up_src_imgui_imgui_demo.cpp$(ObjectSuffix): ../src/imgui/imgui_demo.cpp $(IntermediateDirectory)/up_src_imgui_imgui_demo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/imgui/imgui_demo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_imgui_imgui_demo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_imgui_imgui_demo.cpp$(DependSuffix): ../src/imgui/imgui_demo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_imgui_imgui_demo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_imgui_imgui_demo.cpp$(DependSuffix) -MM ../src/imgui/imgui_demo.cpp

$(IntermediateDirectory)/up_src_imgui_imgui_demo.cpp$(PreprocessSuffix): ../src/imgui/imgui_demo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_imgui_imgui_demo.cpp$(PreprocessSuffix) ../src/imgui/imgui_demo.cpp

$(IntermediateDirectory)/up_src_miniz_miniz.c$(ObjectSuffix): ../src/miniz/miniz.c $(IntermediateDirectory)/up_src_miniz_miniz.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dmitry/TheDwarfAdventure/src/miniz/miniz.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_src_miniz_miniz.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_src_miniz_miniz.c$(DependSuffix): ../src/miniz/miniz.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_src_miniz_miniz.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_src_miniz_miniz.c$(DependSuffix) -MM ../src/miniz/miniz.c

$(IntermediateDirectory)/up_src_miniz_miniz.c$(PreprocessSuffix): ../src/miniz/miniz.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_src_miniz_miniz.c$(PreprocessSuffix) ../src/miniz/miniz.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/
	$(RM) ../src/Precompiled.h.gch


