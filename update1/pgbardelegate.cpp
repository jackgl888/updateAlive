#include "pgbardelegate.h"

pgbarDelegate::pgbarDelegate(ushort barMaxVal,uchar columnIndex,QObject *parent):QStyledItemDelegate(parent)
{
    m_barMaxVal = barMaxVal-1;
    m_columnIndex = columnIndex;

}



pgbarDelegate::~pgbarDelegate()
{

}

void pgbarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid()&&(index.column()==m_columnIndex))
    {
        QStyleOptionProgressBar  bar;
        bar.rect = option.rect;

        bar.progress = index.data().toInt();
        bar.maximum = m_barMaxVal;
        bar.minimum = 0;

        if((bar.progress*100)/m_barMaxVal<=100)
        bar.text = QString::number((bar.progress*100)/m_barMaxVal)+"%";

        bar.textVisible = true;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar,&bar,painter);

   }
    else
        QStyledItemDelegate::paint(painter,option,index);
}


//

