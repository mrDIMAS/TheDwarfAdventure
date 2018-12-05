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
#include "Renderer.h"
#include "Game.h"
#include "Node.h"
#include "Scene.h"
#include "Canvas.h"
#include "Config.h"
#include "Input.h"
#include "Sprite.h"
#include "Light.h"
#include "ParticleSystem.h"
#include "Text.h"
#include "PolyLine.h"
#include "Level.h"
#include "Terrain.h"
#include "Widget.h"
#include "TextureStack.h"
#include "WaterBlob.h"
#include "Editor.h"
#include "AttributeGrid.h"
#include "UIHelpers.h"

#ifdef _MSC_VER
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;        // Nvidia
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;  // AMD
}
#endif

struct VertexDecal
{
	Vec2f position;
	sf::Color color;
	Vec2f diffuseTexCoords;
	Vec2f decalTexCoords;
};

static string GetTimeAndDateForFileName(chrono::system_clock::time_point now)
{
	string timeAndDate;
	time_t tt = chrono::system_clock::to_time_t(now);
	tm localTime = *localtime(&tt);
	timeAndDate += to_string(localTime.tm_mday);
	timeAndDate += ' ';
	switch (localTime.tm_mon)
	{
	case 0:
		timeAndDate += "Jan";
		break;
	case 1:
		timeAndDate += "Feb";
		break;
	case 2:
		timeAndDate += "Mar";
		break;
	case 3:
		timeAndDate += "Apr";
		break;
	case 4:
		timeAndDate += "May";
		break;
	case 5:
		timeAndDate += "Jun";
		break;
	case 6:
		timeAndDate += "Jul";
		break;
	case 7:
		timeAndDate += "Aug";
		break;
	case 8:
		timeAndDate += "Sep";
		break;
	case 9:
		timeAndDate += "Oct";
		break;
	case 10:
		timeAndDate += "Nov";
		break;
	case 11:
		timeAndDate += "Dec";
		break;
	}
	timeAndDate += '-';
	timeAndDate += to_string(localTime.tm_year + 1900);
	if (localTime.tm_hour < 10)
	{
		timeAndDate += '0';
	}
	timeAndDate += '-';
	timeAndDate += to_string(localTime.tm_hour);
	timeAndDate += 'h';
	if (localTime.tm_min < 10)
	{
		timeAndDate += '0';
	}
	timeAndDate += to_string(localTime.tm_min);
	timeAndDate += 'm';
	if (localTime.tm_sec < 10)
	{
		timeAndDate += '0';
	}
	timeAndDate += to_string(localTime.tm_sec);
	timeAndDate += 's';
	return timeAndDate;
}

// grim, grim, grim
void Renderer::LoadLightsIntoShaderIntersectsWithRect(const shared_ptr<Scene> &scene,
	const sf::FloatRect &rect, GLint uLightCount, GLint uLightPosRadius, GLint uLightColor)
{
	auto &lightList = scene->GetLightList();

	if (lightList.empty())
	{
		return;
	}

	constexpr int maxLights = 8;
	sf::Glsl::Vec4 lightPosRadius[maxLights];
	sf::Glsl::Vec4 lightColor[maxLights];

	int lightCount = (lightList.size() < maxLights ? lightList.size() : maxLights);
	glUniform1i(uLightCount, lightCount);
	int n = 0;
	for (auto &light : lightList)
	{
		if (light->IsTrulyVisible())
		{
			if (Math::IsCircleRectIntersection({ rect.left, rect.top }, { rect.width, rect.height }, light->GetGlobalPosition(), light->GetRadius()))
			{
				lightPosRadius[n].x = light->GetGlobalPosition().x;
				lightPosRadius[n].y = light->GetGlobalPosition().y;
				lightPosRadius[n].z = light->GetRadius();
				lightPosRadius[n].w = 1.0f; // free

				lightColor[n].x = light->GetColor().r / 255.0f;
				lightColor[n].y = light->GetColor().g / 255.0f;
				lightColor[n].z = light->GetColor().b / 255.0f;
				lightColor[n].w = 1.0f; // free
				++n;
			}
			if (n >= maxLights)
			{
				break;
			}
		}
	}
	glUniform4fv(uLightPosRadius, maxLights, &lightPosRadius[0].x);
	glUniform4fv(uLightColor, maxLights, &lightColor[0].x);
}

void Renderer::FillQuad(vector<sf::Vertex> &vertices, const Vec2f &pos, const Vec2f &size, const sf::Color &color, const sf::FloatRect &texCoord)
{
	vertices.clear();

	const sf::Vertex leftUpperCorner = { pos, color, { texCoord.left, texCoord.top } };
	const sf::Vertex rightUpperCorner = { { pos.x + size.x, pos.y }, color, { texCoord.left + texCoord.width, texCoord.top } };
	const sf::Vertex rightBottomCorner = { pos + size, color, { texCoord.left + texCoord.width, texCoord.top + texCoord.height } };
	const sf::Vertex leftBottomCorner = { { pos.x, pos.y + size.y }, color, { texCoord.left, texCoord.top + texCoord.height } };

	// Triangle 1
	vertices.emplace_back(leftUpperCorner);
	vertices.emplace_back(rightUpperCorner);
	vertices.emplace_back(leftBottomCorner);

	// Triangle 2
	vertices.emplace_back(rightUpperCorner);
	vertices.emplace_back(rightBottomCorner);
	vertices.emplace_back(leftBottomCorner);
}

void Renderer::UploadScheduledTexturesToGPU()
{
	for (auto &tex : mTextureGPUUploadList)
	{
		// create texture on gpu side if not created before
		if (tex->mGLTexture == 0)
		{
			GLCall(glGenTextures(1, &tex->mGLTexture));
			GLCall(glBindTexture(GL_TEXTURE_2D, tex->mGLTexture));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->mWidth, tex->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->mPixels));
		}
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, tex->mGLTexture));
			GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex->mWidth, tex->mHeight, GL_RGBA, GL_UNSIGNED_BYTE, tex->mPixels));
		}
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex->mRepeated ? GL_REPEAT : GL_CLAMP));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex->mRepeated ? GL_REPEAT : GL_CLAMP));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex->mSmooth ? GL_LINEAR : GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex->mSmooth ? GL_LINEAR : GL_NEAREST));
	}
	mTextureGPUUploadList.clear();
}

