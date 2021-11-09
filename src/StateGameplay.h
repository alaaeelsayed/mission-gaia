#ifndef STATEGAMEPLAY_H
#define STATEGAMEPLAY_H

#include "./StateBase.h"
#include "../samplefw/FreeRoamCamera.h"
#include "../samplefw/Skybox.h"
#include "../samplefw/plane.h"
#include "../samplefw/Camera.h"
#include "../samplefw/Skybox.h"
#include "text/textbox.h"

// #include <irrKlang.h>
// #pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

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

private:
	int _randomNum(int lowerBound, int upperBound);
	float _randomFloat(float lo, float hi);

	bool _isEffectiveLight(const Light *pLight1, const Light *pLight2, Model *pModel) const;

	Camera *m_pCam = 0;
	const std::string m_groundTexPath = "data/textures/ground/ground.png";
	const std::string m_skyboxPath = "data/textures/skybox/skybox.png";

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

	// irrklang::ISoundEngine *m_pSoundEngine = nullptr;

	const int m_gridSize = 20;

	float m_currShininess = 200.0f;

	float m_sunSpeed = 0.01f;
	glm::vec3 m_sunDirection = glm::vec3(10.0f, 10.0f, 30.0f);

	GLfloat m_sunAngle = -60;

	Plane *m_pPlane = 0;
	Skybox *m_pSkybox = 0;

	bool m_bKeyDown = false;
	bool m_bFlashlightEquipped = false;

	Font *m_font;
	TextBox *m_hungerText;
	TextBox *m_thirstText;

	float m_fHunger = 100;
	float m_fThirst = 100;
	int m_iLights = 0;
};

#endif
