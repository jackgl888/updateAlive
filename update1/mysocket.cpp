#include "mysocket.h"
#include  <QTimer>

#include <synchapi.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QDebug>

// CRC 低位字节值表
static  const uchar s_CRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

// CRC 高位字节值表
static  const uchar s_CRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;

MySocket::MySocket( QString mcIp,  uint mcPort, updateTarget target,QObject *parent) : QTcpSocket(parent)

{
    m_target = target;
    m_update = reConnect;//开机允许重连服务器
    m_box = 0;
    lcAddrIndex = 0;
    resendTimes = 0;
    m_checkBuf.rxTimes = 0;
    m_checkBuf.txTimes = 0;
    m_checkBuf.checkedRxtimes = 0;

    hostAddr = mcIp;
    hostPort = mcPort;

    updateStart = false;

    this->m_timer = new QTimer(this);
    connect(this, SIGNAL(readyRead()), this, SLOT(recvData()),Qt::QueuedConnection);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeoutMethod()));
    connect(this,SIGNAL(connected()),this,SLOT(slotConnected()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));   //"联机中位机"

    this->m_timer->start(CONNTIME );

}


MySocket::~MySocket()
{

}




//
void MySocket::slotConnected()
{

    QStringList  msg;
    QString tr;

    resendTimes = 0;
    m_update=   targetConnect;

    m_timer->start(CONNTIME );

}


void MySocket::slotDisconnected()
{

    QStringList  msg;
    QString tr;
  //  deleteLater();
    this->abort();
    if(m_update!=bootJumpApp )
    {
        m_update=reConnect ;


        //        //网络已断开

     //   m_timer->start(CONNTIME );
//        msg.append("APP");
//        tr= "已断开连接";
//        msg.append(tr);
//        emit sigRunMsgToUi(hostAddr,NETLOST,msg,0);
    }

}



//数据下发到boot
void MySocket::slotDataSent(ushort cmd, uchar type,QVariant variant)
{
    QStringList msg;

    QString  tr;
    updatePara *para;
    targetAddr *addrs;

    switch (cmd) {

    case  APPJUMPBOOT :  //自动发送升级数据
        addrs = ( targetAddr *)variant.data();
        getIp=addrs->mcAddr;
    //    updateStart = true;
        if((getIp== this->hostAddr)&&(m_target== MCTRANSMIT )) //判定是否是本socketc对应的中位机
        {
            resendTimes = 0;
            m_timer->stop();
//            cmdAppJumpBoot(m_target,m_box);
//            m_update= appJumpBoot;
            bootWriteAppdata(m_target,  m_box,0);
            //   cmdAppJumpBoot(m_target,m_box);
            m_update =     appDataWrite;


            m_timer->start(3000);
        }
        else   if((addrs->powerAddrList.isEmpty()==false)&&(m_target==POWERMASTER))
        {
            resendTimes = 0;
            m_timer->stop();
            lcAddrIndex = 0;
            this->m_powerList   =addrs->powerAddrList ;
            m_box =m_powerList.at(  lcAddrIndex );
            cmdAppJumpBoot(m_target,m_box);
            m_update= appJumpBoot;
            m_timer->start(3000);
        }
        break;

    case GETAPPFILE :  //得到升级的数据
        para=(updatePara*)variant.data();
        memcpy( m_allData,para->allData,para->totalBytes );
        m_totalBytes =para->totalBytes;
        m_lastbytes = para->lastbytes;
        m_totalSdtimes = para->totalSdtimes;
        tr = QString("%1%2%3%4%5%6").arg("写入总数据为：").arg(QString::number( m_totalBytes)).arg(" 发送次数为：").arg(QString::number( m_totalSdtimes)).
                arg(" 最后一帧字节数为 ：").arg(QString::number(m_lastbytes));
        msg.append("BOOT");
        msg.append(tr);
        emit sigRunMsgToUi(hostAddr,GETAPPFILE ,msg,0);
        break;


    default:
        break;
    }


}



