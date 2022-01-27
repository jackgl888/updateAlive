#ifndef CBDELEGATE_H
#define CBDELEGATE_H
#include <QItemDelegate>
#include <QAbstractItemModel>
#include<QModelIndex>
#include <QPainter>
#include <QProgressBar>
#include<QStandardItemModel>
#include<QStyleOption>
#include<QStyledItemDelegate>


class probarDelegate : public QStyledItemDelegate
{
    Q_OBJECT
   public:
        probarDelegate(QObject  * parent = 0);
        ~probarDelegate();
        void paint(QPainter *painter,
                   const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;


   private:




};

#endif // CBDELEGATE_H
