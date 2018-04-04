#include "imageview.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QHeaderView>
#include "debugInfo.h"
#include "imageprocess.h"
#include <opencv2/opencv.hpp>
#include "datemgr.h"
#include "parametersetting.h"
#include <QDir>



//  QImage::Format 转换成QString

QString format2QString(QImage::Format format)
{
    QString imageFormat;
    switch (format)
    {
        case QImage::Format_Invalid:
            imageFormat = "Format_Invalid";
            break;

        case QImage::Format_Mono:
            imageFormat = "Format_Mono";

            break;

        case QImage::Format_MonoLSB:
            imageFormat = "Format_MonoLSB";

            break;

        case QImage::Format_Indexed8:
            imageFormat = "Format_Indexed8";

            break;

        case QImage::Format_RGB32:
            imageFormat = "Format_RGB32";

            break;

        case QImage::Format_ARGB32:
            imageFormat = "Format_ARGB32";

            break;

        case QImage::Format_ARGB32_Premultiplied:
            imageFormat = "Format_ARGB32_Premultiplied";

            break;

        case QImage::Format_RGB16:
            imageFormat = "Format_RGB16";

            break;

        case QImage::Format_ARGB8565_Premultiplied:
            imageFormat = "Format_ARGB8565_Premultiplied";

            break;

        case QImage::Format_RGB666:
            imageFormat = "Format_RGB666";

            break;

        case QImage::Format_ARGB6666_Premultiplied:
            imageFormat = "Format_ARGB6666_Premultiplied";

            break;

        case QImage::Format_RGB555:
            imageFormat = "Format_RGB555";

            break;

        case QImage::Format_ARGB8555_Premultiplied:
            imageFormat = "Format_ARGB8555_Premultiplied";

            break;

        case QImage::Format_RGB888:
            imageFormat = "Format_RGB888";

            break;

        case QImage::Format_RGB444:
            imageFormat = "Format_RGB444";

            break;

        case QImage::Format_ARGB4444_Premultiplied:
            imageFormat = "Format_ARGB4444_Premultiplied";

            break;

        case QImage::Format_RGBX8888:
            imageFormat = "Format_RGBX8888";

            break;

        case QImage::Format_RGBA8888:
            imageFormat = "Format_RGBA8888";

            break;

        case QImage::Format_RGBA8888_Premultiplied:
            imageFormat = "Format_RGBA8888_Premultiplied";

            break;

        case QImage::Format_BGR30:
            imageFormat = "Format_BGR30";

            break;

        case QImage::Format_A2BGR30_Premultiplied:
            imageFormat = "Format_A2BGR30_Premultiplied";

            break;

        case QImage::Format_RGB30:
            imageFormat = "Format_RGB30";

            break;

        case QImage::Format_A2RGB30_Premultiplied:
            imageFormat = "Format_A2RGB30_Premultiplied";

            break;

        case QImage::Format_Alpha8:
            imageFormat = "Format_Alpha8";

            break;

        case QImage::Format_Grayscale8:
            imageFormat = "Format_Grayscale8";

            break;

        default:
            break;
    }
    return imageFormat;
}

Rect assignSampleObjectSize(Size Original_image_size, Rect target_result_box
                            , Size box_size)
{
    int W_up_num = (box_size.height - target_result_box.height)/2;
    int W_up = target_result_box.y - W_up_num;

    int W_left_num = (box_size.width - target_result_box.width)/2;
    int W_left = target_result_box.x - W_left_num;

    if (W_up <= 0)
    {
        W_up = 1;
    }

    if (W_left <= 0)
    {
        W_left = 1;
    }


    int W_down = W_up + box_size.height - 1;
    int W_right = W_left + box_size.width -1;


    if (W_down >= Original_image_size.height)
    {
        W_down = Original_image_size.height;
    }

    if (W_right >= Original_image_size.width)
    {
        W_right = Original_image_size.width;
    }

    if (W_right-W_left+1 != box_size.width)
    {
        W_left = W_right-box_size.width+1;
    }

    if (W_down-W_up+1 != box_size.height)
    {
        W_up = W_down- box_size.height +1;
    }

    Rect target_box(W_left, W_up, box_size.width, box_size.height);
    return target_box;
}




QImage  QImageWidget::curImage;
QImage QImageWidget::getCentralQImage()
{
    return curImage;
}


QImageWidget::QImageWidget(QWidget* parent):QWidget(parent)
{
    setBaseSize(QSize(100,100));        // 设置窗口大小
    this->setMinimumSize(600, 600);
    this->setMouseTracking(true);
    this->setAcceptDrops(true);       //支持拖放功能

    mRightKeyStartedMove = false;  // 右键开始移动
    mLeftKeyStartedMove = false;   // 左键开始移动
    imageUpdated = false;          // 初始显示为假
}

void QImageWidget::paintEvent(QPaintEvent* e)
{
    if (imageUpdated)  //当curImage 和 curQPixmap中有值后，显示为真
    {
        QPainter painter(this);
        QRectF srcRect(0, 0, curPixmap.width(), curPixmap.height());
        painter.drawPixmap(tgtRect, curPixmap, srcRect); //将原始图像curPixma中srcRect区域 画到图像tgtRect中
    }
    
    if (mRightKeyStartedMove)  //右键按下去了
    {
        this->slot_imageCut();
    }
}


//当窗口发生改变的时候，必须改变目标窗口的尺寸值   tgtRect
void QImageWidget::resizeEvent(QResizeEvent *e)
{
    qDebug() << "QImageWidget::resizeEvent(QResizeEvent *e)";
    this->slot_imageRefresh();
}



//直接传入 QImage 图像后， 显示图像
bool QImageWidget::setImage(const QImage& qImage)
{
    qDebug() << "QImageWidget::setImage(const QImage& qImage)";
    if(qImage.isNull()) //传入的 qImage 错误
    {
        QMessageBox::warning(this, "warning", QStringLiteral("载入图片有误，请输入正确格式的图像文件"));
        return false;
    }

    // 将 QImage 转换成 QPixmap 图像
    curImage = qImage;
    qDebug() << curImage.format();
    curPixmap = QPixmap::fromImage(curImage);  //将QImage 转化成 QPixmap

    if (curPixmap.isNull() || curImage.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("载入图片有误，请输入正确格式的图像文件"));
        return false;
    }

    imageUpdated = true;  // 当curPixmap 和 curImage 当有值后，显示为真
    this->slot_imageRefresh();
    update();             //第一次调用update函数，将图像显示出来



    // 添加图片的描述信息  start
    Mat curMat = QImage2cvMat(curImage);
    ImageDescriptionParas::getImageDescriptionParas()->imageChannels = curMat.channels();
    ImageDescriptionParas::getImageDescriptionParas()->imageSize = curImage.size();
    ImageDescriptionParas::getImageDescriptionParas()->imageDepth = curImage.depth();
    ImageDescriptionParas::getImageDescriptionParas()->format = curImage.format();

    qDebug() << "imagePath: " << ImageDescriptionParas::getImageDescriptionParas()->imagePath;
//    qDebug() << "imageSize: " << ImageDescriptionParas::getImageDescriptionParas()->imageSize;
//    qDebug() << "imageDepth: " << ImageDescriptionParas::getImageDescriptionParas()->imageDepth;
//    qDebug() << "format: " << format2QString(ImageDescriptionParas::getImageDescriptionParas()->format);
//   qDebug() << "imageChannels: " << ImageDescriptionParas::getImageDescriptionParas()->imageChannels;
    // 添加图片的描述信息  end
    emit signals_imageDescriptionParas();


    return true;
}


// 通过载入图片的路径，来显示图片
bool QImageWidget::setImage(const QString& imgFile)
{
    qDebug() << "QImageWidget::setImage(const QString& imgFile)";
    if (imgFile.isEmpty())  //文件为空 直接返回
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径有误，请输入正确的文件名称"));
        return false;
    }

//    Mat curMat = cv::imread(imgFile.toStdString(), 1);
//    curImage = cvMat2QImage(curMat);

//    if (curImage.isNull())
//    {
//        QMessageBox::warning(this, "warning", QStringLiteral("载入图像有误，请输入正确的文件名称"));
//        return false;
//    }


    bool isLoadSuccess = curImage.load(imgFile);  //判断是否加载文件成功

    //curImage = curImage.convertToFormat(QImage::Format_RGBA8888);


    if (!isLoadSuccess)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("载入图像有误，请输入正确的文件名称"));
        return false;
    }

    // 将 QImage 转换成 QPixmap 图像
    curPixmap = QPixmap::fromImage(curImage);  //将QImage 转化成 QPixmap

    if (curPixmap.isNull() || curImage.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("载入图像有误，请输入正确的文件名称"));
        return false;
    }

    imageFile = imgFile;

    imageUpdated = true; // 当curPixmap 和 curImage 当有值后，显示为真
    this->slot_imageRefresh();
    update();            //第一次调用update函数，将图像显示出来



    // 添加图片的描述信息  start

    // 加载图片的时候，设置图片的通道数
    Mat curMat = QImage2cvMat(curImage);
    ImageDescriptionParas::getImageDescriptionParas()->imageChannels = curMat.channels();

    ImageDescriptionParas::getImageDescriptionParas()->imagePath = imageFile;   //2/6 添加了文件的路径
    ImageDescriptionParas::getImageDescriptionParas()->imageSize = curImage.size();
    ImageDescriptionParas::getImageDescriptionParas()->imageDepth = curImage.depth();
    ImageDescriptionParas::getImageDescriptionParas()->format = curImage.format();

//    qDebug() << "imagePath: " << ImageDescriptionParas::getImageDescriptionParas()->imagePath;
//    qDebug() << "imageSize: " << ImageDescriptionParas::getImageDescriptionParas()->imageSize;
//    qDebug() << "imageDepth: " << ImageDescriptionParas::getImageDescriptionParas()->imageDepth;
//    qDebug() << "format: " << format2QString(ImageDescriptionParas::getImageDescriptionParas()->format);
//   qDebug() << "imageChannels: " << ImageDescriptionParas::getImageDescriptionParas()->imageChannels;
    // 添加图片的描述信息  end

    return true;
}

void QImageWidget::wheelEvent(QWheelEvent *e)
{
    float scale = tgtRect.width()/(float)width()>tgtRect.height()/(float)height()?
                tgtRect.width()/(float)width():tgtRect.height()/(float)height();
    float scaleDelta = std::exp(e->delta()*0.001);
    scale = scaleDelta * scale;

    QPointF newDxy = QPointF(e->x(), e->y()) - QPointF(e->x()-tgtRect.x(),e->y()-tgtRect.y())*scaleDelta;
    tgtRect = QRectF(newDxy.x(), newDxy.y(), tgtRect.width()*scaleDelta, tgtRect.height()*scaleDelta);

//    if(scale<1)
//    {
//        this->slot_imageRefresh();
//    }
    update();
}



void QImageWidget::mousePressEvent(QMouseEvent* e)
{
    qDebug() << "mousePressEvent(QMouseEvent* e)";
    if (e->button() == Qt::LeftButton)          //左键 按下
    {
        this->setCursor(Qt::SizeAllCursor);      //设置鼠标： 拖动效果
        mLeftKeyStartedMove = true;
        mStartPosition = e->pos();
    }

    if (e->button() == Qt::RightButton)           //y右键 按下
    {
        this->setCursor(Qt::CrossCursor);        // 设置鼠标： 截图效果
        mRightKeyStartedMove = true;
        mStartPosition = e->pos();
    }
}



void QImageWidget::mouseMoveEvent(QMouseEvent* e)
{
    //qDebug() << "QImageWidget::mouseMoveEvent(QMouseEvent* e)";
    //qDebug() << e->pos();

    if (mLeftKeyStartedMove)   // 左键 实现了拖动
    {
        QPoint mousePoint = e->pos();
        int y_offset = mousePoint.y() - mStartPosition.y();
        int x_offset = mousePoint.x() - mStartPosition.x();

        tgtRect = QRectF(tgtRect.x() + x_offset, tgtRect.y() + y_offset
                       , tgtRect.width(),tgtRect.height());
        update();
        mStartPosition = mousePoint;
    }

    if (mRightKeyStartedMove)    // 右键 实现了截图
    {
        mEndPosition = e->pos();
        update();
    }


    //get pixValue
    QPoint curPoint;
    QVector<uchar> curPixValue;
    curNewMat = QImage2cvMat(curImage);
    get_CurQImage_pixPos_pixValue(e, curPoint, curPixValue);

    emit signals_deliver_CurQImage_pixPos_pixValue(curPoint, curPixValue);
}

void QImageWidget::get_CurQImage_pixPos_pixValue(QMouseEvent* event,QPoint& point, QVector<uchar>& pixValue)
{
    if (curImage.isNull())
    {
        return;
    }
    pixValue.clear();

    QPoint mousePoint = event->pos();
    int x = static_cast<int>((mousePoint.x() - tgtRect.x())/tgtRect.width() * curImage.width());
    int y = static_cast<int>((mousePoint.y() - tgtRect.y())/tgtRect.height() * curImage.height());
    point.setX(x);
    point.setY(y);


    if(x>=0 && x<curNewMat.cols && y>=0 && y< curNewMat.rows)
    {

        if (curNewMat.channels() == 1)
        {
            //qDebug() << "curNewMat.channels(): " << curNewMat.channels();
            pixValue.push_back(curNewMat.at<uchar>(y, x));
        }
        else if(curNewMat.channels() == 3)
        {
            //qDebug() << "curNewMat.channels(): " << curNewMat.channels();

            pixValue.push_back(curNewMat.at<Vec3b>(y, x)[0]);
            pixValue.push_back(curNewMat.at<Vec3b>(y, x)[1]);
            pixValue.push_back(curNewMat.at<Vec3b>(y, x)[2]);

        }
        else if (curNewMat.channels() == 4)
        {
            //qDebug() << "curNewMat.channels(): " << curNewMat.channels();
            pixValue.push_back(curNewMat.at<Vec4b>(y, x)[0]);
            pixValue.push_back(curNewMat.at<Vec4b>(y, x)[1]);
            pixValue.push_back(curNewMat.at<Vec4b>(y, x)[2]);
            pixValue.push_back(curNewMat.at<Vec4b>(y, x)[3]);
        }
    }
}




