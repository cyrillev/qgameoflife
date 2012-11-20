#include "golmodel.h"
#include <stdexcept>
#include <QTimer>

#include <boost/unordered_set.hpp>
#include <boost/foreach.hpp>

GolModel::GolModel()
{
}

GolModel::Status GolModel::data(const point_t &point) const
{
    return  _cells.find(point) != _cells.end() ? GolModel::ALIVE : GolModel::DEAD;
}

GolModel::Status GolModel::data(coord_t x, coord_t y) const
{
    return  data( point_t(x, y) );
}

void GolModel::setData(const point_t &point, GolModel::Status value)
{
    if (value == GolModel::ALIVE)
    {
        if (_cells.insert(point).second)
        {
            _signalCellChanged(point, GolModel::ALIVE);
        }
    }
    else
    {
        if (_cells.erase(point) > 0 )
        {
            _signalCellChanged(point, GolModel::DEAD);
        }
    }
}


void GolModel::setData(coord_t x, coord_t y, GolModel::Status value)
{
    setData(point_t(x, y), value);
}


unsigned int GolModel::numberOfNeighbours(const point_t &point)
{
    unsigned int result = 0;
    // scan all neighboured cells (excluding itself)
    for (coord_t x = point.first-1 ; x <= point.first+1; ++x)
    {
        for (coord_t y = point.second-1 ; y <= point.second+1; ++y)
        {
            if (x != point.first || y != point.second)
            {
                if (data(x, y) == ALIVE)
                {
                    result++;
                }
            }
        }
    }
    return result;
}


bool operator==(GolModel::status_t const& status1, GolModel::status_t const& status2)
{
    point_t p1 = status1.first;
    point_t p2 = status2.first;
    GolModel::Status s1 = status1.second;
    GolModel::Status s2 = status2.second;
    return p1.first == p2.first && p1.second && p2.second && s1 == s2;
}

std::size_t hash_value(GolModel::status_t const& status)
{
    point_t point = status.first;
    std::size_t seed = 0;
    boost::hash_combine(seed, point.first);
    boost::hash_combine(seed, point.second);
    boost::hash_combine(seed, status.second);
    return seed;
}

void GolModel::nextGeneration()
{
    boost::unordered_set<status_t> new_generation;

    BOOST_FOREACH( point_t point, _cells )
    {
        const unsigned int neighbours = numberOfNeighbours(point);

        // Any live cell with fewer than two live neighbours dies, as if caused by under-population.
        // Any live cell with more than three live neighbours dies, as if by overcrowding.
        if (neighbours < 2 || neighbours > 3)
            new_generation.insert( std::make_pair(point, DEAD) );

        // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
        for (coord_t x = point.first-1 ; x <= point.first+1; ++x)
        {
            for (coord_t y = point.second-1 ; y <= point.second+1; ++y)
            {
                if (x != point.first || y != point.second)
                {
                    point_t neighbour(x, y);
                    if (data(neighbour) == DEAD && numberOfNeighbours(neighbour) == 3 )
                        new_generation.insert( std::make_pair(neighbour, ALIVE) );
                }
            }
        }
    }

    BOOST_FOREACH( status_t updated_cell, new_generation )
    {
        setData(updated_cell.first, updated_cell.second);
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
