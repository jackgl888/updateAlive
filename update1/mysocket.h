#ifndef MYSOCKET_H
#define MYSOCKET_H
//#include <QtNetwork>
#include <QTcpSocket>
#include<QMutexLocker>
#include<QTreeWidgetItem>
#include "serverthread.h"
#include "widget.h"
#include <QObject>
#include <QTimer>
#include "struct.h"
class QTimer;

class MySocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MySocket( QString mcIp, uint mcPort , updateTarget target,    QObject *parent = Q_NULLPTR);
    ~MySocket();


    ushort CRC16_Modbus(const uchar *_pBuf, ushort _usLen);  //crc校验
    bool net_packet_checksum(const uchar *pBuff, ushort len);//和校验
    ushort checksum(const uchar *pBuff, ushort len);

signals:


    void  sigRunMsgToUi(QString ip, ushort  cmd,QStringList  msg, ushort value);//运行信息返回 UI

private slots:
    void timeoutMethod(void);

    void slotConnected();
    void slotDisconnected();

    void recvData(void);
    void recvDataMethod(const uchar *data);

    void  slotDataSent(ushort cmd,uchar type,QVariant variant); //
    void  cmdConnectTarget(uchar target,  uchar addr);  //pc与target 建立 联机
    void  cmdAppJumpBoot(uchar target,  uchar addr);  //给中位机发送跳转boot
    void  bootEraseAppSectors(uchar target,  uchar addr);   //擦除指定扇区
    void  bootWriteAppdata(uchar target,  uchar addr,ushort txframeIndex);//下发中位机boot升级数据
    void  bootJumpToApp(uchar target,  uchar addr); //    中位机boot跳转到app运行

    void  readBackLcformerApp(void);  //读取回之前的下位机APP进行备份
    void  backUpFile(QString  filename);     //读回升级备份

    void tcpSendData(uchar *txBuf,const ushort len);   //发送数据



private:

     updateTarget  m_target;   //目的设备
     uchar  m_box;     //正在升级的下位机地址
     uchar  lcAddrIndex;    //地址序列
      uchar resendTimes ;    //重发次数

      QTimer * m_timer;   //
    bool   updateStart;
    uchar recvbuf[RECV_BUFF_LEN ];  //接收数据
    uchar  txBuf[SEND_BUFF_LEN  ];

    enum
    {
        reConnect = 0x01,  //掉线重联
        targetConnect,  //与目标板建立联机,,,,
        appJumpBoot,   //app跳转到boot
        eraseApp, //扇区擦除
        appDataWrite,  //下发app数据
        bootJumpApp  , //boot跳转app
        bootBackupApp     //boot读取之前的app备份
    }m_update;

    QString hostAddr;  //
    QString  getIp;
    uint hostPort;
    QList<uchar>m_powerList; //要升级的电源的地址队列

    uchar  m_allData[APP_SIZE ];
    ushort m_totalSdtimes;  //总的1024包发送次数
    qint64 m_totalBytes;  //数据总字节数
    ushort m_lastbytes;  //数据不满1024字节数

    QList<txNode*>txList;

    quint64 formerAppLen;   //之前的app长度
    uchar   formerAppData[APP_SIZE ];

   ushort   txIndex;

   struct  checkBuf
   {
        ushort txTimes;
        ushort rxTimes;
        ushort checkedRxtimes;
   }m_checkBuf;



};

#endif // MYSOCKET_H
