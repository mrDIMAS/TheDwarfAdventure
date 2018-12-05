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

#include "Texture.h"
#include "Cell.h"

class Light;
class SceneNode;
class Scene;
class TextureStack;

#if 1 // set this to something not equal zero if you expiriencing troubles with
// GL code
#define ENABLE_GL_DEBUG
#endif

#ifdef ENABLE_GL_DEBUG
#define GLCall(func)                                                                               \
  func;                                                                                            \
  if (glGetError() != GL_NO_ERROR) {                                                               \
    throw Exception(StringBuilder("OpenGL Error in ") << __FILE__ << " at line " << __LINE__); \
  }
#else
#define GLCall(func) func;
#endif

struct RendererConfig
{
	int mWidth { 1366 };
	int mHeight { 768 };
	float mVirtualScreenSize { 1200 };
	bool mFullscreen { false };
	int mFrameRateLimit { 0 };
	bool mFrameProfilerEnabled { false };
	bool mBorderlessWindow { true };
};

class Renderer
{
private:
	sf::RenderWindow mWindow;

	// holds textures that was loaded from file
	unordered_map<string, weak_ptr<Texture>> mTextureList;
	unordered_map<string, shared_ptr<sf::Font>> mFontList;
	// holds textures that waits uploading to gpu
	vector<shared_ptr<Texture>> mTextureGPUUploadList;
	vector<GLuint> mTextureGPUDeletionList;
	vector<shared_ptr<Light>> mNearestLights;
	shared_ptr<Texture> mImGuiFontAtlas;
	RendererConfig mConfig;

	mutable recursive_mutex mMutex;

	// some dummy textures
	GLuint mDummyTexture { 0 };
	GLuint mDummyDecal { 0 };

	GLuint mVisibilityGridTexture { 0 };

	// internal routines
	void LoadLightsIntoShaderIntersectsWithRect(const shared_ptr<Scene> &scene, const sf::FloatRect &rect, GLint uLightCount, GLint uLightPosRadius, GLint uLightColor);


	void FillQuad(vector<sf::Vertex> &vertices, const Vec2f &pos, const Vec2f &size, const sf::Color &color, const sf::FloatRect &texCoord);
	void UploadScheduledTexturesToGPU();
	void CreateSpecialTextures();
	void SetupVirtualScreen(float width);
	void LoadShadersAndGetUniforms();
	void LoadTextureStackOnGPU(const shared_ptr<TextureStack> &stck);
	void InitImGui();
	void RenderImGui();
	void UpdateImGuiInput(const sf::Event &event);
	int mDrawCallCount { 0 };

	struct
	{
		sf::Shader mShader;
		// uniforms
		GLint mUDiffuseTexture { 0 };
		GLint mULightPosRadius { 0 };
		GLint mULightColor { 0 };
		GLint mULightCount { 0 };
		GLint mUTopBackgroundlessPartHeight { 0 };
		GLint mUBackgroundMask { 0 };
		GLint mUBackgroundStack { 0 };
		GLint mUAmbientColor { 0 };
		GLint mUBackgroundStackSize { 0 };
	} mTerrainShader;

	struct
	{
		sf::Shader mShader;
		// uniforms
		GLint mUDiffuseTexture { 0 };
		GLint mUDecalTexture { 0 };
		GLint mULightingEnabled { 0 };
		GLint mULightPosRadius { 0 };
		GLint mULightColor { 0 };
		GLint mULightCount { 0 };
		GLint mUTopBackgroundlessPartHeight { 0 };
		GLint mUAmbientColor { 0 };
	} mSpriteShader;

	struct
	{
		sf::Shader mShader;
		// uniforms
		GLint mUDiffuseTexture { 0 };
	} mSimpleShader;

	struct
	{
		sf::Shader mShader;
		// uniforms
		GLint mUDiffuseTexture { 0 };
		GLint mUdxdy { 0 };
	} mWaterBlurShader;

	struct
	{
		sf::Shader mShader;
		// uniforms
		GLint mUDiffuseTexture { 0 };
	} mWaterContrastShader;

	array<sf::Color, Cell::PixelCount> mCellBuffer;
	vector<sf::Vertex> mQuadVertices;

	// framebuffers
	sf::Vector2i mFBOSize;
	GLuint mFBO1 { 0 };
	GLuint mFBO2 { 0 };
	GLuint mFrameTex1 { 0 };
	GLuint mFrameTex2 { 0 };
	GLuint mSceneFBO { 0 };
	void SetupFrameBuffer(int fboWidth, int fboHeight);
	void RenderSprite(const shared_ptr<Sprite> &sprite);
	int mFrameRateLimit { 0 };

	vector<sf::Vertex> mBlobVertices;

	chrono::time_point<chrono::high_resolution_clock> mProfilerStartTime;
	void BuiltInProfiler_BeginProfiling();
	void BuiltInProfiler_EndProfiling(const string & msg);
	//auto lastTime = chrono::high_resolution_clock::now();

	void Restart();
public:
	sf::Color FrameBufferClearColor;
	unsigned int mFrameTime { 0 };
	Renderer(const RendererConfig & config);
	~Renderer();
	// thread-safe
	void Render();
	void ProcessEvents();
	bool IsActive() const;
	sf::RenderWindow &GetWindow();
	// thread-safe
	shared_ptr<Texture> RequestTexture(const string &path, const sf::Color &mask = sf::Color(255, 0, 255));
	// thread-safe
	shared_ptr<Texture> CreateTexture(int width, int height, const sf::Color &fill = sf::Color(255, 255, 255, 255));
	shared_ptr<sf::Font> GetFont(const string &path);
	string GetFontName(const shared_ptr<sf::Font> &font);
	void RenderQuads(const sf::Transform &worldTransform, const vector<sf::Vertex> &vertices);
	Vec2f GetScaleRatio();
	Vec2f ToVirtualScreen(const Vec2f &v) const;
	void SetResolution(const sf::VideoMode &videoMode);
	void SetFullscreen(bool fullscreen);
	// thread-safe
	void ScheduleTextureGPUUpload(const shared_ptr<Texture> &texture);
	void ScheduleTextureGPUDeletion(GLuint textureId);
	void RegisterTexture(const shared_ptr<Texture> &tex);
	int GetDrawCallCount() const;
	Vec2f WorldToDisplay(const Vec2f &v) const;
	GLuint GetDummyTexture() const;
	void BeginImGuiRender();
	// returns mutex for in-place renderer-usercode sync
	recursive_mutex &GetMutex();
	void EndImGuiRender();
	// will render all hierarchy!
	shared_ptr<Texture> RenderToTexture(const shared_ptr<SceneNode> &root, Vec2f *boundsMin = nullptr, Vec2f *boundsMax = nullptr);
	const RendererConfig & GetConfig() const;
};
