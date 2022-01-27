#include "midmachine.h"
#include "single.h"
#include  "slavepower.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QDebug>
#include <synchapi.h>
#include <QMetaType>

midmachine::midmachine( QObject *parent) : QObject(parent)
{

    memset(this->updateBuf,0,WRITEN_SIZE);
    m_bHaveRcvedAck=true;
    m_timeout=0;
    m_curSmplChNo=-1;
    sampleTouchMutex = 1;
    makeExcelInitOnce = true;
    this->connected = false;
    this->run= false;
    this->update = false;
    this->cmdTimeout = false;
    //接收函数变量清零
    this->wait = 0;
    this->recv_cnt = 0;
    this->head_cnt = 0;
    this->length = 0;
    //
    this->totalBytes = 0;
    this->lastbytes = 0;

    this->BytesIn = 0;
    this->frameIndex = 0;
    this->itemNumber = 0;
    this->resendCnt = 0;
    this->totalFrames = 0;  //帧总数
    this->resendTimer  = new QTimer(this); //数据重发送定时器
    this->slaveList.clear();
    this->parentDeviceList.clear();
    this->searchUpdateItems = 0;  //检索标志清零
    this->bootItemNumber = 0;
    this->runItemNumber = 0;
    this->isBoot = false;

    midmcTimer   = new QTimer(this);
    //  midmcTimer->start(1000);  //500毫秒间隔下位机联机
    this->dataSendUdpsocket = new QUdpSocket(this);



    connect(midmcTimer,SIGNAL(timeout()),this,SLOT(timeOutMethodSlot()));
    connect(this->resendTimer,SIGNAL(timeout()),this,SLOT( resendTimerOutEvent()));

    memset(this->sendBuf,0, SEND_BUFF_LEN);
    memset(this->recvBuf,0, RECV_BUFF_LEN);

}


midmachine::~midmachine()
{

}

//创建
void midmachine::creatTestDataPath()
{
    // QString path = QDir::currentPath();
    QFileInfoList yyList,mmList,ddList;
    QString   mmPath,ddPath, dateyyy, datemmm, dateddd,filename;
   QDir  dir(topleverDirPath );
    QString dateyy=   QDateTime::currentDateTime().toString("yyyy");
    QString datemm=   QDateTime::currentDateTime().toString("MM");
    QString datedd=   QDateTime::currentDateTime().toString("dd");
   dateyyy= QString("%1%2%3").arg(topleverDirPath ).arg("/").arg(dateyy);
   datemmm= QString("%1%2%3").arg(dateyyy).arg("/").arg(datemm);
   dateddd= QString("%1%2%3").arg(datemmm).arg("/").arg(datedd);
    yyList=getQfileInfoList( topleverDirPath ); //年的文件路径里的list


    if(!(yyList.contains(dateyyy)))  //不存在该年 创建
    {
         this->testResultSavePath = QString("%1%2%3%4%5%6").arg("./").arg(dateyy).arg("/").arg(datemm).arg("/").arg(datedd);
        dir.mkpath( this->testResultSavePath);
        pcbTestXlsxInit();   //建立 一个excel
        return;
    }
    mmPath = QString("%1%2%3").arg(topleverDirPath ).arg("/").arg(dateyy);//月的文件路径
    mmList= getQfileInfoList(mmPath); //月的文件路径里的list
    if(!mmList.contains(datemmm))  //不存在该月  创建
    {

       this->testResultSavePath= QString("%1%2%3%4%5%6").arg("./").arg(dateyy).arg("/").arg(datemm).arg("/").arg(datedd);
        dir.mkpath( this->testResultSavePath);
        pcbTestXlsxInit();   //建立 一个excel
        return;
    }
    ddPath = QString("%1%2%3").arg(  dateyyy).arg("/").arg(datemm);//日的文件路径
    ddList= getQfileInfoList( ddPath); //日的文件路径里的list
    if(!ddList.contains(dateddd))  //不存在该日  创建
    {
       this->testResultSavePath= QString("%1%2%3%4%5%6").arg("./").arg(dateyy).arg("/").arg(datemm).arg("/").arg(datedd);
        dir.mkpath( this->testResultSavePath);
         pcbTestXlsxInit();   //建立 一个excel
        return;
    }
    //get current file path
     this->testResultSavePath= QString("%1%2%3%4%5").arg(dateyy).arg("/").arg(datemm).arg("/").arg(datedd);
    //get  curent  excelRowNum
     filename=QString("%1%2%3%4%5").arg(topleverDirPath  ).arg("/").arg( this->testResultSavePath).arg("/").arg("test.xlsx") ;
   //   this->pcbTestXlsx  =    new    QXlsx::Document(filename,this);  //打开一个boot3的文件
    //  QStringList  list =  this->pcbTestXlsx->sheetNames();



     // get table  row count  colum
//   if(this->pcbTestXlsx->selectSheet("iii"))
//   {
//       QXlsx::CellRange   range;
//       range = this->pcbTestXlsx->dimension();
//       this->curRowNum = range.rowCount();
//   }
//   else
//  {
//       QStringList  headList;
//       headList<<"条码"<<"通道"<<"电池电压(mV)"<<"第二路电压(mV)"<<"端口电压(mV)"<<"电容电压(mV)"\
//              <<"霍尔电流(mA)"<<"电感电流(mA)"<<"母线电压(mV)"<<"温度(℃)"<<"故障码"<<"测试结果";
//       this->pcbTestXlsx->addSheet("iii");
//       for(uchar i= 0;i<12;i++)
//           this->pcbTestXlsx ->write(1,i+1,headList.value(i));
//        this->curRowNum=1;
//   }

}


QFileInfoList midmachine::getQfileInfoList(  QString filePath)
{
    QFileInfoList list;
    QDir *dir = new QDir(filePath);
    dir->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);//文件夹|不包含./和../
    list = dir->entryInfoList();

    return list;

}

QString midmachine::rdIp()
{
    return this->ip;
}



bool midmachine::rdconnect()
{
    return  this->connected;
}

bool midmachine::rdupdate()
{
    return this->update;
}

bool midmachine::rdrun()
{
    return this->run;
}

bool midmachine::rdTimeout()
{
    return  this->cmdTimeout;
}


quint32 midmachine::rdBytesIn()
{
    return this->BytesIn;
}

void midmachine::wrIp(QString m_ip)
{
    this->ip = m_ip;
}



void midmachine::wrconnect(bool flag)
{
    this->connected  =flag;
}

void midmachine::wrupdate(bool flag)
{
    this->update =flag;
}

void midmachine::wrrun(bool flag)
{
    this->run = flag;
}

void midmachine::wrbackup(bool flag)
{
    this->backUp = flag;
}

void midmachine::wrTimeout(bool flag)
{
    this->cmdTimeout = flag;
}

void midmachine::wrBytesIn(uchar byte)
{

    this->appData[BytesIn++]=byte;
}


void midmachine::cleanAppData()
{
    memset(this->appData,0,MAXSIZE);
}






