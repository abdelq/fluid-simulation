#ifndef MARCHINGTETRAHEDRA_H
#define MARCHINGTETRAHEDRA_H

#include "Geometry/BoundingBox.h"
#include "Geometry/ImplicitSurface.h"
#include "GLShader.h"
#include <QGLBuffer>

/* Given an implicit surface, the marching tetrahedra algorithm will extract
 * a mesh representation of F(x)=0.
 */

class MarchingTetrahedra
{
public:
    MarchingTetrahedra( const BoundingBox& boundingBox, unsigned int nbCubeX, unsigned int nbCubeY, unsigned int nbCubeZ );

    void render( const QMatrix4x4& transformation, GLShader& shader, ImplicitSurface& implicitSurface );

private:
    void computeVertexPositions();

    void computeVertexInfo( ImplicitSurface& implicitSurface );
    void renderCube( unsigned int x, unsigned int y, unsigned int z );
    void renderTetrahedron( unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4 );
    void renderTriangle( unsigned int in1, unsigned int out2, unsigned int out3, unsigned int out4 );
    void renderQuad( unsigned int in1, unsigned int in2, unsigned int out3, unsigned int out4 );
    QVector3D vertexPosition( unsigned int x, unsigned int y, unsigned int z ) const;
    unsigned int vertexIndex( unsigned int x, unsigned int y, unsigned int z ) const;

    void addTriangle( const QVector3D& p0, const QVector3D& p1, const QVector3D& p2,
                      const QVector3D& n0, const QVector3D& n1, const QVector3D& n2 );
    void renderTriangles( const QMatrix4x4& transformation, GLShader& shader );

private:
    QVector<float> _vertexValues;
    QVector<QVector3D> _vertexNormals;
    QVector<QVector3D> _vertexPositions;

    BoundingBox _boundingBox;
    unsigned int _nbCubes[3];
    float _cubeSize[3];

    // Rendering stuff
    int _nbGLVertices;
    QVector<QVector3D> _glVertices;
    QVector<QVector3D> _glNormals;
};



#endif // MARCHINGTETRAHEDRA_H
