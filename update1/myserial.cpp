#include "myserial.h"
#include <QDateTime>
#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QDebug>

#include <synchapi.h>


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


Myserial::Myserial(QString ip, updateTarget target, QObject *parent):
    QSerialPort(parent)
{
    Init_serial( ip);
    this->m_timer = new QTimer(this);
    connect(this,&Myserial:: readyRead, this, &Myserial::recvData);//
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeoutMethod()));


    resendMutex = true;
    m_target = target;
    myAddr = ip;
    m_update = bootConnect;
    m_addr = 0;
    lcAddrIndex = 0;
    resendTimes = 0;
    this->m_timer->start(CONNTIME );

}



Myserial::~Myserial()
{
    
}




void Myserial::recvDataMethod( const uchar *data)
{
    ushort cmd, index,chkSum,chkSumRecv,len=0;

    uchar addr,flag;
    QString tr,ip;
    QStringList msg;
    //  const  QString str = QString::fromUtf8(data.data(),data.length());
    len =*(data + 3) |*(data + 4)<<8 ;  //数据长度
    chkSumRecv = *(data +(len +8)-2)<<8|*(data +(len +8)-3);
    chkSum =  CheckSum16(data+1, len+4);
    if(chkSum!=chkSumRecv )
        return;

    addr= *(data+1);   //层数
    cmd = *(data+2);  //命令字



    switch (cmd) {

    case    LCBOOTCONNECT:        //boot联机 回复
        lcAddrIndex++;

        lcBootReplyConnectAnalysis(data);
        //  m_update = eraseApp;
        break;
    case      LCERASESECTOR:      //擦除
        if(*(data+7) == 'O')  //擦除成功
        {

            lcAddrIndex++;
            if(lcAddrIndex<m_powerList.count())
            {
                  m_addr =m_powerList.at(lcAddrIndex);
                  lcbootEraseAppSectors(  m_target, m_addr);
            }

            else
            {
                m_addr = 0;
                lcAddrIndex =0;
                m_timer->stop();
                //   m_update = appDataWrite;
            }
            msg.append("BOOT");
            tr =  QString("%1%2").arg(QString::number(addr)).arg("flash擦除成功") ;
            msg.append(tr);
            if(m_target == POWERMASTER)
                ip = QString("%1%2%3").arg(myAddr).arg(".").arg(QString::number(m_addr));
            else
                ip = myAddr;
            emit sigRunMsgToUi(myAddr,ERASESECTOR,msg,0);
        }
        else
        {
            msg.append("BOOT");
            tr =  QString("%1%2").arg(QString::number(addr)).arg("flash擦除失败") ;
            msg.append(tr);
            if(m_target == POWERMASTER)
                ip = QString("%1%2%3").arg(myAddr).arg(".").arg(QString::number(m_addr));
            else
                ip = myAddr;
            emit sigRunMsgToUi(myAddr,     ERASESECTOR,msg, addr);
        }
        break;
    case  LCBOOTWRITEDATA:      //app写数据
        m_timer->stop();  //关闭超时重发定时器
        index = *(data+9)|*(data+10)<<8;

        flag = *(data+11);
        if(  flag =='O')  //升级成功
        {
            if( index  >= m_totalSdtimes) //已经发送完成
            {
                lcAddrIndex++;
                if(lcAddrIndex<m_powerList.count())
                {
                    m_addr =m_powerList.at(lcAddrIndex);
                    lcbootWriteAppdata(m_target,m_addr,  0);
                }
                else
                {
                    lcAddrIndex =0;
                    m_timer->stop();
                    m_addr = 0;
                }
                msg.append("BOOT");
                tr="boot完成app数据写入并跳转成功";
                msg.append(tr);
                if(m_target == POWERMASTER)
                    ip = QString("%1%2%3").arg(myAddr).arg(".").arg(QString::number(m_addr));
                else
                    ip = myAddr;
                emit  sigRunMsgToUi(myAddr, BOOTWRITEDATA,msg,index);
            }
            else  //继续发下一帧
            {
                lcbootWriteAppdata(m_target,m_addr,  index);
                m_timer->start(RESENDTIME);  //重新开启超时重发定时器
                msg.append("BOOT");
                tr=QString("%1%2").arg("boot写完帧").arg(QString::number(index));
                msg.append(tr);
                if(m_target == POWERMASTER)
                    ip = QString("%1%2%3").arg(myAddr).arg(".").arg(QString::number(m_addr));
                else
                    ip = myAddr;
                emit  sigRunMsgToUi(ip, BOOTWRITEDATA,msg,index);
            }
        }
        else     //升级失败
        {
            lcAddrIndex++;
            if(lcAddrIndex<m_powerList.count())
            {
                m_addr =m_powerList.at(lcAddrIndex);
                lcbootWriteAppdata(m_target,m_addr,  0);
            }
            else
            {
                lcAddrIndex =0;
                m_addr =0;
            }
            if( index  >= m_totalSdtimes)//写入完成，跳转失败
            {
                msg.append("BOOT");
                tr="boot完成app数据写入并跳转失败";
                msg.append(tr);
                if(m_target == POWERMASTER)
                    ip = QString("%1%2%3").arg(myAddr).arg(".").arg(QString::number(m_addr));
                else
                    ip = myAddr;
                emit  sigRunMsgToUi(myAddr, BOOTWRITEDATA,msg,index);
            }
            else              //写flash中失败
            {
                msg.append("BOOT");
                tr =  QString("%1%2").arg(QString::number(addr)).arg("写入失败") ;
                msg.append(tr);
                if(m_target == POWERMASTER)
                    ip = QString("%1%2%3").arg(myAddr).arg(".").arg(QString::number(m_addr));
                else
                    ip = myAddr;
                emit  sigRunMsgToUi(myAddr, BOOTWRITEDATA,msg,index);
            }
        }
        break;
    default:
        break;

    }

}







