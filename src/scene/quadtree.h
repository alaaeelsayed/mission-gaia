#pragma once

#include "../obj/node.h"
#include "../obj/boundingbox.h"
#include "../obj/boxframe.h"
#include "culling.h"

namespace QuadtreeLimits
{
    const int maxLevel = 8;
    const int minNodes = 2;
    const int treeHeight = 2000;
}

class Quadtree
{
public:
    enum Quadrant
    {
        TopLeft = 0,
        TopRight,
        BottomLeft,
        BottomRight
    };

    Quadtree(int level, const BoundingBox &boundingBox);
    ~Quadtree();

    void AddNode(Node *node);
    void RemoveNode(Node *node);
    void Clear();

    BoundingBox GetBoundingBox() const;
    int GetLevel() const;

    std::vector<Node *> GetNodesInFrustum(const Culling::Frustum &frustum) const;

    void RenderDebug(const glm::mat4 &mProj, const glm::mat4 &mView);

private:
    void _subdivide();

private:
    bool m_isLeaf;

    Quadtree *m_children[4] = {0};

    std::vector<Node *> m_nodes;
    int m_level;
    BoundingBox m_boundingBox;
    BoxFrame m_boxFrame;
};