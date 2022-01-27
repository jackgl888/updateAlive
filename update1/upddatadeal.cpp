#include "upddatadeal.h"
#include "datarecv.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QDebug>

updDataDeal::updDataDeal(QObject *parent) : QObject(parent) ,QRunnable()
{

    this->setAutoDelete(true);
    this->wait = 0;
    this->head_cnt = 0;
    this->recv_cnt =0;
    this->length =0;

}

void updDataDeal::run()
{

    qDebug()<<"son thread:"<<QThread::currentThreadId();

    QByteArray  buf;
    uchar str[500]={0};
    QHostAddress addr;
    quint16   port;
    ushort   tmp;
;
    QMutexLocker  locker(&mtx);
    memset(this->recvBuf,0,300);
    port = single::m_Instance()->getDataRecv(0)->portGet;
    buf =  single::m_Instance()->getDataRecv(0)->dataGet;
    addr =single::m_Instance()->getDataRecv(0)->addrGet;
    this->m_addr=addr.toString();
    single::m_Instance()->removeDataRecvList(0);
   //删除处理完成 的节点
    locker.unlock();
    memcpy(str,buf.data(),buf.length());
    if(*(str+ 6) == 0x03)  //

     emit  addMidmachineNode(this->m_addr,0,0,0,0); //发送信号回ui将此中位机节点上树
#if 0
    for(ushort i=0;i<buf.length();i++)
    {

        if(this->wait == WAIT_HEAD)    //接收
        {
            if(str[i] == 0xEE)
            {
                if(this->head_cnt == this->recv_cnt)
                {
                    this->recvBuf[this->recv_cnt]=str[i];
                    if(++this->head_cnt>=4)
                        this->wait = WAIT_LEN;
                }
                else
                    goto  err_process;
            }
            else
                goto   err_process;
        }
        else  if(this->wait == WAIT_LEN)
        {
            if(this->recv_cnt == 4)
            {
                this->recvBuf[this->recv_cnt]=str[i];
                this->length = str[i];
            }
            if(this->recv_cnt == 5)
            {
                this->recvBuf[this->recv_cnt]=str[i];
                tmp= str[i];        //
                tmp<<=8;
                this->length|=tmp;
                if((this->length +6)<= BUFF_LEN)
                {
                    this->wait = WAIT_MSG;    //
                }
                else {
                    goto err_process;
                }
            }
        }
        else if(this->wait == WAIT_MSG)
        {
            this->recvBuf[this->recv_cnt]=str[i];
            if(this->recv_cnt>= this->length +6-1)
            {
                this->wait = WAIT_HEAD;
                this->recv_cnt =0;
                this->head_cnt =0;
                this->length = 0;
                this->recvDataMethod(this->recvBuf);
                return ;
            }
        }
        else
            goto   err_process;

        this->recv_cnt++;
        continue;

err_process:
        this->wait = WAIT_HEAD;
        this->recv_cnt = 0;
        this->head_cnt = 0;
        this->length = 0;
        return;
    }
  #endif
}



void updDataDeal::recvDataMethod(const uchar *data)
{
    ushort len=0;

    //  const  QString str = QString::fromUtf8(data.data(),data.length());
    len = *(data + 4) +(*(data + 5)<<8) ;  //数据长度
    // if(single::m_Instance()->CRC16_Modbus(data+6, len))
    //   return;

    uchar cmd = *(data+6);   //str[6];

    switch (cmd) {
    case APPCONECT  :  //联机
        this->cmdConnectAnalysis(data);
        break;
    case  0x03     :     //   pcbi测试中位机联机回复
        
        break;
    default:
        break;
    }
}



//
void updDataDeal::cmdConnectAnalysis(const uchar *data)
{

    uchar    len[10]={0};
    uchar    parentVersion[10]={0};
    uchar    slaveVersion[50]={0};
    uchar    bedVersion[10]={0},tempVersion[50]={0};
    QString  transmitorVs;
    QStringList needleVsList,tempVsList,powerVsList,slaveVsList;
    uchar midNum=single::m_Instance()->getMidmachineNum();
    needleVsList.clear();
    tempVsList.clear();
    powerVsList.clear();
    slaveVsList.clear();
    uchar bedNum=0,tempNum=0, parentNum=0,slaveNum=0;
    len[0]=*(data+7);   //中位机属性长度
    transmitorVs=QString::number(*(data+8)) ; //中位机版本号
    parentNum = *(data+9+len[0]);  //子节点个数   4
    len[1] = *(data+10+len[0]);  //电源属性长度
    for(uchar i = 0;i<parentNum;i++)    //获取电源节点的版本号
    {
         parentVersion[i]=*(data+11+len[0]+2*i*len[1]);  //
         powerVsList.append(QString::number( parentVersion[i]));
    }
    slaveNum = *(data+11+len[0]+2*parentNum*len[1]);  //  7
    len[2]=*(data+12+len[0]+2*parentNum*len[1]);
    for(uchar i =0;i<parentNum*slaveNum;i++)   //获取电源从节点的版本号
    {
         slaveVersion[i]=*(data+13+len[0]+2*parentNum*len[1]+2*i*len[2]);
         slaveVsList.append(QString::number(slaveVersion[i]));
    }
    //针床
    bedNum =*(data+13+len[0]+2*parentNum*len[1]+2*parentNum*slaveNum*len[2]);
    len[3] =*(data+14+len[0]+2*parentNum*len[1]+2*parentNum*slaveNum*len[2]);
    for(uchar i = 0;i<bedNum;i++)
    {
        bedVersion[i]=*(data+15+len[0]+2*parentNum*len[1]
                +2*parentNum*slaveNum*len[2]+2*i*len[3]);
        needleVsList.append(QString::number(  bedVersion[i]));
    }
   //温度
    tempNum =*(data+15+len[0]+2*parentNum*len[1]
            +2*parentNum*slaveNum*len[2]+2*bedNum*len[3]);
    len[4]=*(data+16+len[0]+2*parentNum*len[1]
            +2*parentNum*slaveNum*len[2]+2*bedNum*len[3]);
    for(uchar i = 0;i<tempNum;i++)
    {
        tempVersion[i]=*(data+17+len[0]+2*parentNum*len[1]
                +2*parentNum*slaveNum*len[2]+2*bedNum*len[3]+2*i*len[4]);
        tempVsList.append(QString::number(tempVersion[i]));
    }

    //转发针床版本为qstring
    if(midNum == 0)  //如果list里面没有节点
    {
        emit  addMidmachineNode(this->m_addr,bedNum,tempNum,parentNum,slaveNum); //发送信号回ui将此中位机节点上树
        emit refreshVerion(this->m_addr,transmitorVs,needleVsList,tempVsList,powerVsList,slaveVsList);
    }
    else
    {
        for(ushort i=0;i<midNum;i++)
        {
            if(single::m_Instance()->getMidmachineNode(i)->rdIp() == this->m_addr)
            {
                 return;
            }
        }
        emit  addMidmachineNode(this->m_addr,bedNum,tempNum,parentNum,slaveNum); //发送信号回ui将此中位机节点上树
        emit refreshVerion(this->m_addr,transmitorVs,needleVsList,tempVsList,powerVsList,slaveVsList);
    }

    
}



















