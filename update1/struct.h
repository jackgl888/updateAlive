#ifndef STRUCT_H
#define STRUCT_H
#include <QObject>
#include <QHostAddress>





#define  POWERNUM               3



#define  MAX_RESEND_TIMES        50
#define  APP_SIZE               800000
#define  WORDNUM                256
#define  MAX_SEND_APPSIZE       (WORDNUM*4)
#define  CONNTIME                3000      //联机频率
#define  RESENDTIME             3000   //重发时间间隔
#define  SEND_BUFF_LEN          2000
#define  RECV_BUFF_LEN          2000
#define  ERASERESEND            10000  //擦除重发
#define  mcipListPath           "./ipcfig.txt"



//qstringlist msg  数据结构      0.正在运行的程序 APP BOOT    1.运行消息    2.中位机版本号     3.lcid  4.lc版本号

enum  updateCmd{

    CONNECTTARGET = 0x0401,      //与目标板建立联机
    APPJUMPBOOT ,        //BOOT跳转
    ERASESECTOR ,          //擦除扇区
    BOOTWRITEDATA ,        //写APP数据
    BOOTJUMPAPP ,              //BOOT跳转到APP
    BACKUPAPP,             //备份之前的APP数据,,,
    GETAPPFILE,           //从主线程获得APP文件
    NETLOST ,              //tcp掉线,,
    UNCONNECT,              //无法连接
    MANUALSENDDATA,        //手动升级APP
    MANUALERASE            //手动擦除
};

enum   lcUpdateCmd{

    PCCONNECTLC   = 0X41,
    LCJUMPBOOT ,
    LCERASESECTOR ,
    LCBOOTWRITEDATA,
    LCBOOTJUMPAPP

};

# pragma pack(1)
typedef   struct
{
    ushort  txLen;
    uchar   txBuf[ SEND_BUFF_LEN];
}txNode;



typedef    enum
{
    MCTRANSMIT = 0x01,
    POWERMASTER
}updateTarget;

typedef  struct
{
    QByteArray dataGet;
    QHostAddress  addrGet;
    quint16 portGet;
}dataRecv;


typedef   struct
{
    uchar  allData[APP_SIZE];
    uint totalSdtimes;  //总的1024包发送次数
    qint64 totalBytes;  //数据总字节数
    uint lastbytes;  //数据不满1024字节数

} updatePara ;


//升级节点的集合
typedef   struct
{
    QString mcAddr;  //中位机节点
    QList<uchar>powerAddrList;   //此节点电源升级地址队列

}targetAddr;

typedef   struct
{
    QString   qrCode; //二维码
    float voltageOutMin;    //输出电压下限
    float voltageOutMax;    //输出电压上限
    float secondVoltageOutMin;    //电压下限
    float secondVoltageOutMax;    //输出电压上限
    float busVolOutMin;    //母线电压下限
    float busVolOutMax;    //母线电压上限
    float capVolOutMin;    //电容电压下限
    float capVolOutMax;    //电容电压上限
    float hallCurOutMin;    //霍尔电流下限
    float hallCurOutMax;    //霍尔电流上限
    float inducerCurOutMin;    //电感电流下限
    float inducerCurOutMax;    //电感电流上限
    float portVolMin;    //端口电压下限
    float portVolMax;    //端口电压上限
    bool  masterBoard;   //测试目的板
}msg;


typedef struct {


    //steptype, sta, event,rsv
    //uint status;
    quint8 stepType;
    quint8 event;
    quint8 sta;
    quint8 DDC;

    float batVoltage1;  //电池电压
    float current1;  //霍尔电流
    float batVoltage2;  //第二路电压
    float current2;  //电感电流
    float busVoltage;  //母线电压
    float outVoltage;  //电容电压
    float portVol;      //端口电压

    //
    float vPidOut;
    float iInnPidOut;
    float iExtPidOut;
    float testVariable;


}LowVoltageDCDCSampleDataStruct;




typedef   struct {


    struct{

        //    uint chnlNum;
        //    uint stepType;
        //    uint vloopSel;
        //    uint iloopSel;
        //    uint funcSwitch;
        qint8 chnlNum;
        qint8 stepType;
        qint8 ivloopSel;
        qint8 funcSwitch;
    }DebugParameterBitField;

    uint MainPara1;
    uint MainPara2;
    uint MainPara3;
    uint MainPara4;
    uint MainPara5;

    uint LimitPara1;
    uint LimitPara2;
    uint LimitPara3;
    uint LimitPara4;

}DebugParameterBitField;



typedef   struct
{


    uchar  boxId;
    ushort boxVersion;


}LcMsg;  //单个电源柜版本号


typedef  struct
{
    ushort mcVersion;
    LcMsg m_lcMsg[10];     //lc版本信息
}PowerMsg;   //整台设备版本信息


#pragma pack()


Q_DECLARE_METATYPE(updateTarget)
Q_DECLARE_METATYPE(updatePara)
Q_DECLARE_METATYPE(dataRecv)
Q_DECLARE_METATYPE(msg)
Q_DECLARE_METATYPE(LowVoltageDCDCSampleDataStruct)
Q_DECLARE_METATYPE(DebugParameterBitField)
Q_DECLARE_METATYPE(targetAddr)
Q_DECLARE_METATYPE(txNode)
Q_DECLARE_METATYPE(LcMsg)
Q_DECLARE_METATYPE(PowerMsg)
//uint chnlNum:8;
//uint stepType:8;
//uint vloopSel:4;
//uint iloopSel:4;
//uint funcSwitch:8;



#endif // STRUCT_H