void Renderer::CreateSpecialTextures()
{
	// create white one-pixel texture
	uint8_t whitePixel[4] = { 255, 255, 255, 255 };
	GLCall(glGenTextures(1, &mDummyTexture));
	GLCall(glBindTexture(GL_TEXTURE_2D, mDummyTexture));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel));

	// create white transparent one-pixel texture as dummy decal texture
	uint8_t whiteTransparentPixel[4] = { 255, 255, 255, 0 };
	GLCall(glGenTextures(1, &mDummyDecal));
	GLCall(glBindTexture(GL_TEXTURE_2D, mDummyDecal));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whiteTransparentPixel));

	GLCall(glGenTextures(1, &mVisibilityGridTexture));
	GLCall(glBindTexture(GL_TEXTURE_2D, mVisibilityGridTexture));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));


	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Renderer::SetupVirtualScreen(float width)
{
	if (width < 1200)
	{
		width = 1200;
		Log() << "WARNING: Virtual screen can't be less than 1200 pixels width!" << LineEnd;
	}

	game->VirtualScreen.setSize(width, width);
	// make sure, that VirtualScreen does not distort proportions
	// adjust height using aspect ratio
	float aspect = mWindow.getDefaultView().getSize().x / mWindow.getDefaultView().getSize().y;
	game->VirtualScreen.setSize(game->VirtualScreen.getSize().x, game->VirtualScreen.getSize().y / aspect);
	game->VirtualScreen.setCenter(game->VirtualScreen.getSize() / 2.0f);
}

void Renderer::LoadShadersAndGetUniforms()
{
	// load terrain shader
	if (mTerrainShader.mShader.loadFromFile("data/shaders/terrain.vs", "data/shaders/terrain.fs"))
	{
		GLuint handle = mTerrainShader.mShader.getNativeHandle();
		mTerrainShader.mUDiffuseTexture = glGetUniformLocation(handle, "diffuseTexture");
		mTerrainShader.mULightPosRadius = glGetUniformLocation(handle, "lightPosRadius");
		mTerrainShader.mULightColor = glGetUniformLocation(handle, "lightColor");
		mTerrainShader.mULightCount = glGetUniformLocation(handle, "lightCount");
		mTerrainShader.mUBackgroundMask = glGetUniformLocation(handle, "backgroundMask");
		mTerrainShader.mUTopBackgroundlessPartHeight = glGetUniformLocation(handle, "topBackgroundlessPartHeight");
		mTerrainShader.mUBackgroundStack = glGetUniformLocation(handle, "backgroundStack");
		mTerrainShader.mUAmbientColor = glGetUniformLocation(handle, "ambientColor");
		mTerrainShader.mUBackgroundStackSize = glGetUniformLocation(handle, "backgroundStackSize");
	}
	else
	{
		throw Exception(StringBuilder("Unable to compile Terrain Shader!"));
	}

	// load sprite shader
	if (mSpriteShader.mShader.loadFromFile("data/shaders/sprite.vs", "data/shaders/sprite.fs"))
	{
		GLuint handle = mSpriteShader.mShader.getNativeHandle();
		mSpriteShader.mUDiffuseTexture = glGetUniformLocation(handle, "diffuseTexture");
		mSpriteShader.mUDecalTexture = glGetUniformLocation(handle, "decalTexture");
		mSpriteShader.mULightingEnabled = glGetUniformLocation(handle, "lightingEnabled");
		mSpriteShader.mULightPosRadius = glGetUniformLocation(handle, "lightPosRadius");
		mSpriteShader.mULightColor = glGetUniformLocation(handle, "lightColor");
		mSpriteShader.mULightCount = glGetUniformLocation(handle, "lightCount");
		mSpriteShader.mUTopBackgroundlessPartHeight = glGetUniformLocation(handle, "topBackgroundlessPartHeight");
		mSpriteShader.mUAmbientColor = glGetUniformLocation(handle, "ambientColor");
	}
	else
	{
		throw Exception(StringBuilder("Unable to compile Sprite Shader!"));
	}

	// load water shader
	if (mWaterBlurShader.mShader.loadFromFile("data/shaders/water.vs", "data/shaders/water.fs"))
	{
		mWaterBlurShader.mUDiffuseTexture = glGetUniformLocation(mWaterBlurShader.mShader.getNativeHandle(), "diffuseTexture");
		mWaterBlurShader.mUdxdy = glGetUniformLocation(mWaterBlurShader.mShader.getNativeHandle(), "dxdy");
	}
	else
	{
		throw Exception(StringBuilder("Unable to compile Water Blur Shader!"));
	}

	// load simple shader
	if (mSimpleShader.mShader.loadFromFile("data/shaders/simple.vs", "data/shaders/simple.fs"))
	{
		mSimpleShader.mUDiffuseTexture = glGetUniformLocation(mSimpleShader.mShader.getNativeHandle(), "diffuseTexture");
	}
	else
	{
		throw Exception(StringBuilder("Unable to compile Simple Shader!"));
	}

	// load simple shader
	if (mWaterContrastShader.mShader.loadFromFile("data/shaders/water_contrast.vs", "data/shaders/water_contrast.fs"))
	{
		mWaterContrastShader.mUDiffuseTexture = glGetUniformLocation(mWaterContrastShader.mShader.getNativeHandle(), "diffuseTexture");
	}
	else
	{
		throw Exception(StringBuilder("Unable to compile Water Contrast Shader!"));
	}
}

void Renderer::LoadTextureStackOnGPU(const shared_ptr<TextureStack> &stck)
{
	if (!stck->NeedUploadToGPU)
	{
		return;
	}

	const int w = stck->mWidth;
	const int h = stck->mHeight;
	const int d = stck->mSources.size();

	if (!stck->mVolumeTextureID)
	{
		GLCall(glGenTextures(1, &stck->mVolumeTextureID));
		GLCall(glBindTexture(GL_TEXTURE_3D, stck->mVolumeTextureID));
		GLCall(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP));
		GLCall(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, w, h, d, 0, GL_RGBA, GL_UNSIGNED_BYTE, stck->mPixels));
	}
	else
	{
		GLCall(glBindTexture(GL_TEXTURE_3D, stck->mVolumeTextureID));
		GLCall(glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, w, h, d, GL_RGBA, GL_UNSIGNED_BYTE, stck->mPixels));
	}
	stck->NeedUploadToGPU = false;
}

void Renderer::InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();

	// init keyboard mapping
	io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Tab;
	io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Left;
	io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Right;
	io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Up;
	io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Down;
	io.KeyMap[ImGuiKey_PageUp] = sf::Keyboard::PageUp;
	io.KeyMap[ImGuiKey_PageDown] = sf::Keyboard::PageDown;
	io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Home;
	io.KeyMap[ImGuiKey_End] = sf::Keyboard::End;
	io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Delete;
	io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::BackSpace;
	io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Return;
	io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
	io.KeyMap[ImGuiKey_A] = sf::Keyboard::A;
	io.KeyMap[ImGuiKey_C] = sf::Keyboard::C;
	io.KeyMap[ImGuiKey_V] = sf::Keyboard::V;
	io.KeyMap[ImGuiKey_X] = sf::Keyboard::X;
	io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Y;
	io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Z;

	io.DisplaySize = static_cast<Vec2f>(mWindow.getSize());
	io.RenderDrawListsFn = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;



	// load font
	io.Fonts->AddFontFromFileTTF("data/fonts/DroidSans.ttf", 14, NULL, io.Fonts->GetGlyphRangesCyrillic());
	unsigned char *pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	mImGuiFontAtlas = CreateTexture(width, height);
	memcpy(mImGuiFontAtlas->mPixels, pixels, width * height * 4);
	// force texture upload to GPU
	ScheduleTextureGPUUpload(mImGuiFontAtlas);
	UploadScheduledTexturesToGPU();
	// save opengl texture handle
	io.Fonts->TexID = (void *)mImGuiFontAtlas->mGLTexture;
}

