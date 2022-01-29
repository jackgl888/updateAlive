#include "pcbatest.h"




/**********************************************
ui->pushButton->setStyleSheet("QPushButton{background-color:rgba(255,178,0,100%);\
                                            color: white;   border-radius: 10px;  border: 2px groove gray; \
                         border-style: outset;}" // 按键本色
        "QPushButton:hover{background-color:white; color: black;}"  // 鼠标停放时的色彩
        "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }" //按下
 * *********************************************/
/***********************************************************
 * botton 设置背景pic
 * 1.self.button.setStyleSheet("QPushButton{border-image: url(img/1.png)}")
 * 2.self.button.setStyleSheet("QPushButton{border-image: url(img/1.png)}"
                          "QPushButton:hover{border-image: url(img/1_1.png)}"
                          "QPushButton:pressed{border-image: url(img/1_1.png)}")
 * ************************************************************/
/***************************************************
 * qlable  set 背景  pic
 *     QPixmap   pixmap(":/new/res/filesave.png") ;
    savecfgBtn->setFixedSize(50,50);
 *
 * *************************************************************/
pcbaTest::pcbaTest(QWidget *parent) : QWidget(parent)
{
    //get  current widget  object
 //    pcbWidget = dynamic_cast<Widget *>(parent);
     makeOnce = 0;  //线程保护
     mutexUdpMake = true;  //创建一次udp
     connectTimeOut = 1;
 //  uchar j=  pcbWidget->connectTimeOut ;

     this->pool = new QThreadPool(this);
     this->pool->setMaxThreadCount(10);

    this->tableModeInit();
    this->tableViewInit();
     
    this->configBoxInit();

    this->netCfgInit();

    this->openCfgBtn  = new QPushButton(this);
    this->openCfgBtn->setFixedSize(60,60);
    this->openCfgBtn->setStyleSheet("QPushButton{border-image: url(:/new/res/fileopen.png); border-style: outset;}"
                                "QPushButton:hover{border-image: url(:/new/res/fileopen.png);background-color:rgb(255, 128, 255); color: black;}"
                                "QPushButton:pressed{border-image: url(:/new/res/fileopen.png);background-color:black;\
                       border-style: inset;}");
   this->saveCfgBtn = new QPushButton(this);
   this->saveCfgBtn->setFixedSize(60,60);
   this->saveCfgBtn->setStyleSheet("QPushButton{border-image: url(:/new/res/filesave.png); border-style: outset;}"
                                   "QPushButton:hover{border-image: url(:/new/res/filesave.png);background-color:rgb(255, 128, 255); color: black;}"
                                   "QPushButton:pressed{border-image: url(:/new/res/filesave.png);background-color:black;\
                          border-style: inset;}");

    QHBoxLayout  *cfgFileOperateLayout  = new QHBoxLayout();

    cfgFileOperateLayout->addWidget(openCfgBtn);
    cfgFileOperateLayout->addStretch();
     cfgFileOperateLayout->addWidget(saveCfgBtn);

    QGroupBox  *cfgFileOperateBox = new QGroupBox(this);
    cfgFileOperateBox->setTitle("配置文件");
    cfgFileOperateBox->setFixedSize(250,100);
    cfgFileOperateBox->setLayout(cfgFileOperateLayout);

#if 0
    this->netStatus = new QLabel("脱机",this);
    this->statusShow = new QLabel(this);
    this->statusShow->setFixedHeight(15);
    this->statusShow->setStyleSheet("QLabel{background-color:rgb(255,0,0);}");


    QHBoxLayout *showNetLayout = new QHBoxLayout();

    showNetLayout->addSpacing(1);

    showNetLayout->addWidget(netStatus);
    showNetLayout->addWidget(statusShow);
    QGroupBox *netStateBox = new QGroupBox("联机状态",this);
    netStateBox->setStyleSheet("QGroupBox{border: 0.5px solid black;\
                                 border-radius:10px;\
                                margin-top:1ex; }");
    netStateBox->setLayout(showNetLayout);
    this->startTestBtn = new QPushButton("测试",this);
    this->startTestBtn->setStyleSheet("QPushButton{background-color:rgba(255,178,0,100%);\
                                      color: white;   border-radius: 10px;  border: 2px groove gray; \
    border-style: outset;}"\
    "QPushButton:hover{background-color:white; color: black;}"\
    "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");
    QFont font("STFangsong",18);

    this->startTestBtn->setFixedHeight(200);
    this->startTestBtn->setFixedWidth(200);
    this->startTestBtn->setFont(font);
    this->startTestBtn->setEnabled(false);


    QVBoxLayout *showStartLayout  =  new QVBoxLayout();
    showStartLayout->addWidget(netStateBox);
    showStartLayout->addSpacing(50);

    showStartLayout->addWidget(startTestBtn);
    showStartLayout->setMargin(1);

#endif

    QGridLayout  *cfgBoxLayout   = new QGridLayout();
    cfgBoxLayout->setSpacing(2);

    cfgBoxLayout->addWidget(this->volOutBox,0,0,3,2);
    cfgBoxLayout->addWidget(this->tempBox,3,0,3,2);
    cfgBoxLayout->addWidget(this->secondVolOutBox,6,0,3,2);
    cfgBoxLayout->addWidget(this->innerVolOutBox,9,0,3,2);
    cfgBoxLayout->addWidget(this->portVolBox,0,4,3,2);
    cfgBoxLayout->addWidget(this->portVolOutBox,3,4,3,2);

    cfgBoxLayout->addWidget(this->outputCurrentBox,6,4,3,2);
    cfgBoxLayout->addWidget(this-> hallCurrentBox,9,4,3,2);

     cfgBoxLayout->addWidget(this->qrCodeLenBox,0,10,3,2);
    cfgBoxLayout->addWidget(this->radioBtnBox,4,10,3,2);
    cfgBoxLayout->addWidget(netCfgBox,8,10,3,2);

    //文件操作
    cfgBoxLayout->addWidget(cfgFileOperateBox,0,15,4,3);

   //信息打印窗口
     msgBrowserInit();
    QHBoxLayout *msgLayout = new QHBoxLayout();
    msgLayout->addWidget(msgBrowser);

   QGroupBox *browserBox  =new QGroupBox("消息打印",this);
   browserBox->setFixedHeight(350);
     browserBox->setLayout(msgLayout);

    QGroupBox *bottomBox =new QGroupBox(this);
    bottomBox->setTitle("参数配置");
    bottomBox->setFixedHeight(350);
    bottomBox->setStyleSheet("QGroupBox{border: 1px solid black;\
                          border-radius:10px;\
                         margin-top:1ex; }");

    bottomBox->setLayout(cfgBoxLayout);

    QHBoxLayout   *bottomLayout = new  QHBoxLayout();
    bottomLayout->setSpacing(2);
    bottomLayout->addWidget(browserBox);
    bottomLayout->addWidget(bottomBox);
   // bottomLayout->addLayout(showStartLayout);

    QVBoxLayout *mainLayout = new  QVBoxLayout(this);
    mainLayout->addWidget(testDataView);
    mainLayout->addLayout(bottomLayout);


    //创建udp
    this->pcbUdpsocket  = new QUdpSocket(this);
    this->pcbTimer = new QTimer(this);
    this->pcbTimer->start(500);
    this->connectTimeOut =2;
    connect(pcbUdpsocket,SIGNAL(readyRead()),this,SLOT(dataRecved()));  //udp数据接收
    connect(pcbTimer,SIGNAL(timeout()),this,SLOT(timeoutMethodSlot()));  //timer
    connect(this->openCfgBtn,SIGNAL(clicked()),this,SLOT(openCfgSlot()));  //打开配置
   // connect(this->startTestBtn,SIGNAL(clicked()),this,SLOT(startSampleSlot()));   //开始采样
    connect(saveCfgBtn,SIGNAL(clicked()),this,SLOT(saveBtnClickedSlot()));      //保存按下


}