void QImageWidget::mouseReleaseEvent(QMouseEvent* e)
{
    qDebug() << "mouseReleaseEvent(QMouseEvent* e)";
    mRightKeyStartedMove = false ;
    mLeftKeyStartedMove = false;
    this->setCursor(Qt::ArrowCursor);
    mEndPosition = e->pos();
}

void QImageWidget::keyPressEvent(QKeyEvent* e)
{
    switch (e->key())
    {

        case Qt::Key_Enter:
            qDebug() << "keyPressEvent(QKeyEvent* e)";


            slot_imageCut_emit_signals();

            break;
        default:
            break;
    }
}

//void QImageWidget::dragEnterEvent(QDragEnterEvent *event)
//{
//    qDebug() << "QLeftImageWidget::dragEnterEvent(QDragEnterEvent *event)";

//    QList<QUrl> urls = event->mimeData()->urls();
//    if (!urls.empty())
//    {
//        event->acceptProposedAction();
//    }
//}

//void QImageWidget::dropEvent(QDropEvent * event)
//{
//    QList<QUrl> urls = event->mimeData()->urls();
//    QString path = urls.first().toLocalFile();

//    if (path.isEmpty())
//    {
//        return;
//    }

//    this->setImage(path);
//}


void QImageWidget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "QImageWidget::dragEnterEvent(QDragEnterEvent *event)";


        event->acceptProposedAction();

}

void QImageWidget::dropEvent(QDropEvent * event)
{
    qDebug() << "QImageWidget::dropEvent(QDropEvent * event)";
    QImage image = event->mimeData()->imageData().value<QImage>();

    if (image.isNull())
    {
        qDebug() << "isNUll";

        return;
    }

    this->setImage(image);
}


void QImageWidget::slot_imageCut()   //槽函数 实现截图 的功能
{
    
    qDebug() << "QImageWidget::slot_imageCut()" << endl;

    QPoint beginPoint = mStartPosition;
    QPoint endPoint = mEndPosition;

    int x, y, width, height;
    width = qAbs(beginPoint.x() - endPoint.x());
    height = qAbs(beginPoint.y() - endPoint.y());
    x = beginPoint.x() < endPoint.x() ? beginPoint.x() : endPoint.x();
    y = beginPoint.y() < endPoint.y() ? beginPoint.y() : endPoint.y();

    QRect selectedRect = QRect(x, y, width, height);
    // 避免宽或高为零时拷贝截图有误;
    // 可以看QQ截图，当选取截图宽或高为零时默认为2;
    if (selectedRect.width() == 0)
    {
            selectedRect.setWidth(1);
    }
    if (selectedRect.height() == 0)
    {
            selectedRect.setHeight(1);
    }

    //开始重新绘制图像
    QPainter m_painter(this);
    m_painter.setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::FlatCap));    //设置画笔;
    m_painter.drawRect(selectedRect);

    int realX = static_cast<int>((x - tgtRect.x())/tgtRect.width() * curImage.width());
    int realY = static_cast<int>((y - tgtRect.y())/tgtRect.height() * curImage.height());
    int realWidth = static_cast<int>(width/tgtRect.width() * curImage.width());
    int realHeight = static_cast<int>(height/tgtRect.height() * curImage.height());

    QString descriptStr =  "x = " + QString::number(realX) + " " +
            + "y" + " = " + QString::number(realY) + "\n"
            + "width = " + QString::number(realWidth) + " " +
            + "height" + " = " + QString::number(realHeight);

    QRect descriptRect = QRect(x+20, y-70, 200, 60);
    m_painter.drawText(descriptRect, descriptStr);

    captureRectF = selectedRect;
}


void QImageWidget::slot_imageCut_emit_signals()   //截图后 发送信号
{
    // 计算出 截图区域 并保存
    qDebug() << "QImageWidget::slot_imageCut_emit_signals()";
    qDebug() << captureRectF;
    qDebug() << tgtRect;

    int x = static_cast<int>((captureRectF.x() - tgtRect.x())/tgtRect.width() * curImage.width());
    int y = static_cast<int>((captureRectF.y() - tgtRect.y())/tgtRect.height() * curImage.height());
    int width = static_cast<int>(captureRectF.width()/tgtRect.width() * curImage.width());
    int height = static_cast<int>(captureRectF.height()/tgtRect.height() * curImage.height());

    QRect tgtCutRect = QRect(x, y, width, height);

    captureQImage = curImage.copy(tgtCutRect);
    emit signals_deliver_captureQImage(captureQImage);

}

void QImageWidget::slot_littleImage_emit_signals()   //缩略图 发送信号
{
    emit signals_deliver_little_QPixMap(getQImage());
}

void QImageWidget::slot_curQImage_emit_signals_to_filter()     //获得当前的QImage并发射出去
{
    emit signals_deliver_curQImage_to_filter(getQImage());
}

void QImageWidget::slot_curQImage_emit_signals_to_binary()     //获得当前的QImage并发射出去
{
    emit signals_deliver_curQImage_to_binary(getQImage());
}

void QImageWidget::slot_curQImage_emit_signals_to_morph()     //获得当前的QImage并发射出去
{
    emit signals_deliver_curQImage_to_morph(getQImage());
}

void QImageWidget::slot_curQImage_emit_signals_to_cfarDetect()     //获得当前的QImage并发射出去
{
    emit signals_deliver_curQImage_to_cfarDetect(getQImage());
}

void QImageWidget::slot_curQImage_emit_signals_to_hog_svm()          //获得当前的QImage并发射出去
{
    emit signals_deliver_curQImage_to_hog_svm(getQImage());
}


void QImageWidget::slot_curQImage_emit_signals_to_getRoi()           //发送当前窗口中的QImage
{
    emit signals_deliver_curQImage_to_getRoi(getQImage());
}

void QImageWidget::slot_curQImage_emit_signals_to_landDelete()           //发送当前窗口中的QImage
{
    emit signals_deliver_curQImage_to_landDelete(getQImage());
}



void QImageWidget::slot_curQImage_emit_signals_to_drawContour()           //发送当前窗口中的QImage
{
    emit signals_deliver_curQImage_to_drawContour(getQImage());
}

void QImageWidget::slot_curQImage_emit_signals_to_drawContour_afterDeleteFalseObj()           //发送当前窗口中的QImage
{
    emit signals_deliver_curQImage_to_drawContour_afterDeleteFalseObj(getQImage());
}

void QImageWidget::slot_curQImage_emit_signals_to_drawContour_afterPrecise()           //发送当前窗口中的QImage
{
    emit signals_deliver_curQImage_to_drawContour_afterPrecise(getQImage());
}



void QImageWidget::slot_listItemDoubleClicked(QImage image)
{
    this->setImage(image);
//    if (image.isNull())
//    {
//        QMessageBox::warning(this, "warning", QStringLiteral("载入缩略图有误，请重新输入正确的缩略图"));
//        return;
//    }

//    curImage = image;
//    curPixmap = QPixmap::fromImage(curImage);
//    imageUpdated = true;        // 当curPixmap 和 curImage 当有值后，显示为真
//    this->slot_imageRefresh();
//    update();                   //第一次调用update函数，将图像显示出来
}

//接受图像处理的结果，并进行显示
void QImageWidget::slot_receive_image_process_result_to_central_area(QImage image)
{
    this->setImage(image);
//    if (image.isNull())
//    {
//        QMessageBox::warning(this, "warning", QStringLiteral("载入缩略图有误，请重新输入正确的缩略图"));
//        return;
//    }

//    curImage = image;
//    curPixmap = QPixmap::fromImage(image);
//    imageUpdated = true;        // 当curPixmap 和 curImage 当有值后，显示为真
//    this->slot_imageRefresh();
//    update();                   //第一次调用update函数，将图像显示出来
}



/*
该刷新图像的功能为：获取目标图像的显示窗口尺寸
1、目标图像中 宽/高 比例 与原始图像保持一样
2、重新获得目标图像的 左上角区域
*/

//注意：由于针对图像，有各种不同的操作，每一次操作都会间接的 改变tgtRect的值，所以必须使用一个成员变量

void QImageWidget::slot_imageRefresh()           // 刷新图像
{
    qDebug() << "QImageWidget::slot_imageRefresh(): \n";

    float ww = this->width(), wh = this->height();
    float iw = curPixmap.width(), ih = curPixmap.height();

    qDebug() << ww << wh << iw << ih;

    float dx=0,dy=0;

    //减少目标显示窗口的宽度或者高度，时刻保持显示的图片 宽高比例一样
    if(ww/wh > iw/ih)
    {
        ww = iw*wh/ih;
        dx= (this->width()-ww)/2;
    }
    else
    {
        wh = ih*ww/iw;
        dy = (this->height()-wh)/2;
    }


    tgtRect = QRectF(dx, dy, ww, wh);    //ww/wh窗口的比例始终是与原始图片比例一样
    update();
}

// 加载图片文件
bool QImageWidget::slot_loadImageFile()
{
    qDebug() << "QImageWidget::slot_loadImageFile()";
    QString imageFile = QFileDialog::getOpenFileName(this
                , QStringLiteral("载入图片文件"), "./image"
                , QStringLiteral("*.jpg; *.png; *.tif; *.tiff; *.bmp"));


    return this->setImage(imageFile);
}



// 保存图片文件
void QImageWidget::slot_saveImageFile()
{
    qDebug() << "QImageWidget::slot_saveImageFile()";

    QString imageFile = QFileDialog::getSaveFileName(this
                , QStringLiteral("载入图片文件"), "./detect_result_image"
                , QStringLiteral("*.jpg; *.png; *.tif; *.tiff; *.bmp"));


    if (imageFile.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径有误，请输入正确的图片文件保存路径"));
        return ;
    }

    curImage.save(imageFile, 0, 100);

//    Mat dstMat = QImage2cvMat(curImage);
//    cv::imwrite(imageFile.toStdString(), dstMat);
}


// 实现图片的放大功能
void QImageWidget::slot_zoomin()
{

    qDebug() << "QImageWidget::slot_zoomin()";

    float scaleDelta = float(1.05);
    float xCenter = tgtRect.x() + tgtRect.width()/2;
    float yCenter = tgtRect.y() + tgtRect.height()/2;



    QPointF newDxy = QPointF(xCenter, yCenter)    -      QPointF(xCenter- tgtRect.x()
                            ,yCenter - tgtRect.y())*scaleDelta;

    tgtRect = QRectF(newDxy.x(), newDxy.y()
                    ,tgtRect.width()*scaleDelta, tgtRect.height()*scaleDelta);

    update();
}

// 实现图片的缩小功能
void QImageWidget::slot_zoomout()
{
    qDebug() << "QImageWidget::slot_zoomout()";

    float scaleDelta = (float)1.2;
    float xCenter = tgtRect.x() + tgtRect.width()/2;
    float yCenter = tgtRect.y() + tgtRect.height()/2;



    QPointF newDxy = QPointF(xCenter, yCenter)    -      QPointF(xCenter- tgtRect.x()
                            ,yCenter - tgtRect.y())/scaleDelta;

    tgtRect = QRectF(newDxy.x(), newDxy.y()
                    ,tgtRect.width()/scaleDelta, tgtRect.height()/scaleDelta);

    update();

}

void QImageWidget::slot_oneKeyProcessing()        // 图片一键处理
{
    qDebug() << "QImageWidget::slot_oneKeyProcessing()";

}

void QImageWidget::slot_processingExit()          // 程序退出功能
{
    qDebug() << "QImageWidget::slot_processingExit()";


}


QImage QImageWidget::getQImage()
{
    if (this->curImage.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("中央图像区域，图像为空，请载入图像"));
    }

    return curImage;
}

QPixmap QImageWidget::getQPixmap()
{
    if (this->curPixmap.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("中央图像区域，图像为空，请载入图像"));
    }

    return curPixmap;
}






/*
新建一个窗口用来显示读图，然后保存效果

*/
CutImageShowWidget::CutImageShowWidget(QWidget* parent):QImageWidget(parent)
{
    this->setMinimumSize(64, 64);
}


bool CutImageShowWidget::setImage(const QImage& qImage)
{
    qDebug() << "QImageWidget::setImage(const QImage& qImage)";
    if(qImage.isNull()) //传入的 qImage 错误
    {
        QMessageBox::warning(this, "warning", QStringLiteral("载入图片有误，请输入正确格式的图像文件"));
        return false;
    }

    // 将 QImage 转换成 QPixmap 图像

    curPixmap = QPixmap::fromImage(qImage);  //将QImage 转化成 QPixmap

    if (curPixmap.isNull() || curImage.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("载入图片有误，请输入正确格式的图像文件"));
        return false;
    }

    imageUpdated = true;  // 当curPixmap 和 curImage 当有值后，显示为真
    this->slot_imageRefresh();
    update();             //第一次调用update函数，将图像显示出来

    // 添加图片的描述信息  start
    Mat curMat = QImage2cvMat(curImage);
    ImageDescriptionParas::getImageDescriptionParas()->imageChannels = curMat.channels();
    ImageDescriptionParas::getImageDescriptionParas()->imageSize = curImage.size();
    ImageDescriptionParas::getImageDescriptionParas()->imageDepth = curImage.depth();
    ImageDescriptionParas::getImageDescriptionParas()->format = curImage.format();

    qDebug() << "imagePath: " << ImageDescriptionParas::getImageDescriptionParas()->imagePath;
//    qDebug() << "imageSize: " << ImageDescriptionParas::getImageDescriptionParas()->imageSize;
//    qDebug() << "imageDepth: " << ImageDescriptionParas::getImageDescriptionParas()->imageDepth;
//    qDebug() << "format: " << format2QString(ImageDescriptionParas::getImageDescriptionParas()->format);
//   qDebug() << "imageChannels: " << ImageDescriptionParas::getImageDescriptionParas()->imageChannels;
    // 添加图片的描述信息  end
    emit signals_imageDescriptionParas();


    return true;
}