//pc 与target 建立 联机
void MySocket::cmdConnectTarget(uchar target, uchar addr)
{
    ushort chkSum;
    uint   txLen;
    txLen =15;
    memset(txBuf,0,SEND_BUFF_LEN);
    for(uchar i= 0;i<2;i++)
        txBuf[i]=0xAA;
    txBuf[2]=txLen-4;
    txBuf[3]=(uchar)((txLen-4)>>8);
    txBuf[4]=(uchar)((txLen-4)>>16);
    txBuf[5]=(uchar)((txLen-4)>>24);
    txBuf[6]= addr; //
    txBuf[7]=   (uchar) CONNECTTARGET;
    txBuf[8]=  CONNECTTARGET>>8;
    txBuf[9]= target ;   //设备类型
    txBuf[10]= addr ;   //设备层数
    chkSum = checksum(&txBuf[2], txLen-6);
    txBuf[11]= chkSum;
    txBuf[12]=(uchar)( chkSum>>8);
    txBuf[13]=0xBB;
    txBuf[14]=0xBB;   //加密

    this->write(reinterpret_cast<char *>(txBuf),txLen);
}



void MySocket::  timeoutMethod(void)
{

    QStringList  msg;
    QString tr;

    switch (m_update)
    {
    case  reConnect:      //client 联机 host
        if(resendTimes<5)
        {
           this->abort();
            this->connectToHost(hostAddr,hostPort);
        }
        else
        {
            msg.append("APP");
            msg.append( "无法连接服务器！") ;
            emit sigRunMsgToUi(hostAddr, UNCONNECT,msg,0);
            this->m_timer->stop();
        }
        break;
    case  targetConnect:          //与target建立 联机
        if(resendTimes<3)
            cmdConnectTarget(m_target,m_box);
        else
        {
            msg.append("APP");
            msg.append( "与中位机联机失败！") ;
            emit sigRunMsgToUi(hostAddr, UNCONNECT,msg,0);
            this->m_timer->stop();
        }
        break;
    case   eraseApp:      //擦除扇区
        if(resendTimes>3)
        {
            resendTimes = 0;
            if(m_target == POWERMASTER)
            {
                lcAddrIndex++;
                if(lcAddrIndex<m_powerList.count())
                {
                    m_box =m_powerList.at(lcAddrIndex);
                    bootEraseAppSectors(m_target,m_box);
                }
                else
                {
                    lcAddrIndex =m_powerList.count();
                    //                m_addr =m_powerList.at(lcAddrIndex);
                    //                  lcbootWriteAppdata(m_target,m_addr,0);
                    m_update =  targetConnect;
                }
            }

        }
        else
            bootEraseAppSectors(m_target,m_box);
        break;
    case  appDataWrite:      //写入升级数据
        //        if(resendTimes>3)
        //        {
        //            resendTimes = 0;
        //            if(m_target == POWERMASTER)
        //            {
        //                lcAddrIndex++;
        //                if(lcAddrIndex<m_powerList.count())
        //                {
        //                    m_box =m_powerList.at(lcAddrIndex);
        //                    bootWriteAppdata(m_target,m_box, 0);
        //                }
        //                else
        //                {

        //                    resendTimes = 3;
        //                    bootWriteAppdata(m_target,m_box, 0);


        //                }
        //            }
        //            m_timer->stop();
        //        }
        //        else
        // if(writeMutex == false)
        bootWriteAppdata(m_target,m_box, txIndex);
        break;
    case bootJumpApp:  //跳转app
        bootJumpToApp(m_target,m_box);
        break;
    case  appJumpBoot:   //跳转到boot
        cmdAppJumpBoot(m_target,m_box);
        break;
    default:
        break;
    }

    resendTimes ++;

}









