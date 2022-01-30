#ifndef StateMainMenu_H
#define StateMainMenu_H

#include "statebase.h"
#include "../camera/freeroamcamera.h"
#include "../camera/camera.h"
#include "../obj/skybox.h"
#include "../obj/skybox.h"
#include "../obj/model.h"
#include "../text/textbox.h"
#include "../../wolf/wolf.h"

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
	virtual void Reset();
	void RegisterCamera(Camera *pCam);

private:
	int _randomNum(int lowerBound, int upperBound);

	Camera *m_pCam = 0;
	const std::string m_skyboxPath = "data/textures/skybox/skybox.png";

	Skybox *m_pSkybox = 0;
	wolf::Program *m_pWorldProgram = 0;
	glm::vec3 m_sunDirection = glm::vec3(10.0f, 10.0f, 20.0f);

	Font *m_font;
	TextBox *m_pProjectGaia = nullptr;
	TextBox *m_pStartGame = nullptr;
	TextBox *m_pQuitGame = nullptr;
	TextBox *m_pLoading = nullptr;

	bool m_bIsLoading = false;

	wolf::FrameBuffer *m_frameBuffer;
};

#endif