void Renderer::RenderImGui()
{

	ImDrawData *draw_data = ImGui::GetDrawData();
	if (!draw_data || draw_data->CmdListsCount == 0)
	{
		return;
	}
	ImGuiIO &io = ImGui::GetIO();
	// scale stuff (needed for proper handling of window resize)
	int fb_width = static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fb_height = static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	if (fb_width == 0 || fb_height == 0)
	{
		return;
	}
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);

	// Save OpenGL state
	GLint last_viewport[4];
	glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_scissor_box[4];
	glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);

	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_TEXTURE_2D);

	glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	for (int n = 0; n < draw_data->CmdListsCount; ++n)
	{
		const ImDrawList *cmd_list = draw_data->CmdLists[n];
		const char *vertexBuffer = (const char *)&cmd_list->VtxBuffer.front();
		const ImDrawIdx *idx_buffer = &cmd_list->IdxBuffer.front();

		glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), vertexBuffer + offsetof(ImDrawVert, pos));
		glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), vertexBuffer + offsetof(ImDrawVert, uv));
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), vertexBuffer + offsetof(ImDrawVert, col));

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); ++cmd_i)
		{
			const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				GLuint tex_id = (GLuint) * ((unsigned int *)&pcmd->TextureId);
				glBindTexture(GL_TEXTURE_2D, tex_id);
				glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w),
					(int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer);
			}
			idx_buffer += pcmd->ElemCount;
		}
	}

	// Restore modified state
	glBindTexture(GL_TEXTURE_2D, (GLuint)last_texture);
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2],
		(GLsizei)last_scissor_box[3]);
}

void Renderer::UpdateImGuiInput(const sf::Event &event)
{
	ImGuiIO &io = ImGui::GetIO();
	switch (event.type)
	{
	case sf::Event::MouseMoved:
		io.MousePos.x = static_cast<float>(event.mouseMove.x);
		io.MousePos.y = static_cast<float>(event.mouseMove.y);
		break;
	case sf::Event::MouseButtonPressed:
		io.MouseDown[event.mouseButton.button] = true;
		break;
	case sf::Event::MouseButtonReleased:
		io.MouseDown[event.mouseButton.button] = false;
		break;
	case sf::Event::MouseWheelScrolled:
		io.MouseWheel += event.mouseWheelScroll.delta;
		break;
	case sf::Event::KeyPressed:
	case sf::Event::KeyReleased:
		io.KeysDown[event.key.code] = (event.type == sf::Event::KeyPressed);
		io.KeyCtrl = event.key.control;
		io.KeyShift = event.key.shift;
		io.KeyAlt = event.key.alt;
		break;
	case sf::Event::TextEntered:
		if (event.text.unicode > 0 && event.text.unicode < 0x10000)
		{
			io.AddInputCharacter(static_cast<ImWchar>(event.text.unicode));
		}
		break;
    default:
        break;
	}
}

