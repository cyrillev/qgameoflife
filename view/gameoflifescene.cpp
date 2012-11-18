#include "gameoflifescene.h"

#include <QtCore>
#include <QtGui>

#include "model/gameoflifepatternmodel.h"
#include "view/cellgraphicsitem.h"

GameOfLifeScene::GameOfLifeScene(QObject *parent) :
    QGraphicsScene(parent),
    _grid_space(10)
{
    // No index is applied. Item location is of linear complexity,
    // as all items on the scene are searched.
    // Adding, moving and removing items, however, is done in constant time.
    // This approach is ideal for dynamic scenes,
    // where many items are added, moved or removed continuously.
    // setItemIndexMethod(QGraphicsScene::NoIndex);
}

void GameOfLifeScene::patternSelected(const GenGolPatternPtr &pattern)
{
    _selectedPattern = pattern;
}

void GameOfLifeScene::addSelectedPatternAt(const QPointF &pos)
{
    const std::vector<point_t> cells = _selectedPattern->cells();

    std::vector<point_t>::const_iterator it;
    for (it = cells.begin(); it != cells.end(); ++it)
    {
        point_t coord = (*it);
        CellGraphicsItem * item = new CellGraphicsItem(0,0, _grid_space, _grid_space);
        addItem(item);
        item->setPos(pos.x() + coord.first *_grid_space, pos.y() + coord.second *_grid_space  );
    }
}
void GameOfLifeScene::addCellAt(const QPointF &pos)
{
    CellGraphicsItem * item = new CellGraphicsItem(0,0, _grid_space, _grid_space);
    addItem(item);
    item->setPos(pos);
}

void GameOfLifeScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    addSelectedPatternAt(mouseEvent->scenePos());

    QGraphicsScene::mousePressEvent(mouseEvent);
}



void GameOfLifeScene::dragEnterEvent( QGraphicsSceneDragDropEvent * event )
{
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasFormat("text/uri-list"))
    {
        qDebug() << "files dropped:";
        foreach (const QUrl uri , mimeData->urls())
        {
            QFileInfo info( uri.toLocalFile() );
            if (info.suffix() == "rle")
            {
                qDebug() << "Drag&Drop " << info.canonicalFilePath();
            }
        }
    }

    if (!mimeData->hasFormat(GameOfLifePatternModel::PatternMineData))
    {
        event->dropAction();
        return;
    }

    QByteArray encodedData = mimeData->data( GameOfLifePatternModel::PatternMineData );
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QString data;
    stream >> data;
    QString content = mimeData->text().toLatin1().data();

    qDebug() << "dragEnterEvent: " << data << " at pos: " << event->scenePos().x() << "," << event->scenePos().y();


    // Set the drop action to be the proposed action.
    event->acceptProposedAction();

    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
}

void GameOfLifeScene::dragLeaveEvent( QGraphicsSceneDragDropEvent * event )
{
    QApplication::restoreOverrideCursor();
}

void GameOfLifeScene::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
{
    // Set the drop action to be the proposed action.
    event->acceptProposedAction();
}

void GameOfLifeScene::dropEvent( QGraphicsSceneDragDropEvent * event )
{
    const QMimeData *mimeData = event->mimeData();
    QByteArray encodedData = mimeData->data("application/gol-pattern");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QString data;
    stream >> data;
    QString content = event->mimeData()->text().toLatin1().data();

    qDebug() << "dropEvent: " << data << " at pos: " << event->scenePos().x() << "," << event->scenePos().y();

    event->accept();


}
