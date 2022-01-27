#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QDebug>
#include "serverthread.h"


#define  PORT_NUM   500
class Widget;
class Dialog;
class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = Q_NULLPTR);
    ~Server();

signals:


private:
    void incomingConnection(int sockDesc);

private slots:
    void clientDisconnected(const QString &ip);


private:
    Widget *m_dialog;

    int m_sockDesc;


};

#endif // SERVER_H
