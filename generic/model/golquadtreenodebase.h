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

}

#endif // GOLQUADTREENODEBASE_H
