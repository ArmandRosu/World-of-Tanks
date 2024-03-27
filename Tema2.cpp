#include "lab_m1/tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/tema2/transform3D_2.h"

using namespace std;
using namespace m1;

struct proiectil {
    float pos_x;
    float pos_z;
    float alpha;
    int is_it;
    float delta_time;
};

struct tanc {
    proiectil proiectil_array[1000];
    float pos_x;
    float pos_z;
    int last;
    float last_time;
    float tanc_angle;
    float turela_angle;
    int random_movement;
    float time_left;
    int alive;
    float random_bullet;
    float distance;
};

tanc tancuri[10];
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
        lightPosition = glm::vec3(0, 1, 1);
        lightDirection = glm::vec3(0, -1, 0);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;

        isSpotligth = 0;
        cut_off_angle = 60.0f;
    }

    polygonMode = GL_FILL;

    tancuri[0].tanc_angle = 0;
    tancuri[0].turela_angle = 0;

    tancuri[0].pos_x = 5;
    tancuri[0].pos_z = 0;


    {
        Mesh* mesh3 = new Mesh("sphere");
        mesh3->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh3->GetMeshID()] = mesh3;
    }


    {
        Mesh* mesh3 = new Mesh("plane50");
        mesh3->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh3->GetMeshID()] = mesh3;
    }
    
    for (int i = 0; i < 15; i++) {
        random_x[i] = rand() % 50 - 25;
        random_z[i] = rand() % 50 - 25;

        if (random_x[i] > 1 && random_x[i] <= 5 && random_z[i] > -3 && random_z[i] < 3) {
            random_x[i] -= 3;
        }
        if (random_x[i] > 5 && random_x[i] < 8 && random_z[i] > -3 && random_z[i] < 3) {
            random_x[i] += 3;
        }
        
    }
    

    tancuri[0].last = 0;
    tancuri[0].alive = 3;

    for (int i = 1; i < 10; i++) {
        tancuri[i].alive = 3;
        tancuri[i].random_bullet = 10 + (rand() % 3) * 10 + 2 * i;
        tancuri[i].random_movement = rand() % 5;
        tancuri[i].time_left = 2 + rand() % 3;
        tancuri[i].pos_x = rand() % 40 - 20;
        tancuri[i].pos_z = rand() % 40 - 20;
        for (int j = 0; j < 15; j++) {
            if (tancuri[i].pos_x > random_x[j] - 2.94f && tancuri[i].pos_x <= random_x[j] &&
                tancuri[i].pos_z > random_z[j] - 2.94f && tancuri[i].pos_z < random_z[j] + 2.94f) {
                tancuri[i].pos_x -= 2.94f;
            }
            else {
                if (tancuri[i].pos_x > random_x[j] && tancuri[i].pos_x <= random_x[j] + 2.94f &&
                    tancuri[i].pos_z > random_z[j] - 2.94f && tancuri[i].pos_z < random_z[j] + 2.94f) {
                    tancuri[i].pos_x += 2.94f;
                }
            }
        }
        tancuri[i].last = 0;
        tancuri[i].last_time = -1;
        tancuri[i].tanc_angle = rand() % 628 / 100;
        tancuri[i].turela_angle = rand() % 628 / 100;

        for (int j = 0; j < 1000; j++) {
            tancuri[i].proiectil_array[j].is_it = 0;
            tancuri[i].last_time = -1;
            tancuri[i].proiectil_array[j].delta_time = 20;
        }
    }

    for (int i = 0; i < 1000; i++) {
        tancuri[0].proiectil_array[i].is_it = 0;
        tancuri[0].last_time = -1;
        tancuri[0].proiectil_array[i].delta_time = 20;
    }



    {
        vector<VertexFormat> vertices;
        vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.47f, 0.47f, 0.47f)));

        for (int i = 1; i < 100; i++) {
            vertices.push_back(VertexFormat(glm::vec3(cos(2 * 3.1415 / 99 * i), sin(2 * 3.1415 / 99 * i), 0), glm::vec3(0.47f, 0.47f, 0.47f)));
            // TODO(student): Complete the vertices data for the cube mesh

        }

        vertices.push_back(VertexFormat(glm::vec3(0, 0, 10), glm::vec3(0.47f, 0.47f, 0.47f)));

        for (int i = 1; i < 100; i++) {
            vertices.push_back(VertexFormat(glm::vec3(cos(2 * 3.1415 / 99 * i), sin(2 * 3.1415 / 99 * i), 10), glm::vec3(0.47f, 0.47f, 0.47f)));
            // TODO(student): Complete the vertices data for the cube mesh

        }

        vector<unsigned int> indices;
        for (int i = 1; i < 99; i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(100);
            indices.push_back(i + 100);
            indices.push_back(i + 101);



            indices.push_back(i);
            indices.push_back(100 + i);
            indices.push_back(i + 1);
            indices.push_back(i + 1);
            indices.push_back(100 + i);
            indices.push_back(101 + i);
            
        }
        

        indices.push_back(0);
        indices.push_back(99);
        indices.push_back(1);
        indices.push_back(100);
        indices.push_back(199);
        indices.push_back(101);



        indices.push_back(99);
        indices.push_back(199);
        indices.push_back(1);
        indices.push_back(1);
        indices.push_back(199);
        indices.push_back(101);



        meshes["cylinder_A"] = new Mesh("generated cylinder 1");
        meshes["cylinder_A"]->SetDrawMode(GL_TRIANGLES);
        meshes["cylinder_A"]->InitFromData(vertices, indices);

        // Actually create the mesh from the data
        CreateMesh("cylinder_B", vertices, indices);
    }


    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab7", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab7", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    /*
    {
        lightPosition = glm::vec3(0, 1, 1);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
    }
    */
    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

}

void Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // TODO(student): Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    // TODO(student): Create the VBO and bind it
    GLuint VBO_ID;
    glGenBuffers(1, &VBO_ID);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);

    // TODO(student): Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    // TODO(student): Create the IBO and bind it
    GLuint IBO_ID;
    glGenBuffers(1, &IBO_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_ID);

    // TODO(student): Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data. It will be learned later, when GLSL shaders will be
    // introduced. For the moment, just think that each property value from
    // our vertex format needs to be sent to a certain channel, in order to
    // know how to receive it in the GLSL vertex shader.

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // TODO(student): Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderScene() {
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D_2::Translate(0.15f * sin( - tancuri[0].turela_angle / 100) + tancuri[0].pos_x, 0.275f, 0.15f * cos( - tancuri[0].turela_angle / 100) + tancuri[0].pos_z);//tancuri[0].tanc_angle - tancuri[0].turela_angle / 100

    modelMatrix *= transform3D_2::Scale(0.04, 0.04, 0.04);
    modelMatrix *= transform3D_2::RotateOY( - tancuri[0].turela_angle / 100);//tancuri[0].tanc_angle - tancuri[0].turela_angle / 100

    RenderMesh(meshes["cylinder_A"], shaders["VertexColor"], modelMatrix);


    for (int i = 1; i < 10; i++) {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_2::Translate(tancuri[i].pos_x + 0.15f * sin(tancuri[i].turela_angle), 0.275f, tancuri[i].pos_z + 0.15f * cos(tancuri[i].turela_angle));
        modelMatrix *= transform3D_2::Scale(0.04, 0.04, 0.04);
        modelMatrix *= transform3D_2::RotateOY(tancuri[i].turela_angle);
        RenderMesh(meshes["cylinder_A"], shaders["VertexColor"], modelMatrix);
    }


}

