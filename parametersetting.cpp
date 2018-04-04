#include "parametersetting.h"
#include "datemgr.h"
#include <QHeaderView>
#include <QVector>
#include <QString>
#include <QSize>
#include <QMessageBox>
#include <QDebug>

//create single ParameterWidget Object



//input two number    transform to QString
QString ParameterSettingWidget::str_to_lineEdit(int A, int B)
{
    return "(" + QString::number(A) + ", " + QString::number(B) + ")";
}


//input QString transform to QSize

QSize ParameterSettingWidget::lineEdit_to_QSize(QString str)
{
    QVector<QString> strVec;
    for (int i=0; i<str.length(); i++)
    {
        QString temp;
        while (str[i] >='0' && str[i]<='9')
        {
            temp += str[i];
            i++;
        }
        if (!temp.isEmpty())
        {
            strVec.push_back(temp);
            temp.clear();
        }
    }

    QSize size;

    if (strVec.size() == 2)
    {
        bool ok0, ok1;
        size.setWidth(strVec[0].toInt(&ok0));
        size.setHeight(strVec[1].toInt(&ok1));
    }

    return size;
}



QMutex ParameterSettingWidget::mutex;
ParameterSettingWidget* ParameterSettingWidget::instance = NULL;
ParameterSettingWidget* ParameterSettingWidget::getParameterSettingWidget()
{
    if (instance == NULL)
    {
        QMutexLocker locker(&mutex);  //add mutex locker

        if (instance == NULL)
        {
            instance = new ParameterSettingWidget;
        }
    }
    return instance;
}





ParameterSettingWidget::ParameterSettingWidget(QWidget *parent /* = NULL */)
: QWidget(parent)
, m_pMainLayout(NULL)
, m_pGridLayout(NULL)
, m_pFilterLayout(NULL)
, m_pBinaryMorphLayout(NULL)
, m_pCfarLayout(NULL)
, m_pHogLayout(NULL)
, m_pSVMLayout(NULL)
, m_pRadarLayout(NULL)
, m_pConfigBtnLayout(NULL)
, m_pFilterBox(NULL)
, m_pBinaryMorphBox(NULL)
, m_pCfarBox(NULL)
, m_pHogBox(NULL)
, m_pSVMBox(NULL)
, m_pRadarBox(NULL)
, m_pFilterTableWnd(NULL)
, m_pBinaryMorphTableWnd(NULL)
, m_pCfarTableWnd(NULL)
, m_pHogTableWnd(NULL)
, m_pSVMTableWnd(NULL)
, m_pRadarTableWnd(NULL)
, m_pConfigBtn(NULL)
{

    this->setMinimumSize(1800, 800);
    m_pReg = new QDoubleValidator(this);

    createControls();
    layoutControls();

    initTableHeaderWnd(m_pFilterTableWnd);
    initTableHeaderWnd(m_pBinaryMorphTableWnd);
    initTableHeaderWnd(m_pCfarTableWnd);
    initTableHeaderWnd(m_pHogTableWnd);
    initTableHeaderWnd(m_pSVMTableWnd);
    initTableHeaderWnd(m_pRadarTableWnd);



    initFilterTableItem();
    initBinaryMorphItem();
    initCfarTableItem();
    initHogTableItem();
    initSVMTableItem();
    initRadarTableItem();

    setFilterTableItem();
    setBinaryMorphItem();
    setCfarTableItem();
    setHogTableItem();
    setSVMTableItem();
    setRadarTableItem();

    setFilterParas();
    setBinaryMorphParas();
    setCfarParas();
    setHogParas();
    setSvmParas();
    setRadarParas();

    connect(m_pConfigBtn, SIGNAL(clicked()), this, SLOT(slot_ConfigBtnClicked()));
    connect(m_pLoadDefaultConfig, SIGNAL(clicked()), this, SLOT(slot_LoadDefaultParas()));

    connect(this, SIGNAL(signal_ConfigDone()), this, SLOT(hide()));    //paras success  and  hide()
}

ParameterSettingWidget::~ParameterSettingWidget()
{

}




void ParameterSettingWidget::createControls()
{
    m_pMainLayout = new QVBoxLayout();
    m_pGridLayout = new QGridLayout();

    m_pFilterBox = new QGroupBox(QStringLiteral("图像滤波参数"), this);
    m_pBinaryMorphBox = new QGroupBox(QStringLiteral("图像二值化与形态学参数"), this);
    m_pCfarBox = new QGroupBox(QStringLiteral("CFAR检测参数"), this);
    m_pHogBox = new QGroupBox(QStringLiteral("HOG特征参数"), this);
    m_pSVMBox = new QGroupBox(QStringLiteral("SVM参数"), this);
    m_pRadarBox = new QGroupBox(QStringLiteral("目标检测参数"), this);

    m_pFilterLayout = new QHBoxLayout();
    m_pBinaryMorphLayout = new QHBoxLayout();
    m_pCfarLayout = new QHBoxLayout();
    m_pHogLayout = new QHBoxLayout();
    m_pSVMLayout = new QHBoxLayout();
    m_pRadarLayout = new QHBoxLayout();
    m_pConfigBtnLayout = new QHBoxLayout();

    m_pFilterTableWnd = new QTableWidget(this);
    m_pBinaryMorphTableWnd = new QTableWidget(this);
    m_pCfarTableWnd = new QTableWidget(this);
    m_pHogTableWnd = new QTableWidget(this);
    m_pSVMTableWnd = new QTableWidget(this);
    m_pRadarTableWnd = new QTableWidget(this);

    m_pConfigBtn = new QPushButton(QStringLiteral("参数配置"), this);
    m_pLoadDefaultConfig = new QPushButton(QStringLiteral("加载默认参数"), this);
}

