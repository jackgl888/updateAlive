#include "single.h"
#include "widget.h"
#include <QFileDialog>
#include <QTextStream>
#include <iostream>
#include <malloc.h>
#include<QHeaderView>
#include<QDateTime>
#include <synchapi.h>
//using namespace HalconCpp;



Widget::Widget(QWidget *parent) :  QWidget(parent)
{

    initfilePath();  //文件路径布局初始化
    initTable();   //表格初始化
    initModel(); //模型初始化
    treeWidgetUiInit();  //初始化树形


    bottomLayOutCtrlInit();  //右边控件及布局初始化

    initTimer();   //定时器初始化


    //支持插放
    this->setAcceptDrops(true);


    QGridLayout  * mainLayout  = new QGridLayout(this);

    mainLayout->addLayout(leftLayout,0,0,30,12);
    mainLayout->addLayout(topLayout,0,12,20,30);
    mainLayout->addLayout(bottomLayout,20,12,10,30);
    //    this->resize(1600,800);
    this->resize(800,400);

}


Widget::~Widget()
{
    
    
}





//响应主线程状态改变modle
void Widget::slotItemChange(QString rootIp,QString device)
{

    for(uchar i = 0;i<trayTree->topLevelItemCount();i++)
    {
        if( trayTree->topLevelItem(i)->text(0)==rootIp) //找到item对应子节点
        {
            if( trayTree->topLevelItem(i)->checkState(0)== Qt::Checked)  //中位机选中，
            {
                this->midmachineNodeChecked(i,device);
            }
            else  if( trayTree->topLevelItem(i)->checkState(0)== Qt::Unchecked)//中位机没有被选中，
            {
                this->midmachineNodeUnchecked(i,device);
            }
            else                                                                        //根节点半选中
            {
                this->midmachineNodePartiallyChecked(i,device);
            }
        }
    }
}



//running  msg
void Widget::slotRunningMsgProcess(QString ip, ushort cmd, QStringList msg, ushort value)
{
    uchar row;
    QList<QStandardItem *>m_list;

    switch (cmd) {

    case NETLOST :  //掉线
        disableTreeItems(ip); //  //失能父子节点
        m_list =  this->msgModle->findItems(ip);   //at 0 为消息  at 1 为
        if( m_list.isEmpty())
            return;
        row = m_list.at(0)->row();
        this->msgModle->setData(this->msgModle->index(row,2),msg.at(0)); //指示程序运行层
        this->msgModle->setData(this->msgModle->index(row,4),msg.at(1));//信息
        break;


    case  BOOTCONNECT:    //boot联机

        if((value<8)||(this->deviceBox->currentText() == "中位机"))
        {
            if(trayTree->topLevelItemCount()==0)  //没有节点
                treeWidgetInit( ip,value);  //创建
            else  // 有节
                slotRefreshStatus( ip,msg,value);
        }
        msgBox->append(msg.at(1));
        return;

        break;

    case BOOTWRITEDATA:    //写数据
    //   msgBox->append(msg.at(1));
        m_list =  this->msgModle->findItems(ip);   //at 0 为消息  at 1 为
        if( m_list.isEmpty())
            return;
        row = m_list.at(0)->row();

        this->msgModle->setData(this->msgModle->index(row,3),value);  //加载进度条
        this->msgModle->setData(this->msgModle->index(row,2),msg.at(0)); //指示程序运行层
        this->msgModle->setData(this->msgModle->index(row,4),msg.at(1));//信息
        break;

    case GETAPPFILE:         //转发app文件

        msgBox->append(msg.at(1));
        break;

    default:
        break;
    }


}




//
void Widget::midmachineNodePartiallyChecked(uchar itemIndex,  QString device)
{

    QTreeWidgetItem   *itemparent;
    uchar  parentCount;

    itemparent =    trayTree->topLevelItem(itemIndex)->child(0);
    parentCount= itemparent->childCount(); //父点数
    if(device == "电源柜")
    {
        for(uchar j = 0;j<parentCount;j++)
        {
            if(itemparent->child(j)->checkState(0)==Qt::Checked)  //父选中

                this->parentItemInsert(j,itemIndex,device);

            else   //父不选中
                this->parentItemRemove(j,itemIndex);

        }

    }

}


void Widget::midmachineNodeChecked(uchar itemIndex, QString device)
{
    QTreeWidgetItem   *itemparent ;
    uchar  parentCount;
    //插入item
    if(device=="中位机")
        this->rootItemInsert(itemIndex,device);
    else   //电源柜
    {
        itemparent =   trayTree->topLevelItem(itemIndex)->child(0);
        parentCount = itemparent->childCount(); //父节点数
        for(uchar j = 0;j<parentCount;j++)
            this->parentItemInsert(j,itemIndex,device);

    }
}



void Widget::midmachineNodeUnchecked(uchar itemIndex, QString device)
{
    QTreeWidgetItem   *itemparent ;
    uchar  parentCount;
    //移除item
    if(device=="中位机")
        this->rootItemRemove(itemIndex,device);

    else
    {
        itemparent =   trayTree->topLevelItem(itemIndex)->child(0);
        parentCount = itemparent->childCount(); //电源节点数
        for(uchar j = 0;j<parentCount;j++)
            this->parentItemRemove(j,itemIndex);

    }
}


