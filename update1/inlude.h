#ifndef INLUDE_H
#define INLUDE_H
#include <QMetaType>                            //自定义参数类型的头文件----***
#include<QString>

typedef struct  {

      char Rxbuf[200];
      // QString id;

     //  QList<int> m_socketList;

      int last_idnum[100];

}STUD;

extern  STUD  stu;
 Q_DECLARE_METATYPE(STUD);           //要调用Q_DECLARE_METATYPE，向QT声明这个结构体----***
#endif // INLUDE_H
