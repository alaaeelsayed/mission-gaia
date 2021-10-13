#ifndef STATEMAINMENU_H
#define STATEMAINMENU_H

#include "./StateBase.h"
#include "../samplefw/FreeRoamCamera.h"
#include "../samplefw/Skybox.h"
#include "../samplefw/plane.h"
#include "./Model.h"
#include "../samplefw/Camera.h"
#include "../samplefw/Skybox.h"

class StateMainMenu : public Common::StateBase
{
public:
	//------------------------------------------------------------------------------
	// Public methods.
	//------------------------------------------------------------------------------
	StateMainMenu();
	virtual ~StateMainMenu();

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
	glm::vec3 m_sunDirection = glm::vec3(10.0f, 10.0f, 20.0f);

	Plane *m_pPlane = 0;
	Skybox *m_pSkybox = 0;
};

#endif
