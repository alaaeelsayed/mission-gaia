#include "EventManager.h"

using namespace Common;

EventManager *EventManager::s_pEventManagerInstance = NULL;

void EventManager::CreateInstance()
{
    s_pEventManagerInstance = new EventManager();
}

void EventManager::DestroyInstance()
{
    delete s_pEventManagerInstance;
    s_pEventManagerInstance = NULL;
}

EventManager *EventManager::Instance()
{
    return s_pEventManagerInstance;
}

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

void Common::EventManager::QueueEvent(const Event *eventData)
{
    m_eventQueue.push(eventData);
}

void Common::EventManager::TriggerEvent(const Event *eventData)
{
    for (Listener listener : m_listeners[eventData->m_iEventId])
    {
        listener(eventData);
    }
    delete eventData;
}

void Common::EventManager::Update(float p_fDelta)
{
    while (!m_eventQueue.empty())
    {
        const Event *event = m_eventQueue.front();
        m_eventQueue.pop();
        TriggerEvent(event);
    }
}

void Common::EventManager::AddListener(int p_eventId, Listener p_listener)
{
    m_listeners[p_eventId].push_back(p_listener);
}

void Common::EventManager::RemoveListener(int p_eventId, Listener p_listener)
{
    m_listeners.erase(p_eventId);
    // Not sure how to do this
    //std::vector<Listener> listeners = m_listeners[p_eventId];
    //listeners.erase(std::remove(listeners.begin(), listeners.end(), p_listener), listeners.end());
}