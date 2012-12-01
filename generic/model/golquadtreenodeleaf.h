#ifndef GOLQUADTREENODELEAF_H
#define GOLQUADTREENODELEAF_H

#include "goltypes.h"
#include "golquadtreenodebase.h"

namespace gol
{
class GolQuadtreeNodeLeaf;
typedef boost::shared_ptr<GolQuadtreeNodeLeaf> golquadtreenodeleaf_ptr;

class GolQuadtreeNodeLeaf : public GolQuadtreeNodeBase
{
public:
    GolQuadtreeNodeLeaf(coord_t x, coord_t y, coord_t width, coord_t height, golquadtreenode_weakptr parent);
    virtual ~GolQuadtreeNodeLeaf();

    void set(coord_t x, coord_t y);
    bool unset(coord_t x, coord_t y);
    bool get(coord_t x, coord_t y) const;
    bool isLeaf() const;
    void dump()  const;
    void getOverlappedNodesAt(coord_t x, coord_t y, std::vector<golquadtreenode_ptr>& nodes);

    // FIXME: find a better alternative than passing references to vector.
    void nextGeneration(boost::unordered_set<point_t>& new_cells, boost::unordered_set<point_t>& dead_cells);


    static const coord_t MIN_WIDTH = 255 - 2*MARGIN;
    static const coord_t MIN_HEIGHT = 255 - 2*MARGIN;

private:
    size_t countNeighbours(coord_t x, coord_t y) const;

    bool  _grid[MIN_WIDTH+2*MARGIN][MIN_HEIGHT+2*MARGIN];
};

}
#endif // GOLQUADTREENODELEAF_H