//响应主线程指令
void midmachine::slotDataSent(uchar cmd, uchar type)
{

    switch (cmd) {
    case updateCmd :: APPJUMP :  //BOOT跳转
        if(this->searchUpdateItems == 0)
        {
            this->findChoosedItem(type);  //只检测一次
            searchUpdateItems =1;
        }
        switch (type) {
        case  SLAVEBOARD :          //电源从板
            if(slaveList.at(this->bootItemNumber)->rdIsBoot()==false) //在app
                this->sendBootConnect(type,slaveList.at(this->bootItemNumber)->getParentDeviceAddr(),
                                      slaveList.at(bootItemNumber)->rdPowerSlaveAddr());
            else  //在boot
                this->sendRunCommand(type,slaveList.at(runItemNumber)->getParentDeviceAddr(),
                                     slaveList.at(runItemNumber)->rdPowerSlaveAddr());
            break;
        case   PARENTDEVICE :   //父对象设备
            if(parentDeviceList.at(bootItemNumber)->getIsBoot()==false)
                this->sendBootConnect(type,parentDeviceList.at(bootItemNumber)->getParentDeviceAddr(),0);
            else
                this->sendRunCommand(type,parentDeviceList.at(runItemNumber)->getParentDeviceAddr(),0);
            break;
        case  MID_MACHINE  :  //中位机
            if(this->isBoot == false )  //不在boot
                this->sendBootConnect(type,0,0);
            else
                this->sendRunCommand(type,0,0);
            break;

        default:
            break;
        }
        break;

    case updateCmd::APPERASE:  //擦除
        switch (type) {
        case  SLAVEBOARD :          //电源从板
            this->ereaseSectors(type,slaveList.at(itemNumber)->  getParentDeviceAddr(),
                                slaveList.at(itemNumber)->rdPowerSlaveAddr());
            break;
        case   PARENTDEVICE   :   //父节点设备
            this->ereaseSectors(type,parentDeviceList.at(itemNumber)->  getParentDeviceAddr(),0);
            break;
        case  MID_MACHINE  :  //中位机
            this->ereaseSectors(type,0,0);
            break;
        default:
            break;
        }
        break;
    case updateCmd::BOOTWRITEDATA:  //升级
        switch (type) {
        case  SLAVEBOARD :          //电源从板

            this->sendBootData(type,slaveList.at(itemNumber)->  getParentDeviceAddr(),
                               slaveList.at(itemNumber)->rdPowerSlaveAddr());

            break;
        case  PARENTDEVICE   :   //父节点设备
            this->sendBootData(type,parentDeviceList.at(itemNumber)->  getParentDeviceAddr(),0);
            break;
        case  MID_MACHINE  :  //中位机
            this->sendBootData(type,0,0);
            break;
        default:
            break;

        }
        break;   //少了一个break,害得我查了半天的代码！！！！！
    case updateCmd::APPVERIFY:  //校验
        switch (type) {

        case  SLAVEBOARD :          //电源从板
            this->readBackAppDataVeriy(type,slaveList.at(itemNumber)->  getParentDeviceAddr(),
                                       slaveList.at(itemNumber)->rdPowerSlaveAddr());
            break;
        case  PARENTDEVICE   :   //父节点设备
            this->readBackAppDataVeriy(type,parentDeviceList.at(itemNumber)->  getParentDeviceAddr(),0);
            break;
        case  MID_MACHINE  :  //中位机
            this->readBackAppDataVeriy(type,0,0);
            break;
        default:
            break;

        }
        break;
    case updateCmd::BOOTJUMP:  //运行
        switch (type) {
        case  SLAVEBOARD :          //电源从板
            this->sendRunCommand(type,slaveList.at(runItemNumber)->  getParentDeviceAddr(),
                                 slaveList.at(runItemNumber)->rdPowerSlaveAddr());
            break;
        case   PARENTDEVICE   :   //父节点设备
            this->sendRunCommand(type,parentDeviceList.at(runItemNumber)->  getParentDeviceAddr(),0);
            break;
        case  MID_MACHINE  :  //中位机
            this->sendRunCommand(type,0,0);
            break;
        default:
            break;

        }
        break;
    case  0x12 :     //pcb下位机联机

        sampleTouchMutex = 1;  //开始联机
        if(!midmcTimer->isActive())
            midmcTimer->start(100);
        m_bHaveRcvedAck=true;
        m_timeout=0;
        m_curSmplChNo=-1;
        break;
    case 'S':
        chnNumList.clear();
        pcbJudgeResult.clear();

        break;
    default:
        break;
    }


}








//数据重新发送

void midmachine::resendTimerOutEvent()
{
    QString  targetIp,msg;
    QStringList  list;
    list.clear();
    this->resendTimer->stop();

    if(this->resendCnt >= 3)
    {
        switch (this->resendCmd) {
        case updateCmd:: APPJUMP:
            msg = "boot跳转失败!";
            switch (resendType)
            {
            case  SLAVEBOARD :  //电源从板

                targetIp=QString("%1.%2.%3").arg(this->ip).arg( QString::number(resendParentAddr))
                        .arg(QString::number(resendChildAddr));
                this->bootItemNumber++;
                if( this->bootItemNumber<slaveList.count())
                {
                    //发送一个节点boot联机
                    this->slotDataSent(updateCmd:: APPJUMP, SLAVEBOARD );
                }
                else  //已经遍历完成
                {
                    this->bootItemNumber = 0;
                }
                break;
            case   PARENTDEVICE   :   //父节点设备
                targetIp=QString("%1.%2").arg(this->ip).arg( QString::number(resendParentAddr));
                this->bootItemNumber++;
                if( this->bootItemNumber<parentDeviceList.count())
                {
                    //发送一个节点boot联机
                    this->slotDataSent(updateCmd:: APPJUMP, resendType );
                }
                else  //已经遍历完成
                {
                    this->bootItemNumber = 0;
                }
                break;
            case  MID_MACHINE:  //中位机
                this->slotDataSent(resendCmd,resendType);
                break;
            default:
                break;
            }
            break;
        case updateCmd::APPERASE:
        case updateCmd::BOOTWRITEDATA:
        case updateCmd::APPVERIFY:
            switch (resendType)
            {
            case  SLAVEBOARD :  //电源从板
                this->itemNumber++;
                if(this->itemNumber<slaveList.count())
                {
                    msg="升级失败！";
                    this->slotDataSent(updateCmd::APPERASE,SLAVEBOARD);  //发送下一个节点的擦除
                }
                else
                {
                    msg="升级失败，升级结束！";
                    this->itemNumber = 0;
                }
                break;
            case  PARENTDEVICE   :   //父节点设备
                this->itemNumber++;
                if(this->itemNumber<parentDeviceList.count())
                {
                    msg="升级失败！";
                    this->slotDataSent(updateCmd::APPERASE,PARENTDEVICE);  //发送下一个节点的擦除
                }
                else
                {
                    msg="升级失败，升级结束！";
                    this->itemNumber = 0;
                }
                break;
            case  MID_MACHINE:  //中位机
                this->slotDataSent(resendCmd,resendType);
                break;
            default:
                break;
            }
            break;
        case updateCmd::BOOTJUMP :
            switch (resendType)
            {
            case  SLAVEBOARD :  //电源从板
                this->runItemNumber++;
                if(this->runItemNumber<slaveList.count())
                {
                    this->slotDataSent(updateCmd:: BOOTJUMP,SLAVEBOARD);  //发送下一个节点的运行指令
                }
                else
                {
                    msg="升级完成！";    list<<msg; emit sigRunMsgToUi(targetIp,updateCmd:: BOOTJUMP ,list,0);
                    this->runItemNumber= 0;
                }
                break;
            case  PARENTDEVICE   :   //父节点设备
                this->runItemNumber++;
                if(this->runItemNumber<parentDeviceList.count())
                {
                    msg="运行失败！";
                    this->slotDataSent(updateCmd:: BOOTJUMP,PARENTDEVICE);  //发送下一个节点的运行指令
                }
                else
                {
                    msg="运行失败！";    list<<msg; emit sigRunMsgToUi(targetIp,updateCmd:: BOOTJUMP ,list,0);
                    this->runItemNumber= 0;
                }
                break;
            case  MID_MACHINE:  //中位机
                msg="运行失败！";
                this->isBoot = true;
                break;
            default:
                break;
            }
            break;
        case  0x12 :     //lc联机
            break;
        default:
            break;
        }
        list<<msg;
        emit sigRunMsgToUi(targetIp,resendCmd,list,0);
        this->resendCnt = 0;
    }
    else
    {
        this->slotDataSent(resendCmd,resendType);  //重发
        this->resendCnt++;
    }

}









void midmachine::findChoosedItem(uchar  device)
{

    QTreeWidgetItem   *itemparent,*itemchild;
    uchar  parentCount=0,slaveCount=0;
    uchar itemNum = single::m_Instance()->getItemNum();

    for(uchar i = 0;i<itemNum;i++)
    {
        if(single::m_Instance()->getm_itemNode(i)->text(0)==this->ip) //找到item对应子节点
        {
            if(  single::m_Instance()->getm_itemNode(i)->checkState(0)== Qt::Unchecked)//中位机没有被选中
                return;
            else   //中位机节点选中或者半选中
            {
                if(single::m_Instance()->getm_itemNode(i)->checkState(0)==Qt::Checked)//中位机被选中
                {
                    this->updateMidmachine =0x02;
                }
                else
                    this->updateMidmachine =0x01;    //半选中
                for(uchar n = 2;n<5;n++)
                {
                    if(device == 5)
                    {
                        itemparent = single::m_Instance()->getm_itemNode(i)->child(POWER_LABLE_ITEM );
                        parentCount = itemparent->childCount(); //电源节点数
                        break;
                    }
                    else if(n==device)
                    {
                        itemparent = single::m_Instance()->getm_itemNode(i)->child(n-2);
                        parentCount = itemparent->childCount(); //电源节点数
                        break;
                    }
                }
                for(uchar j = 0;j<parentCount;j++)
                {
                    if(itemparent->child(j)->checkState(0)==Qt::Unchecked)  //此节点没选中，子节点也无选中
                    {
                        ;
                    }
                    else  //节点或者子节点有被选中
                    {
                        if(itemparent->child(j)->checkState(0)==Qt::Checked)  //此节点全选中
                        {
                            //创建准备升级节点对象，加入准备升级的节点队列
                            parentdevice *m_parentdevice = new parentdevice(this);
                            m_parentdevice->setParentDeviceAddr(j); //写入地址
                            this->parentDeviceList.append(m_parentdevice);
                        }
                        slaveCount=itemparent->child(j)->childCount();  //
                        itemchild = itemparent->child(j);              //电源节点
                        for(uchar  k= 0;k<slaveCount;k++)              //遍历从节点
                        {
                            if(itemchild->child(k)->checkState(0)==Qt::Checked)  //从节点是否选中
                            {
                                //创建准备升级的子节点对象，加入准备升级的子节点队列
                                slavePower *m_slave  = new slavePower(this);
                                m_slave->wrPowerAddr(j);  //写入主节点地址
                                m_slave->wrPowerSlaveAddr(k);   //写入自身地址
                                slaveList.append(m_slave);      //
                            }
                            else  //没被选中此节点对应位置0
                            {
                                ;
                            }
                        }
                    }
                }
            }
        }
    }
}



