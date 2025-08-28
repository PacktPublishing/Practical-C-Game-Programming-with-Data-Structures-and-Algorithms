#include "MagicAttackEffect.h"

MagicAttackEffect::MagicAttackEffect()
{
    renderQueue = Component::eRenderQueueType::AlphaBlend;
    blendingMode = BLEND_ADDITIVE;
    EnableAlphaTest = false;
}

void MagicAttackEffect::Update(float deltaTime)
{  
    if (currentDelayTime > 0.0f) {
        currentDelayTime -= deltaTime;
        printf("delayed rest = %f!\n", currentDelayTime);
        return; // Wait until delay is over
    }

    for (auto& particle : particles) {
        // Update particle position based on velocity
        particle.position.x += particle.velocity.x * deltaTime;
        particle.position.y += particle.velocity.y * deltaTime;
        particle.position.z += particle.velocity.z * deltaTime;

        // Decrease particle life
        particle.life -= deltaTime;
        float lifeRatio = particle.life / particle.maxLife;
		float ageRatio = 1.0f - lifeRatio;

        //particle.velocity.x += ageRatio * 0.5f;
		//particle.velocity.z += ageRatio * 0.5f;

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
        EmitParticles(deltaTime);
    }
}

void MagicAttackEffect::Draw(RenderHints* pRH)
{
    if (currentDelayTime > 0.0f) {
        return; // Wait until delay is over
    }

    Camera3D* pCam = NULL;
    SceneCamera* pMainCam = _SceneActor->GetMainCamera();
    if (pMainCam != NULL)
        pCam = pMainCam->GetCamera3D();

    if (pCam != NULL) {
        for (const auto& particle : particles) {
            float size = 1.0f; // Size of each particle
            DrawBillboard(*pCam, texture, particle.position, size, particle.color);
        }
    }
}

void MagicAttackEffect::Reset()
{
    delayStart = 0.25f;
    __super::Reset();
    isEnabled = true; // Enable the effect when reset
}


void MagicAttackEffect::EmitParticles(float deltaTime)
{
    int particlesToEmit = 1; // Emit rate

    Vector3 origin = Vector3Add(_SceneActor->Position, offset);

    float ry = _SceneActor->Rotation.y;

    for (int i = 0; i < particlesToEmit && particles.size() < maxParticles; i++) {
        Particle particle = { 0 };
        particle.position = origin;

        // Random velocity with some upward direction
		particle.velocity.x = sinf(DegreesToRadians(ry)) * 15.0f;
		particle.velocity.z = cosf(DegreesToRadians(ry)) * 15.0f;
		particle.velocity.y = 0; 
        Vector3Add(particle.velocity, initialSpeed);

        particle.life = particle.maxLife = 2.0f + float(rand()) / RAND_MAX * 2.0f; // Life in seconds
        particle.color = initialColor; // Initial color

        particles.push_back(particle);
    }
}

//End of MagicAttackEffect.cpp