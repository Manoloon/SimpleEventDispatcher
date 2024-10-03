#include <memory>
#include <unordered_map>
#include <iostream>
#include <string_view>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <functional>
#include <execution>
#include <ctime>

using namespace std::chrono_literals;
using EventCallback = std::function<void(int)>;

class EventDispatcher
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

void OnPlayerJoined(int playerId)
{
    std::cout << "Player Joined : " << playerId << std::endl;
}

void OnPlayerLeft(int playerId)
{
    std::cout << "Player Left :" << playerId << std::endl;
}

class Player
{
    static int count;
    int id = 0;
public:
    explicit Player(EventDispatcher* Dispatcher):id(++count)
    {
        if(Dispatcher == nullptr)
        {
            throw std::invalid_argument("eventDispatcher is null");
        }
        Dispatcher->Register("Joined",[this](int playerId)
        {
            if(playerId == id)
            {
                OnPlayerJoined(playerId);
            }
        });
        Dispatcher->Register("Left",[this](int playerId)
        {
            if(playerId == id)
            {
            OnPlayerLeft(playerId);
            std::cout << "I DIE :" << id << std::endl;
            }
        });
    };
    int GetId() const {return id;}

    void Print()const {std::cout << "I am :" << id << std::endl;}
};
int Player::count = 0;

int main()
{
    EventDispatcher Dispatcher;
    Player playerOne(&Dispatcher);
    Dispatcher.DispatchEvent("Joined",playerOne.GetId());
    Player playerTwo(&Dispatcher);
    Dispatcher.DispatchEvent("Joined",playerTwo.GetId());
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::time_t currentTimeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::ctime(&currentTimeT) << std::endl;
    Dispatcher.DispatchEvent("Left",playerOne.GetId());
    std::this_thread::sleep_for(std::chrono::seconds(2));
    currentTimeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::ctime(&currentTimeT) << std::endl;
    Dispatcher.DispatchEvent("Left",playerTwo.GetId());
    return 0;
}