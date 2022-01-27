#ifndef QIPLINEDITER_H
#define QIPLINEDITER_H
#include <QLineEdit>
#include <QObject>
#include <QEvent>
#include <QRegExpValidator>
#include <QPainter>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMessageBox>

class QIPlinEditer : public QLineEdit
{
    Q_OBJECT
public:
    explicit QIPlinEditer(QWidget *parent = nullptr);
    ~QIPlinEditer();

    void setText(const QString & strIP);
    QString  text() const;
protected:
      void  paintEvent(QPaintEvent *event);
      bool   eventFilter(QObject *obj, QEvent *ev);

      int getIndex(QLineEdit  *pEdit);
      bool  isTextValid(const  QString  &strIP);
signals:

private:
      QLineEdit  *m_lineEdit[4];

};

#endif // QIPLINEDITER_H