//
void Widget::rootItemInsert(uchar rootIndex, QString device)
{
    QString  ip;
    QList<QStandardItem *>m_list;

    ip =  trayTree->topLevelItem(rootIndex)->text(0);
    m_list =  this->msgModle->findItems(ip);
    if( m_list.isEmpty())
    {
        //插入item
        if(device=="中位机")
        {
            this->msgModle->setItem(this->rows,0, new QStandardItem(ip));
            this->msgModle->setItem(this->rows,1, new QStandardItem("中位机"));
            this->msgModle->setItem(this->rows,2, new QStandardItem("0"));
            this->msgModle->setItem(this->rows,3, new QStandardItem("0"));
            this->rows++;
        }
    }
}



void Widget::rootItemRemove(uchar rootIndex, QString device)
{
    QString  ip;
    uchar row;
    QList<QStandardItem *>m_list;

    ip =  trayTree->topLevelItem(rootIndex)->text(0);
    m_list = this->msgModle->findItems(ip);

    if(!m_list.isEmpty()) //存在这个点
    {
        //删除item
        if(device=="中位机")
        {
            row = m_list.at(0)->row();
            this->msgModle->removeRow(row);
            this->rows--;
        }
    }
}



//电源柜节点
void Widget::parentItemInsert(uchar parentIndex, uchar rootIndex, QString device)
{
    QString  ip;
    QList<QStandardItem *>m_list;
    QList<QStandardItem *>list;
    ip = QString("%1.%2").arg( trayTree->topLevelItem(rootIndex)->text(0)).
            arg(QString::number( parentIndex)) ;
    m_list =  this->msgModle->findItems(ip);
    if( m_list.isEmpty())
    {
        list.append(new QStandardItem(ip));
        list.append(new QStandardItem(device));
        list.append(new QStandardItem("0"));
        list.append(new QStandardItem("0"));
        //        this->msgModle->setItem( this->rows,0, new QStandardItem(ip));
        //        this->msgModle->setItem(this->rows,1, new QStandardItem(device));
        //        this->msgModle->setItem(this->rows,2, new QStandardItem("0"));
        //        this->msgModle->setItem(this->rows,3, new QStandardItem("0"));
        this->msgModle->insertRow(rows,list);
        this->rows++;

    }

}


void Widget::parentItemRemove(uchar parentIndex, uchar rootIndex)
{
    QString  ip;
    QList<QStandardItem *>m_list;
    uchar row;

    ip = QString("%1.%2").arg( trayTree->topLevelItem( rootIndex)->text(0))
            .arg(QString::number(parentIndex));
    m_list = this->msgModle->findItems(ip);
    if(!m_list.isEmpty())  //存在
    {
        row = m_list.at(0)->row();
        this->msgModle->removeRow(row); //移除

        this->rows--;
    }

}


//slave节点管理
void Widget:: slaveItemManage(uchar parentIndex,uchar rootIndex,QString device)
{

    QTreeWidgetItem   *itemparent,*itemchild;
    uchar  slaveCount;

    itemparent =   trayTree->topLevelItem(rootIndex)->child(0);
    slaveCount=itemparent->child(parentIndex)->childCount();  //电源从板数量
    itemchild = itemparent->child(parentIndex);  //电源节点
    for(uchar  k= 0;k<slaveCount;k++)  //遍历从节点
    {
        if(itemchild->child(k)->checkState(0)==Qt::Checked)//从节点被选中
        {
            //插入子节点standItem
            this->slaveItemInsert(k,parentIndex,rootIndex,device);
        }
        else   //从节点删除
        {
            this->slaveItemRemove(k,parentIndex,rootIndex);
        }
    }
}



void Widget::slaveItemInsert(uchar childIndex,uchar parentIndex,uchar rootIndex,QString device)
{
    QString  ip;
    QList<QStandardItem *>m_list;

    //插入子节点standItem
    ip = QString("%1.%2.%3").arg(   trayTree->topLevelItem(rootIndex)->text(0)).
            arg(QString::number(parentIndex)).arg(QString::number( childIndex)) ;
    m_list =  this->msgModle->findItems(ip);
    if( m_list.isEmpty())
    {
        this->msgModle->setItem( this->rows,0, new QStandardItem(ip));
        this->msgModle->setItem(this->rows,1,new QStandardItem(device));
        this->msgModle->setItem(this->rows,2, new QStandardItem("0"));
        this->msgModle->setItem(this->rows,3, new QStandardItem("0"));
        this->rows++;
    }
}

void Widget::slaveItemRemove(uchar childIndex,uchar parentIndex,uchar rootIndex)
{

    QString  ip;
    QList<QStandardItem *>m_list;
    uchar row;
    ip = QString("%1.%2.%3").arg(  trayTree->topLevelItem(rootIndex)->text(0))
            .arg(QString::number(parentIndex)).arg(childIndex) ;
    m_list = this->msgModle->findItems(ip);
    if(!m_list.isEmpty())  //存在
    {
        row = m_list.at(0)->row();
        this->msgModle->removeRow(row); //移除
        this->rows--;
    }

}


//增加根节点
QTreeWidgetItem * Widget::AddTreeRoot(QString name,QString desc)
{
    
    QTreeWidgetItem * item=new QTreeWidgetItem(this->trayTree);
    this->trayTree->addTopLevelItem(item);
    item->setText(0, name );
    item->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt:: ItemIsSelectable);
    item->setCheckState(0, Qt::Unchecked);
    return item;
    
}

