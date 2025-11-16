#ifndef STATEGAMEPLAY_H
#define STATEGAMEPLAY_H

#include "statebase.h"
#include "../camera/camera.h"
#include "../camera/groundedcamera.h"
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
#include "../obj/sphere.h"
#include "../particles/effect.h"

#include "../obj/triangle.h"
#include "../obj/quad.h"
#include "../obj/cube.h"

#include "../../irrklang/include/irrKlang.h"
#include <unordered_map>

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
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec3 attenuation;
		glm::vec3 dir = glm::vec3(0.0f, 0.0f, 0.0f);
		float cutOff = 0.0f;
		float coneAngle = 0.0f;
	};

	StateGameplay();
	virtual ~StateGameplay();

	// Overridden from StateBase
	virtual void Enter(std::string arg = "");
	virtual void Update(float dt);
	virtual void Render(const glm::mat4& mProj, const glm::mat4& mView);
	virtual void Exit();

private:
	void _updateCameraAndScreen(Camera* camera);
	void _updateTextboxes(Camera* camera, float width, float height);
	void _updateSpotlight(Camera* camera);
	void _handleFlashlightToggle();
	void _handleCinematicToggle();
	void _handleGravityGunToggle(Camera* camera);
	void _generateTerrain(int rangeStart, int rangeEnd);

	void _renderTerrain();
	void _renderMinimap();

	bool _inRange(int chunkX, int chunkZ);
	int _randomNum(int lowerBound, int upperBound);
	float _randomFloat(float lo, float hi);
	bool _isEffectiveLight(const Light* light1, const Light* light2, Model* model) const;

	const std::string m_skyboxPath = "data/textures/skybox/skybox.png";

	const std::string m_natureSoundPath = "data/sounds/ambient.ogg";
	const std::string m_waterSoundPath = "data/sounds/ocean.wav";
	const std::string m_creatureGrowlPath = "data/sounds/creature_growl.wav";
	const std::string m_flashlightSoundPath = "data/sounds/flashlight.ogg";
	const std::string m_gravityGunSoundPath = "data/sounds/gravity-gun.wav";
	const std::string m_gravityGunShutdownSoundPath = "data/sounds/gravity-gun-shutdown.wav";
	const std::string m_gravityGunFaultyPath = "data/sounds/gravity-gun-faulty.wav";

	const std::string m_runningSoundPath = "data/sounds/running.ogg";
	const std::string m_walkingSoundPath = "data/sounds/walking.ogg";

	std::vector<Light*> m_lights;
	wolf::Program* m_worldProgram = 0;
	wolf::Material* m_mat = nullptr;

	Model* m_flashlight = nullptr;
	Model* m_gravityGun = nullptr;
	Light* m_spotlight = nullptr;

	wolf::SoundManager* m_soundManager = nullptr;

	TerrainGenerator* m_terrainGenerator = 0;
	std::unordered_map<std::pair<int, int>, Terrain*> m_terrainMap;

	Skybox* m_skybox = 0;

	bool m_keyDown = false;
	bool m_gravityKeyDown = false;
	bool m_flashlightEquipped = false;
	bool m_gravityGunEquipped = false;

	Font* m_font;
	TextBox* m_hungerText;
	TextBox* m_thirstText;

	float m_health = 100;
	float m_hunger = 100;
	float m_thirst = 100;

	int m_lightCount = 0;

	// Debug Menu
	bool m_walking = true;
	bool m_running = true;

	float m_dispMult[4] = { 0.5f, 0.125f, 1.0f, 1.0f };

	int m_terrainSize, m_terrainVerts, m_terrainOctaves;
	float m_terrainAmplitude, m_terrainRoughness;

	float m_baseMult = 2.7f;
	float m_valleyMult = 3.5f, m_valleyThreshold = -0.325f, m_valleyDepth = 0.445f;
	float m_lowAreaSmoothFactor = 0.098f, m_lowAreaThreshold = 0.0;

	float m_smoothEdge0 = 0.4f, m_smoothEdge1 = 0.6f;

	// Gravity Bullets
	std::vector<Sphere*> m_bullets;
	bool m_leftMouseDown = false;

	Common::StateMachine* m_stateMachine = nullptr;

	// For Minimap
	Triangle* m_playerIcon = nullptr;

	std::string m_playerIconPath = "data/textures/hud/arrow.png";
	float m_miniWidth = 400.0f;
	float m_miniHeight = 300.0f;
	wolf::FrameBuffer* m_frameBuffer;
	OrthoCamera* m_miniCamera = nullptr;

	Cube* m_cube = nullptr;

	// Cinematic Mode
	bool m_bCinematic = false;
	bool m_bCinematicButtonDown = false;
};

#endif
