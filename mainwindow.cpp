#include "mainwindow.h"
#include "parametersetting.h"
#include "imagePreviewWnds.h"
#include "imageview.h"
#include "global.h"
#include <QtCore>
#include <QtGui>
#include <QtWidgets/QMainWindow>
#include <QPoint>
#include <QDebug>
#include <QMouseEvent>
#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    {
        this->setWindowTitle(QStringLiteral("星载SAR图像舰船目标检测系统"));
        this->setMinimumSize(1350, 850);
        this->setMouseTracking(true);
        //this->setWindowState(Qt::WindowFullScreen);
        this->initialization();
        this->createMenu();    //create all menu and actions
        this->createToolBar();
        this->createStatusBar();
        this->createControls();        //create main frame

        this->slot_showNavigationBar();
        this->slot_showImageProcessArea();
        this->slot_showLittelImageArea();
        this->slot_showImageCentralArea();
        this->slot_showImageCutArea();
        this->slot_showImageDescriptionArea();
        this->slot_showObjectDetectOptionArea();


        // 调用bindSlots函数
        this->bindSlots();

    }
}

MainWindow::~MainWindow()
{
    //调用析构函数的时候，需要注意空间析构的顺序，从里向外面一层一层的析构

    DELETE_OBJ(m_pImageProcessWnd);
    DELETE_OBJ(m_pLitImgListWnd);
    DELETE_OBJ(m_pLogInfoWnd);

    DELETE_OBJ(m_pImageWidget);
    DELETE_OBJ(m_pImageCompareWidget);

    DELETE_OBJ(m_pImageProcessTabWnd);
    DELETE_OBJ(m_pLittleImageTabWnd);
    DELETE_OBJ(m_pCentralTabWnd);

    DELETE_OBJ(m_pCutImageShow);
    DELETE_OBJ(m_pCutAllImageShow);
    DELETE_OBJ(navigationBar);

    DELETE_OBJ(m_pVSplitter);
    DELETE_OBJ(m_pHSplitter);


}

void MainWindow::initialization()
{
    m_pHSplitter = NULL;          //水平分裂器
    m_pHSplitter_cutImage = NULL;
    m_pVSplitter = NULL;           //垂直分裂器

    m_pImageProcessWnd = NULL;    //图像信息
    m_pLittleImageTabWnd = NULL;       //
    m_pCentralTabWnd = NULL;       //中央显示区

    m_pImageProcessWnd = NULL;			  //图片名称列表
    m_pLitImgListWnd = NULL;			  //图像缩略图窗口
    m_pLogInfoWnd = NULL;
    m_pImageWidget = NULL;

    m_pCutImageShow = NULL;        //截图显示区域
    m_pCutAllImageShow = NULL;

    navigationBar = NULL;          //dao hang

    m_pImageCompareWidget = NULL;

}