void MySocket::recvData(void)
{


    typedef  enum{WAIT_HEAD,WAIT_LEN,WAIT_MSG}wait;
    QByteArray  array;

    static wait m_wait = WAIT_HEAD;
    static uchar head_cnt=0;
    static uint recv_cnt=0,appLenght=0;
    static   uchar buf[ SEND_BUFF_LEN]={0};
    memset(buf,0,sizeof (buf));
    array = this->readAll();
    memcpy(buf,array.data(),array.length());
    ushort    cmd = *(buf+7)|*(buf+8)<<8;  //命令字
    if(cmd==   BOOTWRITEDATA )
        m_checkBuf.rxTimes++;
    for(ushort i=0;i<array.length();i++)
    {

        if(m_wait == WAIT_HEAD)  //接收
        {
            if(buf[i] == 0xCC)
            {
                if(head_cnt == recv_cnt)
                {
                    this->recvbuf[recv_cnt]=buf[i];
                    if(++head_cnt>=2)
                        m_wait = WAIT_LEN;
                }
                else
                    goto  err_process;
            }
            else
                goto   err_process;
        }
        else  if(m_wait == WAIT_LEN)
        {
            if(recv_cnt < 5)
            {
                this->recvbuf[recv_cnt]=buf[i];
                appLenght |= buf[i]<<(recv_cnt-2)*8;
            }
            if(recv_cnt == 5)
            {
                this->recvbuf[recv_cnt]=buf[i];
                appLenght |= buf[i]<<(recv_cnt-2)*8;
                if((appLenght +4)<=SEND_BUFF_LEN)
                {
                    m_wait = WAIT_MSG;
                }
                else {
                    goto err_process;
                }
            }
        }
        else if(m_wait == WAIT_MSG)
        {
            this->recvbuf[recv_cnt]=buf[i];
            if(recv_cnt>=(appLenght+4-1))
            {
                m_wait = WAIT_HEAD;
                recv_cnt =0;
                head_cnt =0;
                appLenght = 0;
                //接收一帧数处理
                this->recvDataMethod( this->recvbuf);
                memset(this->recvbuf,0,RECV_BUFF_LEN );
                return ;
            }
        }
        else
            goto   err_process;
        recv_cnt++;
        continue ;
err_process:
        m_wait = WAIT_HEAD;
        recv_cnt = 0;
        head_cnt = 0;
        appLenght = 0;
        return;
    }

}


// qDebug()<<"REthread:"<<QThread::currentThreadId();
//  const  QString str = QString::fromUtf8(data.data(),data.length());

//接收数据

