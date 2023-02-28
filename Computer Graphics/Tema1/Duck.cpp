
#include "Duck.h"

#include <vector>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;
#define resX 1280
#define resY 720

Mesh* Duck::CreateDuck(
    const std::string& name,
    bool fill)
{
    glm::vec3 brown = glm::vec3(0.64, 0.16, 0.16);
    glm::vec3 yellow = glm::vec3(1, 1, 0);
    glm::vec3 green = glm::vec3(0, 1, 0);

    glm::vec3 body_1 = glm::vec3(0, 0, 0);
    glm::vec3 body_2 = glm::vec3(0, 100, 0);
    glm::vec3 body_3 = glm::vec3(100, 50, 0);

    glm::vec3 beak_1 = glm::vec3(140, 55, 0);
    glm::vec3 beak_2 = glm::vec3(140, 45, 0);
    glm::vec3 beak_3 = glm::vec3(155, 50, 0);

    int head_center_x = 120;
    int head_center_y = 50;

    glm::vec3 center = glm::vec3(head_center_x, head_center_y, 0);
    int triangle_number = 30;
    GLfloat twicePi = 2.0f * 3.14;
    GLfloat radius = 20.0f;
    std::vector<float> x_indices;
    std::vector<float> y_indices;
 
    
    for (int i = 0; i <= triangle_number; i++) {
        x_indices.push_back(head_center_x + (radius * cos(i * twicePi / triangle_number)));
        y_indices.push_back(head_center_y + (radius * sin(i * twicePi / triangle_number)));
    }

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(body_1, brown),
        VertexFormat(body_2, brown),
        VertexFormat(body_3, brown),

        VertexFormat(beak_1, yellow),
        VertexFormat(beak_2, yellow),
        VertexFormat(beak_3, yellow),

    };

    for (int i = 0; i < triangle_number ; i++) {
        vertices.push_back(VertexFormat(center, green));
        vertices.push_back(VertexFormat(glm::vec3(x_indices[i], y_indices[i], 0), green));
        vertices.push_back(VertexFormat(glm::vec3(x_indices[i + 1], y_indices[i + 1], 0), green));
    }



    Mesh* body = new Mesh(name);
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < 99; i++) {
        indices.push_back(i);
    }

    body->InitFromData(vertices, indices);
    return body;
}

Mesh* Duck::CreateWing1(
    const std::string& name,
    bool fill)

{
    glm::vec3 brown = glm::vec3(0.64, 0.16, 0.16);
    glm::vec3 wing1_1 = glm::vec3(80, 60, 0);
    glm::vec3 wing1_2 = glm::vec3(80, 120, 0);
    glm::vec3 wing1_3 = glm::vec3(50, 75, 0);

    std::vector<VertexFormat> vertices =
    {

        VertexFormat(wing1_1, brown),
        VertexFormat(wing1_2, brown),
        VertexFormat(wing1_3, brown)
    };


    Mesh* body = new Mesh(name);
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < 3; i++) {
        indices.push_back(i);
    }

    body->InitFromData(vertices, indices);
    return body;
}

Mesh* Duck::CreateWing2(
    const std::string& name,
    bool fill)

{
    glm::vec3 brown = glm::vec3(0.64, 0.16, 0.16);
    glm::vec3 wing2_1 = glm::vec3(80, 40, 0);
    glm::vec3 wing2_2 = glm::vec3(80, -20, 0);
    glm::vec3 wing2_3 = glm::vec3(50, 25, 0);
    
    std::vector<VertexFormat> vertices =
    {

        VertexFormat(wing2_1, brown),
        VertexFormat(wing2_2, brown),
        VertexFormat(wing2_3, brown),
    };


    Mesh* body = new Mesh(name);
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < 3; i++) {
        indices.push_back(i);
    }

    body->InitFromData(vertices, indices);
    return body;
}

Mesh* Duck::CreateGrass(const std::string& name, bool fill) {
    glm::vec3 green = glm::vec3(0.196078, 0.8, 0.19607);
    glm::vec3 down_left = glm::vec3(0, 0, 0);
    glm::vec3 down_right = glm::vec3(resX, 0, 0);
    glm::vec3 up_left = glm::vec3(0, resY / 6, 0);
    glm::vec3 up_right = glm::vec3(resX, resY / 6, 0);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(down_left, green),
        VertexFormat(down_right, green),
        VertexFormat(up_right, green),
        VertexFormat(up_left, green)
    };

    std::vector<unsigned int> indices;
    indices = { 0, 1, 2,
                0, 2, 3 };

    Mesh* body = new Mesh(name);
    body->InitFromData(vertices, indices);
    return body;
}