void Tema2::Update(float deltaTimeSeconds)
{
    RenderScene();
    
    time_left -= deltaTimeSeconds;


    if (time_left < 0 && is_over == 0) {
        //window->Close();
        for (int i = 0; i < 10; i++) {
            if (tancuri[i].alive == 0) {
                score += 100;
            }
            tancuri[i].alive = 0;
        }
        printf("Game Over\n");
        printf("Score is: %d\n", score);
        is_over = 1;
    }

    if (tancuri[0].last_time > -0.1f) {
        tancuri[0].last_time -= deltaTimeSeconds;
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(50, 0.01f, 50));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0, 1, 0));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(tancuri[0].pos_x, 0.15f, tancuri[0].pos_z));
        modelMatrix = glm::rotate(modelMatrix, tancuri[0].tanc_angle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.15f, 0.8f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0.14 + 0.02 * tancuri[0].alive, 0));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(tancuri[0].pos_x, 0.275f, tancuri[0].pos_z));
        modelMatrix = glm::rotate(modelMatrix, - tancuri[0].turela_angle / 100, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.1f, 0.4f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0.25 + 0.05 * tancuri[0].alive, 0));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(tancuri[0].pos_x - 0.125f * cos(tancuri[0].tanc_angle), 0.05f, tancuri[0].pos_z + 0.125f * sin(tancuri[0].tanc_angle)));//angle
        modelMatrix = glm::rotate(modelMatrix, tancuri[0].tanc_angle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.05f, 0.7f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.8, 0.8, 0.8));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(tancuri[0].pos_x + 0.125f * cos(tancuri[0].tanc_angle), 0.05f, tancuri[0].pos_z - 0.125f * sin(tancuri[0].tanc_angle)));//angle
        modelMatrix = glm::rotate(modelMatrix, tancuri[0].tanc_angle, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.05f, 0.7f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.8, 0.8, 0.8));
    }

    int check;
    float new_x, new_z;
    for (int i = 1; i < 10; i++) {

        tancuri[i].distance = sqrt((tancuri[i].pos_x - tancuri[0].pos_x) * (tancuri[i].pos_x - tancuri[0].pos_x) +
            (tancuri[i].pos_z - tancuri[0].pos_z) * (tancuri[i].pos_z - tancuri[0].pos_z));



        tancuri[i].random_bullet -= deltaTimeSeconds;
        if (tancuri[i].random_bullet < 0 && tancuri[i].distance < 20 && tancuri[i].alive > 0) {


            tancuri[i].proiectil_array[tancuri[i].last].is_it = 1;
            tancuri[i].proiectil_array[tancuri[i].last].pos_x = tancuri[i].pos_x + 0.5f * sin(tancuri[i].turela_angle);
            tancuri[i].proiectil_array[tancuri[i].last].pos_z = tancuri[i].pos_z + 0.5f * cos(tancuri[i].turela_angle);
            tancuri[i].proiectil_array[tancuri[i].last].alpha = tancuri[i].turela_angle;
            tancuri[i].last++;
            tancuri[i].random_bullet = 10 + (rand() % 3) * 10 + 5 * i;
        }

        for (int j = 0; j < 1000; j++) {
            if (tancuri[i].proiectil_array[j].is_it == 1) {
                tancuri[i].proiectil_array[i].delta_time -= deltaTimeSeconds;

                check = 1;
                new_x = tancuri[i].proiectil_array[j].pos_x + deltaTimeSeconds * sin(tancuri[i].proiectil_array[j].alpha);
                new_z = tancuri[i].proiectil_array[j].pos_z + deltaTimeSeconds * cos(tancuri[i].proiectil_array[j].alpha);
                for (int k = 0; k < 15; k++) {
                    if (new_x > random_x[k] - 2.5f && new_x < random_x[k] + 2.5f &&
                        new_z > random_z[k] - 2.5f && new_z < random_z[k] + 2.5f) {
                        check = 0;
                        break;
                    }
                }
                if (check == 1) {
                    for (int k = 0; k < 10 && k != i; k++) {
                        if (new_x > tancuri[k].pos_x - 0.3f && new_x < tancuri[k].pos_x + 0.3f &&
                            new_z > tancuri[k].pos_z - 0.3f && new_z < tancuri[k].pos_z + 0.3f) {
                            if (k == 0 && tancuri[0].alive > 0) {
                                tancuri[0].alive--;
                            }
                            check = 0;
                            break;
                        }
                    }
                }
                if (check == 1 && tancuri[i].proiectil_array[i].delta_time > 0) {
                    tancuri[i].proiectil_array[j].pos_x = new_x;
                    tancuri[i].proiectil_array[j].pos_z = new_z;
                }
                else {
                    tancuri[i].proiectil_array[j].is_it = 0;
                }


                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(tancuri[i].proiectil_array[j].pos_x, 0.27f, tancuri[i].proiectil_array[j].pos_z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.07f, 0.07f, 0.07f));

                RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 0));
            }
        }


        if (tancuri[0].pos_x > tancuri[i].pos_x && tancuri[i].alive > 0) {
            if (tancuri[0].pos_z > tancuri[i].pos_z) {
                tancuri[i].turela_angle = atan((tancuri[i].pos_z - tancuri[0].pos_z) / (tancuri[0].pos_x - tancuri[i].pos_x)) - 3 * 3.1415f / 2;
            }
            else {
                tancuri[i].turela_angle = atan((tancuri[i].pos_z - tancuri[0].pos_z) / (tancuri[0].pos_x - tancuri[i].pos_x)) - 3 * 3.1415f / 2;
            }
        }
        if (tancuri[0].pos_x == tancuri[i].pos_x && tancuri[i].alive > 0) {
            if (tancuri[0].pos_z > tancuri[i].pos_z) {
                tancuri[i].turela_angle = 3 * 3.1415f / 2 - 3 * 3.1415f / 2;
            }
            else {
                tancuri[i].turela_angle = 3.1415f / 2 - 3 * 3.1415f / 2;
            }
        }
        if (tancuri[0].pos_x < tancuri[i].pos_x && tancuri[i].alive > 0) {
            if (tancuri[0].pos_z == tancuri[i].pos_z) {
                if (tancuri[0].pos_x > tancuri[i].pos_x) {
                    tancuri[i].turela_angle = 0 - 3 * 3.1415f / 2;
                }
                else {
                    tancuri[i].turela_angle = 3.1415f - 3 * 3.1415f / 2;
                }
            }
            else {
                if (tancuri[0].pos_z > tancuri[i].pos_z) {
                    tancuri[i].turela_angle = 3 * 3.1415f / 2 - atan((tancuri[i].pos_x - tancuri[0].pos_x) / (tancuri[0].pos_z - tancuri[i].pos_z)) - 3 * 3.1415f / 2;
                }
                else {
                    tancuri[i].turela_angle = 3.1415f / 2 + atan((tancuri[0].pos_x - tancuri[i].pos_x) / (tancuri[0].pos_z - tancuri[i].pos_z)) - 3 * 3.1415f / 2;
                }
            }
        }

        if (tancuri[i].random_movement == 0) {
            check = 1;
            new_x = tancuri[i].pos_x + deltaTimeSeconds * cos(tancuri[i].tanc_angle + 1.5f * 3.1415);
            new_z = tancuri[i].pos_z - deltaTimeSeconds * sin(tancuri[i].tanc_angle + 1.5f * 3.1415);
            for (int j = 0; j < 15; j++) {
                if (new_x > random_x[j] - 2.94f && new_x < random_x[j] + 2.94f &&
                    new_z > random_z[j] - 2.94f && new_z < random_z[j] + 2.94f) {
                    check = 0;
                    break;
                }
            }
            if (check == 1) {
                for (int j = 0; j < 10 && j != i; j++) {
                    if (new_x > tancuri[j].pos_x - 0.88f && new_x < tancuri[j].pos_x + 0.88f &&
                        new_z > tancuri[j].pos_z - 0.88f && new_z < tancuri[j].pos_z + 0.88f) {
                        check = 0;
                        break;
                    }
                }
            }
            if (check == 1) {
                tancuri[i].pos_x = new_x;
                tancuri[i].pos_z = new_z;
            }
        }
        if (tancuri[i].random_movement == 1) {
            check = 1;
            new_x = tancuri[i].pos_x - deltaTimeSeconds * cos(tancuri[i].tanc_angle + 1.5f * 3.1415);
            new_z = tancuri[i].pos_z + deltaTimeSeconds * sin(tancuri[i].tanc_angle + 1.5f * 3.1415);
            for (int j = 0; j < 15; j++) {
                if (new_x > random_x[j] - 2.94f && new_x < random_x[j] + 2.94f &&
                    new_z > random_z[j] - 2.94f && new_z < random_z[j] + 2.94f) {
                    check = 0;
                    break;
                }
            }
            if (check == 1) {
                for (int j = 0; j < 10 && j != i; j++) {
                    if (new_x > tancuri[j].pos_x - 0.88f && new_x < tancuri[j].pos_x + 0.88f &&
                        new_z > tancuri[j].pos_z - 0.88f && new_z < tancuri[j].pos_z + 0.88f) {
                        check = 0;
                        break;
                    }
                }
            }
            if (check == 1) {
                tancuri[i].pos_x = new_x;
                tancuri[i].pos_z = new_z;
            }
        }
        if (tancuri[i].random_movement == 2) {
            tancuri[i].tanc_angle += deltaTimeSeconds;
            if (tancuri[i].tanc_angle > 2 * 3.1415) {
                tancuri[i].tanc_angle -= 2 * 3.1415;
            }
            if (tancuri[i].tanc_angle < 0) {
                tancuri[i].tanc_angle += 2 * 3.1415;
            }
        }
        if (tancuri[i].random_movement == 3) {
            tancuri[i].tanc_angle -= deltaTimeSeconds;
            if (tancuri[i].tanc_angle > 2 * 3.1415) {
                tancuri[i].tanc_angle -= 2 * 3.1415;
            }
            if (tancuri[i].tanc_angle < 0) {
                tancuri[i].tanc_angle += 2 * 3.1415;
            }
        }
        
            
        

        tancuri[i].time_left -= deltaTimeSeconds;
        if (tancuri[i].time_left < 0) {
            tancuri[i].time_left = 2 + rand() % 3;
            tancuri[i].random_movement = rand() % 5;
        }

        if (tancuri[i].alive < 1) {
            tancuri[i].random_movement = 4;
        }



        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(tancuri[i].pos_x, 0.15f, tancuri[i].pos_z));
            modelMatrix = glm::rotate(modelMatrix, tancuri[i].tanc_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.15f, 0.8f));
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.5, 0.14 + 0.02 * tancuri[i].alive, 0));
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(tancuri[i].pos_x, 0.275f, tancuri[i].pos_z));
            modelMatrix = glm::rotate(modelMatrix, tancuri[i].turela_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.1f, 0.4f));
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.5, 0.25 + 0.05 * tancuri[i].alive, 0));
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(tancuri[i].pos_x - 0.125f * cos(tancuri[i].tanc_angle), 0.05f, tancuri[i].pos_z + 0.125f * sin(tancuri[i].tanc_angle)));
            modelMatrix = glm::rotate(modelMatrix, tancuri[i].tanc_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.05f, 0.7f));
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.8, 0.8, 0.8));
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(tancuri[i].pos_x + 0.125f * cos(tancuri[i].tanc_angle), 0.05f, tancuri[i].pos_z - 0.125f * sin(tancuri[i].tanc_angle)));
            modelMatrix = glm::rotate(modelMatrix, tancuri[i].tanc_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.05f, 0.7f));
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.8, 0.8, 0.8));
        }

        
        
    }



    for (int i = 0; i < 15; i++) {
        
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(random_x[i], 2.5f, random_z[i]));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(5, 5, 5));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0, 0));

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(random_x[i], 2.5f, random_z[i]));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(5.01f, 5.01f, 1));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 0));

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(random_x[i], 2.5f, random_z[i]));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 5.01f, 5.01f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 0));

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(random_x[i], 5, random_z[i]));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 2, 2));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 0));
    }

    for (int i = 0; i < 1000; i++) {
        if (tancuri[0].proiectil_array[i].is_it == 1) {
            tancuri[0].proiectil_array[i].delta_time -= deltaTimeSeconds;
            new_x = tancuri[0].proiectil_array[i].pos_x + deltaTimeSeconds * sin(tancuri[0].proiectil_array[i].alpha);
            new_z = tancuri[0].proiectil_array[i].pos_z + deltaTimeSeconds * cos(tancuri[0].proiectil_array[i].alpha);
            
            check = 1;
            for (int j = 0; j < 15; j++) {
                if (new_x > random_x[j] - 2.5f && new_x < random_x[j] + 2.5f &&
                    new_z > random_z[j] - 2.5f && new_z < random_z[j] + 2.5f) {
                    check = 0;
                    break;
                }
            }

            if (check == 1) {
                for (int j = 1; j < 10; j++) {
                    if (new_x > tancuri[j].pos_x - 0.3f && new_x < tancuri[j].pos_x + 0.3f &&
                        new_z > tancuri[j].pos_z - 0.3f && new_z < tancuri[j].pos_z + 0.3f) {
                        if (tancuri[j].alive > 0) {
                            tancuri[j].alive--;
                        }
                        check = 0;
                        break;
                    }
                }
            }

            if (check == 0 || tancuri[0].proiectil_array[i].delta_time < 0) {
                tancuri[0].proiectil_array[i].is_it = 0;
            }
            else {

                tancuri[0].proiectil_array[i].pos_x = new_x;
                tancuri[0].proiectil_array[i].pos_z = new_z;
            }


            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(tancuri[0].proiectil_array[i].pos_x, 0.27f, tancuri[0].proiectil_array[i].pos_z));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.07f, 0.07f, 0.07f));
        
            RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 0));
        }
    }
   
}

