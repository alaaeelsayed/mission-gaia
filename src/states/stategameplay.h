#ifndef STATEGAMEPLAY_H
#define STATEGAMEPLAY_H

#include "statebase.h"
#include "../camera/camera.h"
#include "../camera/freeroamcamera.h"
#include "../obj/skybox.h"
#include "../obj/plane.h"
#include "../obj/water.h"
#include "../obj/skybox.h"
#include "../text/textbox.h"
#include "../terrain/terrain.h"
#include "../misc/imgui/imgui.h"
#include "../misc/imgui/imgui_impl_glfw.h"
#include "../misc/imgui/imgui_impl_opengl3.h"

#include "../../irrklang/include/irrKlang.h"

class Model;
class StateGameplay : public Common::StateBase
{
public:
	//------------------------------------------------------------------------------
	// Public methods.
	//------------------------------------------------------------------------------

	class Light
	{
	public:
		bool enabled = true;
		glm::vec4 posRange;
		glm::vec3 color;
		glm::vec3 attenuation;
		glm::vec4 lightSpot = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 lightDir = glm::vec3(0.0f, 0.0f, 0.0f);
	};

	StateGameplay();
	virtual ~StateGameplay();

	// Overridden from StateBase
	virtual void Enter(std::string arg = "");
	virtual void Update(float dt);
	virtual void Render(const glm::mat4 &mProj, const glm::mat4 &mView);
	virtual void Exit();

private:
	void _renderTerrain();
	int _randomNum(int lowerBound, int upperBound);
	float _randomFloat(float lo, float hi);

	bool _isEffectiveLight(const Light *light1, const Light *light2, Model *model) const;

	const std::string m_groundTexPath = "data/textures/ground/ground.png";
	const std::string m_skyboxPath = "data/textures/skybox/skybox.png";

	const std::string m_natureSoundPath = "data/sounds/ambient.ogg";
	const std::string m_waterSoundPath = "data/sounds/ocean.wav";
	const std::string m_creatureGrowlPath = "data/sounds/creature_growl.wav";

	std::vector<Light *> m_lights;
	wolf::Program *m_worldProgram = 0;
	std::vector<Model *> m_models;
	std::vector<glm::vec3> m_positions;
	wolf::Material *m_mat = nullptr;

	wolf::Texture *m_creatureTex = nullptr;

	wolf::Model *m_shipModel = nullptr;
	wolf::Texture *m_shipTex = nullptr;

	Model *m_flashlight = nullptr;
	Light *m_spotlight = nullptr;

	wolf::SoundManager *m_soundManager = nullptr;

	const int m_gridSize = 20;

	float m_currShininess = 200.0f;

	float m_sunSpeed = 0.01f;
	glm::vec3 m_sunDirection = glm::vec3(10.0f, 10.0f, 30.0f);

	GLfloat m_sunAngle = -60;

	TerrainGenerator *m_terrainGenerator = 0;
	std::vector<Terrain *> m_terrains;

	Skybox *m_skybox = 0;

	bool m_keyDown = false;
	bool m_flashlightEquipped = false;

	Font *m_font;
	TextBox *m_hungerText;
	TextBox *m_thirstText;
	float m_hunger = 100;
	float m_thirst = 100;

	int m_lightCount = 0;

	// Water
	Water *m_water = nullptr;

	// Debug Menu
	bool m_debug = true;

	int m_terrainSize, m_terrainVerts, m_terrainOctaves;
	float m_terrainAmplitude, m_terrainRoughness;
};

#endif
