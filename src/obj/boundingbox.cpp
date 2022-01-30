#include "boundingbox.h"

BoundingBox::BoundingBox(const glm::vec3 &min, const glm::vec3 &max)
    : m_min(min), m_max(max)
{
    _recalculateCenter();
    _recalculateDimensions();
}

BoundingBox::BoundingBox()
    : m_min(0.0f), m_max(0.0f), m_center(0.0f)
{
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::Extend(const BoundingBox &child)
{
    glm::vec3 otherMin = child.GetMin();
    glm::vec3 otherMax = child.GetMax();

    // GLM conveniently has a min and a max method for vectors
    m_min = glm::min(m_min, otherMin);
    m_max = glm::max(m_max, otherMax);

    _recalculateCenter();
    _recalculateDimensions();
}

bool BoundingBox::InBounds(const BoundingBox &otherBox)
{
    glm::vec3 otherMin = otherBox.GetMin();
    glm::vec3 otherMax = otherBox.GetMax();
    return m_min.x <= otherMin.x && m_min.y <= otherMin.y && m_min.z <= otherMin.z && m_max.x >= otherMax.x && m_max.y >= otherMax.y && m_max.z >= otherMax.z;
}

bool BoundingBox::Overlaps(const BoundingBox &otherBox)
{
    glm::vec3 otherMin = otherBox.GetMin();
    glm::vec3 otherMax = otherBox.GetMax();
    return m_max.x >= otherMin.x && otherMax.x >= m_min.x && m_max.y >= otherMin.y && otherMax.y >= m_min.y && m_max.z >= otherMin.z && otherMax.z >= m_min.z;
}

void BoundingBox::SetMin(const glm::vec3 &min)
{
    m_min = min;

    _recalculateCenter();
    _recalculateDimensions();
}

void BoundingBox::SetMax(const glm::vec3 &max)
{
    m_max = max;

    _recalculateCenter();
    _recalculateDimensions();
}

void BoundingBox::SetWidth(float width)
{
    if (m_dimensions.x == width)
        return;

    // maxX = maxX - m_dimensions.x + width
    m_max.x += width - m_dimensions.x;
    m_dimensions.x = width;

    _recalculateCenter();
}

void BoundingBox::SetHeight(float height)
{
    if (m_dimensions.y == height)
        return;

    m_max.y += height - m_dimensions.y;
    m_dimensions.y = height;

    _recalculateCenter();
}

void BoundingBox::SetDepth(float depth)
{
    if (m_dimensions.z == depth)
        return;

    m_max.z += depth - m_dimensions.z;
    m_dimensions.z = depth;

    _recalculateCenter();
}

glm::vec3 BoundingBox::GetMin() const
{
    return m_min;
}

glm::vec3 BoundingBox::GetMax() const
{
    return m_max;
}

glm::vec3 BoundingBox::GetCenter() const
{
    return m_center;
}

float BoundingBox::GetWidth() const
{
    return m_dimensions.x;
}

float BoundingBox::GetHeight() const
{
    return m_dimensions.y;
}

float BoundingBox::GetDepth() const
{
    return m_dimensions.z;
}

glm::vec3 BoundingBox::GetDimensions() const
{
    return m_dimensions;
}

void BoundingBox::_recalculateCenter()
{
    m_center = (m_min + m_max) / 2.0f;
}

void BoundingBox::_recalculateDimensions()
{
    // (x+width, y+height, z+depth) - (x, y, z) = (width, height, depth)
    m_dimensions = m_max - m_min;
}