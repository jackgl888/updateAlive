#ifndef MCONTENT_H
#define MCONTENT_H

#include <QWidget>
#include <QStackedWidget>
#include  <widget.h>
#include "pcbatest.h"
class mcontent : public QWidget
{
    Q_OBJECT
public:
    explicit mcontent(QWidget *parent = nullptr);
   ~mcontent();
       QStackedWidget * getStack();
signals:

public slots:

private:
    QStackedWidget  *stack;
    Widget * myWidget;
    pcbaTest  *myPcbaTest;


};

#endif // MCONTENT_H
