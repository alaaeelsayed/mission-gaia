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
		bool bEnabled = true;
		glm::vec4 vPosRange;
		glm::vec3 vColor;
		glm::vec3 vAttenuation;
		glm::vec4 vLightSpot = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 vLightDir = glm::vec3(0.0f, 0.0f, 0.0f);
	};

	StateGameplay();
	virtual ~StateGameplay();

	// Overridden from StateBase
	virtual void Enter(std::string arg = "");
	virtual void Update(float p_fDelta);
	virtual void Exit();
	virtual void Render(const glm::mat4 mProj, const glm::mat4 mView, int width, int height);
	void RegisterCamera(Camera *pCam);
	void RegisterMachine(Common::StateMachine *pMachine);
	virtual void Reset();

private:
	void _renderTerrain();
	int _randomNum(int lowerBound, int upperBound);
	float _randomFloat(float lo, float hi);

	bool _isEffectiveLight(const Light *pLight1, const Light *pLight2, Model *pModel) const;

	Camera *m_pCam = 0;
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

	const std::string m_drinkPrompt = "Press E to drink";
	const std::string m_eatPrompt = "Press E to eat";

	std::vector<Light *> m_vLights;
	wolf::Program *m_pWorldProgram = 0;
	std::vector<Model *> m_lModels;
	std::vector<glm::vec3> m_lPositions;
	wolf::Material *m_pMat = nullptr;

	wolf::Texture *m_pCreatureTex = nullptr;

	wolf::Model *m_pShipModel = nullptr;
	wolf::Texture *m_pShipTex = nullptr;

	Model *m_pFlashlight = nullptr;
	Light *m_pSpotlight = nullptr;

	wolf::SoundManager *m_pSoundManager = nullptr;

	const int m_gridSize = 20;

	float m_currShininess = 200.0f;

	float m_sunSpeed = 0.01f;
	glm::vec3 m_sunDirection = glm::vec3(10.0f, 10.0f, 30.0f);

	GLfloat m_sunAngle = -60;

	TerrainGenerator *m_pTerrainGenerator = 0;
	std::vector<Terrain *> m_lTerrains;

	Skybox *m_pSkybox = 0;

	bool m_bKeyDown = false;
	bool m_bFlashlightEquipped = false;
	bool m_bDrinking = false;
	bool m_bEating = false;

	Font *m_font;
	TextBox *m_hungerText;
	TextBox *m_thirstText;
	TextBox *m_drinkText;
	TextBox *m_eatText;

	bool m_bNearWater = false;
	bool m_bNearFood = false;
	float m_fHunger = 100;
	float m_fThirst = 100;
	int m_iLights = 0;

	Plane *m_pPlane = nullptr;

	// Water
	Water *m_pWater = nullptr;

	// For Flashlight Positioning (should be changed)
	glm::vec3 m_vPrevCamRot;

	// Debug Menu
	bool m_bDebug = true;
	bool m_bWalking = false;
	bool m_bRunning = false;

	int m_terrainSize, m_terrainVerts, m_terrainOctaves;
	float m_terrainAmplitude, m_terrainRoughness;

	float m_fEnemySpeed = 0.003f;
	glm::vec3 m_vEnemyRange = glm::vec3(70.0f, 20.0f, 70.0f);
	glm::vec3 m_vFoodRange = glm::vec3(20.0f, 20.0f, 20.0f);

	Common::StateMachine *m_pStateMachine = nullptr;
};

#endif
