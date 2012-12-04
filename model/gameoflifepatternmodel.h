#ifndef GAMEOFLIFEPATTERNMODEL_H
#define GAMEOFLIFEPATTERNMODEL_H

#include "generic/model/genericgameoflifepatternmodel.h"

#include <QAbstractListModel>
#include <QStringList>

#include <boost/ptr_container/ptr_vector.hpp>

using namespace gol;

class GameOfLifePatternWrapper
{

public:
    GameOfLifePatternWrapper() {}
    GameOfLifePatternWrapper(const GenGolPatternPtr& pattern)
        : _pattern(pattern) {}

    // cast GameOfLifePatternWrapper -> GenGolPatternPtr
    operator GenGolPatternPtr() { return _pattern; }

private:
    GenGolPatternPtr _pattern;
};
Q_DECLARE_METATYPE(GameOfLifePatternWrapper)
Q_DECLARE_METATYPE(GameOfLifePatternWrapper*)


class GameOfLifePatternModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum GolItemDataRole { GenGolPatternRole = Qt::UserRole+1 };

    static const QString PatternMineData;

    GameOfLifePatternModel(QObject *parent = 0);

    void loadDirectory(QString directory);

    // Implement QAbstractListModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;

    // The model is not editable by the user
    // bool setData(const QModelIndex &index, const QVariant &value,int role = Qt::EditRole);
    // bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    // bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());



private:
    GenGolPatternModel _model;
};

#endif // GAMEOFLIFEPATTERNMODEL_H
