#include "genericgameoflifepatternmodel.h"

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <exception>

#include "boost/function.hpp"
#include "boost/bind.hpp"
#include <boost/format.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/irange.hpp>
#include <boost/filesystem.hpp>

using namespace std;
using namespace std::tr1;
using namespace boost;
using namespace boost::filesystem;


GenGolPattern::GenGolPattern(const string& name, coord_t width, coord_t height, const string &rle_pattern)
    : _name(name), _width(width), _height(height), _rle_pattern(rle_pattern)
{
}

GenGolPattern::~GenGolPattern()
{
}

coord_t GenGolPattern::width() const
{
    return _width;
}

coord_t GenGolPattern::height() const
{
    return _height;
}

const string GenGolPattern::name() const
{
    return _name;
}

const string GenGolPattern::rle_string() const
{
    return str( format("x=%1%,y=%2%\n%3%") % _width %  _height % _rle_pattern) ;
}

vector<point_t> GenGolPattern::cells() const
{
    vector<point_t> pattern;

    // read the RLE string
    const regex expression( "(([[:digit:]])*)([ob\\$]{1})");
    string::const_iterator start = _rle_pattern.begin();
    string::const_iterator end = _rle_pattern.end();
    match_results<string::const_iterator>  match;

    // iterate over all matches
    match_flag_type flags = match_default | match_not_null;
    for (coord_t x = 0, y = 0;
         regex_search(start, end, match, expression, flags);
         start = match[0].second )
    {

        const int occurences = match[1].length() > 0 ? lexical_cast<int>( match[1] ) : 1;
        const char c = (static_cast<string>(match[3])).at(0);
        switch (c)
        {
        case '$':
            x = 0;
            y++;
            break;
        case 'o':
            coord_t limit = x + occurences;
            for (; x < limit; ++x )
            {
                pattern.push_back(make_pair(x,y));
            }
            break;
        case 'b':
            x += occurences;
            break;
        }

    }

    return pattern;
}



bool IsJunk (char c) {  return (isspace(c) || c == '!'); }

GenGolPattern::GenGolPattern(const filesystem::path& filename)
    :  _name("invalid"), _width(0), _height(0)
{
    ifstream rlefile(filename.c_str(), ifstream::in);
    if (!rlefile.is_open())
        throw gengol_rle_exception();

    // Skip comments and read header
    string buffer;
    do
    {
        getline (rlefile,buffer);
    } while (rlefile.good() && buffer[0] == '#' );

    regex reg( "^[[:space:]]*"                     // possibly leading whitespace
               "x[[:space:]]*=[[:space:]]*(\\d+)"  // x = number
               "[[:space:]]*,[[:space:]]*"         // ,
               "y[[:space:]]*=[[:space:]]*(\\d+)"  // y = number
               "([[:space:]]*,[[:space:]]*rule[[:space:]]*=[[:space:]]*B(\\d+)/S(\\d+)(:T(\\d+),(\\d+))?)?" // , rule = B3678/S34678
               "\r?$"
               );

    smatch match;
    if (!regex_match(buffer, match, reg))
        throw gengol_rle_exception();

    const coord_t width = lexical_cast<int>( match[1] );
    const coord_t height = lexical_cast<int>( match[2] );

    _name =  filename.stem().c_str();
    _width = width;
    _height = height;
    _rle_pattern = string((istreambuf_iterator<char>(rlefile)), istreambuf_iterator<char>());
    _rle_pattern.erase( remove_if( _rle_pattern.begin(),
                             _rle_pattern.end(),
                             IsJunk),
                  _rle_pattern.end());
}


void GenGolPatternModel::addFile(const filesystem::path &filename)
{
    GenGolPatternPtr pattern = GenGolPatternPtr(new GenGolPattern(filename));
    _model.push_back(pattern);
}

size_t GenGolPatternModel::size() const
{
    return _model.size();
}

const GenGolPatternPtr GenGolPatternModel::get(size_t index) const
{
    if (index < _model.size())
    {
        return _model[index];
    }

    return _null_pattern;
}


class predicate
{
public:
   predicate(const std::string& name) : _name(name) {}
   bool operator()(GenGolPatternPtr pattern) { return pattern->name() == _name; }

private:
   const std::string _name;
};

const GenGolPatternPtr GenGolPatternModel::get(const std::string& name) const
{
    predicate tester(name);

    vector<GenGolPatternPtr>::const_iterator it =
            find_if (_model.begin(),
                     _model.end(),
                     tester );
    if (it != _model.end())
    {
        return (*it);
    }

    return _null_pattern;
}



GenGolPatternModel::GenGolPatternModel()
    : _null_pattern( new GenGolPattern("<null>", 0, 0, "") )
{
    _model.push_back( GenGolPatternPtr(new GenGolPattern("Cell", 1, 1, "o") ) );
}

void GenGolPatternModel::addDirectory(const filesystem::path &directory, bool recursive)
{
    if (is_directory(directory))
    {
        directory_iterator end_itr;
        for (directory_iterator it(directory); it != end_itr; ++it)
        {
            path p(*it);
            if ( p.extension() == ".rle" )
            {
                try
                {
                    _model.push_back( GenGolPatternPtr(new GenGolPattern(*it)) );
                } catch(...)
                {
                    std::cout << "got exception while reading RLE file " << p << std::endl;
                }
            }
        }
    }
}
