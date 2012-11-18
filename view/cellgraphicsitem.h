#ifndef CELLGRAPHICSITEM_H
#define CELLGRAPHICSITEM_H

#include <QGraphicsRectItem>

class CellGraphicsItem : public QGraphicsRectItem
{
public:
    CellGraphicsItem( QGraphicsItem * parent = 0 );
    CellGraphicsItem( const QRectF & rect, QGraphicsItem * parent = 0 );
    CellGraphicsItem( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent = 0 );

    void init();
protected:
    // re-implemented from QGraphicsItem
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // CELLGRAPHICSITEM_H
