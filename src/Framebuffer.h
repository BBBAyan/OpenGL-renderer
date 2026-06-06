#pragma once

#include "Renderer.h"

class Framebuffer {
private:
	unsigned int fbo, rbo, textureBuffer;
	unsigned int textureBuffers[2];
	unsigned int quadVAO, quadVBO;
	int m_width, m_height, m_samples, m_shadowmode, m_hdr, m_db, m_layers; // 0 - no shadow, 1 - directional shadow, 2 - point shadow
public:
	Framebuffer(int width, int height, int samples = 0, int shadowmode = 0, bool hdr = 0, bool doublebuffer = 0, int layers = 1);
	~Framebuffer();

	void Bind() const;
	void Unbind() const;
	void Draw() const;

	void BindCubeFace(int cube, int face) const;

	unsigned int getTextureBuffer() { return textureBuffer; };
	unsigned int getTextureBuffer(int i) { return textureBuffers[i]; };
	unsigned int getFBO() { return fbo; };
};