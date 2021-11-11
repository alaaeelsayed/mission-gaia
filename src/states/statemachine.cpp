#include "./statebase.h"
#include "./statemachine.h"
#include "assert.h"

using namespace Common;

StateMachine::StateMachine()
	: m_iCurrentState(-1),
	  m_pCurrentState(NULL),
	  m_fCurrentStateTime(0.0f),
	  m_pOwner(NULL)
{
}

StateMachine::~StateMachine()
{
}

void StateMachine::RegisterState(int p_iState, StateBase *p_pstate)
{
	p_pstate->SetStateMachineMembership(this);
	m_mStateMap.insert(std::pair<int, StateBase *>(p_iState, p_pstate));
}

void StateMachine::GoToState(int p_iState, std::string arg)
{
	if (p_iState == m_iCurrentState)
	{
		return;
	}

	StateBase *pState = NULL;
	StateMap::iterator it = m_mStateMap.find(p_iState);
	if (it != m_mStateMap.end())
	{
		pState = static_cast<StateBase *>(it->second);
	}
	assert(pState);

	printf("GOING TO %s", arg);
	if (m_pCurrentState)
	{
		m_pCurrentState->Exit();
	}

	if (!arg.empty())
		pState->Enter(arg);
	else
		pState->Enter();

	//if (p_iState == assignment4::ComponentAIController::eAIState_Chasing ||
	//	p_iState == assignment4::ComponentAIController::eAIState_FastChase) {
	//	const char* sTargetGUID = assignment4::AIDecisionNode::getTarget(); // Get guid
	//	pState->Enter(sTargetGUID);
	//}
	//else {
	//}

	m_iCurrentState = p_iState;
	m_pCurrentState = pState;
	m_fCurrentStateTime = 0.0f;
}

void StateMachine::PushState(int p_iState)
{
	assert(false); // NOT YET IMPLEMENTED
}

void StateMachine::PopState()
{
	assert(false); // NOT YET IMPLEMENTED
}

void StateMachine::Update(float p_fDelta)
{
	m_fCurrentStateTime += p_fDelta;
	if (m_pCurrentState)
	{
		m_pCurrentState->Update(p_fDelta);
	}
}

void StateMachine::Render(const glm::mat4 mProj, const glm::mat4 mView, int width, int height)
{
	m_pCurrentState->Render(mProj, mView, width, height);
}