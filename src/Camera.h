#pragma once

#include "glm/glm.hpp"

namespace test {
	struct Camera {
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		float Speed;
	};
}