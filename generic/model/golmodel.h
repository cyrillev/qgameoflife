#ifndef GOLMODEL_H
#define GOLMODEL_H

#include "goltypes.h"
#include "golsetmodel.h"

#include <boost/utility.hpp>
#include <boost/signals2.hpp>
#include <boost/signals2/connection.hpp>
#include <boost/unordered_set.hpp>

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

    bool data(const point_t &point) const;
    bool data(coord_t x, coord_t y) const;

    void setData(const point_t& point, bool value);
    void setData(coord_t y, coord_t y, bool value);

    void nextGeneration();


    // connection to change events
    typedef boost::signals2::signal<void (const point_t, bool)>  signal_t;
    typedef boost::signals2::connection  connection_t;
    connection_t connect(signal_t::slot_function_type subscriber);
    void disconnect(connection_t subscriber);

private:
    signal_t  _signalCellChanged;
    boost::scoped_ptr<gol::GolSetModel> _implementation;
};

};
#endif // GOLMODEL_H