//
void midmachine::slotGetData(QVariant variant)
{
   QStringList list;
   list.clear();
   QString str;
    msg  para = variant.value<msg>();

    this->pcbQrCode = para.qrCode;
    this->m_voltageOutMin = para.voltageOutMin;
    this->m_voltageOutMax = para.voltageOutMax;
    this->m_busVolOutMax = para.busVolOutMax;
    this->m_busVolOutMin = para.busVolOutMin;
    this->m_capVolOutMin = para.capVolOutMin;
    this->m_capVolOutMax = para.capVolOutMax;
    this->m_hallCurOutMin = para.hallCurOutMin;
    this->m_hallCurOutMax = para.hallCurOutMax;
    this->m_inducerCurOutMin = para.inducerCurOutMin;
    this->m_inducerCurOutMax = para.inducerCurOutMax;
    this->m_secondVoltageOutMin = para.secondVoltageOutMin;
    this->m_secondVoltageOutMax = para.secondVoltageOutMax;
    this->m_portVolMin = para.portVolMin;
    this->m_portVolMax = para.portVolMax;
    this->m_testMaster = para.masterBoard;


    if(m_testMaster == true)
    this->topleverDirPath = "./masterData";
    else
    this->topleverDirPath = "./powerData";


    if(makeExcelInitOnce ==true)
    {
        //创建文件夹
         creatTestDataPath();
        //pcb测试表初始化
      //  this->pcbTestXlsxInit();
         makeExcelInitOnce = false;
         str = "初始化成功！";
         list<<str;
         emit sigRunMsgToUi(0,'M',list,0);
    }

}



//发送boot跳转
void midmachine::sendBootConnect(uchar type,uchar parentAddr,uchar childAddr)
{

    uint crc,len;
    len = 12;

    for(uchar j =0;j<4;j++)
        sendBuf[j] = 0xFF;

    sendBuf[4]=len-6 ;   //长度
    sendBuf[5]=(len-6)>>8;   //长度
    sendBuf[6]=parentAddr; //
    sendBuf[7]= updateCmd:: APPJUMP;
    sendBuf[8]= type ;    //
    sendBuf[9]=  childAddr ; //
    sendBuf[10]=0;
    sendBuf[11]=0;
    crc = single::m_Instance()->CRC16_Modbus(&  sendBuf[6], len-6);
    sendBuf[12] = crc&0xff;
    sendBuf[13]=(crc>>8)&0xff;
    this->dataSendUdpsocket->writeDatagram(reinterpret_cast<char *>(sendBuf),len+2,QHostAddress(this->ip),UPDATECLIENTPORT);

    this->resendCmd =updateCmd:: APPJUMP;
    this->resendType = type;
    this->resendParentAddr = parentAddr;
    this->resendChildAddr = childAddr;
    this->resendTimer->start(RESENDDELAY );  //数据重新发送定时器

}

//擦除扇区
void midmachine::ereaseSectors(uchar type,uchar parentAddr,uchar childAddr)
{

    uint crc,len;
    len = 16;
    for(uchar j =0;j<4;j++)
        sendBuf[j] = 0xFF;

    sendBuf[4]=len-6 ;   //长度
    sendBuf[5]=(len-6)>>8;   //长度
    sendBuf[6]=parentAddr; //
    sendBuf[7]= updateCmd:: APPERASE;//
    sendBuf[8]= type ;    //
    sendBuf[9]=  childAddr ; //
    sendBuf[10]=this->totalBytes;      //文件大小
    sendBuf[11]=this->totalBytes>>8;
    sendBuf[12]=this->totalBytes>>16;
    sendBuf[13]=this->totalBytes>>24;
    crc = single::m_Instance()->CRC16_Modbus(&  sendBuf[6], len-6);
    sendBuf[14] = crc&0xff;
    sendBuf[15]=(crc>>8)&0xff;

    this->dataSendUdpsocket->writeDatagram(reinterpret_cast<char *>(sendBuf),len+2,QHostAddress(this->ip),UPDATECLIENTPORT);


    this->resendCmd = updateCmd:: APPERASE;
    this->resendType = type;
    this->resendParentAddr = parentAddr;
    this->resendChildAddr = childAddr;
    this->resendTimer->start(RESENDDELAY);  //数据重新发送定时器
}



//发送BOOT数据
void midmachine::sendBootData(uchar type, uchar parentAddr,  uchar  childAddr)
{
    ushort crc,len=0;
    for(uchar j =0;j<4;j++)
        sendBuf[j] = 0xFF;
    sendBuf[4]=len-6 ;   //长度
    sendBuf[5]=(len-6)>>8;   //长度
    sendBuf[6]= parentAddr;
    sendBuf[7]= updateCmd:: BOOTWRITEDATA;
    sendBuf[8]=type;     //
    sendBuf[9]=childAddr;     //
    sendBuf[10]=frameIndex;

    if(this->frameIndex == (this->totalFrames -1) )//最后一帧
    {
        if( this->lastbytes >0)  //剩余不满1k字节数的发送
        {
            len = 5+ this->lastbytes  +2;  //从箱号到最后帧最后一个字节
            sendBuf[4]=len ;   //长度
            sendBuf[5]=len>>8;   //长度
            memcpy( sendBuf+11,(this->updateBuf+(this->frameIndex* MAX_SEND_SIZE )),this->lastbytes);
            crc = single::m_Instance()->CRC16_Modbus(&sendBuf[6], 4+ this->lastbytes );
            sendBuf[11+this->lastbytes]=  crc& 0XFF;
            sendBuf[11+this->lastbytes+1] =  (crc>> 8)& 0XFF;
            dataSendUdpsocket->writeDatagram(reinterpret_cast<char *>(sendBuf),len+6,QHostAddress(this->ip),UPDATECLIENTPORT);
           // emit dataToSend(ip,  sendBuf,len+6);
        }
        else  //最后一帧刚好1024字节
        {
            len = 5+ MAX_SEND_SIZE  +2;  //从箱号到最后帧最后一个字节
            sendBuf[4]=len ;   //长度
            sendBuf[5]=len>>8;   //长度
            memcpy( sendBuf+11,(this->updateBuf+(this->frameIndex* MAX_SEND_SIZE )),MAX_SEND_SIZE );
            crc =single::m_Instance()-> CRC16_Modbus(&sendBuf[6], 4+ MAX_SEND_SIZE );
            sendBuf[11+MAX_SEND_SIZE ]= crc& 0XFF;
            sendBuf[11+MAX_SEND_SIZE +1] = (crc>> 8)& 0XFF;
            dataSendUdpsocket->writeDatagram(reinterpret_cast<char *>(sendBuf),len+6,QHostAddress(this->ip),UPDATECLIENTPORT);
           // emit dataToSend(ip,  sendBuf,len+6);

        }
    }
    else if(this->frameIndex<(this->totalFrames -1))
    {
        len = 5+ MAX_SEND_SIZE  +2;  //从箱号到最后帧最后一个字节
        sendBuf[4]=len ;   //长度
        sendBuf[5]=len>>8;   //长度
        memcpy( sendBuf+11,(this->updateBuf+(this->frameIndex* MAX_SEND_SIZE )),MAX_SEND_SIZE );
        crc =single::m_Instance()-> CRC16_Modbus(&sendBuf[6], 4+ MAX_SEND_SIZE );
        sendBuf[11+MAX_SEND_SIZE ]= crc& 0XFF;
        sendBuf[11+MAX_SEND_SIZE +1] = (crc>> 8)& 0XFF;
        dataSendUdpsocket->writeDatagram(reinterpret_cast<char *>(sendBuf),len+6,QHostAddress(this->ip),UPDATECLIENTPORT);
       // emit dataToSend(ip,  sendBuf,len+6);
    }
    else
    {
        this->frameIndex = 0;
        return;
    }
    this->resendType = type;
    this->resendCmd =updateCmd:: BOOTWRITEDATA;
    this->resendParentAddr = parentAddr;
    this->resendChildAddr = childAddr;
    this->resendTimer->start(RESENDDELAY);  //数据重新发送定时器

}



