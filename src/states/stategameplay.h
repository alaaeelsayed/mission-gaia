#ifndef STATEGAMEPLAY_H
#define STATEGAMEPLAY_H

#include "statebase.h"
#include "../camera/camera.h"
#include "../camera/freeroamcamera.h"
#include "../camera/orthocamera.h"
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

#include "../obj/triangle.h"
#include "../obj/quad.h"

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
	void _generateTerrain(int rangeStart, int rangeEnd);
	bool _inRange(int chunkX, int chunkZ);
	void _renderTerrain();
	int _randomNum(int lowerBound, int upperBound);
	float _randomFloat(float lo, float hi);

	bool _isEffectiveLight(const Light *light1, const Light *light2, Model *model) const;

	const std::string m_groundTexPath = "data/textures/ground/ground.png";
	const std::string m_skyboxPath = "data/textures/skybox/skybox.png";
	const std::string m_flyingShipTex = "data/textures/flying-ship-texture.png";

	const std::string m_natureSoundPath = "data/sounds/ambient.ogg";
	const std::string m_waterSoundPath = "data/sounds/ocean.wav";
	const std::string m_creatureGrowlPath = "data/sounds/creature_growl.wav";
	const std::string m_flashlightSoundPath = "data/sounds/flashlight.ogg";
	const std::string m_gravityGunSoundPath = "data/sounds/gravity-gun.wav";
	const std::string m_gravityGunShutdownSoundPath = "data/sounds/gravity-gun-shutdown.wav";
	const std::string m_gravityGunFaultyPath = "data/sounds/gravity-gun-faulty.wav";

	const std::string m_runningSoundPath = "data/sounds/running.ogg";
	const std::string m_walkingSoundPath = "data/sounds/walking.ogg";
	const std::string m_drinkingSoundPath = "data/sounds/drinking.ogg";
	const std::string m_eatingSoundPath = "data/sounds/eating.ogg";
	const std::string m_pickupSoundPath = "data/sounds/pickup.wav";
	const std::string m_repairSoundPath = "data/sounds/repair.wav";
	const std::string m_painSoundPath = "data/sounds/pain.wav";
	const std::string m_shipBeepSoundPath = "data/sounds/ship-beep.wav";

	const std::string m_drinkPrompt = "Press E to drink";
	const std::string m_eatPrompt = "Press E to eat";
	const std::string m_collectPrompt = "Press E to Collect";
	const std::string m_repairPrompt = "Press E to Repair";
	const std::string m_noPartsPrompt = "Collect parts to repair ship";

	std::vector<Light *> m_lights;
	wolf::Program *m_worldProgram = 0;
	std::vector<Model *> m_models;
	std::vector<Model *> m_shipParts;
	std::vector<glm::vec3> m_positions;
	wolf::Material *m_mat = nullptr;

	wolf::Texture *m_creatureTex = nullptr;

	Model *m_ship = nullptr;

	wolf::Texture *m_shipTex = nullptr;

	Model *m_flashlight = nullptr;
	Model *m_gravityGun = nullptr;
	bool m_attackingEnemy = false;
	float m_force = 0.0f;
	float m_gravityGunForce = 0.0f;
	Light *m_spotlight = nullptr;

	wolf::SoundManager *m_soundManager = nullptr;

	const int m_gridSize = 20;

	float m_currShininess = 200.0f;

	float m_sunSpeed = 0.01f;
	glm::vec3 m_sunDirection = glm::vec3(10.0f, 10.0f, 30.0f);

	GLfloat m_sunAngle = -60;

	TerrainGenerator *m_terrainGenerator = 0;
	std::map<std::pair<int, int>, Terrain *> m_terrainMap;
	Terrain::Biome m_curBiome = Terrain::Regular;

	int m_loadedStart = 0;
	int m_loadedEnd = 0;

	std::vector<std::thread> m_threads;

	Skybox *m_skybox = 0;

	bool m_keyDown = false;
	bool m_gravityKeyDown = false;
	bool m_scannerButtonDown = false;
	bool m_debugDown = false;
	bool m_flashlightEquipped = false;
	bool m_gravityGunEquipped = false;
	bool m_drinking = false;
	bool m_eating = false;

	Font *m_font;
	TextBox *m_hungerText;
	TextBox *m_thirstText;

	TextBox *m_collectText;
	TextBox *m_repairShipText;
	TextBox *m_collectMorePartsText;
	TextBox *m_partsCollectedText;

	float m_health = 100;
	float m_hunger = 100;
	float m_thirst = 100;
	int m_numParts = 0;

	TextBox *m_drinkText;
	TextBox *m_eatText;

	bool m_nearWater = false;
	int m_waterIndex = 0;

	bool m_nearFood = false;
	bool m_nearCollectible = false;
	bool m_nearShip = false;

	int m_lightCount = 0;

	// Water
	std::map<std::pair<int, int>, Water *> m_waterMap;

	// Debug Menu
	bool m_debug = true;
	bool m_walking = true;
	bool m_running = true;

	int m_terrainSize, m_terrainVerts, m_terrainOctaves;
	float m_terrainAmplitude, m_terrainRoughness;

	float m_enemySpeed = 0.2f;
	glm::vec3 m_enemyRange = glm::vec3(70.0f, 20.0f, 70.0f);
	glm::vec3 m_foodRange = glm::vec3(20.0f, 20.0f, 20.0f);
	glm::vec3 m_collectibleRange = glm::vec3(70.0f, 50.0f, 70.0f);

	bool m_inventoryFull = false;
	int m_repairedParts = 0;

	// Gravity Bullets
	std::vector<Sphere *> m_bullets;
	bool m_usingGravityGun = false;
	bool m_leftMouseDown = false;
	Model *m_gravityItem = nullptr;

	Common::StateMachine *m_stateMachine = nullptr;

	// Effects
	std::vector<Effect *> m_effects;
	std::string m_firepath = "data/effects/fire.pfx";
	std::string m_forcefieldPath = "data/effects/forcefield.pfx";
	std::string m_blueTrail = "data/effects/blue-trail.pfx";
	std::string m_fireSound = "data/sounds/fire-sound.ogg";
	Effect *m_blueTrailEffect = nullptr;
	Effect *m_gravityForcefield = nullptr;

	// For Minimap
	Triangle *m_playerIcon = nullptr;
	Quad *m_shipIcon = nullptr;
	std::string m_playerIconPath = "data/textures/hud/arrow.png";
	float m_miniWidth = 400.0f;
	float m_miniHeight = 300.0f;
	wolf::FrameBuffer *m_frameBuffer;
	OrthoCamera *m_miniCamera = nullptr;
	TextBox *m_characterBox = nullptr;
	Effect *m_scannerEffect = nullptr;

	// Model positioning (testing only)
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float xRot = 0.0f;
	float yRot = 0.0f;
	float zRot = 0.0f;
};

#endif
