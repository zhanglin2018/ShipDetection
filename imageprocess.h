#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QWidget>
#include <QListWidget>
#include <QGridLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QStackedLayout>
#include <QVBoxLayout>
#include "imageFilter.h"
#include "imageFFT.h"
#include "imageBinary.h"
#include "imageSVM.h"
#include "imageview.h"
#include <opencv2/opencv.hpp>
using namespace cv;

// 定义了 QImage 与 Mat 矩阵相互转换的函数
QImage cvMat2QImage(const cv::Mat mat); // 将Mat 转换为 QImage

cv::Mat QImage2cvMat(QImage image);   // 将 QImage 转换为 Mat

//图像滤波操作
class ImageFilterProcessWidget : public QWidget
{
    Q_OBJECT

public:
    ImageFilterProcessWidget(QWidget *parent = NULL);
    ~ImageFilterProcessWidget();

    void init();						//显示图像名称信息

//public slots:
//    QImage slots_getImage_and_ImageFilterProcess(QImage image);      //接收图像，并进行图像处理

//public:

public:
    QTabWidget  * tabWidget;
    QListWidget * listWidget;
    QListWidgetItem * item0;
    QListWidgetItem * item1;
    QListWidgetItem * item2;
    QListWidgetItem * item3;
    QListWidgetItem * item4;
    QListWidgetItem * item5;
    QPushButton * pushbutton;
    QGridLayout * gridLayout;
};


//图像二值化操作
class ImageBinaryProcessWidget : public QWidget
{
    Q_OBJECT
public:
    ImageBinaryProcessWidget(QWidget *parent = NULL);
    ~ImageBinaryProcessWidget();

    void init();						//显示图像名称信息
public:
    QTabWidget  * tabWidget;
    QListWidget * listWidget;
    QListWidgetItem * item0;
    QListWidgetItem * item1;
    QListWidgetItem * item2;
    QListWidgetItem * item3;
    QListWidgetItem * item4;
    QListWidgetItem * item5;
    QPushButton * pushbutton;

    QGridLayout * gridLayout;

};

//二值化后形态学
enum Structuring_Element{
    MORPH_RECT=0,
    MORPH_CROSS=1,
    MORPH_ELLIPSE=2
};

//进行形态学操作  type of morphological operation
enum Morphological_Operation{
    MORPH_ERODE_OBJ = CV_MOP_ERODE,
    MORPH_DILATE_OBJ = CV_MOP_DILATE,
    MORPH_OPEN_OBJ = CV_MOP_OPEN,              //开运算       dilate(erode(src, element))
    MORPH_CLOSE_OBJ = CV_MOP_CLOSE,            //闭运算       erode(dilate(src, element))  比较理想
    MORPH_GRADIENT_OBJ = CV_MOP_GRADIENT,      //形态学梯度   dilate(src, element) - erode(src, element)
    MORPH_TOPHAT_OBJ = CV_MOP_TOPHAT,          //顶帽运算     src - open(src, element)
    MORPH_BLACKHAT_OBJ = CV_MOP_BLACKHAT       //黑帽运算     close（src, element) - src
};


//图像形态学操作
class ImageMorphProcessWidget : public QWidget
{
    Q_OBJECT
public:
    ImageMorphProcessWidget(QWidget *parent = NULL);
    ~ImageMorphProcessWidget();

    void init();						//显示图像名称信息

public:
    QTabWidget  * tabWidget;
    QListWidget * listWidget;
    QListWidgetItem * item0;
    QListWidgetItem * item1;
    QListWidgetItem * item2;
    QListWidgetItem * item3;
    QListWidgetItem * item4;
    QListWidgetItem * item5;
    QListWidgetItem * item6;
    QPushButton * pushbutton;

    QGridLayout * gridLayout;
    Mat srcImage;
};



//图像CfarDetect操作
class ImageCfarDectectProcessWidget : public QWidget
{
    Q_OBJECT
public:
    ImageCfarDectectProcessWidget(QWidget *parent = NULL);
    ~ImageCfarDectectProcessWidget();

    void init();						//显示图像名称信息


public:
    QTabWidget  * tabWidget;
    QListWidget * listWidget;
    QListWidgetItem * item0;
    QListWidgetItem * item1;
    QListWidgetItem * item2;
    QListWidgetItem * item3;
    QListWidgetItem * item4;
    QPushButton * pushbutton;

    QGridLayout * gridLayout;

};




