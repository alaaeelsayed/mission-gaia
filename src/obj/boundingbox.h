#pragma once

#include "../../wolf/wolf.h"

// AABB
class BoundingBox
{
public:
    BoundingBox(const glm::vec3 &min, const glm::vec3 &max);
    BoundingBox();
    ~BoundingBox();

    // Extend bounding box to include this child box
    void Extend(const BoundingBox &child);

    bool InBounds(const BoundingBox &otherBox);
    bool Overlaps(const BoundingBox &otherBox);

    void SetMin(const glm::vec3 &min);
    void SetMax(const glm::vec3 &max);

    void SetWidth(float width);
    void SetHeight(float height);
    void SetDepth(float depth);

    glm::vec3 GetMin() const;
    glm::vec3 GetMax() const;
    glm::vec3 GetCenter() const;
    float GetWidth() const;
    float GetHeight() const;
    float GetDepth() const;
    glm::vec3 GetDimensions() const;

private:
    void _recalculateDimensions();
    void _recalculateCenter();

    // A vec3 instead of 3 floats, to do calculations more easily
    glm::vec3 m_dimensions;

    glm::vec3 m_center;
    glm::vec3 m_min, m_max;
};