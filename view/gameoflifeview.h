#ifndef GAMEOFLIFEVIEW_H
#define GAMEOFLIFEVIEW_H

#include <QGraphicsView>
#include <memory>
#include <generic/model/golmodel.h>

class GameOfLifeScene;

class GameOfLifeView : public QGraphicsView
{
    Q_OBJECT
public:
    GameOfLifeView(QWidget *parent = 0);

    void setZoomLevel(double zoom);
    void ZoomIn();
    void ZoomOriginal();
    void ZoomOut();
    void ZoomFit();
    void scaleView(qreal scaleFactor);
    void patternSelected(QString name);
signals:

public slots:


protected:
    // reimplemented from QGraphicsView
    virtual void drawBackground(QPainter* painter, const QRectF& rect);
    virtual void wheelEvent(QWheelEvent* event);


private:
    Q_DISABLE_COPY(GameOfLifeView)

    GameOfLifeScene *_scene;

    double _zoomLevel;

    QString _patternSelected;
};

#endif // GAMEOFLIFEVIEW_H
