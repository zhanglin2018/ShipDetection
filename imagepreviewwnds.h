#ifndef _IMAGEPREVIEWWNDS_H_
#define _IMAGEPREVIEWWNDS_H_


#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTextEdit>
#include <QTreeWidget>
#include <QList>
#include <QMap>
#include <QPixmap>
#include <QVBoxLayout>
#include <QtCore>
#include <QApplication>
#include <QDrag>
#include <QMouseEvent>


//图像名称列表
class CImgNameListWidget : public QListWidget
{
    Q_OBJECT
public:
    CImgNameListWidget(QWidget *parent = NULL);
    ~CImgNameListWidget();

    void updateImgsName();						//显示图像名称信息

};

//图像缩略图列表
class ClitImgListWidget : public QWidget
{
    Q_OBJECT
public:
    ClitImgListWidget(QWidget *parent = NULL);
    ~ClitImgListWidget();

    void initialization();                     //初始化方式
    void clearAllImageAndNameList();           //清空所有的缩略图像
    void keyPressEvent(QKeyEvent* e);          //按键功能

    //void mousePressEvent(QMouseEvent *event);

    //void mouseMoveEvent(QMouseEvent *event);

    //void start_drag();


signals:
    void signals_listItemDoubleClicked(QImage);
    void signals_Load_iamge_compare_left_area(QImage);
    void signals_Load_iamge_compare_right_area(QImage);


public slots:
    void slot_addListItem();                        //在空间当中添加缩略图
    void slot_addQImage(QImage image);           //添加 缩略图
    void slot_itemDoubleClicked();                  //双击listWidget 中的item项

    void slot_customContextMenuRequested(const QPoint &pos);
    void slot_deleteListItem();
    void slot_clearList();
    void slot_saveListItem();        //保存单张图片
    void slot_saveAllListItem();     //保存所有图片

    void slot_Load_iamge_compare_left_area();
    void slot_Load_iamge_compare_right_area();

    // 图像处理 保存结果
    void slot_receive_little_image(QImage image, QString str);


public:
    QListWidget * listWidget;
    QVBoxLayout * layout;
    QList<QString> imageNameList;
    QList<QImage> imageMap;
    QList<QString> processNameList;   //图像处理流程的名字
    QList<QString> imagePathList;     //2/7 add imagePathList
    QPoint startPos;
};




//Log 信息显示窗口
class CLogInfoWidget : public QTextEdit
{
    Q_OBJECT
public:
    CLogInfoWidget(QWidget *parent = NULL);
    ~CLogInfoWidget();

public:
    void updateLogInfo(const QString &strInfo);
};



#endif
