#include "golmodel.h"
#include <stdexcept>
#include <QTimer>

#include <boost/unordered_set.hpp>
#include <boost/foreach.hpp>

using namespace gol;


typedef boost::unordered_set<point_t>::iterator iterator;
typedef boost::unordered_set<point_t>::const_iterator const_iterator;

GolSetModel::GolSetModel()
{
}

bool GolSetModel::data(const point_t &point) const
{
    return  _cells.find(point) != _cells.end() ? true : false;
}

void GolSetModel::setData(const point_t &point, bool value)
{
    if (value)
    {
        _cells.insert(point);
    }
    else
    {
        _cells.erase(point);
    }
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
                if (data(point_t(x, y)))
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
    boost::unordered_set<status_t> new_generation;

    BOOST_FOREACH( point_t point, _cells )
    {
        const unsigned int neighbours = numberOfNeighbours(point);

        // Any live cell with fewer than two live neighbours dies, as if caused by under-population.
        // Any live cell with more than three live neighbours dies, as if by overcrowding.
        if (neighbours < 2 || neighbours > 3)
        {
            dead_cells.insert(point);
            new_generation.insert( std::make_pair(point, false) );
        }

        // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
        for (coord_t x = point.x-1 ; x <= point.x+1; ++x)
        {
            for (coord_t y = point.y-1 ; y <= point.y+1; ++y)
            {
                if (x != point.x || y != point.y)
                {
                    point_t neighbour(x, y);
                    if (!data(neighbour) && numberOfNeighbours(neighbour) == 3 )
                    {
                        new_generation.insert( std::make_pair(neighbour, true) );
                        new_cells.insert(neighbour);
                    }
                }
            }
        }
    }

    BOOST_FOREACH( status_t updated_cell, new_generation )
    {
        setData(updated_cell.first, updated_cell.second);
    }
}