#include "AbstractObject.h"

AbstractObject::AbstractObject( AbstractObject* parent )
    : _parent( parent )
{
    if ( parent )
        parent->addChild( this );
}

AbstractObject::~AbstractObject()
{
}

QMatrix4x4& AbstractObject::localTransformation()
{
    return _localTransformation;
}

const QMatrix4x4& AbstractObject::globalTransformation() const
{
    return _globalTransformation;
}

void AbstractObject::update()
{
    if ( _parent )
        _globalTransformation = _parent->globalTransformation() * _localTransformation;
    else
        _globalTransformation = _localTransformation;

    for ( int i=0 ; i<_children.size() ; ++i )
        _children[i]->update();
}

void AbstractObject::animate( const TimeState& timeState )
{
    for ( int i=0 ; i<_children.size() ; ++i )
        _children[i]->animate( timeState );
}

void AbstractObject::render( GLShader& shader )
{
    for ( int i=0 ; i<_children.size() ; ++i )
        _children[i]->render( shader );
}

void AbstractObject::setParent( AbstractObject* parent )
{
    if ( _parent )
        _parent->removeChild( this );

    _parent = parent;

    if ( _parent )
        _parent->addChild( this );
}

void AbstractObject::addChild( AbstractObject* child )
{
    _children.append( child );
}

void AbstractObject::removeChild( AbstractObject* child )
{
    int index = _children.indexOf( child );

    if ( index != -1 )
        _children.remove( index );
}