void MainWindow::createMenu()     //create Menu
{
    menuBar = new QMenuBar(this);
    this->setMenuBar(menuBar);    //解决了菜单栏的显示问题了


    if (NULL != menuBar)
    {
        //add all menu
        menuFile = menuBar->addMenu(STR_FILE);
        menuProcess = menuBar->addMenu(STR_PROCESS);
        menuSAR_Image_preProcessing = menuProcess->addMenu(STR_SAR_IMAGE_PREPROCESS);
        menuImage_Filter = menuSAR_Image_preProcessing->addMenu(STR_FILTER);
        menuImage_Binary = menuSAR_Image_preProcessing->addMenu(STR_BINARY);
        menuImage_morphology = menuSAR_Image_preProcessing->addMenu(STR_MORPH);
        menuTarget_Rough_Detection = menuProcess->addMenu(STR_TARGET_ROUGH_DETECTION);
        menuTarget_Precision_Detection = menuProcess->addMenu(STR_TARGET_PRECISION_DETECTION);
        menuOption = menuBar->addMenu(STR_OPTION);
        menuControls = menuBar->addMenu(STR_SHOWCONTROLS);

        //添加 DrawContour
        menuDrawContour = menuOption->addMenu(QStringLiteral("DrawContour"));



        //add all actions

        //file operation
        actionLoad_File = menuFile->addAction(QIcon(PATH_LOADIMAGE), STR_LOAD);
        actionLoad_File->setShortcut(QKeySequence::Open);
        actionLoad_File->setStatusTip(QStringLiteral("加载图片文件"));

        actionSave_File = menuFile->addAction(QIcon(PATH_SAVEIMAGE), STR_SAVE);
        actionSave_File->setShortcut(QKeySequence::Save);
        actionSave_File->setStatusTip(QStringLiteral("保存图片文件"));

        //filter operation
        actionBoxFilter = menuImage_Filter->addAction(STR_BOX_FILTER);
        actionMeanFilter = menuImage_Filter->addAction(STR_MEAN_FILTER);
        actionGaussianFilter = menuImage_Filter->addAction(STR_GAUSSIAN_FILTER);
        actionMedianFilter = menuImage_Filter->addAction(STR_MEDIAN_FILTER);
        actionBilateralFilter = menuImage_Filter->addAction(STR_BILATERAL_FILTER);
        actionBilateralAdaptiveFilter = menuImage_Filter->addAction(STR_BILATERAL_ADAPTIVE_FILTER);

        //binary operation
        actionThreshold_Binary = menuImage_Binary->addAction(STR_BINARY_THRESHOLD);
        actionThreshold_Binary_Inverse = menuImage_Binary->addAction(STR_BINARY_INVERSE);
        actionThreshold_trunc = menuImage_Binary->addAction(STR_BINARY_TRUNC);
        actionThreshold_To_Zero = menuImage_Binary->addAction(STR_BINARY_TO_ZERO);
        actionThreshold_To_Zero_Inverse = menuImage_Binary->addAction(STR_BINARY_TO_ZERO_INVERSE);
        actionThreshold_Otsu = menuImage_Binary->addAction(STR_BINARY_OTSU);

        //Morph operation
        actionMorph_Dilate = menuImage_morphology->addAction(STR_MORPH_DILATE);
        actionMorph_Erode = menuImage_morphology->addAction(STR_MORPH_ERODE);
        actionMorph_Open = menuImage_morphology->addAction(STR_MORPH_OPEN);
        actionMorph_Close = menuImage_morphology->addAction(STR_MORPH_CLOSE);
        actionMorph_Gradient = menuImage_morphology->addAction(STR_MORPH_GRADIENT);
        actionMorph_TopHat = menuImage_morphology->addAction(STR_MORPH_TOPHAT);
        actionMorph_BlackHat = menuImage_morphology->addAction(STR_MORPH_BLACKHAT);

        //detection raw operation
        actionRuly_CfarDetect = menuTarget_Rough_Detection->addAction(STR_CFAR_RULY);
        actionGaussian_CfarDetection = menuTarget_Rough_Detection->addAction(STR_CFAR_GAUSSIAN);
        actionGaussian_Adaptive_CfarDetection = menuTarget_Rough_Detection->addAction(STR_CFAR_GAUSSIAN_ADA);
        actionLogNormal_CfarDetectrion = menuTarget_Rough_Detection->addAction(STR_CFAR_LOGNORMAL);
        actionLogNormal_Adaptive_CfarDetection = menuTarget_Rough_Detection->addAction(STR_CFAR_LOGNORMAL_ADA);


        //detection precision operation
        actionHog_SVM = menuTarget_Precision_Detection->addAction(STR_SVM_HOG);
        actionSVM_Detector = menuTarget_Precision_Detection->addAction(STR_SVM_DETECTOR);

        //evaluation action
        actionMachine_Learning_Recognition_Evaluation = menuProcess->addAction(STR_MACHINE_EVALUATION);

        //option operation
        actionDetection_Parameters_Set = menuOption->addAction(QIcon(PATH_SETTINGICON), STR_PARAMETER_SET);
        actionDetection_Parameters_Set->setStatusTip(QStringLiteral("设置参数"));


        actionZoomIn = menuOption->addAction(QIcon(PATH_ZOOMIN), STR_ZOOMIN);
        actionZoomIn->setShortcut(QKeySequence::ZoomIn);
        actionZoomIn->setStatusTip(QStringLiteral("放大图片"));

        actionZoomOut = menuOption->addAction(QIcon(PATH_ZOOMOUT), STR_ZOOMOUT);
        actionZoomOut->setShortcut(QKeySequence::ZoomOut);
        actionZoomOut->setStatusTip(QStringLiteral("缩小图片"));

        actionGetRoI = menuOption->addAction(QIcon(PATH_GET_ROI), STR_GETROT);
        actionGetRoI->setStatusTip(QStringLiteral("获取ROI目标"));

        actionDrawContour = menuDrawContour->addAction(QStringLiteral("DrawContour_RawDetect"));
        actionDrawContour->setStatusTip(QStringLiteral("获取目标边框"));

        actionDrawContour_afterDeleteFalseObj = menuDrawContour->addAction(QStringLiteral("DrawContour_afterDeleteFalseObj"));
        actionDrawContour_afterDeleteFalseObj->setStatusTip(QStringLiteral("获取目标边框"));

        actionDrawContour_afterPrecise = menuDrawContour->addAction(QStringLiteral("DrawContour_afterPrecise"));
        actionDrawContour_afterPrecise->setStatusTip(QStringLiteral("获取目标边框"));

        actionGetSvm = menuOption->addAction(QIcon(PATH_GET_SVM), STR_GETSVM);
        actionGetSvm->setStatusTip(QStringLiteral("获取SVM训练窗口"));

        actionLandDetect = menuOption->addAction(QIcon(PATH_LAND_DELETE), STR_LAND_DETECT);
        actionLandDetect->setStatusTip(QStringLiteral("获取陆地轮廓"));
        actionImage_Cut = menuOption->addAction(QIcon(PATH_CUTICON), STR_IMAGE_CUT);
        actionImage_Cut->setShortcut(QKeySequence::Cut);
        actionImage_Cut->setStatusTip(QStringLiteral("截图"));


        actionImageDetection_OneKey_Processing = menuOption->addAction(QIcon(PATH_PLAY), STR_ONEKEY_PROCESSING);
        actionProcessing_Exit = menuOption->addAction(QIcon(PATH_STOP), STR_PROCESS_EXIT_ABORT);        
        actionProcessing_Exit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
        actionProcessing_Exit->setStatusTip(QStringLiteral("终止操作"));


        actionRefresh = menuOption->addAction(QIcon(PATH_REFRESH), STR_IMAGE_REFRESH);
        actionRefresh->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
        actionRefresh->setStatusTip(QStringLiteral("刷新图片"));



        actionAddLittleImage = menuOption->addAction(QIcon(PATH_ADDLITTLEIMAGE), STR_ADD_LITTLE_IMAGE);
        actionAddLittleImage->setShortcut(QKeySequence::Copy);
        actionAddLittleImage->setStatusTip(QStringLiteral("放置缩略图"));


        // add controls layout option

        showNavigationBar = menuControls->addAction(STR_SHOW_NAVIGATION_BAR);
        showNavigationBar->setCheckable(true);
        showNavigationBar->setChecked(true);

        showImageProcessArea = menuControls->addAction(STR_SHOW_IMAGE_PROCESS_AREA);
        showImageProcessArea->setCheckable(true);
        showImageProcessArea->setChecked(true);

        showLittelImageArea = menuControls->addAction(STR_SHOW_THUMBNAIL_AREA);
        showLittelImageArea->setCheckable(true);
        showLittelImageArea->setChecked(true);

        showImageCentralArea = menuControls->addAction(STR_SHOW_CENTRAL_IMAGE_AREA);
        showImageCentralArea->setCheckable(true);
        showImageCentralArea->setChecked(true);

        showImageCutArea = menuControls->addAction(STR_SHOW_CUT_IMAGE_AREA);
        showImageCutArea->setCheckable(true);
        showImageCutArea->setChecked(false);

        showImageDescriptionArea = menuControls->addAction(STR_SHOW_IMAGE_DESCRIPTION_AREA);
        showImageDescriptionArea->setCheckable(true);
        showImageDescriptionArea->setChecked(false);

        showObjectDetectOptionArea = menuControls->addAction(QStringLiteral("Object Detect Option Area"));
        showObjectDetectOptionArea->setCheckable(true);
        showObjectDetectOptionArea->setChecked(false);


        showDefaultImageLayout = menuControls->addAction(STR_SHOW_DEFAULT_IMAGE_LAYOUT);  //显示默认的图像布局
        showDefaultImageLayout->setCheckable(true);
        showDefaultImageLayout->setChecked(false);
    }
}

