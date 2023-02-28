#include "lab_m1/lab5/lab5.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */
#define angle 0.001f

Lab5::Lab5()
{
}


Lab5::~Lab5()
{
}

glm::mat4 RotateOY(float radians)
{
    // TODO(student): Implement the rotation matrix
    return glm::transpose(
        glm::mat4(cos(radians), 0, sin(radians), 0,
            0, 1, 0, 0,
            -sin(radians), 0, cos(radians), 0,
            0, 0, 0, 1)
    );
}

void Lab5::Init()
{
    renderCameraTarget = false;

    camera = new implemented::Camera();
    camera->Set(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    //projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);

}


void Lab5::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab5::Update(float deltaTimeSeconds)
{
    {
        
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, ang, glm::vec3(0, 1, 0));

        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    //{
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
    //    modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
    //    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    //}

    //{
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
    //    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
    //}

    //// TODO(student): Draw more objects with different model matrices.
    //// Attention! The `RenderMesh()` function overrides the usual
    //// `RenderMesh()` that we've been using up until now. This new
    //// function uses the view matrix from the camera that you just
    //// implemented, and the local projection matrix.
    //{
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    modelMatrix = glm::translate(modelMatrix, glm::vec3(-4, 1, 2));
    //    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
    //    modelMatrix = glm::rotate(modelMatrix, RADIANS(10.0f), glm::vec3(0, 1, 1));
    //    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    //}

    //{
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 3, 4));
    //    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5));
    //    modelMatrix = glm::rotate(modelMatrix, RADIANS(20.0f), glm::vec3(1, 1, 0));
    //    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    //}

    //{
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
    //    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
    //}

    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }
}


void Lab5::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Lab5::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab5::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;
        

        if (window->KeyHold(GLFW_KEY_W)) {
        //    // TODO(student): Translate the camera forward
        //    camera->TranslateForward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
        //    // TODO(student): Translate the camera to the left
        //    //camera->TranslateRight(-cameraSpeed * deltaTime);
           camera->RotateThirdPerson_OY(ang);
        //    //ang = camera->forward.z / glm::length(camera->forward);
            ang -= angle;
        }

        //if (window->KeyHold(GLFW_KEY_S)) {
        //    // TODO(student): Translate the camera backward
        //    camera->TranslateForward(-cameraSpeed * deltaTime);
        //}

        if (window->KeyHold(GLFW_KEY_D)) {
        //    // TODO(student): Translate the camera to the right
       
        //    //camera->RotateThirdPerson_OX(10 * sensivityOX);
            camera->RotateThirdPerson_OY(ang);
        //    //ang = camera->forward.x / glm::length(camera->forward);
            ang += angle;
        }
            //modelMatrix *= glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));

        //if (window->KeyHold(GLFW_KEY_Q)) {
        //    // TODO(student): Translate the camera downward
        //    camera->TranslateUpward(-cameraSpeed * deltaTime);
        //}

        //if (window->KeyHold(GLFW_KEY_E)) {
        //    // TODO(student): Translate the camera upward
        //    camera->TranslateUpward(cameraSpeed * deltaTime);
        //}

        //if (window->KeyHold(GLFW_KEY_U)) {
        //    fov += 0.05;
        //    projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
        //}

        //if (window->KeyHold(GLFW_KEY_I)) {
        //    fov -= 0.05;
        //    projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
        //}

        //if (window->KeyHold(GLFW_KEY_J)) {
        //    left -= 0.05f;
        //    right += 0.05f;
        //    bottom -= 0.05f;
        //    top += 0.05f;
        //    projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        //}

        //if (window->KeyHold(GLFW_KEY_K)) {
        //    left += 0.05f;
        //    right -= 0.05f;
        //    bottom += 0.05f;
        //    top -= 0.05f;
        //    projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        //}
    }

    // TODO(student): Change projection parameters. Declare any extra
    // variables you might need in the class header. Inspect this file
    // for any hardcoded projection arguments (can you find any?) and
    // replace them with those extra variables.

}


void Lab5::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    // TODO(student): Switch projections
    if (key == GLFW_KEY_P) {
        //glm::mat4 Projection = glm::perspective(float fov, float aspect, float zNear, float zFar);
        projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
    }
    if (key == GLFW_KEY_O) {
        //glm::ortho(float left, float right, float bottom, float top, float zNear, float zFar);
        projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    }
}


void Lab5::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    //if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    //{
    //    float sensivityOX = 0.001f;
    //    float sensivityOY = 0.001f;

    //    if (window->GetSpecialKeyState() == 0) {
    //        renderCameraTarget = false;
    //        // TODO(student): Rotate the camera in first-person mode around
    //        // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
    //        // variables for setting up the rotation speed.
    //        camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
    //        camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
    //    }

    //    if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
    //        renderCameraTarget = true;
    //        // todo(student): rotate the camera in third-person mode around
    //        // ox and oy using `deltax` and `deltay`. use the sensitivity
    //        // variables for setting up the rotation speed.
    //        camera->RotateThirdPerson_OX(deltaY * sensivityOX);
    //        camera->RotateThirdPerson_OY(deltaX * sensivityOY);

    //    }
    //}
}


void Lab5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab5::OnWindowResize(int width, int height)
{
}
