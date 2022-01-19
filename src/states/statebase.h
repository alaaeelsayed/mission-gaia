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
		StateBase() : m_pStateMachine(NULL) {}
		virtual ~StateBase() {}
		virtual void Enter(std::string arg = "") = 0;
		virtual void Update(float p_fDelta) = 0;
		virtual void Exit() = 0;
		virtual void Render(const glm::mat4 mProj, const glm::mat4 mView, int width, int height) = 0;
		virtual void Reset() = 0;
		void RegisterApp(wolf::App *pApp) { m_pApp = pApp; };

	private:
		void SetStateMachineMembership(StateMachine *p_pStateMachine)
		{
			m_pStateMachine = p_pStateMachine;
		}

	protected:
		StateMachine *m_pStateMachine;
		wolf::App *m_pApp;
	};
}

#endif
