//Finite State Machine implementation for character states
#include "FSM.h"
#include "Knight.h"

FSM::FSM(SceneActor* Character, ModelComponent* AnimController)
    : _States{ nullptr }
{
    _Character = Character;
    _AnimController = AnimController;
}

FSM::~FSM()
{
    for (int i = 0; i < 15; ++i)
    {
        if (_States[i] != nullptr)
        {
            delete _States[i];
        }
    }
}

void FSM::SetState(ECharacterState NewState)
{
    _PrevState = _CurrentState;
    _CurrentState = NewState;
    _States[_CurrentState]->Enter(this);
}

void FSM::Update(float DeltaTime)
{
    _States[_CurrentState]->Update(this, DeltaTime);
}


