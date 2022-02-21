#pragma once

#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <GL/glew.h>
#include <glm/ext/vector_int2.hpp>
#include <glm/vec3.hpp>
#include <array>
#include <memory>

namespace txt
{
	struct CharacterData
	{
		GLuint TextureID;		// ID handle of the glyph texture
		glm::ivec2   Size;      // Size of glyph
		glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
		GLuint Advance;			// Offset to advance to next glyph
	};
	struct FontData
	{
		FT_Face font;
		std::array<CharacterData, 128> Characters;
	};

	extern FT_Library Library;
	extern std::unordered_map < std::string, std::shared_ptr<FontData> > Fonts;

	void Init();
	void LoadFont(std::string path, std::string name = "");

	void RenderText(std::string text, std::string fontName, float x, float y, float scale, glm::vec3 colour);
}