void MainWindow::createToolBar()     //create Menu
{
    mainToolBar = addToolBar("ToolBar");
    if (NULL != mainToolBar)
    {
        mainToolBar->addAction(actionLoad_File);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionSave_File);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionDetection_Parameters_Set);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionZoomIn);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionZoomOut);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionImage_Cut);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionImageDetection_OneKey_Processing);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionProcessing_Exit);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionRefresh);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionAddLittleImage);

        mainToolBar->addSeparator();
        mainToolBar->addAction(actionLandDetect);

        mainToolBar->addSeparator();
        mainToolBar->addAction(actionGetRoI);

        mainToolBar->addSeparator();
        mainToolBar->addAction(actionGetSvm);

    }
}



void MainWindow::createStatusBar()    //create StatusBar
{
    statusBar = new QStatusBar(this);
    this->setStatusBar(statusBar);    //解决了状态栏的显示问题了

    statusLabel = new QLabel(statusBar);
    statusLabel->setText((QStringLiteral("就绪")));
    QFont font ("Microsoft YaHei", 10, 20);   //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
    statusLabel->setFont(font);

    viewPushButton = new QPushButton(statusBar);
    viewPushButton->setText(QStringLiteral("视图"));
    viewPushButton->setMenu(menuControls);
    viewPushButton->setFont(font);

    statusBar->addWidget(statusLabel);
    statusBar->addPermanentWidget(viewPushButton);
}


