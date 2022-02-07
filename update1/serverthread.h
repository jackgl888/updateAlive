#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H
#include "server.h"
#include <QThread>
#include "mysocket.h"
#include "myserial.h"
#include <QTimer>
#include <QTreeWidgetItem>
#include<QMutexLocker>


class MySocket;
class Myserial;

class serverThread : public QThread
{
    Q_OBJECT
public:
    serverThread(QString ip,  QString mcId, bool isNet, uint sockDesc,uint mcSockDesc, updateTarget target,   QObject *parent = Q_NULLPTR);
    ~serverThread();



signals:

    void  disconnectTCP(const QString &ip);

    void  sendUpdateCmd(ushort cmd,uchar type,QVariant para);   //转发升级数据
   void   sigSendRunningMsg(QString ip, ushort  cmd,QStringList  msg, ushort value);

private slots:   


    void  disconnectToHost(void);

    void  slotSendUpdateCmd(ushort cmd ,uchar type,QVariant); //转发升级命令

    void  slotSendRunningMsg(QString ip, ushort  cmd,QStringList  msg, ushort value);

private:

    void run(void);
    MySocket *m_socket;
     Myserial *m_serial;
    uint m_sockDesc;
    uint m_mcSockDesc;
    updateTarget  m_target;
    QString  m_ip;   //电脑IP
    QString  m_mcId;  //中位机IP
    bool m_isNet;
};

#endif // SERVERTHREAD_H
