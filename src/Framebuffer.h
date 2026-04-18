#pragma once

#include "Renderer.h"

class Framebuffer {
private:
	unsigned int fbo, rbo, textureColorBuffer;
	unsigned int quadVAO, quadVBO;
	int m_width, m_height;
public:
	Framebuffer(int width, int height);
	~Framebuffer();

	void Bind() const;
	void Unbind() const;
	void Draw() const;

	unsigned int getTextureColorBuffer() { return textureColorBuffer; };
};