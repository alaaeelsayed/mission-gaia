#pragma once

#include "../../wolf/wolf.h"
#include "../camera/camera.h"
#include "../obj/node.h"
#include "quadtree.h"

class Scene
{
public:
    static void CreateInstance();
    static void DestroyInstance();
    static Scene *Instance();

    void AddNode(Node *node);
    void RemoveNode(Node *node);
    void Clear();

    void SetActiveCamera(Camera *camera);
    Camera *GetActiveCamera();

    void BuildQuadtree();

    void SetWidth(int width);
    void SetHeight(int height);

    void Render();
    void Update(float dt);

    void ToggleDebug();
    glm::vec3 GetLightDirection();

private:
    Scene();
    ~Scene();

private:
    static Scene *s_sceneInstance;

    int m_width, m_height;

    bool m_renderDebug = false;
    float m_rotation = 0.0f;

    Quadtree *m_quadtree = 0;

    std::vector<Node *> m_nodes;
    std::vector<Node *> m_objectsToRender;

    Camera *m_activeCamera;
};