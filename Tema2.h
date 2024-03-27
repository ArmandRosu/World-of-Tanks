#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema2/lab_camera_2.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderScene();
    protected:
        implemented::Camera2* camera;
        bool renderCameraTarget;
        glm::mat4 modelMatrix;
        glm::mat4 projectionMatrix;
        float scaleX, scaleY, scaleZ;
        float angularStepOX, angularStepOY, angularStepOZ;
        GLenum polygonMode;
        ViewportArea miniViewportArea;

        glm::vec3 lightPosition, lightDirection;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;
        int isSpotligth;
        int score = 0;
        float cut_off_angle;

        float random_x[15], random_z[15];

        int next_proiectil = 0;
        float time_left = 60;
        int is_over = 0;

    };
}   // namespace m1

