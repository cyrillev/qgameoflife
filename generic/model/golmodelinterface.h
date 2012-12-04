#ifndef GOLMODELINTERFACE_H
#define GOLMODELINTERFACE_H

#include "goltypes.h"

#include <boost/noncopyable.hpp>
#include <boost/unordered_set.hpp>

namespace gol
{
/**
 * GolModelInterface is a generic interface
 * for all Game of Life implementations
 *
 * This interface is meant to be used by GolModel class
 */

class GolModelInterface : private boost::noncopyable
{
public:
    virtual ~GolModelInterface() {}

    /**
     * Getter method
     * return true if the cell at the given coordinates  is alive
     * return false if the cell at the given coordinates is dead
     */
    virtual bool get(const point_t &point) const = 0;

    /**
     * Setter method
     * set the cell at the given coordinates alive
     */
    virtual void set(const point_t &point) = 0;

    /**
     * Setter method
     * set the cell at the given coordinates dead
     */
    virtual void unset(const point_t &point) = 0;

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
     * new cells are added to the unordered set of new_cells
     * dead cells are added to the unordered set of dead_cells
     *
     */
    virtual void nextGeneration(boost::unordered_set<point_t> &new_cells, boost::unordered_set<point_t> &dead_cells) = 0;
};
}
#endif // GOLMODELINTERFACE_H
