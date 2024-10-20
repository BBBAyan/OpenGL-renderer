#pragma once

#include "Test.h"

#include "vertexBuffer.h"
#include "vertexBufferLayout.h"

namespace test {

	class TestRotatingCube : public Test
	{
	public:
		TestRotatingCube();
		~TestRotatingCube();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		glm::mat4 m_Proj, m_View;

		float angley{ 0.5f };
		float anglex{ 0.5f };

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
	};
};