//失能对应节点
void Widget::disableTreeItems(QString text)
{

    for(uchar i =0;i<trayTree->topLevelItemCount();i++)
    {
        if(trayTree->topLevelItem(i)->text(0)==text)
        {

            this->trayTree->topLevelItem(i)->setDisabled(true);
            break;

        }
    }


}





//增加子节点
QTreeWidgetItem *  Widget::AddTreeNode(QTreeWidgetItem *parent,QString name,QString desc)
{
    
    QTreeWidgetItem * item=new QTreeWidgetItem(parent);
    parent->addChild(item);
    item->setText(0,name);
    item->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt:: ItemIsSelectable);
    item->setCheckState(0, Qt::Unchecked);
    return item;
    
    
}




//节点状态处理
void Widget::treeItemChanged(QTreeWidgetItem* item, int column)
{
    this->updateChild(item);
    this->updateParent(item);

}


//清除msg按下
void Widget::clearBtnClickedSlot()
{
    this->msgBox->clear();
}


//跳转
void Widget::jumpBtnClickedSlot()
{
    ;
}


// 手动下发
void Widget::appSendBtnClickedSlot()
{



    uchar  childCount;

    QVariant  variant;
    targetAddr  m_targetAddr;

    static bool  updateBtnMutex  = true;

    // if( updateBtnMutex  == true)
    //  {
    //  updateBtnMutex =false;

    m_targetAddr.mcAddr=nullptr;

    for(uchar i = 0;i<trayTree->topLevelItemCount();i++)
    {
        if(  trayTree->topLevelItem(i)->checkState(0)!= Qt::Unchecked)//有节点被选择
        {
            if(this->deviceBox->currentText() =="中位机")
            {
                if( trayTree->topLevelItem(i)->checkState(0)==Qt::Checked)//中位机被选中
                {
                    m_targetAddr.mcAddr=trayTree->topLevelItem(i)->text(0);
                    variant.setValue( m_targetAddr);
                    emit sigdatasent(  MANUALSENDDATA,  MCTRANSMIT,variant);

                }
            }
            else if(this->deviceBox->currentText() =="电源柜")
            {
                childCount=  trayTree->topLevelItem(i)->child(0)->childCount();
                for(uchar  j= 0;j<childCount;j++)
                {
                    if( trayTree->topLevelItem(i)->child(0)->child(j)->checkState(0)==Qt::Checked)    //电源主板被选择
                    {
                        m_targetAddr.mcAddr=trayTree->topLevelItem(i)->text(0);
                        m_targetAddr.powerAddrList.append( trayTree->topLevelItem(i)->child(0)->child(j)->text(0).toUShort());  //把升级的电源柜地址收集入列

                    }


                }
                variant.setValue(m_targetAddr);
                emit sigdatasent(  MANUALSENDDATA,  POWERMASTER,variant);

            }

        }
    }
    //  }

}

//擦除
void Widget::eraseBtnClickedSlot()
{

    uchar  childCount;

    QVariant  variant;
    targetAddr  m_targetAddr;

    static bool  updateBtnMutex  = true;

    // if( updateBtnMutex  == true)
    //  {
    //  updateBtnMutex =false;

    m_targetAddr.mcAddr=nullptr;

    for(uchar i = 0;i<trayTree->topLevelItemCount();i++)
    {
        if(  trayTree->topLevelItem(i)->checkState(0)!= Qt::Unchecked)//有节点被选择
        {
            if(this->deviceBox->currentText() =="中位机")
            {
                if( trayTree->topLevelItem(i)->checkState(0)==Qt::Checked)//中位机被选中
                {
                    m_targetAddr.mcAddr=trayTree->topLevelItem(i)->text(0);
                    variant.setValue( m_targetAddr);
                    emit sigdatasent( MANUALERASE,  MCTRANSMIT,variant);

                }
            }
            else if(this->deviceBox->currentText() =="电源柜")
            {
                childCount=  trayTree->topLevelItem(i)->child(0)->childCount();
                for(uchar  j= 0;j<childCount;j++)
                {
                    if( trayTree->topLevelItem(i)->child(0)->child(j)->checkState(0)==Qt::Checked)    //电源主板被选择
                    {
                        m_targetAddr.mcAddr=trayTree->topLevelItem(i)->text(0);
                        m_targetAddr.powerAddrList.append( trayTree->topLevelItem(i)->child(0)->child(j)->text(0).toUShort());  //把升级的电源柜地址收集入列
                        variant.setValue(m_targetAddr);
                        emit sigdatasent( MANUALERASE,  POWERMASTER,variant);
                    }


                }

            }
        }
    }
    //  }



}


//手动搜箱
void Widget::bootConnBtnClickedSlot()
{
    sigdatasent(MANUALERASE,MCTRANSMIT,0);
}



//父节点状态处理
void Widget::updateParentItem(QTreeWidgetItem* item)
{
    
    QTreeWidgetItem *parent = item->parent();
    if (parent == NULL)
    {
        return;
    }
    
    //选中的子节点个数
    int selectedCount = 0;
    int childCount = parent->childCount();
    
    for (int i = 0; i < childCount; i++)
    {
        QTreeWidgetItem *childItem = parent->child(i);
        if (childItem->checkState(0) == Qt::Checked)
        {
            selectedCount++;
        }
    }
    if (selectedCount <= 0)
    {
        //未选中状态
        parent->setCheckState(0, Qt::Unchecked);
    }
    else if (selectedCount > 0 && selectedCount < childCount)
    {
        //部分选中状态
        parent->setCheckState(0, Qt::PartiallyChecked);
        
    }
    else if (selectedCount == childCount)
    {
        //选中状态
        parent->setCheckState(0, Qt::Checked);
    }
    
}