void MySocket::recvDataMethod(const uchar * data)
{
    ushort cmd, chkSum,chkSumRecv;
    uint len=0;
    uchar addr,flag,result;
    QString tr,ip;
    QStringList msg;
    uchar rcbuf[50]={0};

    len =*(data + 2)|(*(data + 3)<<8) |(*(data + 4)<<16)|(*(data + 5)<<24) ;  //数据长度
    chkSumRecv = *(data +(len +4)-3)<<8|*(data +(len +4)-4);
    chkSum =  checksum(data+2, len-2);
    memcpy(rcbuf,data,len +4);

    if(chkSum!=chkSumRecv )
        return;
    resendTimes =0;

    addr= *(data+10);  //lc地址
    cmd = *(data+7)|*(data+8)<<8;  //命令字

    result= *(data+11) ;
    resendTimes = 0;
   // this->m_timer->stop();
    switch (cmd) {

    case  CONNECTTARGET :      //BOOT联机

        if(result == 0) //是中位机boot回复 ls
        {
            if(updateStart == false )
            {
//                 bootJumpToApp(m_target,0);
//                 m_update =   bootJumpApp;
//                 m_timer->start(3000);
                 msg.append("BOOT");
                tr = QString("%1%2").arg(QString::number(addr)).arg( "中位机运行boot，需要跳转到app") ;
                msg.append(tr);
                emit sigRunMsgToUi(hostAddr,  UNCONNECT,msg,100);
            }
            else
            {
                m_update = eraseApp;
                bootEraseAppSectors(m_target,0);
                m_timer->start( RESENDTIME );
                return;
            }

        }
        else if(result == 1)    //是中位机APP回复
        {

            PowerMsg m_powerMsg;
            QVariant  variant;
            memset(&m_powerMsg,0,sizeof (m_powerMsg));  //清0

            m_powerMsg.mcVersion = *(data+12)|(*(data+13)<<8) ;  //MC  version
            ushort lcNum = *(data+14); //lc num
            memcpy(&m_powerMsg.m_lcMsg,data+15,lcNum*(sizeof (LcMsg)));
            msg.append("BOOT");     //
            tr = QString("%1%2").arg(QString::number(addr)).arg( "app跳转boot完成，联机boot...") ;
            msg.append(tr);

            variant.setValue(m_powerMsg);
            msg.append(QString::number( m_powerMsg.mcVersion));
            for(uchar i =0;i<lcNum;i++)
            {
                msg.append(QString::number(m_powerMsg.m_lcMsg[i].boxId));
                msg.append(QString::number(m_powerMsg.m_lcMsg[i].boxVersion));
            }
            emit sigRunMsgToUi(hostAddr,CONNECTTARGET,msg,lcNum);
        }
        else if (result == 2)   //中位机在boot内，无法与下位机通信 ，需要跳转
        {
            msg.append("BOOT");
            tr = QString("%1%2").arg(QString::number(addr)).arg( "中位机运行boot，需要跳转到app") ;
            msg.append(tr);
            emit sigRunMsgToUi(hostAddr,  CONNECTTARGET,msg,100);
            this->disconnectFromHost();
        }
        else
        {
            ;
        }
        break;
    case  APPJUMPBOOT:   //app跳转到boot
        if(m_target == MCTRANSMIT)
        {
            if(result == 0)  //跳转成功
            {
                m_update = reConnect;
                disconnectFromHost();

            }
            else
            {
                ;
            }
        }
        break;
    case  BOOTJUMPAPP:    //boot跳转到app
        if(m_target == MCTRANSMIT)
        {
            if(result == 0)  //跳转成功
            {
               m_update = reConnect;
             //   m_timer->start(3000);
                 // this->abort();
                disconnectFromHost();

            }
            else
            {
                ;
            }
        }
        break;
    case ERASESECTOR:      //擦除
     //   if( result==0)  //擦除成功
       // {
            if(m_target == POWERMASTER)
            {
                lcAddrIndex++;
                if(lcAddrIndex<m_powerList.count())
                {
                    m_box =m_powerList.at(lcAddrIndex);
                    bootEraseAppSectors(  m_target,m_box);
                }
                else
                {
                    m_box = 100;
                    lcAddrIndex =m_powerList.count();
                    cmdAppJumpBoot(m_target,m_box);
                    m_update =  targetConnect;
                    //   m_update = appDataWrite;
                }
                ip = QString("%1%2%3").arg(hostAddr).arg(".").arg(QString::number(m_box ));
            }
            else
            {
                // m_timer->stop();
                ip = hostAddr;
                bootWriteAppdata(m_target,  m_box,0);
                //   cmdAppJumpBoot(m_target,m_box);
                m_update =     appDataWrite;
                m_timer->start( RESENDTIME );

            }
            msg.append("BOOT");
            tr =  QString("%1%2").arg( ip).arg("flash擦除成功") ;
            msg.append(tr);
            emit sigRunMsgToUi( ip ,ERASESECTOR,msg,0);
    //    }
//        else
//        {
//            if(m_target == POWERMASTER)
//                ip = QString("%1%2%3").arg(hostAddr).arg(".").arg(QString::number( m_box));
//            else
//                ip = hostAddr;
//            msg.append("BOOT");
//            tr =  QString("%1%2").arg(  ip).arg("flash擦除失败") ;
//            msg.append(tr);
//            emit sigRunMsgToUi(hostAddr,ERASESECTOR,msg, addr);
//        }
        break;




    case BOOTWRITEDATA:      //app写数据
        m_checkBuf.checkedRxtimes++;
        txIndex= *(data+13)|*(data+14)<<8;
        flag = *(data+15);
        if(  flag =='O')  //升级成功
        {
            if( txIndex  >= m_totalSdtimes) //已经发送完成
            {
                if(m_target == POWERMASTER)
                {
                    lcAddrIndex++;
                    if(lcAddrIndex<m_powerList.count())
                    {
                        m_box =m_powerList.at(lcAddrIndex);
                        bootWriteAppdata(m_target, m_box, 0);
                    }
                    else
                    {
                        lcAddrIndex =0;
                        m_timer->stop();
                        m_box = 0;
                    }
                    ip = QString("%1%2%3").arg(hostAddr).arg(".").arg(QString::number(m_box));
                }
                else
                {
                    m_timer->stop();
                    ip = hostAddr;
                    bootJumpToApp(m_target,0);  //MC从boot跳转到app
                    m_update =  bootJumpApp;
                }
                msg.append("BOOT");
                tr="boot完成app数据写入并跳转成功";
                msg.append(tr);
                emit  sigRunMsgToUi( ip, BOOTWRITEDATA,msg,txIndex);
            }
            else  //继续发下一帧
            {
                bootWriteAppdata( m_target,m_box, txIndex);
                m_timer->start(3000);  //重新开启超时重发定时器
                msg.append("BOOT");
                tr=QString("%1%2").arg("boot写完帧").arg(QString::number(txIndex));
                msg.append(tr);
                if(m_target == POWERMASTER)
                    ip = QString("%1%2%3").arg( hostAddr).arg(".").arg(QString::number(m_box));
                else
                    ip = hostAddr;
                emit  sigRunMsgToUi(ip, BOOTWRITEDATA,msg,txIndex);
            }
        }
        else  //写flash失败
        {
            if(m_target == POWERMASTER)
            {
                lcAddrIndex++;
                if(lcAddrIndex<m_powerList.count())
                {
                    m_box =m_powerList.at(lcAddrIndex);
                    bootWriteAppdata(m_target,m_box,  0);
                }
                else
                {
                    lcAddrIndex =0;
                    m_box =0;
                }
                ip = QString("%1%2%3").arg(hostAddr).arg(".").arg(QString::number(m_box));
            }
            else
            {
                m_timer->stop();
                ip = hostAddr;
                m_update =  bootJumpApp;
            }
            if( txIndex >= m_totalSdtimes)//写入完成，跳转失败
            {
                msg.append("BOOT");
                tr="boot完成app数据写入并跳转失败";
                msg.append(tr);
                emit  sigRunMsgToUi(hostAddr, BOOTWRITEDATA,msg,txIndex);
            }
            else              //写flash中失败
            {
                msg.append("BOOT");
                tr =  QString("%1%2").arg(QString::number(addr)).arg("写入失败") ;
                msg.append(tr);
                emit  sigRunMsgToUi(hostAddr, BOOTWRITEDATA,msg,txIndex);
            }
        }
        break;
    default:
        break;
    }
}