void Tema2::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light & material properties
    // TODO(student): Set light position uniform
    GLint light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(light_position, 1, glm::value_ptr(lightPosition));

    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    // TODO(student): Set eye position (camera position) uniform
    GLint eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(eye_position, 1, glm::value_ptr(eyePosition));

    // TODO(student): Set material property uniforms (shininess, kd, ks, object color)
    GLint pos_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(pos_shininess, materialShininess);

    GLint pos_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(pos_kd, materialKd);

    GLint pos_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(pos_ks, materialKs);

    GLint pos_object_colour = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(pos_object_colour, 1, glm::value_ptr(color));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    int check;
    float new_x, new_z;
    // TODO(student): Add transformation logic
    if (window->KeyHold(GLFW_KEY_W) && !(window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) && tancuri[0].alive > 0) {
        check = 1;
        new_x = tancuri[0].pos_x + deltaTime * cos(tancuri[0].tanc_angle + 1.5f * 3.1415);
        new_z = tancuri[0].pos_z - deltaTime * sin(tancuri[0].tanc_angle + 1.5f * 3.1415);
        for (int i = 0; i < 15; i++) {
            if (new_x > random_x[i] - 2.94f && new_x < random_x[i] + 2.94f &&
                new_z > random_z[i] - 2.94f && new_z < random_z[i] + 2.94f) {
                check = 0;
                break;
            }
        }
        if (check == 1) {
            for (int i = 1; i < 10; i++) {
                if (new_x > tancuri[i].pos_x - 0.88f && new_x < tancuri[i].pos_x + 0.88f &&
                    new_z > tancuri[i].pos_z - 0.88f && new_z < tancuri[i].pos_z + 0.88f) {
                    check = 0;
                    break;
                }
            }
        }
        if (check == 1) {
            tancuri[0].pos_x = new_x;
            tancuri[0].pos_z = new_z;
        }
    }

    if (window->KeyHold(GLFW_KEY_S) && !(window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) && tancuri[0].alive > 0) {
        check = 1;
        new_x = tancuri[0].pos_x - deltaTime * cos(tancuri[0].tanc_angle + 1.5f * 3.1415);
        new_z = tancuri[0].pos_z + deltaTime * sin(tancuri[0].tanc_angle + 1.5f * 3.1415);
        for (int i = 0; i < 15; i++) {
            if (new_x > random_x[i] - 2.94f && new_x < random_x[i] + 2.94f &&
                new_z > random_z[i] - 2.94f && new_z < random_z[i] + 2.94f) {
                check = 0;
                break;
            }
        }
        if (check == 1) {
            for (int i = 1; i < 10; i++) {
                if (new_x > tancuri[i].pos_x - 0.88f && new_x < tancuri[i].pos_x + 0.88f &&
                    new_z > tancuri[i].pos_z - 0.88f && new_z < tancuri[i].pos_z + 0.88f) {
                    check = 0;
                    break;
                }
            }
        }

        if (check == 1) {
            tancuri[0].pos_x = new_x;
            tancuri[0].pos_z = new_z;
        }
    }

    if (window->KeyHold(GLFW_KEY_A) && !(window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) && tancuri[0].alive > 0) {
        tancuri[0].tanc_angle += deltaTime;
        if (tancuri[0].tanc_angle > 2 * 3.1415) {
            tancuri[0].tanc_angle -= 2 * 3.1415;
        }
        if (tancuri[0].tanc_angle < 0) {
            tancuri[0].tanc_angle += 2 * 3.1415;
        }
    }

    if (window->KeyHold(GLFW_KEY_D) && !(window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) && tancuri[0].alive > 0) {
        tancuri[0].tanc_angle -= deltaTime;
        if (tancuri[0].tanc_angle > 2 * 3.1415) {
            tancuri[0].tanc_angle -= 2 * 3.1415;
        }
        if (tancuri[0].tanc_angle < 0) {
            tancuri[0].tanc_angle += 2 * 3.1415;
        }
    }

    if (window->KeyHold(GLFW_KEY_1)) {
        scaleX += deltaTime;
        scaleY += deltaTime;
        scaleZ += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_2)) {
        scaleX -= deltaTime;
        scaleY -= deltaTime;
        scaleZ -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_3)) {
        angularStepOX -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_4)) {
        angularStepOX += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_5)) {
        angularStepOY -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_6)) {
        angularStepOY += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_7)) {
        angularStepOZ -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_8)) {
        angularStepOZ += deltaTime;
    }


}


