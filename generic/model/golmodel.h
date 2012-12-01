#ifndef GOLMODEL_H
#define GOLMODEL_H

#include "goltypes.h"
#include "golmodelinterface.h"

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/signals2.hpp>
#include <boost/signals2/connection.hpp>

namespace gol
{
class GolModel : boost::noncopyable
{
public:
    enum Algorithm
    {
        UNORDERED_SET,
        QUAD_TREE
    };

    GolModel(Algorithm algo);

    bool get(const point_t &point) const;
    bool get(coord_t x, coord_t y) const;

    void set(const point_t& point);
    void set(coord_t y, coord_t y);

    void nextGeneration();

    // connection to change events
    typedef boost::signals2::signal<void (const point_t, bool)>  signal_t;
    typedef boost::signals2::connection  connection_t;
    connection_t connect(signal_t::slot_function_type subscriber);
    void disconnect(connection_t subscriber);

private:
    signal_t  _signalCellChanged;
    boost::scoped_ptr<gol::GolModelInterface> _implementation;
};

};
#endif // GOLMODEL_H
