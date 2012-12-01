#include "golquadtree.h"

#include <assert.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <deque>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

using namespace gol;
using namespace std;

golquadtree::golquadtree(coord_t width, coord_t height)
{
    _root = GolQuadtreeNodeBase::make_node(-width/2, -height/2, width, height, golquadtreenode_ptr());
}

golquadtree::~golquadtree()
{
}

coord_t golquadtree::width()
{
    return _root->width();
}

coord_t golquadtree::height()
{
    return _root->height();
}

void golquadtree::set(coord_t x, coord_t y)
{
    _root->set(x, y);
}

void golquadtree::unset(coord_t x, coord_t y)
{
    _root->unset(x, y);
}

bool golquadtree::get(coord_t x, coord_t y)
{
    return _root->get(x, y);
}


void golquadtree::set(point_t cell)
{
    _root->set(cell.x, cell.y);
}

void golquadtree::unset(point_t cell)
{
    _root->unset(cell.x, cell.y);
}

bool golquadtree::get(point_t cell)
{
    return _root->get(cell.x, cell.y);
}

void golquadtree::dump() const
{
    _root->dump();
}

void golquadtree::nextGeneration(boost::unordered_set<point_t> &new_cells, boost::unordered_set<point_t> &dead_cells)
{
    _root->nextGeneration(new_cells, dead_cells);

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


GolQuadtreeNodeLeaf::GolQuadtreeNodeLeaf(coord_t x, coord_t y, coord_t width, coord_t height, boost::weak_ptr<GolQuadtreeNodeBase> parent)
    : GolQuadtreeNodeBase( x, y, width, height, parent)
{
}

GolQuadtreeNodeLeaf::~GolQuadtreeNodeLeaf()
{
}

void GolQuadtreeNodeLeaf::set(coord_t x, coord_t y)
{
    _grid[x - this->x() + MARGIN][y - this->y() + MARGIN] = true;
    _cells++;
}

bool GolQuadtreeNodeLeaf::unset(coord_t x, coord_t y)
{
    _grid[x - this->x() + MARGIN][y - this->y() + MARGIN] = false;
    _cells--;

    return (_cells == 0);
}

bool GolQuadtreeNodeLeaf::get(coord_t x, coord_t y) const
{
    if (!contains(x, y))
        return parent()->get(x, y);

    return _grid[x - this->x() + MARGIN][y - this->y() + MARGIN];
}


bool GolQuadtreeNodeLeaf::isLeaf() const
{
    return true;
}

void GolQuadtreeNodeLeaf::dump() const
{
    cout << "node (lvl:" << level() << ", cells: " << _cells << ") [" << x() << "," << y() << "," << width() << "," << height() << "]" << endl;
}

void GolQuadtreeNodeLeaf::getOverlappedNodesAt(coord_t x, coord_t y, std::vector<golquadtreenode_ptr>& nodes)
{
    assert (contains_with_margin(x,y) );
    nodes.push_back(shared_from_this());
}

void GolQuadtreeNodeLeaf::nextGeneration(boost::unordered_set<point_t>& new_cells, boost::unordered_set<point_t>& dead_cells)
{
    coord_t i, j;

    // fill borders
    for (i = 0 ; i < MARGIN; ++i)
    {
        for (j = 0 ; j < height()+2*MARGIN; ++j)
        {
            _grid[i][j] = get( x()+i-MARGIN, y()+j-MARGIN );
            _grid[i+width()][j] = get( x()+width()+i-MARGIN, y()+j-MARGIN );
        }
    }
    for (i = 0 ; i < width()+2*MARGIN; ++i)
    {
        for (j = 0 ; j < MARGIN; ++j)
        {
            _grid[i][j] = get( x()+i-MARGIN, y()+j-MARGIN );
            _grid[i][j+height()] = get( x()+i-MARGIN, y()+height()+j-MARGIN );
        }
    }

    for (i = 0 ; i < width(); ++i)
    {
        for (j = 0 ; j < height(); ++j)
        {
            bool cell = _grid[i+MARGIN][j+MARGIN];
            if (cell)
            {
                const coord_t neighbours = countNeighbours(i+MARGIN, j+MARGIN);

                // Any live cell with fewer than two live neighbours dies, as if caused by under-population.
                // Any live cell with more than three live neighbours dies, as if by overcrowding.
                if (neighbours < 2 || neighbours > 3)
                {
                    if (dead_cells.insert( point_t(i+x(), j+y()) ).second)
                    {
                        // cout << " => " << i+x() << "," << j+y() << " is dead" << endl;
                    }
                }

                // FIXME: find better names
                // FIXME: make an algo and refactor with countNeighbours
                for (coord_t i1 = i-1 ; i1 <= i+1 ; ++i1)
                {
                    for (coord_t j1 = j-1 ; j1 <= j+1 ; ++j1)
                    {
                        // Any dead cell with exactly three live neighbours becomes a live cell
                        bool cell = _grid[i1+MARGIN][j1+MARGIN];
                        if (i1!=i && j1 != j && !cell && countNeighbours(i1+MARGIN, j1+MARGIN) == 3)
                        {
                            if (new_cells.insert( point_t(i1+x(), j1+y()) ).second)
                            {
                             //   cout << " => " << i1+x() << "," << j1+y() << " is born" << endl;
                            }
                        }
                    }
                }
            }
        }
    }

    // TODO: new/dead cells within the tile can be updated directly
}

coord_t GolQuadtreeNodeLeaf::countNeighbours(coord_t x, coord_t y) const
{
    coord_t neighbours = 0;

    for (coord_t i = x-1 ; i <= x+1 ; ++i)
    {
        for (coord_t j = y-1 ; j <= y+1 ; ++j)
        {
            const bool cell = _grid[i][j];
            if ( ! (i==x && j== y) && cell)
            {
                neighbours++;
            }
        }
    }

    return neighbours;
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

GolQuadtreeNode::GolQuadtreeNode(coord_t x, coord_t y, coord_t width, coord_t height, golquadtreenode_weakptr parent)
    : GolQuadtreeNodeBase(x, y, width, height, parent)
{
}

GolQuadtreeNode::~GolQuadtreeNode()
{
}
