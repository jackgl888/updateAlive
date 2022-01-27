#ifndef PGBARDELEGATE_H
#define PGBARDELEGATE_H

#include <QObject>
#include<QStyleOption>
#include<QStyledItemDelegate>
#include <QPainter>
#include <QProgressBar>
#include <QApplication>

class pgbarDelegate :  public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit pgbarDelegate(ushort barMaxVal,uchar columnIndex,QObject *parent = NULL);

public:
     pgbarDelegate();
     ~pgbarDelegate();
     void paint(QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index) const Q_DECL_OVERRIDE;
signals:

public slots:

private:
       ushort m_barMaxVal;
       uchar m_columnIndex;
};

#endif // PGBARDELEGATE_H
