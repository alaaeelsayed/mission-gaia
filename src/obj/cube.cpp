#include "cube.h"
#include "../../wolf/W_PhysicsManager.h"

const Vertex Cube::s_cubeVertices[36] = {
	// Front
	{-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
	{0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
	{0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},

	// Back
	{0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f},
	{0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f},
	{-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f},
	{-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f},
	{-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f},
	{0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f},

	// Left
	{-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f},
	{-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f},
	{-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f},
	{-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f},
	{-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
	{-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f},

	// Right
	{0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
	{0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
	{0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f},
	{0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f},
	{0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
	{0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},

	// Top
	{-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
	{0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
	{0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
	{0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
	{-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
	{-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},

	// Bottom
	{-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f},
	{-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f},
	{0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f},
	{0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f},
	{0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f},
	{-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f},
};

Cube::~Cube()
{
	delete m_decl;
	wolf::BufferManager::DestroyBuffer(m_vb);
	wolf::TextureManager::DestroyTexture(m_texture);
	wolf::MaterialManager::DestroyMaterial(m_material);
}

Cube::Cube(const std::string& texturePath)
	: Node(BoundingBox())
{
	m_vb = wolf::BufferManager::CreateVertexBuffer(s_cubeVertices, sizeof(Vertex) * 6 * 3 * 2);

	m_texture = wolf::TextureManager::CreateTexture(texturePath);

	// Material should be unique to a texture
	m_material = wolf::MaterialManager::CreateMaterial(texturePath);
	if (!m_material->GetProgram())
	{
		m_material->SetProgram("data/shaders/world.vsh", "data/shaders/world.fsh");
		m_material->SetTexture("u_texture", m_texture);
	}

	m_decl = new wolf::VertexDeclaration();
	m_decl->Begin();
	m_decl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
	m_decl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
	m_decl->AppendAttribute(wolf::AT_Normal, 3, wolf::CT_Float);
	m_decl->SetVertexBuffer(m_vb);
	m_decl->End();


	m_rigidBody = PxCreateDynamic(*wolf::PhysicsManager::Instance()->GetPhysics(),
		PxTransform(PxVec3(0, 1000, 0)), PxBoxGeometry(0.5f, 0.5f, 0.5f),
		*wolf::PhysicsManager::Instance()->GetPhysics()->createMaterial(0.5f, 0.5f, 0.6f), 10.0f);
	m_rigidBody->setAngularDamping(0.5f);
}

void Cube::SetPos(const glm::vec3& position)
{
	Node::SetPos(position);
	m_rigidBody->setGlobalPose(PxTransform(PxVec3(position.x, position.y, position.z)));
}

void Cube::Update(float dt)
{
	Node::SetPos(glm::vec3(m_rigidBody->getGlobalPose().p.x, m_rigidBody->getGlobalPose().p.y, m_rigidBody->getGlobalPose().p.z));

	if (m_dirty)
	{
		glm::vec3 dimensions = GetScale();
		glm::vec3 min = GetWorldPos() - dimensions / 2.0f;
		glm::vec3 max = min + dimensions;
		GetBoundingBox().SetMin(min);
		GetBoundingBox().SetMax(max);
		m_dirty = false;
	}
}

void Cube::Render(const glm::mat4& mProj, const glm::mat4& mView)
{
	m_material->SetUniform("projection", mProj);
	m_material->SetUniform("view", mView);
	m_material->SetUniform("world", GetWorldTransform());
	m_material->Apply();

	m_texture->Bind();

	m_decl->Bind();

	glDrawArrays(GL_TRIANGLES, 0, 6 * 3 * 2);
}