void Myserial::slotDataSent(ushort cmd, uchar type, QVariant variant)
{

    QStringList msg;
    QString  ip,tr;
    updatePara *para;
    targetAddr *addrs;
    resendMutex = false;
    switch (cmd) {

    case MANUALERASE:  //手动擦除
        m_target = type;
        addrs = ( targetAddr *)variant.data();
        this->m_powerList   =addrs->powerAddrList ;
        if(this->m_powerList.isEmpty()==NULL)
        {
            lcAddrIndex = 0;
            m_addr =m_powerList.at(  lcAddrIndex );
            m_update=  eraseApp ;
            lcbootEraseAppSectors(m_target,m_addr);
            m_timer->start(RESENDTIME);
        }


        break;
    case BOOTWRITEDATA :  //自动发送升级数据
        addrs = ( targetAddr *)variant.data();
        this->m_powerList   =addrs->powerAddrList ;
        if(!m_powerList.isEmpty())
        {
            m_update=   appDataWrite ;
            lcAddrIndex = 0;
            m_addr = m_powerList.at( lcAddrIndex);
            lcbootWriteAppdata(m_target,m_addr,0);
            m_timer->start(RESENDTIME);
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
        emit sigRunMsgToUi(myAddr,GETAPPFILE ,msg,0);
        break;
    case BOOTCONNECT :  //搜箱
        m_update = bootConnect;
        m_timer->start(RESENDTIME);
        break;

    default:
        break;
    }
}










void Myserial::conncetToLcBoot(uchar target,uchar addr)
{

    ushort  chkSum,  txLen;
    txLen =10;
    memset(txBuf,0,SEND_BUFF_LEN);
    txBuf[0]=0x5A;
    txBuf[1]= addr;   //板卡地址
    txBuf[2]= LCBOOTCONNECT;
    txBuf[3]= txLen-8;
    txBuf[4]=(uchar)((txLen-8)>>8);
    txBuf[5]=target;
    txBuf[6]= addr;//板卡地址

    chkSum =   CheckSum16(&txBuf[1], txLen-4);
    txBuf[7]= chkSum;
    txBuf[8]= (uchar)(chkSum>>8);
    txBuf[9]= 0xA5;



    this->write(reinterpret_cast<char *>(txBuf),txLen);


}





void Myserial::lcbootEraseAppSectors(uchar target,uchar addr)
{

    ushort  chkSum,txLen;
    memset(txBuf,0,SEND_BUFF_LEN);

    txLen =14;

    txBuf[0]=0x5A;
    txBuf[1]=addr;  //箱号
    txBuf[2]= LCERASESECTOR;
    txBuf[3]=txLen-8;
    txBuf[4]=(uchar)((txLen-8)>>8);
    txBuf[5]= target;
    txBuf[6]= addr;
    txBuf[7]= m_totalBytes;   //写入总32字数
    txBuf[8]=(uchar)( m_totalBytes>>8);
    txBuf[9]=(uchar)( m_totalBytes>>16);
    txBuf[10]=(uchar)( m_totalBytes>>24);
    chkSum =  CheckSum16(&txBuf[1], txLen-4);
    txBuf[11]=   chkSum;
    txBuf[12]=(uchar)( chkSum>>8);
    txBuf[13]= 0xA5;

   this->write(reinterpret_cast<char *>(txBuf),txLen);
}




void Myserial::lcBootReplyConnectAnalysis(const uchar *data)
{
    QStringList msg;
    uchar addr = *(data+1);    //层地址
    msg.append("BOOT");
    msg.append("已跳入boot");
    emit sigRunMsgToUi(myAddr,BOOTCONNECT,msg,addr);
}




void Myserial::lcbootWriteAppdata(uchar target,uchar addr,ushort txframeIndex)
{
    ushort  chkSum,txLen;
    txLen = 14+MAX_SEND_APPSIZE  ;
    memset(txBuf,0,SEND_BUFF_LEN);
    txBuf[0]=0x5A;
    txBuf[1]=addr;  //箱号
    txBuf[2]=  LCBOOTWRITEDATA;
    txBuf[3]=txLen-8;
    txBuf[4]=(uchar)((txLen-8)>>8);
    txBuf[5]=target;
    txBuf[6]= addr;  //箱号
    txBuf[7] =m_totalSdtimes;  //总帧数
    txBuf[8] =m_totalSdtimes>>8;  //总帧数
    txBuf[9]=txframeIndex;   //帧索引
    txBuf[10] =txframeIndex>>8;
    memcpy(&txBuf[11],&m_allData[0+(txframeIndex)*MAX_SEND_APPSIZE],MAX_SEND_APPSIZE);
    chkSum = CheckSum16((const uchar *)&txBuf[1], (txLen-4));
    txBuf[12+MAX_SEND_APPSIZE-1]=  chkSum;
    txBuf[13+MAX_SEND_APPSIZE-1]=(uchar)(  chkSum>>8);
    txBuf[14+MAX_SEND_APPSIZE-1]=0XA5;

    this->write(reinterpret_cast<char *>(txBuf),txLen);


}





void Myserial::lcbootJumpToApp(uchar target,uchar addr)
{

    ushort  chkSum,txLen;
    memset(txBuf,0,SEND_BUFF_LEN);
    txLen = 10;
    txBuf[0]=0x5A;
    txBuf[1]=addr;  //箱号
    txBuf[2]=  LCBOOTJUMPAPP ;
    txBuf[3]=txLen-8;
    txBuf[4]=(uchar)((txLen-8)>>8);
    txBuf[5]= target;
    txBuf[6]= addr;  //箱号

    chkSum =  CheckSum16(&txBuf[1], txLen-4);
    txBuf[7]= chkSum;
    txBuf[8]=(uchar)( chkSum>>8);
    txBuf[9]=0xA5;   //

    this->write(reinterpret_cast<char *>(txBuf),txLen);
}



void Myserial::tcpSendData(uchar *txBuf, const ushort len)
{


        txNode *m_txNode = new txNode;
        m_txNode->txLen=len;
        memcpy(m_txNode->txBuf,txBuf,len);
        txList.append(m_txNode);


}






void  Myserial::Init_serial(QString ip)
{

    //设置串口名
    this->setPortName(ip);
    //设置波特率
    this->setBaudRate(115200);
    //设置数据位数
    this->setDataBits(QSerialPort::Data8);
    //设置奇偶校验
    this->setParity(QSerialPort::NoParity);
    //设置停止位
    this->setStopBits(QSerialPort::OneStop);
    //设置流控制
    this->setFlowControl(QSerialPort::NoFlowControl);
    this->open(QIODevice::ReadWrite);
    

    
}









//
void Myserial::timeoutMethod(void)
{
    QStringList msg;
    resendMutex = true;
    switch (m_update)
    {

    case bootConnect:        //boot联机
        if(resendTimes<3)
            conncetToLcBoot(m_target,m_addr);
        else
        {
            resendTimes = 0;
            lcAddrIndex++;
            if(lcAddrIndex>POWERNUM -1)
            {
                this->m_timer->stop();
                m_addr =0;
                lcAddrIndex=0;

                msg.append("BOOT");
                msg.append("boot跳转已完成");
                emit sigRunMsgToUi(myAddr,BOOTCONNECT,msg,100);

            }
            else
            {
                m_addr = lcAddrIndex;
                conncetToLcBoot(m_target,m_addr);
            }

        }

        break;
    case   eraseApp:      //擦除扇区
        if(resendTimes>3)
        {
            resendTimes = 0;
            lcAddrIndex++;
            if(lcAddrIndex<m_powerList.count())
            {
                m_addr =m_powerList.at(lcAddrIndex);
                lcbootEraseAppSectors(m_target,m_addr);
            }
            else
            {
                lcAddrIndex =0;
                //                m_addr =m_powerList.at(lcAddrIndex);
                //                  lcbootWriteAppdata(m_target,m_addr,0);
                //   m_update = appDataWrite;
            }
        }
        else
            lcbootEraseAppSectors(m_target,m_addr);
        break;
    case  appDataWrite:      //写入升级数据
        if(resendTimes>3)
        {
            resendTimes = 0;
            lcAddrIndex++;
            if(lcAddrIndex<m_powerList.count())
            {
                m_addr =m_powerList.at(lcAddrIndex);
                lcbootWriteAppdata(m_target,m_addr,0);
            }
            else
            {
                 lcAddrIndex =0;
                 m_addr=0;
            }
        }
        else
            lcbootWriteAppdata(m_target,m_addr,0);
        break;

    default:
        break;
    }
    resendTimes++;


}



void Myserial::recvData()
{
    typedef  enum{WAIT_HEAD,WAIT_LEN,WAIT_MSG}wait;
    QByteArray  array;
    uint tmp;
    static wait m_wait = WAIT_HEAD;
    static uchar head_cnt=0;
    static uint recv_cnt=0,appLenght=0;
    static   uchar buf[ SEND_BUFF_LEN]={0};
    memset(buf,0,sizeof (buf));
    array = this->readAll();
    memcpy(buf,array.data(),array.length());
    for(ushort i=0;i<array.length();i++)
    {

        if(m_wait == WAIT_HEAD)  //接收
        {
            if(buf[i] == 0x5A)
            {
                if(head_cnt == recv_cnt)
                {
                    this->recvbuf[recv_cnt]=buf[i];
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
            if(recv_cnt ==3)
            {
                this->recvbuf[recv_cnt]=buf[i];
                appLenght |= buf[i];
            }
            else  if(recv_cnt == 4)
            {
                this->recvbuf[recv_cnt]=buf[i];
                appLenght |= buf[i]<<2;
                if((appLenght +8)<=SEND_BUFF_LEN)
                {
                    m_wait = WAIT_MSG;
                }
                else {
                    goto err_process;
                }
            }
            else
                this->recvbuf[recv_cnt]=buf[i];
        }
        else if(m_wait == WAIT_MSG)
        {
            this->recvbuf[recv_cnt]=buf[i];
            if(recv_cnt>=(appLenght+8-1))
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











//crc校验
ushort Myserial::CRC16_Modbus(const uchar *_pBuf, ushort _usLen)
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

ushort Myserial::checksum(const uchar *pBuff, ushort len)
{
    ushort sum;
    ushort i;
    sum = 0;
    for (i = 0; i < len; i++)
        sum += *pBuff++;

    return sum;
}


bool Myserial::net_packet_checksum(const uchar *pBuff, ushort len)
{
    ushort sum;

    sum = checksum(pBuff, (len - 1));

    if (sum == *(pBuff + len - 1 ))
        return true;
    else
        return false;
}



/*-----------------------------------------------------------------------------
 * @name  : CheckSum16
 * @brief : 校验和分析函数
 * @param : buff校验缓冲区首地址  len校验长度
 * @retval: uint16_t 校验和
 * @date  : 2021/09/10
 * @note  :
 * ---------------------------------------------------------------------------*/
ushort Myserial:: CheckSum16(const uchar * buff, ushort len)
{
    uint acc = 0;
    ushort src = 0;

    while(len > 1)
    {
        src = (*buff << 8);								//16位数高8位
        buff++;											//buff移位
        src |= (*buff);									//16位数低8位
        buff++;											//buff移位
        acc += src;										//16位数累加
        len -= 2;										//长度递减
    }

    if(len > 0)
    {
        src = (*buff) << 8;								//16位数高8位，低8位补零
        acc += src;										//16位数累加
    }
    acc = (acc >> 16) + (acc & 0x0000FFFF);				//32位数的高16位+低16位
    if((acc & 0xFFFF0000) != 0)							//判断32位数是否大于0xFFFF
    {
        acc = (acc >> 16) + (acc & 0x0000FFFF);			//32位数的高16位+低16位
    }
    src = (ushort) acc;								//获取32位数低16位
    return src;
}
