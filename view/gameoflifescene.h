#ifndef GAMEOFLIFESCENE_H
#define GAMEOFLIFESCENE_H

#include <QGraphicsScene>
#include <QTimer>

#include "model/gameoflifepatternmodel.h"
#include "generic/model/golmodel.h"

class QGraphicsItem;

class GameOfLifeScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameOfLifeScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);

    void addSelectedPatternAt(const QPointF &pos);
    void onCellChanged(const point_t point, GolModel::Status status);


signals:

public slots:
    void Start();
    void requestNewGeneration();

    void patternSelected(const GenGolPatternPtr &pattern);

protected:
    virtual void dropEvent( QGraphicsSceneDragDropEvent * event );
    virtual void dragEnterEvent( QGraphicsSceneDragDropEvent * event );
    virtual void dragLeaveEvent( QGraphicsSceneDragDropEvent * event );
    virtual void dragMoveEvent( QGraphicsSceneDragDropEvent * event );

private:
    QTimer _timer;

    GolModel _model;
    GolModel::connection_t  _connection;

    GenGolPatternPtr _selectedPattern;
};

#endif // GAMEOFLIFESCENE_H
