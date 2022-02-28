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

/// <summary>
/// OpenGL text rendering.
/// Note: This is quite primative and uses a statically sized bitmap of the font.
/// </summary>
namespace txt
{
	/// <summary>
	/// Data for a single font character.
	/// </summary>
	struct CharacterData
	{
		GLuint TextureID;		// ID handle of the glyph texture
		glm::ivec2   Size;      // Size of glyph
		glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
		GLuint Advance;			// Offset to advance to next glyph
	};
	/// <summary>
	/// Data for a font, including all characters.
	/// </summary>
	struct FontData
	{
		FT_Face font;
		std::array<CharacterData, 128> Characters;
	};

	/// <summary>
	/// The FreeType library object.
	/// </summary>
	extern FT_Library Library;
	/// <summary>
	/// Hash map of fonts by their name.
	/// </summary>
	extern std::unordered_map < std::string, std::shared_ptr<FontData> > Fonts;

	/// <summary>
	/// Initialise the text renderer and font library.
	/// </summary>
	void Init();
	/// <summary>
	/// Load a font from a given path and add it to the map under 'name'.
	/// Uses the file name if no name is provided.
	/// </summary>
	/// <param name="path"></param>
	/// <param name="name"></param>
	void LoadFont(std::string path, std::string name = "");

	/// <summary>
	/// Render an instance of text to the screen using the provided font.
	/// </summary>
	/// <param name="text"></param>
	/// <param name="fontName"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="scale"></param>
	/// <param name="colour"></param>
	void RenderText(std::string text, std::string fontName, float x, float y, float scale, glm::vec3 colour);
}