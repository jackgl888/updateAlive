#ifndef UPDDATADEAL_H
#define UPDDATADEAL_H
#include<QRunnable>
#include <QObject>
#include  <QFile>
#include  <QDataStream>
#include"single.h"
#include  <QMutex>
#include <QMutexLocker>

#define  WAIT_HEAD 0x00
#define  WAIT_LEN  0x01
#define  WAIT_MSG  0x02
#define  BUFF_LEN     2000
class updDataDeal : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit updDataDeal(QObject *parent = nullptr);
     void run();
signals:
        void msgToUi(QString & ip);//(version1,version2,version3,中位机IP，针床/电源IP，从节点IP)
        void addMidmachineNode(QString  ip,uchar bednum,uchar tempnum,uchar powernum,uchar slavepowernum);//ip,针床num,温度num，电源num，电源板num
        void refreshVerion(QString ip,QString version,QStringList bedList,QStringList tempList,
                           QStringList powerList,QStringList slaveList);

       void  sendRunResult(const QStringList  list);   //返回运行结果
private slots:

          void  recvDataMethod( const uchar *data);
          void  cmdConnectAnalysis(const uchar *data);  //解析联机指令

 private:
          QMutex  mtx;
          uchar recvBuf[300];
          uint recv_cnt;		  //当前通讯口第几个字符   三个通讯口串口1 2 3
          uchar head_cnt;
          uchar wait;
          ushort  length;
          QString  m_addr;
};

#endif // UPDDATADEAL_H
