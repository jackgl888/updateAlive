#include "datarecv.h"


datarecv::datarecv(QObject *parent) : QObject(parent)
{
    this->m_timer  = new QTimer(this);

    connect(this->m_timer,SIGNAL(timeout()),this,SLOT(m_timerEvent()));


}

datarecv::~datarecv()
{

}



void datarecv::m_timerEvent()
{
       ;
}

