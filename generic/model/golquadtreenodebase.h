#ifndef GOLQUADTREENODEBASE_H
#define GOLQUADTREENODEBASE_H

#include "goltypes.h"

#include <vector>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/unordered_set.hpp>

namespace gol
{
class GolQuadtreeNodeBase;

typedef boost::shared_ptr<GolQuadtreeNodeBase> golquadtreenode_ptr;
typedef boost::weak_ptr<GolQuadtreeNodeBase>   golquadtreenode_weakptr;


/**
 * GolQuadtreeNodeBase is the base class for one of the quadtree's concreate node.
 * A concreate node is either a GolQuadtreeNode or a GolQuadtreeNodeLeaf
 *
 * A GolQuadtreeNodeBase covers a rectangular part of a 2D universe.
 *
 * this class is meant to be used by GolQuadtree
 *
*/
class GolQuadtreeNodeBase : public boost::enable_shared_from_this<GolQuadtreeNodeBase>, private boost::noncopyable
{
public:
    /** make a concreate GolQuadtreeNodeBase, depending on the given width and height */
    static golquadtreenode_ptr make_node(coord_t x, coord_t y, coord_t width, coord_t height, golquadtreenode_weakptr parent);

    /** FIXME remove me and replace me by GolModelInterface */
    virtual void set(coord_t x, coord_t y) = 0;
    virtual bool unset(coord_t x, coord_t y) = 0;
    virtual bool get(coord_t x, coord_t y) const = 0;
    virtual void nextGeneration(boost::unordered_set<point_t>& new_cells, boost::unordered_set<point_t>& dead_cells) = 0;

    /** destroy the GolQuadtreeNodeBase object */
    virtual ~GolQuadtreeNodeBase();

    /** getter methods */
    coord_t x()     const { return _x; }
    coord_t y()     const { return _y; }
    coord_t width()  const { return _width; }
    coord_t height() const { return _height; }
    coord_t level()  const { return _level; }
    golquadtreenode_ptr parent()   const { return _parent.lock(); }

    /** return true if this node is the root of the quadtree */
    bool isRoot()   const { return _parent._empty(); }

    /** return true if this node is a leaf of the quadtree */
    virtual bool isLeaf() const = 0;

    /** Dump the node to cout */
    virtual void dump()  const = 0;

    bool contains(coord_t x, coord_t y) const;
    bool contains_with_margin(coord_t x, coord_t y) const;
    virtual void getOverlappedNodesAt(coord_t x, coord_t y, std::vector<golquadtreenode_ptr>& nodes) = 0;

protected:
    /** Create a GolQuadtreeNodeBase that covers the given rectangular area and attached it to his parent */
    GolQuadtreeNodeBase(coord_t x, coord_t y, coord_t width, coord_t height, golquadtreenode_weakptr parent);

    coord_t _cells;
    static const coord_t MARGIN = 2;

private:
    coord_t _x;
    coord_t _y;
    coord_t  _width;
    coord_t  _height;
    coord_t _level;                   // NOTE: remove me if not needed
    golquadtreenode_weakptr _parent;  // NOTE: remove me if not needed
};

}

#endif // GOLQUADTREENODEBASE_H
