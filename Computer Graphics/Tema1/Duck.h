#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace Duck {

        Mesh* CreateWing1(const std::string& name, bool fill);
        Mesh* CreateWing2(const std::string& name, bool fill);
        Mesh* CreateDuck(const std::string& name, bool fill);
        Mesh* CreateGrass(const std::string& name, bool fill);
        Mesh* CreateHeart(const std::string& name, bool fill);
        Mesh* CreateBullet(const std::string& name, bool fill);
        Mesh* CreateScore(const std::string& name, bool fill);
        Mesh* CreateScoreUnit(const std::string& name, bool fill);
}
