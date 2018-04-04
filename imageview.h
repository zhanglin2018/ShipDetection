#ifndef IMAGEVIEW
#define IMAGEVIEW

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QResizeEvent>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QtCore>
#include <QTableWidget>
#include <QLineEdit>
#include "imagepreviewwnds.h"
#include <QCheckBox>
#include <QComboBox>
#include "imageSVM.h"
using namespace ObjectImageSVM;

QString format2QString(QImage::Format format);
Rect assignSampleObjectSize(Size Original_image_size, Rect target_result_box
                            , Size box_size);


class QImageWidget : public QWidget
{
    Q_OBJECT
public:

    explicit QImageWidget(QWidget* parent = NULL);

    void paintEvent(QPaintEvent* e);

    // 载入图像
    bool setImage(const QImage& qIamge);
    bool setImage(const QString& imgFile);

    void resizeEvent(QResizeEvent *e);    //当窗口发生改变的时候，必须改变目标窗口的尺寸值   tgtRect

    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void wheelEvent(QWheelEvent *e);

    void dragEnterEvent(QDragEnterEvent *event);

    void dropEvent(QDropEvent * event);

    QImage getQImage();
    QPixmap getQPixmap();

signals:
void signals_deliver_captureQImage(QImage);     // 发送 截屏图
void signals_deliver_little_QPixMap(QImage);   // 发送 缩略图
void signals_deliver_curQImage_to_filter(QImage);
void signals_deliver_curQImage_to_binary(QImage);
void signals_deliver_curQImage_to_morph(QImage);
void signals_deliver_curQImage_to_cfarDetect(QImage);
void signals_deliver_curQImage_to_hog_svm(QImage);
void signals_deliver_curQImage_to_getRoi(QImage);  //发送当前窗口中的QImage
void signals_deliver_curQImage_to_landDelete(QImage);  //发送当前窗口中的QImage
void signals_deliver_curQImage_to_drawContour(QImage);  //发送当前窗口中的QImage
void signals_deliver_curQImage_to_drawContour_afterDeleteFalseObj(QImage);  //发送当前窗口中的QImage
void signals_deliver_curQImage_to_drawContour_afterPrecise(QImage);  //发送当前窗口中的QImage




void signals_imageDescriptionParas();

// 鼠标移动时， 发射当前的 图片的位置 和 像素值 到图片显示 窗口区域
void signals_deliver_CurQImage_pixPos_pixValue(QPoint, QVector<uchar>);

private:

void get_CurQImage_pixPos_pixValue(QMouseEvent* event, QPoint& point, QVector<uchar> &pixValue);


public slots:
    bool slot_loadImageFile();           // 加载图片文件
    void slot_saveImageFile();           // 保存图片文件
    void slot_zoomin();                  // 放大功能
    void slot_zoomout();                 // 缩小功能
    void slot_imageCut();                // 实现截图的功能
    void slot_oneKeyProcessing();        // 图片一键处理
    void slot_processingExit();          // 程序退出功能
    void slot_imageRefresh();            // 图片刷新功能

    void slot_imageCut_emit_signals();   //截图后 发送信号
    void slot_littleImage_emit_signals();  //发送QPixMap
    void slot_curQImage_emit_signals_to_filter();    //发送QImage
    void slot_curQImage_emit_signals_to_binary();    //发送QImage
    void slot_curQImage_emit_signals_to_morph();    //发送QImage
    void slot_curQImage_emit_signals_to_cfarDetect();    //发送QImage
    void slot_curQImage_emit_signals_to_hog_svm();    //发送QImage
    void slot_curQImage_emit_signals_to_getRoi();  //发送当前窗口中的QImage
    void slot_curQImage_emit_signals_to_landDelete();  //发送当前窗口中的QImage
    void slot_curQImage_emit_signals_to_drawContour();
    void slot_curQImage_emit_signals_to_drawContour_afterDeleteFalseObj();
    void slot_curQImage_emit_signals_to_drawContour_afterPrecise();




    void slot_listItemDoubleClicked(QImage);

    void slot_receive_image_process_result_to_central_area(QImage image);   //接受图像处理的结果，并进行显示

public:
    static QImage getCentralQImage();


protected:
    bool    mRightKeyStartedMove;
    bool    mLeftKeyStartedMove;

    QPoint  mStartPosition;
    QRectF  tgtRect;              // 画图的时候，每次确定目标窗口
    QString imageFile;            // 读入图片文件的路径
    bool    imageUpdated;         // 图片更新了
    static QImage  curImage;             // 存储当前图像， 用于存储图像，方便像素处理
    Mat     curNewMat;
    QPixmap curPixmap;            // 存储当前的图像，用于显示 画图的功能

