#include "login.h"



Mlogin::Mlogin(QWidget *parent):QDialog(parent)
{


    QLabel *pUserLabel = new QLabel("用户名:",this);
    QLabel *pPwdLabel = new QLabel("密  码:",this);
    m_pUserLineEdit = new QLineEdit(this);
    m_pPwdLineEdit = new QLineEdit(this);
    pUserLabel->move(70,80);
    m_pUserLineEdit->move(150,80);
     pPwdLabel->move(70,130);
     m_pPwdLineEdit->move(150,130);
    //设置输入框回显样式
    m_pPwdLineEdit->setEchoMode(QLineEdit::Password);

    QPushButton *pOkBtn = new QPushButton("确定",this);
    QPushButton *pCancelBtn = new QPushButton("取消",this);

    pOkBtn->move(100,200);
    pCancelBtn->move(220,200);

    connect(pOkBtn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(pCancelBtn, SIGNAL(clicked()), this, SLOT(reject()));

    setWindowTitle("登录");
    resize(400,300);


}

Mlogin::~Mlogin()
{

}

void Mlogin::accept()
{
    if (m_pUserLineEdit->text() == "123" && m_pPwdLineEdit->text() == "123")
    {
        QDialog::accept();
    }
    else
    {
        QMessageBox::warning(this, "警告", "用户名或密码错误!", QMessageBox::Yes);
        m_pPwdLineEdit->clear();
        m_pUserLineEdit->clear();
        m_pUserLineEdit->setFocus();
    }

}