void ParameterSettingWidget::layoutControls()
{
    m_pFilterLayout->addWidget(m_pFilterTableWnd);
    m_pBinaryMorphLayout->addWidget(m_pBinaryMorphTableWnd);
    m_pCfarLayout->addWidget(m_pCfarTableWnd);
    m_pHogLayout->addWidget(m_pHogTableWnd);
    m_pSVMLayout->addWidget(m_pSVMTableWnd);
    m_pRadarLayout->addWidget(m_pRadarTableWnd);

    m_pConfigBtnLayout->addStretch();
    m_pConfigBtnLayout->addWidget(m_pConfigBtn);
    m_pConfigBtnLayout->addWidget(m_pLoadDefaultConfig);

    m_pFilterBox->setLayout(m_pFilterLayout);
    m_pBinaryMorphBox->setLayout(m_pBinaryMorphLayout);
    m_pCfarBox->setLayout(m_pCfarLayout);
    m_pHogBox->setLayout(m_pHogLayout);
    m_pSVMBox->setLayout(m_pSVMLayout);
    m_pRadarBox->setLayout(m_pRadarLayout);

    m_pGridLayout->addWidget(m_pFilterBox, 0, 0);
    m_pGridLayout->addWidget(m_pBinaryMorphBox, 0, 1);
    m_pGridLayout->addWidget(m_pCfarBox, 0, 2);
    m_pGridLayout->addWidget(m_pHogBox, 0, 3);
    m_pGridLayout->addWidget(m_pSVMBox, 0, 4);
    m_pGridLayout->addWidget(m_pRadarBox, 0, 5);


    m_pMainLayout->addLayout(m_pGridLayout);
    m_pMainLayout->addLayout(m_pConfigBtnLayout);

    setLayout(m_pMainLayout);
}

void ParameterSettingWidget::initTableHeaderWnd(QTableWidget *pTableWidget)
{
    pTableWidget->setColumnCount(2);  //设置成两列
    pTableWidget->setFrameShape(QFrame::NoFrame);    //设置tableWidget为无边框
    pTableWidget->verticalHeader()->setVisible(false);
    pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList header;
    header << QStringLiteral("参数名称") << QStringLiteral("参数值");
    pTableWidget->setHorizontalHeaderLabels(header);
}

//init
void ParameterSettingWidget::initFilterTableItem()
{
    QStringList paraNameList;
    paraNameList << QStringLiteral("depth") << QStringLiteral("kernel")
                 << QStringLiteral("normalize")
        << QStringLiteral("borderType") << QStringLiteral("sigmaX")
        << QStringLiteral("sigmaY") << QStringLiteral("sigmaColor")
        << QStringLiteral("sigmaSpace");

    m_pFilterTableWnd->setRowCount(paraNameList.size());
    for (int i = 0; i < paraNameList.size(); i++)
    {
        QLineEdit *pLineEdit = new QLineEdit("1", this);
        pLineEdit->setValidator(m_pReg);

        m_pFilterTableWnd->setItem(i, 0, new QTableWidgetItem(paraNameList[i]));
        m_pFilterTableWnd->setCellWidget(i, 1, pLineEdit);
    }
}

//set FilterParas
void ParameterSettingWidget::setFilterTableItem()
{
    QVector<QLineEdit *> filterVec;
    for (int i = 0; i < m_pFilterTableWnd->rowCount(); i++)
    {
        filterVec.push_back(dynamic_cast<QLineEdit *>(m_pFilterTableWnd->cellWidget(i, 1)));
    }

    filterVec[0]->setText(QString::number(DateMgr::getDataParas()->imageFilterParas->depth));




    filterVec[1]->setText(str_to_lineEdit(DateMgr::getDataParas()->imageFilterParas->kernel.width(),
                                          DateMgr::getDataParas()->imageFilterParas->kernel.height()));


    filterVec[2]->setText(QString::number(DateMgr::getDataParas()->imageFilterParas->normalize));
    filterVec[3]->setText(QString::number(DateMgr::getDataParas()->imageFilterParas->borderType));
    filterVec[4]->setText(QString::number(DateMgr::getDataParas()->imageFilterParas->sigmaX));
    filterVec[5]->setText(QString::number(DateMgr::getDataParas()->imageFilterParas->sigmaY));
    filterVec[6]->setText(QString::number(DateMgr::getDataParas()->imageFilterParas->sigmaColor));
    filterVec[7]->setText(QString::number(DateMgr::getDataParas()->imageFilterParas->sigmaSpace));
}