    QPoint  mEndPosition;
    QImage  captureQImage;        //截图出来的图片 存储
    QRect  captureRectF;
};


/*
写入一个截图后 显示保存的 窗口
屏蔽掉父类的  各种鼠标事件（目的：该对象只用只读，显示图片对象）
*/

class CutImageShowWidget: public QImageWidget
{
public:

    //直接传入 QImage 图像后， 显示图像
    bool setImage(const QImage& qImage);


    explicit CutImageShowWidget(QWidget* parent = NULL);

    //屏蔽掉 父类的 各种 事件操作

    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void wheelEvent(QWheelEvent *e);

};

//截图显示窗口区域
class CutImageShowWnd: public QWidget
{
    Q_OBJECT
public:
    CutImageShowWnd(QWidget* parent = NULL);

    void createControls();
    void controlsLayout();
public slots:

    void slot_show_cutImage(QImage qImage);
    void slot_save_cutImage();

public:
    CutImageShowWidget *imageWidget;
    QTabWidget * imageTab;
    QPushButton * saveButton;
    QLabel *imageSizeName;
    QLabel *imageSizeLabel;
    QLabel *saveImageLabel;
    QLabel *imageWidthName;
    QLabel *imageHeightName;
    QSpinBox *imageWidth;
    QSpinBox *imageHeight;
    QHBoxLayout * hlayout;
    QGridLayout * layout;
    QGroupBox *saveBox;
    QVBoxLayout * vlayout;
    QGroupBox *saveBox1;
    QVBoxLayout *vlayout1;
    QGroupBox *groupBox1;
};


//2/6  添加图片的描述参数
class ImageDescriptionParas
{
public:
    QString imagePath;
    QSize   imageSize;
    QImage::Format  format;
    int     imageChannels;
    int     imageDepth;
    QPoint  imagePixPos;
    int     imagePixSingleValue;
    QVector<int>  imagePixThreeChannelsValue;
    QVector<int>  imagePixFourChannelsValue;

    static ImageDescriptionParas* getImageDescriptionParas();
    ~ImageDescriptionParas();


private:
    ImageDescriptionParas();
    ImageDescriptionParas(const ImageDescriptionParas&);
    ImageDescriptionParas& operator=(const ImageDescriptionParas*);

    static ImageDescriptionParas* instance;
    static QMutex mutex;
};

//2/28 目标检测信息输出 窗口， 由于窗口不能进行铺满操作，因此没有使用该窗口
class ObjectDetect_OutputInfo: public QWidget
{
    Q_OBJECT
public:
    ObjectDetect_OutputInfo(QWidget* parent = NULL);
    void init();


public:
    QTextEdit *svmPredict_OutputInfo_Edit;
    QGroupBox *mainGroupBox1;
    QHBoxLayout *hLayout1;

    QGridLayout *mainLayout;
};


//2/6 添加图片的描述窗口
class ImageDescriptionWidget: public QWidget
{
    Q_OBJECT
public:
    ImageDescriptionWidget(QWidget* parent = NULL);
    void initTableWidget();
    void initTableWidgetItem();

public slots:
    // 添加图片的描述信息
    void slots_imageDescriptionParas();
    void slots_receive_CurQImage_pixPos_pixValue(QPoint point, QVector<uchar> pixValue);

    //添加目标检测的输出信息
    void slot_ObjectDetect_Output_Info(QString string);

public:
    QTableWidget *tableWidget;
    QTextEdit *svmPredict_OutputInfo_Edit;
    QGroupBox *mainGroupBox1;
    QHBoxLayout *hLayout1;

    QGridLayout *mainLayout;
    QGroupBox *mainGroupBox;
    QHBoxLayout *hLayout;
};


//截图显示窗口区域
class CutAllImageShowWnd: public QWidget
{
    Q_OBJECT
public:
    CutAllImageShowWnd(QWidget* parent = NULL);
public:

    void createControls();
    void controlsLayout();


public slots:
     void slots_save_All_little_images();   //save all little images
     void slots_show_cutAllImage(QImage image);

public:
    ClitImgListWidget *imageWidget;
    //QTabWidget * imageTab;
    QPushButton * saveButton;
    QLabel *imageSizeName;
    QLabel *imageSizeLabel;
    QLabel *saveImageLabel;
    QLabel *imageWidthName;
    QLabel *imageHeightName;
    QSpinBox *imageWidth;
    QSpinBox *imageHeight;
    QHBoxLayout * hlayout;
    QGridLayout * layout;
    QGroupBox *saveBox;
    QVBoxLayout * vlayout;
//    QGroupBox *saveBox1;
//    QVBoxLayout *vlayout1;
    QGroupBox *groupBox1;
};