//十六进制转换
void Widget::convertData(const QByteArray &data)
{
    
    QByteArray data1;
    QString oneLineData;
    static uint m_address =0;
    if(data.isEmpty())
        return;
    oneLineData.clear();
    foreach(char ch,data)
    {
        data1.clear();
        data1.setRawData(&ch,1);
        oneLineData.append(data1.toHex().toUpper());
        oneLineData.append(" ");
    }
    if(!oneLineData.isEmpty())
    {
        
        data1.clear();
        data1.setNum(m_address,16);  //加入行序号
        if(m_address == 0)
            data1=QByteArray("0x0000")+data1.toUpper();
        else if((m_address>0)&&(m_address<=0xf0))
            data1=QByteArray("0x000")+data1.toUpper();
        else
            data1=QByteArray("0x00")+data1.toUpper();
        oneLineData.prepend("      ");
        oneLineData.prepend(data1);
        //this->fileBrowse->append(oneLineData);
        m_address+=16;
    }
    
}

void Widget::initModel()
{

    QStringList   list;
    QFont font("FangSong",9);
    font.setBold(true);
    msgTable  ->setSelectionMode(QAbstractItemView::ExtendedSelection);
    msgTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->rows = 0;
    this->msgModle =   new  QStandardItemModel(this);
    this->msgModle->setColumnCount(5);
    //  this->msgModle->setRowCount(10);
    list <<"节点名称"<<"设备名称"<<"当前程序"<<"升级进度"<<"升级信息";
    this->msgModle->setHorizontalHeaderLabels(list);
    for(uchar i = 0;i<list.count();i++)
    {
        this->msgModle->horizontalHeaderItem(i)->setFont(font);
        this->msgModle->horizontalHeaderItem(i)->setTextAlignment(Qt:: AlignCenter);
    }

    //      this->msgModle->setItem(0,0,new QStandardItem("温度板app"));
    //     this->msgModle->setItem(0,1,new QStandardItem("温度板app"));
    //         this->msgModle->setItem(0,2,new QStandardItem("温度板app"));
    //         this->msgModle->setItem(0,3,new QStandardItem("温度板app"));
    //    this->msgModle->setItem(0,1,new QStandardItem("9"));
    
    //    this->msgModle->setItem(1,0,new QStandardItem("针床app"));
    
    //    this->msgModle->setItem(1,2,new QStandardItem("6"));
    
    //    this->msgModle->setItem(2,0,new QStandardItem("电源app"));
    
    //    this->msgModle->setItem(2,1,new QStandardItem("7"));
    
    this->msgTable->setModel(this->msgModle);
    //    msgTable->setColumnWidth(0,10);
    //    msgTable->setColumnWidth(1,20);
    
    
}

void Widget::initTable()
{
    this->msgTable  =  new  QTableView(this);

    this->msgTable->horizontalHeader()->setDefaultAlignment(Qt::  AlignBaseline);
    this->msgTable->horizontalHeader()->setSectionResizeMode(QHeaderView::   Stretch);
    this->msgTable->verticalHeader()->setSectionResizeMode(QHeaderView:: Interactive);
    this->msgTable->verticalHeader()->setVisible(true);
    this->msgTable->horizontalHeader()->setVisible(true);
    this->msgTable->setAlternatingRowColors(true);
    this->msgTable->verticalHeader()->setStyleSheet("QHeaderView::section {background-color: rgb(255, 255, 255)}");
    this->msgTable->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color: rgb(255, 255, 255)}");
    this->msgTable->setGridStyle(Qt:: SolidLine);
    this->msgTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->msgTable->setStyleSheet("QTableView{background-color:rgb(219,237,255); color: black;}");

    topLayout  = new QVBoxLayout();
    topLayout->addLayout(pathlayout);
    topLayout->addWidget(msgTable);
}


void Widget::initfilePath()
{

    this->pathLab = new QLabel(tr("文件路径:"),this);
    this->pathEdit = new QLineEdit(this);
    this->pathEdit->setFixedHeight(35);
    this->pathEdit->setReadOnly(true);
    this->fileOpen  = new  QPushButton(tr(""),this);
    this->fileOpen->setFixedSize(35,35);
    this->fileOpen->setStyleSheet("QPushButton{border-image: url(:/new/res/fileopen.png); border-style: outset;}"
                                  "QPushButton:hover{border-image: url(:/new/res/fileopen.png);background-color:rgb(128, 128, 128); color: black;}"
                                  "QPushButton:pressed{border-image: url(:/new/res/fileopen.png);background-color:gray;\
                                  border-style: inset;}");
fileOpen->setEnabled(false);
pathlayout =  new  QHBoxLayout();
pathlayout->addWidget(this->pathLab);
pathlayout->addWidget(this->pathEdit);
pathlayout->addWidget(this->fileOpen);
}



void Widget::initDeviceBox()
{
    QStringList strList;

    strList<<"电源柜"<<"中位机"<<"温度控制板"<<"转发板"<<"工装主控板"
          <<"温度modbus协议"<<"电源从板"<<"电源下位机";
    this->deviceBox  = new QComboBox(this);
    this->deviceBox->addItems(strList);
    this->machineLab  = new QLabel(tr("设备类型:"),this);
    machineLayout = new QHBoxLayout();
    machineLayout->addWidget(this->machineLab);
    machineLayout->addWidget(this->deviceBox);
}

