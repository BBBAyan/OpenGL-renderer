#pragma once

#include "Test.h"

#include "Renderer.h"
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

		Camera ProcessInput(GLFWwindow* &window, Camera f_camera) {
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				f_camera.Position += f_camera.Speed * f_camera.Front;
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				f_camera.Position -= f_camera.Speed * f_camera.Front;
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				f_camera.Position -= glm::normalize(glm::cross(f_camera.Front, f_camera.Up)) * f_camera.Speed;
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				f_camera.Position += glm::normalize(glm::cross(f_camera.Front, f_camera.Up)) * f_camera.Speed;
			return f_camera;
		}
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