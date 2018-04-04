#include "imagecomparearea.h"
#include <QtCore>


//左边图像展示区
QLeftImageWidget::QLeftImageWidget(QWidget* parent):QImageWidget(parent)
{
    this->setAcceptDrops(true);   //支持拖放功能
    isCurAppListWidgetDrag = false;
}

void QLeftImageWidget::paintEvent(QPaintEvent* e)     //在paintEvent中发射信号，实现同步画图
{
    QImageWidget::paintEvent(e);
    emit signals_deliver_tgtRect(tgtRect);
}


//void QLeftImageWidget::mouseMoveEvent(QMouseEvent* e)
//{
//    //qDebug() << "QImageWidget::mouseMoveEvent(QMouseEvent* e)";
//    //qDebug() << e->pos();

//    if (mLeftKeyStartedMove)   // 左键 实现了拖动
//    {
//        QPoint mousePoint = e->pos();
//        int y_offset = mousePoint.y() - mStartPosition.y();
//        int x_offset = mousePoint.x() - mStartPosition.x();

//        tgtRect = QRectF(tgtRect.x() + x_offset, tgtRect.y() + y_offset
//                       , tgtRect.width(),tgtRect.height());

//        if (y_offset >=1 || x_offset>=2)
//        {
//            update();
//            mStartPosition = mousePoint;

//        }

//    }

//    if (mRightKeyStartedMove)    // 右键 实现了截图
//    {
//        mEndPosition = e->pos();
//        update();
//    }
//}



//void QLeftImageWidget::mouseReleaseEvent(QMouseEvent* e)
//{
//    qDebug() << "mouseReleaseEvent(QMouseEvent* e)";
//    mRightKeyStartedMove = false ;
//    mLeftKeyStartedMove = false;
//    this->setCursor(Qt::ArrowCursor);
//    mEndPosition = e->pos();
//}

//只要展示的目标窗口发生了改变， 就更新图片区域
void QLeftImageWidget::slots_receive_tgtRect(QRectF rect)
{
    tgtRect = rect;
    update();
}

void QLeftImageWidget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "QLeftImageWidget::dragEnterEvent(QDragEnterEvent *event)";

    QList<QUrl> urls = event->mimeData()->urls();
    if (!urls.empty())
    {
        event->acceptProposedAction();
    }
}

void QLeftImageWidget::dropEvent(QDropEvent * event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    QString path = urls.first().toLocalFile();

    if (path.isEmpty())
    {
        return;
    }

    this->setImage(path);
}

void QLeftImageWidget::slots_Load_iamge_compare_left_area(QImage image)
{
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("载入缩略图有误，请重新输入正确的缩略图"));
        return;
    }
    this->setImage(image);
}





//右边图像展示区
QRightImageWidget::QRightImageWidget(QWidget* parent):QImageWidget(parent)
{
    this->setAcceptDrops(true);   //支持拖放功能
    isCurAppListWidgetDrag = false;
}


void QRightImageWidget::slots_receive_tgtRect(QRectF rect)
{
    tgtRect = rect;
    update();
}

//void QRightImageWidget::mouseMoveEvent(QMouseEvent* e)
//{
//    //qDebug() << "QImageWidget::mouseMoveEvent(QMouseEvent* e)";
//    //qDebug() << e->pos();

//    if (mLeftKeyStartedMove)   // 左键 实现了拖动
//    {
//        QPoint mousePoint = e->pos();
//        int y_offset = mousePoint.y() - mStartPosition.y();
//        int x_offset = mousePoint.x() - mStartPosition.x();

//        tgtRect = QRectF(tgtRect.x() + x_offset, tgtRect.y() + y_offset
//                       , tgtRect.width(),tgtRect.height());

//        if (y_offset >=1 || x_offset>=1)
//        {
//            update();
//            mStartPosition = mousePoint;

//        }


//    }

//    if (mRightKeyStartedMove)    // 右键 实现了截图
//    {
//        mEndPosition = e->pos();
//        update();
//    }
//}



//void QRightImageWidget::mouseReleaseEvent(QMouseEvent* e)
//{
//    qDebug() << "mouseReleaseEvent(QMouseEvent* e)";
//    mRightKeyStartedMove = false ;
//    mLeftKeyStartedMove = false;
//    this->setCursor(Qt::ArrowCursor);
//    mEndPosition = e->pos();
//}


void QRightImageWidget::paintEvent(QPaintEvent* e)     //在paintEvent中发射信号，实现同步画图
{
    QImageWidget::paintEvent(e);
    emit signals_deliver_tgtRect(tgtRect);
}


void QRightImageWidget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "QLeftImageWidget::dragEnterEvent(QDragEnterEvent *event)";

    QList<QUrl> urls = event->mimeData()->urls();
    if (!urls.empty())
    {
        event->acceptProposedAction();
    }
}

