#ifndef DATARECV_H
#define DATARECV_H
#include <QHostAddress>
#include <QObject>
#include <QTimer>
class datarecv : public QObject
{
    Q_OBJECT
public:
    explicit datarecv(QObject *parent = nullptr);
      ~datarecv();

    QByteArray dataGet;
    QHostAddress  addrGet;
    quint16 portGet;
signals:

public slots:
  void  m_timerEvent();
private:
      QTimer  *m_timer;

};



#endif // DATARECV_H
