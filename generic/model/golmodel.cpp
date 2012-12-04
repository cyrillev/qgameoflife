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


Result GolModel::nextGeneration()
{
    Result result;

    _implementation->nextGeneration(result.new_cells, result.dead_cells);

    return result;
}


GolModel::connection_t GolModel::connect(signal_t::slot_function_type subscriber)
{
    return _signalCellChanged.connect(subscriber);
}

void GolModel::disconnect(connection_t subscriber)
{
    subscriber.disconnect();
}
