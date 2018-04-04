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


#ifndef __IMAGE_FILTER_H__
#define __IMAGE_FILTER_H__

#include <opencv2/opencv.hpp>
#include "datemgr.h"
using namespace cv;
using namespace std;


namespace ObjectImageFiter{


//图像滤波的各种方式
enum IMAGE_FILTER_TYPE{
	BOX_FILTER,
	BLUR,
	GAUSSIAN_BLUR,
	MEDIAN_BLUR,
	BILATERAL_FILTER,
	ADAPTIVE_BILATERAL_FILTER
}; //(具有六个接口，后续可以继续添加实现接口)

//---------------------------------【图像滤波 接口设计部分】-----------------------------
//		描述：定义图像滤波 各种类型接口
//-------------------------------------------------------------------------------------------------


//定义了图像滤波基类，用于定义图像滤波的接口
class ImageFilterBase
{
public:
	ImageFilterBase(Size _ksize):ksize(_ksize){}
	~ImageFilterBase(){}
	virtual void ImageFilter(InputArray src, OutputArray dst) = 0;

protected:
	Size ksize;
};

//定义了图像方框滤波接口
class ImageBoxFilter: public ImageFilterBase
{
public:
	ImageBoxFilter(int _depth, Size _ksize, Point _anchor=Point(-1,-1)
        ,bool _normalize=true,int _borderType=BORDER_DEFAULT);

    ~ImageBoxFilter();

    virtual void ImageFilter(InputArray src, OutputArray dst);

protected:
	int ddepth;
	Point anchor;
	bool normalize;
	int borderType;
};


//定义了图像均值滤波接口
class ImageBlurFilter: public ImageFilterBase
{
public:
	ImageBlurFilter(Size _ksize, Point _anchor=Point(-1,-1),
        int _borderType=BORDER_DEFAULT);

    ~ImageBlurFilter();

    virtual void ImageFilter(InputArray src, OutputArray dst);

protected:
	Point anchor;
	int borderType;
};



//定义了图像高斯滤波接口
class ImageGaussianBlurFilter: public ImageFilterBase
{
public:
	ImageGaussianBlurFilter(Size _ksize,
		double _sigmaX, double _sigmaY=0,
        int _borderType=BORDER_DEFAULT);

    ~ImageGaussianBlurFilter();

    virtual void ImageFilter(InputArray src, OutputArray dst);

protected:
	double sigmaX;
	double sigmaY;
	int borderType;
};

//定义了中值滤波接口
class ImageMedianBlurFilter: public ImageFilterBase
{
public:
    ImageMedianBlurFilter(Size _ksize);

    ~ImageMedianBlurFilter();

    virtual void ImageFilter(InputArray src, OutputArray dst);
};

//定义了图像 双边滤波 接口
class ImageBilateralFilter: public ImageFilterBase
{
public:
	ImageBilateralFilter(Size _ksize, int _d,
		double _sigmaColor, double _sigmaSpace,
        int _borderType);

    ~ImageBilateralFilter();

    virtual void ImageFilter(InputArray src, OutputArray dst);

protected:
	int d;
	double sigmaColor;
	double sigmaSpace;
	int borderType;
};

//定义了图像 自适应双边滤波 接口
class ImageAdaptiveBilateralFilter: public ImageFilterBase
{
public:
	ImageAdaptiveBilateralFilter(Size _ksize,
        double _sigmaSpace, double _maxSigmaColor, Point _anchor,
        int _borderType);

    ~ImageAdaptiveBilateralFilter();