Mesh* Duck::CreateHeart(const std::string& name, bool fill) {
    glm::vec3 red = glm::vec3(1, 0, 0);
    std::vector<VertexFormat> vertices =
    {

        VertexFormat(glm::vec3(10, 0, 0), red),
        VertexFormat(glm::vec3(0, 20, 0), red),
        VertexFormat(glm::vec3(20, 20, 0), red)
    };

    glm::vec3 center1 = glm::vec3(5, 20, 0);
    int triangle_number = 30;
    GLfloat twicePi = 3.14;
    GLfloat radius = 5.0f;
    std::vector<float> x_indices1;
    std::vector<float> y_indices1;


    for (int i = 0; i <= triangle_number; i++) {
        x_indices1.push_back(5 + (radius * cos(i * twicePi / triangle_number)));
        y_indices1.push_back(20 + (radius * sin(i * twicePi / triangle_number)));
    }

    glm::vec3 center2 = glm::vec3(15, 20, 0);
    std::vector<float> x_indices2;
    std::vector<float> y_indices2;


    for (int i = 0; i <= triangle_number; i++) {
        x_indices2.push_back(15 + (radius * cos(i * twicePi / triangle_number)));
        y_indices2.push_back(20 + (radius * sin(i * twicePi / triangle_number)));
    }

    for (int i = 0; i < triangle_number; i++) {
        vertices.push_back(VertexFormat(glm::vec3(5, 20, 0), red));
        vertices.push_back(VertexFormat(glm::vec3(x_indices1[i], y_indices1[i], 0), red));
        vertices.push_back(VertexFormat(glm::vec3(x_indices1[i + 1], y_indices1[i + 1], 0), red));
    }

    for (int i = 0; i < triangle_number; i++) {
        vertices.push_back(VertexFormat(glm::vec3(15, 20, 0), red));
        vertices.push_back(VertexFormat(glm::vec3(x_indices2[i], y_indices2[i], 0), red));
        vertices.push_back(VertexFormat(glm::vec3(x_indices2[i + 1], y_indices2[i + 1], 0), red));
    }

    Mesh* body = new Mesh(name);
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < 189; i++) {
        indices.push_back(i);
    }

    body->InitFromData(vertices, indices);
    return body;
}

Mesh* Duck::CreateBullet(const std::string& name, bool fill) {
    glm::vec3 gold = glm::vec3(0.8, 0.498039, 0.196078);
    
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, 0), gold),
        VertexFormat(glm::vec3(20, 0, 0), gold),
        VertexFormat(glm::vec3(0, 10, 0), gold),
        VertexFormat(glm::vec3(20, 10, 0), gold)
    };
    
    std::vector<unsigned int> indices;
    indices = { 0, 1, 3,
                0, 2, 3 };

    glm::vec3 center = glm::vec3(10, 10, 0);
    int triangle_number = 30;
    GLfloat twicePi = 3.14;
    GLfloat radius = 10.0f;
    std::vector<float> x_indices;
    std::vector<float> y_indices;


    for (int i = 0; i <= triangle_number; i++) {
        x_indices.push_back(10 + (radius * cos(i * twicePi / triangle_number)));
        y_indices.push_back(10 + (radius * sin(i * twicePi / triangle_number)));
    }

    for (int i = 0; i < triangle_number; i++) {
        vertices.push_back(VertexFormat(center, gold));
        vertices.push_back(VertexFormat(glm::vec3(x_indices[i], y_indices[i], 0), gold));
        vertices.push_back(VertexFormat(glm::vec3(x_indices[i + 1], y_indices[i + 1], 0), gold));
    }
    
    for (unsigned int i = 4; i < 97; i++) {
        indices.push_back(i);
    }

    Mesh* body = new Mesh(name);
    body->InitFromData(vertices, indices);
    return body;
}

Mesh* Duck::CreateScore(const std::string& name, bool fill) {
    glm::vec3 blue = glm::vec3(0, 0, 1);
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, 0), blue),
        VertexFormat(glm::vec3(100, 0, 0), blue),
        VertexFormat(glm::vec3(100, 20, 0), blue),
        VertexFormat(glm::vec3(0, 20, 0), blue)
    };
    
    std::vector<unsigned int> indices;
    indices = { 0, 1, 1,
                1, 2, 2,
                2, 3, 3,
                3, 0, 0
    };

    Mesh* body = new Mesh(name);
    body->InitFromData(vertices, indices);
    return body;
}

Mesh* Duck::CreateScoreUnit(const std::string& name, bool fill) {
    glm::vec3 blue = glm::vec3(0, 0, 1);
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, 0), blue),
        VertexFormat(glm::vec3(5, 0, 0), blue),
        VertexFormat(glm::vec3(5, 20, 0), blue),
        VertexFormat(glm::vec3(0, 20, 0), blue)
    };

    std::vector<unsigned int> indices;
    indices = { 0, 1, 2,
                0, 2, 3
    };

    Mesh* body = new Mesh(name);
    body->InitFromData(vertices, indices);
    return body;
}