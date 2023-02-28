#include "Calcule.h"

#include <vector>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;

glm::vec3 vector_normale[4] = {glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, -1, 0)};


glm::vec3 Calcule::recalculateDirection(int normala, glm::vec3 vector_initial)
{	
	glm::vec3 vector_nou;
	vector_nou = vector_initial - 2 * glm::dot(vector_normale[normala], vector_initial) * vector_normale[normala];
	return vector_nou;
}

float Calcule::new_angle(int normala, glm::vec3 vector_nou)
{
	float angle;
	angle = acos(glm::dot(vector_nou, vector_normale[normala]) / (glm::length(vector_normale[normala]) * glm::length(vector_nou)));

	if (vector_nou.x > 0) {
		if (vector_nou.y > 0) { //cadran 1
			angle = angle;
		}
		else { //cadran 4
			angle = 6.28 -  angle;
		}
	}
	else {
		if (vector_nou.y > 0) { //cadran 2
			angle = 3.14 - angle;
		}
		else { //cadran 3
			angle = 3.14 + angle;
		}
	}
	return angle;
}
