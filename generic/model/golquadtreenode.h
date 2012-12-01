#ifndef GOLQUADTREENODE_H
#define GOLQUADTREENODE_H


#include "golquadtreenodebase.h"

namespace gol
{
class GolQuadtreeNode : public GolQuadtreeNodeBase
{
public:
    enum Quadrant
    {
        NorthWest,
        NorthEast,
        SouthEast,
        SouthWest,
        NumberOfQuadrants
    };

    GolQuadtreeNode(coord_t x, coord_t y, coord_t width, coord_t  height, golquadtreenode_weakptr parent);
    virtual ~GolQuadtreeNode();

    void set(coord_t x, coord_t y);
    bool unset(coord_t x, coord_t y);
    bool get(coord_t x, coord_t y)  const;
    bool isLeaf() const;
    void dump()  const;

    // FIXME: find a better alternative than passing references to vector.
    void nextGeneration(boost::unordered_set<point_t>& new_cells, boost::unordered_set<point_t>& dead_cells);
    void getOverlappedNodesAt(coord_t x, coord_t y, std::vector<golquadtreenode_ptr>& nodes);
private:

    coord_t  getQuadrantX(Quadrant q) const;
    coord_t  getQuadrantY(Quadrant q) const;
    coord_t  getQuadrantWidth(Quadrant q) const;
    coord_t  getQuadrantHeight(Quadrant q) const;
    Quadrant getQuadrant(coord_t x, coord_t y) const;

    golquadtreenode_ptr _quadrants[NumberOfQuadrants];
};
}

#endif // GOLQUADTREENODE_H
