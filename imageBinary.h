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


#ifndef __IMAGE_BINARY_H__
#define __IMAGE_BINARY_H__

#include <opencv2/opencv.hpp> 
#include <memory>
#include "datemgr.h"
using namespace cv;
using namespace std;


namespace ObjectImageBinary{
	
	//定义图像二值化接口类型
	enum IMAGE_THRELD_TYPE{
		THRESHOLD,
		ADAPTIVE_THRESHOLD
	};   //(具有两个接口，后续可以继续添加实现接口)

	enum{
    CV_THRESH_BINARY      =0,  /* value = value > threshold ? max_value : 0       */
    CV_THRESH_BINARY_INV  =1,  /* value = value > threshold ? 0 : max_value       */
    CV_THRESH_TRUNC       =2,  /* value = value > threshold ? threshold : value   */
    CV_THRESH_TOZERO      =3,  /* value = value > threshold ? value : 0           */
    CV_THRESH_TOZERO_INV  =4,  /* value = value > threshold ? 0 : value           */
    CV_THRESH_MASK        =7,
    CV_THRESH_OTSU        =8  /* use Otsu algorithm to choose the optimal threshold value;
                                 combine the flag with one of the above CV_THRESH_* values */
	};


	//图像二值化的各种操作方式  type of the threshold operation
	enum IMAGE_BINARY_TYPE{
		THRESH_BINARY = CV_THRESH_BINARY,         /* value = value > threshold ? max_value : 0       */
		THRESH_BINARY_INV = CV_THRESH_BINARY_INV,     /* value = value > threshold ? 0 : max_value       */
		THRESH_TRUNC = CV_THRESH_TRUNC,          /* value = value > threshold ? threshold : value   */
		THRESH_TOZERO = CV_THRESH_TOZERO,         /* value = value > threshold ? value : 0           */
		THRESH_TOZERO_INV = CV_THRESH_TOZERO_INV,     /* value = value > threshold ? 0 : value           */
		THRESH_MASK = CV_THRESH_MASK,
		THRESH_OTSU =  CV_THRESH_OTSU           /* use Otsu algorithm to choose the optimal threshold value;
                                 combine the flag with one of the above CV_THRESH_* values */
	};

	//---------------------------------【图像二值化 接口设计部分】-----------------------------
	//		描述：定义图像二值化 各种类型接口
	//-------------------------------------------------------------------------------------------------


	//定义了图像二值化基类，用于定义图像二值化的接口
	class ThresholdBase
	{
	public:
		virtual double ImageThreshold(InputArray src, OutputArray dst) = 0;
	};

	//定义了图像threld接口
	class Threshold: public ThresholdBase
	{
	public:
        Threshold(double _thresh, double _maxval, IMAGE_BINARY_TYPE _threldType);

        ~Threshold();

        virtual double ImageThreshold(InputArray src, OutputArray dst);

	protected:
		double thresh;
		double maxval;
		IMAGE_BINARY_TYPE threldType;
	};


	//! adaptive threshold algorithm
	enum IMAGE_ADAPTIVE_METHOD{ 
		ADAPTIVE_THRESH_MEAN_C, 
		ADAPTIVE_THRESH_GAUSSIAN_C 
	};

	//定义了图像自适应threld接口
	class AdaptiveThreshold: public ThresholdBase
	{
	public:
		AdaptiveThreshold(double _maxValue, IMAGE_ADAPTIVE_METHOD _adaptiveMethod,
            IMAGE_BINARY_TYPE _thresholdType, int _blockSize, double _C);

        ~AdaptiveThreshold();

        virtual double ImageThreshold(InputArray src, OutputArray dst);

	protected:
		double maxValue;
		IMAGE_ADAPTIVE_METHOD adaptiveMethod;
		IMAGE_BINARY_TYPE thresholdType;
		int blockSize;
		double C;
	};

	//---------------------------------【图像图像二值化 实例化部分】-----------------------------
	//		描述：利用设计模式中的 context模式  分情况实例化 图像图像二值化接口
	//-------------------------------------------------------------------------------------------------
	class ImageBinary
	{
	public:
		//通过传入不同的滤波类型，进行实例化操作
        ImageBinary(enum IMAGE_THRELD_TYPE type, enum IMAGE_BINARY_TYPE binary_type);


        double getImageBinaryResult(InputArray src, OutputArray dst);

	private:
		shared_ptr<ThresholdBase> imageBinaryPtr;
	};

}

#endif


