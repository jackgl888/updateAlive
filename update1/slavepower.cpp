#include "slavepower.h"

slavePower::slavePower(QObject *parent) : QObject(parent)
{
   this->m_timer  = new QTimer(this);
  // connect(this->m_timer,SIGNAL(timeout()),this,SLOT(m_timerEvent()));
    this->m_timer->start(1000);
  this->isBoot = false;
}

slavePower::~slavePower()
{

}

void slavePower::wrPowerSlaveAddr(uchar addr)
{
    this->slavePowerAddr = addr;
}



void slavePower::wrPowerAddr(uchar addr)
{
    this->powerAddr = addr;
}

void slavePower::wrRootIp(QString ip)
{
     this->rootIp = ip;
}

uchar slavePower::rdPowerSlaveAddr()
{
    return  this->slavePowerAddr;
}


uchar slavePower::getParentDeviceAddr()
{
     return  this->powerAddr;
}


QString slavePower::rdRootIp()
{
    return this->rootIp;
}

void slavePower::wrIsBoot(bool status)
{
    this->isBoot = status;
}

bool slavePower::rdIsBoot()
{
    return this->isBoot;
}