void ParameterSettingWidget::setFilterParas()
{
    QVector<QLineEdit *> filterVec;
    for (int i = 0; i < m_pFilterTableWnd->rowCount(); i++)
    {
        filterVec.push_back(dynamic_cast<QLineEdit *>(m_pFilterTableWnd->cellWidget(i, 1)));
    }


    DateMgr::getDataParas()->imageFilterParas->depth = filterVec[0]->text().toInt();
    DateMgr::getDataParas()->imageFilterParas->kernel = lineEdit_to_QSize(filterVec[1]->text());
    DateMgr::getDataParas()->imageFilterParas->normalize = filterVec[2]->text().toInt();
    DateMgr::getDataParas()->imageFilterParas->borderType = filterVec[3]->text().toInt();
    DateMgr::getDataParas()->imageFilterParas->sigmaX = filterVec[4]->text().toDouble();
    DateMgr::getDataParas()->imageFilterParas->sigmaY = filterVec[5]->text().toDouble();
    DateMgr::getDataParas()->imageFilterParas->sigmaColor = filterVec[6]->text().toDouble();
    DateMgr::getDataParas()->imageFilterParas->sigmaSpace = filterVec[7]->text().toDouble();

    qDebug() << DateMgr::getDataParas()->imageFilterParas->depth
             << DateMgr::getDataParas()->imageFilterParas->kernel
                   << DateMgr::getDataParas()->imageFilterParas->normalize
                      << DateMgr::getDataParas()->imageFilterParas->borderType
                         << DateMgr::getDataParas()->imageFilterParas->sigmaX
                            << DateMgr::getDataParas()->imageFilterParas->sigmaY
                               << DateMgr::getDataParas()->imageFilterParas->sigmaColor
                                  << DateMgr::getDataParas()->imageFilterParas->sigmaSpace;
}


//二值化 与 形态学
//init
void ParameterSettingWidget::initBinaryMorphItem()
{
    QStringList paraNameList;
    paraNameList << QStringLiteral("threshold") << QStringLiteral("maxThreshold")
                 << QStringLiteral("blockSize")
        << QStringLiteral("C") << QStringLiteral("dilateSize")
        << QStringLiteral("erodeSize") << QStringLiteral("structuringElement");

    m_pBinaryMorphTableWnd->setRowCount(paraNameList.size());
    for (int i = 0; i < paraNameList.size(); i++)
    {
        QLineEdit *pLineEdit = new QLineEdit("1", this);
        pLineEdit->setValidator(m_pReg);

        m_pBinaryMorphTableWnd->setItem(i, 0, new QTableWidgetItem(paraNameList[i]));
        m_pBinaryMorphTableWnd->setCellWidget(i, 1, pLineEdit);
    }
}

//set FilterParas
void ParameterSettingWidget::setBinaryMorphItem()
{
    QVector<QLineEdit *> binaryMorphVec;
    for (int i = 0; i < m_pBinaryMorphTableWnd->rowCount(); i++)
    {
        binaryMorphVec.push_back(dynamic_cast<QLineEdit *>(m_pBinaryMorphTableWnd->cellWidget(i, 1)));
    }

    binaryMorphVec[0]->setText(QString::number(DateMgr::getDataParas()->imageBinaryAndMorphParas->threshold));

    binaryMorphVec[1]->setText(QString::number(DateMgr::getDataParas()->imageBinaryAndMorphParas->maxThreshold));
    binaryMorphVec[2]->setText(QString::number(DateMgr::getDataParas()->imageBinaryAndMorphParas->blockSize));
    binaryMorphVec[3]->setText(QString::number(DateMgr::getDataParas()->imageBinaryAndMorphParas->C));
    binaryMorphVec[4]->setText(QString::number(DateMgr::getDataParas()->imageBinaryAndMorphParas->dilateSize));
    binaryMorphVec[5]->setText(QString::number(DateMgr::getDataParas()->imageBinaryAndMorphParas->erodeSize));
    binaryMorphVec[6]->setText(QString::number(DateMgr::getDataParas()->imageBinaryAndMorphParas->structuringElement));
}

