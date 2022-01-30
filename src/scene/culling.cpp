#include "culling.h"
#include "quadtree.h"

Culling *Culling::s_cullingInstance = NULL;

void Culling::CreateInstance()
{
    // TODO: Implement occlusion culling
    s_cullingInstance = new Culling();
}

void Culling::DestroyInstance()
{
    delete s_cullingInstance;
}

Culling *Culling::Instance()
{
    return s_cullingInstance;
}

Culling::Culling()
{
}

Culling::~Culling()
{
}

// Default frustum culling
// Based on https://www.gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf
std::vector<Node *> Culling::DoCulling(const Quadtree *quadtree, const glm::mat4 &mProj, const glm::mat4 &mView)
{
    Frustum frustum;
    glm::mat4 comboMatrix = mProj * mView;

    // Left plane
    frustum.planes[Left] = glm::row(comboMatrix, 3) + glm::row(comboMatrix, 0);

    // Right plane
    frustum.planes[Right] = glm::row(comboMatrix, 3) - glm::row(comboMatrix, 0);

    // Bottom plane
    frustum.planes[Bottom] = glm::row(comboMatrix, 3) + glm::row(comboMatrix, 1);

    // Top plane
    frustum.planes[Top] = glm::row(comboMatrix, 3) - glm::row(comboMatrix, 1);

    // Near plane
    frustum.planes[Near] = glm::row(comboMatrix, 3) + glm::row(comboMatrix, 2);

    // Far plane
    frustum.planes[Far] = glm::row(comboMatrix, 3) - glm::row(comboMatrix, 2);

    _normalize(frustum);

    return quadtree->GetNodesInFrustum(frustum);
}

bool Culling::BoxInFrustum(const BoundingBox &boundingBox, const Frustum &frustum)
{

    const glm::vec3 extents = boundingBox.GetDimensions() / 2.0f;
    const glm::vec3 center = boundingBox.GetCenter();

    bool inside = false;

    for (int i = 0; i < 6; i++)
    {
        const glm::vec4 plane = frustum.planes[i];
        for (int j = 0; j < 8; j++)
        {
            // Check if the corner is in the frustum
            const glm::vec3 offset{
                j % 2 == 0 ? -1.0f : 1.0f,
                (j / 2) % 2 == 0 ? -1.0f : 1.0f,
                (j / 4) % 2 == 0 ? -1.0f : 1.0f,
            };

            const glm::vec4 point = glm::vec4(center + extents * offset, 1.0f);

            inside |= glm::dot(plane, point) >= 0;
        }
        if (!inside)
        {
            return false;
        }
    }

    return inside;
}

void Culling::_normalize(Frustum &frustum)
{
    for (int i = 0; i < 6; i++)
    {
        glm::vec4 plane = frustum.planes[i];
        float length = sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);

        frustum.planes[i] /= length;
    }
}