pcbaTest::~pcbaTest()
{

}




/**************************************************************************************************

font.setFamily("KaiTi");//楷体
    ui->txtEdit->setFont(font);//设置
    //其他常见样式：宋体 SimSun，黑体 SimHei，微软雅黑 Microsoft YaHei
    //新宋体 NSimSun，细明体 MingLiU，标楷体 DFKai-SB，仿宋 FangSong
    //楷体 KaiTi，仿宋_GB2312 FangSong_GB2312，楷体_GB2312 KaiTi_GB2312

QFont TextFont = ui->PlainTextEdit->font();//获取font
    bool ok = false;
    QFont Font = QFontDialog::getFont(&ok,TextFont);//选择字体，getFont无返回量，以变量ok作为是否选择成功标志
    if(ok)
    {
        ui->PlainTextEdit->setFont(Font);
    }
*************************************************************************************************/
void pcbaTest::tableModeInit()
{
    QString  str;
    QStringList   list;
    QStandardItem  *item;
    testDataModel  = new QStandardItemModel(this);
    this->testDataModel =   new  QStandardItemModel(this);
    this->testDataModel->setColumnCount(12);
    this->testDataModel->setRowCount(4);
    QFont font("FangSong",9);
         font.setBold(true);
    list <<"条码"<<"通道"<<"电池电压(mV)"<<"探针电压(mV)"<<"端口电压(mV)"<<"电容电压(mV)"<<"霍尔电流(mA)"\
        <<"电感电流(mA)"<<"母线电压(mV)"<<"温度(℃)"<<"故障码"<<"测试结果";
    this->testDataModel->setHorizontalHeaderLabels(list);
    for(uchar i = 0;i<list.count();i++)
   {
         this->testDataModel->horizontalHeaderItem(i)->setFont(font);
         this->testDataModel->horizontalHeaderItem(i)->setTextAlignment(Qt::AlignCenter);
    }

    for(uchar i = 0;i<12;i++)
    {

        if(i == 0)  //条码
        {
            QFont font("FangSong",9);
            font.setBold(true);
            for(uchar j = 0;j<4;j++)
            {

                item = new QStandardItem ("1234567892568");
                item->setTextAlignment(Qt::AlignCenter);
                item->setFont(font);
              //  item->setForeground(QBrush(QColor(255,0,0)));
                this->testDataModel->setItem(j,i,item);
            }
        }
        else  if(i == 1)
        {
            QFont font("FangSong",18);

            for(uchar j = 0;j<4;j++)
            {
                str=QString("%1%2").arg("CH").arg(QString::number(j));
                item = new QStandardItem (str);
                item->setTextAlignment(Qt::AlignCenter);
                item->setFont(font);
              //  item->setForeground(QBrush(QColor(0,210,0)));
                this->testDataModel->setItem(j,i,item);
            }

        }
        else if((i>1)&&(i<6)) //电压item
        {
            QFont font("FangSong",9);
             font.setBold(true);
            for(uchar j = 0;j<4;j++)
            {
                item = new QStandardItem ("0.0000");
                item->setFont(font);
                item->setTextAlignment(Qt::AlignCenter);
                item->setForeground(QBrush(QColor(0,128,64)));//(255,128,64)));
                this->testDataModel->setItem(j,i,item);
            }

        }
        else if((i>5)&&(i<8))  //电流item
        {
            QFont font("FangSong",9);
             font.setBold(true);
            for(uchar j = 0;j<4;j++)
            {
                item = new QStandardItem ("0.0000");
                item->setFont(font);
                item->setForeground(QBrush(QColor(0,128,255)));
                item->setTextAlignment(Qt::AlignCenter);
                this->testDataModel->setItem(j,i,item);
            }
        }
        else
        {
            QFont font("FangSong",18);
             font.setBold(true);
            if(11 == i)
            {
                for(uchar j = 0;j<4;j++)
                {

                    item = new QStandardItem ("FAIL");
                    item->setForeground(QBrush(QColor(255,0,0)));
                    item->setFont(font);
                    item->setTextAlignment(Qt::AlignCenter);
                    this->testDataModel->setItem(j,i,item);

                }
            }
            else if(8==i)  //母线电压
            {
                 QFont font("FangSong",9);
                  font.setBold(true);
                for(uchar j = 0;j<4;j++)
                {

                    item = new QStandardItem ("0.0000");
                    item->setFont(font);
                    item->setTextAlignment(Qt::AlignCenter);
                    item->setForeground(QBrush(QColor(0,128,64)));
                    this->testDataModel->setItem(j,i,item);

                }
            }
            else if(9==i)  //温度
            {
                QFont font("FangSong",9);
                 font.setBold(true);
               for(uchar j = 0;j<4;j++)
               {

                   item = new QStandardItem ("0.0");
                   item->setFont(font);
                   item->setTextAlignment(Qt::AlignCenter);
                   item->setForeground(QBrush(QColor(128,64,0)));
                   this->testDataModel->setItem(j,i,item);

               }
            }
            else
            {
                 QFont font("FangSong",9);
                  font.setBold(true);
                for(uchar j = 0;j<4;j++)
                {

                       item = new QStandardItem ("");
                    item->setFont(font);
                    item->setTextAlignment(Qt::AlignCenter);
                    this->testDataModel->setItem(j,i,item);

                }
            }
        }

    }
}


