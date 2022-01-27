#ifndef DATASENT_H
#define DATASENT_H
#include "single.h"
#include <QObject>

class dataSent : public QObject
{
    Q_OBJECT
public:
    explicit dataSent(QObject *parent = nullptr);
      ~dataSent();

signals:
      void sendData();  //
public slots:

     slotDataSent();
};

#endif // DATASENT_H
