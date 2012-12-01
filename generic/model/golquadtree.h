#ifndef GOLQUADTREE_H
#define GOLQUADTREE_H

#include "goltypes.h"

#include <cstring>
#include <vector>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/unordered_set.hpp>

namespace gol
{
class GolQuadtreeNodeBase;
class GolQuadtreeNodeLeaf;

typedef boost::shared_ptr<GolQuadtreeNodeBase> golquadtreenode_ptr;
typedef boost::weak_ptr<GolQuadtreeNodeBase> golquadtreenode_weakptr;
typedef boost::shared_ptr<GolQuadtreeNodeLeaf> golquadtreenodeleaf_ptr;



// FIXME: use PimpL idion to hide GolQuadtreeNodeBase class from clients.
class GolQuadtreeNodeBase : public boost::enable_shared_from_this<GolQuadtreeNodeBase>, private boost::noncopyable
{
public:

    GolQuadtreeNodeBase(coord_t x, coord_t y, coord_t width, coord_t height, golquadtreenode_weakptr parent);
    virtual ~GolQuadtreeNodeBase();

    static golquadtreenode_ptr make_node(coord_t x, coord_t y, coord_t width, coord_t height, golquadtreenode_weakptr parent);

    coord_t x()     const { return _x; }
    coord_t y()     const { return _y; }
    coord_t width()  const { return _width; }
    coord_t height() const { return _height; }
    coord_t level()  const { return _level; }
    golquadtreenode_ptr parent()   const { return _parent.lock(); }
    bool isRoot()   const { return _parent._empty(); }

    virtual void set(coord_t x, coord_t y) = 0;
    virtual bool unset(coord_t x, coord_t y) = 0;
    virtual bool get(coord_t x, coord_t y) const = 0;
    virtual bool isLeaf() const = 0;
    virtual void dump()  const = 0;
    virtual void nextGeneration(boost::unordered_set<point_t>& new_cells, boost::unordered_set<point_t>& dead_cells) = 0;

    bool contains(coord_t x, coord_t y) const;
    bool contains_with_margin(coord_t x, coord_t y) const;
    virtual void getOverlappedNodesAt(coord_t x, coord_t y, std::vector<golquadtreenode_ptr>& nodes) = 0;

protected:
    coord_t _cells;
    static const coord_t MARGIN = 2;
private:
    coord_t _x;
    coord_t _y;
    coord_t  _width;
    coord_t  _height;
    coord_t _level;                    // NOTE: remove me if not needed
    golquadtreenode_weakptr _parent;  // NOTE: remove me if not needed
};

// FIXME: use PimpL idion to hide GolQuadtreeNode class from clients.
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
    coord_t countNeighbours(coord_t x, coord_t y) const;

    bool  _grid[MIN_WIDTH+2*MARGIN][MIN_HEIGHT+2*MARGIN];
};


class golquadtree : private boost::noncopyable
{
public:
    golquadtree(coord_t  width, coord_t  height);
    ~golquadtree();

    coord_t width();
    coord_t height();

    void set(coord_t x, coord_t y);
    void unset(coord_t x, coord_t y);
    bool get(coord_t x, coord_t y);

    void set(point_t cell);
    void unset(point_t cell);
    bool get(point_t cell);

    void dump()  const;

    // FIXME: find a better alternative than passing references to vector.
    void nextGeneration(boost::unordered_set<point_t>& new_cells, boost::unordered_set<point_t>& dead_cells);

private:
    golquadtreenode_ptr _root;
};

}
#endif // GOLQUADTREE_H
