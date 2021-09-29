#include "../../wolf/wolf.h"

class SingleMaterialProvider : public wolf::Model::MaterialProvider
{
public:
    SingleMaterialProvider(const std::string &matName) : m_matName(matName) {}

    wolf::Material *getMaterial(const std::string &nodeName, int subMeshIndex, const std::string &name) const override
    {
        // Regardless of what mesh index or mat name the model wants, we just
        // use the mat we were seeded with. Note that we create a new one each
        // time so the DestroyMaterial calls line up. This could be improved,
        // but they do share shaders.
        return wolf::MaterialManager::CreateMaterial(m_matName);
    }

private:
    std::string m_matName;
};
