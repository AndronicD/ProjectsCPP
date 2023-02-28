#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/Models.h"
#include "lab_m1/Tema2/CarMovement.h"
#include "lab_m1/Tema2/PlayerCar.h"
#include "lab_m1/Tema2/ObstacleCar.h"
#include "components/transform.h"

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace m1;

#define angle 0.157f
#define radius 0.15f
#define EPSILON 2.f

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    {
        Shader* shader = new Shader("TemaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "MyVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "MyFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    renderCameraTarget = false;

    camera = new implemented::MyCamera();
    camera->forward = glm::vec3(-0.02324, - 0.474672, 0.879856);
    camera->up = glm::vec3(-0.0125333, 0.880163, 0.474507);
    camera->right = glm::vec3(-0.999651, 0, - 0.0264042);

    playerCar.setCarSpeed(0.0);
    playerCar.setTurnSpeed(0.0);
    playerCar.setPos(glm::vec3(p[0].x * 3, 0, p[0].z * 3));
    playerCar.setRot(0.0);
    playerCar.setVel(glm::vec3(0, 0, 0));



    Mesh* grass = Models::CreateGrass("grass");
    Mesh* road = Models::CreateRoad("road", p, inside, outside, lane1, lane2, rand1, rand2);
    for (int i = 0; i < 16; i++) {
        string name = "car";
        string num = to_string(i);
        name.append(num);
        const string fin_name = name;
        car[i] = Models::CreateCar(fin_name, colors[i]);
        AddMeshToList(car[i]);
    }
    Mesh* tree = Models::CreateTree("tree");

    AddMeshToList(grass);
    AddMeshToList(road);
    AddMeshToList(tree);

    projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
    for (int i = 0; i < 15; i++) {
        if(i % 2)
            carOb[i].Set(glm::vec3(lane1[numb_point[0]].x, 0, lane1[numb_point[i]].z), glm::vec3(lane1[numb_point[i] + 1].x, 0, lane1[numb_point[i] + 1].z));
        else {
            carOb[i].Set(glm::vec3(lane2[numb_point[0]].x, 0, lane2[numb_point[i]].z), glm::vec3(lane2[numb_point[i] + 1].x, 0, lane2[numb_point[i] + 1].z));
        }
    }
}


void Tema2::FrameStart()
{
    glClearColor(0, 214, 214, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
}

bool Tema2::carColision(glm::vec3 playerCarPos, glm::vec3 otherCarPos) {
    glm::vec3 distance = glm::vec3(playerCarPos.x - otherCarPos.x, 0, playerCarPos.z - otherCarPos.z);
    if (glm::length(distance) < 2 * radius) {
        return 1;
    }
    return 0;
}

float ariaTriangle(TPoint A, TPoint B, TPoint C) {
    glm::vec3 AB = glm::vec3(B.z - A.z, 0, B.x - A.x);
    glm::vec3 AC = glm::vec3(C.z - A.z, 0, C.x - A.x);
    float prod = AB.x * AC.z - AB.z * AC.x;
    return abs (prod / 2) ;
}

int orientation(TPoint A, TPoint B, TPoint C) {
    glm::vec3 AB = glm::vec3(abs(B.x) - abs(A.x), 0, abs(B.z) - abs(A.z));
    glm::vec3 AC = glm::vec3(abs(C.x) - abs(A.x), 0, abs(C.z) - abs(A.z));
    float prod = AB.x * AC.z - AB.z * AC.x;
    if (prod > 0.0) {
        return 1;
    }
    else {
        return -1;
    }
}

bool PointInTriangle(TPoint A, TPoint B, TPoint C, TPoint P)
{
    float s1 = C.z - A.z;
    float s2 = C.x - A.x;
    float s3 = B.z - A.z;
    float s4 = P.z - A.z;

    double w1 = (A.x * s1 + s4 * s2 - P.x * s1) / (s3 * s2 - (B.x - A.x) * s1);
    double w2 = (s4 - w1 * s3) / s1;
    return w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1;
}

bool Tema2::insideRoad(TPoint *ins, TPoint *ext, glm::vec3 position) {
    TPoint i1;
    TPoint i2;
    TPoint e1;
    TPoint e2;
    TPoint pos;
    int turns = 0;
    pos.x = position.x;
    pos.z = position.z;
    float arieTriunghi, ariaCalculata = 0.0;
    for (int i = 0; i < 37; i++) {
        if (i == 36) {
            i1 = ins[36];
            i2 = ins[0];
            e1 = ext[36];
            e2 = ext[0];
        }
        else {
            i1 = ins[i];
            i2 = ins[i + 1];
            e1 = ext[i];
            e2 = ext[i + 1];
        }
        if (PointInTriangle(i1, i2, e1, pos) == 1) {
            return 1;
        }
        else if (PointInTriangle(i2, e1, e2, pos) == 1) {
            return 1;
        }
    }
    return 0;
}


void Tema2::RenderScene(int t)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderSimpleMesh(meshes["grass"], shaders["TemaShader"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderSimpleMesh(meshes["road"], shaders["TemaShader"], modelMatrix);
    }

    for (int i = 0; i < 37; i++) {
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(rand1[i].x, 0, rand1[i].z));
            RenderSimpleMesh(meshes["tree"], shaders["TemaShader"], modelMatrix);
        }
    }

    for (int i = 0; i < 37; i++) {
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(rand2[i].x, 0, rand2[i].z));
            RenderSimpleMesh(meshes["tree"], shaders["TemaShader"], modelMatrix);
        }
    }

    {

        glm::mat4 modelMatrix = glm::mat4(1);
        for (int j = 0; j < 15; j++) {
            if (carColision(playerCar.getPos(), carOb[j].position)) {
                collision = 1;
                tick = 0;
                break;
            }
        }

        if (collision) {
            if (tick < 100) {
                playerCar.setVel(glm::vec3(0, 0, 0));
                tick++;
            }
        }
      
        playerCar.setPos(playerCar.getPos() + playerCar.getVel());
        modelMatrix = glm::translate(modelMatrix, playerCar.getPos());
        modelMatrix = glm::rotate(modelMatrix, playerCar.getRot(), glm::vec3(0, 1, 0));
        
        if (t == 1) {
            camera->CalculatePos(camera->distanceFromCar * sin(camera->pitch), camera->distanceFromCar * cos(camera->pitch), playerCar.getRot(), playerCar.getPos());
        }
        RenderSimpleMesh(meshes["car15"], shaders["TemaShader"], modelMatrix);
    }

    for (int i = 0; i < 15; i++) {
        {

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, carOb[i].position);
            float angleCar = carOb[i].getAng();
            modelMatrix = glm::rotate(modelMatrix, angleCar, glm::vec3(0, 1, 0));
            if (i % 2) {
                if (counter[i] >= 100) {
                    counter[i] = 0;
                    if (numb_point[i] == 36) {
                        carOb[i].Set(glm::vec3(lane1[36].x, 0, lane1[36].z), glm::vec3(lane1[0].x, 0, lane1[0].z));
                    }
                    else {
                        carOb[i].Set(glm::vec3(lane1[numb_point[i]].x, 0, lane1[numb_point[i]].z), glm::vec3(lane1[numb_point[i] + 1].x, 0, lane1[numb_point[i] + 1].z));
                    }

                    if (numb_point[i] < 36) { numb_point[i]++; }
                    else { numb_point[i] = 0; }
                }
                else {
                    carOb[i].UpdatePosition();
                }
            }
            else {
                if (counter[i] >= 100) {
                    counter[i] = 0;
                    if (numb_point[i] == 36) {
                        carOb[i].Set(glm::vec3(lane2[0].x, 0, lane2[0].z), glm::vec3(lane2[36].x, 0, lane2[36].z));
                    }
                    else {
                        carOb[i].Set(glm::vec3(lane2[numb_point[i] + 1].x, 0, lane2[numb_point[i] + 1].z), glm::vec3(lane2[numb_point[i]].x, 0, lane2[numb_point[i]].z));
                    }

                    if (numb_point[i] > 0) { numb_point[i]--; }
                    else { numb_point[i] = 36; }
                }
                else {
                    carOb[i].UpdatePosition();
                }
            }
            string name = "car";
            string num = to_string(i);
            name.append(num);
            const string fin_name = name;
            counter[i] ++;
            RenderSimpleMesh(meshes[fin_name], shaders["TemaShader"], modelMatrix);
        }

    }
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);
    
    int v_car_loc = glGetUniformLocation(shader->GetProgramID(), "v_car");
    glUniform3fv(v_car_loc, 1, glm::value_ptr(playerCar.getPos()));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix1 = projectionMatrix;
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix1));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::Update(float deltaTimeSeconds) {
    glm::ivec2 resolution = window->GetResolution();

    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glViewport(0, 0, resolution.x, resolution.y);
    projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
    RenderScene(1);

    glClear(GL_DEPTH_BUFFER_BIT);
    glm::vec3 forw = camera->forward;
    glm::vec3 up = camera->up;
    glm::vec3 righ = camera->right;
    glm::vec3 pos = camera->position;
    
    glViewport(50, 50, resolution.x / 5, resolution.y / 5);
    projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);

    camera->Set(playerCar.getPos() + glm::vec3(0,2,0), playerCar.getPos(), glm::vec3(1,0,0));
    RenderScene(0);
   
    camera->Set(pos, playerCar.getPos(), up);

}


