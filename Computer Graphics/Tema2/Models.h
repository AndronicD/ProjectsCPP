#pragma once

#include <string>
#include <lab_m1/Tema2/Tema2.h>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace Models {
	Mesh* CreateGrass(const std::string& name);
	Mesh* CreateRoad(const std::string& name, TPoint* p, TPoint* inside, TPoint* outside, TPoint* lane1, TPoint* lane2, TPoint* rand1, TPoint* rand2);
	Mesh* CreateTree(const std::string& name);
	Mesh* CreateCar(const std::string& name, const glm::vec3 color);
}