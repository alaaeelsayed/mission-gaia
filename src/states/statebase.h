//------------------------------------------------------------------------
// StateBase
//
// Created:	3/9/2021
// Author:	Hesham El Hamshary
//
// Base class for any states driven by StateMachine.
//------------------------------------------------------------------------

#ifndef STATEBASE_H
#define STATEBASE_H

#include "../../wolf/wolf.h"
#include "./statemachine.h"
#include <string>

namespace Common
{
	class StateBase
	{
		friend class StateMachine;

	public:
		StateBase() : m_stateMachine(NULL) {}
		virtual ~StateBase() {}
		virtual void Enter(std::string arg = "") = 0;
		virtual void Update(float dt) = 0;
		virtual void Render(const glm::mat4 &mProj, const glm::mat4 &mView) = 0;
		virtual void Exit() = 0;
		void RegisterApp(wolf::App *app) { m_app = app; };

	private:
		void SetStateMachineMembership(StateMachine *stateMachine)
		{
			m_stateMachine = stateMachine;
		}

	protected:
		StateMachine *m_stateMachine;
		wolf::App *m_app;
	};
}

#endif
