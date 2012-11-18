#include "genericrleparser.h"

/*
RLE (*.RLE)

RLE ("Run Length Encoded") format is best for larger patterns. It is more cryptic, but not terribly so.

The first line is a header line, which has the form

x = m, y = n
where m and n are the dimensions of the pattern. RLE-writers should produce spacing as shown, but RLE-readers are best not to assume this spacing because it isn't clear that it was originally mandated.

The pattern itself begins on the next line and is encoded as a sequence if items of the form <run_count><tag>, where <tag> is one of the following

b = a dead cell
o = a live cell
$ = end of a line of the pattern

and <run_count> is the number of occurrences of <tag> and can be omitted if equal to 1. The last <run_count><tag> item is followed by a ! character. Dead cells at the end of a pattern line do not need to be encoded, nor does the end of the last line of the pattern. Whitespace is permitted between <run_count><tag> items (and between the last <run_count><tag> item and the following !), but except for carriage returns and line feeds this is not recommended. It is not permitted to place whitespace in the middle of a <run_count><tag> item, although it's a good idea for RLE-readers to be able to cope with this anyway.

Lines in the RLE file must not exceed 70 characters, but again it's a good idea for RLE-readers to be able to cope with longer lines. DOS, Unix and Mac newline conventions are all acceptable.

Anything after the final ! is ignored. It used to be common to put comments here (starting on a new line), but the usual method for adding comments is now by means of #C lines (see below).
*/


GenericRleParser::GenericRleParser(const std::string& filename)
{  string line;
    ifstream myfile ("example.txt");
    if (myfile.is_open())
    {
      while ( myfile.good() )
      {
        getline (myfile,line);
        cout << line << endl;
      }
      myfile.close();
    }

    else cout << "Unable to open file";
}
