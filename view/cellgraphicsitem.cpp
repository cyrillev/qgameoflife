#include "cellgraphicsitem.h"

#include <QtCore>
#include <QtGui>



CellGraphicsItem::CellGraphicsItem( QGraphicsItem * parent)
    : QGraphicsRectItem(parent)
{
    init();
}

CellGraphicsItem::CellGraphicsItem( const QRectF & rect, QGraphicsItem * parent)
    : QGraphicsRectItem(rect, parent)
{

    init();
}

CellGraphicsItem::CellGraphicsItem( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent)
    : QGraphicsRectItem(x, y, width, height, parent)
{

    init();
}

void CellGraphicsItem::init()
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setBrush(QBrush(Qt::black));
}

QVariant CellGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    QVariant newValue = value;

    // implement snap-to-grid
    switch (change)
    {
    case QGraphicsItem::ItemPositionHasChanged:
    {
        int x = (int) value.toPointF().x();
        int y = (int) value.toPointF().y();
        QPointF newValue (x -x % 10, y - y%10);
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
