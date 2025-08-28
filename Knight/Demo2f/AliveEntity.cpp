#pragma once

#include "Demo2f.h"

//Constructor of base class AliveEntity
//All alive entities should have HP, a target to attack and a cooldown time for the next attack
AliveEntity::AliveEntity()
{
	HP = 100;
	_target = NULL;
	attackCooldown = GetCooldownTime();
	rechargeTime = 0;
}

//Execute an attack on the target
void AliveEntity::DoAttack()
{
	char buf[80];

	int dmg = ((rand() % 2) + 1) * 10;

	sprintf_s(buf, "%s %s attack!", Actor->GetName(), (dmg == 10) ? "minor" : "heavy");
	Demo2f::Log(buf);

	_target->AddAction(ActionId::ApplyDamage, dmg);

	//find recharge time for next attack and reset the rechargeTime time to zero
	attackCooldown = GetCooldownTime();
	rechargeTime = 0.0f;
}

//Calculate the damage and trigger the death of the entity if HP is less than zero
void AliveEntity::DealDamage(int originalValue)
{
	char buf[80];

	int dmgVal = originalValue;
	if (HP < 50)
		dmgVal = (originalValue * 13 / 10);
	HP -= dmgVal;

	sprintf_s(buf, "%s receive damage:%d HP:%d", Actor->GetName(), dmgVal, HP);
	Demo2f::Log(buf);

	if (HP < 0)
		AddAction(ActionId::LostLife, 0);
}

void AliveEntity::Die()
{
	HP = 0;
	Actor->IsActive = false;
}

void AliveEntity::Update(float elaspedTime)
{
	//retrieve action events one by one and process them all.
	while (!eventsQueue.empty())
	{
		ActionEvent* currentEvent = eventsQueue.front();
		eventsQueue.pop();
		switch (currentEvent->Id)
		{
		case ApplyDamage:
			DealDamage(currentEvent->Value);
			break;
		case LostLife:
			Die();
		}
	}

	__super::Update(elaspedTime);
}

void AliveEntity::SetTarget(AliveEntity* target)
{
	_target = target;
}


void AliveEntity::AddAction(ActionId id, int value)
{
	eventsQueue.push(new ActionEvent{ id, value });
}

float AliveEntity::GetCooldownTime()
{
	return (float)(rand() % 20) * 0.1f + 1.0f;
}