void MainWindow::createControls()
{
    m_pHSplitter = new QSplitter(Qt::Horizontal, this);
    m_pHSplitter_cutImage = new QSplitter(Qt::Horizontal, this);
    m_pVSplitter = new QSplitter(Qt::Vertical, this);
    QSplitter *vSplitter = new QSplitter(Qt::Vertical, this);

    m_pImageProcessTabWnd = new QTabWidget();     //image process
    m_pLittleImageTabWnd = new QTabWidget();        //little image
    m_pCentralTabWnd = new QTabWidget();        //中央显示区
    m_pCutImageshowWnd = new QTabWidget();


    m_pCutImageShow = new CutImageShowWnd();    //截图显示区域 1/25 add
    m_pCutAllImageShow = new CutAllImageShowWnd();

    m_pImageDescriptionWidget = new ImageDescriptionWidget; //2/6 add


    m_pImageProcessWnd = new ImageProcessWidget();  //图片名称列表  listWidget
    m_pLitImgListWnd = new ClitImgListWidget();	   //图像缩略图窗口 listWidget
    m_pLogInfoWnd = new CLogInfoWidget();		   //打印信息显示窗口

    m_pImageWidget = new QImageWidget();           //主图像区 创建一个图像操作的窗口
    m_pImageCompareWidget = new QImageCompareWidget();  //图片对比窗口

    //测试 滚动条
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);  //背景色
    scrollArea->setWidget(m_pImageWidget);
    scrollArea->setAlignment(Qt::AlignCenter);
    //scrollArea->setAlignment(Qt::AlignLeft);
    scrollArea->setWidgetResizable(true);  // 自动调整大小
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->verticalScrollBar()->setValue((scrollArea->verticalScrollBar()->maximum()
                                              - scrollArea->verticalScrollBar()->minimum())/2 + scrollArea->verticalScrollBar()->minimum());
    scrollArea->horizontalScrollBar()->setValue((scrollArea->horizontalScrollBar()->maximum()
                                              - scrollArea->horizontalScrollBar()->minimum())/2 + scrollArea->horizontalScrollBar()->minimum());


    m_pImageProcessTabWnd->addTab(m_pImageProcessWnd, QStringLiteral("舰船检测流程区"));
    m_pLittleImageTabWnd->addTab(m_pLitImgListWnd, QStringLiteral("舰船检测中间结果保存区"));

    m_pCentralTabWnd->addTab(scrollArea, QStringLiteral("视图区: "));
    m_pCentralTabWnd->addTab(m_pImageCompareWidget, QStringLiteral("图像对比区:"));



    m_pCutImageshowWnd->addTab(m_pCutImageShow, QStringLiteral("截图窗口1"));
    m_pCutImageshowWnd->addTab(m_pCutAllImageShow, QStringLiteral("截图窗口2"));
    navigationBar = new NavigationBar;

    //分裂器布局
    m_pVSplitter->addWidget(m_pImageProcessTabWnd);
    m_pVSplitter->addWidget(m_pLittleImageTabWnd);

    m_pObjectDetectOptionWidget = new ObjectDetectOptionWidget;
// 将目标检测窗口 放入到dock窗口中
    ObjectDetectOptionDock = new QDockWidget();
    ObjectDetectOptionDock->setFeatures(QDockWidget::AllDockWidgetFeatures);  //设置移动，关闭，悬浮
    ObjectDetectOptionDock->setAllowedAreas(Qt::AllDockWidgetAreas);         //设置可移动的 区域

    ObjectDetectOptionDock->setWidget(m_pObjectDetectOptionWidget);
    addDockWidget(Qt::BottomDockWidgetArea, ObjectDetectOptionDock);




    QSplitter *m_pVSplitterCentral = new QSplitter(Qt::Vertical, this);
    m_pVSplitterCentral->addWidget(m_pCentralTabWnd);
    //m_pVSplitterCentral->addWidget(m_pObjectDetectOptionWidget);

    m_pHSplitter_cutImage->addWidget(m_pVSplitterCentral);


    vSplitter->addWidget(m_pImageDescriptionWidget);
    vSplitter->addWidget(m_pCutImageshowWnd);
    m_pHSplitter_cutImage->addWidget(vSplitter);


    m_pHSplitter->addWidget(navigationBar);
    m_pHSplitter->addWidget(m_pVSplitter);
    m_pHSplitter->addWidget(m_pHSplitter_cutImage);


    m_pHSplitter->setHandleWidth(3);   //设置手柄的 宽度
    m_pVSplitter->setHandleWidth(3);    //设置手柄的 宽度
    m_pHSplitter_cutImage->setHandleWidth(3);


//    m_pVSplitter->setStretchFactor(0, 0);
//    m_pVSplitter->setStretchFactor(1, 0);
    //m_pHSplitter->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
   // m_pHSplitter->setStretchFactor(0, 3);
    m_pHSplitter_cutImage->setStretchFactor(0, 2);
    m_pHSplitter->setStretchFactor(2, 2);



    setCentralWidget(m_pHSplitter);
}

