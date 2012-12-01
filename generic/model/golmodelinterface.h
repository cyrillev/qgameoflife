#ifndef GOLMODELINTERFACE_H
#define GOLMODELINTERFACE_H

#include "goltypes.h"

#include <boost/noncopyable.hpp>
#include <boost/unordered_set.hpp>

namespace gol
{
class GolModelInterface : private boost::noncopyable
{
public:
    virtual ~GolModelInterface() {}

    virtual bool get(const point_t &point) const = 0;
    virtual void set(const point_t &point) = 0;
    virtual void unset(const point_t &point) = 0;

    virtual void nextGeneration(boost::unordered_set<point_t> &new_cells, boost::unordered_set<point_t> &dead_cells) = 0;
};
}
#endif // GOLMODELINTERFACE_H
