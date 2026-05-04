//#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

//#include "Renderer.h"

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stb_image/stb_image.h>

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestRotatingCube.h"
#include "tests/TestModel.h"
#include <tests/TestMain.h>

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit()) {
        std::cout << "glfwInit() fail to initialize. \n";
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(1490, 913, "OpenGL Renderer", NULL, NULL);

    if (!window)
    {
        std::cout << "Display window fail to create. \n";
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR GLEW_NOT_OK" << std::endl;
        return -1;
    }

    int width, height, channels;
    unsigned char* pixels = stbi_load("res/icons/dimensions.png", &width, &height, &channels, 4);

    GLFWimage images[1];
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = pixels;

    glfwSetWindowIcon(window, 1, images);

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        const char* glsl_version = "#version 130";
        ImGui_ImplOpenGL3_Init(glsl_version);

        //test::Test test;
        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);

        currentTest = testMenu;
        
        testMenu->RegisterTest<test::TestClearColor>("Clear Color", window);
        testMenu->RegisterTest<test::TestTexture2D>("Texture 2D", window);
        testMenu->RegisterTest<test::TestRotatingCube>("Rotating Cube", window);
        testMenu->RegisterTest<test::TestModel>("Importing Model", window);
        testMenu->RegisterTest<test::TestMain>("Main Window", window);

        while (!glfwWindowShouldClose(window))
        {
            // Render here 
            renderer.SetClearColor();
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::SetNextWindowSize(ImVec2(400.0f, 462.0f));
            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));

            if (currentTest)
            {
                currentTest->OnUpdate(glfwGetTime());
                currentTest->OnRender();

                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-")) {
                    delete currentTest;
                    currentTest = testMenu;
                    renderer.SetClearColor();
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            GLCall(glfwSwapBuffers(window));
            GLCall(glfwPollEvents());

            if (test::isEscapeClicked)
                break;
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
};