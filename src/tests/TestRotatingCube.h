#pragma once

#include "Test.h"
#include "Texture.h"
#include "Camera.h"
#include "Control.h"

#include <glm/glm.hpp>
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"

namespace test {
	class TestRotatingCube : public Test
	{
	public:
		TestRotatingCube(GLFWwindow* window);
		~TestRotatingCube();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		test::Camera ProcessInput(Camera f_camera);
	private:
		glm::mat4 m_Proj, m_View;
		enum FocusState {
			CAMERA,
			UI
		};
		struct Color {
			float r, g, b;
		};
		FocusState curState = UI;
		Color clearColor = { 0.1f, 0.1f, 0.1f };
		Color lightColor = { 1.0f, 1.0f, 1.0f };
		float dirlightIntensity = 1.0f;

		Control m_controls;
		Camera m_camera;
		GLFWwindow* m_window;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Shader> m_ShaderLight;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<Texture> m_Texture_Specular;
		std::unique_ptr<Texture> m_Texture_Matrix;
		std::unique_ptr<Texture> m_Texture_Light;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

		std::unique_ptr<VertexArray> m_VAO_Land;
		std::unique_ptr<Shader> m_Shader_Land;
		std::unique_ptr<Texture> m_Texture_Land;
		std::unique_ptr<Texture> m_Texture_Spotlight;
		std::unique_ptr<IndexBuffer> m_IndexBuffer_Land;
		std::unique_ptr<VertexBuffer> m_VertexBuffer_Land;
	};
};