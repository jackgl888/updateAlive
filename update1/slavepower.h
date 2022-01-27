#ifndef SLAVEPOWERMSG_H
#define SLAVEPOWERMSG_H
#include "parentDevice.h"
#include <QObject>
#include <QTimer>
class slavePower : public QObject
{
    Q_OBJECT
public:
    explicit slavePower(QObject *parent = nullptr);
         ~slavePower();
     void  wrPowerSlaveAddr(uchar addr);
     void  wrPowerAddr(uchar addr); //写入父节点地址
     void  wrRootIp(QString ip);  //写入中位机Ip
     uchar rdPowerSlaveAddr();
     uchar  getParentDeviceAddr();  //读出
     QString  rdRootIp();
     void wrIsBoot(bool status);
     bool  rdIsBoot();

signals:

public slots:

private:
     QString  rootIp;
     uchar powerAddr;
     uchar slavePowerAddr;
     QTimer  *m_timer;
     bool isBoot;  //  boot与app区分
};

#endif // SLAVEPOWERMSG_H
