#include "golquadtreenode.h"

using namespace gol;

GolQuadtreeNode::GolQuadtreeNode(coord_t x, coord_t y, coord_t width, coord_t height, golquadtreenode_weakptr parent)
    : GolQuadtreeNodeBase(x, y, width, height, parent)
{
}

GolQuadtreeNode::~GolQuadtreeNode()
{
}


void GolQuadtreeNode::set(coord_t x, coord_t y)
{
    Quadrant q = getQuadrant(x, y);

    if (!_quadrants[q])
    {
        _quadrants[q] = make_node(
                    getQuadrantX(q),
                    getQuadrantY(q),
                    getQuadrantWidth(q),
                    getQuadrantHeight(q),
                    shared_from_this() );
    }

    _quadrants[q]->set(x, y);
}


bool GolQuadtreeNode::unset(coord_t x, coord_t y)
{
    bool deleteMe = false;

    deleteMe = true;
    for (int i=0; i < NumberOfQuadrants; i++)
    {
        if (_quadrants[i])
        {
            if (_quadrants[i]->contains(x, y) )
            {
                if (_quadrants[i]->unset(x, y))
                {
                    _quadrants[i].reset();
                }
                else
                {
                    deleteMe = false;
                }
            }
            else
            {
                deleteMe = false;
            }
        }
    }

    return deleteMe;
}

bool GolQuadtreeNode::get(coord_t x, coord_t y) const
{
    if (!contains(x, y))
        return parent()->get(x, y);

    for (int i=0; i < NumberOfQuadrants; i++)
    {
        if (_quadrants[i] && _quadrants[i]->contains(x, y) )
        {
            return _quadrants[i]->get(x, y);
        }
    }

    return false;
}

bool GolQuadtreeNode::isLeaf() const
{
    return false;
}


void GolQuadtreeNode::dump() const
{
    for (int i=0; i<NumberOfQuadrants; i++)
    {
        if (_quadrants[i])
        {
            _quadrants[i]->dump();
        }
    }
}

void GolQuadtreeNode::nextGeneration(boost::unordered_set<point_t>& new_cells, boost::unordered_set<point_t>& dead_cells)
{
    for (int i=0; i<NumberOfQuadrants; i++)
    {
        if (_quadrants[i])
        {
            _quadrants[i]->nextGeneration(new_cells, dead_cells);
        }
    }
}



coord_t GolQuadtreeNode::getQuadrantX(Quadrant q) const
{
    switch (q)
    {
    case NorthWest:
    case SouthWest:
        return x();
    default:
        return x() + width()/2;
    }
}

coord_t GolQuadtreeNode::getQuadrantY(GolQuadtreeNode::Quadrant q) const
{
    switch (q)
    {
    case NorthWest:
    case NorthEast:
        return y();
    default:
        return y() + height()/2;
    }
}

coord_t GolQuadtreeNode::getQuadrantWidth(Quadrant q) const
{
    switch (q)
    {
    case NorthWest:
    case SouthWest:
        return width() / 2;
    default:
        return width() - width()/2;
    }
}

coord_t GolQuadtreeNode::getQuadrantHeight(Quadrant q) const
{
    switch (q)
    {
    case NorthWest:
    case NorthEast:
        return height()/2;
    default:
        return height() - height()/2;
    }
}

GolQuadtreeNode::Quadrant GolQuadtreeNode::getQuadrant(coord_t x, coord_t y) const
{
    if (x >= this->x() && x < this->x() + this->width()/2)
    {
        if (y >= this->y() && y < this->y() + this->height()/2)
        {
            return NorthWest;
        }
        else
        {
            return SouthWest;
        }

    }
    else
    {
        if (y >= this->y() && y < this->y() + this->height()/2)
        {
            return NorthEast;
        }
        else
        {
            return SouthEast;
        }
    }
}

void GolQuadtreeNode::getOverlappedNodesAt(coord_t x, coord_t y, std::vector<golquadtreenode_ptr>& nodes)
{
    for (int i=0; i<NumberOfQuadrants; i++)
    {
        if (_quadrants[i])
        {
            if (_quadrants[i]->contains_with_margin(x, y) )
            {
                _quadrants[i]->getOverlappedNodesAt(x, y, nodes);
            }
        }
        else
        {

        }
    }
}

