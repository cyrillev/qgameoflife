#include "cellpainter.h"

#include <QtCore>
#include <QtGui>

#include <algorithm>

QIcon CellPainter::makeIcon(GenGolPatternPtr pattern, size_t size, size_t margin)
{
    if (pattern->width() == 0 || pattern->height() == 0)
        return QIcon();

    QPixmap pixmap( size*(pattern->width()), size*(pattern->height()) );
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QBrush blackBrush(Qt::black);

    foreach (const point_t & c , pattern->cells() )
    {
        QRect rect( QPoint(c.first * size, c.second * size) , QSize(size, size));
        painter.fillRect(rect, blackBrush);
    }
    return QIcon(pixmap);
}
