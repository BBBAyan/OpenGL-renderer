#include "TestRotatingCube.h"

#include "Renderer.h"
#include <imgui/imgui.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
    TestRotatingCube::TestRotatingCube()
        : m_Proj(glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, -500.0f, 500.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0)))
	{
        float positions[] = {
            //front blue
              -100.0f,  100.0f, -100.0f, 0.0f, 0.0f, 1.0f, //Left Top
              -100.0f, -100.0f, -100.0f, 0.0f, 0.0f, 1.0f, //Left Bottom
               100.0f, -100.0f, -100.0f, 0.0f, 0.0f, 1.0f, //Right Bottom
               100.0f,  100.0f, -100.0f, 0.0f, 0.0f, 1.0f, //Right Top

            //top white
              -100.0f,  100.0f,  100.0f, 1.0f, 1.0f, 1.0f, //Left Back
              -100.0f,  100.0f, -100.0f, 1.0f, 1.0f, 1.0f, //Left Front
               100.0f,  100.0f, -100.0f, 1.0f, 1.0f, 1.0f, //Right Front
               100.0f,  100.0f,  100.0f, 1.0f, 1.0f, 1.0f, //Right Back

            //back green
              -100.0f,  100.0f,  100.0f, 0.0f, 0.5f, 0.0f, //Left Top
              -100.0f, -100.0f,  100.0f, 0.0f, 0.5f, 0.0f, //Left Bottom
               100.0f, -100.0f,  100.0f, 0.0f, 0.5f, 0.0f, //Right Bottom
               100.0f,  100.0f,  100.0f, 0.0f, 0.5f, 0.0f, //Right Top

            //bottom yellow
              -100.0f, -100.0f,  100.0f, 1.0f, 1.0f, 0.0f, //Left Back
              -100.0f, -100.0f, -100.0f, 1.0f, 1.0f, 0.0f, //Left Front
               100.0f, -100.0f, -100.0f, 1.0f, 1.0f, 0.0f, //Right Front
               100.0f, -100.0f,  100.0f, 1.0f, 1.0f, 0.0f, //Right Back
            
            //left red
              -100.0f,  100.0f,  100.0f, 1.0f, 0.0f, 0.0f, //Top Back
              -100.0f, -100.0f,  100.0f, 1.0f, 0.0f, 0.0f, //Bottom Back
              -100.0f, -100.0f, -100.0f, 1.0f, 0.0f, 0.0f, //Bottom Front
              -100.0f,  100.0f, -100.0f, 1.0f, 0.0f, 0.0f, //Top Front

            //right orange
               100.0f,  100.0f,  100.0f, 1.0f, 0.5f, 0.1f, //Top Back
               100.0f, -100.0f,  100.0f, 1.0f, 0.5f, 0.1f, //Bottom Back
               100.0f, -100.0f, -100.0f, 1.0f, 0.5f, 0.1f, //Bottom Front
               100.0f,  100.0f, -100.0f, 1.0f, 0.5f, 0.1f  //Top Front
        };

        unsigned int indices[]{
            0, 1, 2,
            0, 3, 2,
            4, 5, 6,
            4, 7, 6,
            8, 9, 10,
            8, 11, 10,
            12, 13, 14,
            12, 15, 14,
            16, 17, 18,
            16, 19, 18,
            20, 21, 22,
            20, 23, 22
        };

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 6 * 4 * 6 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 2 * 6);

        m_Shader = std::make_unique<Shader>("res/shaders/Cube.shader");

        GLCall(glEnable(GL_DEPTH_TEST));
	}

	TestRotatingCube::~TestRotatingCube()
	{
	}

	void TestRotatingCube::OnUpdate(float deltaTime)
	{
	}

	void TestRotatingCube::OnRender()
	{
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCall(glClearColor(0.2f, 0.6f, 0.3f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        auto model = glm::mat4(1.0f);

        model = glm::rotate(model, glm::radians(anglex), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(angley), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 mvp = m_Proj * m_View * glm::mat4(model);
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 lightPos = glm::vec3(300, 200, 200);

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        m_Shader->SetUniform3f("lightColor", lightColor);
        m_Shader->SetUniform3f("lightPos", lightPos);

        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void TestRotatingCube::OnImGuiRender()
	{
        ImGui::SliderFloat("Angle X", &anglex, -500.0f, 500.0f);
        ImGui::SliderFloat("Angle Y", &angley, -500.0f, 500.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}