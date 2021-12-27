#include "node.h"

int Node::s_nodeCounter = 0;

Node::Node(const BoundingBox &boundingBox)
    : m_boundingBox(boundingBox), m_scale(0.0f), m_position(0.0f), m_rotation(0.0f), m_dirty(true)
{
    m_name = "Node" + std::to_string(s_nodeCounter);
    s_nodeCounter++;
}

Node::~Node()
{
    for (Node *node : m_children)
    {
        delete node;
    }
    // shouldn't decrement
}

void Node::SetScale(const glm::vec3 &scale)
{
    m_scale = scale;
    m_dirty = true;
}

void Node::SetPos(const glm::vec3 &pos)
{
    m_position = pos;
    m_dirty = true;
}

void Node::SetRotation(const glm::vec3 &rot)
{
    m_rotation = rot;
    m_dirty = true;
}

void Node::Scale(const glm::vec3 &scale)
{
    m_scale *= scale;
    m_dirty = true;
}

void Node::Rotate(const glm::vec3 &rot)
{
    m_rotation += rot;
    m_dirty = true;
}

void Node::Translate(const glm::vec3 &pos)
{
    m_position += pos;
    m_dirty = true;
}

void Node::AddChild(Node *child)
{
    m_children.push_back(child);

    m_boundingBox.Extend(child->GetBoundingBox());
}

void Node::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
    for (Node *child : m_children)
    {
        // Should probably clean the vector somehow, sometime, or ensure this never happens
        if (!child)
            continue;

        child->Render(mProj, mView);
    }
}

void Node::Update(float dt)
{
    for (Node *child : m_children)
    {
        if (!child)
            continue;

        if (!child->GetBoundingBox().InBounds(GetBoundingBox()))
            GetBoundingBox().Extend(child->GetBoundingBox());

        child->Update(dt);
    }
}

std::string Node::GetName()
{
    return m_name;
}

glm::mat4 Node::GetTransform()
{
    if (m_dirty)
    {
        glm::vec3 radRotation = glm::vec3(DEG_TO_RAD(m_rotation.x), DEG_TO_RAD(m_rotation.y), DEG_TO_RAD(m_rotation.z));
        m_transform = glm::translate(m_position) *
                      glm::mat4_cast(glm::quat(radRotation)) *
                      glm::scale(m_scale);
        m_dirty = false;
    }
    return m_transform;
}

glm::mat4 Node::GetWorldTransform()
{
    if (m_parent)
    {
        return m_parent->GetWorldTransform() * GetTransform();
    }
    return GetTransform();
}

glm::vec3 Node::GetScale()
{
    return m_scale;
}

glm::vec3 Node::GetWorldScale()
{
    if (m_parent)
    {
        return m_parent->GetWorldScale() * m_scale;
    }
    return m_scale;
}

glm::vec3 Node::GetPos()
{
    return m_position;
}

glm::vec3 Node::GetWorldPos()
{
    if (m_parent)
    {
        return m_parent->GetWorldPos() + m_position;
    }
    return m_position;
}

glm::vec3 Node::GetRotation()
{
    return m_rotation;
}

glm::vec3 Node::GetWorldRotation()
{
    if (m_parent)
    {
        return m_parent->GetWorldRotation() + m_rotation;
    }
    return m_rotation;
}

std::vector<Node *> Node::GetChildren()
{
    return m_children;
}

Node *Node::GetParent()
{
    return m_parent;
}

BoundingBox &Node::GetBoundingBox()
{
    return m_boundingBox;
}