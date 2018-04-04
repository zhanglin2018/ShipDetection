#include "imageprocess.h"
#include <QListWidgetItem>
#include <QIcon>
#include "global.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
using namespace ObjectImageFiter;
using namespace ObjectImageBinary;
using namespace ObjectImageFFT;
using namespace ObjectImageSVM;

class ImageDescriptionParas;
// 定义了 QImage 与 Mat 矩阵相互转换的函数
QImage cvMat2QImage(const cv::Mat mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Grayscale8);
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}



cv::Mat QImage2cvMat(QImage image)   //每次 转换的时候，都是引用的形式
{
    cv::Mat mat;
    cv::Mat dstMat;
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        ImageDescriptionParas::getImageDescriptionParas()->imageChannels = 4;
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        mat.copyTo(dstMat);
        break;
    case QImage::Format_RGB888:
        ImageDescriptionParas::getImageDescriptionParas()->imageChannels = 3;

        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        mat.copyTo(dstMat);
        cv::cvtColor(dstMat, dstMat, CV_BGR2RGB);
        break;
    case QImage::Format_Grayscale8:
    case QImage::Format_Indexed8:
        ImageDescriptionParas::getImageDescriptionParas()->imageChannels = 1;
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        mat.copyTo(dstMat);
        break;
    }
    return dstMat;
}

//图像滤波操作

ImageFilterProcessWidget::ImageFilterProcessWidget(QWidget *parent):QWidget(parent)
{
    init();
}

ImageFilterProcessWidget::~ImageFilterProcessWidget(){}

void ImageFilterProcessWidget::init()						//图像滤波操作 初始化
{
    item0 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN), QStringLiteral("方框滤波"));
    item1 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN), QStringLiteral("均值滤波"));
    item2 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN), QStringLiteral("高斯滤波"));
    item3 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN), QStringLiteral("中值滤波"));
    item4 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN), QStringLiteral("Lee滤波"));
    item5 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN), QStringLiteral("Kuan滤波"));

    listWidget = new QListWidget;
    gridLayout  = new QGridLayout;
    tabWidget = new QTabWidget;

    pushbutton = new QPushButton(QStringLiteral("开始运行"));


    listWidget->addItem(item0);
    listWidget->addItem(item1);
    listWidget->addItem(item2);
    listWidget->addItem(item3);
    listWidget->addItem(item4);
    listWidget->addItem(item5);

    tabWidget->addTab(listWidget, QStringLiteral("图像滤波算法："));


    gridLayout->addWidget(tabWidget, 0, 0);
    gridLayout->addWidget(pushbutton, 1, 0);
    this->setLayout(gridLayout);
}

//图像二值化操作

ImageBinaryProcessWidget::ImageBinaryProcessWidget(QWidget *parent):QWidget(parent)
{
    init();
}

ImageBinaryProcessWidget::~ImageBinaryProcessWidget()
{

}

void ImageBinaryProcessWidget::init()						//初始化
{

    item0 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN1), QStringLiteral("图像二值化--OTSU自适应阈值"));
    item1 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN1), QStringLiteral("图像二值化"));
    item2 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN1), QStringLiteral("图像反二值化"));
    item3 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN1), QStringLiteral("图像二值化--阈值截断"));
    item4 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN1), QStringLiteral("图像二值化--零值截断"));
    item5 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN1), QStringLiteral("图像二值化--反零值截断"));

    listWidget = new QListWidget;
    gridLayout  = new QGridLayout;
    tabWidget = new QTabWidget;

    pushbutton = new QPushButton(QStringLiteral("开始运行"));


    listWidget->addItem(item0);
    listWidget->addItem(item1);
    listWidget->addItem(item2);
    listWidget->addItem(item3);
    listWidget->addItem(item4);
    listWidget->addItem(item5);

    tabWidget->addTab(listWidget, QStringLiteral("图像二值化算法："));


    gridLayout->addWidget(tabWidget, 0, 0);
    gridLayout->addWidget(pushbutton, 1, 0);
    this->setLayout(gridLayout);
}

//图像形态学操作

ImageMorphProcessWidget::ImageMorphProcessWidget(QWidget *parent):QWidget(parent)
{
    init();
}

ImageMorphProcessWidget::~ImageMorphProcessWidget()
{

}

void ImageMorphProcessWidget::init()						//初始化
{
    item0 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN2), QStringLiteral("图像膨胀"));
    item1 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN2), QStringLiteral("图像腐蚀"));
    item2 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN2), QStringLiteral("图像开运算"));
    item3 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN2), QStringLiteral("图像闭运算"));
    item4 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN2), QStringLiteral("图像梯度运算"));
    item5 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN2), QStringLiteral("图像顶帽运算"));
    item6 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN2), QStringLiteral("图像黑帽运算"));


    listWidget = new QListWidget;
    gridLayout  = new QGridLayout;
    tabWidget = new QTabWidget;

    pushbutton = new QPushButton(QStringLiteral("开始运行"));


    listWidget->addItem(item0);
    listWidget->addItem(item1);
    listWidget->addItem(item2);
    listWidget->addItem(item3);
    listWidget->addItem(item4);
    listWidget->addItem(item5);
    listWidget->addItem(item6);


    tabWidget->addTab(listWidget, QStringLiteral("图像形态学算法："));


    gridLayout->addWidget(tabWidget, 0, 0);
    gridLayout->addWidget(pushbutton, 1, 0);
    this->setLayout(gridLayout);
}



//图像CfarDetect操作

ImageCfarDectectProcessWidget::ImageCfarDectectProcessWidget(QWidget *parent):QWidget(parent)
{
    init();
}

ImageCfarDectectProcessWidget::~ImageCfarDectectProcessWidget()
{

}

void ImageCfarDectectProcessWidget::init()
{
    item0 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN3), QStringLiteral("基于Ruly分布的快速CFAR检测算法"));
    item1 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN3), QStringLiteral("基于Gaussian分布的快速CFAR检测算法"));
    item2 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN3), QStringLiteral("基于Weibull分布自适应窗口的快速CFAR检测算法"));
    item3 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN3), QStringLiteral("基于LogNormal分布的快速CFAR检测算法"));
    item4 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN3), QStringLiteral("基于K分布自适应窗口的快速CFAR检测算法"));


    listWidget = new QListWidget;
    gridLayout  = new QGridLayout;
    tabWidget = new QTabWidget;

    pushbutton = new QPushButton(QStringLiteral("开始运行"));


    listWidget->addItem(item0);
    listWidget->addItem(item1);
    listWidget->addItem(item2);
    listWidget->addItem(item3);
    listWidget->addItem(item4);


    tabWidget->addTab(listWidget, QStringLiteral("星载SAR图像舰船目标粗检测算法："));


    gridLayout->addWidget(tabWidget, 0, 0);
    gridLayout->addWidget(pushbutton, 1, 0);
    this->setLayout(gridLayout);
}


//图像SVM操作

ImageHogAndSvmProcessWidget::ImageHogAndSvmProcessWidget(QWidget *parent):QWidget(parent)
{
    init();

}



void ImageHogAndSvmProcessWidget::init()					//显示图像名称信息
{
    item0 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN4), QStringLiteral("根据粗检测结果，剔除形态不符合条件目标(显示图标序号)"));
    item1 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN4), QStringLiteral("根据粗检测结果，剔除形态不符合条件目标(不显示图标序号)"));
    item2 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN4), QStringLiteral("根据粗检测结果，剔除形态不符合条件目标(原框显示,不显示图标序号)"));
    item3 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN4), QStringLiteral("根据SVM训练模型，实现精确检测(显示图标序号)"));
    item4 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN4), QStringLiteral("根据SVM训练模型，实现精确检测(不显示图标序号)"));
    item5 = new QListWidgetItem(QIcon(PATH_IMAGE_PROCESS_RUN4), QStringLiteral("根据SVM训练模型，实现精确检测(原框显示,不显示图标序号)"));


    listWidget = new QListWidget;
    gridLayout  = new QGridLayout;
    tabWidget = new QTabWidget;

    pushbutton = new QPushButton(QStringLiteral("开始运行"));


    listWidget->addItem(item0);
    listWidget->addItem(item1);
    listWidget->addItem(item2);
    listWidget->addItem(item3);
    listWidget->addItem(item4);
    listWidget->addItem(item5);


    tabWidget->addTab(listWidget, QStringLiteral("HOG+SVM算法："));

    gridLayout->addWidget(tabWidget, 0, 0);
    gridLayout->addWidget(pushbutton, 1, 0);
    this->setLayout(gridLayout);
}

ImageHogAndSvmProcessWidget::~ImageHogAndSvmProcessWidget()
{

}




//总的图像处理窗口
ImageProcessWidget::ImageProcessWidget(QWidget *parent):QWidget(parent)
{
    init();
}


ImageProcessWidget::~ImageProcessWidget()
{

}