//链接所有的  信号与槽  函数
void MainWindow::bindSlots()
{
    connect(actionDetection_Parameters_Set, SIGNAL(triggered()),
            this, SLOT(slot_parameterSetting()));   //参数设置按钮 实现

    connect(actionLoad_File, SIGNAL(triggered()),
            this, SLOT(slot_loadImageFile()));      //加载文件按钮 实现

    connect(actionSave_File, SIGNAL(triggered()),
            this, SLOT(slot_saveImageFile()));

    connect(actionZoomIn, SIGNAL(triggered()),
            this, SLOT(slot_zoomin()));

    connect(actionZoomOut, SIGNAL(triggered()),
            this, SLOT(slot_zoomout()));

//    connect(actionGetRoI, SIGNAL(triggered()),
//            this, SLOT(slot_getROI()));    // getRoi action

    connect(actionImage_Cut, SIGNAL(triggered()),
            this, SLOT(slot_imageCut()));

    connect(actionImageDetection_OneKey_Processing, SIGNAL(triggered()),
            this, SLOT(slot_oneKeyProcessing()));

    connect(actionProcessing_Exit, SIGNAL(triggered()),
            this, SLOT(slot_processingExit()));

    connect(actionRefresh, SIGNAL(triggered()),
            this, SLOT(slot_imageRefresh()));

    connect(actionAddLittleImage, SIGNAL(triggered()),
            this, SLOT(slot_addLittleImage()));

    connect(actionGetSvm, SIGNAL(triggered()),
            this, SLOT(slot_getSvm()));

    // add controls layout option    start
    connect(showNavigationBar, SIGNAL(triggered()),
            this, SLOT(slot_showNavigationBar()));

    connect(showImageProcessArea, SIGNAL(triggered()),
            this, SLOT(slot_showImageProcessArea()));

    connect(showLittelImageArea, SIGNAL(triggered()),
            this, SLOT(slot_showLittelImageArea()));

    connect(showImageCentralArea, SIGNAL(triggered()),
            this, SLOT(slot_showImageCentralArea()));

    connect(showImageCutArea, SIGNAL(triggered()),
            this, SLOT(slot_showImageCutArea()));

    connect(showImageDescriptionArea, SIGNAL(triggered()),
            this, SLOT(slot_showImageDescriptionArea()));
    connect(showObjectDetectOptionArea, SIGNAL(triggered()),
            this, SLOT(slot_showObjectDetectOptionArea()));

    connect(showDefaultImageLayout, SIGNAL(triggered()),
            this, SLOT(slot_showDefaultImageLayout()));

    // add controls layout option    end

    //发送截图 图片 到指定的窗口   cutImage 和 cutAllImage 当中
    connect(m_pImageWidget, SIGNAL(signals_deliver_captureQImage(QImage)),
            m_pCutImageShow, SLOT(slot_show_cutImage(QImage)));
    connect(m_pImageWidget, SIGNAL(signals_deliver_captureQImage(QImage)),
            m_pCutAllImageShow, SLOT(slots_show_cutAllImage(QImage)));  //显示所有的截图 图片



    connect(m_pImageWidget, SIGNAL(signals_deliver_little_QPixMap(QImage)),
            m_pLitImgListWnd, SLOT(slot_addQImage(QImage)));

    connect(m_pLitImgListWnd, SIGNAL(signals_listItemDoubleClicked(QImage)),
            m_pImageWidget, SLOT(slot_listItemDoubleClicked(QImage)));

    //daohang
    connect(navigationBar, SIGNAL(signals_button_clicked(int)),
            m_pImageProcessWnd, SLOT(slot_switch_different_widget(int)));

    //image compare area
    connect(m_pLitImgListWnd, SIGNAL(signals_Load_iamge_compare_left_area(QImage)),
            m_pImageCompareWidget, SLOT(slots_Load_iamge_compare_left_area(QImage)));
    connect(m_pLitImgListWnd, SIGNAL(signals_Load_iamge_compare_right_area(QImage)),
            m_pImageCompareWidget, SLOT(slots_Load_iamge_compare_right_area(QImage)));

    connect(m_pCutAllImageShow->imageWidget, SIGNAL(signals_Load_iamge_compare_left_area(QImage)),
            m_pImageCompareWidget, SLOT(slots_Load_iamge_compare_left_area(QImage)));
    connect(m_pCutAllImageShow->imageWidget, SIGNAL(signals_Load_iamge_compare_right_area(QImage)),
            m_pImageCompareWidget, SLOT(slots_Load_iamge_compare_right_area(QImage)));

    //图像 滤波操作 实现
    connect(m_pImageProcessWnd->widget0->pushbutton, SIGNAL(clicked()),
            this, SLOT(slot_ImageFilterProcess_pushbutton()));

    connect(m_pImageWidget, SIGNAL(signals_deliver_curQImage_to_filter(QImage)),
            m_pImageProcessWnd, SLOT(slots_getImage_and_ImageFilterProcess(QImage)));

    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_image_filter_result_to_central_area(QImage)),
            m_pImageWidget, SLOT(slot_receive_image_process_result_to_central_area(QImage)));

    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_little_image_filter(QImage,QString)),
            m_pLitImgListWnd, SLOT(slot_receive_little_image(QImage,QString)));

    // 图像 二值化操作
    connect(m_pImageProcessWnd->widget1->pushbutton, SIGNAL(clicked()),
            this, SLOT(slot_ImageBinaryProcess_pushbutton()));

    connect(m_pImageWidget, SIGNAL(signals_deliver_curQImage_to_binary(QImage)),
            m_pImageProcessWnd, SLOT(slots_getImage_and_imageBinaryProcess(QImage)));

    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_image_binary_result_to_central_area(QImage)),
            m_pImageWidget, SLOT(slot_receive_image_process_result_to_central_area(QImage)));

    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_little_image_binary(QImage,QString)),
            m_pLitImgListWnd, SLOT(slot_receive_little_image(QImage,QString)));

    // 图像 形态学操作
    connect(m_pImageProcessWnd->widget2->pushbutton, SIGNAL(clicked()),
            this, SLOT(slot_ImageMorphProcess_pushbutton()));

    connect(m_pImageWidget, SIGNAL(signals_deliver_curQImage_to_morph(QImage)),
            m_pImageProcessWnd, SLOT(slots_getImage_and_imageMorphProcess(QImage)));

    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_image_morph_result_to_central_area(QImage)),
            m_pImageWidget, SLOT(slot_receive_image_process_result_to_central_area(QImage)));

    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_little_image_morph(QImage,QString)),
            m_pLitImgListWnd, SLOT(slot_receive_little_image(QImage,QString)));

    // 图像 CFAR检测操作
    connect(m_pImageProcessWnd->widget3->pushbutton, SIGNAL(clicked()),
            this, SLOT(slot_ImageCfarDetectProcess_pushbutton()));

    connect(m_pImageWidget, SIGNAL(signals_deliver_curQImage_to_cfarDetect(QImage)),
            m_pImageProcessWnd, SLOT(slots_getImage_and_imageCfarDetectProcess(QImage)));

    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_image_cfar_detect_result_to_central_area(QImage)),
            m_pImageWidget, SLOT(slot_receive_image_process_result_to_central_area(QImage)));

    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_little_image_cfarDetect(QImage,QString)),
            m_pLitImgListWnd, SLOT(slot_receive_little_image(QImage,QString)));

    // 获取hog特征 SVM检测操作
    connect(m_pImageProcessWnd->widget4->pushbutton, SIGNAL(clicked()),
            this, SLOT(slot_ImageHogProcess_pushbutton()));

    connect(m_pImageWidget, SIGNAL(signals_deliver_curQImage_to_hog_svm(QImage)),
            m_pImageProcessWnd, SLOT(slots_getImage_and_imageHogAndSvmProcess(QImage)));

    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_image_hog_svm_result_to_central_area(QImage)),
            m_pImageWidget, SLOT(slot_receive_image_process_result_to_central_area(QImage)));
    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_little_image_HogAndSvm(QImage,QString)),
            m_pLitImgListWnd, SLOT(slot_receive_little_image(QImage,QString)));


    // 获取检测图像后 的检测目标 窗口
    connect(actionGetRoI, SIGNAL(triggered()),
            this, SLOT(slot_getROI()));

    connect(m_pImageWidget, SIGNAL(signals_deliver_curQImage_to_getRoi(QImage)),
            m_pImageProcessWnd, SLOT(slots_getImage_and_imageGetRoiProcess(QImage)));

    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_image_getRoi_result_to_central_area(QImage)),
            m_pImageWidget, SLOT(slot_receive_image_process_result_to_central_area(QImage)));  //发送检测图像到 中央窗口
    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_little_image_GetRoi(QImage,QString)),
            m_pLitImgListWnd, SLOT(slot_receive_little_image(QImage,QString)));  //发送检测图像到 小图窗口

    // 获取目标边框 窗口

    connect(actionDrawContour, SIGNAL(triggered()),
            this, SLOT(slot_drawContour()));

    connect(m_pImageWidget, SIGNAL(signals_deliver_curQImage_to_drawContour(QImage)),
            m_pImageProcessWnd, SLOT(slots_getImage_and_imageDrawContourProcess(QImage)));

    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_image_drawContour_result_to_central_area(QImage)),
            m_pImageWidget, SLOT(slot_receive_image_process_result_to_central_area(QImage)));  //发送检测图像到 中央窗口
    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_little_image_DrawContour(QImage,QString)),
            m_pLitImgListWnd, SLOT(slot_receive_little_image(QImage,QString)));  //发送检测图像到 小图窗口

      // 获取目标边框 删除 不符合条件的 目标 窗口
    connect(actionDrawContour_afterDeleteFalseObj, SIGNAL(triggered()),
            this, SLOT(slot_drawContour_afterDeleteFalseObj()));
    connect(m_pImageWidget, SIGNAL(signals_deliver_curQImage_to_drawContour_afterDeleteFalseObj(QImage)),
            m_pImageProcessWnd, SLOT(slots_getImage_and_imageDrawContour_AfterDeleteFalseObjProcess(QImage)));

    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_image_drawContour_result_to_central_area(QImage)),
            m_pImageWidget, SLOT(slot_receive_image_process_result_to_central_area(QImage)));  //发送检测图像到 中央窗口
    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_little_image_DrawContour(QImage,QString)),
            m_pLitImgListWnd, SLOT(slot_receive_little_image(QImage,QString)));  //发送检测图像到 小图窗口



      // 获取目标边框 精确检测后的窗口
    connect(actionDrawContour_afterPrecise, SIGNAL(triggered()),
            this, SLOT(slot_drawContour_afterPrecise()));
    connect(m_pImageWidget, SIGNAL(signals_deliver_curQImage_to_drawContour_afterPrecise(QImage)),
            m_pImageProcessWnd, SLOT(slots_getImage_and_imageDrawContour_AfterPreciseProcess(QImage)));

    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_image_drawContour_result_to_central_area(QImage)),
            m_pImageWidget, SLOT(slot_receive_image_process_result_to_central_area(QImage)));  //发送检测图像到 中央窗口
    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_little_image_DrawContour(QImage,QString)),
            m_pLitImgListWnd, SLOT(slot_receive_little_image(QImage,QString)));  //发送检测图像到 小图窗口

    // 目标检测信息 输出窗口
    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_objectDetectOutputInfo(QString)),
            m_pImageDescriptionWidget, SLOT(slot_ObjectDetect_Output_Info(QString)));


    // 陆地剔除
    connect(actionLandDetect, SIGNAL(triggered()),
            this, SLOT(slot_land_detect()));

    connect(m_pImageWidget, SIGNAL(signals_deliver_curQImage_to_landDelete(QImage)),
            m_pImageProcessWnd, SLOT(slots_getImage_and_imageLandDetectProcess(QImage)));

    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_image_landDetect_result_to_central_area(QImage)),
            m_pImageWidget, SLOT(slot_receive_image_process_result_to_central_area(QImage)));  //发送检测图像到 中央窗口
    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_little_image_landDetect(QImage,QString)),
            m_pLitImgListWnd, SLOT(slot_receive_little_image(QImage,QString)));



    connect(m_pImageProcessWnd, SIGNAL(signals_deliver_ObjectDetectResultParas_to_ObjectDetectOptionWidget(QVector<ObjectDetectResultParas>)),
            m_pObjectDetectOptionWidget, SLOT(slot_receive_ObjectDetectResultParas_from_QImageWidget(QVector<ObjectDetectResultParas>)));


    //加载图像的 描述信息
    connect(this, SIGNAL(signals_imageDescriptionParas()),
            m_pImageDescriptionWidget, SLOT(slots_imageDescriptionParas()));
    connect(m_pImageWidget, SIGNAL(signals_imageDescriptionParas()),
            m_pImageDescriptionWidget, SLOT(slots_imageDescriptionParas()));
    connect(m_pImageWidget, SIGNAL(signals_deliver_CurQImage_pixPos_pixValue(QPoint,QVector<uchar>)),
            m_pImageDescriptionWidget, SLOT(slots_receive_CurQImage_pixPos_pixValue(QPoint,QVector<uchar>)));
}