//2/8  目标检测选择 窗口区
class ObjectDetectResultParas
{
public:
    Rect rect;          // 存储的矩阵
    Point2f massCenter;     //< the rectangle mass center
    Size2f width_Height_size;        //< width and height of the rectangle

    double rectRotate;   // 矩阵旋转的角度
    double widthDivHeightRadio;  // 感兴趣单元中 宽高比值
    int countourPixCount;        // 感兴趣单元中 边框所占 像素的个数
    double countourArcLength;    // 感兴趣单元中 边框的 长度  sqrt(dx.^2 + dy.^2)
    double contourArea;          // 感兴趣单元中 边框的 面积
    bool isChecked;      // 是否 选中
};


class ObjectDetectOptionWidget: public QWidget
{
    Q_OBJECT
public:
    ObjectDetectOptionWidget(QWidget* parent = NULL);
    ~ObjectDetectOptionWidget();

    void initTableWidget();
    void initTableWidgetItem();

private:
    void saveObjectDetectResultParas();     // combox 第一种情况,保存参数
    void saveObjectDetectResultImages();    // combox 第二种情况,保存相对应的 图像

public slots:
    void slot_checkedAllItem(bool isChecked);    //设置 将 tableWidget 进行全选操作
    void slot_receive_ObjectDetectResultParas_from_QImageWidget(QVector<ObjectDetectResultParas> objectDetectResultVec);
    void slot_receive_ObjectDetectResultParas_afterDeleteFalseObj_from_QImageWidget(QVector<ObjectDetectResultParas> objectDetectResultVec
                                                                                    ,QVector<int> objNumber_afterDeleteFalseObject);
    void slot_receive_ObjectDetectResultParas_afterPrecise_from_QImageWidget(QVector<ObjectDetectResultParas> objectDetectResultVec
                                                                             ,QVector<int> objNumber_afterPrecise);


    void slot_saveButton_clicked();              // 保存按钮
    void slot_switchButton_clicked();              // 选择目标窗口分析



public:
    static QVector<ObjectDetectResultParas> getObjectDetectResultParas();
    static QVector<ObjectDetectResultParas> getObjectDetectResultParas_afterDeleteFalseObject();
    static QVector<ObjectDetectResultParas> getObjectDetectResultParas_afterPrecise();

    static void setObjectDetectResultParas(QVector<ObjectDetectResultParas> vec);
    static void setObjectDetectResultParas_afterDeleteFalseObject(QVector<ObjectDetectResultParas> vec);
    static void setObjectDetectResultParas_afterPrecise(QVector<ObjectDetectResultParas> vec);

    static QVector<int> getObjNumber_afterDeleteFalseObject();

    static void setObjNumber_afterDeleteFalseObject(QVector<int> vec);
    static void setObjNumber_afterPrecise(QVector<int> vec);


public:
    QTableWidget *tableWidget;
    QGroupBox *mainGroupBox;
    QVBoxLayout *vLayout;

    QComboBox *comboBox;
    QComboBox *comboBox_switch;
    QCheckBox *checkBox;
    QGridLayout *gLayout;

    QLabel *imageWidthName;
    QLabel *imageHeightName;
    QSpinBox *imageWidth;
    QSpinBox *imageHeight;
    QPushButton * saveButton;
    QPushButton * switchButton;


    QGroupBox *downGroupBox;
    QVBoxLayout *mainLayout;

    static QVector<ObjectDetectResultParas> m_objectDetectResultVec;
    static QVector<ObjectDetectResultParas> m_objectDetectResultVec_afterDeleteFalseObject;  //删除形态不符合条件的目标
    static QVector<int> objNumber_afterDeleteFalseObject;  //删除形态不符合条件的目标
    static QVector<ObjectDetectResultParas> m_objectDetectResultVec_afterPrecise;            //svm检测后的目标
    static QVector<int> objNumber_afterPrecise;  //删除形态不符合条件的目标

};


class HogAndSVM_ObjectDetect_settingWidget: public QWidget
{
    Q_OBJECT
public:
    explicit HogAndSVM_ObjectDetect_settingWidget(QWidget* parent = NULL);

    void createControls();

public slots:
    void slot_svmParasSettingButton_clicked();   //svm参数设置
    void slot_positiveInputButton_clicked();
    void slot_negativeInputButton_clicked();
    void slot_svmModelSaveButton_clicked();

    void slot_getHogFeaturesDimButton_clicked();
    void slot_getSvmTrainingModelButton_clicked();
    void slot_getOpticalSvmTrainingModelButton_clicked();