void Renderer::SetupFrameBuffer(int fboWidth, int fboHeight)
{
	mFBOSize = { fboWidth, fboHeight };

	// init first FBO
	{
		glGenFramebuffers(1, &mFBO1);
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO1);

		glGenTextures(1, &mFrameTex1);
		glBindTexture(GL_TEXTURE_2D, mFrameTex1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fboWidth, fboHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFrameTex1, 0);
	}

	// init second FBO
	{
		glGenFramebuffers(1, &mFBO2);
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO2);

		glGenTextures(1, &mFrameTex2);
		glBindTexture(GL_TEXTURE_2D, mFrameTex2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fboWidth, fboHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFrameTex2, 0);
	}

	// Init scene FBO
	{
		glGenFramebuffers(1, &mSceneFBO);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::RenderSprite(const shared_ptr<Sprite> &sprite)
{
	// setup vertices
	auto t1 = sprite->GetTextureRectangle();
	auto t2 = sprite->GetDecalTextureRectangle();
	auto w = sprite->GetSize().x;
	auto h = sprite->GetSize().y;
	auto c = sprite->GetColor();
	VertexDecal v[4] = {
		{ { 0, 0 }, c, { t1.left, t1.top }, { t2.left, t2.top } },
		{ { w, 0 }, c, { t1.left + t1.width, t1.top }, { t2.left + t2.width, t2.top } },
		{ { w, h }, c, { t1.left + t1.width, t1.top + t1.height },
		{ t2.left + t2.width, t2.top + t2.height } },
		{ { 0, h }, c, { t1.left, t1.top + t1.height }, { t2.left, t2.top + t2.height } }
	};

	if (sprite->IsXFlipped())
	{
		swap(v[0].diffuseTexCoords.x, v[1].diffuseTexCoords.x);
		swap(v[2].diffuseTexCoords.x, v[3].diffuseTexCoords.x);
	}
	if (sprite->IsYFlipped())
	{
		swap(v[0].diffuseTexCoords.y, v[2].diffuseTexCoords.y);
		swap(v[1].diffuseTexCoords.y, v[3].diffuseTexCoords.y);
	}

	// render stuff
	const char *data = reinterpret_cast<const char *>(&v[0]);
	GLCall(glLoadMatrixf(sprite->GetGlobalTransform().getMatrix()));
	GLCall(glVertexPointer(2, GL_FLOAT, sizeof(VertexDecal), data + offsetof(VertexDecal, position)));
	GLCall(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(VertexDecal), data + offsetof(VertexDecal, color)));
	GLCall(glClientActiveTexture(GL_TEXTURE0));
	GLCall(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
	GLCall(glTexCoordPointer(2, GL_FLOAT, sizeof(VertexDecal), data + offsetof(VertexDecal, diffuseTexCoords)));
	GLCall(glClientActiveTexture(GL_TEXTURE1));
	GLCall(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
	GLCall(glTexCoordPointer(2, GL_FLOAT, sizeof(VertexDecal), data + offsetof(VertexDecal, decalTexCoords)));
	GLCall(glDrawArrays(GL_QUADS, 0, 4));
	++mDrawCallCount;
}

void Renderer::BuiltInProfiler_BeginProfiling()
{
	if (mConfig.mFrameProfilerEnabled)
	{
		mProfilerStartTime = chrono::high_resolution_clock::now();
	}
}

void Renderer::BuiltInProfiler_EndProfiling(const string & msg)
{
	if (mConfig.mFrameProfilerEnabled)
	{
		auto timeDiff = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - mProfilerStartTime);
		Log() << "Profiling: " << msg << " - " << (float)timeDiff.count() / 1000.0f << " milliseconds" << LineEnd;
	}
}

void Renderer::Restart()
{
	sf::VideoMode videoMode;
	videoMode.width = mConfig.mWidth;
	videoMode.height = mConfig.mHeight;
	videoMode.bitsPerPixel = 32;

	sf::Uint32 windowStyle;

	if (mConfig.mFullscreen)
	{
		windowStyle = sf::Style::Fullscreen;
	}
	else
	{
		if (mConfig.mBorderlessWindow)
		{
			windowStyle = sf::Style::None;
		}
		else
		{
			windowStyle = sf::Style::Titlebar | sf::Style::Close;
		}
	}

	mWindow.create(videoMode, "The Dwarf Adventure", windowStyle, sf::ContextSettings(24, 8));
	mWindow.setFramerateLimit(mConfig.mFrameRateLimit);

	Log() << "Renderer successfully (re)initialized!" << LineEnd;
	Log() << "Game running on GPU " << glGetString(GL_VENDOR) << ". " << glGetString(GL_RENDERER) << LineEnd;
}

Renderer::Renderer(const RendererConfig & config) : mConfig(config)
{
	Restart();

	glewInit();

	LoadShadersAndGetUniforms();

	SetupVirtualScreen(config.mVirtualScreenSize);
	Log() << "Actual Resolution: " << mConfig.mWidth << "x" << mConfig.mHeight << " pixels." << LineEnd;
	Log() << "Virtual Screen Size: " << game->VirtualScreen.getSize().x << "x" << game->VirtualScreen.getSize().y << " pixels." << LineEnd;

	CreateSpecialTextures();

	SetupFrameBuffer(static_cast<int>(game->VirtualScreen.getSize().x), static_cast<int>(game->VirtualScreen.getSize().y));

	InitImGui();
}

Renderer::~Renderer()
{
	glDeleteTextures(1, &mDummyTexture);
	glDeleteTextures(1, &mDummyDecal);
	glDeleteTextures(1, &mFrameTex1);
	glDeleteTextures(1, &mFrameTex2);
	glDeleteFramebuffers(1, &mFBO1);
	glDeleteFramebuffers(1, &mFBO2);
	ImGui::DestroyContext();
	DBG_ECHO();
}

void Renderer::Render()
{
	if (!IsActive())
	{
		return;
	}

	lock_guard<recursive_mutex> guard(mMutex);
	auto lastTime = chrono::high_resolution_clock::now();

	mDrawCallCount = 0;

	BuiltInProfiler_BeginProfiling();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(FrameBufferClearColor.r / 255.0f, FrameBufferClearColor.g / 255.0f, FrameBufferClearColor.b / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	mWindow.resetGLStates();
	BuiltInProfiler_EndProfiling("glClear");

	for (GLuint textureToDelete : mTextureGPUDeletionList)
	{
		glDeleteTextures(1, &textureToDelete);
	}
	mTextureGPUDeletionList.clear();

	BuiltInProfiler_BeginProfiling();
	UploadScheduledTexturesToGPU();
	BuiltInProfiler_EndProfiling("UploadScheduledTexturesToGPU");

	GLCall(glViewport(0, 0, mWindow.getSize().x, mWindow.getSize().y));

	//////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////
	// render scenes
	BuiltInProfiler_BeginProfiling();
	for (auto scene : game->GetSceneList())
	{
		if (!scene->IsVisible())
		{
			continue;
		}

		const shared_ptr<Terrain> terrain = scene->GetTerrain();
		const sf::View & view = scene->GetView();
		mWindow.setView(view);


		const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
		GLuint targetFBO;
		if (scene->GetRenderingMode() == SceneRenderingMode::ToTexture)
		{
			targetFBO = mSceneFBO;

			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mSceneFBO));

			const shared_ptr<Texture> frame = scene->GetFrame();

			const int w = static_cast<int>(scene->GetFrameSize().x);
			const int h = static_cast<int>(scene->GetFrameSize().y);

			if (w > 0 && h > 0)
			{
				// Viewport size has changed, recreate texture
				if (frame->mWidth != w || frame->mHeight != h)
				{
					GLCall(glDeleteTextures(1, &frame->mGLTexture));

					frame->mWidth = w;
					frame->mHeight = h;

					GLCall(glActiveTexture(GL_TEXTURE0));
					GLCall(glGenTextures(1, &frame->mGLTexture));
					GLCall(glBindTexture(GL_TEXTURE_2D, frame->mGLTexture));
					GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
					GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
					GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
					GLCall(glBindTexture(GL_TEXTURE_2D, 0));
				}

				// Bind frame as render target
				GLCall(glDrawBuffers(1, buffers));
				GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame->mGLTexture, 0));
				GLCall(glClear(GL_COLOR_BUFFER_BIT));

				GLCall(glViewport(0, 0, w, h));
			}
			else
			{
				// Skip this scene because it has invalid frame size
				continue;
			}
		}
		else
		{
			targetFBO = 0;
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, targetFBO));

			const sf::IntRect viewport = mWindow.getViewport(view);
			GLCall(glViewport(viewport.left, viewport.top, viewport.width, viewport.height));
		}

		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		// render parallax background
		sf::Shader::bind(&mSimpleShader.mShader);
		GLCall(glActiveTexture(GL_TEXTURE0));
		glUniform1i(mSimpleShader.mUDiffuseTexture, 0);

		for (const shared_ptr<BackgroundLayer> & layer : scene->GetBackgroundLayers())
		{
			const shared_ptr<Texture> layerTex = layer->GetTexture();

			if (layer == scene->GetBackgroundLayers().front())
			{
				// farthest background
				GLCall(glMatrixMode(GL_PROJECTION));
				GLCall(glLoadMatrixf(game->VirtualScreen.getTransform().getMatrix()));
				GLCall(glMatrixMode(GL_MODELVIEW));
				GLCall(glBindTexture(GL_TEXTURE_2D, layerTex->mGLTexture));
				FillQuad(mQuadVertices, Vec2f(0, 0), game->VirtualScreen.getSize(), sf::Color::White, { 0, 0, 1, 1 });
				RenderQuads(sf::Transform::Identity, mQuadVertices);
			}
			else
			{
				// other layers with parallax
				GLCall(glMatrixMode(GL_PROJECTION));
				GLCall(glLoadMatrixf(view.getTransform().getMatrix()));
				GLCall(glMatrixMode(GL_MODELVIEW));
				GLCall(glBindTexture(GL_TEXTURE_2D, layerTex->mGLTexture));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

				Vec2f size;
				Vec2f position;
				float kw;
				position.x = view.getCenter().x - view.getSize().x / 2.0f;
				if (layer->IsFullscreen())
				{
					kw = layerTex->GetAspectRatio();
					size = Vec2f(layerTex->mWidth * kw, view.getSize().y);
					position.y = view.getCenter().y - view.getSize().y / 2.0f;
				}
				else
				{
					const float aspect = layerTex->GetAspectRatio();
					const float h = (float)layerTex->mHeight / aspect;
					size = Vec2f(view.getSize().x, h);
					position.y = terrain ? terrain->GetTopBackgroundlessPartHeight() - h : -h;
					kw = view.getSize().x / (float)layerTex->mHeight;
				}

				const sf::FloatRect texCoords = { layer->GetTexCoordPosition(), Vec2f(kw, 1.0f) };
				FillQuad(mQuadVertices, position, size, sf::Color::White, texCoords);
				RenderQuads(sf::Transform::Identity, mQuadVertices);
			}
		}
		BuiltInProfiler_EndProfiling("render scenes");

		BuiltInProfiler_BeginProfiling();
		GLCall(glMatrixMode(GL_PROJECTION));
		GLCall(glLoadMatrixf(view.getTransform().getMatrix()));
		GLCall(glMatrixMode(GL_MODELVIEW));

		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		// render terrain
		sf::Shader::bind(&mTerrainShader.mShader);
		const sf::Color ambientColor = scene->GetAmbientColor();
		GLCall(glUniform4f(mTerrainShader.mUAmbientColor, ambientColor.r / 255.0f, ambientColor.g / 255.0f, ambientColor.b / 255.0f, 1.0f));

		if (terrain)
		{
			//BuiltInProfiler_BeginProfiling();
			GLCall(glUniform1f(mTerrainShader.mUTopBackgroundlessPartHeight, terrain->GetTopBackgroundlessPartHeight()));
			GLCall(glUniform1i(mTerrainShader.mUDiffuseTexture, 0));
			GLCall(glUniform1i(mTerrainShader.mUBackgroundStack, 1));
			GLCall(glUniform1i(mTerrainShader.mUBackgroundMask, 2));

			LoadTextureStackOnGPU(terrain->GetBackgroundTextureStack());

			// bind background texture stack as volume texture
			const shared_ptr<TextureStack> stck = terrain->GetBackgroundTextureStack();
			GLCall(glActiveTexture(GL_TEXTURE1));
			GLCall(glBindTexture(GL_TEXTURE_3D, stck->mVolumeTextureID));

			// pass stack size to for correct fetching of texture
			GLCall(glUniform1f(mTerrainShader.mUBackgroundStackSize, static_cast<float>(stck->mSources.size())));

			//BuiltInProfiler_EndProfiling("    terrain 1");

			//////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////
			// render cells (using caching)

			const Vec2f size = { Cell::SizeF, Cell::SizeF };
			for (const shared_ptr<Cell> & cell : terrain->GetCells())
			{
				if (Math::IsRectRectIntersection(cell->GetPosition(), size, view.getCenter() - view.getSize() * 0.5f, view.getSize()))
				{
					//BuiltInProfiler_BeginProfiling();
					if (cell->mNeedUpdate)
					{
						// create texture on gpu side if it wasn't created early
						if (cell->mTextureId == 0)
						{
							GLCall(glGenTextures(1, &cell->mTextureId));
							GLCall(glBindTexture(GL_TEXTURE_2D, cell->mTextureId));
							GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
							GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
							GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
							GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
							GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Cell::Size, Cell::Size, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
						}

						// update texture
						cell->TransferPixelsFromGridToLinearOrder(mCellBuffer);
						GLCall(glBindTexture(GL_TEXTURE_2D, cell->mTextureId));
						GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Cell::Size, Cell::Size, GL_RGBA, GL_UNSIGNED_BYTE, mCellBuffer.data()));

						// create terrain background texture (mask texture)
						if (cell->mBackgroundMaskTextureId == 0)
						{
							GLCall(glGenTextures(1, &cell->mBackgroundMaskTextureId));
							GLCall(glBindTexture(GL_TEXTURE_2D, cell->mBackgroundMaskTextureId));
							GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
							GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
							GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
							GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
							GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Cell::BackgroundSize, Cell::BackgroundSize, 0, GL_RED, GL_UNSIGNED_BYTE, NULL));
						}

						GLCall(glBindTexture(GL_TEXTURE_2D, cell->mBackgroundMaskTextureId));
						GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Cell::BackgroundSize, Cell::BackgroundSize, GL_RED, GL_UNSIGNED_BYTE, cell->GetBackgroundPixels()));

						cell->mNeedUpdate = false;
					}
					//BuiltInProfiler_EndProfiling("    terrain 2");

					//BuiltInProfiler_BeginProfiling();
					GLCall(glActiveTexture(GL_TEXTURE0));
					GLCall(glBindTexture(GL_TEXTURE_2D, cell->mTextureId));

					// bind background mask
					GLCall(glActiveTexture(GL_TEXTURE2));
					GLCall(glBindTexture(GL_TEXTURE_2D, cell->mBackgroundMaskTextureId));

					// render
					LoadLightsIntoShaderIntersectsWithRect(scene, sf::FloatRect(cell->GetPosition(), size), mTerrainShader.mULightCount, mTerrainShader.mULightPosRadius, mTerrainShader.mULightColor);
					FillQuad(mQuadVertices, cell->GetPosition(), size, sf::Color::White, { 0, 0, 1, 1 });
					if (!game->Debug.terrainRenderingDisabled)
					{
						RenderQuads(sf::Transform::Identity, mQuadVertices);
					}
					//BuiltInProfiler_EndProfiling("    terrain 3");
				}
				else
				{
					//BuiltInProfiler_BeginProfiling();

					// Mark texture for further update when it again appears on the screen
					cell->mNeedUpdate = true;
					// Free texture on gpu side
					if (cell->mTextureId > 0)
					{
						GLCall(glDeleteTextures(1, &cell->mTextureId));
						cell->mTextureId = 0;

						GLCall(glDeleteTextures(1, &cell->mBackgroundMaskTextureId))
							cell->mBackgroundMaskTextureId = 0;
					}
					//BuiltInProfiler_EndProfiling("    terrain 4");
				}
			}
		}
		BuiltInProfiler_EndProfiling("render terrain");

		BuiltInProfiler_BeginProfiling();
		GLCall(glMatrixMode(GL_PROJECTION));
		GLCall(glLoadMatrixf(view.getTransform().getMatrix()));
		GLCall(glMatrixMode(GL_MODELVIEW));

		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		// render sprites
		sf::Shader::bind(&mSpriteShader.mShader);
		GLCall(glUniform4f(mSpriteShader.mUAmbientColor, ambientColor.r / 255.0f, ambientColor.g / 255.0f, ambientColor.b / 255.0f, 1.0f));
		GLCall(glUniform1i(mSpriteShader.mUDiffuseTexture, 0));
		GLCall(glUniform1i(mSpriteShader.mUDecalTexture, 1));
		// Hack
		if (game->GetEditor()->IsActive())
		{
			GLCall(glUniform1f(mSpriteShader.mUTopBackgroundlessPartHeight, 9999.0f));
		}
		else
		{
			GLCall(glUniform1f(mSpriteShader.mUTopBackgroundlessPartHeight, terrain ? terrain->GetTopBackgroundlessPartHeight() : 0));
		}

		GLuint lastDiffuseTexture = 0;
		GLuint lastDecalTexture = 0;
		if (scene->mNeedSortRenderOrder)
		{
			scene->SortSpritesByRenderOrder();
			scene->mNeedSortRenderOrder = false;
		}
		for (const shared_ptr<Sprite> & sprite : scene->GetSpriteList())
		{
			if (sprite->IsInView(view) && sprite->IsTrulyVisible())
			{
				// bind diffuse texture
				GLCall(glActiveTexture(GL_TEXTURE0));
				const shared_ptr<Texture> diffuseTex = sprite->GetTexture();
				if (diffuseTex)
				{
					if (lastDiffuseTexture != diffuseTex->mGLTexture)
					{
						GLCall(glBindTexture(GL_TEXTURE_2D, diffuseTex->mGLTexture));
						lastDiffuseTexture = diffuseTex->mGLTexture;
					}
				}
				else
				{
					GLCall(glBindTexture(GL_TEXTURE_2D, mDummyTexture));
					lastDiffuseTexture = mDummyTexture;
				}

				// bind decal texture
				GLCall(glActiveTexture(GL_TEXTURE1));
				const shared_ptr<Texture> decalTex = sprite->GetDecalTexture();
				if (decalTex)
				{
					if (lastDecalTexture != decalTex->mGLTexture)
					{
						GLCall(glBindTexture(GL_TEXTURE_2D, decalTex->mGLTexture));
						lastDecalTexture = decalTex->mGLTexture;
					}
				}
				else
				{
					GLCall(glBindTexture(GL_TEXTURE_2D, mDummyDecal));
					lastDecalTexture = mDummyDecal;
				}

				// set some rendering options
				if ((sprite->GetRenderFlags() & RF_NO_LIGHTING) || !scene->IsLightingEnabled())
				{
					GLCall(glUniform1i(mSpriteShader.mULightingEnabled, GL_FALSE));
				}
				else
				{
					GLCall(glUniform1i(mSpriteShader.mULightingEnabled, GL_TRUE));
				}

				LoadLightsIntoShaderIntersectsWithRect(scene, sf::FloatRect(sprite->GetGlobalPosition(), sprite->GetSize()), mSpriteShader.mULightCount, mSpriteShader.mULightPosRadius, mSpriteShader.mULightColor);
				RenderSprite(sprite);
			}
		}

		BuiltInProfiler_EndProfiling("render sprites");
		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		// render particle systems
		BuiltInProfiler_BeginProfiling();

		GLCall(glClientActiveTexture(GL_TEXTURE1));
		GLCall(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
		GLCall(glClientActiveTexture(GL_TEXTURE0));

		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		GLCall(glActiveTexture(GL_TEXTURE2));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		GLCall(glActiveTexture(GL_TEXTURE0));
		sf::Shader::bind(&mSimpleShader.mShader);
		GLCall(glUniform1i(mSimpleShader.mUDiffuseTexture, 0));

		for (const shared_ptr<ParticleSystem> & particleSystem : scene->GetParticleSystemList())
		{
			if (particleSystem->IsInView(view) && particleSystem->IsTrulyVisible())
			{
				if (particleSystem->GetTexture())
				{
					GLCall(glBindTexture(GL_TEXTURE_2D, particleSystem->GetTexture()->mGLTexture));
				}
				RenderQuads(particleSystem->GetGlobalTransform(), particleSystem->GetVertexArray());
			}
		}
		BuiltInProfiler_EndProfiling("render particle systems");

		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		// render texts
		BuiltInProfiler_BeginProfiling();
		for (const shared_ptr<Text> & text : scene->GetTextList())
		{
			if (text->IsTrulyVisible())
			{
				const shared_ptr<sf::Font> font = text->GetFont();

				if (font)
				{
					text->BreakOnLines();
					GLCall(glBindTexture(GL_TEXTURE_2D, font->getTexture(text->GetCharacterSize()).getNativeHandle()));
					RenderQuads(text->GetGlobalTransform(), text->GetVertices());
				}
			}
		}
		BuiltInProfiler_EndProfiling("render texts");


		BuiltInProfiler_BeginProfiling();
		GLCall(glBindTexture(GL_TEXTURE_2D, mDummyTexture));
		GLCall(glLoadIdentity());

		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		// render polylines
		BuiltInProfiler_BeginProfiling();
		for (const shared_ptr<PolyLine> & polyline : scene->GetPolyLineList())
		{
			if (polyline->IsVisible())
			{
				const vector<Vec2f> & points = polyline->GetPoints();

				if (points.size() > 1)
				{
					glLineWidth(polyline->GetThickness());
					glBegin(GL_LINE_STRIP);
					glColor3ub(polyline->GetColor().r, polyline->GetColor().g, polyline->GetColor().b);
					for (const Vec2f & pt : points)
					{
						glVertex2f(pt.x, pt.y);
					}
					GLCall(glEnd());
				}
			}
		}
		BuiltInProfiler_EndProfiling("render polylines");

		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		// render water blobs (experimental)
		if (!scene->GetWaterBlobList().empty())
		{
			BuiltInProfiler_BeginProfiling();

			GLCall(glActiveTexture(GL_TEXTURE0));

			// bind shader
			sf::Shader::bind(&mSimpleShader.mShader);
			GLCall(glUniform1i(mSimpleShader.mUDiffuseTexture, 0));

			// render water into separate texture
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFBO1));
			GLCall(glDrawBuffers(1, buffers));
			GLCall(glClearColor(0, 0, 0, 0.0f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
			GLCall(glViewport(0, 0, mFBOSize.x, mFBOSize.y));

			GLCall(glMatrixMode(GL_PROJECTION));
			GLCall(glLoadMatrixf(view.getTransform().getMatrix()));

			for (const shared_ptr<WaterBlob> & blob : scene->GetWaterBlobList())
			{
				const shared_ptr<Texture> diffuseTexture = blob->GetTexture();

				if (diffuseTexture)
				{
					GLCall(glBindTexture(GL_TEXTURE_2D, diffuseTexture->mGLTexture));
				}
				else
				{
					GLCall(glBindTexture(GL_TEXTURE_2D, mDummyTexture));
				}
				blob->EmitQuads(mBlobVertices);
				RenderQuads(sf::Transform::Identity, mBlobVertices);
			}

			GLCall(glMatrixMode(GL_PROJECTION));
			GLCall(glLoadMatrixf(game->VirtualScreen.getTransform().getMatrix()));
			sf::Shader::bind(&mWaterBlurShader.mShader);
			GLCall(glUniform1i(mWaterBlurShader.mUDiffuseTexture, 0));
			FillQuad(mQuadVertices, Vec2f(0, 0), Vec2f(mFBOSize), sf::Color::White, { 0, 0, 1, 1 });

			// smooth horizontally
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFBO2));
			GLCall(glDrawBuffers(1, buffers));
			GLCall(glClearColor(0, 0, 0, 0.0f));
			GLCall(glBindTexture(GL_TEXTURE_2D, mFrameTex1));
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
			GLCall(glUniform2f(mWaterBlurShader.mUdxdy, 1.0f / mFBOSize.x, 0.0f));
			RenderQuads(sf::Transform::Identity, mQuadVertices);

			// smooth vertically
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFBO1));
			GLCall(glDrawBuffers(1, buffers));
			GLCall(glClearColor(0, 0, 0, 0.0f));
			GLCall(glBindTexture(GL_TEXTURE_2D, mFrameTex2));
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			GLCall(glUniform2f(mWaterBlurShader.mUdxdy, 0.0f, 1.0f / mFBOSize.y));
			RenderQuads(sf::Transform::Identity, mQuadVertices);

			// render result as fullscreen quad
			glBindFramebuffer(GL_FRAMEBUFFER, targetFBO);

			GLCall(glBindTexture(GL_TEXTURE_2D, mFrameTex1));
			sf::Shader::bind(&mWaterContrastShader.mShader);
			const Vec2f screenSize = mWindow.getDefaultView().getSize();
			GLCall(glViewport(0, 0, static_cast<GLsizei>(screenSize.x), static_cast<GLsizei>(screenSize.y)));
			GLCall(glUniform1i(mWaterContrastShader.mUDiffuseTexture, 0));
			GLCall(glMatrixMode(GL_PROJECTION));
			GLCall(glLoadMatrixf(mWindow.getDefaultView().getTransform().getMatrix()));

			FillQuad(mQuadVertices, Vec2f(0, 0), screenSize, sf::Color::White, { 0, 0, 1, -1 });
			RenderQuads(sf::Transform::Identity, mQuadVertices);

			BuiltInProfiler_EndProfiling("render water blobs");
		}

		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		// render fog of war
		BuiltInProfiler_BeginProfiling();

		GLCall(glMatrixMode(GL_PROJECTION));
		GLCall(glLoadMatrixf(view.getTransform().getMatrix()));
		GLCall(glMatrixMode(GL_MODELVIEW));

		GLCall(glActiveTexture(GL_TEXTURE0));
		sf::Shader::bind(&mSimpleShader.mShader);
		GLCall(glUniform1i(mSimpleShader.mUDiffuseTexture, 0));

		if (terrain && terrain->GetFogOfWar() && terrain->IsFogOfWarEnabled())
		{
			glBindTexture(GL_TEXTURE_2D, terrain->GetFogOfWar()->GetTexture()->mGLTexture);
			mQuadVertices.clear();
			FillQuad(mQuadVertices, Vec2f(0, 0), terrain->GetSizePixels(), sf::Color::White, { 0, 0, 1, 1 });
			RenderQuads(sf::Transform::Identity, mQuadVertices);
		}

		BuiltInProfiler_EndProfiling("render fog of war");
	}

	// Revert to back buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////
	// render GUI
	BuiltInProfiler_BeginProfiling();
	sf::Shader::bind(&mSimpleShader.mShader);
	GLCall(glActiveTexture(GL_TEXTURE0));
	glUniform1i(mSimpleShader.mUDiffuseTexture, 0);
	mWindow.setView(game->VirtualScreen);
	GLCall(glMatrixMode(GL_PROJECTION));
	GLCall(glLoadMatrixf(game->VirtualScreen.getTransform().getMatrix()));
	GLCall(glEnable(GL_STENCIL_TEST));
	GLCall(glMatrixMode(GL_MODELVIEW));
	GLCall(glLoadIdentity());
	for (const shared_ptr<Canvas> & canvas : game->GetCanvasList())
	{
		if (!canvas->IsVisible())
		{
			continue;
		}

		for (const shared_ptr<Widget> & widget : canvas->GetWidgetList())
		{
			if (!widget->HasParent())
			{
				widget->Draw(0, mDrawCallCount);
			}
		}
	}
	GLCall(glDisable(GL_STENCIL_TEST));
	BuiltInProfiler_EndProfiling("render gui");

	BuiltInProfiler_BeginProfiling();
	//////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////
	// render ImGui
	RenderImGui();
	BuiltInProfiler_EndProfiling("render imgui");

	// frame time must be measured before mWindow.display(), cuz if vsync was
	// enabled, frame time will be incorrect
	mFrameTime = static_cast<unsigned int>(
		chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - lastTime)
		.count());

	BuiltInProfiler_BeginProfiling();
	// swap buffers
	mWindow.display();
	BuiltInProfiler_EndProfiling("swapbuffers");
}

