#include "Grid.h"
#include <cmath>

Grid::Grid( const BoundingBox& boundingBox, unsigned int nbCellX, unsigned int nbCellY, unsigned int nbCellZ, float radius )
    : _boundingBox( boundingBox )
{
    QVector3D boxSize = _boundingBox.maximum() - _boundingBox.minimum();

    _nbCell[0] = nbCellX;
    _nbCell[1] = nbCellY;
    _nbCell[2] = nbCellZ;
    _cellSize[0] = boxSize.x() / _nbCell[0];
    _cellSize[1] = boxSize.y() / _nbCell[1];
    _cellSize[2] = boxSize.z() / _nbCell[2];

    buildNeighborhoods( radius );
}

void Grid::buildNeighborhoods( float radius )
{
    unsigned int nbCells = _nbCell[0] * _nbCell[1] * _nbCell[2];
    _neighborhoods.resize( nbCells );
    _cellParticles.resize( nbCells );

    for ( unsigned int x=0 ; x<_nbCell[0] ; ++x )
        for ( unsigned int y=0 ; y<_nbCell[1] ; ++y )
            for ( unsigned int z=0 ; z<_nbCell[2] ; ++z )
                buildNeighborhood( x, y, z, radius );
}

void Grid::buildNeighborhood( unsigned int x, unsigned int y, unsigned int z, float radius )
{
    int sizeX = (int)ceilf( radius / _cellSize[0] );
    int sizeY = (int)ceilf( radius / _cellSize[1] );
    int sizeZ = (int)ceilf( radius / _cellSize[2] );
    int minX = std::max<int>( 0, x - sizeX );
    int minY = std::max<int>( 0, y - sizeY );
    int minZ = std::max<int>( 0, z - sizeZ );
    int maxX = std::min<int>( _nbCell[0], x + sizeX + 1 );
    int maxY = std::min<int>( _nbCell[1], y + sizeY + 1 );
    int maxZ = std::min<int>( _nbCell[2], z + sizeZ + 1 );

    for ( int dx=minX ; dx<maxX ; ++dx )
        for ( int dy=minY ; dy<maxY ; ++dy )
            for ( int dz=minZ ; dz<maxZ ; ++dz )
                if ( shortestDistance( x, y, z, dx, dy, dz ) < radius )
                    _neighborhoods[cellIndex(x,y,z)].append( cellIndex(dx,dy,dz) );
}

float Grid::shortestDistance( unsigned int x, unsigned int y, unsigned int z, unsigned int dx, unsigned int dy, unsigned int dz ) const
{
    QVector3D difference;

	if ( x != dx )
        difference.setX( ( std::abs( (int)dx - (int)x ) - 1 ) * _cellSize[0] );

	if ( y != dy )
        difference.setY( ( std::abs( (int)dy - (int)y ) - 1 ) * _cellSize[1] );

	if ( z != dz )
        difference.setZ( ( std::abs( (int)dz - (int)z ) - 1 ) * _cellSize[2] );

    return difference.length();
}

void Grid::addParticle( unsigned int cellIndex, unsigned int particleIndex )
{
    _cellParticles[cellIndex].append( particleIndex );
}

void Grid::removeParticle( unsigned int cellIndex, unsigned int particleIndex )
{
    QVector<unsigned int>& cell = _cellParticles[cellIndex];
    unsigned int it = cell.indexOf( particleIndex );

    cell[it] = cell.back();
    cell.pop_back();
}

unsigned int Grid::cellIndex( const QVector3D& position ) const
{
    QVector3D relativePosition = position - _boundingBox.minimum();
    unsigned int x = (unsigned int)( relativePosition.x() / _cellSize[0] );
    unsigned int y = (unsigned int)( relativePosition.y() / _cellSize[1] );
    unsigned int z = (unsigned int)( relativePosition.z() / _cellSize[2] );

    x = std::min<unsigned int>( x, _nbCell[0]-1 );
    y = std::min<unsigned int>( y, _nbCell[1]-1 );
    z = std::min<unsigned int>( z, _nbCell[2]-1 );

    return cellIndex( x, y, z );
}

unsigned int Grid::cellIndex( unsigned int x, unsigned int y, unsigned int z ) const
{
    return z * _nbCell[0] * _nbCell[1] + y * _nbCell[0] + x;
}

const QVector<unsigned int>& Grid::neighborhood( unsigned int cell ) const
{
    return _neighborhoods[cell];
}

const QVector<unsigned int>& Grid::cellParticles( unsigned int cell ) const
{
    return _cellParticles[cell];
}
