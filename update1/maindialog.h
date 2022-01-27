#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QObject>
#include <QDialog>
#include  <QTabWidget>
#include "widget.h"
class mainDialog : public QDialog
{
    Q_OBJECT
public:
    explicit mainDialog(QWidget *parent = nullptr);
   ~mainDialog();

signals:

public slots:

private:
      QTabWidget  *funcModules;  //功能模块布局容器

      Widget * m_widget;
};

#endif // MAINDIALOG_H
