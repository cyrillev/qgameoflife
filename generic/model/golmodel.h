#ifndef GOLMODEL_H
#define GOLMODEL_H

#include <vector>

#include <boost/utility.hpp>
#include <boost/signals2.hpp>
#include <boost/signals2/connection.hpp>
#include <boost/unordered_set.hpp>

typedef long long int coord_t;
typedef std::pair<coord_t, coord_t> point_t;


class GolModel : boost::noncopyable
{
public:
    enum Status
    {
        DEAD,
        ALIVE
    };

    typedef std::pair<point_t, GolModel::Status> status_t;

    GolModel();

    Status data(const point_t &point) const;
    Status data(coord_t x, coord_t y) const;

    void setData(const point_t& point, Status value);
    void setData(coord_t x, coord_t y, Status value);

    void nextGeneration();


    // connection to change events
    typedef boost::signals2::signal<void (const point_t, Status)>  signal_t;
    typedef boost::signals2::connection  connection_t;
    connection_t connect(signal_t::slot_function_type subscriber);
    void disconnect(connection_t subscriber);

private:
    unsigned int numberOfNeighbours(const point_t &point);

    boost::unordered_set<point_t>  _cells;
    typedef boost::unordered_set<point_t>::iterator iterator;
    typedef boost::unordered_set<point_t>::const_iterator const_iterator;

    signal_t  _signalCellChanged;
};

#endif // GOLMODEL_H
