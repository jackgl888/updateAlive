#include "maindialog.h"

mainDialog::mainDialog(QWidget *parent) : QDialog(parent)
{
     this->funcModules = new QTabWidget(this);
     this->m_widget = new Widget(
                this->funcModules );
    this->funcModules->addTab(this->m_widget,"在线升级");
    this->funcModules->  adjustSize();

    this->resize(1800,1000);
    this->setWindowTitle("debugTools");
}





mainDialog::~mainDialog()
{
    
}
