#include <iostream>
#include <vector>
#include <functional>

/*
    An event manager with Variadic template functions
*/

namespace EventManager
{
    template<typename...Args>
    using EventListener = std::function<void(Args...)>;

    template<typename...Args>
    std::vector<EventListener<Args...>>& GetListeners()
    {
        static std::vector<EventListener<Args...>> Listeners;
        return Listeners;
    }

    template<typename...Args>
    void RegisterListener(EventListener<Args...> Listener)
    {
        GetListeners<Args...>().emplace_back(Listener);
    }

    template<typename...Args>
    void DispatchEvent(Args... args)
    {
        for(const auto& Listener : GetListeners<Args...>())
        {
            Listener(args...);
        }
    }
}