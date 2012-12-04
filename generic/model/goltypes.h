#ifndef GOLTYPES_H
#define GOLTYPES_H

#include <boost/functional/hash.hpp>

namespace gol
{
typedef long long int coord_t;

/**
 * The point_t structure defines the x and y coordinates of a point.
 * NOTE: is there any STL/Boost class that represents a point?
*/
struct point_t
{
    point_t(coord_t x_, coord_t y_) : x(x_), y(y_) {}


    coord_t x; /** The x-coordinate of the point. **/
    coord_t y; /** The y-coordinate of the point. **/
};


/**
 * The comparison operator of two points.
 * two points are equals if they have the same x and y coordinates.
*/
inline bool operator==(point_t const& p1, point_t const& p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}


/**
 * A custom implementation of hash_value for point_t
 * this implementation is required by boost::unordered_set
*/
inline std::size_t hash_value(point_t const& p)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, p.x);
    boost::hash_combine(seed, p.y);
    return seed;
}

}

#endif // GOLTYPES_H
