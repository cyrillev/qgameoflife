#ifndef GOLMODEL_H
#define GOLMODEL_H

#include "goltypes.h"
#include "golmodelinterface.h"

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/signals2.hpp>

namespace gol
{

typedef boost::unordered_set<point_t> cell_collection_t;

struct Result
{
    cell_collection_t new_cells;
    cell_collection_t dead_cells;
    int number_of_cells_in_universe;
};

class GolModel : boost::noncopyable
{
public:
    enum Algorithm
    {
        UNORDERED_SET,
        QUAD_TREE
    };

    GolModel(Algorithm algo);


    /**
     * Getter method
     * return true if the cell at the given coordinates  is alive
     * return false if the cell at the given coordinates is dead
     */
    bool get(const point_t &point) const;
    bool get(coord_t x, coord_t y) const;

    /**
     * Setter method
     * set the cell at the given coordinates alive
     */
    void set(const point_t& point);
    void set(coord_t y, coord_t y);

    /**
     * Calculate the next generation of the universe using the Game of Life rules
     *
     * http://en.wikipedia.org/wiki/Conway's_Game_of_Life
     *
     * The universe of the Game of Life is an infinite two-dimensional orthogonal grid of square cells,
     * each of which is in one of two possible states, alive or dead.
     *
     * Every cell interacts with its eight neighbours, which are the cells
     * that are horizontally, vertically, or diagonally adjacent.
     *
     * At each step in time, the following transitions occur:
     * Any live cell with fewer than two live neighbours dies, as if caused by under-population.
     * Any live cell with two or three live neighbours lives on to the next generation.
     * Any live cell with more than three live neighbours dies, as if by overcrowding.
     * Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
     *
     * return the pair of new cells and dead cell
     */
    Result nextGeneration();

    // connection to change events
    // FIXME remove me
    typedef boost::signals2::signal<void (const point_t, bool)>  signal_t;
    typedef boost::signals2::connection  connection_t;
    connection_t connect(signal_t::slot_function_type subscriber);
    void disconnect(connection_t subscriber);

private:
    signal_t  _signalCellChanged;
    boost::scoped_ptr<gol::GolModelInterface> _implementation;
};

};
#endif // GOLMODEL_H
