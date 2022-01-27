#include "serverthread.h"਍⌀椀渀挀氀甀搀攀㰀儀䐀攀戀甀最㸀ഀ
#include "widget.h"਍ഀ
਍ഀ
serverThread::serverThread(int sockDesc, QObject *parent) :਍    儀吀栀爀攀愀搀⠀瀀愀爀攀渀琀⤀Ⰰഀ
    m_sockDesc(sockDesc)਍ഀ
{਍ഀ
}਍ഀ
serverThread::~serverThread()਍笀ഀ
    delete  _timer;਍    搀攀氀攀琀攀  洀开猀漀挀欀攀琀㬀ഀ
}਍ഀ
void serverThread::run(void)਍笀ഀ
਍ഀ
    _timer = new QTimer();   //不能指定父类਍    开琀椀洀攀爀ⴀ㸀洀漀瘀攀吀漀吀栀爀攀愀搀⠀琀栀椀猀⤀㬀ഀ
਍    洀开猀漀挀欀攀琀 㴀 渀攀眀 䴀礀匀漀挀欀攀琀⠀ 洀开猀漀挀欀䐀攀猀挀⤀㬀ഀ
    if (!m_socket->setSocketDescriptor( m_sockDesc)) {਍        爀攀琀甀爀渀 㬀ഀ
    }਍    瀀漀爀琀猀⸀愀瀀瀀攀渀搀⠀洀开猀漀挀欀䐀攀猀挀⤀㬀ഀ
    m_socket->moveToThread(this);਍ഀ
    QString  ip =m_socket->peerAddress().toString().remove(0,7);਍    椀瀀䰀椀猀琀⸀愀瀀瀀攀渀搀⠀椀瀀⤀㬀ഀ
    connect(m_socket, &MySocket::disconnected, this, &serverThread::disconnectToHost,Qt::DirectConnection);਍ഀ
    connect(this, SIGNAL(sendData(const QString&,const int ,const char &)),਍            洀开猀漀挀欀攀琀Ⰰ 匀䰀伀吀⠀猀攀渀搀䐀愀琀愀⠀挀漀渀猀琀 儀匀琀爀椀渀最☀Ⰰ 挀漀渀猀琀 椀渀琀Ⰰ挀漀渀猀琀 挀栀愀爀 ☀⤀⤀Ⰰ儀琀㨀㨀 儀甀攀甀攀搀䌀漀渀渀攀挀琀椀漀渀⤀㬀ഀ
਍    ⼀⼀끦�ꚏﵞ﷿㿿ഀ
    connect(m_socket, SIGNAL(progressBarUpdata(int)), this,਍            匀䰀伀吀⠀瀀爀漀最爀攀猀猀䈀愀爀唀瀀搀愀琀愀匀氀漀琀⠀椀渀琀 ⤀⤀Ⰰ儀琀㨀㨀䐀椀爀攀挀琀䌀漀渀渀攀挀琀椀漀渀⤀㬀ഀ
    //更新运行信息਍    挀漀渀渀攀挀琀⠀洀开猀漀挀欀攀琀Ⰰ 匀䤀䜀一䄀䰀⠀爀甀渀渀椀最䴀猀最唀瀀搀愀琀愀⠀挀漀渀猀琀 儀匀琀爀椀渀最 ☀⤀⤀Ⰰ 琀栀椀猀Ⰰഀ
            SLOT(runningMsgUpdateSlot(const QString & )),Qt::DirectConnection);਍    ⼀⼀㤀�ᅓ扨퍟륾൰
    connect(m_socket, SIGNAL(treeWidgetUpdate(QTreeWidgetItem * ,QString,bool  )), this,਍            匀䰀伀吀⠀琀爀攀攀圀椀搀最攀琀唀瀀搀愀琀攀匀氀漀琀⠀ 儀吀爀攀攀圀椀搀最攀琀䤀琀攀洀 ⨀ Ⰰ 儀匀琀爀椀渀最Ⰰ 戀漀漀氀⤀⤀Ⰰ儀琀㨀㨀䐀椀爀攀挀琀䌀漀渀渀攀挀琀椀漀渀⤀㬀ഀ
    //发送下一个节��?਍    挀漀渀渀攀挀琀⠀琀栀椀猀Ⰰ 匀䤀䜀一䄀䰀⠀最漀琀漀一攀砀琀猀攀渀搀⠀挀漀渀猀琀 儀匀琀爀椀渀最 Ⰰ挀漀渀猀琀 挀栀愀爀 ⤀⤀Ⰰ 洀开猀漀挀欀攀琀Ⰰഀ
            SLOT(startNextSend(const QString &,const char )),Qt:: QueuedConnection);਍ഀ
਍    挀漀渀渀攀挀琀⠀开琀椀洀攀爀Ⰰ 匀䤀䜀一䄀䰀⠀琀椀洀攀漀甀琀⠀⤀⤀Ⰰ 琀栀椀猀Ⰰ 匀䰀伀吀⠀猀氀漀琀匀攀渀搀䴀䴀䤀䐀愀琀愀吀椀洀攀伀甀琀⠀⤀⤀Ⰰ儀琀㨀㨀䐀椀爀攀挀琀䌀漀渀渀攀挀琀椀漀渀⤀㬀ഀ
਍    挀漀渀渀攀挀琀⠀琀栀椀猀Ⰰ   匀䤀䜀一䄀䰀⠀爀攀猀攀渀搀䐀愀琀愀⠀挀漀渀猀琀 儀匀琀爀椀渀最 ☀Ⰰ 挀漀渀猀琀 椀渀琀 Ⰰ挀漀渀猀琀  挀栀愀爀 ⤀⤀Ⰰ  洀开猀漀挀欀攀琀Ⰰഀ
                SLOT(partlySendData(const QString&, const int,const  char)),Qt:: QueuedConnection);਍    ⼀⼀ᨀ자⽓ꡔ൒
    connect( m_socket, SIGNAL(timerStart(int )), _timer, SLOT(start(int)),Qt::DirectConnection);//਍    挀漀渀渀攀挀琀⠀琀栀椀猀Ⰰ 匀䤀䜀一䄀䰀⠀琀椀洀攀爀匀琀漀瀀⠀⤀⤀Ⰰ 开琀椀洀攀爀Ⰰ 匀䰀伀吀⠀猀琀漀瀀⠀⤀⤀Ⰰ儀琀㨀㨀䐀椀爀攀挀琀䌀漀渀渀攀挀琀椀漀渀⤀㬀ഀ
    //树初始化਍    攀洀椀琀  椀渀椀琀吀爀攀攀⠀椀瀀⤀㬀ഀ
    this->exec();਍ഀ
}਍ഀ
਍ഀ
਍瘀漀椀搀 猀攀爀瘀攀爀吀栀爀攀愀搀㨀㨀 猀攀渀搀䐀愀琀愀匀氀漀琀⠀挀漀渀猀琀 儀匀琀爀椀渀最☀椀瀀Ⰰ 挀漀渀猀琀  椀渀琀 琀爀愀礀渀甀洀Ⰰ挀漀渀猀琀 挀栀愀爀 挀洀搀⤀ഀ
{਍    攀洀椀琀  猀攀渀搀䐀愀琀愀⠀椀瀀Ⰰ 琀爀愀礀渀甀洀Ⰰ挀洀搀⤀ 㬀ഀ
     qDebug()<<"reID:"<<this->currentThreadId();਍紀ഀ
਍ഀ
void serverThread::progressBarUpdataSlot(int value)਍笀ഀ
     //   emit  progressBarUpdataSig( value);਍紀ഀ
਍瘀漀椀搀 猀攀爀瘀攀爀吀栀爀攀愀搀㨀㨀爀甀渀渀椀渀最䴀猀最唀瀀搀愀琀攀匀氀漀琀⠀挀漀渀猀琀 儀匀琀爀椀渀最 ☀䴀猀最⤀ഀ
{਍    攀洀椀琀 爀甀渀渀椀渀最䴀猀最唀瀀搀愀琀攀⠀  䴀猀最⤀㬀ഀ
}਍ഀ
void serverThread::treeWidgetUpdateSlot( QTreeWidgetItem * parent ,  QString name, bool flag)਍笀ഀ
    emit treeWidgetUpdateSig(parent,   name, flag);਍紀ഀ
਍ഀ
਍ഀ
਍ഀ
void serverThread::disconnectToHost(void)਍笀ഀ
    QString  ip;਍ഀ
    qDebug()<<"dsID:"<<this->currentThreadId();਍    ⼀⼀ﬀ摹練鑛楞瀀฀灎漀爀琀ഀ
    for(int i=0;i<ports.length();i++)਍    笀ഀ
਍        椀昀⠀瀀漀爀琀猀⸀愀琀⠀椀⤀ 㴀㴀 洀开猀漀挀欀䐀攀猀挀⤀ഀ
        {਍ഀ
            ip =ipList.at(i);਍            椀瀀䰀椀猀琀⸀爀攀洀漀瘀攀䄀琀⠀椀⤀㬀ഀ
਍            瀀漀爀琀猀⸀爀攀洀漀瘀攀䄀琀⠀椀⤀㬀ഀ
਍            戀爀攀愀欀㬀ഀ
        }਍ഀ
    }਍    ⼀⼀ﬀ摹練鑛葞ᅶ扨艟ﶂ﷿㿿ഀ
    for(int i=0;i<Widget::items.count();i++)਍    笀ഀ
        if(Widget::items.at(i)->text(0)==ip)਍        笀ഀ
            delete Widget::items.at(i);਍            圀椀搀最攀琀㨀㨀椀琀攀洀猀⸀爀攀洀漀瘀攀䄀琀⠀椀⤀㬀ഀ
