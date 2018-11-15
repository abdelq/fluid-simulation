#include "MarchingTetrahedra.h"
#include <QtOpenGL>

MarchingTetrahedra::MarchingTetrahedra( const BoundingBox& boundingBox, unsigned int nbCubeX, unsigned int nbCubeY, unsigned int nbCubeZ )
    : _boundingBox( boundingBox )
{
    QVector3D boxExtent = boundingBox.maximum() - boundingBox.minimum();

    _nbCubes[0] = nbCubeX;
    _nbCubes[1] = nbCubeY;
    _nbCubes[2] = nbCubeZ;
    _cubeSize[0] = boxExtent.x() / _nbCubes[0];
    _cubeSize[1] = boxExtent.y() / _nbCubes[1];
    _cubeSize[2] = boxExtent.z() / _nbCubes[2];

    // Allocate vertex value and position vector
    unsigned int nbCubes = ( nbCubeX + 1 ) * ( nbCubeY + 1 ) * ( nbCubeZ + 1 );
    _vertexValues.resize( nbCubes );
    _vertexNormals.resize( nbCubes );
    _vertexPositions.resize( nbCubes );

    computeVertexPositions();
}

void MarchingTetrahedra::render(const QMatrix4x4& transformation, GLShader& shader, ImplicitSurface& implicitSurface) {
    // Calculez les valeurs et les normales aux sommets en appelant 'computeVertexInfo' avant de faire le rendu de chaque cube de
    // la grille à l'aide de la fonction 'renderCube'. Une fois fait, appelez 'renderTriangles' pour faire le rendu des triangles.

    _nbGLVertices = 0;

    computeVertexInfo(implicitSurface);

    for (unsigned int z = 0; z < _nbCubes[2]; ++z)
        for (unsigned int y = 0; y < _nbCubes[1]; ++y)
            for (unsigned int x = 0; x < _nbCubes[0]; ++x)
                renderCube(x, y, z);

    renderTriangles(transformation, shader);
}

void MarchingTetrahedra::computeVertexPositions()
{   
    unsigned int currentVertex = 0;

	// Precompute the position of each vertex of the grid
    for ( unsigned int z=0 ; z<_nbCubes[2]+1 ; ++z )
        for ( unsigned int y=0 ; y<_nbCubes[1]+1 ; ++y )
            for ( unsigned int x=0 ; x<_nbCubes[0]+1 ; ++x, ++currentVertex )
                _vertexPositions[currentVertex] = vertexPosition( x, y, z );
}

void MarchingTetrahedra::computeVertexInfo(ImplicitSurface& implicitSurface) {
    // Pour chaque sommet de la grille, remplir les variables membres '_vertexValues' et '_vertexNormals' à l'aide de la position du
    // vertex '_vertexPositions' et de la classe 'implicitSurface'. Notez que les tableaux sont indexés par un seul nombre ... Notez
    // également qu'il y a (_nbCubes[0]+1)x(_nbCubes[1]+1)x(_nbCubes[2]+1) sommets dans la grille.

    int i = 0; // Indice de sommet
    for (unsigned int z = 0; z < _nbCubes[2] + 1; ++z)
        for (unsigned int y = 0; y < _nbCubes[1] + 1; ++y)
            for (unsigned int x = 0; x < _nbCubes[0] + 1; ++x, ++i)
                implicitSurface.surfaceInfo(_vertexPositions[i], _vertexValues[i], _vertexNormals[i]);
}