void MainWindow::slot_parameterSetting()  //参数设置菜单栏 实现
{
    ParameterSettingWidget *paraWidget
            = ParameterSettingWidget::getParameterSettingWidget();   //create single widget
    paraWidget->show();
}

void MainWindow::slot_imageCut()   //截图 实现
{
    qDebug() << "MainWindow::slot_imageCut()";
    showImageCutArea->setChecked(true);
    m_pCutImageshowWnd->show();
    m_pImageWidget->slot_imageCut_emit_signals();

}


void MainWindow::slot_zoomin()
{
    qDebug() << "MainWindow::slot_zoomin()";
    m_pImageWidget->slot_zoomin();
}

void MainWindow::slot_zoomout()
{
    qDebug() << "MainWindow::slot_zoomout()";
    m_pImageWidget->slot_zoomout();

}


void MainWindow::slot_loadImageFile()
{

    qDebug() << "MainWindow::slot_loadImageFile()";
    bool isLoad = m_pImageWidget->slot_loadImageFile();
    if (!isLoad)
    {
        return;
    }

    this->slot_addLittleImage();


    // 显示 图像描述窗口
    showImageDescriptionArea->setChecked(true);
    m_pImageDescriptionWidget->show();

    emit signals_imageDescriptionParas();
}


void MainWindow::slot_saveImageFile()
{

    qDebug() << "MainWindow::slot_saveImageFile()";
    m_pImageWidget->slot_saveImageFile();
}


