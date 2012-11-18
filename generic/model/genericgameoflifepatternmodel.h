#ifndef GENERICGAMEOFLIFEPATTERNMODEL_H
#define GENERICGAMEOFLIFEPATTERNMODEL_H

#include <string>
#include <vector>
#include <exception>
#include <map>
#include <memory>
#include <tr1/memory>

#include <boost/utility.hpp>
#include <boost/filesystem.hpp>

class GenGolPattern;
typedef std::tr1::shared_ptr<const GenGolPattern> GenGolPatternPtr;


typedef unsigned int coord_t;
typedef std::pair<coord_t, coord_t> point_t;

class gengol_rle_exception : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Error when parsing Game Of life's RLE file";
    }
};

class GenGolPatternModel
{
public:
    GenGolPatternModel();

     void addDirectory(const boost::filesystem::path& directory, bool recursive = false);
     void addFile(const boost::filesystem::path& filename);

     size_t size() const;
     const GenGolPatternPtr get(size_t index) const;
     const GenGolPatternPtr get(const std::string& name) const;

private:
     std::vector<GenGolPatternPtr> _model;
     const GenGolPatternPtr _null_pattern;
};

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

#endif // GENERICGAMEOFLIFEPATTERNMODEL_H