void ImageProcessWidget::init()
{
    widget0 = new ImageFilterProcessWidget;
    widget1 = new ImageBinaryProcessWidget;
    widget2 = new ImageMorphProcessWidget;
    widget3 = new ImageCfarDectectProcessWidget;
    widget4 = new ImageHogAndSvmProcessWidget;
    stackedLayout = new QStackedLayout;

    stackedLayout->addWidget(widget0);
    stackedLayout->addWidget(widget1);
    stackedLayout->addWidget(widget2);
    stackedLayout->addWidget(widget3);
    stackedLayout->addWidget(widget4);

    this->setLayout(stackedLayout);
}

void ImageProcessWidget::slot_switch_different_widget(int currentItem)
{
    stackedLayout->setCurrentIndex(currentItem);
}

void ImageProcessWidget::slots_getImage_and_ImageFilterProcess(QImage image)
{
    qDebug() << "ImageProcessWidget::slots_getImage_and_ImageFilterProcess(QImage image)";
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("请添加正确的缩略图"));
        return;
    }

    qDebug() << "image.format: " << image.format();


    Mat curMat = QImage2cvMat(image);

    Mat tgtMat = Mat::zeros(curMat.size(), curMat.type());
    qDebug() << "curMat.type(): " << curMat.type();

    int curNum = widget0->listWidget->currentRow();
    enum IMAGE_FILTER_TYPE filterType;

    QString processName;

    switch (curNum)
    {
        case 0:
            filterType = BOX_FILTER;
            processName = "boxFilter";
            break;
        case 1:
            filterType = BLUR;
            processName = "blurFilter";
            break;
        case 2:
            filterType = GAUSSIAN_BLUR;
            processName = "gaussianFilter";

            break;
        case 3:
            filterType = MEDIAN_BLUR;
            processName = "medianFilter";

            break;
        case 4:
            filterType = BILATERAL_FILTER;
            processName = "bilateralFilter";

            break;
        case 5:
            filterType = ADAPTIVE_BILATERAL_FILTER;
            processName = "adaptiveBilateralFilter";
            break;
        default:
            break;
    }

    ImageFilter imagefilter = ImageFilter(filterType);
    imagefilter.getImageFilterResult(curMat, tgtMat);

    QImage tgtImage = cvMat2QImage(tgtMat);

    emit signals_deliver_image_filter_result_to_central_area(tgtImage);
    emit signals_deliver_little_image_filter(tgtImage, processName);

    //2/28 添加目标检测信息
    QString outputInfo = processName + QStringLiteral("操作已完成");
    emit signals_deliver_objectDetectOutputInfo(outputInfo);

}

void ImageProcessWidget::slots_getImage_and_imageBinaryProcess(QImage image)    //二值化
{
    qDebug() << "ImageProcessWidget::slots_getImage_and_imageBinaryProcess(QImage image)";
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("请添加正确的缩略图"));
        return;
    }

    Mat curMat = QImage2cvMat(image);
    Mat tgtMat = Mat::zeros(curMat.size(), curMat.type());
    qDebug() << "curMat.type(): " << curMat.type();

    int curNum = widget1->listWidget->currentRow();
    enum IMAGE_BINARY_TYPE binaryType;

    QString processName;

    switch (curNum)
    {
        case 0:
            binaryType = ObjectImageBinary::THRESH_OTSU;
            processName = "otsu";
            break;
        case 1:
            binaryType = ObjectImageBinary::THRESH_BINARY;
            processName = "binary";
            break;
        case 2:
            binaryType = ObjectImageBinary::THRESH_BINARY_INV;
            processName = "binaryInv";

            break;
        case 3:
            binaryType = ObjectImageBinary::THRESH_TRUNC;
            processName = "Trunc";

            break;
        case 4:
            binaryType = ObjectImageBinary::THRESH_TOZERO;
            processName = "tozero";

            break;
        case 5:
            binaryType = ObjectImageBinary::THRESH_TOZERO_INV;
            processName = "tozeroInv";
            break;
        default:
            break;
    }

    IMAGE_THRELD_TYPE threldType = ObjectImageBinary::THRESHOLD;    //选择threld接口



    ImageBinary imageBinary = ImageBinary(threldType, binaryType);
    imageBinary.getImageBinaryResult(curMat, tgtMat);

    QImage tgtImage = cvMat2QImage(tgtMat);

    emit signals_deliver_image_binary_result_to_central_area(tgtImage);
    emit signals_deliver_little_image_binary(tgtImage, processName);

    //2/28 添加目标检测信息
    QString outputInfo = processName + QStringLiteral("操作已完成");
    emit signals_deliver_objectDetectOutputInfo(outputInfo);

}


void ImageProcessWidget::slots_getImage_and_imageMorphProcess(QImage image)    //形态学
{
    qDebug() << "ImageProcessWidget::slots_getImage_and_imageMorphProcess(QImage image)";
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("请添加正确的缩略图"));
        return;
    }

    Mat curMat = QImage2cvMat(image);
    Mat tgtMat = Mat::zeros(curMat.size(), curMat.type());
    qDebug() << "curMat.type(): " << curMat.type();

    int curNum = widget2->listWidget->currentRow();
    enum Morphological_Operation morphType;

    QString processName;

    switch (curNum)
    {
        case 0:
            morphType = MORPH_DILATE_OBJ;
            processName = "dilate";
            break;
        case 1:
            morphType = MORPH_ERODE_OBJ;
            processName = "erode";
            break;
        case 2:
            morphType = MORPH_OPEN_OBJ;
            processName = "open";

            break;
        case 3:
            morphType = MORPH_CLOSE_OBJ;
            processName = "close";

            break;
        case 4:
            morphType = MORPH_GRADIENT_OBJ;
            processName = "gradient";

            break;
        case 5:
            morphType = MORPH_TOPHAT_OBJ;
            processName = "tophat";
            break;
        case 6:
            morphType = MORPH_BLACKHAT_OBJ;
            processName = "blackhat";
            break;

        default:
            break;
    }

    int elem = DateMgr::getDataParas()->imageBinaryAndMorphParas->structuringElement;
    if (elem != 0 && elem != 1 && elem != 2)
    {
        elem = 0;
    }

    Structuring_Element shape = static_cast<enum Structuring_Element>(elem);   //三种类型分别为MORPH_RECT=0, MORPH_CROSS=1, MORPH_ELLIPSE=2
    Mat structuringElement = getStructuringElement(shape, Size(DateMgr::getDataParas()->imageBinaryAndMorphParas->dilateSize
                                                               , DateMgr::getDataParas()->imageBinaryAndMorphParas->erodeSize));


    morphologyEx(curMat, tgtMat, morphType, structuringElement);


    QImage tgtImage = cvMat2QImage(tgtMat);

    emit signals_deliver_image_morph_result_to_central_area(tgtImage);
    emit signals_deliver_little_image_morph(tgtImage, processName);

    //2/28 添加目标检测信息
    QString outputInfo = processName + QStringLiteral("操作已完成");
    emit signals_deliver_objectDetectOutputInfo(outputInfo);

}


void ImageProcessWidget::slots_getImage_and_imageCfarDetectProcess(QImage image)   //cfar 检测
{

    qDebug() << "ImageProcessWidget::slots_getImage_and_imageCfarDetectProcess(QImage image)";
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("请添加正确的缩略图"));
        return;
    }

    Mat curMat = QImage2cvMat(image);
    Mat tgtMat = Mat::zeros(curMat.size(), curMat.type());
    qDebug() << "curMat.type(): " << curMat.type();
    int curNum = widget3->listWidget->currentRow();



    enum IMAGE_FFT_TYPE cfarDetectType;
    QString processName;

    switch (curNum)
    {
        case 0:
            cfarDetectType = Ruly;
            processName = "Ruly";
            break;
        case 1:
            cfarDetectType = Gaussian;
            processName = "Gaussian";
            break;
        case 2:
            cfarDetectType = Gaussian_Adaptive_Window;
            processName = "Gaussian_Adaptive_Window";

            break;
        case 3:
            cfarDetectType = LogNormal;
            processName = "LogNormal";

            break;
        case 4:
            cfarDetectType = LogNormal_Adaptive_Window;
            processName = "LogNormal_Adaptive_Window";

            break;


        default:
            break;
    }

    ImageCfarDectect imageCfarDectect = ImageCfarDectect(cfarDetectType);
    imageCfarDectect.getImageCfarDectectResult(curMat, tgtMat);

    QImage tgtImage = cvMat2QImage(tgtMat);

    emit signals_deliver_image_cfar_detect_result_to_central_area(tgtImage);
    emit signals_deliver_little_image_cfarDetect(tgtImage, processName);

    //2/28 添加目标检测信息
    QString outputInfo = processName + QStringLiteral("操作已完成");
    emit signals_deliver_objectDetectOutputInfo(outputInfo);
}


