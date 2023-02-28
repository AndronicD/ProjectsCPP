#include "lab_m1/Tema2/ObstacleCar.h"
#include <iostream>

ObstacleCar::ObstacleCar() {};
ObstacleCar::~ObstacleCar() {};

float ObstacleCar::getAng() {
	return angle;
}

void ObstacleCar::Set(glm::vec3 st, glm::vec3 fin) {
	start = st;
	finish = fin;
	direction = (glm::vec3(fin.x - st.x, 0, fin.z - st.z));
	position = glm::vec3(st.x, 0, st.z);

	if (direction.x > 0) {
		if (direction.z > 0) { //cadran 1
			angle = acos(glm::dot(direction, glm::vec3(0, 0, 1)) / (glm::length(direction) * glm::length(glm::vec3(0, 0, 1))));
			angle = angle;
		}
		else { //cadran 2
			angle = acos(glm::dot(direction, glm::vec3(0, 0, -1)) / (glm::length(direction) * glm::length(glm::vec3(0, 0, -1))));
			angle = 3.14 - angle;
		}
	}
	else {
		if (direction.z > 0) { //cadran 4
			angle = acos(glm::dot(direction, glm::vec3(0, 0, 1)) / (glm::length(direction) * glm::length(glm::vec3(0, 0, 1))));
			angle = 6.28 - angle;
		}
		else { //cadran 3
			angle = acos(glm::dot(direction, glm::vec3(0, 0, -1)) / (glm::length(direction) * glm::length(glm::vec3(0, 0, -1))));
			angle = 3.14 + angle;
		}
	}
	x_step = sin(angle) * glm::length(direction) / 100;
	z_step = cos(angle) * glm::length(direction) / 100;
}
	
void ObstacleCar::UpdatePosition() {
	position = glm::vec3(position.x + x_step, 0, position.z + z_step);
}