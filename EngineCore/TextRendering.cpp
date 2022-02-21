#include "TextRendering.h"

#include "ConsoleLogging.h"
#include "Engine.h"
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

namespace txt
{
	FT_Library Library;
	int FontCount = 0;
	std::unordered_map < std::string, std::shared_ptr<FontData> > Fonts;
	std::shared_ptr<Shader> DefaultTextShader;

	GLuint VAO, VBO;

	void Init()
	{
		if (FT_Init_FreeType(&Library))
		{
			LogError("Could not initialise FreeType library.", false);
		}

		try
		{
			DefaultTextShader = Shaders.at("text");
		}
		catch (const std::out_of_range&)
		{
			LogError("Default text shader was not found. txt initialisation failed", false);
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void LoadFont(std::string path, std::string name)
	{
		FT_Face face;
		FT_Error err = FT_New_Face(Library, path.c_str(), FontCount, &face);
		if (err)
		{
			LogError("Failed to load font at " + path + " Error Code: " + std::to_string(err), false);
			for (const auto& entry : std::filesystem::directory_iterator(".\\Fonts"))
			{
				LogInfo(entry.path().string());
			}

			return;
		}
		if (name == "")
		{
			name = path.substr(path.find_last_of("\\") + 1);
			auto extensionStart = name.find_last_of('.');
			name = name.substr(0, extensionStart);
		}
		
		std::shared_ptr<FontData> fontData = std::shared_ptr<FontData>(new FontData());
		fontData->font = face;

		FT_Set_Pixel_Sizes(face, 0, 48);

		GLuint textures[128];
		glGenTextures(128, textures);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (int c = 0; c < 128; c++)
		{
			if (FT_Error err = FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				LogError("Failed to load glyph " + std::to_string(c) + " Err: " + std::to_string(err), false);
				continue;
			}

			glBindTexture(GL_TEXTURE_2D, textures[c]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			CharacterData character = { 
				textures[c],
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x 
			};

			fontData->Characters[c] = character;
		}

		Fonts.insert({ name, fontData });

		FT_Done_Face(face);
	}

	void PrintLoadedFonts()
	{
		std::string output;

		output += "Loaded fonts:";

		for (auto& it : Fonts)
		{
			output += "\n\t" + it.first;
		}

		LogInfo(output);
	}

	void RenderText(std::string text, std::string fontName, float x, float y, float scale, glm::vec3 colour)
	{
		DefaultTextShader->Activate();

		glm::mat4 projection = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight);

		glUniformMatrix4fv(glGetUniformLocation(DefaultTextShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(glGetUniformLocation(DefaultTextShader->ID, "textColour"), colour.x, colour.y, colour.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);

		std::shared_ptr<FontData> font;

		try
		{
			font = Fonts.at(fontName);
		}
		catch (const std::out_of_range&)
		{
			LogError("Font " + fontName + " does not exist or is not loaded.", false);
			PrintLoadedFonts();
			return;
		}

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			CharacterData charData = font->Characters[*c];

			float xPos = x + charData.Bearing.x * scale;
			float yPos = y - (charData.Size.y - charData.Bearing.y) * scale;

			float w = charData.Size.x * scale;
			float h = charData.Size.y * scale;

			float vertices[6][4] = {
				{ xPos,     yPos + h,   0.0f, 0.0f },
				{ xPos,     yPos,       0.0f, 1.0f },
				{ xPos + w, yPos,       1.0f, 1.0f },

				{ xPos,     yPos + h,   0.0f, 0.0f },
				{ xPos + w, yPos,       1.0f, 1.0f },
				{ xPos + w, yPos + h,   1.0f, 0.0f }
			};

			glBindTexture(GL_TEXTURE_2D, charData.TextureID);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			x += (charData.Advance >> 6) * scale;
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}