void Tema2::FrameEnd()
{
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float distance;
    float dx;
    float dz;

    if (window->KeyHold(GLFW_KEY_W)) {
        playerCar.setCarSpeed(playerCar.RUN_SPEED);
        distance = playerCar.getCarSpeed() * deltaTime;
        dx = distance * sin(playerCar.getRot());
        dz = distance * cos(playerCar.getRot());
        if (!insideRoad(outside, inside, playerCar.getPos() + glm::vec3(dx, 0, dz))) {
            playerCar.setVel(glm::vec3(0, 0, 0));
        }
        else {
            playerCar.setVel(glm::vec3(dx, 0, dz));
        }
    }
    else if (window->KeyHold(GLFW_KEY_S)) {
        playerCar.setCarSpeed(-playerCar.RUN_SPEED);
        distance = playerCar.getCarSpeed() * deltaTime;
        dx = distance * sin(playerCar.getRot());
        dz = distance * cos(playerCar.getRot());
        if (!insideRoad(outside, inside, playerCar.getPos() + glm::vec3(dx, 0, dz))) {
            playerCar.setVel(glm::vec3(0, 0, 0));
        }
        else {
            playerCar.setVel(glm::vec3(dx, 0, dz));
        }
    }
    else {
        playerCar.setCarSpeed(0.0);
        playerCar.setVel(glm::vec3(0, 0, 0));
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        // TODO(student): Translate the camera to the right
        playerCar.setTurnSpeed(-playerCar.TURN_SPEED);
        playerCar.setRot(playerCar.getRot() + playerCar.getTurnSpeed() * deltaTime);
        camera->RotateThirdPerson_OY(playerCar.getTurnSpeed() * deltaTime);
    }
    else if (window->KeyHold(GLFW_KEY_A)) {
        // TODO(student): Translate the camera to the right
        playerCar.setTurnSpeed(playerCar.TURN_SPEED);
        playerCar.setRot(playerCar.getRot() + playerCar.getTurnSpeed() * deltaTime);
        camera->RotateThirdPerson_OY(playerCar.getTurnSpeed() * deltaTime);
    }
    else {
        playerCar.setTurnSpeed(0.0);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}