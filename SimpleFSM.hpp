#pragma once
/*
    Simple Finite State Machine by Manoloon
*/
#include <iostream>
#include <map>
#include <functional>

enum class Event
{
    StartMoving,
    StopMoving,
    TakeDamage,
    InCover,
    OutCover,
    Attack,
    StartHeal,
    StopHeal,
    CheckHeal
};

enum class State
{
    Idle,
    Patrol,
    Attack,
    Cover,
    Heal,
    Die
};

struct Transition
{
    State nextState;
    std::function<void()> action;
};

struct StateEventComparator 
{
    bool operator()(const std::pair<State,Event>& lhs, const std::pair<State,Event>& rhs) const
    {
        return lhs.second < rhs.second;
    }
};

class SSFM
{
    State currentState;
    std::map<std::pair<State,Event>,Transition,StateEventComparator> transitionMap;

    public:
    SSFM() : currentState(State::Idle){}

    void addTransition(State fromState, Event event, State toState, 
                        std::function<void()> action = nullptr)
                        {
                            transitionMap[{fromState,event}] = {toState,action};
                        }
                        
    void handleEvent(Event event)
    {
        auto it = transitionMap.find({currentState,event});
        if(it != transitionMap.end())
        {
            const Transition& transition = it->second;
            if(transition.action)
            {
                transition.action();
            }

            currentState = transition.nextState;
            std::cout << "transition to state: " << static_cast<int>(currentState) << '\n';
        }
        else 
        {
            std::cout << "no transition defined \n" ;
        }
    }

    State getCurrentState() const 
    {
        return currentState;
    }
};