#ifndef CELLPAINTER_H
#define CELLPAINTER_H

#include "generic/model/genericgameoflifepatternmodel.h"

class QIcon;


class CellPainter
{
public:
    static QIcon makeIcon(gol::GenGolPatternPtr pattern, size_t size, size_t margin);
};

#endif // CELLPAINTER_H
