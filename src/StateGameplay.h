#ifndef STATEGAMEPLAY_H
#define STATEGAMEPLAY_H

#include "./StateBase.h"
#include "../samplefw/FreeRoamCamera.h"
#include "../samplefw/Skybox.h"
#include "../samplefw/plane.h"
#include "./Model.h"
#include "../samplefw/Camera.h"
#include "../samplefw/Skybox.h"
#include "text/textbox.h"

class StateGameplay : public Common::StateBase
{
public:
	//------------------------------------------------------------------------------
	// Public methods.
	//------------------------------------------------------------------------------
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

	Camera *m_pCam = 0;
	const std::string m_groundTexPath = "data/textures/ground/ground.png";
	const std::string m_skyboxPath = "data/textures/skybox/skybox.png";

	wolf::Program *m_pWorldProgram = 0;
	std::vector<Model *> m_lModels;
	std::vector<glm::vec3> m_lPositions;
	wolf::Material *m_pMat = nullptr;

	wolf::Texture *m_pCreatureTex = nullptr;

	wolf::Model *m_pShipModel = nullptr;
	wolf::Texture *m_pShipTex = nullptr;

	Model *m_pFlashlight = nullptr;

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
};

#endif
