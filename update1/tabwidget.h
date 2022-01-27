#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>

namespace Ui {
class TabWidget;
}

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit TabWidget(QWidget *parent = 0);
    ~TabWidget();

private slots:


    void on_textEdit_2_destroyed();

    void on_textEdit_destroyed();

private:
    Ui::TabWidget *ui;
};

#endif // TABWIDGET_H
