#ifndef GOLMODEL_H
#define GOLMODEL_H

#include <QAbstractTableModel>

#include <vector>

class QTimer;

class GolModel : public QObject
{
    Q_OBJECT
    Q_ENUMS(Status)
    Q_PROPERTY(unsigned int rows READ rowCount CONSTANT)
    Q_PROPERTY(unsigned int columns READ columnCount CONSTANT)

public:
    enum Status
    {
        DEAD,
        ALIVE
    };

    GolModel();
    GolModel(unsigned int rowCount, unsigned int columnCount);

    unsigned int rowCount() const;
    unsigned int columnCount() const;

    Q_INVOKABLE QVariant data(unsigned int row, unsigned int column) const;
    Q_INVOKABLE QVariant data(unsigned int index) const;

    Q_INVOKABLE void setData(unsigned int row, unsigned int column, QVariant value);
    Q_INVOKABLE void setData(unsigned int index, QVariant value);

signals:
    void dataChanged(unsigned int index, QVariant value);

public slots:
    void start();

private slots:
    void nextGeneration();

private:
    unsigned int numberOfNeighbours(unsigned int index);

    std::vector<Status>  _board;
    int _rowCount;
    int _columnCount;
    QTimer * _timer;
};

#endif // GOLMODEL_H
