#include "patternsview.h"


#include <QtCore>
#include <QtGui>


PatternsView::PatternsView(QWidget *parent) :
    QTreeView(parent)
{
    setDropIndicatorShown(false);
}

void PatternsView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QVariant variant = current.data(GameOfLifePatternModel::GenGolPatternRole);
    GameOfLifePatternWrapper wrapper = variant.value<GameOfLifePatternWrapper>();

    emit selectedPatternChanged( wrapper );
}


/** @short Reimplemented to show custom pixmap during drag&drop

  Qt's model-view classes don't provide any means of interfering with the
  QDrag's pixmap so we just rip off QAbstractItemView::startDrag and provide
  our own QPixmap.
*/

void PatternsView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectedIndexes();
    // indexes for column 0, i.e. subject
    QModelIndexList indexesCol0;

    for (int i = indexes.count() - 1; i >= 0; --i) {
        if (!(model()->flags(indexes.at(i)) & Qt::ItemIsDragEnabled))
            indexes.removeAt(i);
        else if (indexes.at(i).column() == 0)
            indexesCol0.prepend(indexes.at(i));
    }
    if (indexes.count() > 0) {
        QMimeData *data = model()->mimeData(indexes);
        if (!data)
            return;

        // use screen width and itemDelegate()->sizeHint() to determine size of the pixmap
        int screenWidth = QApplication::desktop()->screenGeometry(this).width();
        int maxWidth = qMax(400, screenWidth / 4);
        QSize size(maxWidth, 0);

        QStyleOptionViewItem opt;
        opt.initFrom(this);
        opt.rect.setWidth(maxWidth);
        opt.rect.setHeight(itemDelegate()->sizeHint(opt, indexesCol0.at(0)).height());
        size.setHeight(indexesCol0.count() * opt.rect.height());
        // State_Selected provides for nice background of the items
        opt.state |= QStyle::State_Selected;

        // paint list of selected items using itemDelegate() to be consistent with style
        QPixmap pixmap(size);
        pixmap.fill(Qt::transparent);
        QPainter p(&pixmap);


        /*
         *for (int i = 0; i < indexesCol0.count(); ++i) {
            opt.rect.moveTop(i * opt.rect.height());
            itemDelegate()->paint(&p, opt, indexesCol0.at(i));
        }
*/
        QDrag *drag = new QDrag(this);
        drag->setPixmap(pixmap);
        drag->setMimeData(data);
        drag->setHotSpot(QPoint(0, 0));

        Qt::DropAction dropAction = Qt::IgnoreAction;
        if (defaultDropAction() != Qt::IgnoreAction && (supportedActions & defaultDropAction()))
            dropAction = defaultDropAction();
        else if (supportedActions & Qt::CopyAction && dragDropMode() != QAbstractItemView::InternalMove)
            dropAction = Qt::CopyAction;
        if (drag->exec(supportedActions, dropAction) == Qt::MoveAction) {
            // QAbstractItemView::startDrag calls d->clearOrRemove() here, so
            // this is a copy of QAbstractItemModelPrivate::clearOrRemove();
            const QItemSelection selection = selectionModel()->selection();
            QList<QItemSelectionRange>::const_iterator it = selection.constBegin();

            if (!dragDropOverwriteMode()) {
                for (; it != selection.constEnd(); ++it) {
                    QModelIndex parent = it->parent();
                    if (it->left() != 0)
                        continue;
                    if (it->right() != (model()->columnCount(parent) - 1))
                        continue;
                    int count = it->bottom() - it->top() + 1;
                    model()->removeRows(it->top(), count, parent);
                }
            } else {
                // we can't remove the rows so reset the items (i.e. the view is like a table)
                QModelIndexList list = selection.indexes();
                for (int i = 0; i < list.size(); ++i) {
                    QModelIndex index = list.at(i);
                    QMap<int, QVariant> roles = model()->itemData(index);
                    for (QMap<int, QVariant>::Iterator it = roles.begin(); it != roles.end(); ++it)
                        it.value() = QVariant();
                    model()->setItemData(index, roles);
                }
            }
        }
    }
}
