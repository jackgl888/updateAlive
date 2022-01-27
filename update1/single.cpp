
#include"single.h"
single*  single::m_pInstance  = NULL;
single::CGarbo  single::m_Garbo;
QMutex  single::m_Mutex;
QSqlDatabase single::database;
QString single::dbPath = "./database";
QString single::dbFile = "databse.db";
QString  single :: dbName = "default";
QString  single :: dbUser = "root";
QString  single :: dbPasswd = "1234";





single::single()
{
    ;
}

single::~single()
{

}









//退出数据库位
void single::quiteSql()
{
    if(database.isOpen()){
        database.close();
        qInfo("Close database success.");
    }
    qInfo("[OK] AppDatabase quit.");
}


//插入数据操作
bool single::insertData(const QString sqlStr, const float *val, const int row, const int columns)
{
    QSqlQuery sql_query;
    //  static  uchar k;
    //     for(uchar j = 0;j<24;j++)
    //     {
    //          if((*(val+j)) == 0)
    //          {
    //              k++;
    //              if(k ==20)
    //              {
    //                   k = 0;
    //                    return 0;
    //              }

    //          }
    //     }
    //     k = 0;
    database.transaction();
    sql_query.prepare(sqlStr);

    for(int i = 0; i<=columns;i++)
    {
        if(i == 0)
            sql_query.bindValue(i,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz: "));
        else
        {
            sql_query.bindValue(i,*(val+i-1));
        }

        bool success= sql_query.exec();

        if(!success)
        {
            QSqlError lastError=sql_query.lastError();
            qDebug()<<lastError.driverText()<<QString(QObject::tr("插入失败"));
        }
    }
    database.commit();

#if 0
    qInfo("[SQL] %s",qPrintable(sqlStr));
    if(!sql_query.exec(sqlStr))
    {
        qWarning("[SQL] %s",qPrintable(sql_query.lastError().text()));
        return false;
    }
    database.commit();
#endif
    return true;
}


//更新数据
bool single::updateData(const QString sqlStr)
{
    QSqlQuery sql_query;
    qInfo("[SQL] %s",qPrintable(sqlStr));
    if(!sql_query.exec(sqlStr))
    {
        qWarning("[SQL] %s",qPrintable(sql_query.lastError().text()));
        return false;
    }
    return true;
}



//
bool single::selectData(const QString sqlStr, QList<QMap<QString, QVariant> > &valuesList)
{

}

bool single::deleteData(const QString sqlStr)
{

}

bool single::openDatabase(const QString dbFileName)
{

}


//
bool single::initDatabase(const QString sqlStr)
{

}






