    virtual void ImageFilter(InputArray src, OutputArray dst);

protected:
	double sigmaSpace;
	double maxSigmaColor;
	Point anchor;
	int borderType;
};



//---------------------------------【图像滤波 参数定义部分】-----------------------------
//		描述：定义图像滤波 所需要参数 的全局变量， 用于接口的调用
//-------------------------------------------------------------------------------------------------



//// 方框滤波 参数 全局变量定义
//static Size g_nboxFilter_ksize = QSizeToSize(DateMgr::getDataParas()->imageFilterParas->kernel);   //方框滤波 kernel窗口大小

//static int g_nboxFilter_ddepth = DateMgr::getDataParas()->imageFilterParas->depth;                      //-1 代表使用原图的深度
//static Point g_nboxFilter_anchor = Point(-1, -1);           // 取核的中心为锚点
//static bool g_nboxFilter_normalize = DateMgr::getDataParas()->imageFilterParas->normalize;                 // 表示内核是否被其区域归一化了
//static int g_nboxFilter_borderType = DateMgr::getDataParas()->imageFilterParas->borderType;       // 用于推断图像外部像素的某种边界模式


//// 均值滤波 参数 全局变量定义
//static Size g_nblurFilter_ksize = QSizeToSize(DateMgr::getDataParas()->imageFilterParas->kernel);                //均值滤波 kernel窗口大小
//static Point g_nblurFilter_anchor = Point(-1, -1);
//static int g_nblurFilter_borderType = DateMgr::getDataParas()->imageFilterParas->borderType;


//// 高斯滤波 参数 全局变量定义
//static Size g_ngaussianBlurFilter_ksize = QSizeToSize(DateMgr::getDataParas()->imageFilterParas->kernel);      //高斯滤波 kernel窗口大小
//static double g_ngaussianBlurFilter_sigmaX = DateMgr::getDataParas()->imageFilterParas->sigmaX;
//static double g_ngaussianBlurFilter_sigmaY = DateMgr::getDataParas()->imageFilterParas->sigmaY;
//static int g_ngaussianBlurFilter_borderType = DateMgr::getDataParas()->imageFilterParas->borderType;


//// 中值滤波 参数 全局变量定义
//static Size g_nmedianBlurFilter_ksize = QSizeToSize(DateMgr::getDataParas()->imageFilterParas->kernel);        //中值滤波 kernel窗口大小


//// 双边滤波 参数 全局变量定义
//static Size g_nbilateralFilter_ksize = QSizeToSize(DateMgr::getDataParas()->imageFilterParas->kernel);         //双边滤波 kernel窗口大小
//static int g_nbilateralFilter_d = 25;
//static double g_nbilateralFilter_sigmaColor = DateMgr::getDataParas()->imageFilterParas->sigmaColor;
//static double g_nbilateralFilter_sigmaSpace = DateMgr::getDataParas()->imageFilterParas->sigmaSpace;
//static int g_nbilateralFilter_borderType = DateMgr::getDataParas()->imageFilterParas->borderType;


//// 自适应双边滤波参数 全局变量定义
//static Size g_nadaptiveBilateralFilter_ksize = QSizeToSize(DateMgr::getDataParas()->imageFilterParas->kernel);  //自适应双边滤波 kernel窗口大小
//static double g_nadaptiveBilateralFilter_sigmaSpace = DateMgr::getDataParas()->imageFilterParas->sigmaSpace;
//static double g_nadaptiveBilateralFilter_maxSigmaColor = DateMgr::getDataParas()->imageFilterParas->sigmaColor;
//static Point g_nadaptiveBilateralFilter_anchor = Point(-1, -1);
//static int g_nadaptiveBilateralFilter_borderType = DateMgr::getDataParas()->imageFilterParas->borderType;

//---------------------------------【图像滤波 实例化部分】-----------------------------
//		描述：利用设计模式中的 context模式  分情况实例化 图像滤波接口
//-------------------------------------------------------------------------------------------------
class ImageFilter
{
public:
	//通过传入不同的滤波类型，进行实例化操作
    ImageFilter(enum IMAGE_FILTER_TYPE type);

    void getImageFilterResult(InputArray src, OutputArray dst);

private:
	ImageFilterBase *imageFilterPtr;
};


}

#endif


