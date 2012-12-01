#include "gameoflifescene.h"

#include <QtCore>
#include <QtGui>

#include "model/gameoflifepatternmodel.h"
#include "view/cellgraphicsitem.h"

GameOfLifeScene::GameOfLifeScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
    QGraphicsScene(x, y, width, height, parent),
    _model( GolModel::UNORDERED_SET )
{
    connect(&_timer, SIGNAL(timeout()),
            this,   SLOT(requestNewGeneration()));

    // No index is applied. Item location is of linear complexity,
    // as all items on the scene are searched.
    // Adding, moving and removing items, however, is done in constant time.
    // This approach is ideal for dynamic scenes,
    // where many items are added, moved or removed continuously.
    // setItemIndexMethod(QGraphicsScene::NoIndex);

    _connection = _model.connect(
                boost::bind( &GameOfLifeScene::onCellChanged, this, _1, _2) );
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
        point_t point = (*it);
        const coord_t x = static_cast<coord_t>(pos.x() + point.x);
        const coord_t y = static_cast<coord_t>(pos.y() + point.y);
        _model.setData(x, y, true);
    }
}

void GameOfLifeScene::onCellChanged(const point_t point, bool value)
{
    if (value)
    {
        CellGraphicsItem * item = new CellGraphicsItem();
        addItem(item); // This scene takes ownership of the item.
        item->setPos(point.x, point.y);
    }
    else
    {
        QGraphicsItem * item = itemAt(point.x, point.y );
        removeItem(item);
    }
}

void GameOfLifeScene::Start()
{
    _timer.start();
}

void GameOfLifeScene::requestNewGeneration()
{
    _model.nextGeneration();
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
    Q_UNUSED(event);
    QApplication::restoreOverrideCursor();
}

void GameOfLifeScene::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
{
    Q_UNUSED(event);
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
