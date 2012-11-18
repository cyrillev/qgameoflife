/**
 *
 * QGraphicsView: Smooth Panning and Zooming from
 * http://www.qtcentre.org/wiki/index.php?title=QGraphicsView:_Smooth_Panning_and_Zooming
 *
 */

#include "gameoflifeview.h"

#include <QtCore>
#include <QtGui>

#include "view/gameoflifescene.h"

GameOfLifeView::GameOfLifeView(QWidget *parent) :
    QGraphicsView(parent)
{
    //setDragMode(QGraphicsView::ScrollHandDrag);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    setSceneRect(-1000,-1000,2000,2000);
    SetGridInterval(10);

    ZoomOriginal();
}


void GameOfLifeView::SetGridInterval(int interval)
{
    gridInterval = interval;
    update();
}


void GameOfLifeView::ZoomIn()
{
    setZoomLevel( _zoomLevel + .1 );
}

void GameOfLifeView::ZoomOriginal()
{
    setZoomLevel(1.0);
}

void GameOfLifeView::ZoomOut()
{
    setZoomLevel( _zoomLevel - .1 );
}

void GameOfLifeView::setZoomLevel(double zoom)
{
    _zoomLevel = zoom;

    QMatrix oldMatrix = matrix();
    resetMatrix();
    translate(oldMatrix.dx(), oldMatrix.dy());
    scale(_zoomLevel, _zoomLevel);
}


void GameOfLifeView::ZoomFit()
{
    QRectF bouding = scene()->itemsBoundingRect();
    bouding.adjust(-10, 10, 10, 10);
    fitInView(bouding, Qt::KeepAspectRatio);
}

void GameOfLifeView::drawBackground(QPainter* painter, const QRectF& rect)
{
    bool oldWorldMatrixEnabled = painter->worldMatrixEnabled();

    QPen pen(Qt::DotLine);
    pen.setCosmetic(true);
    pen.setWidth(1);
    pen.setColor(Qt::lightGray);

    QTransform t = transform();
    qreal sx = t.m11();
    qreal sy = t.m22();

    int interval = gridInterval; //interval to draw grid lines at

    while (interval*sx < 5 || interval*sy < 5)
    {
        interval *= 2;
    }
    painter->setWorldMatrixEnabled(true);

    qreal left = int(rect.left()) - (int(rect.left()) % interval );
    qreal top = int(rect.top()) - (int(rect.top()) % interval );



    QVarLengthArray<QLineF, 100> linesX;
    for (qreal x = left; x < rect.right(); x += interval )
        linesX.append(QLineF(x, rect.top(), x, rect.bottom()));

    QVarLengthArray<QLineF, 100> linesY;
    for (qreal y = top; y < rect.bottom(); y += interval )
        linesY.append(QLineF(rect.left(), y, rect.right(), y));


    painter->setPen(pen);
    painter->drawLines(linesX.data(), linesX.size());
    painter->drawLines(linesY.data(), linesY.size());

    painter->setWorldMatrixEnabled(oldWorldMatrixEnabled);
}

void GameOfLifeView::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier))
    {
        qreal scaleFactor = pow((double)2, -event->delta() / 240.0);
        scaleView(scaleFactor);
    }
    else
    {
        QGraphicsView::wheelEvent(event);
    }
}

void GameOfLifeView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GameOfLifeView::patternSelected(QString name)
{
    _patternSelected = name;
}
