#pragma once
#include "components/simple_scene.h"

class ObstacleCar {
public:
	ObstacleCar();
	~ObstacleCar();

	void Set(glm::vec3 st, glm::vec3 fin);

	void UpdatePosition();

	float getAng();

	glm::vec3 start;
	glm::vec3 finish;
	float angle;
	glm::vec3 position;
	glm::vec3 direction;
	float x_step;
	float z_step;
};