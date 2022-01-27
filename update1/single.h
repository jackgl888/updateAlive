#ifndef single_H
#define single_H
#include<QMutexLocker>
//#include <QSharedPointer>
#include <QTreeWidgetItem>

#include "parentDevice.h"
#include "slavepower.h"
#include <QMutex>
#include "widget.h"
#include "databasesql.h"
#include "struct.h"
#include <QStandardItemModel>
#define   MID_MACHINE         1  //中位机
#define   NEEDLEBED           2//针床下位机
#define   PARENTDEVICE         3 //父对象设备
#define   TEMPBOARD           4// 温度采样板
#define   SLAVEBOARD          5 //电源从板




class datarecv;
class midmachine;
class single: public QObject
{
    Q_OBJECT
public:
    static   single * m_Instance()
    {
        if(m_pInstance == NULL)
        {
            QMutexLocker  mlocker(&m_Mutex);  //
            if(m_pInstance == NULL)
            {
                m_pInstance = new  single();
            }
        }
        return  m_pInstance;
    }
    static QMutex  m_Mutex;    //保护ip读写


    //数据库操作部分函数

    static  void quiteSql();
    bool insertData(const QString sqlStr, const float *val ,const int row, const int columns);   //插入数据
    bool updateData(const QString sqlStr);    //更新数据
    bool selectData(const QString sqlStr, QList<QMap<QString,QVariant>> &valuesList);  //查询数据
    bool deleteData(const QString sqlStr);  //删除 数据
    bool openDatabase(const QString dbFileName);  //打开数据库
    bool initDatabase(const QString sqlStr);   //初始化数据库


private:
    single();
    ~single();
    //   single(const single&){}
    // single& operator ==(const   single&){}

    static single* m_pInstance;  //类的指针


    static QSqlDatabase database;  //全局数据库
    static QString dbPath;
    static QString dbFile;
    static QString dbName;
    static QString dbUser;
    static QString dbPasswd;




public:
    class CGarbo    //析构m_pInstance 指针的类
    {
    public:
        ~CGarbo() {
            if(m_pInstance!=NULL)
            {

                delete  m_pInstance;
                m_pInstance = NULL;
            }
        }
    };
    static CGarbo  m_Garbo;

};



#endif