//
void pcbaTest::tableViewInit()
{
    this->testDataView  = new QTableView(this);
      this->testDataView->setModel(testDataModel);
    this->testDataView ->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置表格内容不可编辑
    this->testDataView ->horizontalHeader()->setDefaultAlignment(Qt::  AlignVCenter);
   this->testDataView ->horizontalHeader()->setSectionResizeMode(QHeaderView::  Stretch);
    this->testDataView ->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->testDataView->verticalHeader()->setVisible(true);
    this->testDataView->horizontalHeader()->setVisible(true);
   // this->testDataView->setAlternatingRowColors(true);
    this->testDataView->verticalHeader()->setStyleSheet("QHeaderView::section {background-color: rgb(200, 227, 255)}");
    this->testDataView->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color: rgb(200, 227, 255)}");
    this->testDataView->setGridStyle(Qt:: SolidLine);
    this->testDataView->setStyleSheet("QTableView{ color:black;  \
                                             gridline-color: black;  \
                                                  background-color: rgb(168, 228, 255);}");
   //this->testDataView->setColumnWidth(0,200);
  // this->testDataView->setColumnWidth(1,150);


}


//
void pcbaTest::configBoxInit()
{
    //电池电压

    this->voltageOutMin = new QSpinBox(this);
    this->voltageOutMin->setMaximum(50000);
    this->voltageOutMin->setSuffix("mV");
    this->voltageOutMin->setValue(0);
    this->voltageOutMax = new QSpinBox(this);
    this->voltageOutMax->setMaximum(90000);
    this->voltageOutMax->setValue(3000);
    this->voltageOutMax->setSuffix("mV");
    QLabel   *line1 = new QLabel("->",this);
    line1->setAlignment(Qt::AlignCenter);
    line1->setFixedWidth(50);
    QHBoxLayout  *voloutLayout  = new QHBoxLayout();
    voloutLayout->addWidget(this->voltageOutMin);
    voloutLayout->addWidget(line1);
    voloutLayout->addWidget(this->voltageOutMax);
    volOutBox = new QGroupBox(this);
    volOutBox->setTitle("电池电压");
    volOutBox->setLayout(voloutLayout);
    volOutBox->setFixedSize(300,70);

    //探针电压

    this->secondVoltageOutMin = new QSpinBox(this);
    this->secondVoltageOutMin->setMaximum(50000);
    this->secondVoltageOutMin->setValue(0);
    this->secondVoltageOutMin->setSuffix("mV");
    this->secondVoltageOutMax = new QSpinBox(this);
    this->secondVoltageOutMin->setValue(3000);
    this->secondVoltageOutMax->setMaximum(90000);
    this->secondVoltageOutMax->setSuffix("mV");
    QLabel   *line2 = new QLabel("->",this);
    line2->setAlignment(Qt::AlignHCenter);
    line2->setFixedWidth(50);
    QHBoxLayout  *secondVoloutLayout  = new QHBoxLayout();
    secondVoloutLayout ->addWidget(this->secondVoltageOutMin);
    secondVoloutLayout ->addWidget(line2);
    secondVoloutLayout ->addWidget(this->secondVoltageOutMax);
    secondVolOutBox = new QGroupBox(this);
    secondVolOutBox->setTitle("探针电压");
    secondVolOutBox->setLayout(  secondVoloutLayout);
    secondVolOutBox->setFixedSize(300,70);
    
    //母线电压
    this->busVolOutMin = new QSpinBox(this);
    this->busVolOutMin->setMaximum(55000);
    this->busVolOutMin->setValue(0);
    this->busVolOutMin->setSuffix("mV");
    this->busVolOutMax = new QSpinBox(this);
    this->busVolOutMax->setValue(3000);
    this->busVolOutMax->setMaximum(90000);
    this->busVolOutMax->setSuffix("mV");
    QLabel   *line3 = new QLabel("->",this);
    line3->setAlignment(Qt::AlignCenter);
    line3->setFixedWidth(50);
    QHBoxLayout  *portVoloutLayout  = new QHBoxLayout();
    portVoloutLayout ->addWidget(this->busVolOutMin);
    portVoloutLayout ->addWidget(line3);
    portVoloutLayout ->addWidget(this->busVolOutMax);
    portVolOutBox = new QGroupBox(this);
    portVolOutBox->setTitle("母线电压");
    portVolOutBox->setLayout(  portVoloutLayout);
    portVolOutBox->setFixedSize(300,70);;
    //电容电压
    
    this->capVolOutMin = new QSpinBox(this);
    this->capVolOutMin->setMaximum(50000);
    this->capVolOutMin->setValue(3000);
    this->capVolOutMin->setSuffix("mV");
    this->capVolOutMax = new QSpinBox(this);

    this->capVolOutMax->setMaximum(90000);
    this->capVolOutMax->setValue(3000);
    this->capVolOutMax->setSuffix("mV");
    QLabel   *line4 = new QLabel("  ->  ",this);
    line4->setAlignment(Qt::AlignCenter);
    line4->setFixedWidth(50);
    QHBoxLayout  *innerVoloutLayout  = new QHBoxLayout();
    innerVoloutLayout ->addWidget(this->capVolOutMin);
    innerVoloutLayout ->addWidget(line4);
    innerVoloutLayout ->addWidget(this->capVolOutMax);
    innerVolOutBox = new QGroupBox(this);
    innerVolOutBox->setTitle("电容电压");
    innerVolOutBox->setLayout(  innerVoloutLayout);
    innerVolOutBox->setFixedSize(300,70);;

    //霍尔电流

    this->hallCurOutMin = new QSpinBox(this);
    this->hallCurOutMin->setMaximum(50000);
    this->hallCurOutMin->setValue(3000);
    this->hallCurOutMin->setSuffix("mA");
    this->hallCurOutMax = new QSpinBox(this);

    this->hallCurOutMax->setMaximum(90000);
    this->hallCurOutMax->setValue(3000);
    this->hallCurOutMax->setSuffix("mA");
    QLabel   *line5 = new QLabel("  ->  ",this);
    line5->setAlignment(Qt::AlignCenter);
    line5->setFixedWidth(50);
    QHBoxLayout  *outputCurLayout  = new QHBoxLayout();
    outputCurLayout  ->addWidget(this->hallCurOutMin);
    outputCurLayout  ->addWidget(line5);
    outputCurLayout  ->addWidget(this->hallCurOutMax);
    outputCurrentBox = new QGroupBox(this);
    outputCurrentBox->setTitle("霍尔电流");
    outputCurrentBox->setLayout( outputCurLayout);
    outputCurrentBox->setFixedSize(300,70);;

    //电感电流


    this->inducerCurOutMin = new QSpinBox(this);
    this->inducerCurOutMin->setMaximum(50000);
    this->inducerCurOutMin->setValue(0);
    this->inducerCurOutMin->setSuffix("mA");
    this->inducerCurOutMax = new QSpinBox(this);

    this->inducerCurOutMax->setMaximum(90000);
    this->inducerCurOutMax->setValue(3000);
    this->inducerCurOutMax->setSuffix("mA");
    QLabel   *line6 = new QLabel("  ->  ",this);
    line6->setAlignment(Qt::AlignCenter);
    line6->setFixedWidth(50);
    QHBoxLayout  *hallCurLayout  = new QHBoxLayout();
    hallCurLayout  ->addWidget(this->inducerCurOutMin  );
    hallCurLayout ->addWidget(line6);
    hallCurLayout ->addWidget(this->inducerCurOutMax);
    hallCurrentBox = new QGroupBox(this);
    hallCurrentBox->setTitle("电感电流");
    hallCurrentBox->setLayout(   hallCurLayout );
    hallCurrentBox->setFixedSize(300,70);

    //温度

    this->tempMin = new QSpinBox(this);
    this->tempMin->setMaximum(8000);
    this->tempMin->setValue(0);
    this->tempMin->setSuffix("℃");
    this->tempMax = new QSpinBox(this);

    this->tempMax->setMaximum(8000);
    this->tempMax->setValue(3000);
    this->tempMax->setSuffix("℃");
    QLabel   *line7 = new QLabel("  ->  ",this);
    line7->setAlignment(Qt::AlignCenter);
    line7->setFixedWidth(50);
    QHBoxLayout  *tempLayout  = new QHBoxLayout();
    tempLayout  ->addWidget(this->tempMin  );
    tempLayout ->addWidget(line7);
    tempLayout ->addWidget(this->tempMax);
    tempBox = new QGroupBox(this);
    tempBox->setTitle("温度");
    tempBox->setLayout(  tempLayout );
   tempBox->setFixedSize(300,70);

   //端口电压

   this->portVolMin = new QSpinBox(this);
   this->portVolMin ->setMaximum(50000);
   this->portVolMin->setValue(0);
   this->portVolMin->setSuffix("mV");
   this->portVolMax = new QSpinBox(this);

   this->portVolMax->setMaximum(50000);
   this->portVolMax->setValue(3000);
   this->portVolMax->setSuffix("mV");
   QLabel   *line8 = new QLabel("  ->  ",this);
   line8->setAlignment(Qt::AlignCenter);
   line8->setFixedWidth(50);
   QHBoxLayout  *portVolLayout  = new QHBoxLayout();
  portVolLayout  ->addWidget(this->portVolMin  );
   portVolLayout ->addWidget(line8);
  portVolLayout ->addWidget(this->portVolMax);
   portVolBox = new QGroupBox(this);
   portVolBox->setTitle("端口电压");
    portVolBox->setLayout(  portVolLayout );
   portVolBox->setFixedSize(300,70);


   //功率板与主控板
   this->powerBtn = new QRadioButton("功率板",this);
   this->masterBtn = new QRadioButton("主控板",this);
   this->powerBtn->setChecked(true);
   QHBoxLayout  *radioBtnLayout  = new QHBoxLayout();
   radioBtnLayout->addWidget(this->powerBtn);
   radioBtnLayout->addStretch();
   radioBtnLayout->addWidget(this->masterBtn);
   radioBtnBox = new QGroupBox("设备选择",this);
   radioBtnBox->setLayout(radioBtnLayout);
   radioBtnBox->setFixedSize(300,70);

   //条码长度
   qrCodeLen = new QSpinBox(this);
   qrCodeLen->setMinimum(0);
   QHBoxLayout *qrCodeLayout = new QHBoxLayout();
   qrCodeLayout->addWidget(qrCodeLen);
   qrCodeLenBox = new QGroupBox("条码长度",this);
   qrCodeLenBox->setLayout(qrCodeLayout);
   qrCodeLenBox->setFixedSize(120,70);

}

