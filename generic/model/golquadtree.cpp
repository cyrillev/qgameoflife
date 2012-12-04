#include "golquadtree.h"

#include <boost/foreach.hpp>

using namespace gol;

GolQuadtree::GolQuadtree(coord_t width, coord_t height)
{
    _universe = GolQuadtreeNodeBase::make_node(-width/2, -height/2, width, height, golquadtreenode_ptr());
}

GolQuadtree::~GolQuadtree()
{
}

coord_t GolQuadtree::width()
{
    return _universe->width();
}

coord_t GolQuadtree::height()
{
    return _universe->height();
}

void GolQuadtree::set(const point_t& cell)
{
    _universe->set(cell.x, cell.y);
}

void GolQuadtree::unset(const point_t& cell)
{
    _universe->unset(cell.x, cell.y);
}

bool GolQuadtree::get(const point_t& cell) const
{
    return _universe->get(cell.x, cell.y);
}

void GolQuadtree::dump() const
{
    _universe->dump();
}

void GolQuadtree::nextGeneration(boost::unordered_set<point_t> &new_cells, boost::unordered_set<point_t> &dead_cells)
{
    _universe->nextGeneration(new_cells, dead_cells);

    // FIXME: optimize the update of the leaf's grids.
    // most of the cells can be updated in GolQuadtreeNodeLeaf::nextGeneration
    // only the cells on the border should be updated through golquadtree::nextGeneration
    BOOST_FOREACH(point_t new_cell, new_cells)
    {
        set(new_cell);
    }

    BOOST_FOREACH(point_t dead_cell, dead_cells)
    {
        unset(dead_cell);
    }
}