void QRightImageWidget::dropEvent(QDropEvent * event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    QString path = urls.first().toLocalFile();

    if (path.isEmpty())
    {
        return;
    }

    this->setImage(path);
}

void QRightImageWidget::slots_Load_iamge_compare_right_area(QImage image)
{
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("载入缩略图有误，请重新输入正确的缩略图"));
        return;
    }

    this->setImage(image);
}




//总的展示窗口
QImageCompareWidget::QImageCompareWidget(QWidget* parent):QWidget(parent)
{

    init();

    leftImageWidget->setAcceptDrops(true);
    rightImageWidget->setAcceptDrops(true);
    this->setAcceptDrops(true);

    //注释了 监控器件
    //leftImageWidget->installEventFilter(this);   //必须写定安装 事件过滤
    //rightImageWidget->installEventFilter(this);   //必须写定安装 事件过滤


    signals_slots_binds();
}

void QImageCompareWidget::init()
{
    //左展示区 init
    leftImageWidget = new QLeftImageWidget;
    leftImageWidget->setMinimumSize(1000, 1000);
    leftScroll = new QScrollArea;

    leftScroll->setBackgroundRole(QPalette::Dark);  //背景色
    leftScroll->setWidget(leftImageWidget);
    leftScroll->setAlignment(Qt::AlignCenter);
    leftScroll->setWidgetResizable(true);           // 自动调整大小
    leftScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    leftScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


    //右展示区 init
    rightImageWidget = new QRightImageWidget;
    rightImageWidget->setMinimumSize(1000, 1000);
    rightScroll = new QScrollArea;

    rightScroll->setBackgroundRole(QPalette::Dark);  //背景色
    rightScroll->setWidget(rightImageWidget);
    rightScroll->setAlignment(Qt::AlignCenter);
    rightScroll->setWidgetResizable(true);          // 自动调整大小
    rightScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    rightScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    leftHBar = leftScroll->horizontalScrollBar();
    leftVBar = leftScroll->verticalScrollBar();

    rightHBar = rightScroll->horizontalScrollBar();
    rightVBar = rightScroll->verticalScrollBar();

    layout = new QHBoxLayout;
    layout->addWidget(leftScroll);
    layout->addWidget(rightScroll);
    layout->setSpacing(20);

    this->setLayout(layout);
}

void QImageCompareWidget::signals_slots_binds()
{
    connect(leftImageWidget, SIGNAL(signals_deliver_tgtRect(QRectF))
            ,rightImageWidget, SLOT(slots_receive_tgtRect(QRectF)));  //同步操作

    connect(rightImageWidget, SIGNAL(signals_deliver_tgtRect(QRectF))
            ,leftImageWidget, SLOT(slots_receive_tgtRect(QRectF)));

    connect(leftHBar, SIGNAL(valueChanged(int))
            ,rightHBar, SLOT(setValue(int)));

    connect(leftVBar, SIGNAL(valueChanged(int))
            ,rightVBar, SLOT(setValue(int)));

    connect(rightHBar, SIGNAL(valueChanged(int))
            ,leftHBar, SLOT(setValue(int)));

    connect(rightVBar, SIGNAL(valueChanged(int))
            ,leftVBar, SLOT(setValue(int)));
}


// 实现控件的 时时监控
bool QImageCompareWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == leftImageWidget) {
            if (event->type() == QEvent::DragEnter) {
                qDebug() << "DragEnter   ";
                // [[2]]: 当拖放时鼠标进入label时, label接受拖放的动作
                QDragEnterEvent *dee = dynamic_cast<QDragEnterEvent *>(event);
                dee->acceptProposedAction();
                return true;
            } else if (event->type() == QEvent::Drop) {
                // [[3]]: 当放操作发生后, 取得拖放的数据
                qDebug() << "Drop   ";

                QDropEvent *de = dynamic_cast<QDropEvent *>(event);

                qDebug() << "QRightImageWidget::dropEvent(QDropEvent * event)";
                QList<QUrl> urls = de->mimeData()->urls();
                QString path = urls.first().toLocalFile();
                qDebug() << path;


                QVariant variant =  de->mimeData()->imageData();   //获取图像数据
                QImage image = variant.value<QImage>();

                if (image.isNull())
                {
                    return false;
                }

                leftImageWidget->setImage(image);

                return true;
            }
        }

        return QWidget::eventFilter(watched, event);
}


void QImageCompareWidget::slots_Load_iamge_compare_left_area(QImage image)
{
    qDebug() << "QImageCompareWidget::slots_Load_iamge_compare_left_area(QImage image)";
    leftImageWidget->slots_Load_iamge_compare_left_area(image);
}

void QImageCompareWidget::slots_Load_iamge_compare_right_area(QImage image)
{
    qDebug() << "QImageCompareWidget::slots_Load_iamge_compare_right_area(QImage image)";

    rightImageWidget->slots_Load_iamge_compare_right_area(image);
}
