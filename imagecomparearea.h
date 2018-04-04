#ifndef IMAGECOMPAREAREA_H
#define IMAGECOMPAREAREA_H

#include "imageview.h"
#include <QWidget>
#include <QRect>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QScrollBar>
#include <QMessageBox>

class QImageWidget;

// 实现图片展示区 对比窗口

//左边图像展示区
class QLeftImageWidget: public QImageWidget
{
    Q_OBJECT
public:

    explicit QLeftImageWidget(QWidget* parent = NULL);
    void paintEvent(QPaintEvent* e);     //在paintEvent中发射信号，实现同步画图

//    void mouseMoveEvent(QMouseEvent* e);
//    void mouseReleaseEvent(QMouseEvent* e);


    void dragEnterEvent(QDragEnterEvent *event);  //当用户拖动文件到窗口部件上时候，就会触发dragEnterEvent事件
    void dropEvent(QDropEvent * event);           //当用户放下这个文件后，就会触发dropEvent事件


signals:
    void signals_deliver_tgtRect(QRectF);

public slots:
    void slots_receive_tgtRect(QRectF rect);

    void slots_Load_iamge_compare_left_area(QImage image);



private:
    bool isCurAppListWidgetDrag;   //是否在当前的应用程序 listWidget控件中拖动
};

//右边图像展示区
class QRightImageWidget: public QImageWidget
{
    Q_OBJECT
public:

    explicit QRightImageWidget(QWidget* parent = NULL);
    void paintEvent(QPaintEvent* e);     //在paintEvent中发射信号，实现同步画图


//    void mouseMoveEvent(QMouseEvent* e);
//    void mouseReleaseEvent(QMouseEvent* e);



    void dragEnterEvent(QDragEnterEvent *event);  //当用户拖动文件到窗口部件上时候，就会触发dragEnterEvent事件
    void dropEvent(QDropEvent *event);           //当用户放下这个文件后，就会触发dropEvent事件


signals:
    void signals_deliver_tgtRect(QRectF);

public slots:
    void slots_receive_tgtRect(QRectF);
    void slots_Load_iamge_compare_right_area(QImage);



private:
    bool isCurAppListWidgetDrag;   //是否在当前的应用程序 listWidget控件中拖动
};


//总的展示窗口
class QImageCompareWidget: public QWidget
{
    Q_OBJECT
public:
    explicit QImageCompareWidget(QWidget* parent = NULL);
    virtual ~QImageCompareWidget(){}

    void init();
    void signals_slots_binds();

    bool eventFilter(QObject *watched, QEvent *event);   //设置监控器

public slots:
    void slots_Load_iamge_compare_left_area(QImage image);
    void slots_Load_iamge_compare_right_area(QImage image);



private:
    QLeftImageWidget * leftImageWidget;
    QRightImageWidget * rightImageWidget;
    QScrollArea *leftScroll;
    QScrollArea *rightScroll;
    QHBoxLayout *layout;

    QScrollBar *leftHBar;
    QScrollBar *leftVBar;

    QScrollBar *rightHBar;
    QScrollBar *rightVBar;
};





#endif // IMAGECOMPAREAREA_H
