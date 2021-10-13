#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <map>
#include <string>

namespace Common
{
	class StateBase;
	class StateMachine
	{
	public:
		StateMachine();
		~StateMachine();

		void RegisterState(int p_iState, StateBase *p_pstate);
		void GoToState(int p_iState, std::string arg = "");
		void PushState(int p_iState);
		void PopState();
		void Update(float p_fDelta);
		void Render(const glm::mat4 mProj, const glm::mat4 mView, int width, int height);

		int GetCurrentState() { return m_iCurrentState; }
		float GetCurrentStateTime() { return m_fCurrentStateTime; }

		void SetStateMachineOwner(void *p_pOwner) { m_pOwner = p_pOwner; }
		void *GetStateMachineOwner() { return m_pOwner; }

	private:
		typedef std::map<int, StateBase *> StateMap;

		StateMap m_mStateMap;

		int m_iCurrentState;
		StateBase *m_pCurrentState;

		float m_fCurrentStateTime;

		void *m_pOwner;
	};
}
#endif
