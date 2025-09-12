//Magic attack effect with particle system
#include "MagicAttackEffect.h"

MagicAttackEffect::MagicAttackEffect()
{
    renderQueue = Component::eRenderQueueType::AlphaBlend;
    blendingMode = BLEND_ADDITIVE;
    EnableAlphaTest = false;
}

void MagicAttackEffect::Update(float ElapsedSeconds, RenderHints* pRH)
{  
    if (isEnabled == false)
        return;

    CurrentLifeTime -= ElapsedSeconds;
    if (CurrentLifeTime < 0.0f) {
        isEnabled = false; // Disable the effect after its lifetime
		particles.clear(); // Clear existing particles
		GetSceneActor()->Position = Vector3{ 0, 1.0f, 0 }; 
        return;
	}

    if (currentDelayTime > 0.0f) {
        currentDelayTime -= ElapsedSeconds;
        return; // Wait until delay is over
    }

    GetSceneActor()->Position.x += ForwardDirection.x * ElapsedSeconds * 100.0f;
    GetSceneActor()->Position.z += ForwardDirection.z * ElapsedSeconds * 100.0f;

    for (auto& particle : particles) {
        // Update particle position based on velocity
        particle.position.x += particle.velocity.x * ElapsedSeconds;
        particle.position.y += particle.velocity.y * ElapsedSeconds;
        particle.position.z += particle.velocity.z * ElapsedSeconds;

        // Decrease particle life
        particle.life -= ElapsedSeconds;
        float lifeRatio = particle.life / particle.maxLife;
		float ageRatio = 1.0f - lifeRatio;

        particle.scale = 1.0;

        // Fade out as life decreases
        particle.color.a = static_cast<unsigned char>(255 * lifeRatio);
        particle.color.r = static_cast<unsigned char>(initialColor.r * lifeRatio);
        particle.color.g = static_cast<unsigned char>(initialColor.g * lifeRatio);
        particle.color.b = static_cast<unsigned char>(initialColor.b * lifeRatio);
    }

    // Remove dead particles
    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](const Particle& p) { return p.life <= 0; }),
        particles.end());

    // Emit new particles
    if (isEnabled) {
        EmitParticles(ElapsedSeconds);
    }
}

void MagicAttackEffect::Draw(RenderHints* pRH)
{
    if (isEnabled == false)
        return;

    if (currentDelayTime > 0.0f) {
        return; // Wait until delay is over
    }

	__super::Draw(pRH);
}

void MagicAttackEffect::Reset()
{
	CurrentLifeTime = MaxLifeTime;
    delayStart = 0.25f;
    __super::Reset();
    isEnabled = true; // Enable the effect when reset

    ForwardDirection.x = sinf(DegreesToRadians(_SceneActor->Rotation.y));
	ForwardDirection.y = 0.0f;
    ForwardDirection.z = cosf(DegreesToRadians(_SceneActor->Rotation.y));
	ForwardDirection = Vector3Normalize(ForwardDirection);
}


void MagicAttackEffect::EmitParticles(float deltaTime)
{
    if (isEnabled == false)
		return;

    int particlesToEmit = 1; // Emit rate

    Vector3 origin = offset;

    float ry = _SceneActor->Rotation.y;

    for (int i = 0; i < particlesToEmit && particles.size() < maxParticles; i++) {
        Particle particle = { 0 };
        particle.position = origin;
        particle.scale = 1.0f;

        // Random velocity with some upward direction
		particle.velocity.x = -sinf(DegreesToRadians(ry)) * 5.0f;
		particle.velocity.z = -cosf(DegreesToRadians(ry)) * 5.0f;
		particle.velocity.y = 1; 
        Vector3Add(particle.velocity, initialSpeed);

        particle.life = particle.maxLife = 2.0f + float(rand()) / RAND_MAX * 2.0f; // Life in seconds
        particle.color = initialColor; // Initial color

        particles.push_back(particle);
    }
}

//End of MagicAttackEffect.cpp