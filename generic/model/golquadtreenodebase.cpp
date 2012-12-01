#include "golquadtreenodebase.h"
#include "golquadtreenode.h"
#include "golquadtreenodeleaf.h"

#include <boost/make_shared.hpp>

using namespace gol;

GolQuadtreeNodeBase::GolQuadtreeNodeBase(coord_t x, coord_t y, coord_t width, coord_t height, golquadtreenode_weakptr parent)
    : _cells(0), _x(x), _y(y), _width(width), _height(height), _parent(parent)
{
    golquadtreenode_ptr parent_ptr = parent.lock();
    if (parent_ptr)
        _level = parent_ptr->level() +1;
    else
        _level = 0;
}

GolQuadtreeNodeBase::~GolQuadtreeNodeBase()
{
}

bool GolQuadtreeNodeBase::contains(coord_t x, coord_t y) const
{
    if ( x >= this->x() && x < this->x() + this->width() &&
         y >= this->y() && y < this->y() + this->height())
    {
        return true;
    }
    return false;
}

bool GolQuadtreeNodeBase::contains_with_margin(coord_t x, coord_t y) const
{
    if ( x >= this->x() - MARGIN && x < this->x() + this->width() + MARGIN &&
         y >= this->y() - MARGIN && y < this->y() + this->height() + MARGIN)
    {
        return true;
    }
    return false;
}


golquadtreenode_ptr GolQuadtreeNodeBase::make_node(coord_t x, coord_t y, coord_t width, coord_t height, golquadtreenode_weakptr parent)
{
    if (width <= GolQuadtreeNodeLeaf::MIN_WIDTH && height <= GolQuadtreeNodeLeaf::MIN_HEIGHT)
    {
        return boost::make_shared<GolQuadtreeNodeLeaf>(x, y, width, height, parent);
    }
    else
    {
        return boost::make_shared<GolQuadtreeNode>(x, y, width, height, parent);
    }
}
