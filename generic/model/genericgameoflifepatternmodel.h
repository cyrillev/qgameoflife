#ifndef GENERICGAMEOFLIFEPATTERNMODEL_H
#define GENERICGAMEOFLIFEPATTERNMODEL_H

#include "goltypes.h"
#include "golmodel.h"

#include <string>
#include <vector>
#include <exception>
#include <map>
#include <memory>
#include <tr1/memory>

#include <boost/utility.hpp>
#include <boost/filesystem.hpp>

namespace gol
{
class GenGolPattern;
typedef std::tr1::shared_ptr<const GenGolPattern> GenGolPatternPtr;



class gengol_rle_exception : public std::exception
{
    virtual const char* what() const throw();
};



/**
 * This class maintains a collection of Game of Life's patterns
 * Patterns are loaded from RLE Game of Life's files
 */
class GenGolPatternModel
{
public:
    GenGolPatternModel();

    /**
     * load the given file and add the pattern to the collection.
     * may throw gengol_rle_exception
     */
    void loadFile(const boost::filesystem::path& filename);

    /**
     * load all RLE files within the directory and sub-directories (if recursive)
     * and add the patterns to the collection.
     * may throw gengol_rle_exception
     */
     void LoadDirectory(const boost::filesystem::path& directory, bool recursive = false);

     /** return the size of the collection of patterns */
     size_t size() const;

     /** return the pattern at the given index (or a null pattern if index is out-of-bound) */
     const GenGolPatternPtr get(size_t index) const;

     /** find the pattern with the given name (or a null pattern if index is out-of-bound) */
     const GenGolPatternPtr get(const std::string& name) const;

private:
     std::vector<GenGolPatternPtr> _model;
     const GenGolPatternPtr _null_pattern;
};


/**
 * This class represents a Game of Life's patterns
 */
class GenGolPattern : boost::noncopyable
{
public:
    GenGolPattern(const std::string& name, coord_t width, coord_t height, const std::string &rle_pattern);
    GenGolPattern(const boost::filesystem::path& filename);
    ~GenGolPattern();

    const std::string name() const;
    coord_t width() const;
    coord_t height() const;
    std::vector<point_t> cells() const;
    const std::string rle_string() const;

protected:
    std::string _name;
    coord_t _width;
    coord_t _height;
    std::string _rle_pattern;
};

}
#endif // GENERICGAMEOFLIFEPATTERNMODEL_H
