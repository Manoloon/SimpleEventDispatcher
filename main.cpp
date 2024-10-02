#include <memory>
#include <unordered_map>
#include <iostream>
#include <string_view>
#include <vector>
#include <functional>

using EventCallback = std::function<void()>;

class EventDispatcher
{
	std::unordered_map<std::string_view,std::vector<EventCallback>> Listeners;
public:
    void Register(std::string_view EventName, EventCallback Callback)
    {
        Listeners[EventName].emplace_back(Callback);
    }
    void DispatchEvent(std::string_view EvetName)
    {
        if(Listeners.find(EvetName) == Listeners.end()) return;
        for(const auto& l : Listeners[EvetName])
        {
            l();
        }
    }
};

void EventOne()
{
    std::cout << "Event One Launch" << std::endl;
}

void EventTwo()
{
    std::cout << "Event Two Launch" << std::endl;
}

int main()
{
    EventDispatcher Dispatcher;
    std::string_view EVOne = "EV_One";
    std::string_view EVTwo = "EV_Two";
    Dispatcher.Register(EVOne,EventOne);
    Dispatcher.Register(EVTwo,EventTwo);

    Dispatcher.DispatchEvent(EVTwo);
    Dispatcher.DispatchEvent(EVOne);
    Dispatcher.DispatchEvent(EVOne);
    Dispatcher.DispatchEvent(EVOne);
    Dispatcher.DispatchEvent(EVTwo);
    return 0;
}