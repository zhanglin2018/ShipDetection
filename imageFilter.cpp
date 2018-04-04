//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：设计图像滤波算法基类，确定滤波算法的框架
//		开发测试所用IDE版本：Visual Studio 2010
//		开发测试所用OpenCV版本：	2.4.9
//		2018年1月10日 Created by @章林
//		2018年X月 Revised by @
//------------------------------------------------------------------------------------------------

//---------------------------------【头文件、命名空间包含部分】-----------------------------
//		描述：包含程序所使用的头文件和命名空间
//-------------------------------------------------------------------------------------------------

#include "imageFilter.h"
using namespace ObjectImageFiter;


//---------------------------------【图像滤波 接口设计部分】-----------------------------
//		描述：定义图像滤波 各种类型接口
//-------------------------------------------------------------------------------------------------

//定义了图像方框滤波接口

ImageBoxFilter::ImageBoxFilter(int _depth, Size _ksize, Point _anchor
    ,bool _normalize,int _borderType):
                                    ImageFilterBase(_ksize),
                                    ddepth(_depth),
                                    anchor(_anchor),
                                    normalize(_normalize),
                                    borderType(_borderType){}

ImageBoxFilter::~ImageBoxFilter(){}

void ImageBoxFilter::ImageFilter(InputArray src, OutputArray dst)
{
    cv::boxFilter(src, dst, ddepth, ksize, anchor, normalize, borderType);
}



//定义了图像均值滤波接口

ImageBlurFilter::ImageBlurFilter(Size _ksize, Point _anchor,
    int _borderType):
ImageFilterBase(_ksize),
    anchor(_anchor),
    borderType(_borderType){}

ImageBlurFilter::~ImageBlurFilter(){}

void ImageBlurFilter::ImageFilter(InputArray src, OutputArray dst)
{
    cv::blur(src, dst, ksize, anchor, borderType);
}


//定义了图像高斯滤波接口

ImageGaussianBlurFilter::ImageGaussianBlurFilter(Size _ksize,
    double _sigmaX, double _sigmaY,
    int _borderType):
ImageFilterBase(_ksize),
    sigmaX(_sigmaX),
    sigmaY(_sigmaY),
    borderType(_borderType){}

ImageGaussianBlurFilter::~ImageGaussianBlurFilter(){}

void ImageGaussianBlurFilter::ImageFilter(InputArray src, OutputArray dst)
{
    cv::GaussianBlur(src, dst, ksize, sigmaX, sigmaY, borderType);
}


//定义了中值滤波接口

ImageMedianBlurFilter::ImageMedianBlurFilter(Size _ksize):ImageFilterBase(_ksize){}

ImageMedianBlurFilter::~ImageMedianBlurFilter(){}

void ImageMedianBlurFilter::ImageFilter(InputArray src, OutputArray dst)
{
    cv::medianBlur(src, dst, (ksize.width+ksize.height)/2);
}


//定义了图像 双边滤波 接口
ImageBilateralFilter::ImageBilateralFilter(Size _ksize, int _d,
    double _sigmaColor, double _sigmaSpace,
    int _borderType):
ImageFilterBase(_ksize),
    d(_d),
    sigmaColor(_sigmaColor),
    sigmaSpace(_sigmaSpace),
    borderType(_borderType){}

ImageBilateralFilter::~ImageBilateralFilter(){}

void ImageBilateralFilter::ImageFilter(InputArray src, OutputArray dst)
{
    cv::bilateralFilter(src, dst, d, sigmaColor, sigmaSpace, borderType);
}


//定义了图像 自适应双边滤波 接口
ImageAdaptiveBilateralFilter::ImageAdaptiveBilateralFilter(Size _ksize,
    double _sigmaSpace, double _maxSigmaColor, Point _anchor,
    int _borderType):
ImageFilterBase(_ksize),
    sigmaSpace(_sigmaSpace),
    maxSigmaColor(_maxSigmaColor),
    anchor(_anchor),
    borderType(_borderType){}

ImageAdaptiveBilateralFilter::~ImageAdaptiveBilateralFilter(){}

void ImageAdaptiveBilateralFilter::ImageFilter(InputArray src, OutputArray dst)
{
    cv::adaptiveBilateralFilter(src, dst, ksize, sigmaSpace, maxSigmaColor, anchor, borderType);
}


//---------------------------------【图像滤波 实例化部分】-----------------------------
//		描述：利用设计模式中的 context模式  分情况实例化 图像滤波接口
//-------------------------------------------------------------------------------------------------


    //通过传入不同的滤波类型，进行实例化操作
ImageFilter::ImageFilter(enum IMAGE_FILTER_TYPE type)
{
    switch (type)
    {
    case BOX_FILTER:
        imageFilterPtr = new ImageBoxFilter(DateMgr::getDataParas()->imageFilterParas->depth
                                            , QSizeToSize(DateMgr::getDataParas()->imageFilterParas->kernel)
                                            , Point(-1, -1)
                                            ,DateMgr::getDataParas()->imageFilterParas->normalize
                                            , DateMgr::getDataParas()->imageFilterParas->borderType);
        break;

    case BLUR:
        imageFilterPtr = new ImageBlurFilter(QSizeToSize(DateMgr::getDataParas()->imageFilterParas->kernel)
                                             , Point(-1, -1)
                                             ,DateMgr::getDataParas()->imageFilterParas->borderType);
        break;

    case GAUSSIAN_BLUR:
        imageFilterPtr = new ImageGaussianBlurFilter(QSizeToSize(DateMgr::getDataParas()->imageFilterParas->kernel)
                                                     ,DateMgr::getDataParas()->imageFilterParas->sigmaX
                                                     , DateMgr::getDataParas()->imageFilterParas->sigmaY
                                                     , DateMgr::getDataParas()->imageFilterParas->borderType);
        break;

    case MEDIAN_BLUR:
        imageFilterPtr = new ImageMedianBlurFilter(QSizeToSize(DateMgr::getDataParas()->imageFilterParas->kernel));
        break;

    case BILATERAL_FILTER:
        imageFilterPtr = new ImageBilateralFilter(QSizeToSize(DateMgr::getDataParas()->imageFilterParas->kernel)
                                                  , 25
                                                  ,DateMgr::getDataParas()->imageFilterParas->sigmaColor
                                                  , DateMgr::getDataParas()->imageFilterParas->sigmaSpace
                                                  ,DateMgr::getDataParas()->imageFilterParas->borderType);
        break;

    case ADAPTIVE_BILATERAL_FILTER:
        imageFilterPtr = new ImageAdaptiveBilateralFilter(QSizeToSize(DateMgr::getDataParas()->imageFilterParas->kernel)
                                                          ,DateMgr::getDataParas()->imageFilterParas->sigmaSpace
                                                          , DateMgr::getDataParas()->imageFilterParas->sigmaColor
                                                          ,Point(-1, -1)
                                                          ,DateMgr::getDataParas()->imageFilterParas->borderType);
        break;

    default:
        break;
    }
}

void ImageFilter::getImageFilterResult(InputArray src, OutputArray dst)
{
    imageFilterPtr->ImageFilter(src, dst);    //通过多态调用各自的滤波函数
}
