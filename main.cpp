#include <memory>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <ctime>
#include <string>
#include <vector>
#include "EventManager.hpp"

void OnPlayerJoined(int playerId)
{
    std::cout << "Player Joined : " << playerId << '\n';
}

void OnPlayerLeft(int playerId)
{
    std::cout << "Player Left :" << playerId << '\n';
}
template<typename T>
void OnReceivedData_TwoParam(T )
{
    std::cout << __PRETTY_FUNCTION__ << '\n';
}

class Player
{
    static int count;
    int id = 0;
    const std::vector<float> vec = {12.f,2.3f,4.3f,5.8f};
    SimpleEventManager::Event<int,std::vector<float>,std::string_view> event_threeParam;

public:
Player():id(++count)
{
    event_threeParam.RegisterListener([](int a, const std::vector<float>& vec,std::string_view Name)
    {
        std::cout << "Player: " << a << '\n';
        for(const auto& e : vec)
        {
            std::cout << e << ", ";
        }
        std::cout << Name <<'\n';
    });
}
    explicit Player(ClassBasedEventManager* Dispatcher):id(++count)
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
    void ShootEvent()
    {
        event_threeParam.DispatchEvent(id,vec,"Somos tres");
    }
};
int Player::count = 0;

int main()
{
    ClassBasedEventManager ClassEventManager;
    Player playerOne(&ClassEventManager);
    ClassEventManager.DispatchEvent("Joined",playerOne.GetId());
    Player playerTwo(&ClassEventManager);
    ClassEventManager.DispatchEvent("Joined",playerTwo.GetId());
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::time_t currentTimeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::ctime(&currentTimeT) << std::endl;
    ClassEventManager.DispatchEvent("Left",playerOne.GetId());
    std::this_thread::sleep_for(std::chrono::seconds(2));
    currentTimeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::ctime(&currentTimeT) << std::endl;
    ClassEventManager.DispatchEvent("Left",playerTwo.GetId());
    Player playerThree;
    playerThree.ShootEvent();
    
    // Testing EventManager variadic
    SimpleEventManager::Event<int,std::string> event_twoParams;
    event_twoParams.RegisterListener([](int a,const std::string& b)
    {
        std::cout << "Listener 1:" << a << "," << b << '\n';
        std::cout << __PRETTY_FUNCTION__  << '\n';
    });
    event_twoParams.DispatchEvent(42,"Hey");

    return 0;
}