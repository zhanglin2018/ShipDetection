#include "imagePreviewWnds.h"
#include <QTreeView>
#include <QDir>
#include <QIcon>
#include <QDebug>
#include "global.h"
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QKeyEvent>
#include <QFileDialog>
#include "imageview.h"

CImgNameListWidget::CImgNameListWidget(QWidget *parent /* = NULL */)
	: QListWidget(parent)
{

}

CImgNameListWidget::~CImgNameListWidget()
{

}

void CImgNameListWidget::updateImgsName()
{
    /*clear();

	if (CDataMgr::TARGET_DETECT == CDataMgr::getInstance()->m_nCurDetect_Type)
	{	
			QDir qFileDir(CDataMgr::getInstance()->m_strFileRootPath);
			if (qFileDir.exists())
			{
				for (int i = 0; i < CDataMgr::getInstance()->m_pTargetDetObj->GetFileList().size(); i++)
				{
					addItem(QString::number(i + 1) + ". " + CDataMgr::getInstance()->m_pTargetDetObj->GetFileList()[i].fileName());
				}
			}
	}
	else if (CDataMgr::COMPARISON_DETECT == CDataMgr::getInstance()->m_nCurDetect_Type)
	{
			QDir qSrcFileDir(CDataMgr::getInstance()->m_strSrcFileRootPath);
			if (qSrcFileDir.exists())
			{
				CDataMgr::getInstance()->m_ChangeDetSrcfileInfoList = qSrcFileDir.entryInfoList(QDir::Files, QDir::Name);
			   for (int i = 0; i < CDataMgr::getInstance()->m_ChangeDetSrcfileInfoList.size(); i++)
			   {
				   addItem(QString::number(i + 1) + ". " + CDataMgr::getInstance()->m_ChangeDetSrcfileInfoList[i].fileName());
			   }
		   }
			QDir qRefFileDir(CDataMgr::getInstance()->m_strFileRootPath);
			if (qSrcFileDir.exists())
			{
				CDataMgr::getInstance()->m_ChangeDetReffileInfoList = qRefFileDir.entryInfoList(QDir::Files, QDir::Name);
			}
    }*/
}













//显示缩略列表
ClitImgListWidget::ClitImgListWidget(QWidget *parent)
    : QWidget(parent)
{
    this->initialization();
    this->setMouseTracking(true);         //设置鼠标 移动事件

    listWidget->setDragEnabled(true);

    listWidget->setDragDropMode(QAbstractItemView::DragOnly);


    connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*))
            ,this, SLOT(slot_itemDoubleClicked()));

    connect(listWidget, SIGNAL(customContextMenuRequested(QPoint))
            ,this, SLOT(slot_customContextMenuRequested(QPoint)));
}

ClitImgListWidget::~ClitImgListWidget()
{
    imageNameList.clear();
    imageMap.clear();
    imagePathList.clear();
    processNameList.clear();
}

void ClitImgListWidget::initialization()
{

    listWidget = new QListWidget(this);
    listWidget->setIconSize(QSize(90, 60));        //设置item图标大小
    listWidget->setResizeMode(QListView::Adjust);  //设置QListWidget大小调整时，图标的调整模式为 自动调整 adjust
    listWidget->setViewMode(QListView::IconMode);  //设置viewModel，来确定使用不同的视图进行显示
                                                    //使ListWidgetItem中的图标在上，文字在下
    listWidget->setMovement(QListView::Static);    //设置列表固定不动
    listWidget->setSpacing(5);

    listWidget->setContextMenuPolicy(Qt::CustomContextMenu);  //设置接受菜单
    layout = new QVBoxLayout;
    layout->addWidget(listWidget);
    this->setLayout(layout);
}

void ClitImgListWidget::clearAllImageAndNameList()
{
    if (!imageNameList.isEmpty())
    {
        imageNameList.clear();
    }

    if (!imageMap.isEmpty())
    {
        imageMap.clear();
    }

    if (!processNameList.isEmpty())
    {
        processNameList.clear();
    }

    if (!imagePathList.isEmpty())
    {
        imagePathList.clear();
    }
}

void ClitImgListWidget::slot_addListItem()    //往最后面添加小图
{   
    if (imageMap.size() == 0 || imageNameList.size() == 0)
    {
        return ;
    }

    int num = imageNameList.size();
    QString strName = imageNameList[num-1];
    QIcon icon(QPixmap::fromImage(imageMap[num-1]));
    QListWidgetItem * item = new QListWidgetItem(icon, strName);

    listWidget->addItem(item);
}


void ClitImgListWidget::slot_addQImage(QImage image)
{
    qDebug() << "ClitImgListWidget::slot_addQPixmap(QPixmap pixmap)";
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("请添加正确的缩略图"));
        return;
    }

    int num = imageNameList.size();
    QString strName = "image_" + QString::number(num+1) + ".jpg";
    QString processName = "";
    processNameList.push_back(processName);
    imageNameList.push_back(strName);
    imageMap.push_back(image);

    imagePathList.push_back(ImageDescriptionParas::getImageDescriptionParas()->imagePath);

    slot_addListItem();
}