//数据读回校验
void midmachine::readBackAppDataVeriy(uchar type, uchar parentAddr, uchar childAddr)
{

    uint crc= 0,len;
    len = 12;
    for(uchar j =0;j<4;j++)
        sendBuf[j] = 0xFF;
    sendBuf[4]=len-6 ;   //长度
    sendBuf[5]=(len-6)>>8;   //长度
    sendBuf[6]=parentAddr; //
    sendBuf[7]= updateCmd::APPVERIFY; //校验命令字
    sendBuf[8]= type ;    //
    sendBuf[9]=  childAddr ; //
    sendBuf[10]=this->frameIndex;
    sendBuf[11]=0;
    crc = single::m_Instance()->CRC16_Modbus(&  sendBuf[6], len-6);
    sendBuf[12] = crc&0xff;
    sendBuf[13]=(crc>>8)&0xff;

    dataSendUdpsocket->writeDatagram(reinterpret_cast<char *>(sendBuf),len+2,QHostAddress(this->ip),UPDATECLIENTPORT);
  //  emit dataToSend(ip,  sendBuf,len+2);

    this->resendCmd = updateCmd::APPVERIFY;
    this->resendType = type;
    this->resendParentAddr = parentAddr;
    this->resendChildAddr = childAddr;
    this->resendTimer->start(RESENDDELAY);  //数据重新发送定时器

}





//运行
void midmachine::sendRunCommand(uchar type,uchar parentAddr,uchar  childAddr)
{

    uint crc,len;

    len = 11;
    for(uchar j =0;j<4;j++)
        sendBuf[j] = 0xFF;
    sendBuf[4]=len-6 ;   //长度
    sendBuf[5]=(len-6)>>8;   //长度
    sendBuf[6]= parentAddr;   //
    sendBuf[7]=updateCmd::BOOTJUMP;
    sendBuf[8]=type;
    sendBuf[9]=childAddr;
    sendBuf[10]=0;
    crc = single::m_Instance()->CRC16_Modbus(&  sendBuf[6], len-6);
    sendBuf[11] = crc&0xff;
    sendBuf[12]=(crc>>8)&0xff;
    dataSendUdpsocket->writeDatagram(reinterpret_cast<char *>(sendBuf),len+2,QHostAddress(this->ip),UPDATECLIENTPORT);
   // emit dataToSend(ip,  sendBuf,len+2);
    this->resendCmd = updateCmd::BOOTJUMP;
    this->resendType = type;
    this->resendChildAddr = childAddr;
    this->resendParentAddr = parentAddr;
    this->resendTimer->start(RESENDDELAY);  //数据重新发送定时器

    
}


//pcb联机
void midmachine::sendPcbConnectCommand()
{
    uint len;
    len = 18;

    for(uchar j =0;j<4;j++)
        sendBuf[j] = 0xFF;
    sendBuf[4]=13;   //长度
    sendBuf[5]=0xFF;   //
    sendBuf[6]=0x12; //   下位机联机命令
    sendBuf[7]= 0;
    sendBuf[8]= 0 ;    //
    sendBuf[9]=  0 ; //
    sendBuf[10]=0;
    sendBuf[11]=0;
    sendBuf[12] = 0;
    sendBuf[13]=0;
    sendBuf[14]=0;
    sendBuf[15]=0;
    sendBuf[16] = 0;
    sendBuf[17]=0;
    for(uchar i = 5;i<17;i++)
        sendBuf[17]+= sendBuf[i];
    dataSendUdpsocket->writeDatagram(reinterpret_cast<const char *>(sendBuf),len,QHostAddress(ip),PCBTESTPORT);
    //   emit dataToSend(ip,  sendBuf,len);
}


//PCB采样测试
void midmachine::sendPcbTestCommand(uchar chnNum)
{

    for(uchar j =0;j<4;j++)
        sendBuf[j] = 0xFF;

    sendBuf[4]=6;   //长度
    sendBuf[5]=0;   //
    sendBuf[6]=0x10; //   下位机采样命令
    sendBuf[7]= chnNum;
    sendBuf[8]= 0 ;    //
    sendBuf[9]= 0 ;
    sendBuf[10]=  0; //
    for(uchar i = 6;i<10;i++)
        sendBuf[10]+=sendBuf[i];
    dataSendUdpsocket->writeDatagram(reinterpret_cast<const char *>(sendBuf),11,QHostAddress(ip),PCBTESTPORT);
    //  emit dataToSend(ip,  sendBuf,11);
}



//pwm测试
void midmachine::sendPcbPwmTestCommand(uchar chnNum, DebugParameterBitField pwmTestPara)
{
    uchar len ;
    memset(&pwmTestPara,0,sizeof(pwmTestPara));   //清零
    pwmTestPara.DebugParameterBitField.chnlNum = chnNum;
    pwmTestPara.DebugParameterBitField.stepType = 'Z';
    pwmTestPara.MainPara1=100;
    len = 5+sizeof(pwmTestPara)+2;
    for(uchar j =0;j<4;j++)
        sendBuf[j] = 0xFF;

    sendBuf[4]=len-5;   //长度
    sendBuf[5]=0;   //地址
    sendBuf[6]=0x14; //   下位机启动pwm功能
    mempcpy(sendBuf+7,&pwmTestPara,sizeof(pwmTestPara));

    sendBuf[7+sizeof(pwmTestPara)]= 0;  //seed
    sendBuf[8+sizeof(pwmTestPara)]= 0 ;    //  sum
    for(uchar i = 5;i<len-5;i++)
        sendBuf[8+sizeof(pwmTestPara)]+=sendBuf[i];
    dataSendUdpsocket->writeDatagram(reinterpret_cast<const char *>(sendBuf),\
                                     len,QHostAddress(ip),PCBTESTPORT);
}


//恒流指令
void midmachine::sendPcbFixedCurrentCmd(uchar chnNum, DebugParameterBitField pwmTestPara)
{
    uchar len ;
    memset(&pwmTestPara,0,sizeof(pwmTestPara));   //清零
    pwmTestPara.DebugParameterBitField.chnlNum = chnNum;
    pwmTestPara.DebugParameterBitField.stepType = 'K';
    pwmTestPara.DebugParameterBitField.ivloopSel= (0x03<<4)+0x02;
    pwmTestPara.MainPara1=3000;  // 电流
    pwmTestPara.LimitPara1=5000;   //电压
    len = 5+sizeof(pwmTestPara)+2;
    for(uchar j =0;j<4;j++)
        sendBuf[j] = 0xFF;

    sendBuf[4]=len-5;   //长度
    sendBuf[5]=0;   //地址
    sendBuf[6]=0x14; //   下位机启动pwm功能
    mempcpy(sendBuf+7,&pwmTestPara,sizeof(pwmTestPara));

    sendBuf[7+sizeof(pwmTestPara)]= 0;  //seed
    sendBuf[8+sizeof(pwmTestPara)]= 0 ;    //  sum
    for(uchar i = 5;i<len-5;i++)
        sendBuf[8+sizeof(pwmTestPara)]+=sendBuf[i];
    dataSendUdpsocket->writeDatagram(reinterpret_cast<const char *>(sendBuf),\
                                     len,QHostAddress(ip),PCBTESTPORT);
}




//处理主线程接收的数据

void midmachine::slotRcevDataToThread(const QVariant variant)
{
    
    //  QMutexLocker  locker(&mtx);
    QByteArray  buf;
    uchar str[ RECV_BUFF_LEN ]={0};
    QString  addr;
    ushort   tmp;
    dataRecv  para = variant.value<dataRecv>();
    addr = para.addrGet.toString();
    buf = para.dataGet;
    if(addr !=this->ip)
        return;

    memcpy(str,buf.data(),buf.length());
    
    for(ushort i=0;i<buf.length();i++)
    {

        if(this->wait == WAIT_HEAD)    //接收
        {
            if(str[i] == 0xEE)
            {
                if(this->head_cnt == this->recv_cnt)
                {
                    this->recvBuf[this->recv_cnt]=str[i];
                    if(++this->head_cnt>=4)
                        this->wait = WAIT_LEN;
                }
                else
                    goto  err_process;
            }
            else
                goto   err_process;
        }
        else  if(this->wait == WAIT_LEN)
        {
            if(this->recv_cnt == 4)
            {
                this->recvBuf[this->recv_cnt]=str[i];
                this->length = str[i];
            }
            if(this->recv_cnt == 5)
            {
                this->recvBuf[this->recv_cnt]=str[i];
                tmp= str[i];        //
                tmp<<=8;
                this->length|=tmp;
                if((this->length +6)<= BUFF_LEN)
                {
                    this->wait = WAIT_MSG;    //
                }
                else {
                    goto err_process;
                }
            }

        }
        else if(this->wait == WAIT_MSG)
        {
            this->recvBuf[this->recv_cnt]=str[i];
            if(this->recv_cnt>= this->length +6-1)
            {
                this->wait = WAIT_HEAD;
                this->recv_cnt =0;
                this->head_cnt =0;
                this->length = 0;
                this->recvDataMethod(this->recvBuf);
                return ;
            }
        }
        else
            goto   err_process;
        this->recv_cnt++;
        continue;

err_process:
        this->wait = WAIT_HEAD;
        this->recv_cnt = 0;
        this->head_cnt = 0;
        this->length = 0;
        return;
    }
    //  locker.unlock();
}



