#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Character {
	unsigned int	TextureID;
	glm::ivec2		Size;
	glm::ivec2		Bearing;
	unsigned int	Advance;
	int				height;
};