//屏蔽掉父类的  各种鼠标事件（目的：该对象只用只读，显示图片对象）
void CutImageShowWidget::mousePressEvent(QMouseEvent* e){}
void CutImageShowWidget::mouseMoveEvent(QMouseEvent* e){}
void CutImageShowWidget::mouseReleaseEvent(QMouseEvent* e){}
void CutImageShowWidget::keyPressEvent(QKeyEvent* e){}
void CutImageShowWidget::wheelEvent(QWheelEvent *e){}


CutImageShowWnd::CutImageShowWnd(QWidget* parent):QWidget(parent)
{
    this->createControls();
    this->controlsLayout();

    connect(saveButton, SIGNAL(clicked()),
            this, SLOT(slot_save_cutImage()));
}


void CutImageShowWnd::createControls()
{
    imageWidget = new CutImageShowWidget(NULL);
//    QImage qImage("./detect_object_image/litterImage_500_600.jpg");
//    imageWidget->setImage(qImage);
    imageTab = new QTabWidget;
    imageTab->addTab(imageWidget, QStringLiteral("图片显示区："));

    saveButton = new QPushButton(QStringLiteral("保存截图"));
    imageSizeName = new QLabel(QStringLiteral("截图尺寸： "));
    imageSizeLabel = new QLabel();




    saveImageLabel = new QLabel(QStringLiteral("保存图片的尺寸： "));
    imageWidthName = new QLabel(QStringLiteral("图片宽度： "));
    imageHeightName = new QLabel(QStringLiteral("图片高度： "));
    imageWidth = new QSpinBox;
    imageHeight = new QSpinBox;
    imageWidth->setValue(64);
    imageHeight->setValue(64);
    imageWidth->setMaximum(10000000);
    imageHeight->setMaximum(10000000);

    hlayout = new QHBoxLayout;
    layout = new QGridLayout;
    saveBox = new QGroupBox;
    vlayout = new QVBoxLayout;
    saveBox1 = new QGroupBox;
    vlayout1 = new QVBoxLayout;
    groupBox1 = new QGroupBox(this);
}

void CutImageShowWnd::controlsLayout()
{

    hlayout->addWidget(imageSizeName);
    hlayout->addWidget(imageSizeLabel);

    layout->addWidget(saveImageLabel, 0, 0);
    layout->addWidget(imageWidthName, 1, 0);
    layout->addWidget(imageHeightName, 2, 0);
    layout->addWidget(imageWidth, 1, 1 );
    layout->addWidget(imageHeight, 2, 1);
    layout->addWidget(saveButton, 3, 0, 1, 2);

    saveBox->setLayout(layout);

    vlayout->addLayout(hlayout);
    vlayout->addWidget(saveBox);
    vlayout->setSpacing(5);

    saveBox1->setLayout(vlayout);

    vlayout1->addWidget(imageTab);
    vlayout1->addWidget(saveBox1);
    groupBox1->setLayout(vlayout1);


    QGridLayout * mainLayout = new QGridLayout;
    mainLayout->addWidget(groupBox1);
    this->setLayout(mainLayout);
}



void CutImageShowWnd::slot_show_cutImage(QImage qImage)
{
    imageWidget->setImage(qImage);

    QString imageSizeStr = QString::number(qImage.width()) + " * " + QString::number(qImage.height());
    imageSizeLabel->setText(imageSizeStr);
}



void CutImageShowWnd::slot_save_cutImage()
{

    qDebug() << "CutImageShowWnd::slot_save_cutImage()";

    QString imageFile = QFileDialog::getSaveFileName(this
                , QStringLiteral("载入图片文件"), "./detect_result_image"
                , QStringLiteral("*.jpg; *.png; *.tif; *.tiff; *.bmp"));


    if (imageFile.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径有误，请输入正确的图片文件保存路径"));
        return ;
    }


    int saveWidth = imageWidth->value();
    int saveHeight = imageHeight->value();


    QPixmap image = imageWidget->getQPixmap().scaled(saveWidth, saveHeight);

    //此处保存的截图 没有实现 resize的功能
    image.save(imageFile);
}


//2/6 添加图片 描述参数的窗口
ImageDescriptionParas::ImageDescriptionParas(){}
ImageDescriptionParas::ImageDescriptionParas(const ImageDescriptionParas&){}
ImageDescriptionParas& ImageDescriptionParas::operator=(const ImageDescriptionParas*){return *this;}
ImageDescriptionParas::~ImageDescriptionParas(){}


QMutex ImageDescriptionParas::mutex;
ImageDescriptionParas* ImageDescriptionParas::instance = NULL;

ImageDescriptionParas* ImageDescriptionParas::getImageDescriptionParas()
{
    if (instance == NULL)
    {
        QMutexLocker locker(&mutex);  //add mutex locker

        if (instance == NULL)
        {
            instance = new ImageDescriptionParas;
        }
    }
    return instance;
}



// 2/28 添加目标检测输出信息窗口
ObjectDetect_OutputInfo::ObjectDetect_OutputInfo(QWidget* parent):QWidget(parent)
{
    init();
}

void ObjectDetect_OutputInfo::init()
{
    svmPredict_OutputInfo_Edit = new QTextEdit;

    hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(svmPredict_OutputInfo_Edit);
    mainGroupBox1 = new QGroupBox;
    mainGroupBox1->setTitle(QStringLiteral("目标检测信息输出："));
    mainGroupBox1->setLayout(hLayout1);

    mainLayout = new QGridLayout;
    mainLayout->addWidget(mainGroupBox1);
    this->setLayout(mainLayout);
}



//2/6 添加图片的描述窗口

ImageDescriptionWidget::ImageDescriptionWidget(QWidget* parent):QWidget(parent)
{
    initTableWidget();

    initTableWidgetItem();
}



void ImageDescriptionWidget::initTableWidget()
{
    tableWidget = new QTableWidget;
    tableWidget->setColumnCount(2);  //设置成两列
    //tableWidget->setFrameShape(QFrame::NoFrame);        //设置tableWidget为无边框
    tableWidget->verticalHeader()->setVisible(false);   //设置垂直的 头部说明 不可见
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList header;
    header << QStringLiteral("图片描述") << QStringLiteral("值");
    tableWidget->setHorizontalHeaderLabels(header);

    hLayout = new QHBoxLayout;
    hLayout->addWidget(tableWidget);
    mainGroupBox = new QGroupBox;
    mainGroupBox->setTitle(QStringLiteral("图片描述区"));
    mainGroupBox->setLayout(hLayout);

    svmPredict_OutputInfo_Edit = new QTextEdit;

    hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(svmPredict_OutputInfo_Edit);
    mainGroupBox1 = new QGroupBox;
    mainGroupBox1->setTitle(QStringLiteral("目标检测信息输出："));
    mainGroupBox1->setLayout(hLayout1);

    mainLayout = new QGridLayout;
    mainLayout->addWidget(mainGroupBox, 0, 0);
    mainLayout->addWidget(mainGroupBox1, 1, 0);


    this->setLayout(mainLayout);
}

//init
void ImageDescriptionWidget::initTableWidgetItem()
{
    QStringList paraNameList;
    paraNameList << QStringLiteral("图片路径") << QStringLiteral("图片尺寸")
                 << QStringLiteral("图片格式") << QStringLiteral("图片通道数")
        << QStringLiteral("图片深度") << QStringLiteral("图片像素位置")
        << QStringLiteral("图片像素值");


    tableWidget->setRowCount(paraNameList.size());
    for (int i = 0; i < paraNameList.size(); i++)
    {
        QLabel *plabel = new QLabel("");

        tableWidget->setItem(i, 0, new QTableWidgetItem(paraNameList[i]));
        tableWidget->setCellWidget(i, 1, plabel);
    }
    tableWidget->setCellWidget(0, 1, new QLineEdit(""));
}


// receive image Description
void ImageDescriptionWidget::slots_imageDescriptionParas()
{
    QVector<QLabel *> labelVec;
    for (int i = 0; i < tableWidget->rowCount(); i++)
    {
        labelVec.push_back(dynamic_cast<QLabel *>(tableWidget->cellWidget(i, 1)));
    }

    QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(tableWidget->cellWidget(0, 1));

    lineEdit->setText(ImageDescriptionParas::getImageDescriptionParas()->imagePath);


    QString imageSizeQString;
    imageSizeQString = QString::number(ImageDescriptionParas::getImageDescriptionParas()->imageSize.width()) + " * "
            +   QString::number(ImageDescriptionParas::getImageDescriptionParas()->imageSize.height());

    labelVec[1]->setText(imageSizeQString);

    labelVec[2]->setText(format2QString(ImageDescriptionParas::getImageDescriptionParas()->format));

    labelVec[3]->setText(QString::number(ImageDescriptionParas::getImageDescriptionParas()->imageChannels));
    labelVec[4]->setText(QString::number(ImageDescriptionParas::getImageDescriptionParas()->imageDepth));
}



void ImageDescriptionWidget::slots_receive_CurQImage_pixPos_pixValue(QPoint point, QVector<uchar> pixValue)
{
//    qDebug() << "ImageDescriptionWidget::slots_receive_CurQImage_pixPos_pixValue" ;
//    qDebug() << point;
//    qDebug() << pixValue;


    QVector<QLabel *> labelVec;
    for (int i = 0; i < tableWidget->rowCount(); i++)
    {
        labelVec.push_back(dynamic_cast<QLabel *>(tableWidget->cellWidget(i, 1)));
    }


    QString imagePixPos = "[" + QString::number(point.x())
            + " , " + QString::number(point.y()) + "]";
    labelVec[5]->setText(imagePixPos);

    QString imagePixValue;
    if (pixValue.size() == 1)
    {
        imagePixValue = "[" + QString::number(pixValue[0]) + "]";
    }
    else if (pixValue.size() == 3)
    {
        imagePixValue = "[" + QString::number(pixValue[0])
                +"," + QString::number(pixValue[1])
                + "," + QString::number(pixValue[2]) + "]";
    }
    else if(pixValue.size() == 4)
    {
        imagePixValue = "[" + QString::number(pixValue[0])
                +"," + QString::number(pixValue[1])
                + "," + QString::number(pixValue[2])
                + "," + QString::number(pixValue[3])
                + "]";
    }


    labelVec[6]->setText(imagePixValue);

}

//2/28 添加目标检测的输出信息
void ImageDescriptionWidget::slot_ObjectDetect_Output_Info(QString string)
{
    svmPredict_OutputInfo_Edit->append(string);
    svmPredict_OutputInfo_Edit->repaint();
}







//截图 所有的图片  保存区域

CutAllImageShowWnd::CutAllImageShowWnd(QWidget* parent):QWidget(parent)
{
    this->createControls();
    this->controlsLayout();

    connect(saveButton, SIGNAL(clicked()),
            this, SLOT(slots_save_All_little_images()));   //saveButton  save all images
}


void CutAllImageShowWnd::createControls()
{
    imageWidget = new ClitImgListWidget;
//    QImage qImage("./detect_object_image/litterImage_500_600.jpg");
//    imageWidget->setImage(qImage);
//    imageTab = new QTabWidget;
//    imageTab->addTab(imageWidget, QStringLiteral("图片显示区："));

    saveButton = new QPushButton(QStringLiteral("保存截图"));
//    imageSizeName = new QLabel(QStringLiteral("截图尺寸： "));
//    imageSizeLabel = new QLabel();

    saveImageLabel = new QLabel(QStringLiteral("保存图片的尺寸： "));
    imageWidthName = new QLabel(QStringLiteral("图片宽度： "));
    imageHeightName = new QLabel(QStringLiteral("图片高度： "));
    imageWidth = new QSpinBox;
    imageHeight = new QSpinBox;
    imageWidth->setValue(64);
    imageHeight->setValue(64);
    imageWidth->setMaximum(10000000);
    imageHeight->setMaximum(10000000);

    hlayout = new QHBoxLayout;
    layout = new QGridLayout;
    saveBox = new QGroupBox;
    vlayout = new QVBoxLayout;
    //saveBox1 = new QGroupBox;
    //vlayout1 = new QVBoxLayout;
    groupBox1 = new QGroupBox(this);
}

void CutAllImageShowWnd::controlsLayout()
{

//    hlayout->addWidget(imageSizeName);
//    hlayout->addWidget(imageSizeLabel);

    layout->addWidget(saveImageLabel, 0, 0);
    layout->addWidget(imageWidthName, 1, 0);
    layout->addWidget(imageHeightName, 2, 0);
    layout->addWidget(imageWidth, 1, 1 );
    layout->addWidget(imageHeight, 2, 1);
    layout->addWidget(saveButton, 3, 0, 1, 2);

    saveBox->setLayout(layout);

//    vlayout->addLayout(hlayout);
//    vlayout->addWidget(saveBox);
//    vlayout->setSpacing(5);

//    saveBox1->setLayout(vlayout);

    vlayout->addWidget(imageWidget);
    vlayout->addWidget(saveBox);
    groupBox1->setLayout(vlayout);


    QGridLayout * mainLayout = new QGridLayout;
    mainLayout->addWidget(groupBox1);
    this->setLayout(mainLayout);
}


void CutAllImageShowWnd::slots_save_All_little_images()   //save all little images
{
    int ch = QMessageBox::warning(NULL, "Warning",
                                      "Are you sure to save all images, please input the image preImageName?",
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::Yes);

    if ( ch != QMessageBox::Yes )
        return;

    if (imageWidget->imageMap.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("The Cut All Image Area Is Empty!"));

        return;
    }



    QString imageRootPath = QFileDialog::getSaveFileName(this
                , QStringLiteral("载入图片文件"), "./detect_result_image"
                , QStringLiteral("*.jpg; *.png; *.tif; *.tiff; *.bmp"));


    if (imageRootPath.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径有误，请输入正确的图片文件保存路径"));
        return ;
    }


    int saveWidth = imageWidth->value();
    int saveHeight = imageHeight->value();

    QImage curImage;
    QString imagePath;
    for (int i=0; i<imageWidget->imageMap.size(); i++)
    {
        imagePath = imageRootPath + "_" + imageWidget->imageNameList[i];
        curImage = imageWidget->imageMap[i].scaled(saveWidth, saveHeight);
        curImage.save(imagePath);
    }

    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("所有图片保存成功"));

    return;
}


