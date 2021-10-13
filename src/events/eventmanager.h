#pragma once
#include <queue>
#include <vector>
#include <map>
#include <functional>

namespace Common
{
    class EventManager
    {
    public:
        struct Event
        {
            int m_iEventId;

            Event(int id) : m_iEventId(id) {}
        };
        typedef std::function<void(const Event *)> Listener;

        static void CreateInstance();
        static void DestroyInstance();
        static EventManager *Instance();

        void QueueEvent(const Event *eventData);
        void TriggerEvent(const Event *eventData);
        void Update(float p_fDelta);
        void AddListener(int p_eventId, Listener p_listener);
        void RemoveListener(int p_eventId, Listener p_listener);

    private:
        EventManager();
        ~EventManager();

        static EventManager *s_pEventManagerInstance;

        std::map<int, std::vector<Listener>> m_listeners;
        std::queue<const Event *> m_eventQueue;
    };
}