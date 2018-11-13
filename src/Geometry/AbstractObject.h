#ifndef ABSTRACTOBJECT_H
#define ABSTRACTOBJECT_H

#include <QMatrix4x4>
#include <QVector>

#define M_PI 3.14159265358979323846264338327950288

/* This is the mother class of every object in a scene. It manages the hierarchy,
 * local/global transformations, and provides methods that parse the whole hierarchy
 * to update global transformations, gathering lights in the hierarchy,
 * and rendering every object in the hierarchy given a single shader.
 *
 * One generally needs to create a root, attach every object to a node below this
 * root, then call update/... on the root only.
 *
 * Behavior :
 *   globalVector = globalTransformation * localVector
 *   localVector = globalTransformation^-1 * globalVector;
 */

class GLShader;
class Geometry;
class TimeState;

class AbstractObject
{
public:
    AbstractObject( AbstractObject* parent = 0 );
    virtual ~AbstractObject();

    QMatrix4x4& localTransformation();
    const QMatrix4x4& globalTransformation() const;

    virtual void update();
    virtual void animate( const TimeState& timeState );
    virtual void render( GLShader& shader );

    void setParent( AbstractObject* parent );

protected:
    void addChild( AbstractObject* child );
    void removeChild( AbstractObject* child );

private:
    AbstractObject* _parent;
    QVector<AbstractObject*> _children;
    QMatrix4x4 _localTransformation;
    QMatrix4x4 _globalTransformation;
};

#endif // ABSTRACTOBJECT_H