void ImageProcessWidget::imageHogAndSvmProcess_delete_false_object_showImageNumber(QImage image, QString processName)
{
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("添加正确的图片进行处理"));
        return;
    }

    Mat curMat = QImage2cvMat(image);  //这里是进行了位 copy, 后面的函数可能会改变 QImage的值 ==》改成了深拷贝
    if (curMat.channels() != 1)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("被检测的图像通道数必须为1，请载入符合条件的图像"));
        return;
    }


    Mat tgtMat;
    cv::cvtColor(curMat, tgtMat, CV_GRAY2BGR);
    Mat tgtMat1;
    cv::cvtColor(curMat, tgtMat1, CV_GRAY2BGR);

    QVector<ObjectDetectResultParas> objectDetectResultVec;
    objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas();

    QVector<ObjectDetectResultParas> objectDetectResultVec_afterDeleteFalseObject;
    QVector<int> objNumber_afterDeleteFalseObject;




    float width;   // 宽度和高度
    float height;
    double widthHeightRadio;

    double resolution = DateMgr::getDataParas()->radarParas->resolution;
    int minWidth = DateMgr::getDataParas()->radarParas->minWidth/resolution;
    int maxWidth = DateMgr::getDataParas()->radarParas->maxWidth/resolution;
    int minHeight = DateMgr::getDataParas()->radarParas->minHeight/resolution;
    int maxHeight = DateMgr::getDataParas()->radarParas->maxHeight/resolution;
    double maxWidthHeightRadio =  DateMgr::getDataParas()->radarParas->maxWidthHeightRadio;

    qDebug() << resolution << minWidth << maxWidth << minHeight << maxHeight << maxWidthHeightRadio;

    for (int i=0; i<objectDetectResultVec.size(); i++)
    {

        Rect rect;
        rect = objectDetectResultVec[i].rect;

        width = objectDetectResultVec[i].width_Height_size.width;   // 宽度和高度
        height = objectDetectResultVec[i].width_Height_size.height;   // 宽度和高度
        widthHeightRadio = objectDetectResultVec[i].widthDivHeightRadio;

        if (!((width>=minWidth && width<=maxWidth)
        && (height>=minHeight && height<=maxHeight)
        && (widthHeightRadio <= maxWidthHeightRadio)))
        {
            //对给定的 2D 点集，寻找宣召角度为0的矩形包围圆
            qDebug() << i << width << height << widthHeightRadio;
            qDebug() << i << minWidth << maxWidth << minHeight << maxHeight << maxWidthHeightRadio;

            rectangle( tgtMat,
                rect.tl()-Point(2,2),
                rect.br()+Point(2,2),
                Scalar( 255, 0, 0 ),
                2,        //thickness = -1  代表为 实心的
                8 );
        }
        else
        {
            rectangle( tgtMat,
                rect.tl()-Point(2,2),
                rect.br()+Point(2,2),
                Scalar( 0, 0, 255 ),
                2,        //thickness = -1  代表为 实心的
                8 );

            rectangle( tgtMat1,
                rect.tl()-Point(2,2),
                rect.br()+Point(2,2),
                Scalar( 0, 0, 255 ),
                2,        //thickness = -1  代表为 实心的
                8 );

            objectDetectResultVec_afterDeleteFalseObject.push_back(objectDetectResultVec[i]);
            objNumber_afterDeleteFalseObject.push_back(i);
            cv::putText(tgtMat1, QString::number(i).toStdString(), rect.tl()-Point(6,6)
                        , CV_FONT_HERSHEY_SCRIPT_SIMPLEX, 0.4, Scalar(0,255,0));
        }
        cv::putText(tgtMat, QString::number(i).toStdString(), rect.tl()-Point(6,6)
                    , CV_FONT_HERSHEY_SCRIPT_SIMPLEX, 0.4, Scalar(0,255,0));
    }

    //保存好 删除不符合 目标形态
    ObjectDetectOptionWidget::setObjectDetectResultParas_afterDeleteFalseObject(objectDetectResultVec_afterDeleteFalseObject);
    ObjectDetectOptionWidget::setObjNumber_afterDeleteFalseObject(objNumber_afterDeleteFalseObject);


    QImage tgtImage = cvMat2QImage(tgtMat);

    emit signals_deliver_image_hog_svm_result_to_central_area(tgtImage);
    emit signals_deliver_little_image_HogAndSvm(tgtImage, processName);
    emit signals_deliver_little_image_HogAndSvm(cvMat2QImage(tgtMat1), processName);

    //2/28 添加目标检测信息
    QString outputInfo = QStringLiteral("开始删除 不符合舰船目标尺寸条件的虚假目标\n");
    outputInfo += QStringLiteral("获取粗检测目标个数为：") + QString::number(objectDetectResultVec.size()) +"\n";
    outputInfo += QStringLiteral("不符合舰船目标尺寸条件的虚假目标个数为：") + QString::number(objectDetectResultVec.size()
                                                                           -objectDetectResultVec_afterDeleteFalseObject.size()) +"\n";

    emit signals_deliver_objectDetectOutputInfo(outputInfo);

}

//不显示图像的序号， 固定窗口的大小
void ImageProcessWidget::imageHogAndSvmProcess_delete_false_object_noShowImageNumber(QImage image, QString processName)
{
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("添加正确的图片进行处理"));
        return;
    }

    Mat curMat = QImage2cvMat(image);  //这里是进行了位 copy, 后面的函数可能会改变 QImage的值 ==》改成了深拷贝
    if (curMat.channels() != 1)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("被检测的图像通道数必须为1，请载入符合条件的图像"));
        return;
    }


    Mat tgtMat;
    cv::cvtColor(curMat, tgtMat, CV_GRAY2BGR);
    Mat tgtMat1;
    cv::cvtColor(curMat, tgtMat1, CV_GRAY2BGR);

    QVector<ObjectDetectResultParas> objectDetectResultVec;
    objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas();

    QVector<ObjectDetectResultParas> objectDetectResultVec_afterDeleteFalseObject;
    QVector<int> objNumber_afterDeleteFalseObject;




    float width;   // 宽度和高度
    float height;
    double widthHeightRadio;

    double resolution = DateMgr::getDataParas()->radarParas->resolution;
    int minWidth = DateMgr::getDataParas()->radarParas->minWidth/resolution;
    int maxWidth = DateMgr::getDataParas()->radarParas->maxWidth/resolution;
    int minHeight = DateMgr::getDataParas()->radarParas->minHeight/resolution;
    int maxHeight = DateMgr::getDataParas()->radarParas->maxHeight/resolution;
    double maxWidthHeightRadio =  DateMgr::getDataParas()->radarParas->maxWidthHeightRadio;

    qDebug() << resolution << minWidth << maxWidth << minHeight << maxHeight << maxWidthHeightRadio;

    QSize sliceImageWin = DateMgr::getDataParas()->hogFeatureParas->detectSliceImageWin;

    for (int i=0; i<objectDetectResultVec.size(); i++)
    {

        Rect rect;
        rect = objectDetectResultVec[i].rect;

        Rect objectRect = assignSampleObjectSize(Size(image.width(), image.height())
                                                 ,rect, Size(sliceImageWin.width(), sliceImageWin.height()));

        width = objectDetectResultVec[i].width_Height_size.width;   // 宽度和高度
        height = objectDetectResultVec[i].width_Height_size.height;   // 宽度和高度
        widthHeightRadio = objectDetectResultVec[i].widthDivHeightRadio;

        if (!((width>=minWidth && width<=maxWidth)
        && (height>=minHeight && height<=maxHeight)
        && (widthHeightRadio <= maxWidthHeightRadio)))
        {
            //对给定的 2D 点集，寻找宣召角度为0的矩形包围圆
            qDebug() << i << width << height << widthHeightRadio;
            qDebug() << i << minWidth << maxWidth << minHeight << maxHeight << maxWidthHeightRadio;



            ellipse(tgtMat, rect.tl()+Point(rect.width/2, rect.height/2)
                    ,Size(rect.width+3, rect.height+3),0, 0,360, Scalar( 255, 255, 255), 2, 8);
//            rectangle( tgtMat,
//                rect.tl()-Point(2,2),
//                rect.br()+Point(2,2),
//                Scalar( 255, 0, 0 ),
//                2,        //thickness = -1  代表为 实心的
//                8 );
        }
        else
        {
            rectangle( tgtMat,
                objectRect.tl()-Point(2,2),
                objectRect.br()+Point(2,2),
                Scalar( 255, 255, 255 ),
                2,        //thickness = -1  代表为 实心的
                8 );

            rectangle( tgtMat1,
                objectRect.tl()-Point(2,2),
                objectRect.br()+Point(2,2),
                Scalar( 255, 255, 255 ),
                2,        //thickness = -1  代表为 实心的
                8 );

            objectDetectResultVec_afterDeleteFalseObject.push_back(objectDetectResultVec[i]);
            objNumber_afterDeleteFalseObject.push_back(i);

        }
    }

    //保存好 删除不符合 目标形态
    ObjectDetectOptionWidget::setObjectDetectResultParas_afterDeleteFalseObject(objectDetectResultVec_afterDeleteFalseObject);
    ObjectDetectOptionWidget::setObjNumber_afterDeleteFalseObject(objNumber_afterDeleteFalseObject);


    QImage tgtImage = cvMat2QImage(tgtMat);

    emit signals_deliver_image_hog_svm_result_to_central_area(tgtImage);
    emit signals_deliver_little_image_HogAndSvm(tgtImage, processName);
    emit signals_deliver_little_image_HogAndSvm(cvMat2QImage(tgtMat1), processName);

    //2/28 添加目标检测信息
    QString outputInfo = QStringLiteral("开始删除 不符合舰船目标尺寸条件的虚假目标\n");
    outputInfo += QStringLiteral("获取粗检测目标个数为：") + QString::number(objectDetectResultVec.size()) +"\n";
    outputInfo += QStringLiteral("不符合舰船目标尺寸条件的虚假目标个数为：") + QString::number(objectDetectResultVec.size()
                                                                           -objectDetectResultVec_afterDeleteFalseObject.size()) +"\n";

    emit signals_deliver_objectDetectOutputInfo(outputInfo);