void ParameterSettingWidget::setBinaryMorphParas()
{
    QVector<QLineEdit *> binaryMorphVec;
    for (int i = 0; i < m_pBinaryMorphTableWnd->rowCount(); i++)
    {
        binaryMorphVec.push_back(dynamic_cast<QLineEdit *>(m_pBinaryMorphTableWnd->cellWidget(i, 1)));
    }


    DateMgr::getDataParas()->imageBinaryAndMorphParas->threshold = binaryMorphVec[0]->text().toDouble();
    DateMgr::getDataParas()->imageBinaryAndMorphParas->maxThreshold = binaryMorphVec[1]->text().toDouble();
    DateMgr::getDataParas()->imageBinaryAndMorphParas->blockSize = binaryMorphVec[2]->text().toInt();
    DateMgr::getDataParas()->imageBinaryAndMorphParas->C = binaryMorphVec[3]->text().toDouble();
    DateMgr::getDataParas()->imageBinaryAndMorphParas->dilateSize = binaryMorphVec[4]->text().toInt();
    DateMgr::getDataParas()->imageBinaryAndMorphParas->erodeSize = binaryMorphVec[5]->text().toInt();
    DateMgr::getDataParas()->imageBinaryAndMorphParas->structuringElement = binaryMorphVec[6]->text().toInt();

    qDebug() << DateMgr::getDataParas()->imageBinaryAndMorphParas->threshold
             << DateMgr::getDataParas()->imageBinaryAndMorphParas->maxThreshold
                   << DateMgr::getDataParas()->imageBinaryAndMorphParas->blockSize
                      << DateMgr::getDataParas()->imageBinaryAndMorphParas->C
                         << DateMgr::getDataParas()->imageBinaryAndMorphParas->dilateSize
                            << DateMgr::getDataParas()->imageBinaryAndMorphParas->erodeSize
                               << DateMgr::getDataParas()->imageBinaryAndMorphParas->structuringElement;

}








void ParameterSettingWidget::initCfarTableItem()
{
    QStringList paraNameList;
    paraNameList << QStringLiteral("protectWin") << QStringLiteral("backGroundWin")
                 << QStringLiteral("falseAlarmRate") << QStringLiteral("adapativeThreshold")
                 << QStringLiteral("isUseOverallThreshold") << QStringLiteral("RulyThreldRadio");
    m_pCfarTableWnd->setRowCount(paraNameList.size());
    for (int i = 0; i < paraNameList.size(); i++)
    {
        QLineEdit *pLineEdit = new QLineEdit("1", this);
        pLineEdit->setValidator(m_pReg);

        m_pCfarTableWnd->setItem(i, 0, new QTableWidgetItem(paraNameList[i]));
        m_pCfarTableWnd->setCellWidget(i, 1, pLineEdit);
    }
}

//set cfarParas
void ParameterSettingWidget::setCfarTableItem()
{
    QVector<QLineEdit *> cfarVec;
    for (int i = 0; i < m_pCfarTableWnd->rowCount(); i++)
    {
        cfarVec.push_back(dynamic_cast<QLineEdit *>(m_pCfarTableWnd->cellWidget(i, 1)));
    }

    cfarVec[0]->setText(str_to_lineEdit(DateMgr::getDataParas()->cfarDetectParas->protectWin.width()
                                       ,DateMgr::getDataParas()->cfarDetectParas->protectWin.height()));

    cfarVec[1]->setText(str_to_lineEdit(DateMgr::getDataParas()->cfarDetectParas->backGroundWin.width()
                                          , DateMgr::getDataParas()->cfarDetectParas->backGroundWin.height()));

    cfarVec[2]->setText(QString::number(DateMgr::getDataParas()->cfarDetectParas->falseAlarmRate));
    cfarVec[3]->setText(QString::number(DateMgr::getDataParas()->cfarDetectParas->adaptiveThreshold));
    cfarVec[4]->setText(QString::number(DateMgr::getDataParas()->cfarDetectParas->isUseOverallThreshold));
    cfarVec[5]->setText(QString::number(DateMgr::getDataParas()->cfarDetectParas->RulyThreldRadio));
}

void ParameterSettingWidget::setCfarParas()
{
    QVector<QLineEdit *> cfarVec;
    for (int i = 0; i < m_pCfarTableWnd->rowCount(); i++)
    {
        cfarVec.push_back(dynamic_cast<QLineEdit *>(m_pCfarTableWnd->cellWidget(i, 1)));
    }


    DateMgr::getDataParas()->cfarDetectParas->protectWin = lineEdit_to_QSize(cfarVec[0]->text());
    DateMgr::getDataParas()->cfarDetectParas->backGroundWin = lineEdit_to_QSize(cfarVec[1]->text());
    DateMgr::getDataParas()->cfarDetectParas->falseAlarmRate = cfarVec[2]->text().toDouble();
    DateMgr::getDataParas()->cfarDetectParas->adaptiveThreshold = cfarVec[3]->text().toDouble();
    DateMgr::getDataParas()->cfarDetectParas->isUseOverallThreshold
            = ((cfarVec[4]->text().toInt()!=0)?true:false);
    DateMgr::getDataParas()->cfarDetectParas->RulyThreldRadio = cfarVec[5]->text().toDouble();



    qDebug() << DateMgr::getDataParas()->cfarDetectParas->protectWin
             << DateMgr::getDataParas()->cfarDetectParas->backGroundWin
                   << DateMgr::getDataParas()->cfarDetectParas->falseAlarmRate
                      << DateMgr::getDataParas()->cfarDetectParas->adaptiveThreshold
                         << DateMgr::getDataParas()->cfarDetectParas->isUseOverallThreshold
                            << DateMgr::getDataParas()->cfarDetectParas->RulyThreldRadio;
}



