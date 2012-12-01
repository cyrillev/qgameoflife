#ifndef GOLSETMODEL_H
#define GOLSETMODEL_H

#include "golmodelinterface.h"

#include <boost/utility.hpp>
#include <boost/signals2.hpp>
#include <boost/signals2/connection.hpp>
#include <boost/unordered_set.hpp>

namespace gol
{

class GolSetModel : public GolModelInterface
{
public:
    GolSetModel();

    // Implement GolModelInterface
    bool get(const point_t &point) const;
    void set(const point_t& point);
    void unset(const point_t& point);
    void nextGeneration(boost::unordered_set<point_t> &new_cells, boost::unordered_set<point_t> &dead_cells);

private:
    unsigned int numberOfNeighbours(const point_t &point);
    boost::unordered_set<point_t>  _cells;
};

}

#endif // GOLSETMODEL_H
