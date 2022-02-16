#ifndef PCBATEST_H
#define PCBATEST_H

#include <QWidget>
#include <QPushButton>
#include  <QTextBrowser>
#include   <QGridLayout>
#include <QVBoxLayout>
#include <QWidget>
#include<QLineEdit>
#include <QComboBox>
#include <QLabel>
#include  <QGroupBox>
#include  <QTableView>
#include <QStandardItemModel>
#include "serverthread.h"
#include  <QSpinBox>
#include <QFileDialog>
#include<QHeaderView>
#include<QTimer>
#include <QUdpSocket>
#include  <QThreadPool>
#include <QKeyEvent>
#include <QTextStream>
#include "single.h"
#include  <QThreadPool>
#include  <datarecv.h>
#include "struct.h"
#include "widget.h"
#include <QRadioButton>
class Widget;

enum  cfgType{ FIRSTVOLOUT=0x00, SECONDVOLOUT,  INVOLOUT , PORTVOLOUT, CUROUT, HALLCUROUT,PORTNUM,PORTVOL  };


#define     saveCfgPath              "./testCfg.txt"

class pcbaTest : public QWidget
{
    Q_OBJECT
public:
    explicit pcbaTest(QWidget *parent = nullptr);
    ~pcbaTest();

    void  tableModeInit(void);    //数据初始化
    void  tableViewInit(void);   // 可视表初始化
    void  configBoxInit(void);    //配置spinbox初始化
    void  netCfgInit(void);       //网络部分初始化
    void  tableClear(QString &str);  //清零表
    void  keyPressEvent(QKeyEvent * event);       //键盘事件
    void  getLocalHostIp();  //获取ip
    void  msgBrowserInit();   //对话框初始化

signals:
    void  sigDataToThread(QVariant);
    void  sigdatasent(uchar,uchar);
    void  sig(QVariant);

public   slots:
         void timeoutMethodSlot(void);
private   slots:
    void   cfgSaveSlot(void);  //完成配置
    void   dataRecved(void);  //数据接收
    //发送种线的数据
    void  slotSendMessage(QString ip,  uchar *buf,ushort len);
    //中位机节点上树
    void  slotAddmidmachineNode(QString  ip,uchar bednum,uchar tempnum,uchar powernum,uchar slavepowernum);
    //running msg
    void slotRunningMsgProcess(QString ip, uchar  cmd,QStringList  msg, ushort value);
    //打开配置
    void  openCfgSlot(void);
    void  openfile( QString filename );
    //开始采样
    void  startSampleSlot(void);
    //心跳下发pcbAPP联机
    void  appConnectSend(void);
    //将pcb的配置数据同步到线程
    void cfgDataToThreadSlot(void);
    //另保存配置文件
    void  saveBtnClickedSlot();
private:

    QTableView  *testDataView;
    QStandardItemModel  *testDataModel;
    QString  pcbQrCode;   //pcb二维码
    QSpinBox  *voltageOutMin;    //输出电压下限
    QSpinBox  *voltageOutMax;    //输出电压上限
    QSpinBox  *secondVoltageOutMin;    //电压下限
    QSpinBox  *secondVoltageOutMax;    //输出电压上限
    QSpinBox  *busVolOutMin;    //母线电压下限
    QSpinBox  *busVolOutMax;    //母线电压上限
    QSpinBox  *capVolOutMin;    //电容电压下限
    QSpinBox  *capVolOutMax;    //电容电压上限
    QSpinBox  *hallCurOutMin;    //霍尔电流下限
    QSpinBox  *hallCurOutMax;    //霍尔电流上限
    QSpinBox  *inducerCurOutMin;    //电感电流下限
    QSpinBox  *inducerCurOutMax;    //电感电流上限
    QSpinBox  *portVolMin;    //端口电压
    QSpinBox  *portVolMax;    //端口电压
    QSpinBox  *tempMin;    //温度下限
    QSpinBox  *tempMax;    //温度上限
    QGroupBox  *volOutBox,  *secondVolOutBox ,
    *portVolOutBox,*innerVolOutBox ,*outputCurrentBox,*  hallCurrentBox,*tempBox,*portVolBox,\
      *radioBtnBox,*qrCodeLenBox;
    QSpinBox  *qrCodeLen;    //条码长度
    QSpinBox   *udpPort;      //端口
    QComboBox  *hostIpChoose;  //本机ip配置

    QGroupBox  *netCfgBox;  //
    QPushButton *saveCfgBtn, *startTestBtn,*openCfgBtn;    //保存，开始测试，打开配置测试

    QUdpSocket   *pcbUdpsocket;    //
    QThreadPool   *pool;       //处理联机的线程池
    QTimer  *pcbTimer;
    QLabel  *netStatus,*statusShow;  //网络状态

    QString  qrCodeBuf;
    uchar     connectTimeOut;  //联机命令与采样的互斥

    uchar sendTimes,recvTimes;
    //PCB测试线程保护 
    uchar   makeOnce;    
    bool  mutexUdpMake;
    QRadioButton  *powerBtn,*masterBtn;
    QTextBrowser  *msgBrowser;   //信息窗口

  //  Widget   *pcbWidget;   //
    
};



#endif // PCBATEST_H