void CutAllImageShowWnd::slots_show_cutAllImage(QImage image)
{
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("请添加正确的缩略图"));
        return;
    }

    int num = imageWidget->imageNameList.size();
    QString strName = "image_" + QString::number(image.width())
            + "_" + QString::number(image.height()) +  "_" + QString::number(num+1) + ".jpg";
    QString processName = "";
    imageWidget->processNameList.push_back(processName);
    imageWidget->imageNameList.push_back(strName);
    imageWidget->imageMap.push_back(image);

    imageWidget->imagePathList.push_back(ImageDescriptionParas::getImageDescriptionParas()->imagePath);

    if (imageWidget->imageMap.size() == 0 || imageWidget->imageNameList.size() == 0)
    {
        return ;
    }

    int num1 = imageWidget->imageNameList.size();
    QString strName1 = imageWidget->imageNameList[num1-1];
    QIcon icon(QPixmap::fromImage(imageWidget->imageMap[num1-1]));
    QListWidgetItem * item = new QListWidgetItem(icon, strName1);

    imageWidget->listWidget->addItem(item);
}





ObjectDetectOptionWidget::ObjectDetectOptionWidget(QWidget* parent):QWidget(parent)
{
    initTableWidget();
    initTableWidgetItem();

    connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(slot_checkedAllItem(bool)));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(slot_saveButton_clicked()));
    connect(switchButton, SIGNAL(clicked()), this, SLOT(slot_switchButton_clicked()));

}


ObjectDetectOptionWidget::~ObjectDetectOptionWidget()
{
    DELETE_OBJ(tableWidget);
    DELETE_OBJ(vLayout);
    DELETE_OBJ(mainGroupBox);


    DELETE_OBJ(comboBox);
    DELETE_OBJ(checkBox);

    DELETE_OBJ(imageWidthName);
    DELETE_OBJ(imageHeightName);
    DELETE_OBJ(imageWidth);
    DELETE_OBJ(imageHeight);
    DELETE_OBJ(saveButton);
    DELETE_OBJ(gLayout);


    DELETE_OBJ(downGroupBox);
    DELETE_OBJ(mainLayout);
}



void ObjectDetectOptionWidget::initTableWidget()
{
    tableWidget = new QTableWidget;
    tableWidget->setColumnCount(9);  //设置成两列
    tableWidget->setFrameShape(QFrame::NoFrame);        //设置tableWidget为无边框
    tableWidget->verticalHeader()->setVisible(false);   //设置垂直的 头部说明 不可见
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList header;
    header << QStringLiteral("Target Num") << QStringLiteral("Target Rect")
           << QStringLiteral("mass Center")
           << QStringLiteral("Rotate angle")
           << QStringLiteral("Width-Height Radio")
            << QStringLiteral("countourPixCount")
               << QStringLiteral("countourArcLength")
                  << QStringLiteral("contourArea")
           << QStringLiteral("IsChecked");



    tableWidget->setHorizontalHeaderLabels(header);

    gLayout = new QGridLayout;
    vLayout = new QVBoxLayout;

    comboBox = new QComboBox;
    comboBox->addItem(QStringLiteral("保存感兴趣目标对应的图像"));
    comboBox->addItem(QStringLiteral("保存感兴趣目标参数"));

    comboBox_switch = new QComboBox;
    comboBox_switch->addItem(QStringLiteral("选择舰船目标粗检测后--目标窗口分析"));
    comboBox_switch->addItem(QStringLiteral("剔除不符合舰船尺寸条件后--目标窗口分析"));
    comboBox_switch->addItem(QStringLiteral("舰船检测识别后--目标窗口分析"));



    checkBox = new QCheckBox(QStringLiteral("是否全选"));
    checkBox->setChecked(false);

    imageWidthName = new QLabel(QStringLiteral("图片宽度： "));
    imageHeightName = new QLabel(QStringLiteral("图片高度： "));
    imageWidth = new QSpinBox;
    imageHeight = new QSpinBox;
    imageWidth->setValue(64);
    imageHeight->setValue(64);
    imageWidth->setMaximum(10000000);
    imageHeight->setMaximum(10000000);

    saveButton = new QPushButton(QStringLiteral("保存"));;
    switchButton = new QPushButton(QStringLiteral("选择目标窗口分析"));;

    gLayout->addWidget(comboBox_switch, 0, 0, 1, 2);
    gLayout->addWidget(switchButton, 0, 2, 1, 1);


    gLayout->addWidget(comboBox, 1, 0, 1, 2);
    gLayout->addWidget(checkBox, 1, 2, 1, 1);
    gLayout->addWidget(imageWidthName, 2, 0, 1, 1);
    gLayout->addWidget(imageHeightName, 3, 0, 1, 1);
    gLayout->addWidget(imageWidth, 2, 1, 1, 1);
    gLayout->addWidget(imageHeight, 3, 1, 1, 1);
    gLayout->addWidget(saveButton, 2, 2, 2, 1);


    vLayout->addWidget(tableWidget);


    downGroupBox = new QGroupBox;
    downGroupBox->setLayout(gLayout);

    mainGroupBox = new QGroupBox;
    mainGroupBox->setTitle(QStringLiteral("感兴趣目标窗口分析区"));
    mainGroupBox->setLayout(vLayout);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mainGroupBox);
    mainLayout->addWidget(downGroupBox);

    this->setLayout(mainLayout);
}

//init
void ObjectDetectOptionWidget::initTableWidgetItem()
{
    QVector<ObjectDetectResultParas> targetVec(0);

    //setRowCount(targetVec.size());
    tableWidget->setRowCount(0);

    for (int i = 0; i < targetVec.size(); i++)
    {
        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        tableWidget->item(i, 0)->setTextAlignment(Qt::AlignCenter);    //设置为 居中对齐的方式


        QString rectStr = "[" + QString::number(targetVec[i].rect.x)
                +", " + QString::number(targetVec[i].rect.y)
                + ", " + QString::number(targetVec[i].rect.width)
                + ", " + QString::number(targetVec[i].rect.height)
                + "]";
        tableWidget->setItem(i, 1, new QTableWidgetItem(rectStr));
        tableWidget->item(i, 1)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 2, new QTableWidgetItem("[" + QString::number(targetVec[i].massCenter.x)
                                                       + " , " + QString::number(targetVec[i].massCenter.y)
                                                        + "]"));
       tableWidget->item(i, 2)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(targetVec[i].rectRotate)));
        tableWidget->item(i, 3)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 4, new QTableWidgetItem("[" + QString::number(targetVec[i].width_Height_size.width)
                                                         + " , " + QString::number(targetVec[i].width_Height_size.height)
                                                          + "]" + " / "
                                                        + QString::number(targetVec[i].widthDivHeightRadio)));
        tableWidget->item(i, 4)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(targetVec[i].countourPixCount)));
        tableWidget->item(i, 5)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(targetVec[i].countourArcLength)));
        tableWidget->item(i, 6)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 7, new QTableWidgetItem(QString::number(targetVec[i].contourArea)));
        tableWidget->item(i, 7)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 8, new QTableWidgetItem());
        tableWidget->item(i, 8)->setCheckState(Qt::Unchecked);
    }
}

//从QImageWidget类中，接受 目标检测描述信息
void ObjectDetectOptionWidget::slot_receive_ObjectDetectResultParas_from_QImageWidget(QVector<ObjectDetectResultParas> objectDetectResultVec)
{

    m_objectDetectResultVec = objectDetectResultVec;
    tableWidget->setRowCount(objectDetectResultVec.size());

    for (int i = 0; i < objectDetectResultVec.size(); i++)
    {
        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        tableWidget->item(i, 0)->setTextAlignment(Qt::AlignCenter);    //设置为 居中对齐的方式


        QString rectStr = "[" + QString::number(objectDetectResultVec[i].rect.x)
                +", " + QString::number(objectDetectResultVec[i].rect.y)
                + ", " + QString::number(objectDetectResultVec[i].rect.width)
                + ", " + QString::number(objectDetectResultVec[i].rect.height)
                + "]";
        tableWidget->setItem(i, 1, new QTableWidgetItem(rectStr));
        tableWidget->item(i, 1)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 2, new QTableWidgetItem("[" + QString::number(objectDetectResultVec[i].massCenter.x)
                                                       + " , " + QString::number(objectDetectResultVec[i].massCenter.y)
                                                        + "]"));
       tableWidget->item(i, 2)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(objectDetectResultVec[i].rectRotate)));
        tableWidget->item(i, 3)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 4, new QTableWidgetItem("[" + QString::number(objectDetectResultVec[i].width_Height_size.width)
                                                         + " , " + QString::number(objectDetectResultVec[i].width_Height_size.height)
                                                          + "]" + " / "
                                                        + QString::number(objectDetectResultVec[i].widthDivHeightRadio)));
        tableWidget->item(i, 4)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(objectDetectResultVec[i].countourPixCount)));
        tableWidget->item(i, 5)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(objectDetectResultVec[i].countourArcLength)));
        tableWidget->item(i, 6)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 7, new QTableWidgetItem(QString::number(objectDetectResultVec[i].contourArea)));
        tableWidget->item(i, 7)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 8, new QTableWidgetItem());
        tableWidget->item(i, 8)->setCheckState(Qt::Unchecked);
    }
}


void ObjectDetectOptionWidget::slot_receive_ObjectDetectResultParas_afterDeleteFalseObj_from_QImageWidget(QVector<ObjectDetectResultParas> objectDetectResultVec
                  ,QVector<int> objNumber_afterDeleteFalseObject)
{
    tableWidget->setRowCount(objectDetectResultVec.size());

    for (int i = 0; i < objectDetectResultVec.size(); i++)
    {
        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(objNumber_afterDeleteFalseObject[i])));
        tableWidget->item(i, 0)->setTextAlignment(Qt::AlignCenter);    //设置为 居中对齐的方式


        QString rectStr = "[" + QString::number(objectDetectResultVec[i].rect.x)
                +", " + QString::number(objectDetectResultVec[i].rect.y)
                + ", " + QString::number(objectDetectResultVec[i].rect.width)
                + ", " + QString::number(objectDetectResultVec[i].rect.height)
                + "]";
        tableWidget->setItem(i, 1, new QTableWidgetItem(rectStr));
        tableWidget->item(i, 1)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 2, new QTableWidgetItem("[" + QString::number(objectDetectResultVec[i].massCenter.x)
                                                       + " , " + QString::number(objectDetectResultVec[i].massCenter.y)
                                                        + "]"));
       tableWidget->item(i, 2)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(objectDetectResultVec[i].rectRotate)));
        tableWidget->item(i, 3)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 4, new QTableWidgetItem("[" + QString::number(objectDetectResultVec[i].width_Height_size.width)
                                                         + " , " + QString::number(objectDetectResultVec[i].width_Height_size.height)
                                                          + "]" + " / "
                                                        + QString::number(objectDetectResultVec[i].widthDivHeightRadio)));
        tableWidget->item(i, 4)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(objectDetectResultVec[i].countourPixCount)));
        tableWidget->item(i, 5)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(objectDetectResultVec[i].countourArcLength)));
        tableWidget->item(i, 6)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 7, new QTableWidgetItem(QString::number(objectDetectResultVec[i].contourArea)));
        tableWidget->item(i, 7)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 8, new QTableWidgetItem());
        tableWidget->item(i, 8)->setCheckState(Qt::Unchecked);
    }

}

void ObjectDetectOptionWidget::slot_receive_ObjectDetectResultParas_afterPrecise_from_QImageWidget(QVector<ObjectDetectResultParas> objectDetectResultVec
                  ,QVector<int> objNumber_afterPrecise)
{
    tableWidget->setRowCount(objectDetectResultVec.size());

    for (int i = 0; i < objectDetectResultVec.size(); i++)
    {
        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(objNumber_afterPrecise[i])));
        tableWidget->item(i, 0)->setTextAlignment(Qt::AlignCenter);    //设置为 居中对齐的方式


        QString rectStr = "[" + QString::number(objectDetectResultVec[i].rect.x)
                +", " + QString::number(objectDetectResultVec[i].rect.y)
                + ", " + QString::number(objectDetectResultVec[i].rect.width)
                + ", " + QString::number(objectDetectResultVec[i].rect.height)
                + "]";
        tableWidget->setItem(i, 1, new QTableWidgetItem(rectStr));
        tableWidget->item(i, 1)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 2, new QTableWidgetItem("[" + QString::number(objectDetectResultVec[i].massCenter.x)
                                                       + " , " + QString::number(objectDetectResultVec[i].massCenter.y)
                                                        + "]"));
       tableWidget->item(i, 2)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(objectDetectResultVec[i].rectRotate)));
        tableWidget->item(i, 3)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 4, new QTableWidgetItem("[" + QString::number(objectDetectResultVec[i].width_Height_size.width)
                                                         + " , " + QString::number(objectDetectResultVec[i].width_Height_size.height)
                                                          + "]" + " / "
                                                        + QString::number(objectDetectResultVec[i].widthDivHeightRadio)));
        tableWidget->item(i, 4)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(objectDetectResultVec[i].countourPixCount)));
        tableWidget->item(i, 5)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(objectDetectResultVec[i].countourArcLength)));
        tableWidget->item(i, 6)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 7, new QTableWidgetItem(QString::number(objectDetectResultVec[i].contourArea)));
        tableWidget->item(i, 7)->setTextAlignment(Qt::AlignCenter);

        tableWidget->setItem(i, 8, new QTableWidgetItem());
        tableWidget->item(i, 8)->setCheckState(Qt::Unchecked);
    }


}




void ObjectDetectOptionWidget::slot_checkedAllItem(bool isChecked)
{
    int colcount = tableWidget->columnCount();
    if (isChecked)
    {
        for (int i = 0; i < tableWidget->rowCount(); ++i)
        {
            tableWidget->item(i, colcount-1)->setCheckState(Qt::Checked);   //最后一列设置为 是否 checked
        }
    }
    else
    {
        for (int i = 0; i < tableWidget->rowCount(); ++i)
        {
            tableWidget->item(i, colcount-1)->setCheckState(Qt::Unchecked);
        }
    }
}