//给app发送跳转boot
void MySocket::cmdAppJumpBoot(uchar target,  uchar addr)
{

    ushort chkSum;
    uint   txLen;
    txLen =15;
    memset(txBuf,0,SEND_BUFF_LEN);
    for(uchar i= 0;i<2;i++)
        txBuf[i]=0xAA;
    txBuf[2]=txLen-4;
    txBuf[3]=(uchar)((txLen-4)>>8);
    txBuf[4]=(uchar)((txLen-4)>>16);
    txBuf[5]=(uchar)((txLen-4)>>24);
    txBuf[6]= addr; //
    txBuf[7]=   (uchar)APPJUMPBOOT;
    txBuf[8]= APPJUMPBOOT>>8;
    txBuf[9]= target ;   //设备类型
    txBuf[10]= addr ;   //设备层数
    chkSum = checksum(&txBuf[2], txLen-6);
    txBuf[11]= chkSum;
    txBuf[12]=(uchar)( chkSum>>8);
    txBuf[13]=0xBB;
    txBuf[14]=0xBB;   //加密

    this->write(reinterpret_cast<char *>(txBuf),txLen);

}




//擦除扇区
void MySocket::bootEraseAppSectors(uchar target,  uchar addr)
{

    uint txLen;
    ushort  chkSum;
    memset(txBuf,0,SEND_BUFF_LEN);

    txLen =19;
    for(uchar i= 0;i<2;i++)
        txBuf[i]=0xAA;
    txBuf[2]=txLen-4;
    txBuf[3]=(uchar)((txLen-4)>>8);
    txBuf[4]=(uchar)((txLen-4)>>16);
    txBuf[5]=(uchar)((txLen-4)>>24);
    txBuf[6]= 0;
    txBuf[7]= (uchar)ERASESECTOR;
    txBuf[8]=  ERASESECTOR>>8;  //设备类型
    txBuf[9]=target;
    txBuf[10]=addr;
    txBuf[11]= m_totalBytes;   //写入总32字数
    txBuf[12]=(uchar)( m_totalBytes>>8);
    txBuf[13]=(uchar)( m_totalBytes>>16);
    txBuf[14]=(uchar)( m_totalBytes>>24);
    chkSum = checksum(&txBuf[2], txLen-6);
    txBuf[15]=   chkSum;
    txBuf[16]=(uchar)( chkSum>>8);
    txBuf[17]= 0xBB;
    txBuf[18]=0xBB;

    this->write(reinterpret_cast<char *>(txBuf),txLen);

}