void ParameterSettingWidget::initHogTableItem()
{
    QStringList paraNameList;
    paraNameList << QStringLiteral("detectWin") << QStringLiteral("winStrider")
                 <<QStringLiteral("blockWin") << QStringLiteral("blockStride")
        << QStringLiteral("cellWin") << QStringLiteral("nbins")
        << QStringLiteral("SliceImageWin");
    m_pHogTableWnd->setRowCount(paraNameList.size());
    for (int i = 0; i < paraNameList.size(); i++)
    {
        QLineEdit *pLineEdit = new QLineEdit("1", this);
        pLineEdit->setValidator(m_pReg);

        m_pHogTableWnd->setItem(i, 0, new QTableWidgetItem(paraNameList[i]));
        m_pHogTableWnd->setCellWidget(i, 1, pLineEdit);
    }
}

//set HogParas
void ParameterSettingWidget::setHogTableItem()
{
    QVector<QLineEdit *> hogVec;
    for (int i = 0; i < m_pHogTableWnd->rowCount(); i++)
    {
        hogVec.push_back(dynamic_cast<QLineEdit *>(m_pHogTableWnd->cellWidget(i, 1)));
    }



   hogVec[0]->setText(str_to_lineEdit(DateMgr::getDataParas()->hogFeatureParas->detectWin.width()
                                     ,DateMgr::getDataParas()->hogFeatureParas->detectWin.height()));

    hogVec[1]->setText(str_to_lineEdit(DateMgr::getDataParas()->hogFeatureParas->winStrider.width()
                                          , DateMgr::getDataParas()->hogFeatureParas->winStrider.height()));

    hogVec[2]->setText(str_to_lineEdit(DateMgr::getDataParas()->hogFeatureParas->blockWin.width()
                                          , DateMgr::getDataParas()->hogFeatureParas->blockWin.height()));

    hogVec[3]->setText(str_to_lineEdit(DateMgr::getDataParas()->hogFeatureParas->blockStrider.width()
                                       ,DateMgr::getDataParas()->hogFeatureParas->blockStrider.height()));

    hogVec[4]->setText(str_to_lineEdit(DateMgr::getDataParas()->hogFeatureParas->cellWin.width()
                                       ,DateMgr::getDataParas()->hogFeatureParas->cellWin.height()));

    hogVec[5]->setText(QString::number(DateMgr::getDataParas()->hogFeatureParas->nbins));

    hogVec[6]->setText(str_to_lineEdit(DateMgr::getDataParas()->hogFeatureParas->detectSliceImageWin.width()
                                      ,DateMgr::getDataParas()->hogFeatureParas->detectSliceImageWin.height()));

}

void ParameterSettingWidget::setHogParas()
{
    QVector<QLineEdit *> hogVec;
    for (int i = 0; i < m_pHogTableWnd->rowCount(); i++)
    {
        hogVec.push_back(dynamic_cast<QLineEdit *>(m_pHogTableWnd->cellWidget(i, 1)));
    }

    DateMgr::getDataParas()->hogFeatureParas->detectWin = lineEdit_to_QSize(hogVec[0]->text());
    DateMgr::getDataParas()->hogFeatureParas->winStrider = lineEdit_to_QSize(hogVec[1]->text());
    DateMgr::getDataParas()->hogFeatureParas->blockWin = lineEdit_to_QSize(hogVec[2]->text());
    DateMgr::getDataParas()->hogFeatureParas->blockStrider = lineEdit_to_QSize(hogVec[3]->text());
    DateMgr::getDataParas()->hogFeatureParas->cellWin = lineEdit_to_QSize(hogVec[4]->text());
    DateMgr::getDataParas()->hogFeatureParas->nbins = hogVec[5]->text().toInt();
    DateMgr::getDataParas()->hogFeatureParas->detectSliceImageWin = lineEdit_to_QSize(hogVec[6]->text());


        qDebug() << DateMgr::getDataParas()->hogFeatureParas->detectWin
                 << DateMgr::getDataParas()->hogFeatureParas->winStrider
                       << DateMgr::getDataParas()->hogFeatureParas->blockWin
                          << DateMgr::getDataParas()->hogFeatureParas->blockStrider
                             << DateMgr::getDataParas()->hogFeatureParas->cellWin
                                << DateMgr::getDataParas()->hogFeatureParas->nbins
                                   << DateMgr::getDataParas()->hogFeatureParas->detectSliceImageWin;

}


void ParameterSettingWidget::initSVMTableItem()
{
    QStringList paraNameList;
    paraNameList << QStringLiteral("svmType")
                 << QStringLiteral("kernelType") << QStringLiteral("degree")
                 << QStringLiteral("gamma") << QStringLiteral("coef0")
        << QStringLiteral("Cvalue") << QStringLiteral("nu")
        << QStringLiteral("p") << QStringLiteral("maxIteration")
           << QStringLiteral("epsilon");

    m_pSVMTableWnd->setRowCount(paraNameList.size());
    for (int i = 0; i < paraNameList.size(); i++)
    {
        QLineEdit *pLineEdit = new QLineEdit("1", this);
        pLineEdit->setValidator(m_pReg);

        m_pSVMTableWnd->setItem(i, 0, new QTableWidgetItem(paraNameList[i]));
        m_pSVMTableWnd->setCellWidget(i, 1, pLineEdit);
    }
}

