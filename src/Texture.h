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

class Game;
class Serializer;

// RGBA8 Texture
// this class does not upload texture to GPU, but holds valid id of it
class Texture : public enable_shared_from_this<Texture>
{
public:
	Texture();
	~Texture();
	void AllocPixels(int w, int h);
	sf::Color *GetPixels();
	void Serialize(Serializer &sav);
	Vec2f GetSize() const;
	void PushToUpdateQueue();
	// Do not forget to push texture to update queue in the renderer after any DrawXXX operation
	void DrawRect(const sf::FloatRect &r, const sf::Color &clr);
	void DrawCircle(const Vec2f &p, float radius, const sf::Color &clr);
	void DrawPixel(const Vec2f & p, const sf::Color & clr);
	// Input rect MUST be in normalized coordinates [0; 1]
	shared_ptr<Texture> ExtractPiece(const sf::FloatRect &r);
	int GetWidth() const;
	int GetHeight() const;
	string GetSource() const;
	GLuint GetOpenGLTexture() const;
	void SetRepeated(bool state);
	bool IsRepeated() const;
	void SetSmooth(bool state);
	bool IsSmooth() const;
	float GetAspectRatio() const;
private:
	// The renderer can operate with texture without any restrictions
	friend class Renderer;
	// Trying to load texture from file to RAM, does not load texture on GPU
	// use Renderer::ScheduleTextureGPUUpload to push texture to upload queue
	void LoadPixelsFromFile(const string &path, const sf::Color &mask = sf::Color(255, 0, 255));
	// Pixels array. Always valid while Texture exists, it is needed to avoid redundant transfer from VRAM to RAM
	// in cost of double memory consumption (1x on RAM, 1x on VRAM) - not a big problem on modern hardware
	sf::Color *mPixels { nullptr };
	// Bounds
	int mWidth { 0 };
	int mHeight { 0 };
	// Path to source file
	string mSource;
	// OpenGL's Id of texture
	GLuint mGLTexture { 0 };
	// Is linear filtering enabled?
	bool mSmooth { false };
	// Is wrapping enabled?
	bool mRepeated { false };
};