void Tema2::OnKeyPress(int key, int mods)
{


    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }

    // TODO(student): Add viewport movement and scaling logic
    glm::ivec2 resolution = window->GetResolution();
    if (key == GLFW_KEY_I) {
        miniViewportArea.y += 200;
    }
    else if (key == GLFW_KEY_K) {
        miniViewportArea.y -= 200;
    }
    else if (key == GLFW_KEY_J) {
        miniViewportArea.x -= 200;
    }
    else if (key == GLFW_KEY_L) {
        miniViewportArea.x += 200;
    }

    else if (key == GLFW_KEY_U) {
        miniViewportArea.height += resolution.y * 0.1f;
        miniViewportArea.width += resolution.x * 0.1f;
    }
    else if (key == GLFW_KEY_O) {
        miniViewportArea.height -= resolution.y * 0.1f;
        miniViewportArea.width -= resolution.x * 0.1f;
    }

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        tancuri[0].turela_angle = mouseX;
    }

    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_2 && tancuri[0].last_time < 0 && tancuri[0].alive > 0) {
        int ceva = tancuri[0].last;
        tancuri[0].proiectil_array[ceva].pos_x = tancuri[0].pos_x + 0.5f * sin(- tancuri[0].turela_angle / 100);
        tancuri[0].proiectil_array[ceva].pos_z = tancuri[0].pos_z + 0.5f * cos( - tancuri[0].turela_angle / 100);
        tancuri[0].proiectil_array[ceva].is_it = 1;
        tancuri[0].proiectil_array[ceva].alpha = - tancuri[0].turela_angle / 100;
        tancuri[0].last++;
        tancuri[0].last_time = 1;
    }
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