void midmachine::recvDataMethod(const uchar *data)
{
    ushort len=0;

    uchar cmd;

    //  const  QString str = QString::fromUtf8(data.data(),data.length());
    len = *(data + 4) +(*(data + 5)<<8) ;  //数据长度
    // if(single::m_Instance()->CRC16_Modbus(data+6, len))
    //   return;
    QStringList list;
    list.clear();

    // ff,ff,ff,ff,len,addr,cmd
    cmd = *(data+6);   //命令字
    if(cmd ==0x17)
    {
        if(sampleTouchMutex== 1)  //联机时的采样回复不予理会
            return;
    }
    switch (cmd) {

    case updateCmd:: APPJUMP:   //boot联机
        this->cmdBootConnectAnalysis(data);
        break;
    case  updateCmd::APPERASE:   //擦除
        this->eraseReplyAnalysis(data);
        break;
    case updateCmd:: BOOTWRITEDATA:  //升级

        this->cmdUpgradeAnalysis(data);
        break;
    case updateCmd::APPVERIFY:  //校验

        this->verifyReplyAnalysis(data);
        break;
    case updateCmd::BOOTJUMP:    //运行
        this->cmdRunAnalysis(data);
        break;
    case    0x12      :              // pcba测试联机
        emit  sigRunMsgToUi(ip, 0x12, list,  0);    //告知ui下位机已经 联机成功
        m_bHaveRcvedAck=true;   //联机应答成功
        if(m_testMaster == true)  //测试主控板
        sampleTouchMutex = 0;  //开始采样
        else
        sampleTouchMutex = 2;  //开始发恒流指令
        slotDataSent('S', 0);
        break;
    case   0x17:        //采样电源板下位机数据
          this->pcbTestSampleAnalysis(data);
        break;
    case  0x14:       //pcbPWM测试与恒流测试回复
         this->pcbTestPwmAnalysis(data);
         break;
    default:
        break;
    }

}



//响应主线程状态改变modle
void midmachine::slotItemChange(QString rootIp,QString device)
{

    if(rootIp == this->ip)
    {
        emit itemChange(rootIp,device);
    }

}








//boot跳转
void midmachine::cmdBootConnectAnalysis(const uchar *data)
{
    uchar bedType,addr1,addr2,result;
    QString targetIp,msg;
    QStringList  list;
    list.clear();
    bedType = *(data+8);  //设备类型
    addr1 = *(data+6);  //父节点地址
    addr2 = *(data+9);  //子节点地址
    result = *(data+10);  //联机结果
    this->resendTimer->stop();
    switch (bedType) {
    case MID_MACHINE  :
        if(result == 'O')
        {
            msg="BOOT跳转成功！";
            list<<"BOOT运行中"<<msg;
            this->isBoot = true;  //置为boot状态
        }
        else
        {
            msg="BOOT跳转失败！";
            list<<"APP运行中"<<msg;
            this->isBoot = false;  //置为boot状态
        }
        emit sigRunMsgToUi(this->ip,updateCmd:: APPJUMP,list,0);
        break;
    case  PARENTDEVICE   :   //父节点设备
        targetIp = QString("%1.%2").arg(this->ip).arg(QString::number(addr1));
        if(result == 'O')
        {
            msg="BOOT跳转成功！";
            list<<"BOOT运行中"<<msg;
            parentDeviceList.at(bootItemNumber)->setIsBoot(true);
        }
        else
        {
            msg="BOOT跳转失败！";
            list<<"APP运行中"<<msg;
            parentDeviceList.at(bootItemNumber)->setIsBoot(false);
        }
        emit sigRunMsgToUi(  targetIp,updateCmd:: APPJUMP,list,0);
        bootItemNumber++;
        if(bootItemNumber<parentDeviceList.count())
        {
            this->slotDataSent(updateCmd:: APPJUMP,bedType);
        }
        else
            bootItemNumber=0;
        break;
    case SLAVEBOARD  :
        //回ui,boot联机成功
        targetIp = QString("%1.%2.%3").arg(this->ip).arg(QString::number(addr1)).arg(QString::number(addr2));
        slaveList.at(bootItemNumber)->wrIsBoot(true); //
        if(result == 'O')
        {
            msg="BOOT跳转成功！";
            list<<"BOOT运行中"<<msg;
        }
        else
        {
            msg="BOOT跳转失败！";
            list<<"APP运行中"<<msg;
        }
        emit sigRunMsgToUi(targetIp,updateCmd:: APPJUMP,list,0);
        bootItemNumber++;
        if(bootItemNumber<slaveList.count())
        {
            this->slotDataSent(updateCmd:: APPJUMP,SLAVEBOARD);
        }
        else
            bootItemNumber=0;
        break;

    default:
        break;
    }
}



//擦除命令回复
void midmachine::eraseReplyAnalysis(const uchar *data)
{
    uchar bedType,addr1,addr2,result;
    QString targetIp,msg;
    QStringList  list;
    list.clear();
    bedType = *(data+8);  //设备类型
    addr1 = *(data+6);  //父节点地址
    addr2 = *(data+9);  //子节点地址
    result = *(data+10);  //擦除结果
    this->resendTimer->stop();
    switch (bedType) {
    case MID_MACHINE  :
        this->frameIndex = 0;
        targetIp = QString("%1").arg(this->ip);
        if(result == 'O')  //擦除成功
        {
            msg="flash擦除成功！";
            this->slotDataSent(updateCmd:: BOOTWRITEDATA,bedType);  //发送升级
        }
        else  //
            msg="flash擦除失败！";
        list<<msg;
        emit sigRunMsgToUi(targetIp,updateCmd:: APPERASE,list,0);
        break;

    case PARENTDEVICE   :   //父节点设备
        this->frameIndex = 0;
        targetIp = QString("%1.%2").arg(this->ip).arg(QString::number(addr1));
        if(result == 'O')  //擦除成功
        {
            msg="flash擦除成功！";
            this->slotDataSent(updateCmd:: BOOTWRITEDATA,bedType);  //发送升级
        }
        else  //
        {
            this->itemNumber++;
            if(this->itemNumber<parentDeviceList.count())
            {
                msg="flash擦除失败！";
                this->slotDataSent(updateCmd::APPERASE,bedType);  //发送下一个节点的擦除
            }
            else
            {
                msg="升级失败，升级结束！";
                this->itemNumber = 0;
            }
        }
        list<<msg;
        emit sigRunMsgToUi(targetIp,updateCmd:: APPERASE,list,0);
        break;
    case SLAVEBOARD  :
        this->frameIndex = 0;
        targetIp = QString("%1.%2.%3").arg(this->ip).arg(QString::number(addr1)).arg(QString::number(addr2));
        if(result == 'O')  //擦除成功
        {
            msg="flash擦除成功！";
            this->slotDataSent(updateCmd:: BOOTWRITEDATA,SLAVEBOARD);  //发送升级
        }
        else  //
        {
            this->itemNumber++;
            if(this->itemNumber<slaveList.count())
            {
                msg="flash擦除失败！";
                this->slotDataSent(updateCmd::APPERASE,SLAVEBOARD);  //发送下一个节点的擦除
            }
            else
            {
                msg="升级失败，升级结束！";
                this->itemNumber = 0;
            }
        }
        list<<msg;
        emit sigRunMsgToUi(targetIp,updateCmd:: APPERASE,list,0);
        break;
    default:
        break;
    }
}


