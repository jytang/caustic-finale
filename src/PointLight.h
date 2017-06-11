#pragma once

#include "Light.h"
#include <glm/gtx/norm.hpp>

class PointLight :public Light {
public:
	PointLight() {}
	float Illuminate(const glm::vec3 &pos, Color &col, glm::vec3 &toLight, glm::vec3 &ltPos) {
		toLight = Position - pos;
		float bright = Intensity / glm::length2(toLight); // Inverse square falloff
		toLight = glm::normalize(toLight);
		col = BaseColor;
		ltPos = Position;
		return bright;
	}

	void SetPosition(glm::vec3 pos) { Position = pos; }
private:
	glm::vec3 Position;
};
