#ifndef GOLQUADTREE_H
#define GOLQUADTREE_H


#include "golmodelinterface.h"
#include "golquadtreenodebase.h"

#include <boost/unordered_set.hpp>

namespace gol
{
class GolQuadtree : public GolModelInterface
{
public:
    GolQuadtree(coord_t  width, coord_t  height);
    ~GolQuadtree();

    coord_t width();
    coord_t height();

    // Implement GolModelInterface
    bool get(const point_t &cell) const;
    void set(const point_t& cell);
    void unset(const point_t& cell);
    void nextGeneration(boost::unordered_set<point_t>& new_cells, boost::unordered_set<point_t>& dead_cells);

    void dump()  const;

private:
    golquadtreenode_ptr _root;
};

}
#endif // GOLQUADTREE_H
