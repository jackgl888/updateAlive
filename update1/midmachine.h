
#ifndef MIDMACHINE_H
#define MIDMACHINE_H
#include <QTreeWidgetItem>
#include <QObject>
#include <QHostAddress>
#include <QTimer>
#include  <QFile>
#include <QMutex>
#include <QMutexLocker>
#include "parentDevice.h"
#include "slavepower.h"
#include "widget.h"
#include "custom.h"
//#include "xlsxdocument.h"
//#include  "xlsxformat.h"
//#include  "xlsxcellrange.h"
//#include "xlsxchart.h"
#include "struct.h"
#include  <QUdpSocket>
#include  <QFileInfoList>



class parentdevice;
class slavePower;

enum  operateType{ UPDATE=0x00, PCBATEST };
//中位机对象
#define BED_LABLE_ITEM     0   //针床标签父节点
#define POWER_LABLE_ITEM   1   //电源节点父标签
#define TEMP_LABLE_ITEM    2   //温度
#define FROCK_LABLE_ITEM   3   //工装
#define PCBTESTPORT       26420
#define UPDATECLIENTPORT   6000


#define  MAXSIZE    200000
#define   RESENDDELAY   1000
#define   RECV_BUFF_LEN      2000
#define   SEND_BUFF_LEN      2000



class Singel;
class Widget;
class midmachine : public QObject
{
    Q_OBJECT
public:
    explicit midmachine( QObject *parent = nullptr);
    ~midmachine();


    QString  rdIp();

    bool rdconnect();
    bool rdupdate();
    bool  rdrun();
    bool  rdbackup();
    bool rdTimeout();
    quint32  rdBytesIn();

    void wrIp(QString m_ip);
    void wrconnect(bool flag);
    void wrupdate(bool flag);
    void  wrrun(bool flag);
    void  wrbackup(bool flag);
    void  wrTimeout(bool  flag);
    void   wrBytesIn(uchar byte);
    void   cleanAppData();  //清除app数据

    //读回备份
    void  backUpFile(QString  filename);
    //创建每天的测试数据存储路径
    void  creatTestDataPath();
    //pcb测试表
    void pcbTestXlsxInit(void);
    //得到当前文件下的文件集列表
    QFileInfoList getQfileInfoList(QString  filePath);
public  slots:
    void timeOutMethodSlot(void);
signals:

    void dataSendFinish(QString ip);
    void itemChange(QString ip,QString device); //
    void  sigRunMsgToUi(QString ip, uchar  cmd,QStringList  msg, ushort value);
private  slots:
    void  slotDataSent(uchar cmd,uchar type);
    void  resendTimerOutEvent(void);
    void  findChoosedItem(uchar  device);  //找到选中的各节点
    void  slotGetData(QVariant variant);
    //boot跳转
    void  sendBootConnect(uchar type,uchar parentAddr,uchar childAddr); //
    //发送擦除指令
    void  ereaseSectors(uchar type,uchar parentAddr,uchar childAddr);
    //发送升级数据
    void  sendBootData(uchar type,uchar parentAddr,uchar  childAddr);
    //app数据校验
    void  readBackAppDataVeriy(uchar type,uchar parentAddr,uchar childAddr);
    //发送运行指令
    void  sendRunCommand(uchar type,uchar parentAddr,uchar  childAddr);
    //发送PCB联机指令
    void sendPcbConnectCommand();
    //发送PCB测试指令
    void sendPcbTestCommand(uchar chnNum);
    //发送pcb  pwm测试指令
    void sendPcbPwmTestCommand(uchar chnNum,DebugParameterBitField pwmTestPara);
     //发送pcb  恒流测试指令
   void  sendPcbFixedCurrentCmd(uchar chnNum,DebugParameterBitField pwmTestPara);
    //处理主线程过来的数据
    void  slotRcevDataToThread(const QVariant);
    //数据解析处理
    void  recvDataMethod( const uchar *data);

