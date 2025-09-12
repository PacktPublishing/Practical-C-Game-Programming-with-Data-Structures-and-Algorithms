#pragma once

#include <vector>

#include "ParticleComponent.h"


class MagicAttackEffect : public ParticleComponent
{
public:
	bool isEnabled = false;
    Vector3 ForwardDirection = Vector3{ 0,0,1 };
	float CurrentLifeTime = 1.0f; 
	float MaxLifeTime = 1.0f; // Total lifetime of the effect in seconds

    MagicAttackEffect();

    void Update(float ElapsedSeconds, RenderHints* pRH = nullptr) override;
    void Draw(RenderHints* pRH = nullptr) override;

    void Reset() override;

    void EmitParticles(float deltaTime) override;
};

//end of MagAttackEffect.h
