#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector3D>

/* A particle is simply a collection of properties.
 */

class Particle
{
public:
	Particle();

    // 'Setters'
    void setPosition( const QVector3D& position );
    void setVelocity( const QVector3D& velocity );
    void setAcceleration( const QVector3D& acceleration );
    void setMass( float mass );
    void setDensity( float density );
    void setVolume( float volume );
    void setPressure( float pressure );
    void setCellIndex( unsigned int cellIndex );

	// 'Getters'
    const QVector3D& position() const;
    const QVector3D& velocity() const;
    const QVector3D& acceleration() const;
    float mass() const;
    float density() const;
    float volume() const;
    float pressure() const;
    unsigned int cellIndex() const;

private:
    QVector3D _position;
    QVector3D _velocity;
    QVector3D _acceleration;
    float _mass;
    float _density;
    float _volume;
    float _pressure;
	unsigned int _cellIndex;
};

#endif //PARTICLE_H
