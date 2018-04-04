#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_


#include <QMainWindow>
#include <QtWidgets>

#include <QSplitter>
#include "imagePreviewWnds.h"
#include "imageview.h"
#include "navigationbar.h"
#include "imageprocess.h"
#include "imagecomparearea.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    //menu widget definition
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    QMenu *menuFile;
    QMenu *menuProcess;
    QMenu *menuSAR_Image_preProcessing;
    QMenu *menuImage_Filter;
    QMenu *menuImage_Binary;
    QMenu *menuImage_morphology;
    QMenu *menuTarget_Rough_Detection;
    QMenu *menuTarget_Precision_Detection;
    QMenu *menuOption;

    // 添加 DrawContour
    QMenu *menuDrawContour;
    QAction *actionDrawContour;
    QAction *actionDrawContour_afterDeleteFalseObj;
    QAction *actionDrawContour_afterPrecise;




//file operation
    QAction *actionLoad_File;
    QAction *actionSave_File;

//filter operation
    QAction *actionBoxFilter;
    QAction *actionMeanFilter;
    QAction *actionGaussianFilter;
    QAction *actionMedianFilter;
    QAction *actionBilateralFilter;
    QAction *actionBilateralAdaptiveFilter;

//binary operation
    QAction *actionThreshold_Binary;
    QAction *actionThreshold_Binary_Inverse;
    QAction *actionThreshold_trunc;
    QAction *actionThreshold_To_Zero;
    QAction *actionThreshold_To_Zero_Inverse;
    QAction *actionThreshold_Otsu;

//Morph operation
        QAction *actionMorph_Dilate;
        QAction *actionMorph_Erode;
        QAction *actionMorph_Open;
        QAction *actionMorph_Close;
        QAction *actionMorph_Gradient;
        QAction *actionMorph_TopHat;
        QAction *actionMorph_BlackHat;

//detection raw operation
    QAction *actionRuly_CfarDetect;
    QAction *actionGaussian_CfarDetection;
    QAction *actionGaussian_Adaptive_CfarDetection;
    QAction *actionLogNormal_CfarDetectrion;
    QAction *actionLogNormal_Adaptive_CfarDetection;


//detection precision operation
    QAction *actionHog_SVM;
    QAction *actionSVM_Detector;

//evaluation action
        QAction *actionMachine_Learning_Recognition_Evaluation;

//option operation
    QAction *actionDetection_Parameters_Set;
    QAction *actionGetRoI;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionImage_Cut;
    QAction *actionImageDetection_OneKey_Processing;
    QAction *actionProcessing_Exit;
    QAction *actionRefresh;
    QAction *actionAddLittleImage;
    QAction *actionGetSvm;
    QAction *actionLandDetect;

// 添加 布局 显示的控件
    QMenu *menuControls;
    QAction *showNavigationBar;
    QAction *showImageProcessArea;
    QAction *showLittelImageArea;
    QAction *showImageCentralArea;
    QAction *showImageCutArea;
    QAction *showImageDescriptionArea;
    QAction *showObjectDetectOptionArea;
    QAction *showDefaultImageLayout;  //默认的 图像 布局

// 状态栏 内 控件
    QLabel *statusLabel;
    QPushButton *viewPushButton;


public:
    void mouseMoveEvent(QMouseEvent* e);


private:
    void initialization();     //initialize value
    void createMenu();         //create Menu
    void createToolBar();      //create ToolBar
    void createStatusBar();    //create StatusBar
    void createControls();     //create controls
    void bindSlots();

signals:
    void signals_loadImageFile();
    void signals_saveImageFile();
    void signals_zoomin();
    void signals_zoomout();
    void signals_imageCut();
    void signals_oneKeyProcessing();
    void signals_processingExit();
    void signals_imageRefresh();
    void signals_addLittleImage();
    void signals_imageDescriptionParas();

private slots:

    void slot_parameterSetting();     //parameterSetting
    void slot_loadImageFile();
    void slot_saveImageFile();
    void slot_zoomin();
    void slot_zoomout();
    void slot_imageCut();
    void slot_oneKeyProcessing();
    void slot_processingExit();
    void slot_imageRefresh();
    void slot_addLittleImage();
    void slot_getSvm();
    void slot_land_detect();
    void slot_getROI();
    void slot_drawContour();
    void slot_drawContour_afterDeleteFalseObj();
    void slot_drawContour_afterPrecise();


    // 窗口布局 控件显示
    void slot_showNavigationBar();
    void slot_showImageProcessArea();
    void slot_showLittelImageArea();
    void slot_showImageCentralArea();
    void slot_showImageCutArea();
    void slot_showImageDescriptionArea();
    void slot_showObjectDetectOptionArea();
    void slot_showDefaultImageLayout();  //默认的 图像 布局


    void slot_ImageFilterProcess_pushbutton();
    void slot_ImageBinaryProcess_pushbutton();
    void slot_ImageMorphProcess_pushbutton();
    void slot_ImageCfarDetectProcess_pushbutton();
    void slot_ImageHogProcess_pushbutton();




private:
    QSplitter * m_pHSplitter;            //水平分裂器
    QSplitter * m_pHSplitter_cutImage;            //水平分裂器
    QSplitter * m_pVSplitter;            //垂直分裂器

    QTabWidget *m_pImageProcessTabWnd;     //Image process Tab
    QTabWidget *m_pLittleImageTabWnd;        //little image
    QTabWidget *m_pCentralTabWnd;        //中央显示区
    ObjectDetectOptionWidget *m_pObjectDetectOptionWidget;  // 待检测目标 选择窗口
    QDockWidget *ObjectDetectOptionDock;                    // 待检测目标 加入 dock窗口

    QTabWidget *m_pCutImageshowWnd;
    CutImageShowWnd* m_pCutImageShow;    // 截图显示区域
    CutAllImageShowWnd* m_pCutAllImageShow; //cut all Image


    ImageProcessWidget *m_pImageProcessWnd;				  //Image Process
    ClitImgListWidget *m_pLitImgListWnd;				  //图像缩略图窗口
    CLogInfoWidget *m_pLogInfoWnd;						  //打印信息显示窗口
    QImageWidget * m_pImageWidget;                        //显示图片的窗口
    QImageCompareWidget * m_pImageCompareWidget;          //图片对比窗口

    ImageDescriptionWidget *m_pImageDescriptionWidget;    //图片描述区域

    NavigationBar * navigationBar;


};

#endif // __MAINWINDOW_H_