//    qDebug() << "objectDetectResultVec.size()" << objectDetectResultVec.size();
//    qDebug() << "objectDetectResultVec_afterDeleteFalseObject.size()" << objectDetectResultVec_afterDeleteFalseObject.size();
}

//不显示图像的序号， 原始目标的窗口
void ImageProcessWidget::imageHogAndSvmProcess_delete_false_object_noShowImageNumber_original_rectangle(QImage image, QString processName)
{
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("添加正确的图片进行处理"));
        return;
    }

    Mat curMat = QImage2cvMat(image);  //这里是进行了位 copy, 后面的函数可能会改变 QImage的值 ==》改成了深拷贝
    if (curMat.channels() != 1)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("被检测的图像通道数必须为1，请载入符合条件的图像"));
        return;
    }


    Mat tgtMat;
    cv::cvtColor(curMat, tgtMat, CV_GRAY2BGR);
    Mat tgtMat1;
    cv::cvtColor(curMat, tgtMat1, CV_GRAY2BGR);

    QVector<ObjectDetectResultParas> objectDetectResultVec;
    objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas();

    QVector<ObjectDetectResultParas> objectDetectResultVec_afterDeleteFalseObject;
    QVector<int> objNumber_afterDeleteFalseObject;




    float width;   // 宽度和高度
    float height;
    double widthHeightRadio;

    double resolution = DateMgr::getDataParas()->radarParas->resolution;
    int minWidth = DateMgr::getDataParas()->radarParas->minWidth/resolution;
    int maxWidth = DateMgr::getDataParas()->radarParas->maxWidth/resolution;
    int minHeight = DateMgr::getDataParas()->radarParas->minHeight/resolution;
    int maxHeight = DateMgr::getDataParas()->radarParas->maxHeight/resolution;
    double maxWidthHeightRadio =  DateMgr::getDataParas()->radarParas->maxWidthHeightRadio;

    qDebug() << resolution << minWidth << maxWidth << minHeight << maxHeight << maxWidthHeightRadio;

    QSize sliceImageWin = DateMgr::getDataParas()->hogFeatureParas->detectSliceImageWin;
    for (int i=0; i<objectDetectResultVec.size(); i++)
    {

        Rect rect;
        rect = objectDetectResultVec[i].rect;

        Rect objectRect = assignSampleObjectSize(Size(image.width(), image.height())
                                                 ,rect, Size(sliceImageWin.width(), sliceImageWin.height()));

        width = objectDetectResultVec[i].width_Height_size.width;   // 宽度和高度
        height = objectDetectResultVec[i].width_Height_size.height;   // 宽度和高度
        widthHeightRadio = objectDetectResultVec[i].widthDivHeightRadio;

        if (!((width>=minWidth && width<=maxWidth)
        && (height>=minHeight && height<=maxHeight)
        && (widthHeightRadio <= maxWidthHeightRadio)))
        {
            //对给定的 2D 点集，寻找宣召角度为0的矩形包围圆
            qDebug() << i << width << height << widthHeightRadio;
            qDebug() << i << minWidth << maxWidth << minHeight << maxHeight << maxWidthHeightRadio;



            ellipse(tgtMat, rect.tl()+Point(rect.width/2, rect.height/2)
                    ,Size(rect.width+3, rect.height+3),0, 0,360, Scalar( 255, 255, 255), 2, 8);
//            rectangle( tgtMat,
//                rect.tl()-Point(2,2),
//                rect.br()+Point(2,2),
//                Scalar( 255, 0, 0 ),
//                2,        //thickness = -1  代表为 实心的
//                8 );
        }
        else
        {
            rectangle( tgtMat,
                rect.tl()-Point(2,2),
                rect.br()+Point(2,2),
                Scalar( 255, 255, 255 ),
                2,        //thickness = -1  代表为 实心的
                8 );

            rectangle( tgtMat1,
                rect.tl()-Point(2,2),
                rect.br()+Point(2,2),
                Scalar( 255, 255, 255 ),
                2,        //thickness = -1  代表为 实心的
                8 );

            objectDetectResultVec_afterDeleteFalseObject.push_back(objectDetectResultVec[i]);
            objNumber_afterDeleteFalseObject.push_back(i);


        }
    }

    //保存好 删除不符合 目标形态
    ObjectDetectOptionWidget::setObjectDetectResultParas_afterDeleteFalseObject(objectDetectResultVec_afterDeleteFalseObject);
    ObjectDetectOptionWidget::setObjNumber_afterDeleteFalseObject(objNumber_afterDeleteFalseObject);


    QImage tgtImage = cvMat2QImage(tgtMat);

    emit signals_deliver_image_hog_svm_result_to_central_area(tgtImage);
    emit signals_deliver_little_image_HogAndSvm(tgtImage, processName);
    emit signals_deliver_little_image_HogAndSvm(cvMat2QImage(tgtMat1), processName);


    //2/28 添加目标检测信息
    QString outputInfo = QStringLiteral("开始删除 不符合舰船目标尺寸条件的虚假目标\n");
    outputInfo += QStringLiteral("获取粗检测目标个数为：") + QString::number(objectDetectResultVec.size()) +"\n";
    outputInfo += QStringLiteral("不符合舰船目标尺寸条件的虚假目标个数为：") + QString::number(objectDetectResultVec.size()
                                                                           -objectDetectResultVec_afterDeleteFalseObject.size()) +"\n";

    emit signals_deliver_objectDetectOutputInfo(outputInfo);
//    qDebug() << "objectDetectResultVec.size()" << objectDetectResultVec.size();
//    qDebug() << "objectDetectResultVec_afterDeleteFalseObject.size()" << objectDetectResultVec_afterDeleteFalseObject.size();
}