//图像SVM操作
class ImageHogAndSvmProcessWidget : public QWidget
{
    Q_OBJECT
public:
    ImageHogAndSvmProcessWidget(QWidget *parent = NULL);
    ~ImageHogAndSvmProcessWidget();

    void init();
public:
    QTabWidget  * tabWidget;
    QListWidget * listWidget;
    QListWidgetItem * item0;
    QListWidgetItem * item1;
    QListWidgetItem * item2;
    QListWidgetItem * item3;
    QListWidgetItem * item4;
    QListWidgetItem * item5;

    QPushButton * pushbutton;
    QGridLayout * gridLayout;
};


class ImageProcessWidget: public QWidget
{
    Q_OBJECT
public:
    ImageProcessWidget(QWidget *parent = NULL);
    ~ImageProcessWidget();

    void init();

signals:
    void signals_deliver_image_filter_result_to_central_area(QImage);  //发送图像处理结果到中央图像区域
    void signals_deliver_little_image_filter(QImage, QString);

    void signals_deliver_image_binary_result_to_central_area(QImage);  //发送图像处理结果到中央图像区域
    void signals_deliver_little_image_binary(QImage, QString);

    void signals_deliver_image_morph_result_to_central_area(QImage);  //发送图像处理结果到中央图像区域
    void signals_deliver_little_image_morph(QImage, QString);

    void signals_deliver_image_cfar_detect_result_to_central_area(QImage);  //发送图像处理结果到中央图像区域
    void signals_deliver_little_image_cfarDetect(QImage, QString);

    void signals_deliver_image_hog_svm_result_to_central_area(QImage);  //发送图像处理结果到中央图像区域
    void signals_deliver_little_image_HogAndSvm(QImage, QString);

    void signals_deliver_image_getRoi_result_to_central_area(QImage);  //发送图像处理结果到中央图像区域
    void signals_deliver_little_image_GetRoi(QImage, QString);
    void signals_deliver_ObjectDetectResultParas_to_ObjectDetectOptionWidget(QVector<ObjectDetectResultParas>);

    void signals_deliver_image_landDetect_result_to_central_area(QImage);  //发送图像处理结果到中央图像区域
    void signals_deliver_little_image_landDetect(QImage, QString);

    void signals_deliver_image_drawContour_result_to_central_area(QImage);  //发送图像处理结果到中央图像区域
    void signals_deliver_little_image_DrawContour(QImage, QString);

    void signals_deliver_objectDetectOutputInfo(QString);     //发送目标的检测信息到输出窗口


public slots:
    void slot_switch_different_widget(int);

    void slots_getImage_and_ImageFilterProcess(QImage image);    //滤波
    void slots_getImage_and_imageBinaryProcess(QImage image);    //二值化
    void slots_getImage_and_imageMorphProcess(QImage image);    //形态学
    void slots_getImage_and_imageCfarDetectProcess(QImage image);    //cfar检测
    void slots_getImage_and_imageHogAndSvmProcess(QImage image);    //cfar检测
    void slots_getImage_and_imageGetRoiProcess(QImage image);      //获取检测图像的 ROI
    void slots_getImage_and_imageLandDetectProcess(QImage image);      //获取检测图像的 ROI
    void slots_getImage_and_imageDrawContour_AfterDeleteFalseObjProcess(QImage image);      //获取检测图像的 ROI
    void slots_getImage_and_imageDrawContourProcess(QImage image);      //获取检测图像的 ROI
    void slots_getImage_and_imageDrawContour_AfterPreciseProcess(QImage image);      //获取检测图像的 ROI


private:
    void imageHogAndSvmProcess_delete_false_object_showImageNumber(QImage image, QString processName);
    void imageHogAndSvmProcess_delete_false_object_noShowImageNumber(QImage image, QString processName);
    void imageHogAndSvmProcess_delete_false_object_noShowImageNumber_original_rectangle(QImage image, QString processName);

    void imageHogAndSvmProcess_svm_recognition(QImage image, QString processName);
    void imageHogAndSvmProcess_svm_recognition_showImageNumber(QImage image, QString processName);
    void imageHogAndSvmProcess_svm_recognition__noShowImageNumber_original_rectangle(QImage image, QString processName);


public:
    ImageFilterProcessWidget *widget0;
    ImageBinaryProcessWidget *widget1;
    ImageMorphProcessWidget *widget2;
    ImageCfarDectectProcessWidget *widget3;
    ImageHogAndSvmProcessWidget *widget4;
    QStackedLayout * stackedLayout;
};


#endif // IMAGEPROCESS_H
