#pragma once

#include "Renderer.h"

class Framebuffer {
private:
	unsigned int fbo, rbo, textureBuffer;
	unsigned int quadVAO, quadVBO;
	int m_width, m_height, m_samples, m_shadowmode; // 0 - no shadow, 1 - directional shadow, 2 - point shadow
public:
	Framebuffer(int width, int height, int samples = 0, int shadowmode = 0);
	~Framebuffer();

	void Bind() const;
	void Unbind() const;
	void Draw() const;

	unsigned int getTextureBuffer() { return textureBuffer; };
	unsigned int getFBO() { return fbo; };
};