void MarchingTetrahedra::renderCube(unsigned int x, unsigned int y, unsigned int z) {
    // Divisez votre cube en six tétraèdres en utilisant les sommets du cube, et faire appel à 'renderTetrahedron'
    // pour le rendu de chacun d'eux

    int leftBottomFront  = vertexIndex(x,   y,   z  ),
        leftBottomRear   = vertexIndex(x,   y,   z+1),
        leftTopFront     = vertexIndex(x,   y+1, z  ),
        leftTopRear      = vertexIndex(x,   y+1, z+1),
        rightBottomFront = vertexIndex(x+1, y,   z  ),
        rightBottomRear  = vertexIndex(x+1, y,   z+1),
        rightTopFront    = vertexIndex(x+1, y+1, z  ),
        rightTopRear     = vertexIndex(x+1, y+1, z+1);

    renderTetrahedron(leftBottomFront, leftBottomRear,   leftTopRear,     rightTopRear);
    renderTetrahedron(leftBottomFront, leftBottomRear,   rightBottomRear, rightTopRear);
    renderTetrahedron(leftBottomFront, leftTopFront,     leftTopRear,     rightTopRear);
    renderTetrahedron(leftBottomFront, leftTopFront,     rightTopFront,   rightTopRear);
    renderTetrahedron(leftBottomFront, rightBottomFront, rightBottomRear, rightTopRear);
    renderTetrahedron(leftBottomFront, rightBottomFront, rightTopFront,   rightTopRear);
}

void MarchingTetrahedra::renderTetrahedron(int p1, int p2, int p3, int p4) {
    // En utilisant les valeurs aux sommets, voyez dans quel cas de rendu vous vous trouvez puis faites appel
    // à 'renderTriangle' ou 'renderQuad' dépendant du cas

    bool isP1Pos = _vertexValues[p1] > 0, isP2Pos = _vertexValues[p2] > 0,
         isP3Pos = _vertexValues[p3] > 0, isP4Pos = _vertexValues[p4] > 0;

    // TODO Simplify
    if (isP1Pos == isP2Pos && isP1Pos != isP3Pos && isP1Pos != isP4Pos)
        renderQuad(p1, p2, p3, p4);
    else if (isP1Pos == isP3Pos && isP1Pos != isP2Pos && isP1Pos != isP4Pos)
        renderQuad(p1, p3, p2, p4);
    else if (isP1Pos == isP4Pos && isP1Pos != isP2Pos && isP1Pos != isP3Pos)
        renderQuad(p1, p4, p2, p3);
    else if (isP1Pos != isP2Pos && isP1Pos != isP3Pos && isP1Pos != isP4Pos)
        renderTriangle(p1, p2, p3, p4);
    else if (isP2Pos != isP1Pos && isP2Pos != isP3Pos && isP2Pos != isP4Pos)
        renderTriangle(p2, p1, p3, p4);
    else if (isP3Pos != isP1Pos && isP3Pos != isP2Pos && isP3Pos != isP4Pos)
        renderTriangle(p3, p1, p2, p4);
    else if (isP4Pos != isP1Pos && isP4Pos != isP2Pos && isP4Pos != isP3Pos)
        renderTriangle(p4, p1, p2, p3);
}

void MarchingTetrahedra::renderTriangle(int in1, int out2, int out3, int out4) {
    // Calculez l'interpolation des valeurs et des normales pour les arêtes dont les sommets sont de signes
    // différents. N'oubliez pas de normaliser vos normales. Une fois fait, ajoutez le triangle à la liste des triangles
    // à affichier en utilisant la méthode 'addTriangle'

    float val1 = _vertexValues[in1];

    QVector3D pos1 = _vertexPositions[in1];
    QVector3D p0 = interpolate(pos1, val1, _vertexPositions[out2], _vertexValues[out2]);
    QVector3D p1 = interpolate(pos1, val1, _vertexPositions[out3], _vertexValues[out3]);
    QVector3D p2 = interpolate(pos1, val1, _vertexPositions[out4], _vertexValues[out4]);

    QVector3D nor1 = _vertexNormals[in1];
    QVector3D n0 = interpolate(nor1, val1, _vertexNormals[out2], _vertexValues[out2]).normalized();
    QVector3D n1 = interpolate(nor1, val1, _vertexNormals[out3], _vertexValues[out3]).normalized();
    QVector3D n2 = interpolate(nor1, val1, _vertexNormals[out4], _vertexValues[out4]).normalized();

    addTriangle(p0, p1, p2, n0, n1, n2);
}

