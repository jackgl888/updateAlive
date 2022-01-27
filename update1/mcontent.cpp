#include "mcontent.h"

mcontent::mcontent(QWidget *parent) : QWidget(parent)
{
    this->stack = new QStackedWidget(this);
    this->stack->setFrameStyle(QFrame::Panel|QFrame::Raised|QFrame::  Sunken);
    this->myWidget = new Widget(this);
  //  this->myPcbaTest  = new pcbaTest(this);

   //   this->stack->addWidget(this->myPcbaTest);
   this->stack->addWidget(this->myWidget);


    QVBoxLayout *RightLayout = new  QVBoxLayout(this);

    RightLayout->addWidget(this->stack);


}

mcontent::~mcontent()
{

}

QStackedWidget *mcontent::getStack()
{
     return this->stack;
}
