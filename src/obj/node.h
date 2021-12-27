#pragma once

#include "../../wolf/wolf.h"
#include "boundingbox.h"

struct Vertex
{
    GLfloat x, y, z;
    GLfloat r, g, b, a;
};

class Node
{
public:
    Node(const BoundingBox &boundingBox);
    ~Node();

    void SetScale(const glm::vec3 &scale);
    void SetPos(const glm::vec3 &position);
    void SetRotation(const glm::vec3 &rotation);
    void Scale(const glm::vec3 &scale);
    void Rotate(const glm::vec3 &rotation);
    void Translate(const glm::vec3 &position);

    void AddChild(Node *child);

    virtual void Update(float dt);
    virtual void Render(const glm::mat4 &mProj, const glm::mat4 &mView);

    std::string GetName();

    glm::mat4 GetTransform();
    glm::mat4 GetWorldTransform();
    glm::vec3 GetScale();
    glm::vec3 GetWorldScale();
    glm::vec3 GetPos();
    glm::vec3 GetWorldPos();
    glm::vec3 GetRotation();
    glm::vec3 GetWorldRotation();

    std::vector<Node *> GetChildren();
    Node *GetParent();

    BoundingBox &GetBoundingBox();

private:
    static int s_nodeCounter;

    std::string m_name;

    glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 m_rotation;
    glm::vec3 m_position;
    glm::mat4 m_transform;
    bool m_dirty;

    std::vector<Node *> m_children;
    Node *m_parent = nullptr;

    BoundingBox m_boundingBox;
};