void ObjectDetectOptionWidget::slot_saveButton_clicked()              // 保存按钮
{
    int curIndex = comboBox->currentIndex();  // 选中comboBox的情况，0为第一种，1为第二种情况

    if (curIndex == 0)
    {
        saveObjectDetectResultImages();
    }
    else
    {
        saveObjectDetectResultParas();
    }

}



void ObjectDetectOptionWidget::slot_switchButton_clicked()              // 保存按钮
{
    int curIndex = comboBox_switch->currentIndex();  // 选中comboBox的情况，0为第一种，1为第二种情况
    if (curIndex == 0)
    {
        slot_receive_ObjectDetectResultParas_from_QImageWidget(m_objectDetectResultVec);

    }
    else if (curIndex == 1)
    {
        slot_receive_ObjectDetectResultParas_afterDeleteFalseObj_from_QImageWidget(m_objectDetectResultVec_afterDeleteFalseObject
                                                               ,objNumber_afterDeleteFalseObject);

    }
    else
    {
        slot_receive_ObjectDetectResultParas_afterPrecise_from_QImageWidget(m_objectDetectResultVec_afterPrecise
                                                               ,objNumber_afterPrecise);
    }
}


// 粗略检测后的 目标
QVector<ObjectDetectResultParas> ObjectDetectOptionWidget::m_objectDetectResultVec = QVector<ObjectDetectResultParas>();
QVector<ObjectDetectResultParas> ObjectDetectOptionWidget::getObjectDetectResultParas()
{

//    QVector<ObjectDetectResultParas> tgtVec(m_objectDetectResultVec);
//    return tgtVec;
    return m_objectDetectResultVec;
}

// 删除不符合检测目标形态后的目标
QVector<ObjectDetectResultParas> ObjectDetectOptionWidget::m_objectDetectResultVec_afterDeleteFalseObject = QVector<ObjectDetectResultParas>();
QVector<ObjectDetectResultParas> ObjectDetectOptionWidget::getObjectDetectResultParas_afterDeleteFalseObject()
{
    return m_objectDetectResultVec_afterDeleteFalseObject;
}

// 精确检测后的目标
QVector<ObjectDetectResultParas> ObjectDetectOptionWidget::m_objectDetectResultVec_afterPrecise = QVector<ObjectDetectResultParas>();
QVector<ObjectDetectResultParas> ObjectDetectOptionWidget::getObjectDetectResultParas_afterPrecise()
{
    return m_objectDetectResultVec_afterPrecise;
}


QVector<int> ObjectDetectOptionWidget::getObjNumber_afterDeleteFalseObject()
{
    return objNumber_afterDeleteFalseObject;
}


void ObjectDetectOptionWidget::setObjectDetectResultParas(QVector<ObjectDetectResultParas> vec)
{
    m_objectDetectResultVec = vec;
}

void ObjectDetectOptionWidget::setObjectDetectResultParas_afterDeleteFalseObject(QVector<ObjectDetectResultParas> vec)
{
    m_objectDetectResultVec_afterDeleteFalseObject = vec;

}

void ObjectDetectOptionWidget::setObjectDetectResultParas_afterPrecise(QVector<ObjectDetectResultParas> vec)
{
    m_objectDetectResultVec_afterPrecise = vec;
}


QVector<int> ObjectDetectOptionWidget::objNumber_afterDeleteFalseObject = QVector<int>();
void ObjectDetectOptionWidget::setObjNumber_afterDeleteFalseObject(QVector<int> vec)
{
    objNumber_afterDeleteFalseObject = vec;

}

QVector<int> ObjectDetectOptionWidget::objNumber_afterPrecise = QVector<int>();
void ObjectDetectOptionWidget::setObjNumber_afterPrecise(QVector<int> vec)
{
    objNumber_afterPrecise = vec;
}



void ObjectDetectOptionWidget::saveObjectDetectResultParas()     // combox 第一种情况,保存参数
{
    qDebug() << "m_objectDetectResultVec.size() = " << m_objectDetectResultVec.size();
    int ch = QMessageBox::warning(NULL, "Warning",
                                      "Are you sure to save all object detect result image?",
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::Yes);

    if ( ch != QMessageBox::Yes )
        return;

    if (tableWidget->rowCount() == 0)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("The object detect result is Empty!"));

        return;
    }



    QString imageRootPath = QFileDialog::getSaveFileName(this
                , QStringLiteral("载入图片文件"), "./"
                , QStringLiteral("*.jpg; *.png; *.tif; *.tiff; *.bmp"));

    if (imageRootPath.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径有误，请输入正确的图片文件保存路径"));
        return ;
    }







    //未完  根据 原始图像，以及目标 窗口， 截取 图像 并保存
    int colcount = tableWidget->columnCount();

    for (int i = 0; i < tableWidget->rowCount(); ++i)
    {
        Qt::CheckState returnState;
        returnState = tableWidget->item(i, colcount-1)->checkState();   //最后一列设置为 是否 checked
//        if (returnState == Qt::Unchecked)
//        {
//            qDebug() << "0" ;
//        }
        if (returnState == Qt::Checked)   //选中了 该目标
        {
            Rect objectRect = m_objectDetectResultVec[i].rect;
            QString rectStr = "[" + QString::number(objectRect.x)
                    +", " + QString::number(objectRect.y)
                    + ", " + QString::number(objectRect.width)
                    + ", " + QString::number(objectRect.height)
                    + "]";
            qDebug () << rectStr;

        }
    }




    //m_objectDetectResultVec



    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("所有图片保存成功"));

    return;
}

void ObjectDetectOptionWidget::saveObjectDetectResultImages()    // combox 第二种情况,保存相对应的 图像
{
    int ch = QMessageBox::warning(NULL, "Warning",
                                      "Are you sure to save all object detect result image?",
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::Yes);

    if ( ch != QMessageBox::Yes )
        return;

    if (tableWidget->rowCount() == 0)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("The object detect result is Empty!"));

        return;
    }



    QString imageRootPath = QFileDialog::getSaveFileName(this
                , QStringLiteral("载入图片文件"), "./"
                , QStringLiteral("*.jpg; *.png; *.tif; *.tiff; *.bmp"));


    if (imageRootPath.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径有误，请输入正确的图片文件保存路径"));
        return ;
    }


    int saveWidth = imageWidth->value();
    int saveHeight = imageHeight->value();

    //未完  根据 原始图像，以及目标 窗口， 截取 图像 并保存
    int colcount = tableWidget->columnCount();

//    qDebug () << imageRootPath;   //输出为ship_.jpg
    QFileInfo file = QFileInfo(imageRootPath);
    QString fileName = file.fileName();           //输出为ship_.jpg
    QString filePath = file.absolutePath();       //输出为根 目录
//    qDebug() << fileName;
//    qDebug() << filePath;


    QImage curImage = QImageWidget::getCentralQImage();
    if (curImage.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("中央图片区域为空，请输入正确的图像"));
        return;

    }


    int curIndex = comboBox_switch->currentIndex();  // 选中comboBox的情况，0为第一种，1为第二种情况
    Rect objectRect;
    for (int i = 0; i < tableWidget->rowCount(); ++i)
    {
        Qt::CheckState returnState;
        returnState = tableWidget->item(i, colcount-1)->checkState();   //最后一列设置为 是否 checked

        if (returnState == Qt::Checked)   //选中了 该目标
        {
            if (curIndex == 0)
            {
                objectRect = m_objectDetectResultVec[i].rect;
            }else if(curIndex == 1)
            {
                objectRect = m_objectDetectResultVec_afterDeleteFalseObject[i].rect;

            }else
            {
                objectRect = m_objectDetectResultVec_afterPrecise[i].rect;

            }

            QString rectStr = "[" + QString::number(objectRect.x)
                    +", " + QString::number(objectRect.y)
                    + ", " + QString::number(objectRect.width)
                    + ", " + QString::number(objectRect.height)
                    + "]";
            qDebug () << i << rectStr;

            objectRect = assignSampleObjectSize(Size(curImage.width(), curImage.height())
                                                     ,objectRect, Size(saveWidth, saveHeight));
            rectStr = "[" + QString::number(objectRect.x)
                                +", " + QString::number(objectRect.y)
                                + ", " + QString::number(objectRect.width)
                                + ", " + QString::number(objectRect.height)
                                + "]";
            qDebug () << i << rectStr;


            QRect tgtCutRect = QRect(objectRect.x, objectRect.y
                                     , objectRect.width, objectRect.height);
            QImage tgtImage = curImage.copy(tgtCutRect);

            QString imagePath;
            imagePath = filePath + "/ship_" + QString::number(i) + ".png";
            tgtImage.save(imagePath);

        }
    }




    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("所有图片保存成功"));

    return;
}




//设置读入图像 获取hog特征 并svm检测的窗口，进行检测

HogAndSVM_ObjectDetect_settingWidget::HogAndSVM_ObjectDetect_settingWidget(QWidget* parent):QWidget(parent)
{
    this->createControls();

    connect(svmParasSettingButton, SIGNAL(clicked())
            ,this, SLOT(slot_svmParasSettingButton_clicked()));

    connect(positiveInputButton, SIGNAL(clicked())
            ,this, SLOT(slot_positiveInputButton_clicked()));

    connect(negativeInputButton, SIGNAL(clicked())
            ,this, SLOT(slot_negativeInputButton_clicked()));

    connect(svmModelSaveButton, SIGNAL(clicked())
            ,this, SLOT(slot_svmModelSaveButton_clicked()));

    connect(getHogFeaturesDimButton, SIGNAL(clicked())
            ,this, SLOT(slot_getHogFeaturesDimButton_clicked()));

    connect(getSvmTrainingModelButton, SIGNAL(clicked())
            ,this, SLOT(slot_getSvmTrainingModelButton_clicked()));

    connect(getOpticalSvmTrainingModelButton, SIGNAL(clicked())
            ,this, SLOT(slot_getOpticalSvmTrainingModelButton_clicked()));

    connect(modifyImageNameButton, SIGNAL(clicked())
            ,this, SLOT(slot_modifyImageNameButton_clicked()));

    connect(imageRenameButton, SIGNAL(clicked())
            ,this, SLOT(slot_imageRenameButton_clicked()));

    connect(imageReSizePushButton, SIGNAL(clicked())
            ,this, SLOT(slot_imageReSizeButton_clicked()));


    connect(singleimagePathOptionPushButton, SIGNAL(clicked())
            ,this, SLOT(slot_singleImagePathOptionButton_clicked()));

    connect(singleimageReSizePushButton, SIGNAL(clicked())
            ,this, SLOT(slot_singleImageResizeButton_clicked()));

    connect(rotateImage, SIGNAL(clicked())
            ,this, SLOT(slot_rotateImage_clicked()));

    //单张图片预测
    connect(singleImagePredictPathOptionButton, SIGNAL(clicked())
            ,this, SLOT(slot_singleImagePredictPathOption()));
    connect(singleImagePredictPathOptionSvmButton, SIGNAL(clicked())
            ,this, SLOT(slot_singleImagePredictSvmOption()));
    connect(singleImagePredictButton, SIGNAL(clicked())
            ,this, SLOT(slot_singleImagePredict()));

    //批量图片预测
    connect(batchImagePredictPathOptionButton, SIGNAL(clicked())
            ,this, SLOT(slot_batchImagePredictPathOption()));
    connect(batchImagePredictPathOptionSvmButton, SIGNAL(clicked())
            ,this, SLOT(slot_batchImagePredicctSvmOption()));
    connect(batchImagePredictButton, SIGNAL(clicked())
            ,this, SLOT(slot_batchImagePredict()));
}