void ImageProcessWidget::imageHogAndSvmProcess_svm_recognition(QImage image, QString processName)
{
    QVector<ObjectDetectResultParas> objectDetectResultVec;
    QVector<ObjectDetectResultParas> objectDetectResultVec_afterPrecise;
    QVector<int> objNumber_afterDeleteFalseObj;
    QVector<int> objNumber_afterPrecise;
    objNumber_afterDeleteFalseObj = ObjectDetectOptionWidget::getObjNumber_afterDeleteFalseObject();



    if (!ObjectDetectOptionWidget::getObjectDetectResultParas().isEmpty())
    {
        qDebug() << "getObjectDetectResultParas";
        objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas();
    }

    if (!ObjectDetectOptionWidget::getObjectDetectResultParas_afterDeleteFalseObject().isEmpty())
    {
        qDebug() << "getObjectDetectResultParas_afterDeleteFalseObject";
        objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas_afterDeleteFalseObject();
    }

//    if (!ObjectDetectOptionWidget::getObjectDetectResultParas_afterPrecise().isEmpty())
//    {
//        qDebug() << "getObjectDetectResultParas_afterPrecise";
//        objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas_afterPrecise();
//    }


    QString fileFull = QFileDialog::getOpenFileName(this
                , QStringLiteral("载入SVM模型文件"), "./"
                , QStringLiteral("*.xml"));

    if (fileFull.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    Mat curMat = QImage2cvMat(image);  //这里是进行了位 copy, 后面的函数可能会改变 QImage的值 ==》改成了深拷贝
    if (curMat.channels() != 1)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("被检测的图像通道数必须为1，请载入符合条件的图像"));
        return;
    }


    Mat tgtMat;
    cv::cvtColor(curMat, tgtMat, CV_GRAY2BGR);

    Mat tgtMat1;
    cv::cvtColor(curMat, tgtMat1, CV_GRAY2BGR);


    HogParameters hogParameters;
    hogParameters.blockSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockWin);
    hogParameters.winSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->detectWin);
    hogParameters.winStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->winStrider);
    hogParameters.blockStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockStrider);
    hogParameters.cellSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->cellWin);
    hogParameters.nbins = DateMgr::getDataParas()->hogFeatureParas->nbins;
    QSize sliceImageWin = DateMgr::getDataParas()->hogFeatureParas->detectSliceImageWin;
    //qDebug() << "zhanglin" << sliceImageWin;


    for (int i=0; i<objectDetectResultVec.size(); ++i)
    {
        Rect rect = objectDetectResultVec[i].rect;


        Rect objectRect = assignSampleObjectSize(Size(image.width(), image.height())
                                                 ,rect, Size(sliceImageWin.width(), sliceImageWin.height()));

        QRect tgtCutRect = QRect(objectRect.x, objectRect.y
                                 , objectRect.width, objectRect.height);
        QImage tgtImage = image.copy(tgtCutRect);


//        tgtImage = tgtImage.scaled(64, 64);


        Mat src = QImage2cvMat(tgtImage);
        float svmPredict = getSingleImageSvmPredictResult(src, fileFull.toStdString(), hogParameters);
        qDebug() << i << "==>" << svmPredict;

        if ((svmPredict - 1 >= -0.000001) && (svmPredict - 1 <= 0.000001))   //检测为真
        {

            rectangle( tgtMat,
                objectRect.tl()-Point(2,2),
                objectRect.br()+Point(2,2),
                Scalar( 255, 255, 255 ),
                2,        //thickness = -1  代表为 实心的
                8 );

            rectangle( tgtMat1,
                objectRect.tl()-Point(2,2),
                objectRect.br()+Point(2,2),
                Scalar( 255, 255, 255 ),
                2,        //thickness = -1  代表为 实心的
                8 );

            objectDetectResultVec_afterPrecise.push_back(objectDetectResultVec[i]);
            objNumber_afterPrecise.push_back(objNumber_afterDeleteFalseObj[i]);
        }
        else
        {
            ellipse(tgtMat, rect.tl()+Point(rect.width/2, rect.height/2)
                    ,Size(rect.width+3, rect.height+3),0, 0,360, Scalar( 255, 255, 255), 2, 8);

        }
    }

    //保存好 删除不符合 目标形态
    ObjectDetectOptionWidget::setObjectDetectResultParas_afterPrecise(objectDetectResultVec_afterPrecise);
    ObjectDetectOptionWidget::setObjNumber_afterPrecise(objNumber_afterPrecise);

    emit signals_deliver_image_hog_svm_result_to_central_area(cvMat2QImage(tgtMat));
    emit signals_deliver_little_image_HogAndSvm(cvMat2QImage(tgtMat), processName);
    emit signals_deliver_little_image_HogAndSvm(cvMat2QImage(tgtMat1), processName);

    //2/28 添加目标检测信息
    QString outputInfo = QStringLiteral("通过svm模型，开始舰船识别，删除虚假目标\n");
    outputInfo += QStringLiteral("获取舰船识别前，精确检测目标个数为：") + QString::number(objectDetectResultVec.size()) +"\n";
    outputInfo += QStringLiteral("虚假目标个数为：") + QString::number(objectDetectResultVec.size()
                                                                           -objectDetectResultVec_afterPrecise.size()) +"\n";
    outputInfo += QStringLiteral("舰船检测识别后，目标个数为") + QString::number(objectDetectResultVec_afterPrecise.size()) +"\n";

    emit signals_deliver_objectDetectOutputInfo(outputInfo);

//    qDebug() << "objectDetectResultVec.size()" << objectDetectResultVec.size();
//    qDebug() << "objectDetectResultVec_afterPrecise.size()" << objectDetectResultVec_afterPrecise.size();

}


void ImageProcessWidget::imageHogAndSvmProcess_svm_recognition_showImageNumber(QImage image, QString processName)
{
    QVector<ObjectDetectResultParas> objectDetectResultVec;
    QVector<ObjectDetectResultParas> objectDetectResultVec_afterPrecise;
    QVector<int> objNumber_afterPrecise;
    QVector<int> objNumber_afterDeleteFalseObj;
    objNumber_afterDeleteFalseObj = ObjectDetectOptionWidget::getObjNumber_afterDeleteFalseObject();


    if (!ObjectDetectOptionWidget::getObjectDetectResultParas().isEmpty())
    {
        qDebug() << "getObjectDetectResultParas";
        objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas();
    }

    if (!ObjectDetectOptionWidget::getObjectDetectResultParas_afterDeleteFalseObject().isEmpty())
    {
        qDebug() << "getObjectDetectResultParas_afterDeleteFalseObject";
        objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas_afterDeleteFalseObject();
    }

//    if (!ObjectDetectOptionWidget::getObjectDetectResultParas_afterPrecise().isEmpty())
//    {
//        qDebug() << "getObjectDetectResultParas_afterPrecise";
//        objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas_afterPrecise();
//    }


    QString fileFull = QFileDialog::getOpenFileName(this
                , QStringLiteral("载入SVM模型文件"), "./"
                , QStringLiteral("*.xml"));

    if (fileFull.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    Mat curMat = QImage2cvMat(image);  //这里是进行了位 copy, 后面的函数可能会改变 QImage的值 ==》改成了深拷贝
    if (curMat.channels() != 1)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("被检测的图像通道数必须为1，请载入符合条件的图像"));
        return;
    }


    Mat tgtMat;
    cv::cvtColor(curMat, tgtMat, CV_GRAY2BGR);

    Mat tgtMat1;
    cv::cvtColor(curMat, tgtMat1, CV_GRAY2BGR);


    HogParameters hogParameters;
    hogParameters.blockSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockWin);
    hogParameters.winSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->detectWin);
    hogParameters.winStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->winStrider);
    hogParameters.blockStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockStrider);
    hogParameters.cellSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->cellWin);
    hogParameters.nbins = DateMgr::getDataParas()->hogFeatureParas->nbins;
    QSize sliceImageWin = DateMgr::getDataParas()->hogFeatureParas->detectSliceImageWin;



    for (int i=0; i<objectDetectResultVec.size(); ++i)
    {
        Rect rect = objectDetectResultVec[i].rect;


        Rect objectRect = assignSampleObjectSize(Size(image.width(), image.height())
                                                 ,rect, Size(sliceImageWin.width(), sliceImageWin.height()));

        QRect tgtCutRect = QRect(objectRect.x, objectRect.y
                                 , objectRect.width, objectRect.height);
        QImage tgtImage = image.copy(tgtCutRect);


//        tgtImage = tgtImage.scaled(64, 64);


        Mat src = QImage2cvMat(tgtImage);
        float svmPredict = getSingleImageSvmPredictResult(src, fileFull.toStdString(), hogParameters);
        qDebug() << i << "==>" << svmPredict;

        if ((svmPredict - 1 >= -0.000001) && (svmPredict - 1 <= 0.000001))   //检测为真
        {

            rectangle( tgtMat,
                rect.tl()-Point(2,2),
                rect.br()+Point(2,2),
                Scalar( 0, 0, 255 ),
                2,        //thickness = -1  代表为 实心的
                8 );

            rectangle( tgtMat1,
                rect.tl()-Point(2,2),
                rect.br()+Point(2,2),
                Scalar( 0, 0, 255 ),
                2,        //thickness = -1  代表为 实心的
                8 );

            objectDetectResultVec_afterPrecise.push_back(objectDetectResultVec[i]);
            objNumber_afterPrecise.push_back(objNumber_afterDeleteFalseObj[i]);
            cv::putText(tgtMat1, QString::number(i).toStdString(), rect.tl()-Point(6,6)
                        , CV_FONT_HERSHEY_SCRIPT_SIMPLEX, 0.4, Scalar(0,255,0));
        }
        else
        {
            ellipse(tgtMat, rect.tl()+Point(rect.width/2, rect.height/2)
                    ,Size(rect.width+3, rect.height+3),0, 0,360, Scalar( 255, 0, 0), 2, 8);
        }

        cv::putText(tgtMat, QString::number(i).toStdString(), rect.tl()-Point(6,6)
                , CV_FONT_HERSHEY_SCRIPT_SIMPLEX, 0.4, Scalar(0,255,0));
    }

    //保存好 删除不符合 目标形态
    ObjectDetectOptionWidget::setObjectDetectResultParas_afterPrecise(objectDetectResultVec_afterPrecise);
    ObjectDetectOptionWidget::setObjNumber_afterPrecise(objNumber_afterPrecise);

    emit signals_deliver_image_hog_svm_result_to_central_area(cvMat2QImage(tgtMat));
    emit signals_deliver_little_image_HogAndSvm(cvMat2QImage(tgtMat), processName);
    emit signals_deliver_little_image_HogAndSvm(cvMat2QImage(tgtMat1), processName);


    //2/28 添加目标检测信息
    QString outputInfo = QStringLiteral("通过svm模型，开始舰船识别，删除虚假目标\n");
    outputInfo += QStringLiteral("获取舰船识别前，精确检测目标个数为：") + QString::number(objectDetectResultVec.size()) +"\n";
    outputInfo += QStringLiteral("虚假目标个数为：") + QString::number(objectDetectResultVec.size()
                                                                           -objectDetectResultVec_afterPrecise.size()) +"\n";
    outputInfo += QStringLiteral("舰船检测识别后，目标个数为") + QString::number(objectDetectResultVec_afterPrecise.size()) +"\n";

    emit signals_deliver_objectDetectOutputInfo(outputInfo);

