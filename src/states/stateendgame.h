#ifndef STATEENDGAME_H
#define STATEENDGAME_H

#include "statebase.h"
#include "../camera/freeroamcamera.h"
#include "../camera/camera.h"
#include "../obj/skybox.h"
#include "../obj/skybox.h"
#include "../obj/model.h"
#include "../text/textbox.h"
#include "../../wolf/wolf.h"
#include "../scene/scene.h"

class StateEndGame : public Common::StateBase
{
public:
	//------------------------------------------------------------------------------
	// Public methods.
	//------------------------------------------------------------------------------
	StateEndGame();
	virtual ~StateEndGame();

	// Overridden from StateBase
	virtual void Enter(std::string arg = "");
	virtual void Update(float dt);
	virtual void Render(const glm::mat4 &mProj, const glm::mat4 &mView);
	virtual void Exit();

private:
	int _randomNum(int lowerBound, int upperBound);

	const std::string m_groundTexPath = "data/textures/ground/ground.png";
	const std::string m_skyboxPath = "data/textures/skybox/skybox.png";

	// Sounds
	wolf::SoundManager *m_soundManager = nullptr;

	const std::string m_launchSoundPath = "data/sounds/launch.wav";

	wolf::Program *m_worldProgram = 0;
	glm::vec3 m_sunDirection = glm::vec3(10.0f, 10.0f, 20.0f);

	Skybox *m_skybox = 0;

	Font *m_font;
	TextBox *m_gameFinished = nullptr;
	TextBox *m_quitGame = nullptr;

	wolf::FrameBuffer *m_frameBuffer;
};

#endif