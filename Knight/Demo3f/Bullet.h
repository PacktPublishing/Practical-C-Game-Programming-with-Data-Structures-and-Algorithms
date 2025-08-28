#pragma once

#include "ObjectPool.h"

#define BULLET_LIFETIME 3.0f

class Bullet : public PoolableObject
{
private:
    Vector3 _position;
    Vector3 _velocity;
    float _radius;

public:

    Bullet()
        : _position{ 0.0f, 0.0f, 0.0f }
        , _velocity{ 0.0f, 0.0f, 0.0f }
        , _radius(1.0f)
    {
        _lifespan = BULLET_LIFETIME;
    }

    /* Function SetPosition
    *  Locate the bullet at the provided position and set its moving velocity
    * Parameters:
    *       Positions: The start position for the bullet
    *       Velocity: The velocity will be used to move the bullet
    */
    void SetPosition(Vector3 Position, Vector3 Velocity)
    {
        _position = Position;
        _velocity = Velocity;
    }

    /* Function Update
    *       Reduce the lifespan and move the bullet     
    *  Parameter:
    *       ElapsedTime: The interval time since the previous frame
    */
    void Update(float ElapsedSeconds) override
    {
        if (_isActive)
        {
            _lifespan -= ElapsedSeconds;
            _position.x += _velocity.x * ElapsedSeconds;
            _position.y += _velocity.y * ElapsedSeconds;
            _position.z += _velocity.z * ElapsedSeconds;
        }
    }

    /* Function: SetPosition
    *       Draw a spher to visualize the bullet
    */
    void Draw() override
    {
        if (_isActive)
        {
            DrawSphere(_position, _radius, RED);
        }
    }

    /* Function: SetPosition
    *       Activate the bullet object and reset its lifespan
    */
    void Activate() override
    {
        __super::Activate();
        _lifespan = BULLET_LIFETIME;
    }
};