void pcbaTest::netCfgInit()
{

    QLabel *portLab = new QLabel("本机端口:",this);
    QLabel  *ipLab  = new QLabel("本机IP:",this);
    this->udpPort = new QSpinBox(this);
    this->udpPort->setMaximum(30000);
    this->udpPort->setValue(26421);
    this->hostIpChoose = new QComboBox(this);
    QHBoxLayout *portLayout = new QHBoxLayout();
    portLayout->addWidget(portLab);
    portLayout->addWidget(udpPort);
    QHBoxLayout  *ipLayout = new QHBoxLayout();
    ipLayout->addWidget(ipLab);
    ipLayout->addWidget(hostIpChoose);
    QVBoxLayout  *netCfgLayout = new QVBoxLayout();
    netCfgLayout->addLayout(portLayout);
    netCfgLayout->addLayout(ipLayout);
    netCfgBox = new QGroupBox(this);
    netCfgBox->setTitle("网络配置");
    netCfgBox->setFixedSize(300,120);
    netCfgBox->setLayout(netCfgLayout);



}



void pcbaTest::tableClear(QString &str)
{
    for(uchar i = 0;i<4;i++)
       for(uchar j = 0 ;j <12;j++)
       {
              if(j!=1)
              this->testDataModel->setData(this->testDataModel->index(i,j),0);
              if(j==0)
              this->testDataModel->setData(this->testDataModel->index(i,j),str);
             // this->testDataView->setColumnWidth(i,00);
       }

}




