#include "scene.h"
#include "culling.h"

Scene *Scene::s_sceneInstance = NULL;

void Scene::CreateInstance()
{
    s_sceneInstance = new Scene();
}

void Scene::DestroyInstance()
{
    s_sceneInstance = NULL;
}

Scene *Scene::Instance()
{
    return s_sceneInstance;
}

Scene::Scene()
    : m_activeCamera(NULL)
{
}

Scene::~Scene()
{
}

void Scene::AddNode(Node *node)
{
    m_nodes.push_back(node);
}

void Scene::RemoveNode(Node *node)
{
    std::vector<Node *>::iterator it = std::find(m_nodes.begin(), m_nodes.end(), node);
    if (it != m_nodes.end())
    {
        m_nodes.erase(it);
    }
}

void Scene::Clear()
{
    m_nodes.clear();
}

void Scene::SetActiveCamera(Camera *camera)
{
    m_activeCamera = camera;
}

Camera *Scene::GetActiveCamera()
{
    return m_activeCamera;
}

void Scene::BuildQuadtree()
{
    if (m_quadtree)
        delete m_quadtree;
    m_quadtree = new Quadtree(0, BoundingBox(glm::vec3(-200, 0, -200), glm::vec3(200, QuadtreeLimits::treeHeight, 200)));

    for (Node *node : m_nodes)
    {
        m_quadtree->AddNode(node);
    }
}

void Scene::SetWidth(int width)
{
    m_width = width;
}

void Scene::SetHeight(int height)
{
    m_height = height;
}

void Scene::Render()
{
    if (m_activeCamera == NULL)
    {
        return;
    }

    const glm::mat4 &mProj = m_activeCamera->GetProjMatrix(m_width, m_height);
    const glm::mat4 &mView = m_activeCamera->GetViewMatrix();

    for (Node *node : m_objectsToRender)
    {
        node->Render(mProj, mView);
    }

    if (m_renderDebug)
        m_quadtree->RenderDebug(mProj, mView);
}

void Scene::Update(float dt)
{
    for (Node *node : m_nodes)
    {
        node->Update(dt);
    }

    const glm::mat4 &mProj = m_activeCamera->GetProjMatrix(m_width, m_height);
    const glm::mat4 &mView = m_activeCamera->GetViewMatrix();

    m_objectsToRender = m_nodes;
}

void Scene::ToggleDebug()
{
    m_renderDebug = !m_renderDebug;
}