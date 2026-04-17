#include "TestMain.h"

#include <imgui/imgui.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

namespace test
{
    TestMain::TestMain(GLFWwindow* window)
        : m_Proj(glm::perspective(glm::radians(45.0f), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f)),
        m_View(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.3f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f))),
        m_camera{ glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.1f }, m_controls(window, SCR_WIDTH, SCR_HEIGHT, m_camera), draw_cubemap(0),
        m_window(window), currentIndex(2), modelIndex(0), m_animationTime(0.0f), m_lastTime(glfwGetTime())
    {
        float positions[] = {
            //position             //normals           //texCoord
           //front
             -0.5f,  0.5f,  -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, //Left Top
             -0.5f, -0.5f,  -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, //Left Bottom
              0.5f, -0.5f,  -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, //Right Bottom
              0.5f,  0.5f,  -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, //Right Top

              //top
                -0.5f,  0.5f,   0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, //Left Back
                -0.5f,  0.5f,  -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, //Left Front
                 0.5f,  0.5f,  -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, //Right Front
                 0.5f,  0.5f,   0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, //Right Back

                 //back
                   -0.5f,  0.5f,   0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, //Left Top
                   -0.5f, -0.5f,   0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, //Left Bottom
                    0.5f, -0.5f,   0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, //Right Bottom
                    0.5f,  0.5f,   0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, //Right Top

                    //bottom
                      -0.5f, -0.5f,   0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, //Left Back
                      -0.5f, -0.5f,  -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, //Left Front
                       0.5f, -0.5f,  -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, //Right Front
                       0.5f, -0.5f,   0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, //Right Back

                       //left
                         -0.5f,  0.5f,   0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, //Top Back
                         -0.5f, -0.5f,   0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, //Bottom Back
                         -0.5f, -0.5f,  -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //Bottom Front
                         -0.5f,  0.5f,  -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, //Top Front

                         //right
                            0.5f,  0.5f,   0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, //Top Back
                            0.5f, -0.5f,   0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, //Bottom Back
                            0.5f, -0.5f,  -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //Bottom Front
                            0.5f,  0.5f,  -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f //Top Front
        };

        unsigned int indices[]{
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            4, 6, 7,
            11, 10, 9,
            11, 9, 8,
            13, 12, 15,
            13, 15, 14,
            16, 17, 18,
            16, 18, 19,
            23, 22, 21,
            23, 21, 20
        };
        float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        float quadVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        GLCall(glGenVertexArrays(1, &cubemapVAO));
        GLCall(glGenBuffers(1, &cubemapVBO));

        GLCall(glBindVertexArray(cubemapVAO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));

        GLCall(glGenVertexArrays(1, &quadVAO));
        GLCall(glGenBuffers(1, &quadVBO));

        GLCall(glBindVertexArray(quadVAO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, quadVBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
        GLCall(glEnableVertexAttribArray(1));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));

        GLCall(glGenFramebuffers(1, &fbo));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

        GLCall(glGenTextures(1, &textureColorBuffer));
        GLCall(glBindTexture(GL_TEXTURE_2D, textureColorBuffer));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0));

        unsigned int rbo;
        GLCall(glGenRenderbuffers(1, &rbo));

        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT));
        GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "GL_FRAMEBUFFER_INCOMPLETE" << std::endl;

        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        GLCall(glGenBuffers(1, &ubo));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
        GLCall(glBufferData(GL_UNIFORM_BUFFER, 128, NULL, GL_STATIC_DRAW));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

        GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo));

        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        m_VAO = std::make_unique<VertexArray>();
        m_VAO_Point = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, static_cast<unsigned int>(8 * 4 * 6 * sizeof(float)));
        m_VertexBuffer_Point = std::make_unique<VertexBuffer>(&position[0], static_cast<unsigned int>(3 * sizeof(float)));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        VertexBufferLayout layoutPoint;
        layoutPoint.Push<float>(3);
        m_VAO_Point->AddBuffer(*m_VertexBuffer_Point, layoutPoint);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 2 * 6);
        m_Shader = std::make_unique<Shader>("res/shaders/Model.Shader");
        m_Shader_Geometry = std::make_unique<Shader>("res/shaders/Geometry.Shader");
        m_ShaderLight = std::make_unique<Shader>("res/shaders/Lighting.Shader");
        m_ShaderReflectiveCube = std::make_unique<Shader>("res/shaders/ReflectiveCube.Shader");
        m_ShaderFramebuffer = std::make_unique<Shader>("res/shaders/FramebufferScreen.Shader");
        m_ShaderCubemap = std::make_unique<Shader>("res/shaders/Cubemap.Shader");

        m_Texture = std::make_unique<Texture>("res/textures/container.png");
        m_TextureSpecular = std::make_unique<Texture>("res/textures/container_specular.png");
        m_TextureGrass = std::make_unique<Texture>("res/textures/grass.png");
        m_TextureWindow = std::make_unique<Texture>("res/textures/blending_transparent_window.png");

        std::vector<std::string> textures_faces{
        "res/textures/skybox/right.jpg",
        "res/textures/skybox/left.jpg",
        "res/textures/skybox/top.jpg",
        "res/textures/skybox/bottom.jpg",
        "res/textures/skybox/front.jpg",
        "res/textures/skybox/back.jpg"
        };

        m_TextureCubemap = std::make_unique<TextureCubemap>(textures_faces);

        glfwSetWindowUserPointer(window, &m_controls);
        glfwSetKeyCallback(window, Control::handleKeyboard);
        glfwSetCursorPosCallback(window, Control::handleMouse);
        glfwSetScrollCallback(window, Control::handleScroll);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        GLCall(glEnable(GL_STENCIL_TEST));
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        ImGui::SetWindowSize(ImVec2(480.0f, 250.0f));
    }

    TestMain::~TestMain()
    {
        isPauseClicked = 0;
        glfwSetWindowUserPointer(m_window, nullptr);
    }

    void TestMain::OnUpdate(float curTime)
    {
        float deltaTime = curTime - m_lastTime;
        m_lastTime = curTime;

        if (!test::isPauseClicked) {
            m_animationTime += deltaTime;
        }

        lightPos.x = 6 * std::sin(m_animationTime);
        lightPos.z = 6 * std::cos(m_animationTime);
    }

    void TestMain::OnRender()
    {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(GL_LEQUAL));
        GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));

        GLCall(glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        GLCall(glStencilMask(0x00));

        Renderer renderer;
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

        glm::vec3 cameraDir{};
        cameraDir.x = cos(glm::radians(m_controls.getYaw())) * cos(glm::radians(m_controls.getPitch()));
        cameraDir.y = sin(glm::radians(m_controls.getPitch()));
        cameraDir.z = sin(glm::radians(m_controls.getYaw())) * cos(glm::radians(m_controls.getPitch()));
        m_camera.Front = glm::normalize(cameraDir);

        if (getControls().getCameraMode())
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        ProcessInput(m_camera);
        m_Proj = glm::perspective(glm::radians(m_controls.getFov()), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        m_View = glm::lookAt(m_camera.Position, m_camera.Position + m_camera.Front, m_camera.Up);

        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
        GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, &m_Proj));
        GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, &m_View));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

        GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
        GLCall(glStencilMask(0xFF));

        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glFrontFace(GL_CW));

        //Reflective Cube
        m_ShaderReflectiveCube->Bind();
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
        m_ShaderReflectiveCube->SetUniformMat4f("u_Model", model);
        m_ShaderReflectiveCube->SetUniformMat4f("u_View", m_View);
        m_ShaderReflectiveCube->SetUniformMat4f("u_Proj", m_Proj);
        m_ShaderReflectiveCube->SetUniform3f("viewPos", m_camera.Position);
        m_TextureCubemap->Bind(0);
        m_ShaderReflectiveCube->SetUniform1i("skybox", 0);
        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderReflectiveCube);

        // Light Object
        m_ShaderLight->Bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(lightPos.x, 0.0f, lightPos.z));
        model = glm::scale(model, glm::vec3(0.5f));
        m_ShaderLight->SetUniformMat4f("u_Model", model);
        m_ShaderLight->SetUniformMat4f("u_View", m_View);
        m_ShaderLight->SetUniformMat4f("u_Proj", m_Proj);
        m_ShaderLight->SetUniform3f("pointLightColor", glm::vec3(lightColor.r, lightColor.g, lightColor.b));
        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderLight);

        GLCall(glDisable(GL_CULL_FACE));
        GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
        GLCall(glStencilMask(0x00));
        GLCall(glDisable(GL_DEPTH_TEST));
        GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
        GLCall(glStencilMask(0xFF));
        GLCall(glEnable(GL_DEPTH_TEST));

        //skybox
        if (draw_cubemap == true) {
            m_ShaderCubemap->Bind();
            glm::mat4 cubemapView = glm::mat4(glm::mat3(glm::lookAt(m_camera.Position, m_camera.Position + m_camera.Front, m_camera.Up)));
            m_ShaderCubemap->SetUniformMat4f("u_Proj", m_Proj);
            m_ShaderCubemap->SetUniformMat4f("u_View", cubemapView);
            GLCall(glBindVertexArray(cubemapVAO));
            m_TextureCubemap->Bind(0);
            m_ShaderCubemap->SetUniform1i("skybox", 0);
            GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
        }

        GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
        GLCall(glStencilMask(0x00));
        GLCall(glDisable(GL_DEPTH_TEST));

        GLCall(glStencilMask(0xFF));
        GLCall(glStencilFunc(GL_ALWAYS, 0, 0xFF));

        GLCall(glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        m_ShaderFramebuffer->Bind();
        GLCall(glBindVertexArray(quadVAO));
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, textureColorBuffer));

        GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
        GLCall(glBindVertexArray(0));
    }

    void TestMain::OnImGuiRender()
    {
        ImGui::Text("Mouse Pos: (%.1f, %.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
        ImGui::SliderFloat3("Clear Color", &clearColor.r, 0.0f, 1.0f);
        ImGui::SliderFloat3("Light Color", &lightColor.r, 0.0f, 1.0f);
        ImGui::SliderFloat3("Light Position", &lightPos.x, -5.0f, 5.0f);
        ImGui::SliderFloat("DirLight Intensity", &dirlightIntensity, 0.0f, 2.0f);
        ImGui::Checkbox("Draw a Cubemap", &draw_cubemap);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        if (isPauseClicked) {
            ImGui::SetNextWindowSize(ImVec2(250.0f, 150.0f));
            ImGui::SetNextWindowPos(ImVec2(650.0f, 350.0f));
            ImGui::Begin("Pause");
            ImGui::Text("Pause");
            ImGui::End();
        }
    }

    void TestMain::ProcessInput(Camera& f_camera) {
        auto& keys = getControls().getKeyMap();
        if (keys[GLFW_KEY_ESCAPE].isDown)
            isEscapeClicked = 1;
        if (keys[GLFW_KEY_P].isDown && !keys[GLFW_KEY_P].wasDown) {
            isPauseClicked = !isPauseClicked;
            getControls().getCameraMode() = !getControls().getCameraMode();
        }

        if (!ImGui::GetIO().WantCaptureKeyboard) {
            if (keys[GLFW_KEY_W].isDown)
                f_camera.Position += f_camera.Speed * f_camera.Front;
            if (keys[GLFW_KEY_S].isDown)
                f_camera.Position -= f_camera.Speed * f_camera.Front;
            if (keys[GLFW_KEY_A].isDown)
                f_camera.Position -= glm::normalize(glm::cross(f_camera.Front, f_camera.Up)) * f_camera.Speed;
            if (keys[GLFW_KEY_D].isDown)
                f_camera.Position += glm::normalize(glm::cross(f_camera.Front, f_camera.Up)) * f_camera.Speed;
            if (keys[GLFW_KEY_LEFT_SHIFT].isDown)
                f_camera.Position += f_camera.Speed * f_camera.Up;
            if (keys[GLFW_KEY_Z].isDown)
                f_camera.Position -= f_camera.Speed * f_camera.Up;
        }

        for (auto& [k, st] : keys)
            st.wasDown = st.isDown;
    }
}