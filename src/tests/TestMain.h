#pragma once

#include "Test.h"
#include "Texture.h"
#include "TextureCubemap.h"
#include "Model.h"
#include "Camera.h"
#include "Control.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Framebuffer.h"

namespace test {

	class TestMain : public Test
	{
	public:
		TestMain(GLFWwindow* window);
		~TestMain();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		Control& getControls() { return m_controls; }
		void ProcessInput(Camera& camera);
	private:
		glm::mat4* modelMatrices; int rockAmount;
		glm::mat4 m_Proj, m_View, m_Proj2D;
		struct Color {
			float r, g, b;
		};
		Color clearColor = { 0.1f, 0.1f, 0.1f };
		Color lightColor = { 0.54f, 0.0f, 1.0f };
		glm::vec3 lightPos = { 1.0f, 2.0f, 2.0f };
		glm::vec3 modelPos = { 0.0f, 0.0f, 0.0f };
		glm::vec3 planetPos;
		float dirlightIntensity = 1.0f;
		float m_lastTime;
		float m_animationTime;
		bool draw_cubemap, draw_house, draw_quads;

		int currentIndex;
		int modelIndex;
		unsigned int cubemapVAO, cubemapVBO;
		unsigned int ubo, instanceVBO, instanceRockVBO;

		Control m_controls;
		Camera m_camera;
		GLFWwindow* m_window;

		std::unique_ptr<Model> m_ModelPlanet;
		std::unique_ptr<Model> m_ModelAsteroid;
		std::unique_ptr<Shader> m_ShaderCubemap;
		std::unique_ptr<Shader> m_ShaderExplode;
		std::unique_ptr<Shader> m_ShaderFramebuffer;
		std::unique_ptr<Shader> m_ShaderGeometry;
		std::unique_ptr<Shader> m_ShaderLight;
		std::unique_ptr<Shader> m_ShaderModel;
		std::unique_ptr<Shader> m_ShaderModelAsteroid;
		std::unique_ptr<Shader> m_ShaderNormal;
		std::unique_ptr<Shader> m_ShaderReflectiveCube;
		std::unique_ptr<Shader> m_ShaderQuad;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<Texture> m_TextureGrass;
		std::unique_ptr<Texture> m_TextureSpecular;
		std::unique_ptr<Texture> m_TextureWindow;
		std::unique_ptr<TextureCubemap> m_TextureCubemap;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexArray> m_VAO_Point;
		std::unique_ptr<VertexArray> m_VAO_Quad;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer_Point;
		std::unique_ptr<VertexBuffer> m_VertexBuffer_Quad;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Framebuffer> m_Framebuffer;
		const char* inputModeNames[3] = {
			"GLFW_CURSOR_NORMAL",
			"GLFW_CURSOR_HIDDEN",
			"GLFW_CURSOR_DISABLED"
		};

		const int inputModeValues[3] = {
			GLFW_CURSOR_NORMAL,
			GLFW_CURSOR_HIDDEN,
			GLFW_CURSOR_DISABLED
		};
	};
};