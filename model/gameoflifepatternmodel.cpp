#include "gameoflifepatternmodel.h"


#include <QtCore>
#include <QtGui>

#include <algorithm>
#include <boost/filesystem.hpp>

#include "model/cellpainter.h"

const QString GameOfLifePatternModel::PatternMineData = "application/gol-pattern";

GameOfLifePatternModel::GameOfLifePatternModel(QObject *parent)
    : QAbstractListModel(parent)
{
    setSupportedDragActions(Qt::MoveAction);
}

void GameOfLifePatternModel::loadDirectory(QString directory)
{
    _model.LoadDirectory( boost::filesystem::path(directory.toStdString()) );
    reset();
}

int GameOfLifePatternModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _model.size();
}


/*!
    Returns an appropriate value for the requested data.
    If the view requests an invalid index, an invalid variant is returned.
    Any valid index that corresponds to a string in the list causes that
    string to be returned.
*/
QVariant GameOfLifePatternModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (static_cast<size_t>(index.row()) >= _model.size())
        return QVariant();

    GenGolPatternPtr pattern = _model.get(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
    {
        return QString(pattern->name().data());
    }
    case  Qt::DecorationRole:
    {
        QIcon icon;
        // FIXME : too slow to generate icon
        // the resut must be cached
        // icon = CellPainter::makeIcon(pattern, 5, 1);
        return icon;
    }
    case GenGolPatternRole:
    {
        return QVariant::fromValue<GameOfLifePatternWrapper>(
                    GameOfLifePatternWrapper(pattern) );
    }
    default:
        return QVariant();
    }
}

/*!
    Returns the appropriate header string depending on the orientation of
    the header and the section. If anything other than the display role is
    requested, we return an invalid variant.
*/
QVariant GameOfLifePatternModel::headerData(int section, Qt::Orientation orientation,
                                            int role) const
{
    Q_UNUSED(section);

    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("Icon");
    else
        return QString("Name");
}


/*!
    Returns an appropriate value for the item's flags. Valid items are
    enabled, selectable, and editable.
*/
Qt::ItemFlags GameOfLifePatternModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return defaultFlags | Qt::ItemIsDragEnabled;
}



QStringList GameOfLifePatternModel::mimeTypes() const
{
    QStringList types;
    types << GameOfLifePatternModel::PatternMineData;
    return types;
}

QMimeData *GameOfLifePatternModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes)
    {

        if (index.isValid())
        {
            GenGolPatternPtr pattern = _model.get(index.row());
            QString text = pattern->rle_string().c_str();
            QVariant icon = data(index, Qt::DecorationRole);
            mimeData->setImageData(icon);
            stream << text;
        }
    }
    mimeData->setData("application/gol-pattern", encodedData);
    return mimeData;
}
