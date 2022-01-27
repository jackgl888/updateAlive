#ifndef PARENTDEVICE_H
#define PARENTDEVICE_H


#include <QObject>


class parentdevice : public QObject
{
    Q_OBJECT
public:
    explicit parentdevice( QObject *parent = nullptr);
        ~parentdevice();

       void setParentDeviceAddr(uchar addr );
       uchar getParentDeviceAddr();
       bool  getIsBoot();
       void setIsBoot(bool status);
signals:

public slots:

private:
       uchar  parentDeviceAddr;
       bool  isBoot;

};

#endif // POWERMSG_H
