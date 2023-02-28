#include "lab_m1/Tema1/Tema1.h"
#include "lab_m1/Tema1/Duck.h"
#include "lab_m1/Tema1/Transformari.h"
#include "lab_m1/Tema1/Calcule.h"
#define GLT_IMPLEMENTATION
#include "lab_m1/Tema1/glText.h"
#define Up 1.57
#define Down 4.71
#define VietiX 50
#define VietiY 650
#define distance 30
#define GloanteX 50
#define GloanteY 600
#define ScorX 50
#define ScorY 550
#define getting_out 20


#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

Tema1::Tema1()
{
    srand(time(0));

    direction.x = rand() % 10;
    direction.y = rand() % 10;

    if (direction.x < 0) { direction.x += 2 * (- direction.x); }
    if (direction.y < 0) { direction.y += 2 * (- direction.y); }
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    
    Mesh* duck = Duck::CreateDuck("ducky", false);
    Mesh* wing1 = Duck::CreateWing1("wingy1", false);
    Mesh* wing2 = Duck::CreateWing2("wingy2", false);
    Mesh* grass = Duck::CreateGrass("grass", false);
    Mesh* heart = Duck::CreateHeart("heart", false);
    Mesh* bullet = Duck::CreateBullet("bullet", false);
    Mesh* score = Duck::CreateScore("score", false);
    Mesh* scoreUnit = Duck::CreateScoreUnit("scoreUnit", false);

    
    AddMeshToList(duck);
    AddMeshToList(wing1);
    AddMeshToList(wing2);
    AddMeshToList(grass);
    AddMeshToList(heart);
    AddMeshToList(bullet);
    AddMeshToList(score);
    AddMeshToList(scoreUnit);


    float item1;
    float item2;
    int i;
    
    for (i = 0, item1 = 0.2, item2 = -0.2; i < 10; i++, item1 += 0.03, item2 -= 0.03) {
        angle_wing_1.push_back(item1);
        angle_wing_2.push_back(item2);
    }

    counter = 1;
    state = 1;
    scor = 0;
    numar_gloante = 3;
    numar_vieti = 3;
    time(&start_time);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    //Init
    glm::mat3 modelMatrixBody = glm::mat3(1);
    glm::mat3 modelMatrixWing1 = glm::mat3(1);
    glm::mat3 modelMatrixWing2 = glm::mat3(1);
    glm::mat3 modelMatrixVieti = glm::mat3(1);
    glm::mat3 modelMatrixGloante = glm::mat3(1);
    glm::mat3 modelMatrixScor = glm::mat3(1);
    glm::mat3 modelMatrixScorUnit = glm::mat3(1);
    
    glm::mat3 modelMatrixGrass = glm::mat3(1);
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelMatrixGrass);
    
    if (numar_vieti == 0 || scor == 20) {
        gltInit();

        // Creating text
        GLTtext* text = gltCreateText();
        char* mesaj_final = (char*)calloc(20, sizeof(char));
        strcpy(mesaj_final, "Game Over! Score:");
        string score = to_string(scor);
        strcpy(mesaj_final + 17, score.c_str()); 

        gltSetText(text, mesaj_final);
        free(mesaj_final);

        // Begin text drawing (this for instance calls glUseProgram)
        gltBeginDraw();

        // Draw any amount of text between begin and end
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2D(text, 250, 300, 5);

        // Finish drawing text
        gltEndDraw();

        // Deleting text
        gltDeleteText(text);

        // Destroy glText
        gltTerminate();
        goto end;
    }
 
    if (state == 1) { //Initialize parameters
        numar_gloante = 3;

        direction.x += speed;
        direction.y += speed;
        
        angle = atan(direction.y / direction.x);
        if (direction.x > 0) {
             angle = angle;
        }
        else {
             angle = 3.14 + angle;
        }

        translateX = ((rand() % 100) * (rand() % 100)) / ((rand() % 10 + 1) * (rand() % 10 + 1));
        if (translateX > 1000) {
            translateX /= 10;
        }
        
        centruX = translateX + width / 2.0f;

        state = 2;
    }

    else if (state == 2) { // Duck movement

        if (counter % 10 == 0) {
            std::reverse(angle_wing_1.begin(), angle_wing_1.end());
            std::reverse(angle_wing_2.begin(), angle_wing_2.end());
        }


        if (translateX > window->GetResolution().x - width / 2.0f) {
            direction = Calcule::recalculateDirection(1, direction);
            direction.z = 0;
            angle = Calcule::new_angle(1, direction);
        }

        if (translateX < 0) {
            direction = Calcule::recalculateDirection(0, direction);
            direction.z = 0;
            angle = Calcule::new_angle(0, direction);
        }

        if (translateY > window->GetResolution().y - height / 2.0f) {
            direction = Calcule::recalculateDirection(3, direction);
            direction.z = 0;
            angle = Calcule::new_angle(3, direction);
        }

        if (translateY < 0) {
            direction = Calcule::recalculateDirection(2, direction);
            direction.z = 0;
            angle = Calcule::new_angle(2, direction);
        }
        

        translateX += direction.x;
        translateY += direction.y;

        centruX += direction.x;
        centruY += direction.y;

        float xsj = -width / 2.0f;
        float ysj = -height / 2.0f;
        left_down.x = xsj * cos(angle) - ysj * sin(angle);
        left_down.y = xsj * sin(angle) + ysj * cos(angle);
        left_down.x += centruX;
        left_down.y += centruY;

        float xdj = width / 2.0f;
        float ydj = -height / 2.0f;
        right_down.x = xdj * cos(angle) - ydj * sin(angle);
        right_down.y = xdj * sin(angle) + ydj * cos(angle);
        right_down.x += centruX;
        right_down.y += centruY;

        float xss = -width / 2.0f;
        float yss = height / 2.0f;
        left_up.x = xss * cos(angle) - yss * sin(angle);
        left_up.y = xss * sin(angle) + yss * cos(angle);
        left_up.x += centruX;
        left_up.y += centruY;

        float xds = width / 2.0f;
        float yds = height / 2.0f;
        right_up.x = xds * cos(angle) - yds * sin(angle);
        right_up.y = xds * sin(angle) + yds * cos(angle);
        right_up.x += centruX;
        right_up.y += centruY;

        modelMatrixBody *= Transformari::Translate(translateX, translateY);
        modelMatrixBody *= Transformari::Translate(+cx, +cy);
        modelMatrixBody *= Transformari::Rotate(angle);
        modelMatrixBody *= Transformari::Translate(-cx, -cy);

        modelMatrixWing1 *= Transformari::Translate(translateX, translateY);
        modelMatrixWing1 *= Transformari::Translate(+cx, +cy);
        modelMatrixWing1 *= Transformari::Rotate(angle);
        modelMatrixWing1 *= Transformari::Translate(-cx, -cy);
        modelMatrixWing1 *= Transformari::Translate(+80, +60);
        modelMatrixWing1 *= Transformari::Rotate(angle_wing_1.at(counter % 10));
        modelMatrixWing1 *= Transformari::Translate(-80, -60);

        modelMatrixWing2 *= Transformari::Translate(translateX, translateY);
        modelMatrixWing2 *= Transformari::Translate(+cx, +cy);
        modelMatrixWing2 *= Transformari::Rotate(angle);
        modelMatrixWing2 *= Transformari::Translate(-cx, -cy);
        modelMatrixWing2 *= Transformari::Translate(+80, +60);
        modelMatrixWing2 *= Transformari::Rotate(angle_wing_2.at(counter % 10));
        modelMatrixWing2 *= Transformari::Translate(-80, -60);
        time(&end_time);

        if (numar_gloante == 0 || ((end_time - start_time) > 5)) {
            numar_vieti--;
            speed = 0;
            state = 3;
        }

        if (hit_duck == 1) {
            speed = 1;
            state = 4;
        }

    }

    else if (state == 3) { //Escape
        translateY += getting_out;

        modelMatrixBody *= Transformari::Translate(translateX, translateY);
        modelMatrixBody *= Transformari::Translate(+cx, +cy);
        modelMatrixBody *= Transformari::Rotate(Up);
        modelMatrixBody *= Transformari::Translate(-cx, -cy);

        modelMatrixWing1 *= Transformari::Translate(translateX, translateY);
        modelMatrixWing1 *= Transformari::Translate(+cx, +cy);
        modelMatrixWing1 *= Transformari::Rotate(Up);
        modelMatrixWing1 *= Transformari::Translate(-cx, -cy);
        modelMatrixWing1 *= Transformari::Translate(+80, +60);
        modelMatrixWing1 *= Transformari::Rotate(angle_wing_1.at(counter % 10));
        modelMatrixWing1 *= Transformari::Translate(-80, -60);

        modelMatrixWing2 *= Transformari::Translate(translateX, translateY);
        modelMatrixWing2 *= Transformari::Translate(+cx, +cy);
        modelMatrixWing2 *= Transformari::Rotate(Up);
        modelMatrixWing2 *= Transformari::Translate(-cx, -cy);
        modelMatrixWing2 *= Transformari::Translate(+80, +60);
        modelMatrixWing2 *= Transformari::Rotate(angle_wing_2.at(counter % 10));
        modelMatrixWing2 *= Transformari::Translate(-80, -60);

        if (translateY > window->GetResolution().y - width / 2.0f) {
            state = 5;
        }
    }

    else if (state == 4) { //Kill
        translateY -= getting_out;

        modelMatrixBody *= Transformari::Translate(translateX, translateY);
        modelMatrixBody *= Transformari::Translate(+cx, +cy);
        modelMatrixBody *= Transformari::Rotate(Down);
        modelMatrixBody *= Transformari::Translate(-cx, -cy);

        modelMatrixWing1 *= Transformari::Translate(translateX, translateY);
        modelMatrixWing1 *= Transformari::Translate(+cx, +cy);
        modelMatrixWing1 *= Transformari::Rotate(Down);
        modelMatrixWing1 *= Transformari::Translate(-cx, -cy);
        modelMatrixWing1 *= Transformari::Translate(+80, +60);
        modelMatrixWing1 *= Transformari::Rotate(angle_wing_1.at(counter % 10));
        modelMatrixWing1 *= Transformari::Translate(-80, -60);

        modelMatrixWing2 *= Transformari::Translate(translateX, translateY);
        modelMatrixWing2 *= Transformari::Translate(+cx, +cy);
        modelMatrixWing2 *= Transformari::Rotate(Down);
        modelMatrixWing2 *= Transformari::Translate(-cx, -cy);
        modelMatrixWing2 *= Transformari::Translate(+80, +60);
        modelMatrixWing2 *= Transformari::Rotate(angle_wing_2.at(counter % 10));
        modelMatrixWing2 *= Transformari::Translate(-80, -60);

        if (translateY < width / 2.0f) {
            state = 5;
        }
    }

    else if (state == 5) { //Reload Game
        time(&start_time);
        hit_duck = 0;
        translateX = 0;
        translateY = 0;
        centruX = 0;
        centruY = 0;
        state = 1;
    }

    for (int i = 0; i < numar_gloante; i++) {
        modelMatrixGloante *= Transformari::Translate(GloanteX + i * distance, GloanteY);
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrixGloante);
        modelMatrixGloante *= Transformari::Translate(-GloanteX - i * distance, -GloanteY);
    }
    
    for (int i = 0; i < numar_vieti; i++) {
        modelMatrixVieti *= Transformari::Translate(VietiX + i * distance, VietiY);
        RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrixVieti);
        modelMatrixVieti *= Transformari::Translate(-VietiX - i * distance, -VietiY);
    }
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    modelMatrixScor *= Transformari::Translate(ScorX, ScorY);
    RenderMesh2D(meshes["score"], shaders["VertexColor"], modelMatrixScor);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    modelMatrixScorUnit *= Transformari::Translate(ScorX, ScorY);
    modelMatrixScorUnit *= Transformari::Scale(scor, 1);
    RenderMesh2D(meshes["scoreUnit"], shaders["VertexColor"], modelMatrixScorUnit);
    
    RenderMesh2D(meshes["ducky"], shaders["VertexColor"], modelMatrixBody);
    RenderMesh2D(meshes["wingy1"], shaders["VertexColor"], modelMatrixWing1);
    RenderMesh2D(meshes["wingy2"], shaders["VertexColor"], modelMatrixWing2);
    end:

    counter++;
}


void Tema1::FrameEnd()
{
}



void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    //std::cout << mouseX << " " << mouseY << endl;
    coord_X = mouseX;
    coord_Y = window->GetResolution().y - mouseY;
   
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        int Xs[4] = {std::ceil(left_up.x), std::ceil(left_down.x), std::ceil(right_down.x), std::ceil(right_up.x)};
        int Ys[4] = {std::ceil(left_up.y), std::ceil(left_down.y), std::ceil(right_down.y), std::ceil(right_up.y)};

        int* minX = min_element(Xs, Xs + 4);
        int* minY = min_element(Ys, Ys + 4);

        int* maxX = max_element(Xs, Xs + 4);
        int* maxY = max_element(Ys, Ys + 4);


        if (coord_X > *minX && coord_X < *maxX && coord_Y > *minY && coord_Y < *maxY) {
            hit_duck = 1;
            numar_gloante--;
            scor++;
        }
        else {
            numar_gloante--;
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
