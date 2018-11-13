#ifndef GRID_H
#define GRID_H

#include "Geometry/BoundingBox.h"
#include <QVector>
#include <vector>

/* A acceleration structure for the SPH simulation. The grid is a set of
 * cells. Each cell contain a list of particles, and a list of neighboring
 * cells that can be reached within a given radius.
 */

class Grid
{
public:
    Grid( const BoundingBox& boundingBox, unsigned int nbCellX, unsigned int nbCellY, unsigned int nbCellZ, float radius );

    const QVector<unsigned int>& neighborhood( unsigned int cell ) const;
    const QVector<unsigned int>& cellParticles( unsigned int cell ) const;

    void addParticle( unsigned int cellIndex, unsigned int particleIndex );
    void removeParticle( unsigned int cellIndex, unsigned int particleIndex );
    unsigned int cellIndex( const QVector3D& position ) const;

private:
    void buildNeighborhoods( float radius );
    void buildNeighborhood( unsigned int x, unsigned int y, unsigned int z, float radius );
    float shortestDistance( unsigned int x, unsigned int y, unsigned int z, unsigned int dx, unsigned int dy, unsigned int dz ) const;
    unsigned int cellIndex( unsigned int x, unsigned int y, unsigned int z ) const;

private:
    QVector<QVector<unsigned int> > _neighborhoods;
    QVector<QVector<unsigned int> > _cellParticles;

    BoundingBox _boundingBox;
    unsigned int _nbCell[3];
    float _cellSize[3];
};

#endif //GRID_H