    //响应主线程item状态改变更新modle
    void  slotItemChange(QString rootIp,QString device);
    void  cmdBootConnectAnalysis(const uchar *data);  //解析boot联机回复
    void  cmdUpgradeAnalysis(const uchar *data);  //解析升级回复
    void  eraseReplyAnalysis(const uchar *data);   //擦除命令回复
    void  verifyReplyAnalysis(const uchar *data);   //校验命令回复
    void  cmdRunAnalysis(const  uchar *data);  //运行命令解析回复
    void  pcbTestSampleAnalysis(const  uchar *data);  //pcbtest采样回复处理
    void  pcbTestDataJudge(LowVoltageDCDCSampleDataStruct  *ptrSampleData);  //进行数据判定
    void  pcbTestPwmAnalysis(const uchar *data);   // pwm测试回复处理

private:

    QString  ip;       //  父节点名

    bool  connected, update,run,backUp, cmdTimeout;        //boot联机标志 升级标志 运行标志 备份校验 命令执行超时
    quint32   BytesIn;     //写入字节数
    uchar    appData[MAXSIZE];   //读回的app数据
    QTimer  *resendTimer;  //重发定时器

    uchar  resendType, resendChildAddr,resendParentAddr,resendCmd,resendCnt; //
    uchar  sendBuf[SEND_BUFF_LEN];  //当前发送包
    uchar   runningCmd;   // 正在执行的指令
    uchar updateMidmachine;  //0:没选 中   1：半选中   2： 全选中

    //  2.从电源低地址位依次到高地址。
    uchar  itemCount[4];     // 0:针床   1：电源    2：温度采样板  3：电源从板
    qint64   totalBytes;  //数据总字节数
    uint    totalFrames,  lastbytes;  //总的1024包发送次数 数据不满1024字节数
    uchar    updateBuf[MAXSIZE];  //升级数据
    uchar frameIndex; //升级数据帧序列

    uchar  itemNumber,bootItemNumber, runItemNumber;  //队列里节点序号 boot跳转的序列号  运行节点的序列号

    QMutex  mtx;
    uchar recvBuf[RECV_BUFF_LEN];
    ushort recv_cnt,length;		  //当前通讯口第几个字符   三个通讯口串口1 2 3   接收次数 长度
    uchar head_cnt,wait;

    QList<parentdevice*>parentDeviceList;  //
    QList<slavePower*>slaveList;
    uchar  searchUpdateItems;        //遍历等待升级的节点标志
    bool isBoot;                     //是否在boot中   true  等于在   false 不在

    //QXlsx ::Document  * pcbTestXlsx;  //建个表用于存储pcb测试数据
    QTimer   *midmcTimer;


    uchar sampleTouchMutex;  //联机与采样及pwm测试互斥   1：联机 0：采样  2：pwm测试

    QString chnNumList;   //通道地址队列
    QStringList pcbJudgeResult;  //最终的判定结果

    qint8 m_curSmplChNo;
    bool m_bHaveRcvedAck;
    int m_timeout;

    QUdpSocket * dataSendUdpsocket; //用于发送数据

    QString  pcbQrCode;  //二维码值

     float m_voltageOutMin;    //输出电压下限
     float m_voltageOutMax;    //输出电压上限
     float m_secondVoltageOutMin;    //电压下限
     float m_secondVoltageOutMax;    //输出电压上限
     float m_busVolOutMin;    //母线电压下限
     float m_busVolOutMax;    //母线电压上限
     float m_capVolOutMin;    //电容电压下限
     float m_capVolOutMax;    //电容电压上限
     float m_hallCurOutMin;    //霍尔电流下限
     float m_hallCurOutMax;    //霍尔电流上限
     float m_inducerCurOutMin;    //电感电流下限
     float m_inducerCurOutMax;    //电感电流上限
     float m_portVolMin;    //端口电压下限
     float m_portVolMax;    //端口电压上限
     bool  m_testMaster;    //测试主控板

     QString   testResultSavePath  ;   //保存路径
     QString   topleverDirPath;   //功率板与主控制板测试数据保存地址

     bool  makeExcelInitOnce;    //创建一次

     uint  curRowNum;  //表的存储行index
};



#endif //