    void slot_modifyImageNameButton_clicked();
    void slot_imageRenameButton_clicked();
    void slot_imageReSizeButton_clicked();
    void slot_rotateImage_clicked();

    void slot_singleImagePathOptionButton_clicked();
    void slot_singleImageResizeButton_clicked();

    //单张图片预测
    void slot_singleImagePredictPathOption();
    void slot_singleImagePredictSvmOption();
    void slot_singleImagePredict();

    //批量图片进行预测
    void slot_batchImagePredictPathOption();
    void slot_batchImagePredicctSvmOption();
    void slot_batchImagePredict();

public:
    //svm 训练窗口
    QPushButton * svmParasSettingButton;

    QLabel * positiveLable;
    QLineEdit* positiveLineEdit;
    QPushButton * positiveInputButton;

    QLabel * negativeLable;
    QLineEdit* negativeLineEdit;
    QPushButton*  negativeInputButton;

    QLabel * svmModelSaveLable;
    QLineEdit* svmModelSaveLineEdit;
    QPushButton*  svmModelSaveButton;

    QPushButton*  getHogFeaturesDimButton;

    QPushButton * getSvmTrainingModelButton;
    QPushButton * getOpticalSvmTrainingModelButton;

    QGroupBox *svmModelGroupBox;

    QGridLayout *svmModelGLayout;

    //批量修改图片文件的 名称
    QGroupBox *modifyImageNameGroupBox;
    QLabel * modifyImageNameLabel;
    QLineEdit* modifyImageNameLineEdit;
    QPushButton*  modifyImageNameButton;
    QGridLayout *modifyImageNameGLayout;

    //重命名
    QLabel *imageRenameLabel;
    QLineEdit* imageRenameLineEdit;
    QPushButton *imageRenameButton;

    //是否批量 修改 图片的尺寸大小
    QCheckBox * imageReSizeCheckBox;
    QLabel * imageReSizeWidthLabel;
    QLineEdit* imageReSizeWidthLineEdit;
    QLabel * imageReSizeHeightLabel;
    QLineEdit* imageReSizeHeightLineEdit;
    QPushButton *imageReSizePushButton;

    QCheckBox * singleimageReSizeCheckBox;
    QLabel * singleimageReSizePathLabel;
    QLineEdit* singleimageReSizePathLineEdit;
    QPushButton *singleimagePathOptionPushButton;
    QPushButton *singleimageReSizePushButton;

    QCheckBox * isRotateImage;
    QPushButton *rotateImage;



    QGroupBox *imageResizeGroupBox;
    QGridLayout *imageResizeGLayout;



    // svm训练 输出信息
    QTextEdit *svmTrainInputInfoEdit;
    QHBoxLayout *svmTrainInputInfoLayout;
    QGroupBox *svmTrainInputInfoGroupBox;

    //单张图片进行预测
    QGroupBox *singleImagePredictGroupBox;
    QLabel * singleImagePredictLabel;
    QLineEdit* singleImagePredictLineEdit;
    QPushButton*  singleImagePredictPathOptionButton;
    QPushButton*  singleImagePredictButton;

    QLabel * singleImagePredictSvmLabel;
    QLineEdit* singleImagePredictSvmLineEdit;
    QPushButton*  singleImagePredictPathOptionSvmButton;
    QGridLayout *singleImagePredictGLayout;



    //批量图片进行预测

    QGroupBox *batchImagePredictGroupBox;
    QLabel * batchImagePredictLabel;
    QLineEdit* batchImagePredictLineEdit;
    QPushButton*  batchImagePredictPathOptionButton;
    QPushButton*  batchImagePredictButton;

    QLabel * batchImagePredictSvmLabel;
    QLineEdit* batchImagePredictSvmLineEdit;
    QPushButton*  batchImagePredictPathOptionSvmButton;
    QGridLayout *batchImagePredictGLayout;


    // 窗口主布局
    QGridLayout *mainLayout;
};


// 利用单列模式生成 单个窗口
class HogAndSVM_ObjectDetect_settingWidget_singleProduct
{
public:
    static HogAndSVM_ObjectDetect_settingWidget* getParameterSettingWidget();
    ~HogAndSVM_ObjectDetect_settingWidget_singleProduct();

private:
    explicit HogAndSVM_ObjectDetect_settingWidget_singleProduct();
    HogAndSVM_ObjectDetect_settingWidget_singleProduct(const HogAndSVM_ObjectDetect_settingWidget_singleProduct&);
    HogAndSVM_ObjectDetect_settingWidget_singleProduct& operator=(const HogAndSVM_ObjectDetect_settingWidget_singleProduct*);

    static HogAndSVM_ObjectDetect_settingWidget* instance;
    static QMutex mutex;
};


#endif // IMAGEVIEW