void MainWindow::slot_imageRefresh()
{
    qDebug() << "MainWindow::slot_imageRefresh()";
    m_pImageWidget->slot_imageRefresh();
}


void MainWindow::slot_getROI()      // 获取图像的 ROI
{
    qDebug() << "MainWindow::slot_getROI()";

    statusLabel->setText(QStringLiteral("Busy"));
    statusLabel->repaint();


    m_pImageWidget->slot_curQImage_emit_signals_to_getRoi();

    statusLabel->setText(QStringLiteral("Ready"));
}

void MainWindow::slot_drawContour()
{

    statusLabel->setText(QStringLiteral("Busy"));
    statusLabel->repaint();


    m_pImageWidget->slot_curQImage_emit_signals_to_drawContour();

    statusLabel->setText(QStringLiteral("Ready"));
}


void MainWindow::slot_drawContour_afterDeleteFalseObj()
{
    statusLabel->setText(QStringLiteral("Busy"));
    statusLabel->repaint();


    m_pImageWidget->slot_curQImage_emit_signals_to_drawContour_afterDeleteFalseObj();

    statusLabel->setText(QStringLiteral("Ready"));
}


void MainWindow::slot_drawContour_afterPrecise()
{
    statusLabel->setText(QStringLiteral("Busy"));
    statusLabel->repaint();


    m_pImageWidget->slot_curQImage_emit_signals_to_drawContour_afterPrecise();

    statusLabel->setText(QStringLiteral("Ready"));
}



void MainWindow::slot_land_detect()
{
    statusLabel->setText(QStringLiteral("Busy"));
    statusLabel->repaint();


    m_pImageWidget->slot_curQImage_emit_signals_to_landDelete();

    statusLabel->setText(QStringLiteral("Ready"));
}



void MainWindow::slot_oneKeyProcessing()
{
    qDebug() << "MainWindow::slot_oneKeyProcessing()";

    m_pImageWidget->slot_oneKeyProcessing();
}


void MainWindow::slot_processingExit()
{
    qDebug() << "MainWindow::slot_processingExit()";
    m_pImageWidget->slot_processingExit();
}


void MainWindow::slot_addLittleImage()
{
    qDebug() << " MainWindow::slot_addLittleImage()";
    m_pImageWidget->slot_littleImage_emit_signals();
}


