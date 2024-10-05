#include <string_view>
#include <unordered_map>
#include <execution>
#include <functional>

/*
    Simple event manager with Flexible event signatures
*/

namespace SimpleEventManager
{
    template<typename...Args>
    using EventListener = std::function<void(Args...)>;
    
    template<typename...Args>
    class Event
    {
    public:
    template<typename Callable>
    void RegisterListener(Callable&& Listener)
    {
        Listeners.emplace_back(EventListener<Args...>(std::forward<Callable>(Listener)));
    }

    void DispatchEvent(Args... args)
    {
        for(const auto& Listener : Listeners)
        {
            Listener(args...);
        }
    }
    private:
        std::vector<EventListener<Args...>> Listeners;
    };
}

/*
    Class Based EventManager
*/
using EventCallback = std::function<void(int)>;

class ClassBasedEventManager
{
	std::unordered_map<std::string_view,std::vector<EventCallback>> Listeners;
public:
    void Register(std::string_view EventName, EventCallback Callback)
    {
        Listeners[EventName].emplace_back(Callback);
    }
    void DispatchEvent(std::string_view EventName,int EventData)
    {
        if(Listeners.find(EventName) == Listeners.end()) return;
        std::for_each(std::execution::par,Listeners[EventName].begin(),
                                            Listeners[EventName].end(),
                                            [&](const auto& l)
        {
            l(EventData);
        });
    }
};