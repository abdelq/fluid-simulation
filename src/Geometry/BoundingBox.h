#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QVector3D>
#include <QVector>

/* This is the axis aligned bounding box class. Given a set of points,
 * it computes the minimum and maximum bounds of the set.
 */

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox( const QVector3D& minimum, const QVector3D& maximum );
    BoundingBox( const QVector<QVector3D>& vertices );

    const QVector3D& minimum() const;
    const QVector3D& maximum() const;

private:
    QVector3D _minimum;
    QVector3D _maximum;
};

#endif // BOUNDINGBOX_H
