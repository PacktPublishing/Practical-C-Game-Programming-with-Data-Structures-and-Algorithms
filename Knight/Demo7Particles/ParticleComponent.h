#pragma once

#include <vector>
#include <algorithm>

#include "Knight.h"

// Struct to represent a particle
struct Particle {
    Vector3 position;
    Vector3 velocity;
    float life;      // Remaining life of the particle
    float maxLife;   // Total life duration of the particle
    Color color;     // Particle color
};

class ParticleComponent : public Component
{
    public:
        bool CreateFromFile(const char* path, int maxp, Vector3 v, Color ic = WHITE, Vector3 isp = {0,0,0});
        void Update(float deltaTime) override;
        void Draw(RenderHints* pRH = nullptr) override;

    private:
        int maxParticles = 500;
        Vector3 offset = Vector3{ 0,0,0 };
        Color initialColor = Color{255,255,255,255};
        Vector3 initialSpeed = Vector3{0,0,0};
        Texture2D texture = { 0 };
        std::vector<Particle> particles;

        virtual void EmitParticles(float deltaTime);
};

//End of ParticleSyste.h