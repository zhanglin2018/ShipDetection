#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H

#include <QWidget>
#include <QToolButton>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QLabel>
#include <QPushButton>

class  NavigationBar: public QWidget
{
    Q_OBJECT
public:

    explicit NavigationBar(QWidget* parent = NULL);

public:
    void init();

signals:
    void signals_button_clicked(int);

public slots:
    void slot_button0_clicked();
    void slot_button1_clicked();
    void slot_button2_clicked();
    void slot_button3_clicked();
    void slot_button4_clicked();

public:
    QToolButton *pButton0;
    QToolButton *pButton1;
    QToolButton *pButton2;
    QToolButton *pButton3;
    QToolButton *pButton4;
    QVBoxLayout *layout;
};


//class StatusBar: public QStatusBar
//{
//     Q_OBJECT
//public:
//    explicit StatusBar(QWidget* parent = NULL);
//    void createControls();

//public:
//    void initViewPushButton();              //创建 右键菜单栏目

//signals:
//    void signal_getQMenu_from_MainWindow();

//public slots:
//    void slot_changeQLabelStatus(QString);      //改变状态栏中 label的显示状态
//    void slot_viewPushButton_clicked();         //viewPushButton 单机槽函数
//    //void slot_getQMenu_from_MainWindow();     //从mainWindow窗口中 获取设置好的QMenu


//public:
//    QLabel *statusLabel;
//    QPushButton* viewPushButton;

//};


#endif // NAVIGATIONBAR_H