void Widget::initUpdateBtn()
{
    QFont font("STFangsong",25);
    this->updateBtn = new  QPushButton(tr("升级"),this);
    this->updateBtn->setMaximumHeight(200);
    this->updateBtn->setFont(font);
    this->updateBtn->setStyleSheet("QPushButton{background-color:rgb(185,255,203);\
                                   color: black;   border-radius: 10px;  border: 2px groove gray; \
    border-style: outset;}"\
"QPushButton:hover{background-color:red; color: white;}"\
"QPushButton:pressed{background-color:rgb(219,237,255); border-style: inset; }");

updateBtn->setEnabled(false);
updateBtnLayout  = new QVBoxLayout();
updateBtnLayout->addWidget(this->updateBtn);

updateBtnBox = new QGroupBox(this);
updateBtnBox->setLayout(updateBtnLayout);
}


void Widget::initPortAndIp()
{


    this->portLab  = new QLabel(tr("端口绑定:"),this);
    this->portbox  = new QSpinBox (this);
    this->portbox->setMaximum(50000);
    this->portbox->setValue(10002);


    portLayout  = new QHBoxLayout();

    portLayout->addWidget(this->portLab);
    portLayout->addWidget(this->portbox);



    this->m_editerLab = new QLabel(tr("本机IP:"),this);
    //   this->m_editer  = new QIPlinEditer(this);  //ip输入框
    this->ipbox = new QComboBox(this);
    m_editerLayout  = new QHBoxLayout();
    m_editerLayout->addWidget(m_editerLab);
    m_editerLayout->addWidget(this->ipbox);

}

void Widget::initCfgBtn()
{
    this->cfgBtn  = new QPushButton(this);
    this->cfgBtn->setFixedSize(70,70);
    this->cfgBtn->setStyleSheet("QPushButton{border-image: url(:/new/res/filesave.png); border-style: outset;}"
                                "QPushButton:hover{border-image: url(:/new/res/filesave.png);background-color:rgb(128, 128, 128); color: black;}"
                                "QPushButton:pressed{border-image: url(:/new/res/filesave.png);background-color:blue;\
                                border-style: inset;}");
QLabel  *lable = new QLabel("保存",this);
setNetLayout  =new QHBoxLayout();
setNetLayout->addStretch(3);
setNetLayout->addWidget(lable);
setNetLayout->addWidget(this->cfgBtn);
}



void Widget::initSerialUi()
{
    QStringList list;

    list.clear();
    for(uchar i = 0;i<10;i++)
        list.append(QString("%1%2").arg("COM").arg(QString::number(i)));

    this->serialBox = new QComboBox(this);
    this->serialBox->addItems(list);
    this->serialLab = new QLabel("串口名：",this);
    QHBoxLayout  *serialLayout = new QHBoxLayout();
    serialLayout->addWidget(serialLab);
    serialLayout->addWidget(serialBox);

    this->serialGpBox = new  QGroupBox(this);
    serialGpBox->setLayout(serialLayout);
    serialGpBox->setCheckable(true);
    serialGpBox->setChecked(true);

}


void Widget::bottomLayOutCtrlInit()
{
    cfgBoxInit(); //配置控件及布局初始化

    rightBottomLayout  = new QVBoxLayout();//

//    clearBtn = new QPushButton("清除信息",this);
//    clearBtn->setMaximumHeight(150);
//    jumpBtn = new QPushButton("跳转程序",this); //
//    jumpBtn->setMaximumHeight(150);
//    appSendBtn = new QPushButton("APP下发",this); //
//    appSendBtn->setMaximumHeight(150);
//    eraseBtn = new QPushButton("擦除扇区",this);
//    eraseBtn ->setMaximumHeight(150);
//    bootConnBtn = new QPushButton("目标搜索",this);
//    bootConnBtn  ->setMaximumHeight(150);





//    QGroupBox  *manualUpdateBox  =new QGroupBox("手动升级",this);
//    manualUpdateBox->setCheckable(true);
//    manualUpdateBox->setChecked(false);
//    QVBoxLayout  *manualUpdateLayout = new QVBoxLayout();
//    manualUpdateLayout->addWidget(clearBtn);
//    manualUpdateLayout->addWidget( bootConnBtn);
//    manualUpdateLayout->addWidget(jumpBtn);
//    manualUpdateLayout->addWidget(appSendBtn);
//    manualUpdateLayout->addWidget(eraseBtn);
//    manualUpdateBox->setLayout(manualUpdateLayout);
//    rightBottomLayout->addWidget( manualUpdateBox);
//    rightBottomLayout->addWidget( clearBtn);

    rightBottomLayout->addWidget(netConfigBox);
    rightBottomLayout->addWidget(updateBtnBox);
    this->msgBox = new     QTextBrowser(this);  //消息 框
    bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(msgBox);
    bottomLayout->addLayout(rightBottomLayout);
    connect(this->fileOpen,SIGNAL(clicked()),this,SLOT(on_openfile_clicked()));  //打开升级文件
    connect(cfgBtn,SIGNAL(clicked()),this,SLOT(cfgBtnClickedSlot()));    //配置

    connect(this->updateBtn,SIGNAL(clicked()),this,SLOT(updateBtnClickedSlot()));    //执行具体升级过程
//    connect(clearBtn,SIGNAL(clicked()),this,SLOT(clearBtnClickedSlot()));    //清除消息
//    connect(jumpBtn,SIGNAL(clicked()),this,SLOT(jumpBtnClickedSlot()));
//    connect(appSendBtn,SIGNAL(clicked()),this,SLOT(appSendBtnClickedSlot())); //手动下发
//    connect(eraseBtn,SIGNAL(clicked()),this,SLOT(eraseBtnClickedSlot()));//擦除扇区
//    connect(bootConnBtn,SIGNAL(clicked()),this,SLOT(eraseBtnClickedSlot()));//擦除扇区
}