//


//




//boot跳转APP运行
void MySocket::bootJumpToApp(uchar target,  uchar addr)
{

    ushort chkSum;
    uint txLen;
    memset(txBuf,0,SEND_BUFF_LEN);
    txLen =15;
    for(uchar i= 0;i<2;i++)
        txBuf[i]=0xAA;
    txBuf[2]=txLen-4;
    txBuf[3]=(uchar)((txLen-4)>>8);
    txBuf[4]=(uchar)((txLen-4)>>16);
    txBuf[5]=(uchar)((txLen-4)>>24);
    txBuf[6]= 0;
    txBuf[7]= (uchar)BOOTJUMPAPP  ;
    txBuf[8]=BOOTJUMPAPP >>8;
    txBuf[9]=target ;
    txBuf[10]=addr ;//设备层数
    chkSum = checksum(&txBuf[2], txLen-6);
    txBuf[11]= chkSum;
    txBuf[12]=(uchar)( chkSum>>8);
    txBuf[13]=0xBB;   //
    txBuf[14]=0xBB;



    this->write(reinterpret_cast<char *>(txBuf),txLen);
}





//
void MySocket::readBackLcformerApp()
{
    ushort chkSum;
    uint txLen;
    memset(txBuf,0,SEND_BUFF_LEN);
    txLen =14;
    for(uchar i= 0;i<2;i++)
        txBuf[i]=0xFE;
    txBuf[2]=txLen-4;
    txBuf[3]=(uchar)((txLen-4)>>8);
    txBuf[4]=(uchar)((txLen-4)>>16);
    txBuf[5]=(uchar)((txLen-4)>>24);
    txBuf[6]= 0;
    txBuf[7]=   BACKUPAPP ;
    txBuf[8]=   BACKUPAPP>>8 ;
    txBuf[9]=POWERMASTER;
    txBuf[10]=POWERMASTER>>8;   //设备类型
    chkSum= checksum(&txBuf[6], txLen-4);
    txBuf[11]=chkSum;
    txBuf[12]=(uchar)(chkSum>>8);
    txBuf[13]=0xEE;   //
    txBuf[14]=0xEE;
    this->write(reinterpret_cast<char *>(txBuf),txLen);
}



