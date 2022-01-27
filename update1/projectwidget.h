#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H
#include"mcontent.h"
#include <QSplitter>
#include <QListWidget>
#include <QWidget>

class projectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit projectWidget(QWidget *parent = nullptr);
  ~projectWidget();
signals:

public slots:
private:

    QSplitter  *splitterMain;
    QListWidget *list;
    mcontent  *myContent;
};

#endif // PROJECTWIDGET_H