//扫码枪事件
void pcbaTest::keyPressEvent(QKeyEvent *event)
{
    QString  text,time;
    time = QDateTime::currentDateTime().toString(" hh:mm:ss: ");
    if(event->key() ==Qt::  Key_Return )
    {
        if(qrCodeBuf.length() == qrCodeLen->value())  //条码长度匹配
        {
            text=QString("%1%2%3").arg(time).arg(qrCodeBuf).arg("扫码成功,已启动测试... ");

            this->msgBrowser->append(text);
            tableClear( qrCodeBuf);
            this->pcbQrCode =  qrCodeBuf;
            qrCodeBuf.clear();
           // this->startTestBtn->setEnabled(true);
            //开始测试
            startSampleSlot();
        }
        else
        {
            qrCodeBuf.clear();
            text=QString("%1%2").arg(time).arg("扫码失败,请重新扫码！");
            this->msgBrowser->append(text);
        }

    }
    else
         qrCodeBuf.append(event->text());

}


void pcbaTest::getLocalHostIp()
{

    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)

         single::m_Instance()->insertIplist(address.toString());
    }

}


//
void pcbaTest::msgBrowserInit()
{
    QString  text,time;
    time = QDateTime::currentDateTime().toString(" hh:mm:ss: ");
    this->msgBrowser = new QTextBrowser(this);
    this->msgBrowser->setFixedHeight(300);
    text=QString("%1%2").arg(time).arg("请先进行参数配置！");
    this->msgBrowser->append(text);

}