void MainWindow::slot_getSvm()
{
    HogAndSVM_ObjectDetect_settingWidget *imageWidget = HogAndSVM_ObjectDetect_settingWidget_singleProduct::getParameterSettingWidget();
    imageWidget->show();
}




// 窗口布局 控件显示
void MainWindow::slot_showNavigationBar()
{
    qDebug() << "MainWindow::slot_showNavigationBar()";

    bool isShow = showNavigationBar->isChecked();
    if (isShow)
    {
        navigationBar->show();
    }
    else
    {
        navigationBar->hide();
    }
}

void MainWindow::slot_showImageProcessArea()
{
    qDebug() << "MainWindow::slot_showImageProcessArea()";

    bool isShow = showImageProcessArea->isChecked();
    if (isShow)
    {
        m_pImageProcessTabWnd->show();
    }
    else
    {
        m_pImageProcessTabWnd->hide();
    }
}

void MainWindow::slot_showLittelImageArea()
{
    qDebug() << "MainWindow::slot_showLittelImageArea()";

    bool isShow = showLittelImageArea->isChecked();
    if (isShow)
    {
        m_pLittleImageTabWnd->show();
    }
    else
    {
        m_pLittleImageTabWnd->hide();
    }
}

void MainWindow::slot_showImageCentralArea()
{
    qDebug() << "MainWindow::slot_showImageCentralArea()";

    bool isShow = showImageCentralArea->isChecked();
    if (isShow)
    {
        m_pCentralTabWnd->show();
    }
    else
    {
        m_pCentralTabWnd->hide();
    }
}

void MainWindow::slot_showImageCutArea()
{
    qDebug() << "MainWindow::slot_showImageCutArea()";

    bool isShow = showImageCutArea->isChecked();
    if (isShow)
    {
        m_pCutImageshowWnd->show();
    }
    else
    {
        m_pCutImageshowWnd->hide();
    }
}



void MainWindow::slot_showImageDescriptionArea()
{
    qDebug() << "MainWindow::slot_showImageDescriptionArea()";

    bool isShow = showImageDescriptionArea->isChecked();
    if (isShow)
    {
        m_pImageDescriptionWidget->show();
    }
    else
    {
        m_pImageDescriptionWidget->hide();
    }


}

void MainWindow::slot_showObjectDetectOptionArea()
{

    qDebug() << "MainWindow::slot_showObjectDetectOptionArea()";

    bool isShow = showObjectDetectOptionArea->isChecked();
    if (isShow)
    {
        m_pObjectDetectOptionWidget->show();
        ObjectDetectOptionDock->show();

    }
    else
    {
        m_pObjectDetectOptionWidget->hide();
    }

}



void MainWindow::slot_showDefaultImageLayout()  //默认的 图像 布局
{
    qDebug() << "MainWindow::slot_showDefaultImageLayout()";

    bool isShow = showDefaultImageLayout->isChecked();
    if (isShow)
    {
        navigationBar->show();
        showNavigationBar->setChecked(true);

        m_pImageProcessTabWnd->show();
        showImageProcessArea->setChecked(true);

        m_pLittleImageTabWnd->show();
        showLittelImageArea->setChecked(true);

        m_pCentralTabWnd->show();
        showImageCentralArea->setChecked(true);

        m_pCutImageshowWnd->show();
        showImageCutArea->setChecked(true);

        m_pImageDescriptionWidget->show();
        showImageDescriptionArea->setChecked(true);

        m_pObjectDetectOptionWidget->show();
        showObjectDetectOptionArea->setChecked(true);
    }
}


// 点击 开始运行按钮
void MainWindow::slot_ImageFilterProcess_pushbutton()
{
   statusLabel->setText(QStringLiteral("Busy"));
   statusLabel->repaint();

   m_pImageWidget->slot_curQImage_emit_signals_to_filter();
   statusLabel->setText(QStringLiteral("Ready"));
}


void MainWindow::slot_ImageBinaryProcess_pushbutton()
{
    statusLabel->setText(QStringLiteral("Busy"));
    statusLabel->repaint();   //在此处，必须调用repaint函数，label才会更新


    m_pImageWidget->slot_curQImage_emit_signals_to_binary();

    statusLabel->setText(QStringLiteral("Ready"));

}

void MainWindow::slot_ImageMorphProcess_pushbutton()
{
    statusLabel->setText(QStringLiteral("Busy"));
    statusLabel->repaint();


    m_pImageWidget->slot_curQImage_emit_signals_to_morph();

    statusLabel->setText(QStringLiteral("Ready"));


}

void MainWindow::slot_ImageCfarDetectProcess_pushbutton()
{
    statusLabel->setText(QStringLiteral("Busy"));
    statusLabel->repaint();

    m_pImageWidget->slot_curQImage_emit_signals_to_cfarDetect();

    statusLabel->setText(QStringLiteral("Ready"));

}

void MainWindow::slot_ImageHogProcess_pushbutton()
{
    statusLabel->setText(QStringLiteral("Busy"));
    statusLabel->repaint();


    m_pImageWidget->slot_curQImage_emit_signals_to_hog_svm();

    statusLabel->setText(QStringLiteral("Ready"));
}


void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
//    QPoint point = e->pos();
//    qDebug() << point;
//    statusLabel->setText(QString::number(e->pos().x()));
}

