//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：完成图像的FFT操作，IFFT操作，实现CFAR函数
//		开发测试所用IDE版本：Visual Studio 2010
//		开发测试所用OpenCV版本：	2.4.9
//		2018年1月11日 Created by @章林
//		2018年X月 Revised by @
//------------------------------------------------------------------------------------------------

//---------------------------------【头文件、命名空间包含部分】-----------------------------
//		描述：包含程序所使用的头文件和命名空间
//-------------------------------------------------------------------------------------------------


#ifndef __IMAGE_FFT_H__
#define __IMAGE_FFT_H__

#include <opencv2/opencv.hpp> 
#include "imageBinary.h"
using namespace cv;
using namespace std;
using namespace ObjectImageBinary;

namespace ObjectImageFFT{

#define FLOAT_ZERO_BOUNDS 0.000001     //定义了浮点形  零的边界性质   如：  (abs(value) >=0)  &&   (abs(value) <= FLOAT_ZERO_BOUNDS)

//---------------------------------【CFAR检测算法 接口设计】-----------------------------
//		描述：CFAR检测算法接口设计 与 具体的算法实现
//-------------------------------------------------------------------------------------------------
static void convolveDFT(Mat A, Mat B, Mat& outC);    //32位数据类型  float形
static void convolveDFT_64F(Mat A, Mat B, Mat &outC); //64位数据类型  double形

//图像滤波的各种方式

enum IMAGE_FFT_TYPE{
	Ruly,
	Gaussian,
	Gaussian_Adaptive_Window,
	LogNormal,
	LogNormal_Adaptive_Window
};    

//(具有三个接口，后续可以继续添加实现接口)


class CfarDetectBase{
public:
	virtual void cfarDetect(InputArray src, OutputArray dst) = 0;
    ~CfarDetectBase(){}
};


//基于 瑞利分布 的快速CFAR检测算法 
/*      threshold:  检测门限
	    protectWin: 保护窗口尺寸
	    beijingWin: 背景窗口尺寸
*/
class Ruly_CfarDetect: public CfarDetectBase{
public:
    Ruly_CfarDetect(Size _protectWin, Size _beijingWin, double _threshold);

    virtual void cfarDetect(InputArray src, OutputArray dst);

    ~Ruly_CfarDetect();
protected:
	Size protectWin;
	Size beijingWin;
	double threshold;
};

//基于 高斯双参数分布 的快速CFAR检测算法 
class Gaussian_CfarDetect: public CfarDetectBase{
public:
    Gaussian_CfarDetect(Size _protectWin, Size _beijingWin
                        , double _varRatio, double _threShold
                        , bool _isUseOverallThreshold);



    virtual void cfarDetect(InputArray src, OutputArray dst);

    ~Gaussian_CfarDetect();

protected:
	Size protectWin;
	Size beijingWin;
	double varRatio;
	double threShold;
	bool isUseOverallThreshold;   //是否使用全局的阈值变量
};




	//基于 高斯双参数分布 自适应窗口 的快速CFAR检测算法 
class Gaussian_Adaptive_Window_CfarDetect: public CfarDetectBase{
public:
	Gaussian_Adaptive_Window_CfarDetect(Size _protectWin, Size _beijingWin, 
        double _varRatio, double _threShold, bool _isUseOverallThreshold);


      virtual void cfarDetect(InputArray src, OutputArray dst);


//	  virtual void cfarDetect(InputArray src, OutputArray dst)


      ~Gaussian_Adaptive_Window_CfarDetect();

protected:
	Size protectWin;
	Size beijingWin;
	double varRatio;
	double threShold;
	bool isUseOverallThreshold;   //是否使用全局的阈值变量
};



//基于 对数正态分布 的快速CFAR检测算法 
class LogNormal_CfarDetect: public CfarDetectBase{
public:

    LogNormal_CfarDetect(Size _protectWin, Size _beijingWin
                         , double _varRatio, double _threShold
                         , bool _isUseOverallThreshold);

    virtual void cfarDetect(InputArray src, OutputArray dst);


