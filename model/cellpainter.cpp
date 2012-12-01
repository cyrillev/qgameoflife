#include "cellpainter.h"

#include <QtCore>
#include <QtGui>

#include <algorithm>

QIcon CellPainter::makeIcon(gol::GenGolPatternPtr pattern, size_t size, size_t margin)
{
    Q_UNUSED(margin);

    if (pattern->width() == 0 || pattern->height() == 0)
        return QIcon();

    QPixmap pixmap( size*(pattern->width()), size*(pattern->height()) );
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QBrush blackBrush(Qt::black);

    foreach (const gol::point_t & c , pattern->cells() )
    {
        QRect rect( QPoint(c.x * size, c.y * size) , QSize(size, size));
        painter.fillRect(rect, blackBrush);
    }
    return QIcon(pixmap);
}
