#include "./statebase.h"
#include "./statemachine.h"
#include "assert.h"

using namespace Common;

StateMachine::StateMachine()
	: m_currentState(-1),
	  m_currentStateBase(NULL),
	  m_currentStateTime(0.0f),
	  m_owner(NULL)
{
}

StateMachine::~StateMachine()
{
}

void StateMachine::RegisterState(int state, StateBase *stateBase)
{
	stateBase->SetStateMachineMembership(this);
	m_stateMap.insert(std::pair<int, StateBase *>(state, stateBase));
}

void StateMachine::GoToState(int state, std::string arg)
{
	if (state == m_currentState)
	{
		return;
	}

	StateBase *stateBase = NULL;
	StateMap::iterator it = m_stateMap.find(state);
	if (it != m_stateMap.end())
	{
		stateBase = static_cast<StateBase *>(it->second);
	}
	assert(stateBase);

	if (m_currentStateBase)
	{
		m_currentStateBase->Exit();
	}

	if (!arg.empty())
		stateBase->Enter(arg);
	else
		stateBase->Enter();

	//if (p_iState == assignment4::ComponentAIController::eAIState_Chasing ||
	//	p_iState == assignment4::ComponentAIController::eAIState_FastChase) {
	//	const char* sTargetGUID = assignment4::AIDecisionNode::getTarget(); // Get guid
	//	pState->Enter(sTargetGUID);
	//}
	//else {
	//}

	m_currentState = state;
	m_currentStateBase = stateBase;
	m_currentStateTime = 0.0f;
}

void StateMachine::PushState(int state)
{
	assert(false); // NOT YET IMPLEMENTED
}

void StateMachine::PopState()
{
	assert(false); // NOT YET IMPLEMENTED
}

void StateMachine::Update(float dt)
{
	m_currentStateTime += dt;
	if (m_currentStateBase)
	{
		m_currentStateBase->Update(dt);
	}
}

void StateMachine::Render(const glm::mat4 &mProj, const glm::mat4 &mView)
{
	m_currentStateBase->Render(mProj, mView);
}