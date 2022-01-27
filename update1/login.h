#ifndef login_H
#define login_H
#include <QDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QLabel>
#include <QGridLayout>
#include  <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class QLineEdit;

class Mlogin : public QDialog
{

    Q_OBJECT
public:
    Mlogin(QWidget *parent = 0);
    ~Mlogin();

private:
     QLineEdit *m_pUserLineEdit;
     QLineEdit *m_pPwdLineEdit;

private   slots:
     virtual void  accept();
};

#endif // Mlogin_H
