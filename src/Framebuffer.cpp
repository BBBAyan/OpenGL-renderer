#include <iostream>
#include "Framebuffer.h"

static const float quadVertices[] = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};

Framebuffer::Framebuffer(int width, int height, int samples, int shadowmode, bool hdr)
    : m_width(width), m_height(height), m_samples(samples), m_shadowmode(shadowmode), m_hdr(hdr)
{
    GLCall(glGenVertexArrays(1, &quadVAO));
    GLCall(glGenBuffers(1, &quadVBO));

    GLCall(glBindVertexArray(quadVAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, quadVBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));

    if (samples <= 0) {
        GLCall(glGenFramebuffers(1, &fbo));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

        if (shadowmode == 0) {
            GLCall(glGenTextures(1, &textureBuffer));
            GLCall(glBindTexture(GL_TEXTURE_2D, textureBuffer));
            if (hdr) {
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
            } else {
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
            }
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0));
            GLCall(glGenRenderbuffers(1, &rbo));
            GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));

            GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
            GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
        }else if (shadowmode == 1) {
            GLCall(glGenTextures(1, &textureBuffer));
            GLCall(glBindTexture(GL_TEXTURE_2D, textureBuffer));
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
            //GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
            //GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureBuffer, 0));
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
		} else if (shadowmode == 2) {
            GLCall(glGenTextures(1, &textureBuffer));
            GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureBuffer));
            for (int i = 0; i < 6; i++) {
                GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
            }
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
            GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureBuffer, 0));
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE" << std::endl;

        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
    else {
        GLCall(glGenFramebuffers(1, &fbo));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
        GLCall(glGenTextures(1, &textureBuffer));
        GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureBuffer));
        if (hdr) {
            GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB16F, width, height, GL_TRUE));
        } else {
            GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE));
        }
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureBuffer, 0));

        GLCall(glGenRenderbuffers(1, &rbo));
        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));

        GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height));
        GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "GL_FRAMEBUFFER_MULTISAMPLE_INCOMPLETE" << std::endl;

        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &textureBuffer);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}

void Framebuffer::Bind() const {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void Framebuffer::Unbind() const {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::Draw() const {
    GLCall(glBindVertexArray(quadVAO));
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}