//set cfarParas
void ParameterSettingWidget::setSVMTableItem()
{
    QVector<QLineEdit *> svmVec;
    for (int i = 0; i < m_pSVMTableWnd->rowCount(); i++)
    {
        svmVec.push_back(dynamic_cast<QLineEdit *>(m_pSVMTableWnd->cellWidget(i, 1)));
    }

    svmVec[0]->setText(QString::number(DateMgr::getDataParas()->svmParas->svmType));
    svmVec[1]->setText(QString::number(DateMgr::getDataParas()->svmParas->kernelType));
    svmVec[2]->setText(QString::number(DateMgr::getDataParas()->svmParas->degree));
    svmVec[3]->setText(QString::number(DateMgr::getDataParas()->svmParas->gamma));
    svmVec[4]->setText(QString::number(DateMgr::getDataParas()->svmParas->coef0));
    svmVec[5]->setText(QString::number(DateMgr::getDataParas()->svmParas->Cvalue));
    svmVec[6]->setText(QString::number(DateMgr::getDataParas()->svmParas->nu));
    svmVec[7]->setText(QString::number(DateMgr::getDataParas()->svmParas->p));
    svmVec[8]->setText(QString::number(DateMgr::getDataParas()->svmParas->maxIteration));
    svmVec[9]->setText(QString::number(DateMgr::getDataParas()->svmParas->epsilon));

}

void ParameterSettingWidget::setSvmParas()
{
    QVector<QLineEdit *> svmVec;
    for (int i = 0; i < m_pSVMTableWnd->rowCount(); i++)
    {
        svmVec.push_back(dynamic_cast<QLineEdit *>(m_pSVMTableWnd->cellWidget(i, 1)));
    }

    DateMgr::getDataParas()->svmParas->svmType = svmVec[0]->text().toInt();
    DateMgr::getDataParas()->svmParas->kernelType = svmVec[1]->text().toInt();
    DateMgr::getDataParas()->svmParas->degree = svmVec[2]->text().toDouble();
    DateMgr::getDataParas()->svmParas->gamma = svmVec[3]->text().toDouble();
    DateMgr::getDataParas()->svmParas->coef0 = svmVec[4]->text().toDouble();
    DateMgr::getDataParas()->svmParas->Cvalue = svmVec[5]->text().toDouble();

    DateMgr::getDataParas()->svmParas->nu = svmVec[6]->text().toDouble();

    DateMgr::getDataParas()->svmParas->p = svmVec[7]->text().toDouble();
    DateMgr::getDataParas()->svmParas->maxIteration = svmVec[8]->text().toInt();
    DateMgr::getDataParas()->svmParas->epsilon = svmVec[9]->text().toDouble();

        qDebug() << DateMgr::getDataParas()->svmParas->svmType
                 << DateMgr::getDataParas()->svmParas->kernelType
                       << DateMgr::getDataParas()->svmParas->degree
                          << DateMgr::getDataParas()->svmParas->gamma
                             << DateMgr::getDataParas()->svmParas->coef0
                                << DateMgr::getDataParas()->svmParas->Cvalue
                                << DateMgr::getDataParas()->svmParas->nu

                                << DateMgr::getDataParas()->svmParas->p
                                << DateMgr::getDataParas()->svmParas->maxIteration
                                << DateMgr::getDataParas()->svmParas->epsilon;

}


void ParameterSettingWidget::initRadarTableItem()
{
    QStringList paraNameList;
    paraNameList << QStringLiteral("resolution")
                 << QStringLiteral("object min width") << QStringLiteral("object max width")
                 << QStringLiteral("object min height") << QStringLiteral("object max height")
                 << QStringLiteral("max width_height Radio");

    m_pRadarTableWnd->setRowCount(paraNameList.size());
    for (int i = 0; i < paraNameList.size(); i++)
    {
        QLineEdit *pLineEdit = new QLineEdit("1", this);
        pLineEdit->setValidator(m_pReg);

        m_pRadarTableWnd->setItem(i, 0, new QTableWidgetItem(paraNameList[i]));
        m_pRadarTableWnd->setCellWidget(i, 1, pLineEdit);
    }
}

