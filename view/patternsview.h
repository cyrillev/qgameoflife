#ifndef PATTERNVIEW_H
#define PATTERNVIEW_H

#include <QTreeView>

#include "model/gameoflifepatternmodel.h"

class PatternsView : public QTreeView
{
    Q_OBJECT
public:
    explicit PatternsView(QWidget *parent = 0);

signals:

public slots:
    void currentChanged(const QModelIndex & current, const QModelIndex & previous );

signals:
    void selectedPatternChanged(const GenGolPatternPtr& name);

protected:
    virtual void startDrag(Qt::DropActions supportedActions);
};

#endif // PATTERNVIEW_H
