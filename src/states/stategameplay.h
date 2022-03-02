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
#include "../misc/util.h"
#include "./statemachine.h"
#include "../../samplefw/Sphere.h"
#include "../particles/effect.h"

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
	const std::string m_flashlightSoundPath = "data/sounds/flashlight.ogg";
	const std::string m_runningSoundPath = "data/sounds/running.ogg";
	const std::string m_walkingSoundPath = "data/sounds/walking.ogg";
	const std::string m_drinkingSoundPath = "data/sounds/drinking.ogg";
	const std::string m_eatingSoundPath = "data/sounds/eating.ogg";
	const std::string m_pickupSoundPath = "data/sounds/pickup.wav";
	const std::string m_repairSoundPath = "data/sounds/repair.wav";

	const std::string m_drinkPrompt = "Press E to drink";
	const std::string m_eatPrompt = "Press E to eat";
	const std::string m_collectPrompt = "Press E to Collect";
	const std::string m_repairPrompt = "Press E to Repair";
	const std::string m_noPartsPrompt = "Collect parts to repair ship";

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
	bool m_debugDown = false;
	bool m_flashlightEquipped = false;
	bool m_drinking = false;
	bool m_eating = false;

	Font *m_font;
	TextBox *m_hungerText;
	TextBox *m_thirstText;

	TextBox *m_collectText;
	TextBox *m_repairShipText;
	TextBox *m_collectMorePartsText;
	TextBox *m_partsCollectedText;

	float m_hunger = 100;
	float m_thirst = 100;
	int m_numParts = 0;

	TextBox *m_drinkText;
	TextBox *m_eatText;

	bool m_nearWater = false;
	bool m_nearFood = false;
	bool m_nearCollectible = false;
	bool m_nearShip = false;

	int m_lightCount = 0;

	// Water
	std::vector<Water *> m_waters;

	// Debug Menu
	bool m_debug = true;
	bool m_walking = true;
	bool m_running = true;

	int m_terrainSize, m_terrainVerts, m_terrainOctaves;
	float m_terrainAmplitude, m_terrainRoughness;

	float m_enemySpeed = 0.8f;
	glm::vec3 m_enemyRange = glm::vec3(70.0f, 20.0f, 70.0f);
	glm::vec3 m_foodRange = glm::vec3(20.0f, 20.0f, 20.0f);
	glm::vec3 m_collectibleRange = glm::vec3(5.0f, 5.0f, 5.0f);

	bool m_inventoryFull = false;
	int m_repairedParts = 0;

	// Bullets
	std::vector<Sphere *> m_bullets;
	bool m_rightMouseDown = false;

	Common::StateMachine *m_stateMachine = nullptr;

	// Effects
	std::vector<Effect *> m_effects;
	std::string m_firepath = "data/effects/fire.pfx";
	std::string m_forcefieldPath = "data/effects/forcefield.pfx";

	std::string m_fireSound = "data/sounds/fire-sound.ogg";
};

#endif
