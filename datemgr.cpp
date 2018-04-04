#include "datemgr.h"

#include <QWidget>


//图像滤波参数
ImageFilterParas::ImageFilterParas(int depth
        , QSize    kernel
        , bool     normalize
        , int      borderType
        , double   sigmaX
        , double   sigmaY
        , double   sigmaColor
        , double   sigmaSpace)
{
    this->depth = depth;
    this->kernel = kernel;
    this->normalize = normalize;
    this->borderType = borderType;
    this->sigmaX = sigmaX;
    this->sigmaY = sigmaY;
    this->sigmaColor = sigmaColor;
    this->sigmaSpace = sigmaSpace;
}

//二值化 形态学参数

ImageBinaryAndMorphParas::ImageBinaryAndMorphParas(double threshold,
double    maxThreshold,
int      blockSize,
double      C,
int      dilateSize,
int      erodeSize,
int      elem)
{
    this->threshold = threshold;
    this->maxThreshold = maxThreshold;
    this->blockSize = blockSize;
    this->C = C;
    this->dilateSize = dilateSize;
    this->erodeSize = erodeSize;
    this->structuringElement = elem;
}


//CFAR检测参数
CfarDetectParas::CfarDetectParas(QSize protectWin
        , QSize    backGroundWin
        , double     falseAlarmRate
        , double      adaptiveThreshold
        , bool       isUseOverallThreshold
        , double     RulyThreldRadio)
{
    this->protectWin = protectWin;
    this->backGroundWin = backGroundWin;
    this->falseAlarmRate = falseAlarmRate;
    this->adaptiveThreshold = adaptiveThreshold;
    this->isUseOverallThreshold = isUseOverallThreshold;
    this->RulyThreldRadio = RulyThreldRadio;

}


//Hog特征参数
HogFeatureParas::HogFeatureParas(QSize detectWin
        , QSize    winStrider
        , QSize     blockWin
        , QSize      blockStrider
        , QSize       cellWin
        , int         nbins
        , QSize detectSliceImageWin)
{
    this->detectWin = detectWin;
    this->winStrider = winStrider;
    this->blockWin = blockWin;
    this->blockStrider = blockStrider;
    this->cellWin = cellWin;
    this->nbins = nbins;
    this->detectSliceImageWin = detectSliceImageWin;
}

//Hog特征参数
SvmParas::SvmParas(int svmType
        ,int kernelType
        , double degree
        , double gamma
        , double coef0
        , double Cvalue
        , double nu
        , double p
        , int maxIteration
        , double epsilon)
{
    this->svmType = svmType;
    this->kernelType = kernelType;
    this->degree = degree;
    this->gamma = gamma;
    this->coef0 = coef0;
    this->Cvalue = Cvalue;
    this->nu = nu;
    this->p = p;
    this->maxIteration = maxIteration;
    this->epsilon = epsilon;
}

RadarParas::RadarParas(double _resolution
           , int _minWidth
           , int _maxWidth
           , int _minHeight
           , int _maxHeight
           , double _maxWidthHeightRadio)
{
    resolution = _resolution;
    minWidth = _minWidth;
    maxWidth = _maxWidth;
    minHeight = _minHeight;
    maxHeight = _maxHeight;
    maxWidthHeightRadio = _maxWidthHeightRadio;
}

// DateMgr 初始化
DateMgr::DateMgr()
{
    imageFilterParas = new ImageFilterParas;
    imageBinaryAndMorphParas = new ImageBinaryAndMorphParas;
    cfarDetectParas = new CfarDetectParas;
    hogFeatureParas = new HogFeatureParas;
    svmParas = new SvmParas;
    radarParas = new RadarParas;
}


QMutex DateMgr::mutex;
DateMgr* DateMgr::instance = NULL;

DateMgr* DateMgr::getDataParas()
{
    if (instance == NULL)
    {
        QMutexLocker locker(&mutex);  //add mutex locker

        if (instance == NULL)
        {
            instance = new DateMgr;
        }
    }
    return instance;
}

cv::Size QSizeToSize(const QSize size)
{
    Size tgtSize;
    tgtSize.width = size.width();
    tgtSize.height = size.height();
    return tgtSize;
}

