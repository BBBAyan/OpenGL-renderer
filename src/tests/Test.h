#pragma once

#include "Renderer.h"

#include <iostream>
#include <vector>
#include <string>
#include <functional>

namespace test {
	const float SCR_WIDTH = 1290.0f;
	const float SCR_HEIGHT = 913.0f;
	extern bool isEscapeClicked;
	class Test
	{
	public:
		Test() {};
		virtual ~Test() {};

		virtual void OnUpdate(float deltaTime) {};
		virtual void OnRender() {};
		virtual void OnImGuiRender() {}; 
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name, GLFWwindow* window)
		{
			std::cout << "Registering test: " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, [window]() { return new T(window); }));
		}
	private:
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
		Test*& m_CurrentTest;
	};
}