//更新子节点
void Widget::updateChild(QTreeWidgetItem *item)
{
    QString  text = item->text(0);
    if(item->checkState(0) == Qt::PartiallyChecked)
        return;
    int child_count = item->childCount();
    if(child_count > 0)
    {
        for(int i = 0;i<child_count;i++)
        {
            item->child(i)->setCheckState(0,item->checkState(0));
            if(item->child(i)->childCount()>0)
                //递归判定子节点
                updateChild(item->child(i));
        }
        //        for(int i = 0;i<child_count;i++)
        //        {
        
        //        }
    }
}

//设置父节点 关更新状态
void Widget::setParentPartialyChecked(QTreeWidgetItem *item)
{
    QTreeWidgetItem  *parent = item->parent();
    if(parent)
    {
        parent->setCheckState(0,Qt::PartiallyChecked);
        //递归判定父节点
        setParentPartialyChecked(parent);
    }
    else   //此为根节点
        emit sigItemChanged(item->text(0),this->deviceBox->currentText());  //状态改变到线程
    
}


//清除子节点
void Widget::clearChildItem(QTreeWidgetItem *item)
{
    int count  = item->childCount();
    while(count --)
    {
        QTreeWidgetItem  *child = item->child(count);//从大到小去做删除处理
        if(child->childCount()!=0) clearChildItem(child);
        item->removeChild(child);
        delete  child;
        child = nullptr;
    }
}












//刷新版本号
void Widget::slotRefreshStatus(QString ip,QStringList versoinList,ushort powerAddr)
{

    for(uchar i =0;i<trayTree->topLevelItemCount();i++)
    {
        if(trayTree->topLevelItem(i)->text(0)==ip)
        {
            if( trayTree->topLevelItem(i)->isDisabled()==true)
                trayTree->topLevelItem(i)->setDisabled(false);
            if(deviceBox->currentText()=="电源柜")
            {
                if(  trayTree->topLevelItem(i)->child(0)->child( powerAddr)->isDisabled()==true)
                    trayTree->topLevelItem(i)->child(0)->child( powerAddr)->setDisabled(false);
            }

            return;
        }

    }
    treeWidgetInit( ip,powerAddr); //不存在则初始化
}




//配置好ip和port并执行连接host
void Widget::cfgBtnClickedSlot()
{
    static bool cfgBtnMutex=true;
    QString ip;
    ushort port = 0 ;

    if(cfgBtnMutex == true)
    {
        cfgBtnMutex = false;
        if(this->serialGpBox->isChecked()==true)  //串口通信
        {
            ip =   this->serialBox->currentText();
            clientThreadInit(ip,0, false, port,POWERMASTER) ;
        }
        else
        {
            ip =   this->ipbox->currentText();
            port =portbox->value();
            if(deviceBox->currentText()=="中位机")
            {
                clientThreadInit(ip, "192.168.0.100", true,  port, MCTRANSMIT) ;
                clientThreadInit(ip, "192.168.0.101", true,  port, MCTRANSMIT) ;
                clientThreadInit(ip, "192.168.0.102", true,  port, MCTRANSMIT) ;
            }

            else if(deviceBox->currentText()=="电源柜")
                clientThreadInit(ip, "192.168.0.100", true,  port,   POWERMASTER)  ;
            else
                ;
        }

        fileOpen->setEnabled(true);

    }
}





//
void Widget::clientThreadInit(QString myIp, QString  mcId, bool isNet, ushort myPort,updateTarget target)
{
    
    serverThread  *thread = new serverThread(myIp,mcId, isNet,myPort,target,this);
    connect(this,SIGNAL(sigdatasent(ushort,uchar,QVariant)),thread,SLOT(slotSendUpdateCmd(ushort,uchar,QVariant))); //ui命令到线程

    //接收running msg
    connect(thread,SIGNAL(sigSendRunningMsg(QString,ushort,QStringList,ushort)),this,
            SLOT(slotRunningMsgProcess(QString,ushort,QStringList,ushort)));

    thread->start();
    
}







//升级
void Widget::updateBtnClickedSlot(void)
{
    uchar  childCount;

    QVariant  variant;
    targetAddr  m_targetAddr;

    static bool  updateBtnMutex  = true;

    if( updateBtnMutex  == true)
    {
        updateBtnMutex =false;

        m_targetAddr.mcAddr=nullptr;

        for(uchar i = 0;i<trayTree->topLevelItemCount();i++)
        {
            if(  trayTree->topLevelItem(i)->checkState(0)!= Qt::Unchecked)//有节点被选择
            {
                if(this->deviceBox->currentText() =="中位机")
                {
                    if( trayTree->topLevelItem(i)->checkState(0)==Qt::Checked)//中位机被选中
                    {
                        m_targetAddr.mcAddr=trayTree->topLevelItem(i)->text(0);
                        variant.setValue( m_targetAddr);
                        emit sigdatasent( BOOTWRITEDATA,  MCTRANSMIT,variant);
                    }
                }
                else if(this->deviceBox->currentText() =="电源柜")
                {
                    childCount=  trayTree->topLevelItem(i)->child(0)->childCount();
                    for(uchar  j= 0;j<childCount;j++)
                    {
                        if( trayTree->topLevelItem(i)->child(0)->child(j)->checkState(0)==Qt::Checked)    //电源主板被选择

                            m_targetAddr.powerAddrList.append( trayTree->topLevelItem(i)->child(0)->child(j)->text(0).toUShort());  //把升级的电源柜地址收集入列
                    }
                    variant.setValue(m_targetAddr);
                    emit sigdatasent( BOOTWRITEDATA,  POWERMASTER,variant);
                }
            }
        }
    }

}