//读取回备份数据
void MySocket::backUpFile(QString filename)
{
    QFile  f(filename);
    f.open(QIODevice::WriteOnly);
    QDataStream  qs(&f);
    for(quint64 i = 0;i<formerAppLen;i++)
    {
        qs<<*(this->formerAppData + i);
    }
    f.close();
}



//数据发送
void MySocket::tcpSendData( uchar *txBuf, const ushort len)
{

    txNode *m_txNode = new txNode;
    m_txNode->txLen=len;
    memcpy(m_txNode->txBuf,txBuf,len);
    txList.append(m_txNode);

}


//下发中位机boot升级数据
void MySocket::bootWriteAppdata(uchar target,  uchar addr,ushort txframeIndex)
{

    uint txLen;
    ushort           chkSum  ;
    txLen = 19+MAX_SEND_APPSIZE  ;
    memset(txBuf,0,SEND_BUFF_LEN);
    for(uchar i= 0;i<2;i++)
        txBuf[i]=0xAA;
    txBuf[2]=txLen-4;
    txBuf[3]=(uchar)((txLen-4)>>8);
    txBuf[4]=(uchar)((txLen-4)>>16);
    txBuf[5]=(uchar)((txLen-4)>>24);
    txBuf[6]=addr;
    txBuf[7]=  (uchar)BOOTWRITEDATA ;
    txBuf[8]=  BOOTWRITEDATA>>8 ;
    txBuf[9]= target;  //设备类型
    txBuf[10]= addr;  //设备层数
    txBuf[11] =m_totalSdtimes;  //总帧数
    txBuf[12] =m_totalSdtimes>>8;  //总帧数
    txBuf[13]=txframeIndex;   //帧索引
    txBuf[14] =txframeIndex>>8;
    memcpy(&txBuf[15],&m_allData[0+(txframeIndex)*MAX_SEND_APPSIZE],MAX_SEND_APPSIZE);
    chkSum =checksum((const uchar *)&txBuf[2], (txLen-6));
    txBuf[16+MAX_SEND_APPSIZE-1]=  chkSum;
    txBuf[17+MAX_SEND_APPSIZE-1]=(uchar)(  chkSum>>8);
    txBuf[18+MAX_SEND_APPSIZE-1]=0XBB;
    txBuf[19+MAX_SEND_APPSIZE-1]=0XBB;


    Sleep(10);
    m_checkBuf.txTimes++;
    this->write(reinterpret_cast<char *>(txBuf),txLen);

}






//crc校验
ushort MySocket::CRC16_Modbus(const uchar *_pBuf, ushort _usLen)
{

    uint 	i = 0;
    uchar 	CRCH = 0xFF;
    uchar	CRCL = 0xFF;
    ushort 	index = 0x0000;

    while (_usLen-- > 0)
    {
        index = (ushort)(CRCL ^ _pBuf[i++]);
        CRCL = (uchar)(CRCH ^ s_CRCHi[index]);
        CRCH = s_CRCLo[index];
    }

    return (CRCH << 8 | CRCL);

}

ushort MySocket::checksum(const uchar *pBuff, ushort len)
{
    //    ushort sum;
    //    ushort i;
    //    sum = 0;
    //    for (i = 0; i < len; i++)
    //        sum += *pBuff++;

    //    return sum;

    uint16_t src = 0;

    while(len != 0)
    {
        src += (*pBuff);                     //16位数高8位
        pBuff++;                             //buff移位
        len--;                              //长度递减
    }

    return src;
}


bool MySocket::net_packet_checksum(const uchar *pBuff, ushort len)
{
    uchar sum;

    sum = checksum(pBuff, (len - 1));

    if (sum == *(pBuff + len - 1 ))
        return true;
    else
        return false;
}



