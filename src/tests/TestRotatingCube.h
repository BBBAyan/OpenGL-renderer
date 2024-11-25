#pragma once

#include "Test.h"

#include "glm/glm.hpp"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"

namespace test {
	struct Camera
	{
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		float Speed;
	};

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

		float angley{ 0.5f };
		float anglex{ 0.5f };

		Camera camera;

		GLFWwindow* window;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
	};
};