਍        紀ഀ
    }਍    ⼀⼀ﬀ摹ₖ 猀搀琀爀愀礀渀甀洀  豈鑛艞릂൰
    for (QMultiMap<QString,int>::iterator ites=Widget::sdTraySta.begin();ites!=Widget::sdTraySta.end();ites++ )਍    笀ഀ
        if(ites.key()==ip)਍        笀ഀ
            do{਍                椀琀攀猀㴀圀椀搀最攀琀㨀㨀猀搀吀爀愀礀匀琀愀⸀攀爀愀猀攀⠀椀琀攀猀⤀㬀ഀ
            }while (ites.key()==ip);਍            戀爀攀愀欀㬀ഀ
਍        紀ഀ
    }਍    ⼀⼀ﬀ摹練鑛葞獶搀吀爀愀礀一甀洀 瀀콥එ
    Widget::sdMapNum -=16;਍ഀ
    m_socket->disconnectFromHost();਍ഀ
    emit disconnectTCP( ip);਍ഀ
    this->quit();਍紀ഀ
਍ഀ
਍ഀ
void serverThread::slotSendMMIDataTimeOut(void)਍笀ഀ
਍    ⼀⼀  儀䴀甀琀攀砀䰀漀挀欀攀爀  䰀漀挀欀攀爀⠀☀圀椀搀最攀琀㨀㨀洀甀琀攀砀⤀㬀ഀ
    qDebug()<<"CHID:"<<this->currentThreadId();਍    儀匀琀爀椀渀最  琀爀愀渀甀洀㬀ഀ
