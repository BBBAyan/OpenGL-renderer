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

		void HDRSetup();
	private:
		glm::mat4 m_Proj, m_View, model, m_Proj2D, lightSpaceMatrix;
		glm::mat4* modelMatrices; int rockAmount;
		glm::mat4 objectModel, floorModel, wallModel, wallModelParallax;
		glm::mat3 normalMatrixFloor, normalMatrixWall;
		std::vector<glm::mat4> pointShadowMatrices, boxModels;
		struct Color { float r, g, b; };
		struct DirLight {
			glm::vec3 direction;
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			float intensity;
		};
		struct PointLight {
			std::vector<glm::vec3> positions;
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			float constant;
			float linear;
			float quadratic;
		};
		DirLight directionalLight;
		PointLight pointLight;
		Color clearColor = { 0.1f, 0.1f, 0.1f };
		Color lightColor = { 1.0f, 1.0f, 1.0f };
		glm::vec3 planetPos, modelPos = { 0.0f, 0.0f, 0.0f };
		glm::vec3 tangent1, tangent2, bitangent1, bitangent2;
		float m_lastTime;
		float m_animationTime;
		float near_plane, far_plane, exposure;
		float shininess = 32.0f;
		bool draw_cubemap, blinn, hdr, prev_hdr, exposure_adaptation;

		int currentIndex;
		int modelIndex, SHADOW_WIDTH, SHADOW_HEIGHT;
		unsigned int cubemapVAO, cubemapVBO;
		unsigned int ubo;

		Control m_controls;
		Camera m_camera;
		GLFWwindow* m_window;

		std::unique_ptr<Model> m_ModelPlanet;
		std::unique_ptr<Model> m_ModelAsteroid;
		std::unique_ptr<Shader> m_ShaderCube;
		std::unique_ptr<Shader> m_ShaderCubemap;
		std::unique_ptr<Shader> m_ShaderExplode;
		std::unique_ptr<Shader> m_ShaderFloor;
		std::unique_ptr<Shader> m_ShaderWall;
		std::unique_ptr<Shader> m_ShaderWallParallax;
		std::unique_ptr<Shader> m_ShaderFramebuffer;
		std::unique_ptr<Shader> m_ShaderFramebufferDouble;
		std::unique_ptr<Shader> m_ShaderFramebufferBloom;
		std::unique_ptr<Shader> m_ShaderGeometry;
		std::unique_ptr<Shader> m_ShaderLight;
		std::unique_ptr<Shader> m_ShaderModel;
		std::unique_ptr<Shader> m_ShaderModelAsteroid;
		std::unique_ptr<Shader> m_ShaderNormal;
		std::unique_ptr<Shader> m_ShaderReflectiveCube;
		std::unique_ptr<Shader> m_ShaderDirShadow;
		std::unique_ptr<Shader> m_ShaderPointShadow;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<Texture> m_TextureFloor;
		std::unique_ptr<Texture> m_TextureBrick;
		std::unique_ptr<Texture> m_TextureBrickNormal;
		std::unique_ptr<Texture> m_TextureBrickDisplacement;
		std::unique_ptr<Texture> m_TextureGrass;
		std::unique_ptr<Texture> m_TextureSpecular;
		std::unique_ptr<Texture> m_TextureWindow;
		std::unique_ptr<TextureCubemap> m_TextureCubemap;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexArray> m_VAO_Point;
		std::unique_ptr<VertexArray> m_VAO_Floor;
		std::unique_ptr<VertexArray> m_VAO_Square;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer_Point;
		std::unique_ptr<VertexBuffer> m_VertexBuffer_Floor;
		std::unique_ptr<VertexBuffer> m_VertexBuffer_Square;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBufferFloor;
		std::unique_ptr<IndexBuffer> m_IndexBufferSquare;
		std::unique_ptr<Framebuffer> m_FramebufferIntermediate;
		std::unique_ptr<Framebuffer> m_FramebufferDouble;
		std::unique_ptr<Framebuffer> m_FramebufferBloomHorizontal;
		std::unique_ptr<Framebuffer> m_FramebufferBloomVertical;
		std::unique_ptr<Framebuffer> m_FramebufferFinal;
		std::unique_ptr<Framebuffer> m_FramebufferDirShadow;
		std::unique_ptr<Framebuffer> m_FramebufferPointShadow;
		std::unique_ptr<Framebuffer> m_FramebufferMultisample;
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