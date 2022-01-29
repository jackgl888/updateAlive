#include "serverthread.h"
#include<QDebug>





serverThread::serverThread(QString ip, QString mcId, uint sockDesc,uint mcSockDesc, bool isNet, updateTarget target, QObject *parent) :
    QThread(parent),
    m_sockDesc(sockDesc),
    m_mcSockDesc(mcSockDesc)

{
     m_target = target;
     this->m_ip = ip;
     m_isNet = isNet;
    m_mcId = mcId;

}


serverThread::~serverThread()
{
    // qDebug()<<"XGID:"<<this->currentThreadId();

   if(m_isNet == true)
    delete  this->m_socket;
   else
       delete  m_serial;

}

void serverThread::run(void)
{

      if(m_isNet == true)
      {
          m_socket = new MySocket(m_mcId, m_mcSockDesc, m_target);

          m_socket->bind(QHostAddress(m_ip),m_sockDesc);
          m_socket->abort();
          m_socket->connectToHost(m_mcId,m_mcSockDesc);   //
          m_socket->moveToThread(this);

          //回传  running msg
          connect(this->m_socket,SIGNAL(sigRunMsgToUi(QString,ushort,QStringList,ushort)),this,
                  SLOT(slotSendRunningMsg(QString,ushort,QStringList,ushort)));

          //发送指令到线程
          connect(this,SIGNAL(sendUpdateCmd(ushort,uchar,QVariant)), this->m_socket,SLOT(slotDataSent(ushort ,uchar ,QVariant )));   //  主线程
      }
      else
      {
          m_serial = new   Myserial(m_ip,m_target);
          m_serial->moveToThread(this);

          //回传  running msg
          connect( m_serial,SIGNAL(sigRunMsgToUi(QString,ushort,QStringList,ushort)),this,
                  SLOT(slotSendRunningMsg(QString,ushort,QStringList,ushort)));

          //发送指令到线程
          connect(this,SIGNAL(sendUpdateCmd(ushort,uchar,QVariant)),  m_serial,SLOT(slotDataSent(ushort,uchar ,QVariant )));   //  主线程
      }

   this->exec();


}


//










//转发升级命令
void serverThread::slotSendUpdateCmd(ushort cmd, uchar type,QVariant para)
{

    emit sendUpdateCmd(cmd,type,para);
}














void serverThread::slotSendRunningMsg(QString ip, ushort cmd, QStringList msg, ushort value)
{
      emit sigSendRunningMsg(ip,cmd,msg,value);
}











void serverThread::disconnectToHost(void)
{



    m_socket->disconnectFromHost();
  //  emit disconnectTCP( ip);
    this->quit();
    this->wait();
}