//定时溢出方法
void pcbaTest::timeoutMethodSlot()
{
   QStringList list;

#if 1
    if(this->connectTimeOut == 2)  //serach ip
    {
        getLocalHostIp();
        list = single::m_Instance()->getIplist();
        if(single::m_Instance()->getIplist().count()==4)
        {
                this->hostIpChoose->addItems(single::m_Instance()->getIplist());
                this->connectTimeOut = 1;
                this->pcbTimer->stop();
        }
    }
    else if(this->connectTimeOut == 1)   //心跳联机
    {
        appConnectSend();  //下发中位机联机
    }
    else
    {
        cfgDataToThreadSlot();  //发送配置数据
        emit sigdatasent(0x12,0);  //下发下位机联机
    }
#endif

}

//保存配置
void pcbaTest::cfgSaveSlot()
{

    QString  ip  =this->hostIpChoose->currentText();

    uint  port=  this->udpPort->value();
    this->pcbUdpsocket->bind(QHostAddress(ip),port);  //绑定ip和端口

    //保存配置数据

}

//数据接收
void pcbaTest::dataRecved()
{
    while(this->pcbUdpsocket->hasPendingDatagrams())				//(a)
    {

        if(this->connectTimeOut  == 0) //正在升级
        {
            dataRecv  cv   ;

            cv.dataGet.resize( this->pcbUdpsocket->pendingDatagramSize());
            this->pcbUdpsocket ->readDatagram(cv.dataGet.data(),cv.dataGet.size(),&cv.addrGet,&cv.portGet);
            QVariant  variant;
            variant.setValue(cv );
            emit sigDataToThread( variant);
            ;
        }
        else  //联机回复
        {

            dataRecv  *cv  =new  dataRecv  ;
            cv->dataGet.resize( this->pcbUdpsocket->pendingDatagramSize());
            this->pcbUdpsocket ->readDatagram(cv->dataGet.data(),cv->dataGet.size(),&cv->addrGet,&cv->portGet);

            single::m_Instance()->insertDataRecvList(cv);
            //
            updDataDeal  *deal  = new  updDataDeal();
            //添加节点
            connect(deal,SIGNAL(addMidmachineNode(QString,uchar,uchar,uchar,uchar)),
                    this,SLOT(slotAddmidmachineNode(QString,uchar,uchar,uchar,uchar)));

            this->pool->start(deal);

        }
    }
}


//发送线程的数据
void pcbaTest::slotSendMessage(QString ip, uchar *buf, ushort len)
{

      this->pcbUdpsocket->writeDatagram(reinterpret_cast<const char *>(buf),len,QHostAddress(ip),26420);
}


//中位机节点上树
void pcbaTest::slotAddmidmachineNode(QString ip, uchar bednum, uchar tempnum, uchar powernum, uchar slavepowernum)
{
    if(makeOnce == 0)
    {
        makeOnce = 1;  
        
        this->connectTimeOut = 0;    //停止下发联机
    
        serverThread  *thread = new serverThread(ip,0,this);
        connect(this,SIGNAL(sigdatasent(uchar,uchar)),thread,SLOT(slotSendUpdateCmd(uchar,uchar))); //ui命令到线程
        connect(thread,SIGNAL(dataToSend(QString,  uchar*,ushort)),this,SLOT(slotSendMessage(QString, uchar*,ushort)));  //线程数据到ui
        connect(this,SIGNAL(sig(QVariant)),thread,SLOT(slotGetData(QVariant)));      //同步共享资源到各线程
        //主线程将接收的数据转发给子线程
        connect(this,SIGNAL(sigDataToThread(QVariant)),thread,SLOT(slotDataToThread(QVariant)));
    
        //接收running msg
        connect(thread,SIGNAL(sigSendRunningMsg(QString,uchar,QStringList,ushort)),this,
                SLOT(slotRunningMsgProcess(QString,uchar,QStringList,ushort)));
    
        thread->start();

    }

}


