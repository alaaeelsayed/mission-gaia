#pragma once

#include "../obj/node.h"
#include <vector>

class Quadtree;

class Culling
{
public:
    // for reference and accessibility
    enum Plane
    {
        Left = 0,
        Right,
        Bottom,
        Top,
        Near,
        Far
    };
    struct Frustum
    {
        glm::vec4 planes[6];
    };

    static void CreateInstance();
    static void DestroyInstance();
    static Culling *Instance();

    std::vector<Node *> DoCulling(const Quadtree *quadtree, const glm::mat4 &mProj, const glm::mat4 &mView);

    bool BoxInFrustum(const BoundingBox &boundingBox, const Frustum &frustum);
    bool PointInFrustum(const glm::vec3 &point, const Frustum &frustum);

private:
    static Culling *s_cullingInstance;

    Culling();
    ~Culling();
    void _normalize(Frustum &frustum);
};