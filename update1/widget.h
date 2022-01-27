#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QTimer>
#include <QFile>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMessageBox>
#include "single.h"
#include<QDateTime>
#include<QMutexLocker>
#include "server.h"
#include <QVBoxLayout> //垂直布局
#include <QLabel>
#include  <QProgressBar>
#include  <QTableView>
#include  <QTreeWidget>
#include   <QTextEdit>
#include  <QComboBox>
#include <QTableView>
#include <QStandardItemModel>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include "pgbardelegate.h"
#include <QTextBrowser>
#include <QRadioButton>
#include <QMutex>
#include <QCheckBox>
#include <QHostInfo>
#include "struct.h"

class serverThread ;

#define  PROBARCOlUMN    3       //进度条所在列
//namespace Ui {
//class Widget;
//}
class Server;
class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void treeWidgetInit(const QString & ip,uchar addr);//初始化树形结点
    QTreeWidgetItem *  AddTreeRoot(QString name,QString desc);
    void  disableTreeItems(QString text);   //掉线后移除treeitem
    QTreeWidgetItem *  AddTreeNode(QTreeWidgetItem *parent,QString name,QString desc);
    void updateParentItem(QTreeWidgetItem* item);

    void initfilePath();  //文件路径
    void initModel();   //模型初始化
    void initTable();  //表格初始化

    void cfgBoxInit(); //配置控件及布局初始化
    void initDeviceBox();  //设备选择框
    void initUpdateBtn();  //升级按钮初始化
    void initPortAndIp();  //初始化端口与ip控件
    void initCfgBtn();  //
    void initSerialUi();  //串口相关ui初始化
    void bottomLayOutCtrlInit(void); //右边布局

    void updateChild(QTreeWidgetItem *item);  //更新子节点
    void updateParent(QTreeWidgetItem *item); //更新父节点
    void treeWidgetUiInit();  //树初始化

    void initTimer(void);

    void convertData(const QByteArray &data);
private:



    //中间布局
    QVBoxLayout  *topLayout;
    QTableView  * msgTable;//信息显示表
    pgbarDelegate *  progressBarDelegate;  //进度条的代理
    QStandardItemModel  *msgModle;  //
    uchar   rows; //model的行


    QLabel  *pathLab;  //
    QLineEdit  * pathEdit; //文件路径显示
    QPushButton * fileOpen;
    QHBoxLayout *pathlayout;



    //左边布局
    QVBoxLayout  *leftLayout;//节点树布局
    QTreeWidget   *trayTree; //节点树

     //下边布局
    QVBoxLayout  * rightBottomLayout ; //右下边布局
    QHBoxLayout  *bottomLayout;     //左下边布局
    QPushButton  *updateBtn;  //升级按键
    QVBoxLayout  *updateBtnLayout;
    QGroupBox *updateBtnBox ;

    QTextBrowser  *msgBox;
    QGroupBox     *mannualUpateBox;  //手动升级
    QPushButton *clearBtn;   //
    QPushButton  *jumpBtn;   //跳转
    QPushButton  *appSendBtn;//手动下发APP数据
    QPushButton  *eraseBtn;  //擦除按钮
    QPushButton  *bootConnBtn;   //boot联机



    QPushButton * cfgBtn;    //配置网络通信方式
    QHBoxLayout  *setNetLayout;
    QGroupBox *netConfigBox ;



    QLabel  *portLab;  //ip与端口
    QSpinBox  *portbox;
    QComboBox *ipbox;
    QLabel    *m_editerLab;  //绑定IP
    QHBoxLayout  *portLayout;
    QVBoxLayout * netLayout;
    QHBoxLayout  *m_editerLayout;


    QLabel *serialLab;
    QComboBox *serialBox;   //串口名
    QGroupBox  *serialGpBox;


    QLabel  *machineLab;  //设备选择框
    QComboBox  *deviceBox;
    QHBoxLayout *machineLayout;


    QTimer  *m_timer;
    serverThread  *thread;






protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

signals:


    void  sigdatasent(ushort cmd, uchar type,QVariant); //数据到线程处理  命令  设备类型代号  4，中位机，2，针床下位机，3，电源，1，从电源，5，温度
    void  sigItemChanged(QString ip,QString device);  //节点状态改变

public slots:

    void treeItemChanged(QTreeWidgetItem* item, int column);

private slots:


    void timeoutEventMethod(void); //定时超时方法
    void getLocalHostIp();  //获取本机ip

    void clientThreadInit(QString myIp,QString  mcId,  bool isNet, ushort myPort,updateTarget target);//创建中位机线程


    void on_openfile_clicked();   //打开文件
    void cfgBtnClickedSlot(void);   //配置
    void updateBtnClickedSlot(void);    //升级按键的槽
    void clearBtnClickedSlot(void);
    void jumpBtnClickedSlot(void);//跳转
    void appSendBtnClickedSlot(void); //手动下发
    void eraseBtnClickedSlot(void); //擦除
    void bootConnBtnClickedSlot(void); //手动搜箱

    //running msg
    void slotRunningMsgProcess(QString ip, ushort  cmd,QStringList  msg, ushort value);
    void  slotRefreshStatus(QString ip,QStringList versoinList,ushort powerAddr);//刷新各电源箱在线状态
    void on_exportMsg_clicked();//导出运行信息

    //node状态改变
    void slotItemChange(QString rootIp,QString device);
    void  setParentPartialyChecked(QTreeWidgetItem  *item);  //设置父节点半选中状态
    void  clearChildItem(QTreeWidgetItem  *item);
    //中位机节点半选中节点插入处理
    void  midmachineNodePartiallyChecked(uchar itemIndex,QString device);
    void  midmachineNodeChecked(uchar itemIndex,QString device);
    void  midmachineNodeUnchecked(uchar itemIndex,QString device);
    //中位机节点管理
    void rootItemInsert(uchar rootIndex,QString device);
    void rootItemRemove(uchar rootIndex,QString device);
    //父节点管理
    void parentItemInsert(uchar parentIndex,uchar rootIndex,QString device);
    void parentItemRemove(uchar parentIndex,uchar rootIndex);
    //电源从节点管理
    void slaveItemManage(uchar parentIndex,uchar rootIndex,QString device);
    void slaveItemInsert(uchar childIndex,uchar parentIndex,uchar rootIndex,QString device);  //从节点插入
    void slaveItemRemove(uchar childIndex,uchar parentIndex,uchar rootIndex);  //从节点移除

};




/*************
 * 待优化事项：  1.udp创建一次，加保保护标志
 *             2.子线程创建udp发送数据
 *
 * *****************************/

#ifndef __APPLE__
//#  include "HalconCpp.h"
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#  endif
#endif

#endif // WIDGET_H
