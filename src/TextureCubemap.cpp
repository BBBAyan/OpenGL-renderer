#include "TextureCubemap.h"

#include <iostream>
#include "stb_image/stb_image.h"

TextureCubemap::TextureCubemap(const std::vector<std::string>& textures_faces)
	: m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(0);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	for (unsigned int i = 0; i < textures_faces.size(); i++)
	{
		unsigned char* data = stbi_load(textures_faces[i].c_str(), &m_Width, &m_Height, &m_BPP, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap didn't loaded at path: " << textures_faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	//GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

TextureCubemap::~TextureCubemap()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void TextureCubemap::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
}

void TextureCubemap::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}