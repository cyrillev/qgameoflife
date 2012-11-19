#ifndef CELLGRAPHICSITEM_H
#define CELLGRAPHICSITEM_H

#include <QGraphicsRectItem>

class CellGraphicsItem : public QGraphicsRectItem
{
public:
    CellGraphicsItem(QGraphicsItem * parent = 0 );
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // CELLGRAPHICSITEM_H