//显示线程的数据
void pcbaTest::slotRunningMsgProcess(QString ip, uchar cmd, QStringList msg, ushort value)
{

    QString   str,time;
    time = QDateTime::currentDateTime().toString(" hh:mm:ss: ");
    switch (cmd) {
    case 0x01:
        if(value==0)
        {
           // this->netStatus  ->setText("联机");
           // this->statusShow->setStyleSheet("QLabel{background-color:rgb(0,255,0);}");
              this->pcbTimer->stop();
            str=QString("%1%2").arg(time).arg("已联机 ");
            this->msgBrowser->append(str);
        }
        else
        {
           // this->netStatus ->setText("脱机");
           // this->statusShow->setStyleSheet("QLabel{background-color:rgb(255,0,0);}");
            str=QString("%1%2").arg(time).arg("断通信 ");
            this->msgBrowser->append(str);
        }
        break;
    case  'S':
        if(value==1)  //
        {
           // this->netStatus ->setText("脱机");
           // this->statusShow->setStyleSheet("QLabel{background-color:rgb(255,0,0);}");
            str=QString("%1%2").arg(time).arg("断通信 ");
            this->msgBrowser->append(str);

        }
        if(msg.value(0)=="0")
        {
            for(uchar i = 2;i<msg.count()+2;i++)
                this->testDataModel->setData(this->testDataModel->index(0,i),msg.value(i-1));
        }
        else if(msg.value(0)=="1")
        {
            for(uchar i = 2;i<msg.count()+2;i++)
                this->testDataModel->setData(this->testDataModel->index(1,i),msg.value(i-1));
        }
        else if(msg.value(0)=="2")
        {
            for(uchar i = 2;i<msg.count()+2;i++)
                this->testDataModel->setData(this->testDataModel->index(2,i),msg.value(i-1));
        }
        else if(msg.value(0)=="3")
        {
            for(uchar i = 2;i<msg.count()+2;i++)
                this->testDataModel->setData(this->testDataModel->index(3,i),msg.value(i-1));
        }

        break;
     case  0x14 :
            if(value==1)  //
            {
               // this->netStatus ->setText("脱机");
               // this->statusShow->setStyleSheet("QLabel{background-color:rgb(255,0,0);}");
              //  str=QString("%1%2").arg(time).arg("断通信 ");
               // this->msgBrowser->append(str);
                ;
            }
            if((msg.value(0)=="FAIL")||msg.value(0)=="PASS")    //显示测试结果
            {
                for(uchar i = 0;i<msg.count();i++)
                {
                    if(msg.value(i)=="PASS")
                    {

                         this->testDataModel->item(i,11)->setForeground(QBrush(QColor(0,255,0)));
                         this->testDataModel->setData(this->testDataModel->index(i,11),msg.value(i));
                    }
                    else
                    {
                        this->testDataModel->item(i,11)->setForeground(QBrush(QColor(255,0,0)));
                        this->testDataModel->setData(this->testDataModel->index(i,11),msg.value(i));
                    }
                }


            }
            str=QString("%1%2").arg(time).arg("测试完成,通信断开,请继续下一个扫码 ");
            this->msgBrowser->append(str);
        break;
        case 'M' :
        for(uchar i =0;i<msg.count();i++)
        {
            str=QString("%1%2").arg(time).arg(msg.at(i));
            this->msgBrowser->append(str);
        }
        break;
    default:
        break;
    }

}


//打开配置
void pcbaTest::openCfgSlot()
{

    QString   str,time,name;
    time = QDateTime::currentDateTime().toString(" hh:mm:ss: ");
    name = QFileDialog::getOpenFileName(this,"打开",".","cfg(*.txt)");
    if(!name.isEmpty())
        openfile(name);

    str=QString("%1%2").arg(time).arg("请保存配置");
    this->msgBrowser->append(str);
}

void pcbaTest::openfile( QString filename)
{
    uchar row=0;
    QString cfgVlue;

       if(!filename.isEmpty())
       {
             QFile  file(filename);
             if(file.open(QFile::ReadOnly|QFile::Text))
             {
                 QTextStream  stream(&file);
                 QString  line;
                 do
                 {
                   line = stream.readLine();
                   if(!line.isEmpty())
                   {
                        QStringList pieces= line.split(",",QString::SkipEmptyParts);
                        switch (row) {
                        case cfgType::FIRSTVOLOUT:
                                cfgVlue = pieces.value(1);
                                this->voltageOutMin->setValue( cfgVlue.toInt());
                                 cfgVlue = pieces.value(2);
                                 this->voltageOutMax->setValue( cfgVlue.toInt());
                            break;
                        case cfgType:: SECONDVOLOUT:
                                cfgVlue = pieces.value(1);
                                this->secondVoltageOutMin->setValue( cfgVlue.toInt());
                                 cfgVlue = pieces.value(2);
                                 this->secondVoltageOutMax->setValue( cfgVlue.toInt());
                            break;

                        case cfgType:: INVOLOUT:   //电容 电压
                                cfgVlue = pieces.value(1);
                                this->capVolOutMin->setValue( cfgVlue.toInt());
                                 cfgVlue = pieces.value(2);
                                 this->capVolOutMax->setValue( cfgVlue.toInt());
                            break;

                        case cfgType:: PORTVOLOUT:   //母线电压
                                cfgVlue = pieces.value(1);
                                this->busVolOutMin->setValue( cfgVlue.toInt());
                                 cfgVlue = pieces.value(2);
                                 this->busVolOutMax->setValue( cfgVlue.toInt());
                            break;
                        case cfgType:: CUROUT:    //hall 电流
                                cfgVlue = pieces.value(1);
                                this->hallCurOutMin->setValue( cfgVlue.toInt());
                                 cfgVlue = pieces.value(2);
                                 this->hallCurOutMax->setValue( cfgVlue.toInt());
                            break;
                        case cfgType::  HALLCUROUT:
                                cfgVlue = pieces.value(1);
                                this->inducerCurOutMin->setValue( cfgVlue.toInt());  //电感电流
                                 cfgVlue = pieces.value(2);
                                 this->inducerCurOutMax->setValue( cfgVlue.toInt());
                            break;
                        case cfgType::  PORTNUM :
                                cfgVlue = pieces.value(1);
                                this->udpPort->setValue( cfgVlue.toInt());

                            break;
                        case cfgType::  PORTVOL :
                            cfgVlue = pieces.value(1);
                            this->portVolMin->setValue( cfgVlue.toInt());  //端口电压
                             cfgVlue = pieces.value(2);
                             this->portVolMax->setValue( cfgVlue.toInt());

                            break;
                        default:
                            break;
                        }

                   }
                   row++;
                 }while(!line.isEmpty());
             }
             file.close();
       }
}