    ~LogNormal_CfarDetect();

protected:
	//将矩阵为零的元素  置换成1， 用于求对数   注意矩阵中0 浮点形的表现形式
	template<typename _Tp>
    void Set_ZeroElementOfMatrix_To_One(InputArray _src, OutputArray _dst);

protected:
	Size protectWin;
	Size beijingWin;
	double varRatio;
	double threShold;
	bool isUseOverallThreshold;   //是否使用全局的阈值变量
};


//基于 对数正态分布 的快速CFAR检测算法 
class LogNormal_Adaptive_Window_CfarDetect: public LogNormal_CfarDetect{
public:

	LogNormal_Adaptive_Window_CfarDetect(Size _protectWin, Size _beijingWin, 
        double _varRatio, double _threShold, bool _isUseOverallThreshold);

      virtual void cfarDetect(InputArray src, OutputArray dst);

      ~LogNormal_Adaptive_Window_CfarDetect();
};

//---------------------------------【图像FFT操作 参数定义部分】-----------------------------
//		描述：定义图像FFT 所需要参数 的全局变量， 用于接口的调用
//-------------------------------------------------------------------------------------------------
/*enum IMAGE_FFT_TYPE{
Ruly,
Gaussian,
LogNormal,
}; */ 

//// Ruly分布 Ruly 参数 全局变量定义  Ruly_CfarDetect
//static Size g_nRuly_CfarDetect_protectWin = Size(21, 21);
//static Size g_nRuly_CfarDetect_beijingWin = Size(51, 51);
//static double g_nRuly_CfarDetect_threshold = 7;


//// 高斯分布 Gaussian 参数 全局变量定义 Gaussian_CfarDetect
//static Size g_nGaussian_CfarDetect_protectWin = Size(21, 21);
//static Size g_nGaussian_CfarDetect_beijingWin = Size(51, 51);
//static double g_nGaussian_CfarDetect_varRatio = 3;
//static double g_nGaussian_CfarDetect_threShold = 100;
//static bool g_nGaussian_CfarDetect_isUseOverallThreshold = false;

//// 高斯分布  Gaussian_Adaptive_Window 参数 全局变量定义 Gaussian_Adaptive_Window_CfarDetect
//static Size g_nGaussian_Adaptive_Window_CfarDetect_protectWin = Size(21, 21);
//static Size g_nGaussian_Adaptive_Window_CfarDetect_beijingWin = Size(51, 51);
//static double g_nGaussian_Adaptive_Window_CfarDetect_varRatio = 3;
//static double g_nGaussian_Adaptive_Window_CfarDetect_threShold = 100;
//static bool g_nGaussian_Adaptive_Window_CfarDetect_isUseOverallThreshold = false;


//// 对数正态分布  Lognormal  参数 全局变量定义 LogNormal_CfarDetect
//static Size g_nLogNormal_CfarDetect_protectWin = Size(21, 21);
//static Size g_nLogNormal_CfarDetect_beijingWin = Size(51, 51);
//static double g_nLogNormal_CfarDetect_varRatio = 3;
//static double g_nLogNormal_CfarDetect_threShold = 100;
//static bool g_nLogNormal_CfarDetect_isUseOverallThreshold = false;

//// 对数正态分布  Lognormal 自适应窗口  参数 全局变量定义
//static Size g_nLogNormal_Adaptive_Window_CfarDetect_protectWin = Size(21, 21);
//static Size g_nLogNormal_Adaptive_Window_CfarDetect_beijingWin = Size(51, 51);
//static double g_nLogNormal_Adaptive_Window_CfarDetect_varRatio = 3;
//static double g_nLogNormal_Adaptive_Window_CfarDetect_threShold = 100;
//static bool g_nLogNormal_Adaptive_Window_CfarDetect_isUseOverallThreshold = false;

//---------------------------------【图像FFT操作 实例化部分】-----------------------------
//		描述：利用设计模式中的 context模式  分情况实例化 图像FFT操作
//-------------------------------------------------------------------------------------------------
/*enum IMAGE_FFT_TYPE{
Ruly,
Gaussian,
LogNormal,
}; */ 


class ImageCfarDectect
{
public:
	//通过传入不同的滤波类型，进行实例化操作
    ImageCfarDectect(enum IMAGE_FFT_TYPE type);

    void getImageCfarDectectResult(InputArray src, OutputArray dst);

private:
    CfarDetectBase* imageFFTPtr;
};



}




#endif
