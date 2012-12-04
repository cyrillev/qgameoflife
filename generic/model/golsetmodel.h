#ifndef GOLSETMODEL_H
#define GOLSETMODEL_H

#include "golmodelinterface.h"

#include <boost/utility.hpp>
#include <boost/signals2.hpp>
#include <boost/signals2/connection.hpp>
#include <boost/unordered_set.hpp>

namespace gol
{

/**
 * GolSetModel is an implementaton of the Game of Life
 * that uses an unordered set to store live cells
*/
class GolSetModel : public GolModelInterface
{
public:
    GolSetModel();

    /** Implement GolModelInterface */
    bool get(const point_t &point) const;
    void set(const point_t& point);
    void unset(const point_t& point);
    void nextGeneration(boost::unordered_set<point_t> &new_cells, boost::unordered_set<point_t> &dead_cells);

private:
    /** return the number of alive cells that have a connection to the cell at the given coordinates */
    unsigned int numberOfNeighbours(const point_t &point);

    /** the universe is an unordered set of alive cells */
    boost::unordered_set<point_t>  _universe;
};

}

#endif // GOLSETMODEL_H