//更新父节点
void Widget::updateParent(QTreeWidgetItem *item)
{
    
    QString  text = item->text(0);
    QTreeWidgetItem *parent  = item->parent();
    
    if(parent)
    {
        int checkedCount = 0;
        int partiallyCount = 0;
        int itemCount = parent->childCount();
        for(int i = 0; i<itemCount;i++)
        {
            QTreeWidgetItem *item = parent->child(i);
            
            if(Qt::Checked == item->checkState(0))
                checkedCount++;
            else if(Qt::PartiallyChecked == item->checkState(0))
            {
                partiallyCount++;
            }
        }
        if(checkedCount <= 0)  //没有选中
        {
            if(partiallyCount > 0) //有半选中状态
                goto  Partially;
            else
                parent->setCheckState(0,Qt::Unchecked);
            
        }
        else if(checkedCount >0 && checkedCount <itemCount)  //半选中
        {
Partially:
            parent->setCheckState(0,Qt::PartiallyChecked);
            //递归判定父节点
            setParentPartialyChecked(parent);
            return;
        }
        else if(checkedCount > 0 && checkedCount == itemCount) //全选中
        {
            parent->setCheckState(0,Qt::Checked);
        }
        //递归判定父节点
        updateParent(parent);
    }
    else  //此为根节点
    {
        emit sigItemChanged(item->text(0),this->deviceBox->currentText());  //状态改变到线程
    }
}


void Widget::cfgBoxInit()
{

    initDeviceBox(); //设备选择框初始化
    initUpdateBtn();//升级按键初始化
    initPortAndIp();  //端口与ip控制
    initCfgBtn();   //cfgbtn初始化
    initSerialUi();  //初始化串口ui


    netLayout  =new QVBoxLayout();
    netLayout->addLayout(setNetLayout);
    netLayout->addWidget(serialGpBox);
    netLayout->addLayout(machineLayout);
    netLayout->addLayout(portLayout);
    netLayout->addLayout(m_editerLayout);

    netConfigBox  = new QGroupBox(tr("配置"),this);

    netConfigBox->setLayout(netLayout);
}






void Widget::treeWidgetUiInit()
{
    
    this->trayTree  = new  QTreeWidget(this);
    //  this->trayTree->setRootIsDecorated(false);
    this->trayTree->setStyleSheet("QTreeWidget{background-color:rgb(219,237,255); color: black;}"\
                                  "QTreeWidget::item{height:25px}");

    this->trayTree->setSelectionMode(QAbstractItemView::    ContiguousSelection);
    //   this->trayTree->setStyleSheet("QTreeWidget::item{height:25px}");  //设置行宽,     我也找不到提供的接口  setStyleSheet很强大建议看
    this->trayTree->setColumnCount(2);
    this->trayTree->setColumnWidth(0,200);
    QStringList  header;
    header <<"设备节点"<<"版本号";
    this->trayTree->setHeaderLabels(header);
    this->trayTree->setAlternatingRowColors(true);

    leftLayout  = new QVBoxLayout();

    leftLayout->addWidget(this->trayTree);

    connect(this->trayTree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this, SLOT(treeItemChanged(QTreeWidgetItem*, int)));


    //主线程将item状态改变更新msgmodle
    connect(this,SIGNAL(sigItemChanged(QString,QString)),this,SLOT(slotItemChange(QString,QString)));
    
}

void Widget::initTimer()
{
    m_timer=new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeoutEventMethod()));//
    m_timer->start(1000);
}


void Widget::getLocalHostIp()
{
    static  bool iplistMutex = false;
    QStringList list;
    if(iplistMutex == false)
    {
        this->ipbox->clear();
        QString localHostName = QHostInfo::localHostName();
        QHostInfo info = QHostInfo::fromName(localHostName);
        foreach(QHostAddress address,info.addresses())
        {
            if(address.protocol() == QAbstractSocket::IPv4Protocol)
                list.append(address.toString());

        }
        if(list.count() == 4)
        {
            this->ipbox->addItems(list);
            iplistMutex = true;
        }
    }



}







