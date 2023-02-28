#pragma once

#include <iostream>
#include "utils/glm_utils.h"

namespace Calcule {
	glm::vec3 recalculateDirection(int normala, glm::vec3 vector_initial);
	float new_angle(int normala, glm::vec3 vector_nou);
}
