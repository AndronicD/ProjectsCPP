#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/Camera.h"
#include "lab_m1/Tema2/PlayerCar.h"
#include "lab_m1/Tema2/ObstacleCar.h"
#include "components/transform.h"

typedef struct Point {
    float x;
    float z;
}TPoint;

namespace m1
{

    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;
        bool insideRoad(TPoint* ins, TPoint* ext, glm::vec3 position);
        bool carColision(glm::vec3 playerCarPos, glm::vec3 otherCarPos);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderScene(int t);

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;


    protected:
        implemented::MyCamera* camera;
        implemented::MyCamera* camera2;
        glm::mat4 projectionMatrix;

        bool renderCameraTarget;
        // TODO(student): If you need any other class variables, define them here.
        float fov = RADIANS(60);
        float aspect = window->props.aspectRatio;
        float zNear = 0.01f;
        float zFar = 300.0f;

        float left = -5.0f;
        float right = 5.0f;
        float bottom = -5.0f;
        float top = 5.0f;
        TPoint p[37] = { {0, 14}, {-2.22, 12.7}, {-2.98, 10.78}, {-1.36, 8.41}, {-2.38, 6.22},
        {-3.73, 5.92}, {-5.28, 6.18}, {-6.11, 7.48}, {-7.65, 8.58}, {-9.23, 8.45}, {-10.94, 7.47}
        ,{-11.59, 5.51}, {-10.56, 4.26}, {-10.1, 2.44}, {-11.0, 1}, {-11.36, -1.1}, {-9.29, -2.75}, {-7.37, -2.26}
        ,{-5.18, -0.98}, {-4.01, 1.56}, {-1.9, 2.93}, {0.22, 2.99}, {2,2}, {3.23, 0.58}, {3.6, -0.89}
        ,{5.44, -1.82}, {7.28, -1.35}, {8.32, 0.19}, {7.74, 2.35}, {8.93, 3.8}, {9.35, 6.02}, {8.06, 7.57}
        ,{6.32, 7.88}, {4.72, 9.35}, {5.35, 11.36}, {4.32, 13.33}, {2, 14.6} };
        PlayerCar playerCar = PlayerCar();
        TPoint inside[37], outside[37], lane1[37], lane2[37], rand1[37], rand2[37];
        int numb_point[15] = { 1, 3, 6, 9, 10, 11, 14, 15, 2, 17, 24, 13, 29, 31, 34 };
        int counter[15] = { 0 };
        ObstacleCar carOb[15];
        glm::vec3 colors[16] = { glm::vec3(0.8, 0.498039, 0.196078), glm::vec3(0.623529, 0.623529, 0.372549), glm::vec3(0.6, 0.196078, 0.8),
        glm::vec3(0.556863, 0.137255, 0.137255), glm::vec3(0.52, 0.39, 0.39), glm::vec3(0.59, 0.41, 0.31), glm::vec3(1.00, 0.11, 0.68),
        glm::vec3(0.22, 0.69, 0.87), glm::vec3(0.858824, 0.439216, 0.576471), glm::vec3(0.372549, 0.623529, 0.623529), glm::vec3(0.137255, 0.137255, 0.556863),
        glm::vec3(0.847059, 0.74902, 0.847059), glm::vec3(0.847059, 0.847059, 0.74902), glm::vec3(0.65, 0.49, 0.24), glm::vec3(0.42, 0.26, 0.15),
        glm::vec3(0.576471, 0.858824, 0.439216) };
        Mesh* car[16];
        int collision = 0;
        int tick = 0;
    };
};