void ParameterSettingWidget::setRadarTableItem()
{
    QVector<QLineEdit *> radarVec;
    for (int i = 0; i < m_pRadarTableWnd->rowCount(); i++)
    {
        radarVec.push_back(dynamic_cast<QLineEdit *>(m_pRadarTableWnd->cellWidget(i, 1)));
    }

    radarVec[0]->setText(QString::number(DateMgr::getDataParas()->radarParas->resolution));
    radarVec[1]->setText(QString::number(DateMgr::getDataParas()->radarParas->minWidth));
    radarVec[2]->setText(QString::number(DateMgr::getDataParas()->radarParas->maxWidth));
    radarVec[3]->setText(QString::number(DateMgr::getDataParas()->radarParas->minHeight));
    radarVec[4]->setText(QString::number(DateMgr::getDataParas()->radarParas->maxHeight));
    radarVec[5]->setText(QString::number(DateMgr::getDataParas()->radarParas->maxWidthHeightRadio));
}

void ParameterSettingWidget::setRadarParas()
{
    QVector<QLineEdit *> radarVec;
    for (int i = 0; i < m_pRadarTableWnd->rowCount(); i++)
    {
        radarVec.push_back(dynamic_cast<QLineEdit *>(m_pRadarTableWnd->cellWidget(i, 1)));
    }

    DateMgr::getDataParas()->radarParas->resolution = radarVec[0]->text().toDouble();
    DateMgr::getDataParas()->radarParas->minWidth = radarVec[1]->text().toInt();
    DateMgr::getDataParas()->radarParas->maxWidth = radarVec[2]->text().toInt();
    DateMgr::getDataParas()->radarParas->minHeight = radarVec[3]->text().toInt();
    DateMgr::getDataParas()->radarParas->maxHeight = radarVec[4]->text().toInt();
    DateMgr::getDataParas()->radarParas->maxWidthHeightRadio = radarVec[5]->text().toDouble();


//        qDebug() << DateMgr::getDataParas()->radarParas->resolution
//                 << DateMgr::getDataParas()->radarParas->minWidth
//                       << DateMgr::getDataParas()->radarParas->maxWidth
//                          << DateMgr::getDataParas()->radarParas->minHeight
//                             << DateMgr::getDataParas()->radarParas->maxHeight
//                                   << DateMgr::getDataParas()->radarParas->maxWidthHeightRadio;
}







void ParameterSettingWidget::slot_ConfigBtnClicked()
{
    for (int i = 0; i < m_pFilterTableWnd->rowCount(); i++)
    {
        QLineEdit *pLineEdit = dynamic_cast<QLineEdit*>(m_pFilterTableWnd->cellWidget(i, 1));
        if (NULL != pLineEdit)
        {
            if (pLineEdit->text().isEmpty())
            {
                QMessageBox::warning(this, "warning", QStringLiteral("请设置图像滤波第") + QString::number(i+1) + QStringLiteral("行参数"));
                return;
            }
        }
    }


    for (int i = 0; i < m_pBinaryMorphTableWnd->rowCount(); i++)
    {
        QLineEdit *pLineEdit = dynamic_cast<QLineEdit*>(m_pBinaryMorphTableWnd->cellWidget(i, 1));
        if (NULL != pLineEdit)
        {
            if (pLineEdit->text().isEmpty())
            {
                QMessageBox::warning(this, "warning", QStringLiteral("请设置图像二值化与形态学第") + QString::number(i+1) + QStringLiteral("行参数"));
                return;
            }
        }
    }




    for (int i = 0; i < m_pCfarTableWnd->rowCount(); i++)
    {
        QLineEdit *pLineEdit = dynamic_cast<QLineEdit*>(m_pCfarTableWnd->cellWidget(i, 1));
        if (NULL != pLineEdit)
        {
            if (pLineEdit->text().isEmpty())
            {
                QMessageBox::warning(this, "warning", QStringLiteral("请设置Cfar检测第") + QString::number(i+1) + QStringLiteral("行参数"));
                return;
            }
        }
    }

    for (int i = 0; i < m_pHogTableWnd->rowCount(); i++)
    {
        QLineEdit *pLineEdit = dynamic_cast<QLineEdit*>(m_pHogTableWnd->cellWidget(i, 1));
        if (NULL != pLineEdit)
        {
            if (pLineEdit->text().isEmpty())
            {
                QMessageBox::warning(this, "warning", QStringLiteral("请设置Hog特征第") + QString::number(i + 1) + QStringLiteral("行参数"));
                return;
            }
        }
    }

    for (int i = 0; i < m_pSVMTableWnd->rowCount(); i++)
    {
        QLineEdit *pLineEdit = dynamic_cast<QLineEdit*>(m_pSVMTableWnd->cellWidget(i, 1));
        if (NULL != pLineEdit)
        {
            if (pLineEdit->text().isEmpty())
            {
                QMessageBox::warning(this, "warning", QStringLiteral("请设置SVM检测第") + QString::number(i + 1) + QStringLiteral("行参数"));
                return;
            }
        }
    }

    for (int i = 0; i < m_pRadarTableWnd->rowCount(); i++)
    {
        QLineEdit *pLineEdit = dynamic_cast<QLineEdit*>(m_pRadarTableWnd->cellWidget(i, 1));
        if (NULL != pLineEdit)
        {
            if (pLineEdit->text().isEmpty())
            {
                QMessageBox::warning(this, "warning", QStringLiteral("请设置目标检测参数第") + QString::number(i + 1) + QStringLiteral("行参数"));
                return;
            }
        }
    }

    setFilterParas();
    setBinaryMorphParas();
    setCfarParas();
    setHogParas();
    setSvmParas();
    setRadarParas();
    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("参数配置成功"));
    emit signal_ConfigDone();
}


