#ifndef MYSERIAL_H
#define MYSERIAL_H
#include<QtSerialPort/qserialport.h>
#include<QtSerialPort/qserialportinfo.h>
#include<QTimer>
#include<QMutexLocker>
#include<QTreeWidgetItem>
#include "single.h"
#include "widget.h"
#include <QSemaphore>



class Myserial : public QSerialPort
{
    Q_OBJECT
public:
    explicit Myserial( QString ip, updateTarget target, QObject *parent = Q_NULLPTR);
    ~Myserial();

    void   Init_serial(QString ip);

    ushort CRC16_Modbus(const uchar *_pBuf, ushort _usLen);  //crc校验
    bool net_packet_checksum(const uchar *pBuff, ushort len);//和校验
    ushort checksum(const uchar *pBuff, ushort len);
    ushort CheckSum16(const uchar* buff, ushort len);

signals:

    void  sigRunMsgToUi(QString ip, ushort  cmd,QStringList  msg, ushort value);//运行信息返回 UI


private  slots:
    void timeoutMethod(void);


    void recvData(void);
    void recvDataMethod(const uchar *data);

    void  slotDataSent(ushort cmd,uchar type,QVariant variant); //


    void  conncetToLcBoot(uchar target,uchar addr);  //给boot发送联机
    void  lcbootEraseAppSectors(uchar target,uchar addr);   //下位机擦除指定扇区
    void lcBootReplyConnectAnalysis(const uchar *data);//boot联机 回复处理

    void lcbootWriteAppdata(uchar target,uchar addr,ushort txframeIndex); //下发下位机boot升级数据

    void lcbootJumpToApp(uchar target,uchar addr); //下位机boot进行app跳转

        void tcpSendData(uchar *txBuf,const ushort len);   //发送数据

private:

   uchar  m_target;  //升级设备
   uchar  m_addr;   //正在升级的地址
   uchar    lcAddrIndex;    //地址序列
    uchar resendTimes ;    //重发次数

    QString myAddr;  //pc串口地址
  bool  resendMutex;        //重发互斥

    QTimer * m_timer;   //

    uchar recvbuf[RECV_BUFF_LEN ];  //接收数据
    uchar  txBuf[SEND_BUFF_LEN  ];

    enum
    {
        bootConnect=1,//boot联,机
        eraseApp, //扇区擦除
        appDataWrite,  //下发app数据
        bootJumpApp   //boot跳转app
    }m_update;


    QList<uchar>m_powerList; //要升级的电源的地址队列

    uchar  m_allData[APP_SIZE ];
    uint m_totalSdtimes;  //总的1024包发送次数
    qint64 m_totalBytes;  //数据总字节数
    uint m_lastbytes;  //数据不满1024字节数



    QList<txNode*>txList;
};



#endif // Myserial_H
