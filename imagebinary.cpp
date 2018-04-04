//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：设计图像二值化 算法基类，确定 求取阈值 算法的框架
//		开发测试所用IDE版本：Visual Studio 2010
//		开发测试所用OpenCV版本：	2.4.9
//		2018年1月11日 Created by @章林
//		2018年X月 Revised by @
//------------------------------------------------------------------------------------------------

//---------------------------------【头文件、命名空间包含部分】-----------------------------
//		描述：包含程序所使用的头文件和命名空间
//-------------------------------------------------------------------------------------------------


#include "imageBinary.h"
using namespace ObjectImageBinary;


//---------------------------------【图像二值化 接口设计部分】-----------------------------
//		描述：定义图像二值化 各种类型接口
//-------------------------------------------------------------------------------------------------

//定义了图像threld接口

Threshold::Threshold(double _thresh, double _maxval, IMAGE_BINARY_TYPE _threldType):
  thresh(_thresh), maxval(_maxval), threldType(_threldType){}

Threshold::~Threshold(){}

double Threshold::ImageThreshold(InputArray src, OutputArray dst)
{
    double threshold = cv::threshold( src, dst, thresh, maxval, threldType );
    return threshold;
}


//定义了图像自适应threld接口

AdaptiveThreshold::AdaptiveThreshold(double _maxValue, IMAGE_ADAPTIVE_METHOD _adaptiveMethod,
    IMAGE_BINARY_TYPE _thresholdType, int _blockSize, double _C):
    maxValue(_maxValue),
    adaptiveMethod(_adaptiveMethod),
    thresholdType(_thresholdType),
    blockSize(_blockSize),
    C(_C){}

AdaptiveThreshold::~AdaptiveThreshold(){}

double AdaptiveThreshold::ImageThreshold(InputArray src, OutputArray dst)
{
    cv::adaptiveThreshold(  src,  dst,
         maxValue,  adaptiveMethod,
         thresholdType,  blockSize,  C );
    return 0.0;
}

//---------------------------------【图像二值化 参数定义部分】-----------------------------
//		描述：定义图像二值化 所需要参数 的全局变量， 用于接口的调用
//-------------------------------------------------------------------------------------------------

//---------------------------------【图像图像二值化 实例化部分】-----------------------------
//		描述：利用设计模式中的 context模式  分情况实例化 图像图像二值化接口
//-------------------------------------------------------------------------------------------------


//// Threshold 参数 全局变量定义
//static	double g_nThreshold_thresh = DateMgr::getDataParas()->imageBinaryAndMorphParas->threshold;
//static	double g_nThreshold_maxval = DateMgr::getDataParas()->imageBinaryAndMorphParas->maxThreshold;
//static	IMAGE_BINARY_TYPE g_nThreshold_threldType = THRESH_OTSU;


//// AdaptiveThreshold 参数 全局变量定义
//static double g_nAdaptiveThreshold_maxValue = DateMgr::getDataParas()->imageBinaryAndMorphParas->maxThreshold;
//static IMAGE_ADAPTIVE_METHOD g_nAdaptiveThreshold_adaptiveMethod = ADAPTIVE_THRESH_MEAN_C;
//static IMAGE_BINARY_TYPE g_nAdaptiveThreshold_thresholdType = THRESH_BINARY;     //只能选择THRESH_BINARY或者THRESH_BINARY_INV
//static int g_nAdaptiveThreshold_blockSize = DateMgr::getDataParas()->imageBinaryAndMorphParas->blockSize;  //用于计算阈值大小的一个像素的邻域尺寸，取值为3,5,7等
//static double g_nAdaptiveThreshold_C = DateMgr::getDataParas()->imageBinaryAndMorphParas->C;           //计算出该像素blockSize邻域的均值或者高斯均值，再减去第七个参数C的值




    //通过传入不同的滤波类型，进行实例化操作
ImageBinary::ImageBinary(enum IMAGE_THRELD_TYPE type, IMAGE_BINARY_TYPE binary_type)
{
    switch (type)
    {
        case THRESHOLD:
        imageBinaryPtr = shared_ptr<ThresholdBase>(new Threshold(DateMgr::getDataParas()->imageBinaryAndMorphParas->threshold,
            DateMgr::getDataParas()->imageBinaryAndMorphParas->maxThreshold
            , binary_type));
        break;

        case ADAPTIVE_THRESHOLD:
        imageBinaryPtr = shared_ptr<ThresholdBase>(new AdaptiveThreshold(DateMgr::getDataParas()->imageBinaryAndMorphParas->maxThreshold,
                ADAPTIVE_THRESH_MEAN_C,
                binary_type,
                DateMgr::getDataParas()->imageBinaryAndMorphParas->blockSize,
                DateMgr::getDataParas()->imageBinaryAndMorphParas->C));
        break;

    default:
        break;
    }
}

double ImageBinary::getImageBinaryResult(InputArray src, OutputArray dst)
{
    double threshold = imageBinaryPtr->ImageThreshold(src, dst);    //通过多态调用各自的滤波函数
    return threshold;
}