//    qDebug() << "objectDetectResultVec.size()" << objectDetectResultVec.size();
//    qDebug() << "objectDetectResultVec_afterPrecise.size()" << objectDetectResultVec_afterPrecise.size();
}

void ImageProcessWidget::imageHogAndSvmProcess_svm_recognition__noShowImageNumber_original_rectangle(QImage image, QString processName)
{
    QVector<ObjectDetectResultParas> objectDetectResultVec;
    QVector<ObjectDetectResultParas> objectDetectResultVec_afterPrecise;
    QVector<int> objNumber_afterPrecise;
    QVector<int> objNumber_afterDeleteFalseObj;
    objNumber_afterDeleteFalseObj = ObjectDetectOptionWidget::getObjNumber_afterDeleteFalseObject();



    if (!ObjectDetectOptionWidget::getObjectDetectResultParas().isEmpty())
    {
        qDebug() << "getObjectDetectResultParas";
        objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas();
    }

    if (!ObjectDetectOptionWidget::getObjectDetectResultParas_afterDeleteFalseObject().isEmpty())
    {
        qDebug() << "getObjectDetectResultParas_afterDeleteFalseObject";
        objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas_afterDeleteFalseObject();
    }

//    if (!ObjectDetectOptionWidget::getObjectDetectResultParas_afterPrecise().isEmpty())
//    {
//        qDebug() << "getObjectDetectResultParas_afterPrecise";
//        objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas_afterPrecise();
//    }


    QString fileFull = QFileDialog::getOpenFileName(this
                , QStringLiteral("载入SVM模型文件"), "./"
                , QStringLiteral("*.xml"));

    if (fileFull.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    Mat curMat = QImage2cvMat(image);  //这里是进行了位 copy, 后面的函数可能会改变 QImage的值 ==》改成了深拷贝
    if (curMat.channels() != 1)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("被检测的图像通道数必须为1，请载入符合条件的图像"));
        return;
    }


    Mat tgtMat;
    cv::cvtColor(curMat, tgtMat, CV_GRAY2BGR);

    Mat tgtMat1;
    cv::cvtColor(curMat, tgtMat1, CV_GRAY2BGR);


    HogParameters hogParameters;
    hogParameters.blockSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockWin);
    hogParameters.winSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->detectWin);
    hogParameters.winStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->winStrider);
    hogParameters.blockStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockStrider);
    hogParameters.cellSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->cellWin);
    hogParameters.nbins = DateMgr::getDataParas()->hogFeatureParas->nbins;
    QSize sliceImageWin = DateMgr::getDataParas()->hogFeatureParas->detectSliceImageWin;



    for (int i=0; i<objectDetectResultVec.size(); ++i)
    {
        Rect rect = objectDetectResultVec[i].rect;


        Rect objectRect = assignSampleObjectSize(Size(image.width(), image.height())
                                                 ,rect, Size(sliceImageWin.width(), sliceImageWin.height()));

        QRect tgtCutRect = QRect(objectRect.x, objectRect.y
                                 , objectRect.width, objectRect.height);
        QImage tgtImage = image.copy(tgtCutRect);


//        tgtImage = tgtImage.scaled(64, 64);


        Mat src = QImage2cvMat(tgtImage);
        float svmPredict = getSingleImageSvmPredictResult(src, fileFull.toStdString(), hogParameters);
        qDebug() << i << "==>" << svmPredict;

        if ((svmPredict - 1 >= -0.000001) && (svmPredict - 1 <= 0.000001))   //检测为真
        {

            rectangle( tgtMat,
                rect.tl()-Point(2,2),
                rect.br()+Point(2,2),
                Scalar( 255, 255, 255 ),
                2,        //thickness = -1  代表为 实心的
                8 );

            rectangle( tgtMat1,
                rect.tl()-Point(2,2),
                rect.br()+Point(2,2),
                Scalar( 255, 255, 255 ),
                2,        //thickness = -1  代表为 实心的
                8 );

            objectDetectResultVec_afterPrecise.push_back(objectDetectResultVec[i]);
            objNumber_afterPrecise.push_back(objNumber_afterDeleteFalseObj[i]);
        }
        else
        {
            ellipse(tgtMat, rect.tl()+Point(rect.width/2, rect.height/2)
                    ,Size(rect.width+3, rect.height+3),0, 0,360, Scalar( 255, 255, 255), 2, 8);

        }
    }

    //保存好 删除不符合 目标形态
    ObjectDetectOptionWidget::setObjectDetectResultParas_afterPrecise(objectDetectResultVec_afterPrecise);
    ObjectDetectOptionWidget::setObjNumber_afterPrecise(objNumber_afterPrecise);

    emit signals_deliver_image_hog_svm_result_to_central_area(cvMat2QImage(tgtMat));
    emit signals_deliver_little_image_HogAndSvm(cvMat2QImage(tgtMat), processName);
    emit signals_deliver_little_image_HogAndSvm(cvMat2QImage(tgtMat1), processName);

    //2/28 添加目标检测信息
    QString outputInfo = QStringLiteral("通过svm模型，开始舰船识别，删除虚假目标\n");
    outputInfo += QStringLiteral("获取舰船识别前，精确检测目标个数为：") + QString::number(objectDetectResultVec.size()) +"\n";
    outputInfo += QStringLiteral("虚假目标个数为：") + QString::number(objectDetectResultVec.size()
                                                                           -objectDetectResultVec_afterPrecise.size()) +"\n";
    outputInfo += QStringLiteral("舰船检测识别后，目标个数为") + QString::number(objectDetectResultVec_afterPrecise.size()) +"\n";

    emit signals_deliver_objectDetectOutputInfo(outputInfo);
//    qDebug() << "objectDetectResultVec.size()" << objectDetectResultVec.size();
//    qDebug() << "objectDetectResultVec_afterPrecise.size()" << objectDetectResultVec_afterPrecise.size();
}


void ImageProcessWidget::slots_getImage_and_imageHogAndSvmProcess(QImage image)   //hog 和 svm 检测
{
    qDebug() << "ImageProcessWidget::slots_getImage_and_imageHogAndSvmProcess(QImage image)";
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("请添加正确的缩略图"));
        return;
    }

    int curNum = widget4->listWidget->currentRow();

    QString processName;

    switch (curNum)
    {
        case 0:
            processName = "DeleteFalseObject_ShowImageNumber";
            imageHogAndSvmProcess_delete_false_object_showImageNumber(image, processName);
            break;

        case 1:
            processName = "DeleteFalseObject_NoShowImageNumber";
            imageHogAndSvmProcess_delete_false_object_noShowImageNumber(image, processName);
            break;

        case 2:
            processName = "DeleteFalseObject_NoShowImageNumber_Original_rectangle";
            imageHogAndSvmProcess_delete_false_object_noShowImageNumber_original_rectangle(image, processName);
            break;
        case 3:
            processName = "SVM_Recognition_showImageNumber";

            imageHogAndSvmProcess_svm_recognition_showImageNumber(image, processName);
            break;

        case 4:
            processName = "SVM_Recognition";
            imageHogAndSvmProcess_svm_recognition(image, processName);
            break;

        case 5:
            processName = "SVM_Recognition_original_rectangle";

            imageHogAndSvmProcess_svm_recognition__noShowImageNumber_original_rectangle(image, processName);
            break;

        default:
            break;
    }

}


void ImageProcessWidget::slots_getImage_and_imageGetRoiProcess(QImage image)     //获取检测图像的 ROI
{
    qDebug() << "ImageProcessWidget::slots_getImage_and_imageGetRoiProcess(QImage image)";
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("添加正确的图片进行处理"));
        return;
    }

    Mat curMat = QImage2cvMat(image);  //这里是进行了位 copy, 后面的函数可能会改变 QImage的值 ==》改成了深拷贝
    if (curMat.channels() != 1)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("被检测的图像通道数必须为1，请载入符合条件的图像"));
        return;
    }

    //Mat tgtMat = Mat::zeros(curMat.size(), curMat.type());
    //Mat tgtMat = Mat::zeros(curMat.size(), CV_8UC3);
    Mat tgtMat;
    cv::cvtColor(curMat, tgtMat, CV_GRAY2BGR);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    QVector<ObjectDetectResultParas> objectDetectResultVec;   //用来存储 检测出来的目标 描述信息