//手动打开文件
void Widget::on_openfile_clicked()
{
    
    QString m_data;   m_data.clear();
    char buff[18];
    int readSize=0;
    int totalSize=0,k=0;
    QFile *localfile;
    QString fileName;
    updatePara   para;
    QVariant variant;

    fileName =QFileDialog::getOpenFileName(this,"打开");
    localfile =  new QFile (fileName );
    this->pathEdit->setText(fileName);
    para.totalBytes = localfile->size();
    para.totalSdtimes = para.totalBytes/ MAX_SEND_APPSIZE;  //包总数
    if(para.totalBytes% MAX_SEND_APPSIZE!= 0)
        para.totalSdtimes+=1;
    para.lastbytes = para.totalBytes%MAX_SEND_APPSIZE;  //剩余要发送的字节数

    this->progressBarDelegate =  new pgbarDelegate(   para.totalSdtimes, PROBARCOlUMN  ,this);  //单元格设置进度条代理
    this->msgTable->setItemDelegate(this->progressBarDelegate);

    if( localfile->open(QIODevice::ReadOnly))
    {

        QDataStream textStream(localfile);
        
        while (!textStream.atEnd()) {
            readSize=textStream.readRawData(buff,16);
            if(readSize<0)
                continue;
            totalSize+= readSize;
            //qDebug()<<readSize<<totalSize<<file.pos();
            localfile->seek(totalSize);
            //qDebug()<<file.pos();
            //   data.append(totalSendBuf,readSize);
            //  m_data.append(totalSendBuf);
            for(int i=0;i<16;i++)
                para. allData[i+16*k]=buff[i];
            
            //  convertData(QByteArray(buff,readSize));
            k++;
            
        }
        
    }
    localfile->close();
    variant.setValue(para);
    emit sigdatasent( GETAPPFILE, MCTRANSMIT,variant);

    this->updateBtn->setEnabled(true);
}




//当用户拖动文件到窗口部件上时候，就会触发dragEnterEvent事件
void  Widget::dragEnterEvent(QDragEnterEvent *event)
{
    //如果为文件，则支持拖放
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

//当用户放下这个文件后，就会触发dropEvent事件
void Widget::dropEvent(QDropEvent *event)
{
    QFile *localfile;
    QString fileName;
    updatePara   para;
    QVariant variant;

    
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;
    //多个文件取第一个来进行后面的操作
    fileName = urls.first().toLocalFile();
    if (fileName.isEmpty()) {
        return;
    }
    localfile =  new QFile (fileName );
    this->pathEdit->setText(fileName);
    
    para. totalBytes = localfile->size();
    para.totalSdtimes = para.totalBytes/ MAX_SEND_APPSIZE;  //包总数
    if(para.totalBytes% MAX_SEND_APPSIZE!= 0)
        para.totalSdtimes+=1;
    para.lastbytes = para.totalBytes%MAX_SEND_APPSIZE;  //剩余要发送的字节数

    this->progressBarDelegate =  new pgbarDelegate(   para.totalSdtimes,PROBARCOlUMN ,this);  //单元格设置进度条代理
    this->msgTable->setItemDelegate(this->progressBarDelegate);

    if( localfile->open(QIODevice::ReadOnly))
    {
        
        QString   text =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ")+"文件导入已完成...";
        QMessageBox* pInfor =new QMessageBox(this);
        pInfor->setText(text);
        pInfor->setWindowTitle("");
        pInfor->show();
        
        QDataStream textStream(localfile);
        QByteArray  data;
        char buff[18];
        int readSize=0;
        data.clear();
        int totalSize=0,k=0;
        while (!textStream.atEnd()) {
            readSize=textStream.readRawData(buff,16);
            if(readSize<0)
                continue;
            totalSize+= readSize;
            
            localfile->seek(totalSize);
            
            for(int i=0;i<16;i++)
                para.allData[i+16*k]=buff[i];
            
            //   convertData(QByteArray(buff,readSize));
            k++;
            
        }
        
    }  //关闭文件
    
    localfile->close();
    variant.setValue(para);
    emit sigdatasent( GETAPPFILE, MCTRANSMIT,variant);
    this->updateBtn->setEnabled(true);
    
}






//初始化树形窗口与托盘管理类
void Widget::treeWidgetInit(const QString & ip,uchar addr)
{

    QTreeWidgetItem *itemRoot=NULL;
    QTreeWidgetItem *itemParent=NULL;
    QTreeWidgetItem *itemChild=NULL;

    itemRoot=AddTreeRoot(ip,0);//创建父节点

    itemRoot->setExpanded(true);
    itemRoot->setBackgroundColor(1,QColor(160,207,207));
    
    itemParent= AddTreeNode(itemRoot,"电源柜",0);
    itemParent->setExpanded(true);

   if(deviceBox->currentText()=="电源柜")
   {
       for(ushort i=0;i<POWERNUM ;i++)//创建子节点
       {
           itemChild= AddTreeNode(itemParent,QString::number(i),0);
           itemChild->setExpanded(true);
           itemChild->setBackgroundColor(1,QColor(160,207,207));
           if(i==addr)  //串口升级状态
               itemChild->setDisabled(false);
           else
               itemChild->setDisabled(true);
       }

   }

    itemRoot->setDisabled(false);
}



//定时更新界面
void Widget::timeoutEventMethod(void)
{
    
    getLocalHostIp();//定时读取ip显示到box
    
}



























//导出运行信息
void Widget::on_exportMsg_clicked()
{
    QFile  myfile("C:/Users/Administrator/Desktop/upgradeMessage.txt");//创建一个输出文件的文档
    if (myfile.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text))//注意WriteOnly是往文本中写入的时候用，ReadOnly是在读文本中内容的时候用，Truncate表示将原来文件中的内容清空
    {
        // ui->runnigMsg->setPlainText("OK");
        //读取之前setPlainText的内容，或直接输出字符串内容QObject::tr()
        QTextStream out(&myfile);
        //  out<<ui->runnigMsg->toPlainText();   //  ->acceptRichText()<<endl;
        QMessageBox* pInfor =new QMessageBox(this);
        pInfor->setText("信息导出成功！");
        pInfor->setWindowTitle("");
        pInfor->show();
    }
}













