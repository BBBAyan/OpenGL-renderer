#pragma once

#include "Test.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include "Control.h"

#include "vertexBuffer.h"
#include "vertexBufferLayout.h"

namespace test {

	class TestModel : public Test
	{
	public:
		TestModel(GLFWwindow* window);
		~TestModel();

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
		glm::vec3 lightPos = { 1.0f, 2.0f, 2.0f };
		float dirlightIntensity = 1.0f;

		Control m_controls;
		Camera m_camera;
		GLFWwindow* m_window;

		std::unique_ptr<Model> m_Model;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Shader> m_ShaderLight;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	};
};