//升级命令
void midmachine::cmdUpgradeAnalysis(const uchar *data)
{
    uchar bedType,addr1,addr2,result,index;
    QString targetIp,msg;
    QStringList  list;
    list.clear();
    bedType = *(data+8);  //设备类型
    addr1 = *(data+6);  //父节点地址
    addr2 = *(data+9);  //子节点地址
    index = *(data+10);
    result = *(data+11);
    this->resendTimer->stop();
    if(index != this->frameIndex)
        return;
    switch (bedType) {
    case MID_MACHINE  :
        targetIp = QString("%1").arg(this->ip);
        if(result == 'O')  //升级成功
        {
            this->frameIndex++;
            if(this->frameIndex>=this->totalFrames) //数据发送完成，开始校验
            {

                msg="数据下发完成，开始读回校验！";
                list<<msg;
                emit sigRunMsgToUi(targetIp,updateCmd:: BOOTWRITEDATA,list,  this->frameIndex);
                this->frameIndex = 0;
                this->slotDataSent(updateCmd::APPVERIFY,bedType);  //
            }
            else
            {
                msg = "正在下发app数据！";
                list<<msg;
                this->sendBootData(bedType,0, 0);   //继续下发下一包
                emit sigRunMsgToUi(targetIp,updateCmd:: BOOTWRITEDATA,list,  this->frameIndex);
            }

        }
        else  //下发失败
        {
            msg="升级失败！";
            list<<msg;
            emit sigRunMsgToUi(targetIp,updateCmd:: BOOTWRITEDATA,list, this->frameIndex);
            this->frameIndex = 0;//
        }
        break;
    case  PARENTDEVICE   :   //父节点设备
        targetIp = QString("%1.%2").arg(this->ip).arg(addr1);
        if(result == 'O')  //升级成功
        {
            this->frameIndex++;
            if(this->frameIndex>=this->totalFrames) //数据发送完成，开始校验
            {
                msg="数据下发完成，开始读回校验！";
                list<<msg;
                emit sigRunMsgToUi(targetIp,updateCmd:: BOOTWRITEDATA,list,  this->frameIndex);
                this->frameIndex = 0;
                this->slotDataSent(updateCmd::APPVERIFY,bedType);  //
            }
            else
            {
                msg = "正在下发app数据！";
                list<<msg;
                this->sendBootData(bedType,parentDeviceList.at(itemNumber)->  getParentDeviceAddr(), 0);   //继续下发下一包
                emit sigRunMsgToUi(targetIp,updateCmd:: BOOTWRITEDATA,list,  this->frameIndex);
            }

        }
        else  //下发失败
        {
            this->itemNumber++;
            if(this->itemNumber<parentDeviceList.count())
            {
                this->slotDataSent(updateCmd:: APPERASE,bedType);  //发送下一个节点的擦除
                msg="升级失败！";
            }
            else
            {
                msg="升级失败,升级结束！";
                this->itemNumber = 0;
            }
            list<<msg;
            emit sigRunMsgToUi(targetIp,updateCmd:: BOOTWRITEDATA,list, this->frameIndex);
            this->frameIndex = 0;//
        }
        break;
    case SLAVEBOARD  :

        targetIp = QString("%1.%2.%3").arg(this->ip).arg(QString::number(addr1)).arg(QString::number(addr2));
        if(result == 'O')  //升级成功
        {
            this->frameIndex++;
            if(this->frameIndex>=this->totalFrames) //数据发送完成，开始校验
            {

                msg="数据下发完成，开始读回校验！";
                list<<msg;
                emit sigRunMsgToUi(targetIp,updateCmd:: BOOTWRITEDATA,list,  this->frameIndex);
                this->frameIndex = 0;
                this->slotDataSent(updateCmd::APPVERIFY,SLAVEBOARD);  //
            }
            else
            {
                msg = "正在下发app数据！";
                list<<msg;
                this->sendBootData(bedType,slaveList.at(itemNumber)->  getParentDeviceAddr(),
                                   slaveList.at(itemNumber)->rdPowerSlaveAddr());   //继续下发下一包
                emit sigRunMsgToUi(targetIp,updateCmd:: BOOTWRITEDATA,list,  this->frameIndex);
            }

        }
        else  //下发失败
        {
            this->itemNumber++;
            if(this->itemNumber<slaveList.count())
            {
                this->slotDataSent(updateCmd:: APPERASE,SLAVEBOARD);  //发送下一个节点的擦除
                msg="升级失败！";
            }
            else
            {
                msg="升级失败,升级结束！";
                this->itemNumber = 0;
            }
            list<<msg;
            emit sigRunMsgToUi(targetIp,updateCmd:: BOOTWRITEDATA,list, this->frameIndex);
            this->frameIndex = 0;//
        }
        break;
    default:
        break;
    }
}


//校验命令回复处理
void midmachine::verifyReplyAnalysis(const uchar *data)
{
    uchar bedType,addr1,addr2,index;
    QString targetIp,msg;
    QStringList  list;
    list.clear();
    bedType = *(data+8);  //设备类型
    addr1 = *(data+6);  //父节点地址
    addr2 = *(data+9);  //子节点地址
    index= *(data+10);  //读取回帧的序列
    this->resendTimer->stop();   //关闭重发计时定时器
    switch (bedType) {
    case MID_MACHINE  :
        if(this->frameIndex == index)  //帧序列一致
        {
            if(index == this->totalFrames)  //完全读回所有帧
            {
                memcpy(this->appData+MAX_SEND_SIZE*index,data+11, MAX_SEND_SIZE);   //保存读取回来的数据
                for(qint64 byte = 0;byte<this->totalBytes;byte++ )  //校验
                {
                    if(appData[byte]!=updateBuf[byte])  //校验不通过，升级失败，升级下一个节点
                    {
                        targetIp = QString("%1").arg(this->ip);
                        msg ="升级失败，升级结束！";
                        list<<msg;
                        emit sigRunMsgToUi(targetIp,updateCmd::APPVERIFY,list,index);
                        return;
                    }
                }
                //校验成功
                targetIp = QString("%1").arg(this->ip);
                msg="app校验成功！";
                list<<msg;
                emit sigRunMsgToUi(targetIp,updateCmd::APPVERIFY,list,index);
                return;
            }
            else //继续读
            {
                this->frameIndex++;  //继续往回读
                memcpy(this->appData+MAX_SEND_SIZE*index,data+11, MAX_SEND_SIZE);   //保存读取回来的数据
                this->slotDataSent(updateCmd::APPVERIFY,bedType);
            }
        }
        else
        {
            msg="app校验失败！";
            list<<msg;
            emit sigRunMsgToUi(targetIp,updateCmd::APPVERIFY,list,index);
        }
        break;
    case  PARENTDEVICE   :   //父节点设备
        if(this->frameIndex == index)  //帧序列一致
        {
            if(index == this->totalFrames)  //完全读回所有帧
            {
                memcpy(this->appData+MAX_SEND_SIZE*index,data+11, MAX_SEND_SIZE);   //保存读取回来的数据
                for(qint64 byte = 0;byte<this->totalBytes;byte++ )  //校验
                {
                    if(appData[byte]!=updateBuf[byte])  //校验不通过，升级失败，升级下一个节点
                    {
                        targetIp = QString("%1.%2").arg(this->ip).arg(QString::number(addr1));
                        this->itemNumber++; //到下一个节点
                        if(this->itemNumber<parentDeviceList.count())
                        {
                            msg="app校验失败！";
                            list<<msg;
                            emit sigRunMsgToUi(targetIp,updateCmd::APPVERIFY,list,index);
                            this->slotDataSent(updateCmd::  APPERASE,bedType);  //发送下一节点erase
                            return;
                        }
                        else
                        {
                            msg ="升级失败，升级结束！";
                            list<<msg;
                            emit sigRunMsgToUi(targetIp,updateCmd::APPVERIFY,list,index);
                            this->itemNumber =0;
                            return;
                        }
                    }
                }
                this->itemNumber++; //到下一个节点
                if(this->itemNumber<parentDeviceList.count())
                {
                    //校验成功
                    targetIp = QString("%1.%2").arg(this->ip).arg(QString::number(addr1));
                    msg="app校验成功！";
                    list<<msg;
                    this->slotDataSent(updateCmd::  APPERASE,bedType);  //发送下一节点erase
                    emit sigRunMsgToUi(targetIp,updateCmd::APPVERIFY,list,index);
                    return;
                }
                else
                {
                    //校验成功
                    targetIp = QString("%1.%2").arg(this->ip).arg(QString::number(addr1));
                    msg="app校验成功！";
                    list<<msg;
                    this->itemNumber =0;
                    emit sigRunMsgToUi(targetIp,updateCmd::APPVERIFY,list,index);
                    return;
                }
            }
            else //继续读
            {
                this->frameIndex++;  //继续往回读
                memcpy(this->appData+MAX_SEND_SIZE*index,data+11, MAX_SEND_SIZE);   //保存读取回来的数据
                this->slotDataSent(updateCmd::APPVERIFY,bedType);
            }
        }
        else
        {
            msg="app校验失败！";
            list<<msg;
            emit sigRunMsgToUi(targetIp,updateCmd::APPVERIFY,list,index);
            this->slotDataSent(updateCmd::  APPERASE,bedType);  //发送下一节点cachu
        }
        break;
    case SLAVEBOARD  :
        if(this->frameIndex == index)  //帧序列一致
        {
            if(index == this->totalFrames)  //完全读回所有帧
            {
                memcpy(this->appData+MAX_SEND_SIZE*index,data+11, MAX_SEND_SIZE);   //保存读取回来的数据
                for(qint64 byte = 0;byte<this->totalBytes;byte++ )  //校验
                {
                    if(appData[byte]!=updateBuf[byte])  //校验不通过，升级失败，升级下一个节点
                    {
                        targetIp = QString("%1.%2.%3").arg(this->ip).arg(QString::number(addr1)).arg(QString::number(addr2));
                        this->itemNumber++; //到下一个节点
                        if(this->itemNumber<slaveList.count())
                        {
                            msg="app校验失败！";
                            list<<msg;
                            emit sigRunMsgToUi(targetIp,updateCmd::APPVERIFY,list,index);
                            this->slotDataSent(updateCmd::  APPERASE,SLAVEBOARD);  //发送下一节点erase
                            return;
                        }
                        else
                        {
                            msg ="升级失败，升级结束！";
                            list<<msg;
                            emit sigRunMsgToUi(targetIp,updateCmd::APPVERIFY,list,index);
                            this->itemNumber =0;
                            return;
                        }
                    }
                }
                this->itemNumber++; //到下一个节点
                if(this->itemNumber<slaveList.count())
                {
                    //校验成功
                    targetIp = QString("%1.%2.%3").arg(this->ip).arg(QString::number(addr1)).arg(QString::number(addr2));
                    msg="app校验成功！";
                    list<<msg;
                    this->slotDataSent(updateCmd::  APPERASE,SLAVEBOARD);  //发送下一节点erase
                    emit sigRunMsgToUi(targetIp,updateCmd::APPVERIFY,list,index);
                    return;
                }
                else
                {
                    //校验成功
                    targetIp = QString("%1.%2.%3").arg(this->ip).arg(QString::number(addr1)).arg(QString::number(addr2));
                    msg="app校验成功！";
                    list<<msg;
                    this->itemNumber =0;
                    emit sigRunMsgToUi(targetIp,updateCmd::APPVERIFY,list,index);
                    return;
                }
            }
            else //继续读
            {
                this->frameIndex++;  //继续往回读
                memcpy(this->appData+MAX_SEND_SIZE*index,data+11, MAX_SEND_SIZE);   //保存读取回来的数据
                this->slotDataSent(updateCmd::APPVERIFY,SLAVEBOARD);
            }
        }
        else
        {
            msg="app校验失败！";
            list<<msg;
            emit sigRunMsgToUi(targetIp,updateCmd::APPVERIFY,list,index);
            this->slotDataSent(updateCmd::  APPERASE,SLAVEBOARD);  //发送下一节点boot联机
        }
        break;
    default:
        break;
    }
}




