#include "projectwidget.h"


/*******************************
 *     "QListWidget::Item{padding-top:20px; padding-bottom:4px; }"
       "QListWidget::Item:hover{background:skyblue; }"
       "QListWidget::item:selected{background:lightgray; color:red; }"
       "QListWidget::item:selected:!active{border-width:0px; background:lightgreen; }"
 *
 *
 * *************************************/

projectWidget::projectWidget(QWidget *parent) : QWidget(parent)
{
    QFont    font;
    font.setFamily("MS Shell Dlg 2");
    this->setFont(font);


    this->splitterMain = new  QSplitter(Qt:: Horizontal, this);
    splitterMain->setOpaqueResize(false);
    //this->splitterMain->setStyleSheet("margin:2px,0px,0px,0px;}");//182 solid gray

    this->list = new QListWidget(splitterMain);
    this->list->setMaximumWidth(120);

    this->list->setFrameStyle(QFrame::Panel|QFrame::Raised|QFrame::  Sunken);
    // this->list->setMaximumSize(500,6000);border-image: url(:/new/prefix1/res/filesave.png);

    this->list->setStyleSheet("QListWidget{border:1px solid gray; color:rgb(132,132,132);\
 background-color:rgb(71,71,71);margin:12px,0px,0px,0px;}" \
"QListWidget::Item:hover{background-color:rgb(170, 170, 170); color: black;}"  // 鼠标停放时的色彩
"QListWidget::Item:selected{background-color:rgb(68, 68, 68); border-style: inset; }"); //按下);/*上、下、左、右，间距*/);/*上、下、左、右，间距*/

    list->setIconSize(QSize(60,60));
    list->setViewMode(QListView  ::IconMode);
    list->setSpacing(0);
    list->setMovement(QListView::Static);  //不可移动
    list->setResizeMode(QListView  ::Adjust);
    list->setFocusPolicy(Qt::NoFocus);
    list->insertItem(1,QObject::tr("在线升级"));
    list->insertItem(0,QObject::tr("PCBA测试"));

    //   list->insertItem(1,QObject::tr("温度调试"));
    //   list->insertItem(3,QObject::tr("OCV调试"));
    //   list->insertItem(4,QObject::tr("库位感应器"));
    //   list->insertItem(5,QObject::tr("通信数据"));

    for(uchar i = 0;i<list->count();i++)
    {
        list->item(i)->setSizeHint(QSize(120,100));
         if(1==i)
        {

              // list->item(i)->setFlags(list->item(i)->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
              list->item(i)->setIcon(QIcon(":/new/res/color.jpg"));
         }

        else if(i==0)
         list->item(i)->setIcon(QIcon(":/new/res/pcbatest.png"));

    }
    this->myContent =   new  mcontent(splitterMain);
    QObject::connect(list,SIGNAL(currentRowChanged(int)),myContent->getStack(),
                     SLOT(setCurrentIndex(int)));
    QVBoxLayout  *m_layout  =  new  QVBoxLayout(this);
   m_layout->setMargin(0);
   m_layout->setSpacing(0);
    m_layout->addWidget(this->splitterMain);

    splitterMain->show();
    this->resize(1800,900);

}

projectWidget::~projectWidget()
{

}