਍    攀洀椀琀  琀椀洀攀爀匀琀漀瀀⠀⤀㬀ഀ
    QMultiMap<QString,int>::iterator ites; //遍历map ;਍    儀䴀甀氀琀椀䴀愀瀀㰀儀匀琀爀椀渀最Ⰰ椀渀琀㸀㨀㨀椀琀攀爀愀琀漀爀 椀琀攀猀爀㬀 ⼀⼀䴀蚐浓愀瀀 㬀ഀ
਍    儀匀琀爀椀渀最  椀瀀 㴀洀开猀漀挀欀攀琀ⴀ㸀瀀攀攀爀䄀搀搀爀攀猀猀⠀⤀⸀琀漀匀琀爀椀渀最⠀⤀⸀爀攀洀漀瘀攀⠀　Ⰰ㜀⤀㬀  ⼀⼀搀楫瀀ﴀ⚀띔횃靓たﵒ﷿㿿ഀ
਍    昀漀爀⠀椀渀琀 椀㴀　㬀椀㰀圀椀搀最攀琀㨀㨀椀琀攀洀猀⸀氀攀渀最琀栀⠀⤀㬀椀⬀⬀⤀ഀ
    {਍        椀昀⠀圀椀搀最攀琀㨀㨀椀琀攀洀猀⸀愀琀⠀椀⤀ⴀ㸀琀攀砀琀⠀　⤀㴀㴀椀瀀⤀ഀ
        {਍            昀漀爀⠀椀渀琀 樀 㴀 　㬀樀㰀圀椀搀最攀琀㨀㨀椀琀攀洀猀⸀愀琀⠀椀⤀ⴀ㸀挀栀椀氀搀䌀漀甀渀琀⠀⤀㬀樀⬀⬀⤀ഀ
            {਍                椀昀⠀圀椀搀最攀琀㨀㨀椀琀攀洀猀⸀愀琀⠀椀⤀ⴀ㸀挀栀椀氀搀⠀樀⤀ⴀ㸀挀栀攀挀欀匀琀愀琀攀⠀　⤀ 㴀㴀 儀琀㨀㨀䌀栀攀挀欀攀搀⤀ഀ
                {਍                    琀爀愀渀甀洀㴀 圀椀搀最攀琀㨀㨀椀琀攀洀猀⸀愀琀⠀椀⤀ⴀ㸀挀栀椀氀搀⠀樀⤀ⴀ㸀琀攀砀琀⠀　⤀㬀     ⼀⼀뜀鞃塟�ﵶ﷿㿿ഀ
                    int trayNum = tranum.toInt();਍                    椀琀攀猀㴀圀椀搀最攀琀㨀㨀猀搀吀爀愀礀匀琀愀⸀昀椀渀搀⠀椀瀀⤀㬀ഀ
                    for ( int k =0;k< Widget::sdTraySta.count(ip); k++,ites++ )਍                    笀ഀ
                        if((k%4 == 0)&&(ites.value() ==trayNum  ))  //找到每个ip对应的托盘号位置਍                        笀ഀ
                            itesr=ites;਍                            昀漀爀 ⠀ 椀渀琀 渀 㴀　㬀渀㰀 圀椀搀最攀琀㨀㨀猀搀吀爀愀礀匀琀愀⸀挀漀甀渀琀⠀椀瀀⤀㬀 渀⬀⬀Ⰰ椀琀攀猀爀⬀⬀ ⤀ഀ
                            {਍                                椀昀⠀渀 㴀㴀 ㄀⤀  ⼀⼀縀ぢ卒䵟텒œސﵨ﷿㿿ഀ
                                {਍                                    椀昀⠀椀琀攀猀爀⸀瘀愀氀甀攀⠀⤀ 㴀㴀 ㄀⤀ ⼀⼀⼀墋�蕶텥ﵓ﷿㿿ഀ
                                    {਍                                        攀洀椀琀  爀攀猀攀渀搀䐀愀琀愀⠀椀瀀Ⰰ  琀爀愀礀一甀洀ⴀ㄀Ⰰ 圀椀搀最攀琀㨀㨀爀甀渀渀椀渀最䌀洀搀⤀㬀  ⼀⼀촀톑ﵓ﷿㿿ഀ
਍                                    紀ഀ
                                    else਍                                        戀爀攀愀欀㬀ഀ
਍                                紀ഀ
                                if(n == 3)//定位到重新发送次��?਍                                笀ഀ
                                    itesr.value()++;਍                                    椀昀⠀椀琀攀猀爀⸀瘀愀氀甀攀⠀⤀㸀㴀㌀⤀ ⼀⼀舀啙쵏톑œ←火❥ﵙ﷿㿿ഀ
                                    {਍                                        ⼀⼀ᨀ䱷扵㺗㩦᩹ㅏ╙ඍ
਍                                        琀栀椀猀ⴀ㸀猀栀漀眀䌀漀洀洀䘀愀甀氀琀⠀椀瀀Ⰰ琀爀愀礀一甀洀ⴀ㄀⤀㬀ഀ
਍                                        爀攀琀甀爀渀㬀ഀ
                                    }਍                                    爀攀琀甀爀渀㬀ഀ
                                }਍                            紀ഀ
                        }਍                    紀ഀ
                }਍            紀ഀ
਍        紀ഀ
    }਍ഀ
}਍ഀ
//处理重新发送失败异��?਍瘀漀椀搀 猀攀爀瘀攀爀吀栀爀攀愀搀㨀㨀猀栀漀眀䌀漀洀洀䘀愀甀氀琀⠀挀漀渀猀琀 儀匀琀爀椀渀最 ☀椀瀀Ⰰ 挀漀渀猀琀 椀渀琀 琀爀愀礀渀甀洀⤀ഀ
{਍    ⼀⼀  儀䴀甀琀攀砀䰀漀挀欀攀爀  䰀漀挀欀攀爀⠀☀洀甀琀攀砀⤀㬀ഀ
    uint a =0,k=0;਍    儀匀琀爀椀渀最  挀栀攀㬀ഀ
    QString numbr = QString::number(traynum +1);਍ഀ
਍    昀漀爀⠀椀渀琀 椀㴀　㬀椀㰀圀椀搀最攀琀㨀㨀椀琀攀洀猀⸀氀攀渀最琀栀⠀⤀㬀椀⬀⬀⤀ഀ
    {਍        椀昀⠀圀椀搀最攀琀㨀㨀椀琀攀洀猀⸀愀琀⠀椀⤀ⴀ㸀琀攀砀琀⠀　⤀㴀㴀椀瀀⤀ഀ
        {਍            昀漀爀⠀椀渀琀 樀 㴀 　㬀樀㰀圀椀搀最攀琀㨀㨀椀琀攀洀猀⸀愀琀⠀椀⤀ⴀ㸀挀栀椀氀搀䌀漀甀渀琀⠀⤀㬀樀⬀⬀⤀ഀ
            {਍                椀昀⠀圀椀搀最攀琀㨀㨀椀琀攀洀猀⸀愀琀⠀椀⤀ⴀ㸀挀栀椀氀搀⠀樀⤀ⴀ㸀琀攀砀琀⠀　⤀ 㴀㴀 渀甀洀戀爀⤀ഀ
                {਍ഀ
                    //items.at(i)->child(j)->setCheckState(0,Qt::Unchecked);  //设置其为不选中状��?਍                    ⼀⼀ 猀攀琀吀爀攀攀一漀搀攀䠀椀搀搀攀渀⠀椀琀攀洀猀⸀愀琀⠀椀⤀Ⰰ渀甀洀戀爀Ⰰ㄀⤀㬀ഀ
                    for (QMultiMap<QString,int>::iterator ites=Widget::sdTraySta.begin();ites!=Widget::sdTraySta.end();ites++ )਍                    笀ഀ
                        che = ites.key();਍                        椀昀⠀椀琀攀猀⸀欀攀礀⠀⤀ 㴀㴀 椀瀀⤀ഀ
                        {਍                            椀昀⠀⠀欀─㐀 㴀㴀 　⤀☀☀⠀椀琀攀猀⸀瘀愀氀甀攀⠀⤀ 㴀㴀 ⠀琀爀愀礀渀甀洀⬀㄀⤀⤀⤀ഀ
                            {਍                                椀昀⠀圀椀搀最攀琀㨀㨀爀甀渀渀椀渀最䌀洀搀 㴀㴀 ✀砀✀⤀ഀ
                                {਍                                    儀匀琀爀椀渀最 琀攀砀琀 㴀儀䐀愀琀攀吀椀洀攀㨀㨀挀甀爀爀攀渀琀䐀愀琀攀吀椀洀攀⠀⤀⸀琀漀匀琀爀椀渀最⠀∀礀礀礀礀ⴀ䴀䴀ⴀ搀搀 栀栀㨀洀洀㨀猀猀㨀 ∀⤀⬀ഀ
                                            ip+ " "+numbr+"号托盘升级失败！";਍                                    攀洀椀琀 爀甀渀渀椀渀最䴀猀最唀瀀搀愀琀攀⠀琀攀砀琀⤀㬀ഀ
                                }਍                                攀氀猀攀 椀昀⠀圀椀搀最攀琀㨀㨀爀甀渀渀椀渀最䌀洀搀 㴀㴀 ✀稀✀⤀ഀ
                                {਍                                    儀匀琀爀椀渀最 琀攀砀琀 㴀儀䐀愀琀攀吀椀洀攀㨀㨀挀甀爀爀攀渀琀䐀愀琀攀吀椀洀攀⠀⤀⸀琀漀匀琀爀椀渀最⠀∀礀礀礀礀ⴀ䴀䴀ⴀ搀搀 栀栀㨀洀洀㨀猀猀㨀 ∀⤀⬀ഀ
                                            ip+ " "+numbr+"号托盘运行失败！";਍                                    攀洀椀琀 爀甀渀渀椀渀最䴀猀最唀瀀搀愀琀攀⠀琀攀砀琀⤀㬀ഀ
                                }਍                                攀氀猀攀ഀ
                                {਍                                    㬀ഀ
                                }਍ഀ
                                a = 1;਍                            紀ഀ
                            if(a == 1)਍                            笀ഀ
਍                                昀漀爀⠀椀渀琀 砀㴀　㬀砀㰀㐀㬀砀⬀⬀⤀ഀ
                                {਍                                    椀琀攀猀 㴀圀椀搀最攀琀㨀㨀猀搀吀爀愀礀匀琀愀⸀攀爀愀猀攀⠀椀琀攀猀⤀㬀ഀ
                                    Widget::sdMapNum --;਍                                紀ഀ
                                a =0;਍                                攀洀椀琀  最漀琀漀一攀砀琀猀攀渀搀⠀椀瀀Ⰰ圀椀搀最攀琀㨀㨀爀甀渀渀椀渀最䌀洀搀 ⤀㬀ഀ
਍                            紀ഀ
                        }਍                        欀⬀⬀㬀ഀ
                        if(k>Widget::sdMapNum )਍                            爀攀琀甀爀渀㬀ഀ
                    }਍                紀ഀ
            }਍ഀ
਍        紀ഀ
    }਍ഀ
}਍