//开始采样
void pcbaTest::startSampleSlot()
{
    if(mutexUdpMake == true)
    {
         this->cfgSaveSlot();//创建udp
         mutexUdpMake = false;
    }
     this->pcbTimer->start(100);
}

//APP联机
void pcbaTest::appConnectSend()
{


    uchar sendBuf[100]={0};

    for(uchar j =0;j<4;j++)
        sendBuf[j] = 0xFF;

    sendBuf[4]=4 ;   //长度
    sendBuf[5]=0xff;
    sendBuf[6]= 3;
    sendBuf[7]=0;  //命令
    sendBuf[8]= 2;    //

    this->pcbUdpsocket->writeDatagram(reinterpret_cast<const char *>( sendBuf),9,QHostAddress(DESIP ),26420);
}


//同步配置数据
void pcbaTest::cfgDataToThreadSlot()
{
    msg  pcbMsg;
    pcbMsg.qrCode = this->pcbQrCode;
    pcbMsg.voltageOutMin = this->voltageOutMin->value();
    pcbMsg.voltageOutMax = this->voltageOutMax->value();
    pcbMsg.secondVoltageOutMin= this->secondVoltageOutMin->value();
    pcbMsg.secondVoltageOutMax = this->secondVoltageOutMax->value();
    pcbMsg.busVolOutMin = this->busVolOutMin->value();
    pcbMsg.busVolOutMax= this->busVolOutMax->value();
    pcbMsg.capVolOutMin = this->capVolOutMin->value();
    pcbMsg.capVolOutMax=this->capVolOutMax->value();
    pcbMsg.hallCurOutMin = this->hallCurOutMin->value();
    pcbMsg.hallCurOutMax= this->hallCurOutMax->value();
    pcbMsg.inducerCurOutMin=this->inducerCurOutMin->value();
    pcbMsg.inducerCurOutMax= this->inducerCurOutMax->value();
    pcbMsg.portVolMin = this->portVolMin->value();
    pcbMsg.portVolMax = this->portVolMax->value();
    if(this->masterBtn->isChecked())
    pcbMsg.masterBoard = true;
    else
    pcbMsg.masterBoard = false;

    QVariant  variant;
    variant.setValue(pcbMsg);

    emit (sig(variant));
}



//另存配置文件
void pcbaTest::saveBtnClickedSlot()
{
    QString str1,str2,text;
    QString   str,time;
    time = QDateTime::currentDateTime().toString(" hh:mm:ss: ");
    //保存cgf文件
    this->grabKeyboard();   //get num & letter
    QFile   file( saveCfgPath );

     if(file.open(QFile::ReadWrite|QFile::Text))
    {

            QTextStream   out(&file);
            //写入阈值  电池电压
            str1 = QString::number(voltageOutMin->value());
            str2 = QString::number(voltageOutMax->value());
            text = QString("%1%2%3%4%5").arg("电池电压").arg(",").arg(str1).arg(",").arg(str2);
            out<<text<<endl;

            //第二路电压
            str1 = QString::number(secondVoltageOutMin->value());
            str2 = QString::number(secondVoltageOutMax->value());  //浮点转字符串
            text = QString("%1%2%3%4%5").arg("第二路电压").arg(",").arg(str1).arg(",").arg(str2);
            out<<text<<endl;

            //电容电压
            str1 = QString::number(capVolOutMin->value());
            str2 = QString::number(capVolOutMax->value());
            text = QString("%1%2%3%4%5").arg("电容电压").arg(",").arg(str1).arg(",").arg(str2);
            out<<text<<endl;

            //母线电压
            str1 = QString::number(busVolOutMin->value());
            str2 = QString::number(busVolOutMax->value());  //浮点转字符串
            text = QString("%1%2%3%4%5").arg("母线电压").arg(",").arg(str1).arg(",").arg(str2);
            out<<text<<endl;

            //霍尔电流
            str1 = QString::number(hallCurOutMin->value());
            str2 = QString::number(hallCurOutMax->value());
            text = QString("%1%2%3%4%5").arg("霍尔电流").arg(",").arg(str1).arg(",").arg(str2);
            out<<text<<endl;

            //电感电流
            str1 = QString::number(inducerCurOutMin->value());
            str2 = QString::number(inducerCurOutMax->value());
            text = QString("%1%2%3%4%5").arg("电感电流").arg(",").arg(str1).arg(",").arg(str2);
            out<<text<<endl;

            //端口

            str2 = QString::number(udpPort->value());
            text = QString("%1%2%3").arg("端口").arg(",").arg(str2);
            out<<text<<endl;

            //端口电压
            str1 = QString::number(portVolMin->value());
            str2 = QString::number(portVolMax->value());
            text = QString("%1%2%3%4%5").arg("端口电压").arg(",").arg(str1).arg(",").arg(str2);
            out<<text<<endl;

    }
     file.close();

     str=QString("%1%2").arg(time).arg("保存成功");
     this->msgBrowser->append(str);
     str=QString("%1%2").arg(time).arg("请扫码");
     this->msgBrowser->append(str);

}



























