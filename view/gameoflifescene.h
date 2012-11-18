#ifndef GAMEOFLIFESCENE_H
#define GAMEOFLIFESCENE_H

#include <QGraphicsScene>

#include "model/gameoflifepatternmodel.h"

class QGraphicsItem;

class GameOfLifeScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameOfLifeScene(QObject *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void addCellAt(const QPointF &pos);
    void addSelectedPatternAt(const QPointF &pos);
signals:

public slots:
    void patternSelected(const GenGolPatternPtr &pattern);

protected:
    virtual void dropEvent( QGraphicsSceneDragDropEvent * event );
    virtual void dragEnterEvent( QGraphicsSceneDragDropEvent * event );
    virtual void dragLeaveEvent( QGraphicsSceneDragDropEvent * event );
    virtual void dragMoveEvent( QGraphicsSceneDragDropEvent * event );

private:
    GenGolPatternPtr _selectedPattern;
    int _grid_space;
};

#endif // GAMEOFLIFESCENE_H
