#define DEBUG

#include "quadtree.h"

Quadtree::Quadtree(int level, const BoundingBox &boundingBox)
    : m_level(level), m_boundingBox(boundingBox), m_boxFrame(boundingBox), m_isLeaf(true)
{
}

Quadtree::~Quadtree()
{
    for (int i = 0; i < 4; i++)
    {
        if (!m_children[i])
            continue;

        delete m_children[i];
        m_children[i] = nullptr;
    }
}

void Quadtree::AddNode(Node *node)
{
    if (!GetBoundingBox().Overlaps(node->GetBoundingBox()))
        return;

    bool added = false;
    for (int i = 0; i < 4; i++)
    {
        if (!m_children[i])
            continue;
        Quadtree *childTree = m_children[i];
        childTree->AddNode(node);
        added = true;
    }

    if (added)
        return;

    m_nodes.push_back(node);

    if (m_isLeaf && m_nodes.size() > QuadtreeLimits::minNodes && m_level < QuadtreeLimits::maxLevel)
    {
        _subdivide();
    }
}

void Quadtree::RemoveNode(Node *node)
{
    std::vector<Node *>::iterator it = std::find(m_nodes.begin(), m_nodes.end(), node);
    if (it != m_nodes.end())
    {
        m_nodes.erase(it);
    }

    for (int i = 0; i < 4; i++)
    {
        if (!m_children[i])
            continue;

        m_children[i]->RemoveNode(node);
    }
}

void Quadtree::Clear()
{
    m_nodes.clear();

    for (int i = 0; i < 4; i++)
    {
        delete m_children[i];
        m_children[i] = nullptr;
    }
}

BoundingBox Quadtree::GetBoundingBox() const
{
    return m_boundingBox;
}

int Quadtree::GetLevel() const
{
    return m_level;
}

std::vector<Node *> Quadtree::GetNodesInFrustum(const Culling::Frustum &frustum) const
{
    std::vector<Node *> nodes = m_nodes;

    for (int i = 0; i < 4; i++)
    {
        if (!m_children[i])
            continue;
        Quadtree *childTree = m_children[i];
        if (Culling::Instance()->BoxInFrustum(childTree->GetBoundingBox(), frustum))
        {
            std::vector<Node *> otherNodes = childTree->GetNodesInFrustum(frustum);
            nodes.insert(nodes.end(), otherNodes.begin(), otherNodes.end());
        }
    }

    return nodes;
}

void Quadtree::RenderDebug(const glm::mat4 &mProj, const glm::mat4 &mView)
{
#ifdef DEBUG
    m_boxFrame.Render(mProj, mView);

    for (int i = 0; i < 4; i++)
    {
        if (!m_children[i])
            continue;
        Quadtree *childTree = m_children[i];
        childTree->RenderDebug(mProj, mView);
    }
#endif
}

void Quadtree::_subdivide()
{
    glm::vec3 center = m_boundingBox.GetCenter();
    glm::vec3 min = m_boundingBox.GetMin();
    glm::vec3 max = m_boundingBox.GetMax();

    /*
    MINX MAXZ    CENTX MAXZ     MAXX MAXZ

    MINX CENTZ   CENTX CENTZ    MAXX CENTZ

    MINX MINZ    CENTX MINZ     MAXX MINZ
    */

    m_children[TopLeft] = new Quadtree(m_level + 1, BoundingBox(glm::vec3(min.x, 0, center.z),
                                                                glm::vec3(center.x, QuadtreeLimits::treeHeight, max.z)));

    m_children[TopRight] = new Quadtree(m_level + 1, BoundingBox(glm::vec3(center.x, 0, center.z),
                                                                 glm::vec3(max.x, QuadtreeLimits::treeHeight, max.z)));

    m_children[BottomLeft] = new Quadtree(m_level + 1, BoundingBox(glm::vec3(min.x, 0, min.z),
                                                                   glm::vec3(center.x, QuadtreeLimits::treeHeight, center.z)));

    m_children[BottomRight] = new Quadtree(m_level + 1, BoundingBox(glm::vec3(center.x, 0, min.z),
                                                                    glm::vec3(max.x, QuadtreeLimits::treeHeight, center.z)));

    m_isLeaf = false;

    for (auto it = m_nodes.begin(); it != m_nodes.end(); it = m_nodes.erase(it))
    {
        for (int i = 0; i < 4; i++)
        {
            m_children[i]->AddNode(*it);
        }
    }
}
