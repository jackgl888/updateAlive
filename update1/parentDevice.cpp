#include "parentDevice.h"

parentdevice::parentdevice( QObject *parent) :QObject(parent)
{
      this->isBoot = false;
}


parentdevice::~parentdevice()
{

}

void parentdevice::setParentDeviceAddr(uchar addr)
{
      this-> parentDeviceAddr = addr;
}

uchar parentdevice::getParentDeviceAddr()
{
    return  this-> parentDeviceAddr;
}


bool parentdevice::getIsBoot()
{
     return  this->isBoot;
}

void parentdevice::setIsBoot(bool status)
{
    this->isBoot = status;
}






