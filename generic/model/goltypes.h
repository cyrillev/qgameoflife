#ifndef GOLTYPES_H
#define GOLTYPES_H

#include <boost/functional/hash.hpp>

namespace gol
{
typedef long long int coord_t;

// NOTE: is there any STL/Boost class ?
struct point_t
{
    point_t(coord_t x_, coord_t y_) : x(x_), y(y_) {}
    coord_t x;
    coord_t y;
};

inline bool operator==(point_t const& p1, point_t const& p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}


inline std::size_t hash_value(point_t const& p)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, p.x);
    boost::hash_combine(seed, p.y);
    return seed;
}

}


typedef std::pair<gol::point_t, bool> status_t;

inline bool operator==(status_t const& status1, status_t const& status2)
{
    gol::point_t p1 = status1.first;
    gol::point_t p2 = status2.first;
    bool s1 = status1.second;
    bool s2 = status2.second;
    return p1.x == p2.x && p1.y && p2.y && s1 == s2;
}

inline std::size_t hash_value(status_t const& status)
{
    gol::point_t point = status.first;
    std::size_t seed = 0;
    boost::hash_combine(seed, point.x);
    boost::hash_combine(seed, point.y);
    boost::hash_combine(seed, status.second);
    return seed;
}
#endif // GOLTYPES_H