//运行
void midmachine::cmdRunAnalysis(const uchar *data)
{
    uchar bedType,addr1,addr2,result;
    QString targetIp,msg;
    QStringList  list;
    list.clear();
    bedType = *(data+8);  //设备类型
    addr1 = *(data+6);  //父节点地址
    addr2 = *(data+9);  //子节点地址
    result = *(data+10);  //联机结果
    this->resendTimer->stop();
    switch (bedType) {
    case MID_MACHINE  :
        if(result == 'O')  //运行成功
        {
            msg="APP运行成功！";
            list<<"APP运行中"<<msg;
            this->isBoot=false; //
            emit sigRunMsgToUi(this->ip,updateCmd:: BOOTJUMP ,list,1);
        }
        else
        {
            msg="APP运行失败！";
            list<<"BOOT运行中"<<msg;
            this->isBoot=true; //
            emit sigRunMsgToUi(this->ip,updateCmd:: BOOTJUMP ,list,0);
        }
        break;
    case   PARENTDEVICE   :   //父节点设备
        if(result == 'O')  //运行成功
        {
            msg="APP运行成功！";
            list<<"APP运行中"<<msg;
            parentDeviceList.at(  this->runItemNumber)->setIsBoot(false); //
        }
        else
        {
            msg="APP运行失败！";
            list<<"BOOT运行中"<<msg;
            parentDeviceList.at(  this->runItemNumber)->setIsBoot(true); //
        }
        targetIp = QString("%1.%2").arg(this->ip).arg(QString::number(addr1));
        emit sigRunMsgToUi(targetIp,updateCmd:: BOOTJUMP ,list,0);
        this->runItemNumber++;
        if(this->runItemNumber<parentDeviceList.count())
        {
            this->slotDataSent(updateCmd:: BOOTJUMP,bedType);  //发送下一个节点的运行指令
        }
        else
        {
            msg="升级完成！";    list<<msg; emit sigRunMsgToUi(targetIp,updateCmd:: BOOTJUMP ,list,1);
            this->runItemNumber= 0;
        }
        break;
    case SLAVEBOARD  :

        if(result == 'O')  //运行成功
        {
            msg="APP运行成功！";
            list<<"APP运行中"<<msg;
            slaveList.at(  this->runItemNumber)->wrIsBoot(false); //
        }
        else
        {
            msg="APP运行失败！";
            list<<"BOOT运行中"<<msg;
            slaveList.at(  this->runItemNumber)->wrIsBoot(true); //
        }
        targetIp = QString("%1.%2.%3").arg(this->ip).arg(QString::number(addr1)).arg(QString::number(addr2));
        emit sigRunMsgToUi(targetIp,updateCmd:: BOOTJUMP ,list,0);
        this->runItemNumber++;
        if(this->runItemNumber<slaveList.count())
        {
            this->slotDataSent(updateCmd:: BOOTJUMP,SLAVEBOARD);  //发送下一个节点的运行指令
        }
        else
        {
            msg="升级完成！";    list<<msg; emit sigRunMsgToUi(targetIp,updateCmd:: BOOTJUMP ,list,1);
            this->runItemNumber= 0;
        }
        break;
    default:
        break;
    }
}


//pcbtest采样回复处理
void midmachine::pcbTestSampleAnalysis(const uchar *data)
{

    QString batVol1,current1,batVol2,current2,busVoltage,capVoltage,portVol,errorCode,batemp,filePath;
    QStringList list,headList;

    LowVoltageDCDCSampleDataStruct   sampleData;
    list.clear();
    m_bHaveRcvedAck=true;
    memset(&sampleData,sizeof(sampleData),0);
    memcpy(&sampleData.stepType,data+7,sizeof(sampleData));
    //去ui显示
    errorCode =  QString("%1").arg(sampleData.DDC );//异常码
   portVol =  QString("%1").arg(sampleData.portVol/100 );//端口电压
    batVol1  = QString("%1").arg(sampleData.batVoltage1/100)      ; //电池电压
    current1 = QString("%1").arg( fabsf(sampleData.current1/100 ) )  ; //霍尔电流
    batVol2 =  QString("%1").arg(sampleData.batVoltage2/100);//第二路电压
    current2 = QString("%1").arg(0) ;    //电感电流
    busVoltage = QString("%1").arg(sampleData.busVoltage/100 ); //母线电压
    capVoltage = QString("%1").arg(sampleData.outVoltage/100 );//电容电压

    list.append(QString::number( m_curSmplChNo));    //通道号
    list.append(batVol1 );   //电池电压
    list.append(batVol2);  //第二路电压
    list.append(portVol);  //  端口电压
    list.append(capVoltage);//电容电压
    list.append(current1);//霍尔电流
    list.append(current2);   //电感电流
    list.append(busVoltage);  //母线电压
    list.append(batemp);  //温度
    list.append(errorCode );   //异常码

    emit  sigRunMsgToUi(ip, 'S', list,  0); //发送
      //作判定
   pcbTestDataJudge(&sampleData);
    //写excle
//    if(!chnNumList.contains(QString::number(m_curSmplChNo)))
//    {
//        chnNumList.append(QString::number( m_curSmplChNo));
//        if(!this->pcbTestXlsx->selectSheet("iii"))
//        {
//            headList<<"条码"<<"通道"<<"电池电压(mV)"<<"第二路电压(mV)"<<"端口电压(mV)"<<"电容电压(mV)"\
//                   <<"霍尔电流(mA)"<<"电感电流(mA)"<<"母线电压(mV)"<<"温度(℃)"<<"故障码"<<"测试结果";
//            this->pcbTestXlsx->addSheet("iii");
//            for(uchar i= 0;i<12;i++)
//                this->pcbTestXlsx ->write(1,i+1,headList.value(i));
//        }
//        for(uchar i= 0;i<12;i++)
//        {
//            if(i == 0)
//            {

//                this->pcbTestXlsx ->write( curRowNum+1,i+1,this->pcbQrCode);
//            }
//            else if(i==11)
//            {
//                 this->pcbTestXlsx->write( curRowNum+1,i+1,pcbJudgeResult.value(m_curSmplChNo));
//            }
//            else
//            {
//                this->pcbTestXlsx->write( curRowNum+1,i+1,list.value(i-1));
//            }
//        }

//     //   this->pcbTestXlsx ->write( 1,15,curRowNum);
//        curRowNum++;
//        filePath=QString("%1%2%3%4%5").arg(topleverDirPath  ).arg("/").arg( this->testResultSavePath).arg("/").arg("test.xlsx") ;
//        this->pcbTestXlsx->saveAs( filePath);
   // }

}