void ClitImgListWidget::slot_itemDoubleClicked()
{
    qDebug() << "ClitImgListWidget::slot_itemDoubleClicked(QPixmap pixmap)";
    int currentNum = listWidget->currentRow();  //从0行开始

    QImage currentMap = imageMap[currentNum];

    ImageDescriptionParas::getImageDescriptionParas()->imagePath = imagePathList[currentNum];
    emit signals_listItemDoubleClicked(currentMap);
}


// 右键菜单栏实现
void ClitImgListWidget::slot_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem * item = listWidget->itemAt(pos);
    if (item == NULL)
    {
        return;
    }

    QMenu *popMenu = new QMenu( this );
    QAction *actionDelete = new QAction(QStringLiteral("Delete this image"), this);
    QAction *actionClear = new QAction(QStringLiteral("Clear all image"), this);
    QAction *actionSave = new QAction(QStringLiteral("Save this image"), this);
    QAction *actionSaveAll = new QAction(QStringLiteral("Save all images"), this);
    QAction *actionLoadLeftImage = new QAction(QStringLiteral("Load image to Left Compare Area"), this);
    QAction *actionLoadRightImage = new QAction(QStringLiteral("Load image to Right Compare Area"), this);


    popMenu->addAction( actionDelete );
    popMenu->addAction( actionClear );
    popMenu->addAction( actionSave  );
    popMenu->addAction( actionSaveAll  );
    popMenu->addAction( actionLoadLeftImage  );
    popMenu->addAction( actionLoadRightImage  );



    connect( actionDelete, SIGNAL(triggered() ), this, SLOT( slot_deleteListItem()) );
    connect( actionClear, SIGNAL(triggered() ), this, SLOT( slot_clearList()) );
    connect( actionSave, SIGNAL(triggered() ), this, SLOT( slot_saveListItem()) );
    connect( actionSaveAll, SIGNAL(triggered() ), this, SLOT( slot_saveAllListItem()) );

    connect( actionLoadLeftImage, SIGNAL(triggered() ), this, SLOT( slot_Load_iamge_compare_left_area()) );
    connect( actionLoadRightImage, SIGNAL(triggered() ), this, SLOT( slot_Load_iamge_compare_right_area()) );

    popMenu->exec(QCursor::pos());
}

void ClitImgListWidget::slot_deleteListItem()
{
    int ch = QMessageBox::warning(NULL, "Warning",
                                      "Are you sure to delete this image?",
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::Yes);

    if ( ch != QMessageBox::Yes )
        return;

    QListWidgetItem * item = listWidget->currentItem();
    if( item == NULL )
    {
        return;
    }

    int curIndex = listWidget->row(item);
    listWidget->takeItem(curIndex);

    imageNameList.removeAt(curIndex);
    imageMap.removeAt(curIndex);
    processNameList.removeAt(curIndex);
    imagePathList.removeAt(curIndex);
}

void ClitImgListWidget::slot_clearList()
{
    int ch = QMessageBox::warning(NULL, "Warning",
                                      "Are you sure to clear all images ?",
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::Yes);

    if ( ch != QMessageBox::Yes )
    {
        return;
    }

    QListWidgetItem * item = listWidget->currentItem();
    if( item == NULL )
    {
        return;
    }

    clearAllImageAndNameList();
    listWidget->clear();
}

//保存图像
void ClitImgListWidget::slot_saveListItem()
{
    int ch = QMessageBox::warning(NULL, "Warning",
                                      "Are you sure to save this image?",
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::Yes);

    if ( ch != QMessageBox::Yes )
        return;

    QListWidgetItem * item = listWidget->currentItem();
    if( item == NULL )
    {
        return;
    }

    int curIndex = listWidget->row(item);


    QString imageRootPath = QFileDialog::getSaveFileName(this
                , QStringLiteral("载入图片文件"), "./detect_result_image"
                , QStringLiteral("*.jpg; *.png; *.tif; *.tiff; *.bmp"));


    if (imageRootPath.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("输入路径有误，请输入正确的图片文件保存路径"));
        return ;
    }

    QImage curImage = imageMap[curIndex];
    QString imageName = imageNameList[curIndex];
    QString imagePath = imageRootPath + "_" + imageName;

    curImage.save(imagePath);

    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("图片保存成功"));
    return;
}