void HogAndSVM_ObjectDetect_settingWidget::createControls()
{
    //svm 训练窗口
    svmParasSettingButton = new QPushButton(QStringLiteral("HOG特征和SVM参数设置"));

    positiveLable = new QLabel(QStringLiteral("正样本路径"));
    positiveLineEdit = new QLineEdit();
    positiveLineEdit->setReadOnly(true);
    positiveInputButton = new QPushButton(QStringLiteral("路径选择"));

    negativeLable = new QLabel(QStringLiteral("负样本路径"));
    negativeLineEdit = new QLineEdit();
    negativeLineEdit->setReadOnly(true);
    negativeInputButton = new QPushButton(QStringLiteral("路径选择"));

    svmModelSaveLable = new QLabel(QStringLiteral("SVM训练模型"));
    svmModelSaveLineEdit = new QLineEdit();
    svmModelSaveButton = new QPushButton(QStringLiteral("路径选择"));

    getHogFeaturesDimButton = new QPushButton(QStringLiteral("获取Hog特征的维度"));
    getSvmTrainingModelButton = new QPushButton(QStringLiteral("获取SVM训练模型"));
    getOpticalSvmTrainingModelButton = new QPushButton(QStringLiteral("获取SVM最佳训练模型"));

    svmModelGLayout = new QGridLayout;
    svmModelGLayout->addWidget(svmParasSettingButton, 0, 0, 1, 3);
    svmModelGLayout->addWidget(positiveLable, 1, 0);
    svmModelGLayout->addWidget(positiveLineEdit, 1, 1);
    svmModelGLayout->addWidget(positiveInputButton, 1, 2);

    svmModelGLayout->addWidget(negativeLable, 2, 0);
    svmModelGLayout->addWidget(negativeLineEdit, 2, 1);
    svmModelGLayout->addWidget(negativeInputButton, 2, 2);

    svmModelGLayout->addWidget(svmModelSaveLable, 3, 0);
    svmModelGLayout->addWidget(svmModelSaveLineEdit, 3, 1);
    svmModelGLayout->addWidget(svmModelSaveButton, 3, 2);

    svmModelGLayout->addWidget(getHogFeaturesDimButton, 4, 0);

    svmModelGLayout->addWidget(getSvmTrainingModelButton, 4, 1);
    svmModelGLayout->addWidget(getOpticalSvmTrainingModelButton, 4, 2, 1, 1);

    svmModelGroupBox = new QGroupBox(QStringLiteral("获取SVM训练模型"));
    svmModelGroupBox->setLayout(svmModelGLayout);
    //批量修改图片文件的 名称
    modifyImageNameLabel = new QLabel(QStringLiteral("文件路径"));
    modifyImageNameLineEdit = new QLineEdit;
    modifyImageNameButton = new QPushButton(QStringLiteral("路径选择"));


    imageRenameLabel = new QLabel(QStringLiteral("图像重命名_前缀"));
    imageRenameLineEdit = new QLineEdit;
    imageRenameLineEdit->setToolTip(QStringLiteral("输入批量图像的前缀名称"));
    imageRenameButton = new QPushButton(QStringLiteral("批量重命名"));

    // 添加图片尺寸 改变的布局
    imageReSizeCheckBox = new QCheckBox(QStringLiteral("是否批量修改图片的尺寸"));
    imageReSizeCheckBox->setChecked(false);
    imageReSizeWidthLabel = new QLabel(QStringLiteral("图片的宽度："));
    imageReSizeWidthLineEdit = new QLineEdit;
    imageReSizeHeightLabel = new QLabel(QStringLiteral("图片的高度："));
    imageReSizeHeightLineEdit = new QLineEdit;
    imageReSizePushButton = new QPushButton(QStringLiteral("批量修改图片的尺寸"));


    singleimageReSizeCheckBox = new QCheckBox(QStringLiteral("是否修改单张图片的尺寸"));
    singleimageReSizeCheckBox->setChecked(false);
    isRotateImage= new QCheckBox(QStringLiteral("是否旋转图片扩大数据集合"));
    isRotateImage->setChecked(false);


    singleimageReSizePathLabel = new QLabel(QStringLiteral("图片的路径"));
    singleimageReSizePathLineEdit = new QLineEdit;;
    singleimagePathOptionPushButton = new QPushButton(QStringLiteral("路径选择"));
    singleimageReSizePushButton = new QPushButton(QStringLiteral("修改单张图片的尺寸"));
    rotateImage = new QPushButton(QStringLiteral("旋转图片扩大数据集合"));



    imageResizeGroupBox = new QGroupBox(QStringLiteral("修改图片尺寸"));
    imageResizeGLayout = new QGridLayout;


    imageResizeGLayout->addWidget(singleimageReSizeCheckBox, 0, 0);
    imageResizeGLayout->addWidget(imageReSizeCheckBox, 0, 1);
    imageResizeGLayout->addWidget(isRotateImage, 0, 2);

    imageResizeGLayout->addWidget(singleimageReSizePathLabel, 1, 0);
    imageResizeGLayout->addWidget(singleimageReSizePathLineEdit, 1, 1);
    imageResizeGLayout->addWidget(singleimagePathOptionPushButton, 1, 2);


    imageResizeGLayout->addWidget(imageReSizeWidthLabel, 2, 0);
    imageResizeGLayout->addWidget(imageReSizeWidthLineEdit, 2, 1, 1, 2);
    imageResizeGLayout->addWidget(imageReSizeHeightLabel, 3, 0);
    imageResizeGLayout->addWidget(imageReSizeHeightLineEdit, 3, 1, 1, 2);


    imageResizeGLayout->addWidget(singleimageReSizePushButton, 4, 0, 1, 1);
    imageResizeGLayout->addWidget(imageReSizePushButton, 4, 1, 1, 1);
    imageResizeGLayout->addWidget(rotateImage, 4, 2, 1, 1);


    imageResizeGroupBox->setLayout(imageResizeGLayout);





    modifyImageNameGLayout = new QGridLayout;
    modifyImageNameGLayout->addWidget(modifyImageNameLabel, 1, 0);
    modifyImageNameGLayout->addWidget(modifyImageNameLineEdit, 1, 1);
    modifyImageNameGLayout->addWidget(modifyImageNameButton, 1, 2);

    modifyImageNameGLayout->addWidget(imageRenameLabel, 2, 0);
    modifyImageNameGLayout->addWidget(imageRenameLineEdit, 2, 1);
    modifyImageNameGLayout->addWidget(imageRenameButton, 2, 2);



    modifyImageNameGroupBox = new QGroupBox(QStringLiteral("批量修改图片文件名称"));
    modifyImageNameGroupBox->setLayout(modifyImageNameGLayout);

    svmTrainInputInfoEdit = new QTextEdit;
    svmTrainInputInfoLayout = new QHBoxLayout;
    svmTrainInputInfoLayout->addWidget(svmTrainInputInfoEdit);
    svmTrainInputInfoGroupBox = new QGroupBox(QStringLiteral("SVM训练输出相关信息"));
    svmTrainInputInfoGroupBox->setLayout(svmTrainInputInfoLayout);


    //单张图片进行预测
    singleImagePredictGroupBox = new QGroupBox(QStringLiteral("单张图片预测"));
    singleImagePredictLabel = new QLabel(QStringLiteral("单张图片路径"));
    singleImagePredictLineEdit = new QLineEdit;
    singleImagePredictPathOptionButton = new QPushButton(QStringLiteral("路径选择"));
    singleImagePredictButton = new QPushButton(QStringLiteral("SVM预测"));

    singleImagePredictSvmLabel = new QLabel(QStringLiteral("SVM模型选择"));
    singleImagePredictSvmLineEdit = new QLineEdit;
    singleImagePredictPathOptionSvmButton = new QPushButton(QStringLiteral("路径选择"));
    singleImagePredictGLayout = new QGridLayout;

    singleImagePredictGLayout->addWidget(singleImagePredictLabel, 0, 0);
    singleImagePredictGLayout->addWidget(singleImagePredictLineEdit, 0, 1);
    singleImagePredictGLayout->addWidget(singleImagePredictPathOptionButton, 0, 2);
    singleImagePredictGLayout->addWidget(singleImagePredictSvmLabel, 1, 0);
    singleImagePredictGLayout->addWidget(singleImagePredictSvmLineEdit, 1, 1);
    singleImagePredictGLayout->addWidget(singleImagePredictPathOptionSvmButton, 1, 2);
    singleImagePredictGLayout->addWidget(singleImagePredictButton, 2, 0, 1, 3);
    singleImagePredictGroupBox->setLayout(singleImagePredictGLayout);



    //批量图片进行预测

    batchImagePredictGroupBox = new QGroupBox(QStringLiteral("批量图片预测"));
    batchImagePredictLabel = new QLabel(QStringLiteral("批量图片路径"));
    batchImagePredictLineEdit = new QLineEdit;
    batchImagePredictPathOptionButton = new QPushButton(QStringLiteral("路径选择"));
    batchImagePredictButton = new QPushButton(QStringLiteral("SVM预测"));

    batchImagePredictSvmLabel = new QLabel(QStringLiteral("SVM模型选择"));
    batchImagePredictSvmLineEdit = new QLineEdit;
    batchImagePredictPathOptionSvmButton = new QPushButton(QStringLiteral("路径选择"));
    batchImagePredictGLayout = new QGridLayout;

    batchImagePredictGLayout->addWidget(batchImagePredictLabel, 0, 0);
    batchImagePredictGLayout->addWidget(batchImagePredictLineEdit, 0, 1);
    batchImagePredictGLayout->addWidget(batchImagePredictPathOptionButton, 0, 2);
    batchImagePredictGLayout->addWidget(batchImagePredictSvmLabel, 1, 0);
    batchImagePredictGLayout->addWidget(batchImagePredictSvmLineEdit, 1, 1);
    batchImagePredictGLayout->addWidget(batchImagePredictPathOptionSvmButton, 1, 2);

    batchImagePredictGLayout->addWidget(batchImagePredictButton, 2, 0, 1, 3);
    batchImagePredictGroupBox->setLayout(batchImagePredictGLayout);




    mainLayout = new QGridLayout;
    mainLayout->addWidget(modifyImageNameGroupBox, 0, 0);
    mainLayout->addWidget(imageResizeGroupBox, 1, 0);


    mainLayout->addWidget(svmModelGroupBox, 2, 0);
    mainLayout->addWidget(singleImagePredictGroupBox, 3, 0);
    mainLayout->addWidget(batchImagePredictGroupBox, 4, 0);

    svmTrainInputInfoGroupBox->setMinimumSize(500, 200);
    mainLayout->addWidget(svmTrainInputInfoGroupBox, 0, 1, 5, 1);

    this->setLayout(mainLayout);
}


void HogAndSVM_ObjectDetect_settingWidget::slot_svmParasSettingButton_clicked()   //svm参数设置
{
    ParameterSettingWidget::getParameterSettingWidget()->show();


}

void HogAndSVM_ObjectDetect_settingWidget::slot_positiveInputButton_clicked()
{
    QString fileFull = QFileDialog::getOpenFileName(this
                , QStringLiteral("载入图片文件"), "./"
                , QStringLiteral("*.jpg; *.png; *.tif; *.tiff; *.bmp"));

    if (fileFull.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    QFileInfo file = QFileInfo(fileFull);
    QString fileName = file.fileName();
    QString filePath = file.absolutePath();

    positiveLineEdit->setText(filePath);
    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("文件路径输入正确"));
    svmTrainInputInfoEdit->append(QStringLiteral("正样本，文件路径设置成功"));

    QString outputInfo = QStringLiteral("正样本路径为：") + filePath;
    svmTrainInputInfoEdit->append(outputInfo);

// 输出正样本的 总个数
    vector<string> _imageNameSet;
    getImageNamesSetFromSpecifiedPath(filePath.toStdString(), _imageNameSet);
    outputInfo = QStringLiteral("正样本总个数为：") + QString::number(_imageNameSet.size()) + "\n";
    svmTrainInputInfoEdit->append(outputInfo);

    return;
}



void HogAndSVM_ObjectDetect_settingWidget::slot_negativeInputButton_clicked()
{
    QString fileFull = QFileDialog::getOpenFileName(this
                , QStringLiteral("载入图片文件"), "./"
                , QStringLiteral("*.jpg; *.png; *.tif; *.tiff; *.bmp"));

    if (fileFull.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    QFileInfo file = QFileInfo(fileFull);
    QString fileName = file.fileName();
    QString filePath = file.absolutePath();

    negativeLineEdit->setText(filePath);
    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("文件路径输入正确"));
    svmTrainInputInfoEdit->append(QStringLiteral("负样本，文件路径输入成功"));

    QString outputInfo = QStringLiteral("负样本路径为：") + filePath;
    svmTrainInputInfoEdit->append(outputInfo);

    // 输出负样本的 总个数
        vector<string> _imageNameSet;
        getImageNamesSetFromSpecifiedPath(filePath.toStdString(), _imageNameSet);
        outputInfo = QStringLiteral("负样本总个数为：") + QString::number(_imageNameSet.size()) + "\n";
        svmTrainInputInfoEdit->append(outputInfo);

    return;
}

void HogAndSVM_ObjectDetect_settingWidget::slot_svmModelSaveButton_clicked()
{
    QString fileFull = QFileDialog::getSaveFileName(this
                , QStringLiteral("载入svm训练模型"), "./"
                , QStringLiteral("*.xml"));

    if (fileFull.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

//    QFileInfo file = QFileInfo(fileFull);
//    QString fileName = file.fileName();
//    QString filePath = file.absolutePath();

    svmModelSaveLineEdit->setText(fileFull);
    svmTrainInputInfoEdit->append(QStringLiteral("SVM训练模型，保存路径设置成功"));

    QString outputInfo = QStringLiteral("SVM训练模型保存为：") + fileFull + "\n";
    svmTrainInputInfoEdit->append(outputInfo);
    return;
}

void HogAndSVM_ObjectDetect_settingWidget::slot_getHogFeaturesDimButton_clicked()
{
    QString positiveSampleInputPathName = positiveLineEdit->text();
    QString negativeSampleInputPathName = negativeLineEdit->text();

    if (positiveSampleInputPathName.isEmpty()
     || negativeSampleInputPathName.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return;
    }

    HogParameters hogParameters;
    hogParameters.blockSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockWin);
    hogParameters.winSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->detectWin);
    hogParameters.winStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->winStrider);
    hogParameters.blockStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockStrider);
    hogParameters.cellSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->cellWin);
    hogParameters.nbins = DateMgr::getDataParas()->hogFeatureParas->nbins;



    vector<string> imageNameSet;
    bool isReadCorrect = getImageNamesSetFromSpecifiedPath(positiveSampleInputPathName.toStdString(), imageNameSet);  //get image names set
    if (!isReadCorrect)
    {
        QMessageBox::warning(this, "warning", QStringLiteral("指定的路径当中，图片为空"));
        return;
    }

    vector<float> imageHogFeature;   //single image hog feature

    //_winSize, _blockSize, _blockStride, _cellSize, _nbins
    HOGDescriptor *hog = new HOGDescriptor(hogParameters.winSize, hogParameters.blockSize,
        hogParameters.blockStride, hogParameters.cellSize, hogParameters.nbins);
    hog->gammaCorrection = true;


    Mat imreadImage;

    imreadImage = imread(imageNameSet[0], 0);
    hog->compute(imreadImage,  imageHogFeature, hogParameters.winStride);

    int HogFeaturesDims = (int)(imageHogFeature.size());

    QString outputInfo = QStringLiteral("Hog特征的维度为：") + QString::number(HogFeaturesDims) + "\n";
    svmTrainInputInfoEdit->append(outputInfo);
}




