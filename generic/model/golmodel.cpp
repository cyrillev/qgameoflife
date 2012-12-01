#include "golmodel.h"
#include <stdexcept>
#include <QTimer>

#include <boost/unordered_set.hpp>
#include <boost/foreach.hpp>

using namespace gol;


typedef boost::unordered_set<point_t>::iterator iterator;
typedef boost::unordered_set<point_t>::const_iterator const_iterator;

GolModel::GolModel(Algorithm algo)
{
    switch (algo)
    {
    case UNORDERED_SET:
        _implementation.reset( new gol::GolSetModel());
        break;
    }
}

bool GolModel::data(const point_t &point) const
{
    return _implementation->data( point );
}

bool GolModel::data(coord_t x, coord_t y) const
{
    return _implementation->data( point_t(x, y) );
}

void GolModel::setData(const point_t &point, bool value)
{
    if (value != data(point))
    {
        _implementation->setData(point, value);
        _signalCellChanged(point, value);
    }
}


void GolModel::setData(coord_t x, coord_t y, bool value)
{
    setData(point_t(x, y), value);
}


void GolModel::nextGeneration()
{
    boost::unordered_set<point_t> new_cells, dead_cells;
    _implementation->nextGeneration(new_cells, dead_cells);

    BOOST_FOREACH( point_t cell, new_cells )
    {
        _signalCellChanged(cell, true);
    }
    BOOST_FOREACH( point_t cell, dead_cells )
    {
        _signalCellChanged(cell, false);
    }
}


GolModel::connection_t GolModel::connect(signal_t::slot_function_type subscriber)
{
    return _signalCellChanged.connect(subscriber);
}

void GolModel::disconnect(connection_t subscriber)
{
    subscriber.disconnect();
}
