#pragma once

#include "FSM.h"
#include "MagicAttackEffect.h"

class PlayerFSM : public FSM
{
public:
    PlayerFSM(SceneActor* Character, ModelComponent* AnimController);
};


class PlayerIdleState : public FSMState
{
public:
    void Enter(FSM* FiniteStateMachine) override;
    void Update(FSM* FiniteStateMachine, float DeltaTime) override;
};

class PlayerWalkState : public FSMState
{
public:
    void Enter(FSM* FiniteStateMachine) override;
    void Update(FSM* FiniteStateMachine, float DeltaTime) override;
};

class PlayerAttackState : public FSMState
{
private:
	MagicAttackEffect* pAttackEffect = nullptr;
public:
    void Enter(FSM* FiniteStateMachine) override;
    void Update(FSM* FiniteStateMachine, float DeltaTime) override;
};