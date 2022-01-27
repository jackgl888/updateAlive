#include "widget.h"
#include <QApplication>
#include <QTextCodec>
#include <QSplitter>
#include <QListWidget>
#include "projectwidget.h"
#include<QDebug>
#include "login.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QDebug>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    // qRegisterMetaType<STUD>("STUD");
    QFont font("AR PL KaitiM GB",9);
    a.setFont(font);

  // qDebug()<<"main thread:"<<QThread::currentThreadId();
    Mlogin loginDlg;
    projectWidget  w;
//  Widget w ;
//  w.show();
  w.show();
  return a.exec();
#if 0
        if (loginDlg.exec() == QDialog::Accepted)
       {
             Widget w ;
             w.show();

             return a.exec();
        }
        else
        {
            return 0;
       }

#endif

}


/***********
 *  v2.02
 * *************/

