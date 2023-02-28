#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"


namespace m1
{
    class Lab5 : public gfxc::SimpleScene
    {
     public:
        Lab5();
        ~Lab5();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        implemented::Camera *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        float fov = RADIANS(60);
        float aspect = window->props.aspectRatio;
        float zNear = 0.01f;
        float zFar = 200.0f;

        float left = -5.0f;
        float right = 5.0f;
        float bottom = -5.0f;
        float top = 5.0f;

        glm::mat4 modelMatrix;
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;
        float angle = 10 * sensivityOY;
        glm::vec3 velocity = glm::vec3(1, 0, 1);
        float x = velocity.x;
        float z = velocity.z;
        float ang = 0.0;
    };
}   // namespace m1
