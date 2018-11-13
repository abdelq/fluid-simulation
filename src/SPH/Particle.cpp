#include "Particle.h"
#include <QtOpenGL>
#include <cmath>

Particle::Particle()
    : _mass( 0 )
    , _density( 0 )
    , _volume( 0 )
    , _pressure( 0 )
    , _cellIndex( 0 )
{
}

void Particle::setPosition( const QVector3D& position )
{
    _position = position;
}

void Particle::setVelocity( const QVector3D& velocity )
{
    _velocity = velocity;
}

void Particle::setAcceleration( const QVector3D& acceleration )
{
    _acceleration = acceleration;
}

void Particle::setMass( float mass )
{
    _mass = mass;
}

void Particle::setDensity( float density )
{
    _density = density;
}

void Particle::setVolume( float volume )
{
    _volume = volume;
}

void Particle::setPressure( float pressure )
{
    _pressure = pressure;
}

void Particle::setCellIndex( unsigned int cellIndex )
{
    _cellIndex = cellIndex;
}

const QVector3D& Particle::position() const
{
    return _position;
}

const QVector3D& Particle::velocity() const
{
    return _velocity;
}

const QVector3D& Particle::acceleration() const
{
    return _acceleration;
}

float Particle::mass() const
{
    return _mass;
}

float Particle::density() const
{
    return _density;
}

float Particle::volume() const
{
    return _volume;
}

float Particle::pressure() const
{
    return _pressure;
}

unsigned int Particle::cellIndex() const
{
    return _cellIndex;
}
