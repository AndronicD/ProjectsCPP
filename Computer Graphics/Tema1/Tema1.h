#pragma once

#include "components/simple_scene.h"
#include <ctime>


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        float translateX = 0.0, translateY = 0.0;
        float cx = 77.5;
        float cy = 50.0;

        float centruX = 77.5, centruY = 50;
        std::vector<float> angle_wing_1;
        std::vector<float> angle_wing_2;
        int counter = 1;

        glm::vec3 direction;

        glm::vec3 left_down;
        glm::vec3 right_down;
        glm::vec3 left_up;
        glm::vec3 right_up;

        float width = 155.0;
        float height = 100.0;

        float angle;

        int coord_X, coord_Y;

        int hit_left = 1;
        int hit_right = 0;

        int hit_duck = 0;
        int speed = 1;

        time_t start_time;
        time_t end_time;

        int state;

        int upper_bound_x = window->GetResolution().x / 10;
        int lower_bound_x = 9 * window->GetResolution().x / 10;

        int scor;
        int numar_vieti;
        int numar_gloante;

    };
}   // namespace m1