void MarchingTetrahedra::renderQuad(int in1, int in2, int out3, int out4) {
    // Calculez l'interpolation des valeurs et des normales pour les arêtes dont les sommets sont de signes
    // différents. Vous aurez quatre sommets. Séparez le quadrilatère en deux triangles, puis ajoutez les à
    // la liste des triangles à affichier en utilisant la méthode 'addTriangle'

    float val1 = _vertexValues[in1], val2 = _vertexValues[in2],
          val3 = _vertexValues[out3], val4 = _vertexValues[out4];

    QVector3D p0 = interpolate(_vertexPositions[in1], val1, _vertexPositions[out3], val3);
    QVector3D p1 = interpolate(_vertexPositions[in1], val1, _vertexPositions[out4], val4);
    QVector3D p2 = interpolate(_vertexPositions[in2], val2, _vertexPositions[out3], val3);
    QVector3D p3 = interpolate(_vertexPositions[in2], val2, _vertexPositions[out4], val4);

    QVector3D n0 = interpolate(_vertexNormals[in1], val1, _vertexNormals[out3], val3).normalized();
    QVector3D n1 = interpolate(_vertexNormals[in1], val1, _vertexNormals[out4], val4).normalized();
    QVector3D n2 = interpolate(_vertexNormals[in2], val2, _vertexNormals[out3], val3).normalized();
    QVector3D n3 = interpolate(_vertexNormals[in2], val2, _vertexNormals[out4], val4).normalized();

    addTriangle(p0, p1, p2, n0, n1, n2);
    addTriangle(p1, p2, p3, n1, n2, n3);
}

QVector3D MarchingTetrahedra::vertexPosition( unsigned int x, unsigned int y, unsigned int z ) const
{
    return _boundingBox.minimum() + QVector3D( x * _cubeSize[0], y * _cubeSize[1], z * _cubeSize[2] );
}

int MarchingTetrahedra::vertexIndex( unsigned int x, unsigned int y, unsigned int z ) const
{
    return z * ( _nbCubes[0] + 1 ) * ( _nbCubes[1] + 1 ) + y * ( _nbCubes[0] + 1 ) + x;
}

QVector3D MarchingTetrahedra::interpolate(const QVector3D& vec1, float val1, const QVector3D& vec2, float val2) {
    return vec1 + (vec2 - vec1) * val1 / (val1 - val2);
}

void MarchingTetrahedra::addTriangle( const QVector3D& p0, const QVector3D& p1, const QVector3D& p2,
                                      const QVector3D& n0, const QVector3D& n1, const QVector3D& n2 )
{
    if ( _glVertices.size() <= _nbGLVertices )
    {
        _glVertices.resize( _glVertices.size() + 192 );
        _glNormals.resize( _glNormals.size() + 192 );
    }

    _glVertices[_nbGLVertices+0] = p0;
    _glVertices[_nbGLVertices+1] = p1;
    _glVertices[_nbGLVertices+2] = p2;
    _glNormals[_nbGLVertices+0] = n0;
    _glNormals[_nbGLVertices+1] = n1;
    _glNormals[_nbGLVertices+2] = n2;
    _nbGLVertices += 3;
}

void MarchingTetrahedra::renderTriangles( const QMatrix4x4& transformation, GLShader& shader )
{
    shader.setGlobalTransformation( transformation );

    shader.enableVertexAttributeArray();
    shader.enableNormalAttributeArray();
    shader.setVertexAttributeArray( _glVertices.data() );
    shader.setNormalAttributeArray( _glNormals.data() );

    glDrawArrays( GL_TRIANGLES, 0, _nbGLVertices );

    shader.disableVertexAttributeArray();
    shader.disableNormalAttributeArray();
}