void HogAndSVM_ObjectDetect_settingWidget::slot_getSvmTrainingModelButton_clicked()
{
    QString positiveSampleInputPathName = positiveLineEdit->text();
    QString negativeSampleInputPathName = negativeLineEdit->text();

    QString svmModelSaveFillFull = svmModelSaveLineEdit->text();
    QFileInfo file = QFileInfo(svmModelSaveFillFull);
    QString svmModelName = file.fileName();               //only included the svm Model Name
    QString svmTrainModelSavePath = file.absolutePath();  //don't included the last backSlash

    qDebug() << svmModelName;
    qDebug() << svmTrainModelSavePath;

    if (positiveSampleInputPathName.isEmpty()
     || negativeSampleInputPathName.isEmpty()
     || svmModelSaveFillFull.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return;
    }

    //输入 正在获取 svm 训练模型
    svmTrainInputInfoEdit->append(QStringLiteral("SVM训练模型获取中。。。请耐心的等待"));
    svmTrainInputInfoEdit->repaint();

    HogParameters hogParameters;
    hogParameters.blockSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockWin);
    hogParameters.winSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->detectWin);
    hogParameters.winStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->winStrider);
    hogParameters.blockStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockStrider);
    hogParameters.cellSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->cellWin);
    hogParameters.nbins = DateMgr::getDataParas()->hogFeatureParas->nbins;

    qDebug() << hogParameters.blockSize.width << hogParameters.blockSize.height;
    qDebug() << hogParameters.winSize.width << hogParameters.winSize.height;;
    qDebug() << hogParameters.winStride.width << hogParameters.winStride.height;;
    qDebug() << hogParameters.blockStride.width << hogParameters.blockStride.height;;
    qDebug() << hogParameters.cellSize.width << hogParameters.cellSize.height;;
    qDebug() << hogParameters.nbins;



    CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,
                                             DateMgr::getDataParas()->svmParas->maxIteration
                                           , DateMgr::getDataParas()->svmParas->epsilon);
    /*CvSVMParams svmParameters(CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.09, 1.0,
    10.0, 0.5, 1.0, 0, criteria);*/  //model2的参数

    /*CvSVMParams parameters:

        enum { C_SVC=100, NU_SVC=101, ONE_CLASS=102, EPS_SVR=103, NU_SVR=104 };
        enum { LINEAR=0, POLY=1, RBF=2, SIGMOID=3 };
        enum { C=0, GAMMA=1, P=2, NU=3, COEF=4, DEGREE=5 };
            CV_PROP_RW int         svm_type;
            CV_PROP_RW int         kernel_type;
            CV_PROP_RW double      degree; // for poly
            CV_PROP_RW double      gamma;  // for poly/rbf/sigmoid
            CV_PROP_RW double      coef0;  // for poly/sigmoid
            CV_PROP_RW double      C;    // for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
            CV_PROP_RW double      nu;   // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
            CV_PROP_RW double      p;    // for CV_SVM_EPS_SVR
            CvMat*      class_weights;   // for CV_SVM_C_SVC
            CV_PROP_RW CvTermCriteria term_crit;    // termination criteria
    */

    CvSVMParams svmParameters;
    svmParameters.svm_type = DateMgr::getDataParas()->svmParas->svmType;
    svmParameters.kernel_type = DateMgr::getDataParas()->svmParas->kernelType;
    svmParameters.degree = DateMgr::getDataParas()->svmParas->degree;
    svmParameters.gamma = DateMgr::getDataParas()->svmParas->gamma;   //多项式/rbf/sigmoid核函数
    svmParameters.coef0 = DateMgr::getDataParas()->svmParas->coef0;      //poly/sigmoid 核函数

    svmParameters.C = DateMgr::getDataParas()->svmParas->Cvalue;  //为损失函数
    svmParameters.nu = DateMgr::getDataParas()->svmParas->nu;
    svmParameters.p = DateMgr::getDataParas()->svmParas->p;         //p为设置e-SVR中损失函数的值
    svmParameters.class_weights = 0;
    svmParameters.term_crit = criteria;

    qDebug() << svmParameters.svm_type;
    qDebug() << svmParameters.kernel_type;
    qDebug() << svmParameters.degree;
    qDebug() << svmParameters.gamma;
    qDebug() << svmParameters.coef0;
    qDebug() << svmParameters.C;
    qDebug() << svmParameters.nu;
    qDebug() << svmParameters.p;
    qDebug() << criteria.max_iter;
    qDebug() << criteria.epsilon;

    getSvmTrainingModel(positiveSampleInputPathName.toStdString(), negativeSampleInputPathName.toStdString(),
        svmTrainModelSavePath.toStdString(), svmModelName.toStdString(), criteria, svmParameters, hogParameters);
    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("获取SVM训练模型成功"));
    svmTrainInputInfoEdit->append(QStringLiteral("获取SVM训练模型成功"));

    QString outputInfo = QStringLiteral("SVM训练模型保存为：")
            + svmModelSaveLineEdit->text() + "\n";
    svmTrainInputInfoEdit->append(outputInfo);
}

void HogAndSVM_ObjectDetect_settingWidget::slot_getOpticalSvmTrainingModelButton_clicked()
{
    QString positiveSampleInputPathName = positiveLineEdit->text();
    QString negativeSampleInputPathName = negativeLineEdit->text();

    QString svmModelSaveFillFull = svmModelSaveLineEdit->text();
    QFileInfo file = QFileInfo(svmModelSaveFillFull);
    QString svmModelName = file.fileName();               //only included the svm Model Name
    QString svmTrainModelSavePath = file.absolutePath();  //don't included the last backSlash

    qDebug() << svmModelName;
    qDebug() << svmTrainModelSavePath;

    if (positiveSampleInputPathName.isEmpty()
     || negativeSampleInputPathName.isEmpty()
     || svmModelSaveFillFull.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return;
    }


    //输入 正在获取 svm 训练模型
    svmTrainInputInfoEdit->append(QStringLiteral("SVM最佳训练模型获取中。。。请耐心的等待"));
    svmTrainInputInfoEdit->repaint();

    HogParameters hogParameters;
    hogParameters.blockSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockWin);
    hogParameters.winSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->detectWin);
    hogParameters.winStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->winStrider);
    hogParameters.blockStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockStrider);
    hogParameters.cellSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->cellWin);
    hogParameters.nbins = DateMgr::getDataParas()->hogFeatureParas->nbins;

    qDebug() << hogParameters.blockSize.width << hogParameters.blockSize.height;
    qDebug() << hogParameters.winSize.width << hogParameters.winSize.height;;
    qDebug() << hogParameters.winStride.width << hogParameters.winStride.height;;
    qDebug() << hogParameters.blockStride.width << hogParameters.blockStride.height;;
    qDebug() << hogParameters.cellSize.width << hogParameters.cellSize.height;;
    qDebug() << hogParameters.nbins;



    CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,
                                             DateMgr::getDataParas()->svmParas->maxIteration
                                           , DateMgr::getDataParas()->svmParas->epsilon);
    /*CvSVMParams svmParameters(CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.09, 1.0,
    10.0, 0.5, 1.0, 0, criteria);*/  //model2的参数

    /*CvSVMParams parameters:

        enum { C_SVC=100, NU_SVC=101, ONE_CLASS=102, EPS_SVR=103, NU_SVR=104 };
        enum { LINEAR=0, POLY=1, RBF=2, SIGMOID=3 };
        enum { C=0, GAMMA=1, P=2, NU=3, COEF=4, DEGREE=5 };
            CV_PROP_RW int         svm_type;
            CV_PROP_RW int         kernel_type;
            CV_PROP_RW double      degree; // for poly
            CV_PROP_RW double      gamma;  // for poly/rbf/sigmoid
            CV_PROP_RW double      coef0;  // for poly/sigmoid
            CV_PROP_RW double      C;    // for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
            CV_PROP_RW double      nu;   // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
            CV_PROP_RW double      p;    // for CV_SVM_EPS_SVR
            CvMat*      class_weights;   // for CV_SVM_C_SVC
            CV_PROP_RW CvTermCriteria term_crit;    // termination criteria
    */

    CvSVMParams svmParameters;
    svmParameters.svm_type = DateMgr::getDataParas()->svmParas->svmType;
    svmParameters.kernel_type = DateMgr::getDataParas()->svmParas->kernelType;
    svmParameters.degree = DateMgr::getDataParas()->svmParas->degree;
    svmParameters.gamma = DateMgr::getDataParas()->svmParas->gamma;   //多项式/rbf/sigmoid核函数
    svmParameters.coef0 = DateMgr::getDataParas()->svmParas->coef0;      //poly/sigmoid 核函数

    svmParameters.C = DateMgr::getDataParas()->svmParas->Cvalue;  //为损失函数
    svmParameters.nu = DateMgr::getDataParas()->svmParas->nu;
    svmParameters.p = DateMgr::getDataParas()->svmParas->p;         //p为设置e-SVR中损失函数的值
    svmParameters.class_weights = 0;
    svmParameters.term_crit = criteria;

    qDebug() << svmParameters.svm_type;
    qDebug() << svmParameters.kernel_type;
    qDebug() << svmParameters.degree;
    qDebug() << svmParameters.gamma;
    qDebug() << svmParameters.coef0;
    qDebug() << svmParameters.C;
    qDebug() << svmParameters.nu;
    qDebug() << svmParameters.p;
    qDebug() << criteria.max_iter;
    qDebug() << criteria.epsilon;

    getSvmTrainingOptimalModel(positiveSampleInputPathName.toStdString(), negativeSampleInputPathName.toStdString(),
        svmTrainModelSavePath.toStdString(), svmModelName.toStdString(), criteria, svmParameters, hogParameters);

    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("获取SVM最佳训练模型成功"));

    svmTrainInputInfoEdit->append(QStringLiteral("获取SVM最佳训练模型成功"));

    QString outputInfo = QStringLiteral("SVM最佳训练模型保存为：")
            + svmModelSaveLineEdit->text() + "\n";
    svmTrainInputInfoEdit->append(outputInfo);

    QString outputCvalueInfo = QStringLiteral("SVM最佳训练模型获得的Cvalue：")
            + QString::number(DateMgr::getDataParas()->svmParas->Cvalue);
    QString outputGammaInfo = QStringLiteral("SVM最佳训练模型获得的Gamma：")
            + QString::number(DateMgr::getDataParas()->svmParas->gamma);
    QString outputCoef0Info = QStringLiteral("SVM最佳训练模型获得的coef0：")
            + QString::number(DateMgr::getDataParas()->svmParas->coef0);
    QString outputDegreeInfo = QStringLiteral("SVM最佳训练模型获得的degree：")
            + QString::number(DateMgr::getDataParas()->svmParas->degree);
    QString outputNuInfo = QStringLiteral("SVM最佳训练模型获得的nu：")
            + QString::number(DateMgr::getDataParas()->svmParas->nu);
    QString outputPInfo = QStringLiteral("SVM最佳训练模型获得的P：")
            + QString::number(DateMgr::getDataParas()->svmParas->p);

    svmTrainInputInfoEdit->append(outputCvalueInfo);
    svmTrainInputInfoEdit->append(outputGammaInfo);
    svmTrainInputInfoEdit->append(outputCoef0Info);
    svmTrainInputInfoEdit->append(outputDegreeInfo);
    svmTrainInputInfoEdit->append(outputNuInfo);
    svmTrainInputInfoEdit->append(outputPInfo);

}