void ParameterSettingWidget::slot_LoadDefaultParas()  //加载默认的参数
{

    // 加载默认的滤波参数
    QVector<QLineEdit *> filterVec;
    for (int i = 0; i < m_pFilterTableWnd->rowCount(); i++)
    {
        filterVec.push_back(dynamic_cast<QLineEdit *>(m_pFilterTableWnd->cellWidget(i, 1)));
    }

    filterVec[0]->setText(QString::number(-1));
    filterVec[1]->setText(str_to_lineEdit(5,5));
    filterVec[2]->setText(QString::number(1));
    filterVec[3]->setText(QString::number(1));
    filterVec[4]->setText(QString::number(0));
    filterVec[5]->setText(QString::number(0));
    filterVec[6]->setText(QString::number(25));
    filterVec[7]->setText(QString::number(25));

    //加载默认的binary morph参数
    QVector<QLineEdit *> binaryMorphVec;
    for (int i = 0; i < m_pBinaryMorphTableWnd->rowCount(); i++)
    {
        binaryMorphVec.push_back(dynamic_cast<QLineEdit *>(m_pBinaryMorphTableWnd->cellWidget(i, 1)));
    }

    binaryMorphVec[0]->setText(QString::number(100));
    binaryMorphVec[1]->setText(QString::number(255));
    binaryMorphVec[2]->setText(QString::number(13));
    binaryMorphVec[3]->setText(QString::number(5));
    binaryMorphVec[4]->setText(QString::number(5));
    binaryMorphVec[5]->setText(QString::number(5));
    binaryMorphVec[6]->setText(QString::number(0));

    //加载默认的cfar参数
    QVector<QLineEdit *> cfarVec;
    for (int i = 0; i < m_pCfarTableWnd->rowCount(); i++)
    {
        cfarVec.push_back(dynamic_cast<QLineEdit *>(m_pCfarTableWnd->cellWidget(i, 1)));
    }

    cfarVec[0]->setText(str_to_lineEdit(21,21));

    cfarVec[1]->setText(str_to_lineEdit(51, 51));

    cfarVec[2]->setText(QString::number(3));
    cfarVec[3]->setText(QString::number(100));
    cfarVec[4]->setText(QString::number(1));
    cfarVec[5]->setText(QString::number(7));

    //加载默认的hog参数
    QVector<QLineEdit *> hogVec;
    for (int i = 0; i < m_pHogTableWnd->rowCount(); i++)
    {
        hogVec.push_back(dynamic_cast<QLineEdit *>(m_pHogTableWnd->cellWidget(i, 1)));
    }



   hogVec[0]->setText(str_to_lineEdit(64,64));

    hogVec[1]->setText(str_to_lineEdit(8, 8));

    hogVec[2]->setText(str_to_lineEdit(16, 16));

    hogVec[3]->setText(str_to_lineEdit(8,8));

    hogVec[4]->setText(str_to_lineEdit(8,8));

    hogVec[5]->setText(QString::number(9));

    hogVec[6]->setText(str_to_lineEdit(64,64));


    //加载默认的svm参数
    QVector<QLineEdit *> svmVec;
    for (int i = 0; i < m_pSVMTableWnd->rowCount(); i++)
    {
        svmVec.push_back(dynamic_cast<QLineEdit *>(m_pSVMTableWnd->cellWidget(i, 1)));
    }

    svmVec[0]->setText(QString::number(100));
    svmVec[1]->setText(QString::number(2));
    svmVec[2]->setText(QString::number(0));
    svmVec[3]->setText(QString::number(0.09));
    svmVec[4]->setText(QString::number(0));
    svmVec[5]->setText(QString::number(10));
    svmVec[6]->setText(QString::number(0));
    svmVec[7]->setText(QString::number(0));
    svmVec[8]->setText(QString::number(ITERATIVE_NUMBER));
    svmVec[9]->setText(QString::number(FLT_EPSILON));

    //加载默认的目标检测参数
    QVector<QLineEdit *> radarVec;
    for (int i = 0; i < m_pRadarTableWnd->rowCount(); i++)
    {
        radarVec.push_back(dynamic_cast<QLineEdit *>(m_pRadarTableWnd->cellWidget(i, 1)));
    }

    radarVec[0]->setText(QString::number(1));
    radarVec[1]->setText(QString::number(5));
    radarVec[2]->setText(QString::number(340));
    radarVec[3]->setText(QString::number(5));
    radarVec[4]->setText(QString::number(50));
    radarVec[5]->setText(QString::number(10));



    setFilterParas();
    setBinaryMorphParas();
    setCfarParas();
    setHogParas();
    setSvmParas();
    setRadarParas();

    QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("默认参数加载成功"));
    emit signal_ConfigDone();
}









