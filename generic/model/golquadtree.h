#ifndef GOLQUADTREE_H
#define GOLQUADTREE_H


#include "golmodelinterface.h"
#include "golquadtreenodebase.h"

#include <boost/unordered_set.hpp>

namespace gol
{

/**
 * GolSetModel is an implementaton of the Game of Life
 * that uses a quad-tree structure.
 *
 * Each leaf of the quad-tree is a small 255:255 grid
*/
class GolQuadtree : public GolModelInterface
{
public:
    /**
     * Create an universe of the given width and height
    */
    GolQuadtree(coord_t  width, coord_t  height);

    ~GolQuadtree();

    /**
     * return the width of the universe
    */
    coord_t width();


    /**
     * return the height of the universe
    */
    coord_t height();

    // Implement GolModelInterface
    bool get(const point_t &cell) const;
    void set(const point_t& cell);
    void unset(const point_t& cell);
    void nextGeneration(boost::unordered_set<point_t>& new_cells, boost::unordered_set<point_t>& dead_cells);

    /**
     * Dump the universe to cout
     * use for debugging purpose
    */
    void dump()  const;

private:
    /** pointer to the root element of the quadtree */
    golquadtreenode_ptr _universe;
};

}
#endif // GOLQUADTREE_H
