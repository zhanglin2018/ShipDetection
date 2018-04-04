#ifndef DATEMGR
#define DATEMGR

#include <QWidget>
#include "global.h"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

//图像滤波参数
class ImageFilterParas
{
public:
    ImageFilterParas(int depth = -1
     , QSize    kernel = QSize(5, 5)
     , bool     normalize = true
     , int      borderType = 1
     , double   sigmaX = 0.0
     , double   sigmaY = 0.0
     , double   sigmaColor = 25
     , double   sigmaSpace = 25);


public:
    int      depth;
    QSize    kernel;
    bool     normalize;
    int      borderType;
    double   sigmaX;
    double   sigmaY;
    double   sigmaColor;
    double   sigmaSpace;
};

//图像滤波参数
class ImageBinaryAndMorphParas
{
public:
    ImageBinaryAndMorphParas(double threshold = 100,
    double    maxThreshold = 255,
    int      blockSize = 13,
    double      C = 5.0,
    int      dilateSize = 5,
    int      erodeSize = 5,
    int      structuringElement = 0);


public:
    double      threshold;
    double    maxThreshold;
    int      blockSize;
    double      C;
    int      dilateSize;
    int      erodeSize;
    int      structuringElement;   //0,1,2 三种取值
};

//CFAR检测参数
class CfarDetectParas
{

public:
    CfarDetectParas(QSize protectWin = QSize(21, 21)
     , QSize    backGroundWin = QSize(51, 51)
     , double     falseAlarmRate = 3.0
     , double      adaptiveThreshold = 100
     , bool       isUseOverallThreshold = true
     , double      RulyThreldRadio = 7);

public:
    QSize    protectWin;
    QSize    backGroundWin;
    double   falseAlarmRate;
    double   adaptiveThreshold;
    bool     isUseOverallThreshold;
    double   RulyThreldRadio;

};


//Hog特征参数
class HogFeatureParas
{

public:
    HogFeatureParas(QSize detectWin = QSize(64, 64)
     , QSize    winStrider = QSize(8, 8)
     , QSize     blockWin = QSize(16, 16)
     , QSize      blockStrider = QSize(8, 8)
     , QSize       cellWin = QSize(8, 8)
     , int         nbins = 9
     , QSize    detectSliceImageWin = QSize(64, 64));

public:
    QSize    detectWin;
    QSize    winStrider;
    QSize    blockWin;
    QSize    blockStrider;
    QSize    cellWin;
    int      nbins;
    QSize    detectSliceImageWin;
};

//Hog特征参数
class SvmParas
{
public:
    SvmParas(int svmType = 100
            ,int kernelType = 2
            , double degree = 3.0
            , double gamma = 0.09
            , double coef0 = 0.0
            , double Cvalue = 10.0
            , double nu = 0.5
            , double p = 0.1
            , int maxIteration = ITERATIVE_NUMBER
            , double epsilon = FLT_EPSILON);


public:
    int       svmType;
    int       kernelType;
    double    degree;
    double    gamma;
    double    coef0;
    double    Cvalue;
    double    nu;
    double      p;
    int       maxIteration;
    double    epsilon;
};


//Hog特征参数
class RadarParas
{
public:
    RadarParas(double _resolution = 1.0
               , int _minWidth = 5
               , int _maxWidth = 340
               , int _minHeight = 5
               , int _maxHeight = 50
               , double _maxWidthHeightRadio = 10.0);

public:
    double    resolution;
    int       minWidth;
    int       maxWidth;
    int       minHeight;
    int       maxHeight;
    double    maxWidthHeightRadio;
};

// 雷达参数
//class RadarParas
//{
//public:
//    RadarParas(double resolution
//             , double incidenceAngle);

//public:

//    double resolution;      //分辨率
//    double incidenceAngle;  //入射角

//};



//创建单列数据参数

class DateMgr
{

public:
    static DateMgr* getDataParas();
    ~DateMgr();

public:
    ImageFilterParas *imageFilterParas;
    ImageBinaryAndMorphParas* imageBinaryAndMorphParas;
    CfarDetectParas *cfarDetectParas;
    HogFeatureParas *hogFeatureParas;
    SvmParas *svmParas;
    RadarParas *radarParas;

private:
    DateMgr();
    DateMgr(const DateMgr&);
    DateMgr& operator=(const DateMgr*);

    static DateMgr* instance;
    static QMutex mutex;

};

cv::Size QSizeToSize(const QSize size);










#endif // DATEMGR






