void midmachine::pcbTestDataJudge(LowVoltageDCDCSampleDataStruct *ptrSampleData)
{
    float pcbCur1 =(float)(ptrSampleData->current1/100);

    if((((ptrSampleData->batVoltage1/100)<m_voltageOutMin))||((ptrSampleData->batVoltage1/100)>m_voltageOutMax))//输出 电压
    {
           pcbJudgeResult<<  "FAIL";
           return;
    }

    if(( fabsf(pcbCur1) <m_hallCurOutMin)||fabsf(pcbCur1)>m_hallCurOutMax)//霍尔电流
    {
          pcbJudgeResult<<  "FAIL";
           return;
    }

//    if((ptrSampleData->current2/100 <m_inducerCurOutMin)||ptrSampleData->current2/100>m_inducerCurOutMax)//电感电流
//    {
//            pcbJudgeResult<<  "FAIL";
//           return;
//    }

    if((ptrSampleData->batVoltage2 /100<m_secondVoltageOutMin)||ptrSampleData->batVoltage2/100>m_secondVoltageOutMax)//
    {
            pcbJudgeResult<<  "FAIL";
           return;
    }

    if((ptrSampleData->busVoltage/100 <m_busVolOutMin)||ptrSampleData->busVoltage/100>m_busVolOutMax)//
    {
             pcbJudgeResult<<  "FAIL";
             return;
    }

    if((ptrSampleData->outVoltage/100 <m_capVolOutMin)||ptrSampleData->outVoltage/100>m_capVolOutMax)//
    {
           pcbJudgeResult<<  "FAIL";
           return;
    }
    if((ptrSampleData->portVol/100 <m_portVolMin)||ptrSampleData->portVol/100>m_portVolMax)//temp
    {
           pcbJudgeResult<<  "FAIL";
           return;
    }

    pcbJudgeResult<<  "PASS";


}


//pwm测试回复处理
void midmachine::pcbTestPwmAnalysis(const uchar *data)
{
        m_bHaveRcvedAck=true;
}








//程序的备份
void midmachine::backUpFile(QString filename)
{
    QFile  f(filename);
    f.open(QIODevice::WriteOnly);
    QDataStream  qs(&f);
    for(quint64 i = 0;i<this->BytesIn;i++)
    {
        qs<<*(this->appData + i);
    }
    f.close();
}


//pcb测试表初始化
void midmachine::pcbTestXlsxInit()
{

//    QString   filePath;
//    this->pcbTestXlsx  =    new    QXlsx::Document("test.xlsx",this);  //打开一个boot3的文件
//    QXlsx::Format  format3;   //建立样式
//    format3.setFontColor(QColor(Qt::red));
//    format3.setPatternBackgroundColor(QColor(152,251,152));//北京颜色
//    format3.setFontSize(10); //设置字体大小
//    format3.setHorizontalAlignment(QXlsx::Format::AlignHCenter);//设置对齐方式
//    format3.setBorderStyle(QXlsx::Format::BorderDashDotDot);//边框样式
//    pcbTestXlsx->write("A2","hello QQ",format3);
//    pcbTestXlsx->write(5,1,222);
//    filePath=QString("%1%2%3%4%5").arg(topleverDirPath  ).arg("/").arg( this->testResultSavePath).arg("/").arg("test.xlsx") ;
//    pcbTestXlsx->saveAs(filePath);
//    curRowNum = 1;

}


//
void midmachine::timeOutMethodSlot()
{
    DebugParameterBitField pwmTestPara;
    QStringList  list;
    list.clear();
    if(m_testMaster)  //如果是测试主控制板
    {
        if(sampleTouchMutex == 1)  //联机
        {

           // sendPcbPwmTestCommand(1, pwmTestPara);
            if(m_bHaveRcvedAck)
            {
                m_timeout=0;
                m_bHaveRcvedAck=false;
            }
            else
            {
                m_timeout++;
                if(m_timeout>3)
                {
                    //超时，停止测试，通知界面掉线
                   this->midmcTimer->stop();
                   emit sigRunMsgToUi(ip,0x12,list,1);   //联机失败
                }
            }
            sendPcbConnectCommand();  //发送联机
        }

        else if(sampleTouchMutex == 0)                  //采样
        {
            if(m_bHaveRcvedAck)
            {
                m_curSmplChNo++;
                if(m_curSmplChNo>3)
                {
                    //结束测试   发送测试结果
                    m_curSmplChNo=-1;
                  //  this->midmcTimer->stop();
                  //  emit sigRunMsgToUi(ip,'S',pcbJudgeResult,1);   //
                    sampleTouchMutex = 2;
                    return;
                }
                m_timeout=0;
                m_bHaveRcvedAck=false;
            }
            else
            {
                m_timeout++;
                if(m_timeout>3)
                {
                    //超时，停止测试，通知界面掉线
                    this->midmcTimer->stop();
                    emit sigRunMsgToUi(ip,'S',list,1);   //通信失败
                }
            }
            sendPcbTestCommand(m_curSmplChNo);  //采样

        }
        else if(sampleTouchMutex == 2)
        {
            if(m_bHaveRcvedAck)
            {
                m_curSmplChNo++;
                if(m_curSmplChNo>3)
                {
                    //结束测试   发送测试结果
                    m_curSmplChNo=-1;
                    this->midmcTimer->stop();
                    emit sigRunMsgToUi(ip,0x14,pcbJudgeResult,1);   //通信失败
                }
                m_timeout=0;
                m_bHaveRcvedAck=false;
            }
            else
            {
                m_timeout++;
                if(m_timeout>3)
                {
                    //超时，停止测试，通知界面掉线
                    this->midmcTimer->stop();
                    emit sigRunMsgToUi(ip,0x14,list,1);   //通信失败
                }
            }
            sendPcbPwmTestCommand(m_curSmplChNo, pwmTestPara);//pwm测试
        }
    }
    else   //功率板
    {
        if(sampleTouchMutex == 1)  //联机
        {

           // sendPcbPwmTestCommand(1, pwmTestPara);
            if(m_bHaveRcvedAck)
            {
                m_timeout=0;
                m_bHaveRcvedAck=false;
            }
            else
            {
                m_timeout++;
                if(m_timeout>3)
                {
                    //超时，停止测试，通知界面掉线
                   this->midmcTimer->stop();
                   emit sigRunMsgToUi(ip,0x12,list,1);   //联机失败
                }
            }
            sendPcbConnectCommand();  //发送联机
        }

        else if(sampleTouchMutex == 0)                  //采样
        {
            if(m_bHaveRcvedAck)
            {
                m_curSmplChNo++;
                if(m_curSmplChNo>1)
                {
                    //结束测试   发送测试结果
                    m_curSmplChNo=-1;
                    this->midmcTimer->stop();
                    emit sigRunMsgToUi(ip,0x14,pcbJudgeResult,1);
                    return;
                }
                m_timeout=0;
                m_bHaveRcvedAck=false;
            }
            else
            {
                m_timeout++;
                if(m_timeout>3)
                {
                    //超时，停止测试，通知界面掉线
                    this->midmcTimer->stop();
                    emit sigRunMsgToUi(ip,'S',list,1);   //通信失败
                }
            }
            sendPcbTestCommand(m_curSmplChNo);  //采样

        }
        else if(sampleTouchMutex == 2)
        {
            if(m_bHaveRcvedAck)
            {
                m_curSmplChNo++;
                if(m_curSmplChNo>1)
                {
                    //恒流发完，开始采样
                    m_curSmplChNo=-1;
                   sampleTouchMutex = 0;
                     this->midmcTimer->stop();
                  this->midmcTimer->start(2000);
                   return;
                }
                m_timeout=0;
                m_bHaveRcvedAck=false;
            }
            else
            {
                m_timeout++;
                if(m_timeout>3)
                {
                    //超时，停止测试，通知界面掉线
                 //   this->midmcTimer->stop();
                  //  emit sigRunMsgToUi(ip,0x14,list,1);   //通信失败
                }
            }
            sendPcbFixedCurrentCmd(m_curSmplChNo, pwmTestPara);//恒流
        }
    }



}


