//保存所有图像
void ClitImgListWidget::slot_saveAllListItem()
{
    int ch = QMessageBox::warning(NULL, "Warning",
                                      "Are you sure to save all images?",
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::Yes);

    if ( ch != QMessageBox::Yes )
        return;



    if (imageMap.isEmpty())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("The Cut Image Area Is Empty!"));

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



    QImage curImage;
    QString imagePath;
    for (int i=0; i<imageMap.size(); i++)
    {
        imagePath = imageRootPath + "_" + imageNameList[i];
        curImage = imageMap[i];
        curImage.save(imagePath);
    }

    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("所有图片保存成功"));
    return;
}


void ClitImgListWidget::slot_Load_iamge_compare_left_area()
{

    int ch = QMessageBox::warning(NULL, "Warning",
                                      "Are you sure to Load this image to Left Compare Area?",
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::Yes);

    if ( ch != QMessageBox::Yes )
        return;

    QListWidgetItem * item = listWidget->currentItem();
    if( item == NULL )
    {
        return;
    }

    int curIndex = listWidget->row(item);


    QImage curImage = imageMap[curIndex];

    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("图片加载成功"));


    emit signals_Load_iamge_compare_left_area(curImage);
    return;

}

void ClitImgListWidget::slot_Load_iamge_compare_right_area()
{

    int ch = QMessageBox::warning(NULL, "Warning",
                                      "Are you sure to Load this image to Right Compare Area?",
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::Yes);

    if ( ch != QMessageBox::Yes )
        return;

    QListWidgetItem * item = listWidget->currentItem();
    if( item == NULL )
    {
        return;
    }

    int curIndex = listWidget->row(item);

    QImage curImage = imageMap[curIndex];

    emit signals_Load_iamge_compare_right_area(curImage);


    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("图片加载成功"));
    return;

}


void ClitImgListWidget::slot_receive_little_image(QImage image, QString str)
{
    //qDebug() << "ClitImgListWidget::slot_receive_little_image(QImage image, QString str)";
    if (image.isNull())
    {
        QMessageBox::warning(this, "warning", QStringLiteral("图像处理结果格式错误"));
        return;
    }

    int num = imageNameList.size();


    QListWidgetItem * item = listWidget->currentItem();
    QString strName;
    if( item == NULL )
    {
        QString processName = processNameList[num-1] + "_" + str;
        processNameList.push_back(processName);

        strName = "image" + processName + QString::number(num+1) + ".jpg";

        imageNameList.push_back(strName);
        imageMap.push_back(image);
        imagePathList.push_back(ImageDescriptionParas::getImageDescriptionParas()->imagePath);
    }
    else
    {
        int curIndex = listWidget->row(item);
        QString processName = processNameList[curIndex] + "_" + str;
        processNameList.push_back(processName);


        strName = "image" + processName + QString::number(num+1) + ".jpg";


        imageNameList.push_back(strName);
        imageMap.push_back(image);
        imagePathList.push_back(ImageDescriptionParas::getImageDescriptionParas()->imagePath);
    }



    QPixmap pixmap = QPixmap::fromImage(image);
    item = new QListWidgetItem(QIcon(pixmap), strName);

    listWidget->addItem(item);
    listWidget->setCurrentItem(item);
}




void ClitImgListWidget::keyPressEvent(QKeyEvent* e)
{
    qDebug() << "keyPressEvent(QKeyEvent* e)";
    switch (e->key())
    {

        case Qt::Key_Delete:

            slot_deleteListItem();

            break;
        default:
            break;
    }
}



//void ClitImgListWidget::mousePressEvent(QMouseEvent *event)
//{
//    qDebug() << "ClitImgListWidget::mousePressEvent(QMouseEvent *event)" ;

//    if (event->button() == Qt::LeftButton)
//    {
//        startPos = event->pos();
//        qDebug() << startPos;
//    }
//}

//void ClitImgListWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    qDebug() << "ClitImgListWidget::mouseMoveEvent(QMouseEvent *event)" ;


//    start_drag();

//}


//void ClitImgListWidget::start_drag()
//{
//    qDebug() << "start_drag" ;
//    QListWidgetItem * item = listWidget->currentItem();
//    if( item == NULL )
//    {
//        return;
//    }

//    int curIndex = listWidget->row(item);

//    QPixmap curPixmap = imageMap[curIndex];
//    QImage curImage = curPixmap.toImage();

//    qDebug() << curImage.size();


//    QMimeData *mimeData = new QMimeData;
//    mimeData->setImageData(curImage);

//    QImage image2 = mimeData->imageData().value<QImage>();
//    qDebug() << image2.size();



//    QDrag *drag = new QDrag(this);
//    drag->setMimeData(mimeData);
//}














//设置log信息表格
CLogInfoWidget::CLogInfoWidget(QWidget *parent /* = NULL */)
	: QTextEdit(parent)
{
    //setReadOnly(true);
}

CLogInfoWidget::~CLogInfoWidget()
{

}

void CLogInfoWidget::updateLogInfo(const QString &strInfo)
{
    //append("\n" + strInfo);
}