void HogAndSVM_ObjectDetect_settingWidget::slot_modifyImageNameButton_clicked()
{

    QString fileFull = QFileDialog::getOpenFileName(this
                , QStringLiteral("批量修改图片文件"), "./"
                , QStringLiteral("*.jpg; *.png; *.tif; *.tiff; *.bmp"));

    if (fileFull.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    QFileInfo file = QFileInfo(fileFull);
    QString fileName = file.fileName();
    QString filePath = file.absolutePath();

    modifyImageNameLineEdit->setText(filePath);

    svmTrainInputInfoEdit->append(QStringLiteral("批量修改图片文件路径-加载成功"));
    QString outputInfo = QStringLiteral("加载路径名为： ") + filePath + "\n";
    svmTrainInputInfoEdit->append(outputInfo);
    return;
}


//对文件夹内的图像 进行批量的修改 文件名称
void HogAndSVM_ObjectDetect_settingWidget::slot_imageRenameButton_clicked()
{
    QString imageRootPath = modifyImageNameLineEdit->text();  //输入图像的 前缀名称
    QString imagePreName = imageRenameLineEdit->text();

    if (imageRootPath.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    if (imagePreName.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入图像重命名——前缀为空，请输入正确的前缀名称"));
        return ;
    }


    vector<string> _imageNameSet;
    getImageNamesSetFromSpecifiedPath(imageRootPath.toStdString(), _imageNameSet);

    vector<Mat> imageMat;
    for (int i=0; i<_imageNameSet.size(); ++i)
    {

        imageMat.push_back(imread(_imageNameSet[i], 2|4));
    }


    //删除原来的 图像

    for (int i=0; i<_imageNameSet.size(); ++i)
    {
        remove(_imageNameSet[i].c_str());
    }

    QString imageFilePreName = imageRootPath + "/" + imagePreName + "_";
    QString imageFileName;
    for (int i=0; i<imageMat.size(); ++i)
    {
        imageFileName = imageFilePreName + QString::number(i) + ".jpg";
        imwrite(imageFileName.toStdString(), imageMat[i]);
    }

    svmTrainInputInfoEdit->append(QStringLiteral("图像文件重命名成功"));
}

void HogAndSVM_ObjectDetect_settingWidget::slot_imageReSizeButton_clicked()
{
    QString imageRootPath = modifyImageNameLineEdit->text();      //输入图像的 根目录路径

    if (imageRootPath.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    if (imageReSizeWidthLineEdit->text().isEmpty()
            || imageReSizeHeightLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入图片的尺寸为空"));
        return ;
    }


    bool isImageResizeChecked = imageReSizeCheckBox->isChecked();
    int imageReSizeWidth = imageReSizeWidthLineEdit->text().toInt();
    int imageReSizeHeight= imageReSizeHeightLineEdit->text().toInt();

    if (!isImageResizeChecked)  //没有选中
    {
        QMessageBox::warning(this, "warning", QStringLiteral("不需要修改图片的尺寸"));
        return;
    }

    vector<string> _imageNameSet;
    getImageNamesSetFromSpecifiedPath(imageRootPath.toStdString(), _imageNameSet);

    vector<Mat> imageMat;
    for (int i=0; i<_imageNameSet.size(); ++i)
    {
        imageMat.push_back(imread(_imageNameSet[i], 2|4));
    }


    //删除原来的 图像
    for (int i=0; i<_imageNameSet.size(); ++i)
    {
        remove(_imageNameSet[i].c_str());
    }

    //重新写入 图片
    Mat dst;
    for (int i=0; i<imageMat.size(); ++i)
    {
        cv::resize(imageMat[i], dst, Size(imageReSizeWidth, imageReSizeHeight));
        imwrite(_imageNameSet[i], dst);
    }


    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("批量修改图片的尺寸成功"));
    svmTrainInputInfoEdit->append(QStringLiteral("批量修改图片的尺寸成功"));
}

void HogAndSVM_ObjectDetect_settingWidget::slot_rotateImage_clicked()
{
    svmTrainInputInfoEdit->append(QStringLiteral("开始扩充图片数据集合"));
    svmTrainInputInfoEdit->repaint();
    QString imageRootPath = modifyImageNameLineEdit->text();      //输入图像的 根目录路径

    if (imageRootPath.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    bool isRotateImageBool = isRotateImage->isChecked();

    if (!isRotateImageBool)  //没有选中
    {
        QMessageBox::warning(this, "warning", QStringLiteral("不需要旋转扩充图像数据集合"));
        return;
    }


    vector<string> _imageNameSet;
    getImageNamesSetFromSpecifiedPath(imageRootPath.toStdString(), _imageNameSet);

    cv::Mat matSrc;
    for (int i=0; i<_imageNameSet.size(); ++i)
    {

        matSrc = cv::imread(_imageNameSet[i], 2|4);
        if (!matSrc.data) {
            QMessageBox::warning(this, "warning", QStringLiteral("图像读取错误"));
            return;
        }

        int width = matSrc.cols;
        int height = matSrc.rows;

        Mat mat1;
        matSrc.copyTo(mat1);
        Mat matTranspose;
        cv::transpose(mat1, matTranspose);

        // clockwise rotation  90
        Mat matRotate90;
        cv::flip(matTranspose, matRotate90, 1);
        cv::Mat tmp2(width, height, CV_8UC1, matRotate90.data);
        cv::imwrite(QString(imageRootPath+"/"+QString::number(i)+"_90_.png").toStdString(), tmp2);

        // clockwise rotation 180
        Mat matRotate180;
        cv::flip(mat1, matRotate180, -1);
        cv::Mat tmp3(height, width, CV_8UC1, matRotate180.data);
        cv::imwrite(QString(imageRootPath+"/"+QString::number(i)+"_180_.png").toStdString(), tmp3);

        // clockwise rotation 270
        Mat matRotate270;
        cv::flip(matTranspose, matRotate270, 0);
        cv::Mat tmp4(matTranspose.rows, matTranspose.cols, CV_8UC1, matRotate270.data);
        cv::imwrite(QString(imageRootPath+"/"+QString::number(i)+"_270_.png").toStdString(), tmp4);
    }


    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("扩充图片数据集合成功"));
    svmTrainInputInfoEdit->append(QStringLiteral("扩充图片数据集合成功"));
    svmTrainInputInfoEdit->repaint();
}


void HogAndSVM_ObjectDetect_settingWidget::slot_singleImagePathOptionButton_clicked()
{
    QString fileFull = QFileDialog::getOpenFileName(this
                , QStringLiteral("输入单张图片的路径"), "./"
                , QStringLiteral("*.jpg; *.png; *.tif; *.tiff; *.bmp"));

    if (fileFull.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    QFileInfo file = QFileInfo(fileFull);
    QString fileName = file.fileName();
    QString filePath = file.absolutePath();

    singleimageReSizePathLineEdit->setText(fileFull);
    svmTrainInputInfoEdit->append(QStringLiteral("输入单张图片的路径-加载成功"));
    QString outputInfo = QStringLiteral("加载路径名为： ") + fileFull + "\n";
    svmTrainInputInfoEdit->append(outputInfo);

    Mat src = imread(fileFull.toStdString(), 2|4);

    outputInfo = QStringLiteral("加载图片的尺寸为:  ")
            + QString::number(src.cols) + " * " + QString::number(src.rows) + "\n";
    svmTrainInputInfoEdit->append(outputInfo);
    return;
}

void HogAndSVM_ObjectDetect_settingWidget::slot_singleImageResizeButton_clicked()
{
    QString imageRootPath = singleimageReSizePathLineEdit->text();      //输入图像的 根目录路径
    QFileInfo file = QFileInfo(imageRootPath);
    QString fileName = file.fileName();
    QString filePath = file.absolutePath();


    if (imageRootPath.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    if (imageReSizeWidthLineEdit->text().isEmpty()
            || imageReSizeHeightLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入图片的尺寸为空"));
        return ;
    }


    bool isImageResizeChecked = singleimageReSizeCheckBox->isChecked();
    int imageReSizeWidth = imageReSizeWidthLineEdit->text().toInt();
    int imageReSizeHeight= imageReSizeHeightLineEdit->text().toInt();

    if (!isImageResizeChecked)  //没有选中
    {
        QMessageBox::warning(this, "warning", QStringLiteral("不需要修改图片的尺寸"));
        return;
    }


    Mat imageMat;

    imageMat = imread(imageRootPath.toStdString(), 2|4);


//    //删除原来的 图像
//    remove(_imageNameSet[i].c_str());


    //重新写入 图片
    Mat dst;

    QString newFileName = "new_" + fileName;
    QString newFileAbsoluteName = filePath + "/" + newFileName;
    cv::resize(imageMat, dst, Size(imageReSizeWidth, imageReSizeHeight));
    imwrite(newFileAbsoluteName.toStdString(), dst);



    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("修改单张图片的尺寸成功"));
    svmTrainInputInfoEdit->append(QStringLiteral("修改单张图片的尺寸成功"));

}



//单张图片预测  路径选择
void HogAndSVM_ObjectDetect_settingWidget::slot_singleImagePredictPathOption()
{
    QString fileFull = QFileDialog::getOpenFileName(this
                , QStringLiteral("载入图片文件"), "./"
                , QStringLiteral("*.jpg; *.png; *.tif; *.tiff; *.bmp"));

    if (fileFull.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    QFileInfo file = QFileInfo(fileFull);
    QString fileName = file.fileName();
    QString filePath = file.absolutePath();

    singleImagePredictLineEdit->setText(fileFull);
    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("文件路径输入正确"));
    svmTrainInputInfoEdit->append(QStringLiteral("单张图片预测，文件路径设置成功"));

    QString outputInfo = QStringLiteral("单张图片路径为：") + fileFull + "\n";
    svmTrainInputInfoEdit->append(outputInfo);

}

//svm模型选择
void HogAndSVM_ObjectDetect_settingWidget::slot_singleImagePredictSvmOption()
{
    QString fileFull = QFileDialog::getOpenFileName(this
                , QStringLiteral("载入SVM模型文件"), "./"
                , QStringLiteral("*.xml"));

    if (fileFull.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    QFileInfo file = QFileInfo(fileFull);
    QString fileName = file.fileName();
    QString filePath = file.absolutePath();

    singleImagePredictSvmLineEdit->setText(fileFull);
    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("文件路径输入正确"));
    svmTrainInputInfoEdit->append(QStringLiteral("SVM模型选择，文件路径设置成功"));

    QString outputInfo = QStringLiteral("SVM模型选择为：") + fileFull + "\n";
    svmTrainInputInfoEdit->append(outputInfo);
}

void HogAndSVM_ObjectDetect_settingWidget::slot_singleImagePredict()
{
    QString imageFileNameQString = singleImagePredictLineEdit->text();
    QString svmTrainModelXMLNameQString = singleImagePredictSvmLineEdit->text();


    std::string imageFileName = imageFileNameQString.toStdString();         //图像路径
    std::string svmTrainModelXMLName = svmTrainModelXMLNameQString.toStdString() ;   //svm模型路径


    HogParameters hogParameters;
    hogParameters.blockSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockWin);
    hogParameters.winSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->detectWin);
    hogParameters.winStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->winStrider);
    hogParameters.blockStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockStrider);
    hogParameters.cellSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->cellWin);
    hogParameters.nbins = DateMgr::getDataParas()->hogFeatureParas->nbins;



    Mat src = imread(imageFileName, 0);
    float svmPredict = getSingleImageSvmPredictResult(src, svmTrainModelXMLName, hogParameters);

    QString outputInfo = QStringLiteral("该图片预测值为") + QString::number(svmPredict) + "\n";
    svmTrainInputInfoEdit->append(outputInfo);
}


//批量图片进行预测 路径选择
void HogAndSVM_ObjectDetect_settingWidget::slot_batchImagePredictPathOption()
{
    QString fileFull = QFileDialog::getOpenFileName(this
                , QStringLiteral("载入图片文件"), "./"
                , QStringLiteral("*.jpg; *.png; *.tif; *.tiff; *.bmp"));

    if (fileFull.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    QFileInfo file = QFileInfo(fileFull);
    QString fileName = file.fileName();
    QString filePath = file.absolutePath();

    batchImagePredictLineEdit->setText(filePath);
    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("文件路径输入正确"));
    svmTrainInputInfoEdit->append(QStringLiteral("批量图片预测，文件路径设置成功"));

    QString outputInfo = QStringLiteral("批量图片根路径为：") + filePath + "\n";
    svmTrainInputInfoEdit->append(outputInfo);
}

//批量图片进行预测  SVM模型选择
void HogAndSVM_ObjectDetect_settingWidget::slot_batchImagePredicctSvmOption()
{
    QString fileFull = QFileDialog::getOpenFileName(this
                , QStringLiteral("载入SVM模型文件"), "./"
                , QStringLiteral("*.xml"));

    if (fileFull.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径为空，请输入正确的文件路径"));
        return ;
    }

    QFileInfo file = QFileInfo(fileFull);
    QString fileName = file.fileName();
    QString filePath = file.absolutePath();

    batchImagePredictSvmLineEdit->setText(fileFull);
    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("文件路径输入正确"));
    svmTrainInputInfoEdit->append(QStringLiteral("SVM模型选择，文件路径设置成功"));

    QString outputInfo = QStringLiteral("SVM模型选择为：") + fileFull + "\n";
    svmTrainInputInfoEdit->append(outputInfo);
}

void HogAndSVM_ObjectDetect_settingWidget::slot_batchImagePredict()
{
    QString imageFileNameQString = batchImagePredictLineEdit->text();
    QString svmTrainModelXMLNameQString = batchImagePredictSvmLineEdit->text();


    std::string imageFileName = imageFileNameQString.toStdString();         //图像路径
    std::string svmTrainModelXMLName = svmTrainModelXMLNameQString.toStdString() ;   //svm模型路径


    HogParameters hogParameters;
    hogParameters.blockSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockWin);
    hogParameters.winSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->detectWin);
    hogParameters.winStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->winStrider);
    hogParameters.blockStride = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->blockStrider);
    hogParameters.cellSize = QSizeToSize(DateMgr::getDataParas()->hogFeatureParas->cellWin);
    hogParameters.nbins = DateMgr::getDataParas()->hogFeatureParas->nbins;

    vector<float> PredictResult;
    vector<string> imageNameSets;
    getImageNamesSetFromSpecifiedPath(imageFileName, imageNameSets);   //图片文件 指定路径
    getImageInBatchSvmPredictResult(imageFileName, svmTrainModelXMLName, hogParameters, PredictResult);

    QString outputInfo;
    QFileInfo file;
    QString fileName;
    QString filePath;

    for (int i=0; i<PredictResult.size(); ++i)
    {
        file = QFileInfo(imageNameSets[i].c_str());
        fileName = file.fileName();
//        QString filePath = file.absolutePath();

        outputInfo = fileName + QStringLiteral(" SVM训练后，预测值为： ")
                + QString::number(PredictResult[i]);
        svmTrainInputInfoEdit->append(outputInfo);
    }

    int trainPositiveCorrentNum = 0;
    for (int i=0; i<PredictResult.size(); ++i)
    {
        if ((PredictResult[i] - 1 >= -0.000001) && (PredictResult[i] - 1 <= 0.000001))
        {
            trainPositiveCorrentNum++;
        }
    }

    int trainNegativeCorrentNum = 0;
    for (int i=0; i<PredictResult.size(); ++i)
    {
        if ((PredictResult[i] + 1 >= -0.000001) && (PredictResult[i] + 1 <= 0.000001))
        {
            trainNegativeCorrentNum++;
        }
    }


    svmTrainInputInfoEdit->append(QStringLiteral("批量图片总个数为： ") + QString::number(imageNameSets.size()));

    svmTrainInputInfoEdit->append(QStringLiteral("The train Set predict result analyse:  \n"));
    outputInfo = QStringLiteral("trainPositiveCorrentNum: = ") + QString::number(trainPositiveCorrentNum)
            + QStringLiteral("，预测值比率为: = ")
            + QString::number(static_cast<float>(trainPositiveCorrentNum)/imageNameSets.size())
            + QStringLiteral("\n");
    svmTrainInputInfoEdit->append(outputInfo);

    outputInfo = QStringLiteral("trainNegativeCorrentNum: = ") + QString::number(trainNegativeCorrentNum)
            + QStringLiteral("，预测值比率为: = ")
            + QString::number(static_cast<float>(trainNegativeCorrentNum)/imageNameSets.size())
            + QStringLiteral("\n");
    svmTrainInputInfoEdit->append(outputInfo);
}



HogAndSVM_ObjectDetect_settingWidget_singleProduct::~HogAndSVM_ObjectDetect_settingWidget_singleProduct()
{

}



QMutex HogAndSVM_ObjectDetect_settingWidget_singleProduct::mutex;
HogAndSVM_ObjectDetect_settingWidget* HogAndSVM_ObjectDetect_settingWidget_singleProduct::instance = NULL;
HogAndSVM_ObjectDetect_settingWidget* HogAndSVM_ObjectDetect_settingWidget_singleProduct::getParameterSettingWidget()
{
    if (instance == NULL)
    {
        QMutexLocker locker(&mutex);  //add mutex locker

        if (instance == NULL)
        {
            instance = new HogAndSVM_ObjectDetect_settingWidget;
        }
    }
    return instance;
}







