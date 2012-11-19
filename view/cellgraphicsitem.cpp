#include "cellgraphicsitem.h"

#include <QtCore>
#include <QtGui>


CellGraphicsItem::CellGraphicsItem( QGraphicsItem * parent)
    : QGraphicsRectItem(0, 0, 1, 1, parent)
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setBrush(QBrush(Qt::black));


}

QVariant CellGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    // implement snap-to-grid
    switch (change)
    {
    case QGraphicsItem::ItemPositionHasChanged:
    {
        int x = (int) value.toPointF().x();
        int y = (int) value.toPointF().y();
        QPointF newValue (x, y );
        if (newValue!=value)
        {
            setPos(newValue);
        }
        return QGraphicsItem::itemChange(change, newValue);
    }
    default:
        break;
    }

    return QGraphicsItem::itemChange(change, value);
}