void Renderer::ProcessEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			mWindow.close();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			// do screenshot
			if (event.key.code == game->GetControls().GetKey(ControlKey::TakeScreenshot))
			{
				sf::Vector2u windowSize = mWindow.getSize();
				sf::Texture texture;
				texture.create(windowSize.x, windowSize.y);
				texture.update(mWindow);
				sf::Image screenshot = texture.copyToImage();
				screenshot.saveToFile(StringBuilder("TDA ") << GetTimeAndDateForFileName(chrono::system_clock::now()) << ".png");
			}
		}
		game->GetInput()->Listen(event);
		UpdateImGuiInput(event);
	}
}

bool Renderer::IsActive() const
{
	return mWindow.isOpen();
}

sf::RenderWindow &Renderer::GetWindow()
{
	return mWindow;
}

shared_ptr<Texture> Renderer::RequestTexture(const string &path, const sf::Color &mask)
{
	lock_guard<recursive_mutex> guard(mMutex);
	shared_ptr<Texture> texture;

	// remove unreferenced pointers
	for (auto iter = mTextureList.begin(); iter != mTextureList.end();)
	{
		if (iter->second.expired())
		{
			iter = mTextureList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	auto existing = mTextureList.find(path);
	if (existing == mTextureList.end())
	{
		texture = make_shared<Texture>();
		texture->LoadPixelsFromFile(path, mask);
		ScheduleTextureGPUUpload(texture);
		mTextureList[path] = texture;
	}
	else
	{
		texture = existing->second.lock();
	}

	return texture;
}

// thread-safe
shared_ptr<Texture> Renderer::CreateTexture(int width, int height, const sf::Color &fill)
{
	lock_guard<recursive_mutex> guard(mMutex);

	auto texture = make_shared<Texture>();
	texture->AllocPixels(width, height);
	for (int i = 0; i < width * height; ++i)
	{
		texture->mPixels[i] = fill;
	}

	return texture;
}

shared_ptr<sf::Font> Renderer::GetFont(const string &path)
{
	shared_ptr<sf::Font> font;
	auto existing = mFontList.find(path);
	if (existing == mFontList.end())
	{
		font = make_shared<sf::Font>();
		font->loadFromFile(path);
		mFontList[path] = font;
	}
	else
	{
		font = existing->second;
	}
	return font;
}

string Renderer::GetFontName(const shared_ptr<sf::Font> &font)
{
	for (auto &kv : mFontList)
	{
		if (kv.second == font)
		{
			return kv.first;
		}
	}
	return "<nullptr>";
}

void Renderer::RenderQuads(const sf::Transform &worldTransform, const vector<sf::Vertex> &vertices)
{
	if (vertices.size())
	{
		const char *data = reinterpret_cast<const char *>(&vertices[0]);
		GLCall(glClientActiveTexture(GL_TEXTURE1));
		GLCall(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
		GLCall(glMatrixMode(GL_MODELVIEW));
		GLCall(glLoadMatrixf(worldTransform.getMatrix()));
		GLCall(glVertexPointer(2, GL_FLOAT, sizeof(sf::Vertex), data + offsetof(sf::Vertex, position)));
		GLCall(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(sf::Vertex), data + offsetof(sf::Vertex, color)));
		GLCall(glClientActiveTexture(GL_TEXTURE0));
		GLCall(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
		GLCall(glTexCoordPointer(2, GL_FLOAT, sizeof(sf::Vertex), data + offsetof(sf::Vertex, texCoords)));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, vertices.size()));
		++mDrawCallCount;
	}
}

Vec2f Renderer::GetScaleRatio()
{
	const float kx = (game->VirtualScreen.getSize().x / mWindow.getDefaultView().getSize().x);
	const float ky = (game->VirtualScreen.getSize().y / mWindow.getDefaultView().getSize().y);
	return { kx, ky };
}

Vec2f Renderer::ToVirtualScreen(const Vec2f &v) const
{
	const float kx = game->VirtualScreen.getSize().x / mWindow.getDefaultView().getSize().x;
	const float ky = game->VirtualScreen.getSize().y / mWindow.getDefaultView().getSize().y;
	return { v.x * kx, v.y * ky };
}

void Renderer::SetResolution(const sf::VideoMode &videoMode)
{
	mConfig.mWidth = videoMode.width;
	mConfig.mHeight = videoMode.height;

	Restart();
}

void Renderer::SetFullscreen(bool fullscreen)
{
	mConfig.mFullscreen = fullscreen;

	Restart();
}

void Renderer::ScheduleTextureGPUUpload(const shared_ptr<Texture> &texture)
{
	lock_guard<recursive_mutex> guard(mMutex);
	auto notScheduled = find(mTextureGPUUploadList.begin(), mTextureGPUUploadList.end(), texture) == mTextureGPUUploadList.end();
	if (notScheduled)
	{
		mTextureGPUUploadList.emplace_back(texture);
	}
}

void Renderer::ScheduleTextureGPUDeletion(GLuint textureId)
{
	lock_guard<recursive_mutex> guard(mMutex);
	mTextureGPUDeletionList.push_back(textureId);
}

void Renderer::RegisterTexture(const shared_ptr<Texture> &tex)
{
	if (tex->mSource.size())
	{
		mTextureList[tex->mSource] = tex;
	}
	else
	{
		throw Exception(StringBuilder(__func__) << " - Unnamed texture cannot be registered!");
	}
}

int Renderer::GetDrawCallCount() const
{
	return mDrawCallCount;
}

Vec2f Renderer::WorldToDisplay(const Vec2f &v) const
{
	return Vec2f(mWindow.mapCoordsToPixel(v, mWindow.getDefaultView()));
}

GLuint Renderer::GetDummyTexture() const
{
	return mDummyTexture;
}

void Renderer::BeginImGuiRender()
{
	ImGui::NewFrame();
	ui::PrepareFrame();
}

recursive_mutex &Renderer::GetMutex()
{
	return mMutex;
}

void Renderer::EndImGuiRender()
{
	ImGui::Render();
}

shared_ptr<Texture> Renderer::RenderToTexture(const shared_ptr<SceneNode> &root, Vec2f *boundsMin, Vec2f *boundsMax)
{
	// extract hierarchy
	vector<shared_ptr<SceneNode>> nodes;
	root->ExtractAllChildren(nodes);

	// leave only sprites
	vector<shared_ptr<Sprite>> sprites;
	for (auto &node : nodes)
	{
		if (auto sprite = dynamic_pointer_cast<Sprite>(node))
		{
			if (sprite->IsTrulyVisible())
			{
				sprites.push_back(sprite);
			}
		}
	}

	nodes.clear();

	// sort sprites by render order
	auto sorter = [] (const shared_ptr<Sprite> &a, const shared_ptr<Sprite> &b)
	{
		return a->GetRenderOrder() < b->GetRenderOrder();
	};
	sort(sprites.begin(), sprites.end(), sorter);

	// calculate bounds
	Vec2f min = { FLT_MAX, FLT_MAX };
	Vec2f max = { FLT_MIN, FLT_MIN };
	for (auto &sprite : sprites)
	{
		Math::RotatedRectangle r(sprite->GetSize(), sprite->GetGlobalTransform());
		for (auto &pt : r.points)
		{
			min.x = Math::Min(min.x, pt.x);
			min.y = Math::Min(min.y, pt.y);
			max.x = Math::Max(max.x, pt.x);
			max.y = Math::Max(max.y, pt.y);
		}
	}

	if (boundsMin)
	{
		*boundsMin = min;
	}
	if (boundsMax)
	{
		*boundsMax = max;
	}

	// calculate view position and texture size
	Vec2f viewPos = (min + max) * 0.5f;
	int texWidth = static_cast<int>(max.x - min.x);
	int texHeight = static_cast<int>(max.y - min.y);

	// create texture
	GLuint tex;
	GLCall(glGenTextures(1, &tex));
	GLCall(glBindTexture(GL_TEXTURE_2D, tex));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

	// bind texture to FBO
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFBO1));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0));

	// render
	sf::View view;
	view.setCenter(viewPos);
	view.setSize(static_cast<float>(texWidth), static_cast<float>(-texHeight));
	GLCall(glClearColor(0, 0, 0, 0));
	GLCall(glViewport(0, 0, texWidth, texHeight));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
	GLCall(glMatrixMode(GL_PROJECTION));
	GLCall(glLoadMatrixf(view.getTransform().getMatrix()));
	GLCall(glMatrixMode(GL_MODELVIEW));
	GLCall(glLoadIdentity());

	// bind shader
	sf::Shader::bind(&mSimpleShader.mShader);
	GLCall(glUniform1i(mSimpleShader.mUDiffuseTexture, 0));

	for (auto &sprite : sprites)
	{
		// bind diffuse texture
		GLCall(glActiveTexture(GL_TEXTURE0));
		if (auto diffuseTex = sprite->GetTexture())
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, diffuseTex->mGLTexture));
		}
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, mDummyTexture));
		}

		// bind decal texture
		GLCall(glActiveTexture(GL_TEXTURE1));
		if (auto decalTex = sprite->GetDecalTexture())
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, decalTex->mGLTexture));
		}
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, mDummyDecal));
		}

		RenderSprite(sprite);
	}

	// set back
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFrameTex1, 0));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	// extract texture from GPU
	shared_ptr<Texture> texture = CreateTexture(texWidth, texHeight);
	glBindTexture(GL_TEXTURE_2D, tex);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->mPixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	ScheduleTextureGPUUpload(texture);

	return texture;
}

const RendererConfig & Renderer::GetConfig() const
{
	return mConfig;
}
