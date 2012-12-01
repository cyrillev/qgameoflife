#include "golmodel.h"
#include "golsetmodel.h"
#include "golquadtree.h"

#include <boost/unordered_set.hpp>
#include <boost/foreach.hpp>

using namespace gol;

GolModel::GolModel(Algorithm algo)
{
    switch (algo)
    {
    case UNORDERED_SET:
        _implementation.reset( new GolSetModel());
        break;
    case QUAD_TREE:
        _implementation.reset( new GolQuadtree(100000,100000));
        break;
    }
}

bool GolModel::get(const point_t &point) const
{
    return _implementation->get( point );
}

bool GolModel::get(coord_t x, coord_t y) const
{
    return _implementation->get( point_t(x, y) );
}

void GolModel::set(const point_t &point)
{
    if (!get(point))
    {
        _implementation->set(point);
        _signalCellChanged(point, true);
    }
}


void GolModel::set(coord_t x, coord_t y)
{
    set( point_t(x, y) );
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