//    /* Contour retrieval modes */
//    enum
//    {
//        CV_RETR_EXTERNAL=0,
//        CV_RETR_LIST=1,
//        CV_RETR_CCOMP=2,
//        CV_RETR_TREE=3,
//        CV_RETR_FLOODFILL=4
//    };

    //查找轮廓
    findContours( curMat, contours, hierarchy,
        CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

    ObjectDetectResultParas singleObjectDetectResult;

    for (int i=0; i<contours.size(); i++)
    {
        //对给定的 2D 点集，寻找宣召角度为0的矩形包围圆
        Rect rect;
        rect = boundingRect(contours[i]);
        singleObjectDetectResult.rect.x = rect.x;
        singleObjectDetectResult.rect.y = rect.y;
        singleObjectDetectResult.rect.width = rect.width;
        singleObjectDetectResult.rect.height = rect.height;

        rectangle( tgtMat,
            rect.tl()-Point(2,2),
            rect.br()+Point(2,2),
            Scalar( 0, 0, 255 ),
            2,        //thickness = -1  代表为 实心的
            8 );

        //drawContours( tgtMat, contours, i, Scalar(0,0,255), 1);
        cv::putText(tgtMat, QString::number(i).toStdString(), rect.tl()-Point(6,6)
                    , CV_FONT_HERSHEY_SCRIPT_SIMPLEX, 0.4, Scalar(0,255,0));

        //继续添加 描述信息
        //qDebug() << QString::number(i) << " = " << contours[i].size();
        RotatedRect box = minAreaRect(contours[i]);
        //qDebug() << QString::number(i) << " ==> pixNum = " << contours[i].size();
        //qDebug() << QString::number(i) << " ==> angle = " << box.angle;
        //qDebug() << QString::number(i) << " ==> arcLength = " << arcLength(contours[i], true);
        //qDebug() << QString::number(i) << " ==> contourArea = " << contourArea(contours[i]);
        //qDebug() << QString::number(i) << singleObjectDetectResult.rect.x
//                 << singleObjectDetectResult.rect.y
//                 << singleObjectDetectResult.rect.width
//                 << singleObjectDetectResult.rect.height;


        // 画出最小的 边框矩形
//        Point2f vertex[4];         //四个角点从右下角开始，顺时针环绕
//        box.points(vertex);        //返回矩阵的四个角点的坐标值


//        //绘制出最小面积的包围矩形
//        for( int i = 0; i < 4; i++ )
//        {
//            line(tgtMat, vertex[i], vertex[(i+1)%4], Scalar(0, 0, 255), 1, 8);
//        }



        singleObjectDetectResult.rectRotate = box.angle;
        singleObjectDetectResult.massCenter = box.center;        // 矩形的质心
        singleObjectDetectResult.width_Height_size.width = box.size.width>box.size.height?box.size.width:box.size.height;   // 宽度和高度
        singleObjectDetectResult.width_Height_size.height = box.size.width>box.size.height?box.size.height:box.size.width;   // 宽度和高度

        singleObjectDetectResult.widthDivHeightRadio = singleObjectDetectResult.width_Height_size.width/singleObjectDetectResult.width_Height_size.height;         // 2/19添加宽高比率
        singleObjectDetectResult.countourPixCount = contours[i].size();
        singleObjectDetectResult.countourArcLength = arcLength(contours[i], true);
        singleObjectDetectResult.contourArea = contourArea(contours[i]);
        singleObjectDetectResult.isChecked = false;

        objectDetectResultVec.push_back(singleObjectDetectResult);

    }

//    for (int i=0; i<contours.size(); i++)
//    {
//        //对给定的 2D 点集，寻找最小包围矩形

//        RotatedRect box = minAreaRect(contours[i]);
////        Point2f vertex[4];         //四个角点从右下角开始，顺时针环绕
////        box.points(vertex);        //返回矩阵的四个角点的坐标值

////        Rect brect = box.boundingRect();
////        rectangle(tgtMat, brect, Scalar(255,0,0));


////        //绘制出最小面积的包围矩形
////        for( int i = 0; i < 4; i++ )
////            line(tgtMat, vertex[i], vertex[(i+1)%4], Scalar(0, 0, 255), 2, CV_AA);



////        //( tgtMat, contours, i, Scalar(0,0,255), 1);
////        cv::putText(tgtMat, QString::number(i).toStdString(), vertex[0]-Point2f(6,6)
////                    , CV_FONT_HERSHEY_SCRIPT_SIMPLEX, 0.4, Scalar(255,0,0));

//        qDebug() << QString::number(i) << " ==> pixNum = " << contours[i].size();
//        qDebug() << QString::number(i) << " ==> angle = " << box.angle;
//        qDebug() << QString::number(i) << " ==> arcLength = " << arcLength(contours[i], true);
//        qDebug() << QString::number(i) << " ==> contourArea = " << contourArea(contours[i]);
//        qDebug() << QString::number(i) << singleObjectDetectResult.rect.x
//                 << singleObjectDetectResult.rect.y
//                 << singleObjectDetectResult.rect.width
//                 << singleObjectDetectResult.rect.height;


////        qDebug() << QString::number(i) << vertex[0].x << vertex[0].y
////                 << vertex[1].x << vertex[1].y
////                    << vertex[2].x << vertex[2].y
////                       << vertex[3].x << vertex[3].y;

//        singleObjectDetectResult.rectRotate = box.angle;
//        singleObjectDetectResult.countourPixCount = contours[i].size();
//        singleObjectDetectResult.countourArcLength = arcLength(contours[i], true);
//        singleObjectDetectResult.contourArea = contourArea(contours[i]);
//        singleObjectDetectResult.isChecked = false;

//        objectDetectResultVec.push_back(singleObjectDetectResult);
//    }



    qDebug() << "objectDetectResultVec.size" << objectDetectResultVec.size();
//将处理后的图片 显示在中央和小图窗口中

    QImage tgtImage = cvMat2QImage(tgtMat);
    QString processName = "GetRoi";
    emit signals_deliver_image_getRoi_result_to_central_area(tgtImage);
    emit signals_deliver_little_image_GetRoi(tgtImage, processName);
    emit signals_deliver_ObjectDetectResultParas_to_ObjectDetectOptionWidget(objectDetectResultVec);
}




void ImageProcessWidget::slots_getImage_and_imageDrawContourProcess(QImage image)     //获取检测图像的 目标边框
{
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("添加正确的图片进行处理"));
        return;
    }

    Mat curMat = QImage2cvMat(image);  //这里是进行了位 copy, 后面的函数可能会改变 QImage的值 ==》改成了深拷贝
    if (curMat.channels() != 1)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("被检测的图像通道数必须为1，请载入符合条件的图像"));
        return;
    }

    //Mat tgtMat = Mat::zeros(curMat.size(), curMat.type());
    //Mat tgtMat = Mat::zeros(curMat.size(), CV_8UC3);
    Mat tgtMat;
    cv::cvtColor(curMat, tgtMat, CV_GRAY2BGR);

    Mat tgtMat1;
    cv::cvtColor(curMat, tgtMat1, CV_GRAY2BGR);

    QVector<ObjectDetectResultParas> objectDetectResultVec;
    objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas();



    for (int i=0; i<objectDetectResultVec.size(); i++)
    {
        //对给定的 2D 点集，寻找宣召角度为0的矩形包围圆
        Rect rect;
        rect = objectDetectResultVec[i].rect;

        rectangle( tgtMat,
            rect.tl()-Point(2,2),
            rect.br()+Point(2,2),
            Scalar( 0, 0, 255 ),
            2,        //thickness = -1  代表为 实心的
            8 );

        rectangle( tgtMat1,
            rect.tl()-Point(2,2),
            rect.br()+Point(2,2),
            Scalar( 255, 255, 255 ),
            2,        //thickness = -1  代表为 实心的
            8 );

        //drawContours( tgtMat, contours, i, Scalar(0,0,255), 1);
        cv::putText(tgtMat, QString::number(i).toStdString(), rect.tl()-Point(6,6)
                    , CV_FONT_HERSHEY_SCRIPT_SIMPLEX, 0.4, Scalar(0,255,0));
    }




    QImage tgtImage = cvMat2QImage(tgtMat);
    QString processName = "DrawContour";
    emit signals_deliver_image_getRoi_result_to_central_area(tgtImage);
    emit signals_deliver_little_image_GetRoi(tgtImage, processName);
    emit signals_deliver_little_image_GetRoi(cvMat2QImage(tgtMat1), processName);

}


