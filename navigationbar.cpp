#include "navigationbar.h"
#include "global.h"
#include <QDebug>



NavigationBar::NavigationBar(QWidget* parent):QWidget(parent)
{
    init();

    connect(pButton0, SIGNAL(clicked()), this, SLOT(slot_button0_clicked()));
    connect(pButton1, SIGNAL(clicked()), this, SLOT(slot_button1_clicked()));
    connect(pButton2, SIGNAL(clicked()), this, SLOT(slot_button2_clicked()));
    connect(pButton3, SIGNAL(clicked()), this, SLOT(slot_button3_clicked()));
    connect(pButton4, SIGNAL(clicked()), this, SLOT(slot_button4_clicked()));


}


void NavigationBar::init()
{
    pButton0 = new QToolButton;
    pButton0->setText(QStringLiteral("滤波"));
    pButton0->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    pButton0->setIcon(QIcon(PATH_IMAGE_FILTER_PROCESS_Navigation));
    pButton0->setIconSize(QSize(64, 64));
    pButton0->setFixedSize(QSize(96, 96));
    //pButton0->setCheckable(true);

    pButton1 = new QToolButton;
    pButton1->setText(QStringLiteral("二值化"));
    pButton1->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    pButton1->setIcon(QIcon(PATH_IMAGE_BINARY_PROCESS_Navigation));
    pButton1->setIconSize(QSize(64, 64));
    pButton1->setFixedSize(QSize(96, 96));
    //pButton1->setCheckable(true);

    pButton2 = new QToolButton;
    pButton2->setText(QStringLiteral("形态学"));
    pButton2->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    pButton2->setIcon(QIcon(PATH_IMAGE_MORPH_PROCESS_Navigation));
    pButton2->setIconSize(QSize(64, 64));
    pButton2->setFixedSize(QSize(96, 96));
    //pButton2->setCheckable(true);


    pButton3 = new QToolButton;
    pButton3->setText(QStringLiteral("粗检测"));
    pButton3->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    pButton3->setIcon(QIcon(PATH_IMAGE_CFAR_DETECT_PROCESS_Navigation));
    pButton3->setIconSize(QSize(64, 64));
    pButton3->setFixedSize(QSize(96, 96));
    //pButton3->setCheckable(true);


    pButton4 = new QToolButton;
    pButton4->setText(QStringLiteral("精检测"));
    pButton4->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    pButton4->setIcon(QIcon(PATH_IMAGE_HOG_AND_SVM_PROCESS_Navigation));
    pButton4->setIconSize(QSize(64, 64));
    pButton4->setFixedSize(QSize(96, 96));
    //pButton4->setCheckable(true);


    layout = new QVBoxLayout;
    layout->addWidget(pButton0);
    layout->addWidget(pButton1);
    layout->addWidget(pButton2);
    layout->addWidget(pButton3);
    layout->addWidget(pButton4);

    this->setLayout(layout);
}


void NavigationBar::slot_button0_clicked()
{
    emit signals_button_clicked(0);
}

void NavigationBar::slot_button1_clicked()
{
    emit signals_button_clicked(1);
}


void NavigationBar::slot_button2_clicked()
{
    emit signals_button_clicked(2);
}


void NavigationBar::slot_button3_clicked()
{
    emit signals_button_clicked(3);
}


void NavigationBar::slot_button4_clicked()
{
    emit signals_button_clicked(4);

}



//StatusBar::StatusBar(QWidget* parent):QStatusBar(parent)
//{
//    createControls();

//    connect(viewPushButton, SIGNAL(clicked())
//            ,this, SLOT(slot_viewPushButton_clicked()));

//}

//void StatusBar::createControls()
//{
//    //QLabel init
//    statusLabel = new QLabel;
//    statusLabel->setText((QStringLiteral("就绪")));
//    QFont font ("Microsoft YaHei", 10, 20);   //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
//    statusLabel->setFont(font);

//    //QPushButton init
//    viewPushButton = new QPushButton;
//    viewPushButton->setText(QStringLiteral("测试按钮菜单"));
//   // viewPushButton->setMenu(m_menu);//设置菜单
//    viewPushButton->setStyleSheet("QPushButton::menu-indicator{image:none;}");//不显示下拉图片


//    this->addWidget(statusLabel);
//    this->addWidget(viewPushButton);


//}


//void StatusBar::slot_changeQLabelStatus(QString str)
//{
//    statusLabel->setText(str);
//}

//void StatusBar::initViewPushButton()             //创建 右键菜单栏目
//{


//}



//void StatusBar::slot_viewPushButton_clicked()
//{


//}
