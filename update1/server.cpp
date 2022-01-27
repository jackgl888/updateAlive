#include "widget.h"
#include"server.h"




Server::Server(QObject *parent) :
    QTcpServer(parent)

{

    /* get current dialog object */
    m_dialog = dynamic_cast< Widget *>(parent);
  //  qDebug()<<"parent:"<<this->parent();

}

Server::~Server()
{

}




void Server::incomingConnection(int sockDesc)
{

  //  serverThread *thread = new serverThread(sockDesc);

//    //线程管理
//    connect(thread, SIGNAL(disconnectTCP(const QString &)), this, SLOT(clientDisconnected(const QString &)));
//    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

//    connect(m_dialog, SIGNAL(startSend(const QString&, const  int,const uchar )),
//            thread, SLOT(sendDataSlot(const QString&, const  int , const uchar )),Qt::QueuedConnection);


//    //初始化树形结点/
//    connect(thread,   SIGNAL(initTree(const QString&)), m_dialog,
//            SLOT(treeWidgetInit(const QString&)),Qt::QueuedConnection);

  //  thread->start();

}



void Server::clientDisconnected(const QString &ip)
{

}