void ImageProcessWidget::slots_getImage_and_imageDrawContour_AfterDeleteFalseObjProcess(QImage image)      //获取检测图像的 ROI
{
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("添加正确的图片进行处理"));
        return;
    }

    Mat curMat = QImage2cvMat(image);  //这里是进行了位 copy, 后面的函数可能会改变 QImage的值 ==》改成了深拷贝
    if (curMat.channels() != 1)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("被检测的图像通道数必须为1，请载入符合条件的图像"));
        return;
    }

    //Mat tgtMat = Mat::zeros(curMat.size(), curMat.type());
    //Mat tgtMat = Mat::zeros(curMat.size(), CV_8UC3);
    Mat tgtMat;
    cv::cvtColor(curMat, tgtMat, CV_GRAY2BGR);

    Mat tgtMat1;
    cv::cvtColor(curMat, tgtMat1, CV_GRAY2BGR);

    QVector<ObjectDetectResultParas> objectDetectResultVec;
    objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas_afterDeleteFalseObject();



    for (int i=0; i<objectDetectResultVec.size(); i++)
    {
        //对给定的 2D 点集，寻找宣召角度为0的矩形包围圆
        Rect rect;
        rect = objectDetectResultVec[i].rect;

        rectangle( tgtMat,
            rect.tl()-Point(2,2),
            rect.br()+Point(2,2),
            Scalar( 0, 0, 255 ),
            2,        //thickness = -1  代表为 实心的
            8 );

        rectangle( tgtMat1,
            rect.tl()-Point(2,2),
            rect.br()+Point(2,2),
            Scalar( 0, 0, 255 ),
            2,        //thickness = -1  代表为 实心的
            8 );


        //drawContours( tgtMat, contours, i, Scalar(0,0,255), 1);
        cv::putText(tgtMat, QString::number(i).toStdString(), rect.tl()-Point(6,6)
                    , CV_FONT_HERSHEY_SCRIPT_SIMPLEX, 0.4, Scalar(0,255,0));
    }




    QImage tgtImage = cvMat2QImage(tgtMat);
    QString processName = "DrawContour";
    emit signals_deliver_image_getRoi_result_to_central_area(tgtImage);
    emit signals_deliver_little_image_GetRoi(tgtImage, processName);
    emit signals_deliver_little_image_GetRoi(cvMat2QImage(tgtMat1), processName);


}


void ImageProcessWidget::slots_getImage_and_imageDrawContour_AfterPreciseProcess(QImage image)      //获取检测图像的 ROI
{if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("添加正确的图片进行处理"));
        return;
    }

    Mat curMat = QImage2cvMat(image);  //这里是进行了位 copy, 后面的函数可能会改变 QImage的值 ==》改成了深拷贝
    if (curMat.channels() != 1)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("被检测的图像通道数必须为1，请载入符合条件的图像"));
        return;
    }

    //Mat tgtMat = Mat::zeros(curMat.size(), curMat.type());
    //Mat tgtMat = Mat::zeros(curMat.size(), CV_8UC3);
    Mat tgtMat;
    cv::cvtColor(curMat, tgtMat, CV_GRAY2BGR);

    Mat tgtMat1;
    cv::cvtColor(curMat, tgtMat1, CV_GRAY2BGR);

    QVector<ObjectDetectResultParas> objectDetectResultVec;
    objectDetectResultVec = ObjectDetectOptionWidget::getObjectDetectResultParas_afterPrecise();



    for (int i=0; i<objectDetectResultVec.size(); i++)
    {
        //对给定的 2D 点集，寻找宣召角度为0的矩形包围圆
        Rect rect;
        rect = objectDetectResultVec[i].rect;

        rectangle( tgtMat,
            rect.tl()-Point(2,2),
            rect.br()+Point(2,2),
            Scalar( 0, 0, 255 ),
            2,        //thickness = -1  代表为 实心的
            8 );

        rectangle( tgtMat1,
            rect.tl()-Point(2,2),
            rect.br()+Point(2,2),
            Scalar( 0, 0, 255 ),
            2,        //thickness = -1  代表为 实心的
            8 );

        //drawContours( tgtMat, contours, i, Scalar(0,0,255), 1);
        cv::putText(tgtMat, QString::number(i).toStdString(), rect.tl()-Point(6,6)
                    , CV_FONT_HERSHEY_SCRIPT_SIMPLEX, 0.4, Scalar(0,255,0));
    }




    QImage tgtImage = cvMat2QImage(tgtMat);
    QString processName = "DrawContour";
    emit signals_deliver_image_getRoi_result_to_central_area(tgtImage);
    emit signals_deliver_little_image_GetRoi(tgtImage, processName);
    emit signals_deliver_little_image_GetRoi(cvMat2QImage(tgtMat1), processName);


}




void ImageProcessWidget::slots_getImage_and_imageLandDetectProcess(QImage image)      //获取检测图像的 ROI
{
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("添加正确的图片进行处理"));
        return;
    }

    Mat curMat = QImage2cvMat(image);  //这里是进行了位 copy, 后面的函数可能会改变 QImage的值 ==》改成了深拷贝
    if (curMat.channels() != 1)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("被检测的图像通道数必须为1，请载入符合条件的图像"));
        return;
    }

    //Mat tgtMat = Mat::zeros(curMat.size(), curMat.type());
    //Mat tgtMat = Mat::zeros(curMat.size(), CV_8UC3);
    qDebug() << "zhanlgin2";

    Mat tgtMat;

    //对当前图像进行处理
    enum IMAGE_FILTER_TYPE filterType = BOX_FILTER;

    ImageFilter imagefilter = ImageFilter(filterType);
    imagefilter.getImageFilterResult(curMat, tgtMat);

    emit signals_deliver_little_image_landDetect(cvMat2QImage(tgtMat), QString("filter"));  //保存滤波处理后的图像




    cv::threshold( tgtMat, tgtMat, 100, 255, ObjectImageBinary::THRESH_OTSU);

    emit signals_deliver_little_image_landDetect(cvMat2QImage(tgtMat), QString("otsu"));  //保存滤波处理后的图像

    //三种类型分别为MORPH_RECT=0, MORPH_CROSS=1, MORPH_ELLIPSE=2
//    Mat structuringElement = getStructuringElement(0, Size(50, 50));
    Mat structuringElement = getStructuringElement(0, Size(DateMgr::getDataParas()->imageBinaryAndMorphParas->dilateSize
                                                               , DateMgr::getDataParas()->imageBinaryAndMorphParas->erodeSize));

//        morphType = MORPH_CLOSE_OBJ;
//        morphType = MORPH_GRADIENT_OBJ;

    morphologyEx(tgtMat, tgtMat, MORPH_CLOSE_OBJ, structuringElement);
    emit signals_deliver_little_image_landDetect(cvMat2QImage(tgtMat), QString("close"));  //保存滤波处理后的图像


    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

//    enum
//    {
//        CV_RETR_EXTERNAL=0,
//        CV_RETR_LIST=1,
//        CV_RETR_CCOMP=2,    //提取所有轮廓
//        CV_RETR_TREE=3,
//        CV_RETR_FLOODFILL=4
//    };

    //查找轮廓
    findContours( tgtMat, contours, hierarchy,
        CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

    Mat curMat1;
    Mat curMat2;

    cv::cvtColor(curMat, curMat1, CV_GRAY2BGR);
    curMat.copyTo(curMat2);

    float width;   // 宽度和高度
    float height;
    double widthHeightRadio;

    double resolution = DateMgr::getDataParas()->radarParas->resolution;
    int minWidth = DateMgr::getDataParas()->radarParas->minWidth/resolution;
    int maxWidth = DateMgr::getDataParas()->radarParas->maxWidth/resolution;
    int minHeight = DateMgr::getDataParas()->radarParas->minHeight/resolution;
    int maxHeight = DateMgr::getDataParas()->radarParas->maxHeight/resolution;
    double maxWidthHeightRadio =  DateMgr::getDataParas()->radarParas->maxWidthHeightRadio;

    qDebug() << resolution << minWidth << maxWidth << minHeight << maxHeight << maxWidthHeightRadio;

    for (int i=0; i<contours.size(); i++)
    {
        RotatedRect box = minAreaRect(contours[i]);

        width = box.size.width>box.size.height?box.size.width:box.size.height;   // 宽度和高度
        height = box.size.width>box.size.height?box.size.height:box.size.width;   // 宽度和高度
        widthHeightRadio = double(width)/double(height);

        if (!((width>=minWidth && width<=maxWidth)
        && (height>=minHeight && height<=maxHeight)
        && (widthHeightRadio <= maxWidthHeightRadio)))
        {
            //对给定的 2D 点集，寻找宣召角度为0的矩形包围圆
            qDebug() << i << width << height << widthHeightRadio;
            qDebug() << i << minWidth << maxWidth << minHeight << maxHeight << maxWidthHeightRadio;
            drawContours( curMat2, contours, i, Scalar(0,0,0), -1);
        }

        if (width>maxWidth || height>maxHeight)
        {
            drawContours( curMat1, contours, i, Scalar(255,255,255), 3);

        }

    }


    //将处理后的图片 显示在中央和小图窗口中
    QImage tgtImage = cvMat2QImage(curMat1);
    QString processName = "landDelete";

    emit signals_deliver_image_landDetect_result_to_central_area(tgtImage);
    emit signals_deliver_little_image_landDetect(tgtImage, processName);

    tgtImage = cvMat2QImage(curMat2);
    emit signals_deliver_little_image_landDetect(tgtImage, processName);
}




