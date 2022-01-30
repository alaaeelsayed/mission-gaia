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

		void RegisterState(int state, StateBase *stateBase);
		void GoToState(int state, std::string arg = "");
		void PushState(int state);
		void PopState();

		void Update(float dt);
		void Render(const glm::mat4 &mProj, const glm::mat4 &mView);

		int GetCurrentState() { return m_currentState; }
		float GetCurrentStateTime() { return m_currentStateTime; }

		void SetStateMachineOwner(void *owner) { m_owner = owner; }
		void *GetStateMachineOwner() { return m_owner; }

	private:
		typedef std::map<int, StateBase *> StateMap;

		StateMap m_stateMap;

		int m_currentState;
		StateBase *m_currentStateBase;

		float m_currentStateTime;

		void *m_owner;
	};
}
#endif
