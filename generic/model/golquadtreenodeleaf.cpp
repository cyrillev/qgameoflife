#include "golquadtreenodeleaf.h"

#include <assert.h>
#include <iostream>

using namespace gol;

GolQuadtreeNodeLeaf::GolQuadtreeNodeLeaf(coord_t x, coord_t y, coord_t width, coord_t height, boost::weak_ptr<GolQuadtreeNodeBase> parent)
    : GolQuadtreeNodeBase( x, y, width, height, parent)
{
    for (size_t i =0;i < MIN_WIDTH+2*MARGIN; ++i)
        for (size_t j =0;j < MIN_HEIGHT+2*MARGIN; ++j)
            _grid[i][j] = false;
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
    std::cout << "node (lvl:" << level() << ", cells: " << _cells << ") [" << x() << "," << y() << "," << width() << "," << height() << "]" << std::endl;
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
                        std::cout << " => " << i+x() << "," << j+y() << " is dead" << std::endl;
                    }
                }

                // FIXME: find better names
                // FIXME: make an algo and refactor with countNeighbours
                for (coord_t i1 = i-1 ; i1 <= i+1 ; ++i1)
                {
                    for (coord_t j1 = j-1 ; j1 <= j+1 ; ++j1)
                    {
                        // Any dead cell with exactly three live neighbours becomes a live cell
                        const bool cell = _grid[i1+MARGIN][j1+MARGIN];
                        const int neighbours = countNeighbours(i1+MARGIN, j1+MARGIN);
                        if (i1!=i && j1 != j && !cell && neighbours == 3)
                        {
                            if (new_cells.insert( point_t(i1+x(), j1+y()) ).second)
                            {
                                std::cout << " => " << i1+x() << "," << j1+y() << " is born" << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }

    // TODO: new/dead cells within the tile can be updated directly
}

size_t GolQuadtreeNodeLeaf::countNeighbours(coord_t x, coord_t y) const
{
    size_t neighbours = 0;

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

