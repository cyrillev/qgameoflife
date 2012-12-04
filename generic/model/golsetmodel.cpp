#include "golsetmodel.h"

#include <boost/unordered_set.hpp>
#include <boost/foreach.hpp>

using namespace gol;

GolSetModel::GolSetModel()
{
}

bool GolSetModel::get(const point_t &point) const
{
    return _universe.find(point) != _universe.end() ? true : false;
}

void GolSetModel::set(const point_t &point)
{
    _universe.insert(point);
}

void GolSetModel::unset(const point_t &point)
{
    _universe.erase(point);
}


unsigned int GolSetModel::numberOfNeighbours(const point_t &point)
{
    unsigned int result = 0;
    // scan all neighboured cells (excluding itself)
    for (coord_t x = point.x-1 ; x <= point.x+1; ++x)
    {
        for (coord_t y = point.y-1 ; y <= point.y+1; ++y)
        {
            if (x != point.x || y != point.y)
            {
                if (get(point_t(x, y)))
                {
                    result++;
                }
            }
        }
    }
    return result;
}



void GolSetModel::nextGeneration(boost::unordered_set<point_t>& new_cells, boost::unordered_set<point_t>& dead_cells)
{
    BOOST_FOREACH( point_t point, _universe )
    {
        const unsigned int neighbours = numberOfNeighbours(point);

        // Any live cell with fewer than two live neighbours dies, as if caused by under-population.
        // Any live cell with more than three live neighbours dies, as if by overcrowding.
        if (neighbours < 2 || neighbours > 3)
        {
            dead_cells.insert(point);
        }

        // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
        for (coord_t x = point.x-1 ; x <= point.x+1; ++x)
        {
            for (coord_t y = point.y-1 ; y <= point.y+1; ++y)
            {
                if (x != point.x || y != point.y)
                {
                    point_t neighbour(x, y);
                    if (!get(neighbour) && numberOfNeighbours(neighbour) == 3 )
                    {
                        new_cells.insert(neighbour);
                    }
                }
            }
        }
    }

    BOOST_FOREACH(point_t new_cell, new_cells)
    {
        set(new_cell);
    }

    BOOST_FOREACH(point_t dead_cell, dead_cells)
    {
        unset(dead_cell);
    }
}
