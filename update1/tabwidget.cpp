#include "tabwidget.h"


TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::TabWidget)
{
    ui->setupUi(this);
}

TabWidget::~TabWidget()
{
    delete ui;
}



void TabWidget::on_textEdit_2_destroyed()
{

}

void TabWidget::on_textEdit_destroyed()
{

}
