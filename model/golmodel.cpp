#include "golmodel.h"
#include <stdexcept>
#include <QTimer>

GolModel::GolModel()
    : _rowCount(0),
      _columnCount(0),
      _timer( NULL )
{
}

GolModel::GolModel(unsigned int rowCount, unsigned int columnCount)
    : _rowCount(rowCount),
      _columnCount(columnCount),
      _timer( new QTimer(this) )
{
    _board.resize( _rowCount * _columnCount );
    connect(_timer, SIGNAL(timeout()), this, SLOT(nextGeneration()));
}

unsigned int GolModel::rowCount() const
{
    return _rowCount;
}

unsigned int GolModel::columnCount() const
{
    return _columnCount;
}

QVariant GolModel::data(unsigned int row, unsigned int column) const
{
    const int index = row*columnCount() + column;
    return data(index);
}

QVariant GolModel::data(unsigned int index) const
{
    return _board[index];
}


void GolModel::setData(unsigned int row, unsigned int column, QVariant value)
{
    const int index = row*columnCount() + column;
    setData(index, value);
}

void GolModel::setData(unsigned int index, QVariant value)
{
    _board[index] = (value.toInt() == ALIVE ? ALIVE : DEAD);

    emit dataChanged(index, _board[index]);
}



unsigned int GolModel::numberOfNeighbours(unsigned int index)
{
    const unsigned int row = index / columnCount();
    const unsigned int column = index - row * columnCount();

    unsigned int result = 0;

    if (row > 0 && column > 0 && data(row-1, column-1) == ALIVE)
        result++;

    if (row > 0  && data(row-1, column) == ALIVE)
        result++;

    if (row > 0  && column < columnCount() && data(row-1, column+1) == ALIVE)
        result++;

    if (column > 0 && data(row, column-1) == ALIVE)
        result++;

    if (column < columnCount() && data(row, column+1) == ALIVE)
        result++;

    if (row < rowCount() && column > 0 && data(row+1, column-1) == ALIVE)
        result++;

    if (row < rowCount() && data(row+1, column) == ALIVE)
        result++;

    if (row < rowCount() && column < columnCount() && data(row+1, column+1) == ALIVE)
        result++;


    return result;
}

void GolModel::start()
{
    if (_timer)
        _timer->start(1000);
}

void GolModel::nextGeneration()
{
    for (unsigned int index = 0; index < rowCount()*columnCount() ; index++)
    {
        const unsigned int neighbours = numberOfNeighbours(index);

        // Any live cell with fewer than two live neighbours dies, as if caused by under-population.
        if (data(index) == ALIVE && neighbours < 2 )
            setData(index, DEAD);


        // Any live cell with more than three live neighbours dies, as if by overcrowding.
        if (data(index) == ALIVE && neighbours > 3 )
            setData(index, DEAD);

        // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
        if (data(index) == DEAD && neighbours == 3 )
            setData(index, ALIVE);
    }
}
