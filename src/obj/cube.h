#pragma once
#include "../../wolf/wolf.h"
#include "node.h"
#include <PxPhysicsAPI.h>

using namespace physx;

class Cube : public Node
{
public:
	Cube(const std::string& texturePath);
	virtual ~Cube();

	PxRigidDynamic* GetRigidBody() { return m_rigidBody; }

	void Update(float dt) override;
	void Render(const glm::mat4& mProj, const glm::mat4& mView) override;

	void SetPos(const glm::vec3& position) override;

	void SetWidth(float width);
	void SetHeight(float height);
	void SetDepth(float depth);

	float GetWidth() const;
	float GetHeight() const;
	float GetDepth() const;

private:
	static const Vertex s_cubeVertices[36];

	bool m_dirty;

	PxRigidDynamic* m_rigidBody;

	wolf::VertexBuffer* m_vb = 0;
	wolf::VertexDeclaration* m_decl = 0;
	wolf::Texture* m_texture = 0;
	wolf::Material* m_material = 0;
};
