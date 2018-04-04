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

#include "imageFFT.h"
#include <QDebug>
using namespace ObjectImageFFT;


void ObjectImageFFT::convolveDFT(Mat A, Mat B, Mat &outC)
{
    // 设置临时变量 扩大傅里叶变换的尺寸
    Mat tempA = Mat::zeros(A.rows + B.rows -1, A.cols + B.cols -1, CV_32FC1);
    Mat tempB = Mat::zeros(tempA.size(), tempA.type());

    //cout << "A.channels: " << A.channels() << endl;
    // 分别复制A和B到tempA和tempB的左上角
    A.copyTo(tempA(Rect(0,0,A.cols, A.rows)));
    B.copyTo(tempB(Rect(0,0,B.cols, B.rows)));


    Mat planesA[] = {Mat_<float>(tempA), Mat::zeros(tempA.size(), CV_32FC1)};
    Mat tempA_complex;
    merge(planesA, 2, tempA_complex);

    Mat planesB[] = {Mat_<float>(tempB), Mat::zeros(tempB.size(), CV_32FC1)};
    Mat tempB_complex;
    merge(planesB, 2, tempB_complex);

    /*cout << "FFT before: \n\n";
    cout << "tempA_complex:		\n" << tempA_complex(Rect(0,0,4, 4)) << endl;
    cout << "tempB_complex:		\n" << tempB_complex(Rect(0,0,4, 4)) << endl;*/

    //【4】进行就地离散傅里叶变换
    dft(tempA_complex, tempA_complex);
    dft(tempB_complex, tempB_complex);

    /*cout << "FFT after: \n\n";

    cout << "tempA_complex:		\n" << tempA_complex(Rect(0,0,4, 4)) << endl;
    cout << "tempB_complex:		\n" << tempB_complex(Rect(0,0,4, 4)) << endl;*/

    // 将得到的频谱相乘，结果存放于tempC_complex中  mulSpectrums()函数可以实现两个复数的相乘

    //Mat tempC_complex;
    mulSpectrums(tempA_complex, tempB_complex, tempA_complex, 0);   //将结果直接存在tempA_complex中


    Mat tempD;
    //dft(tempC_complex, tempD, DFT_INVERSE + DFT_SCALE, C.rows);
    dft(tempA_complex, tempD, DFT_INVERSE+ DFT_SCALE + DFT_REAL_OUTPUT);
    //cout << "tempD:		\n" << tempD.channels() << "\n" << tempD(Rect(0,0,4, 4)) << endl;

    int r_start = static_cast<int>(B.rows/2.0 + 1);
    int r_end = r_start + A.rows - 1;
    int c_start = static_cast<int>(B.cols/2.0 + 1);
    int c_end = c_start + A.cols -1;  //此处返回的矩阵大小和A相同， 返回result1
    outC = tempD(Rect(c_start-1, r_start-1, c_end-c_start+1,r_end-r_start+1));
    //cout << "C: \n" << C(Rect(0,0,4, 4)) << endl;
    return;
}

void ObjectImageFFT::convolveDFT_64F(Mat A, Mat B, Mat &outC)
{
    // 设置临时变量 扩大傅里叶变换的尺寸
    Mat tempA = Mat::zeros(A.rows + B.rows -1, A.cols + B.cols -1, CV_64FC1);
    Mat tempB = Mat::zeros(tempA.size(), tempA.type());

    //cout << "A.channels: " << A.channels() << endl;
    // 分别复制A和B到tempA和tempB的左上角
    A.copyTo(tempA(Rect(0,0,A.cols, A.rows)));
    B.copyTo(tempB(Rect(0,0,B.cols, B.rows)));


    Mat planesA[] = {Mat_<double>(tempA), Mat::zeros(tempA.size(), CV_64FC1)};
    Mat tempA_complex;
    merge(planesA, 2, tempA_complex);

    Mat planesB[] = {Mat_<double>(tempB), Mat::zeros(tempB.size(), CV_64FC1)};
    Mat tempB_complex;
    merge(planesB, 2, tempB_complex);

    /*cout << "FFT before: \n\n";
    cout << "tempA_complex:		\n" << tempA_complex(Rect(0,0,4, 4)) << endl;
    cout << "tempB_complex:		\n" << tempB_complex(Rect(0,0,4, 4)) << endl;*/

    //【4】进行就地离散傅里叶变换
    dft(tempA_complex, tempA_complex);
    dft(tempB_complex, tempB_complex);

    cout << "FFT after: \n\n";

    cout << "tempA_complex(310,26,10,10):		\n" << tempA_complex(Rect(310,26,10,10)) << endl;
    cout << "tempB_complex(310,26,10,10):		\n" << tempB_complex(Rect(310,26,10,10)) << endl;

    // 将得到的频谱相乘，结果存放于tempC_complex中  mulSpectrums()函数可以实现两个复数的相乘

    //Mat tempC_complex;
    mulSpectrums(tempA_complex, tempB_complex, tempA_complex, 0);   //将结果直接存在tempA_complex中

    cout << "tempA_complex(310,26,10,10):		\n" << tempA_complex(Rect(310,26,10,10)) << endl;
    Mat tempD;
    //dft(tempC_complex, tempD, DFT_INVERSE + DFT_SCALE, C.rows);
    dft(tempA_complex, tempD, DFT_INVERSE+ DFT_SCALE + DFT_REAL_OUTPUT);
    cout << "tempD(310,26,10,10):		\n" << tempD.channels() << "\n" << tempD(Rect(310,26,10,10)) << endl;

    int r_start = static_cast<int>(B.rows/2.0 + 1);
    int r_end = r_start + A.rows - 1;
    int c_start = static_cast<int>(B.cols/2.0 + 1);
    int c_end = c_start + A.cols -1;  //此处返回的矩阵大小和A相同， 返回result1
    outC = tempD(Rect(c_start-1, r_start-1, c_end-c_start+1,r_end-r_start+1));
    //cout << "C: \n" << C(Rect(0,0,4, 4)) << endl;
    return;
}



//基于 瑞利分布 的快速CFAR检测算法
/*      threshold:  检测门限
        protectWin: 保护窗口尺寸
        beijingWin: 背景窗口尺寸
*/

Ruly_CfarDetect::Ruly_CfarDetect(Size _protectWin, Size _beijingWin, double _threshold):
      protectWin(_protectWin),
      beijingWin(_beijingWin),
      threshold(_threshold){}

void Ruly_CfarDetect::cfarDetect(InputArray src, OutputArray dst)
{
    qDebug() << "Ruly_CfarDetect::cfarDetect(InputArray src, OutputArray dst)";
    Mat inputImage = Mat_<float>(src.getMat());
    const int rows = static_cast<const int>(inputImage.rows);
    const int cols = static_cast<const int>(inputImage.cols);

    //设置窗口大小
    int threshold_unit_a = beijingWin.width;
    int threshold_unit_r = beijingWin.height;
    int protect_unit_a = protectWin.width;
    int protect_unit_r = protectWin.height;

    int pro_azi_win=(protect_unit_a-1)/2;
    int pro_rang_win=(protect_unit_r-1)/2;
    int thre_azi_win=(threshold_unit_a-1)/2;
    int thre_rang_win=(threshold_unit_r-1)/2;
    int center_a=thre_azi_win+1;
    int center_r=thre_rang_win+1;


    int threshold_power_num = threshold_unit_a*threshold_unit_r - protect_unit_a*protect_unit_r;  //滤波模板内像素的个数
    double threshold_radio = pow(10.0, threshold / 10.0);

    //设置滤波模板
    Mat detectFilterModule = Mat::ones(threshold_unit_a, threshold_unit_r, CV_32FC1);
    detectFilterModule(Rect(center_a - pro_azi_win - 1,
        center_r - pro_rang_win - 1,
        protect_unit_a, protect_unit_r)) = Mat::zeros(protect_unit_a, protect_unit_r, CV_32FC1);

    //求图像的能量值
    Mat inputImagePower = inputImage.mul(inputImage);   //computes multiplication
    //cout << "inputImage:	" << inputImage(Rect(0, 0, 8, 8)) << endl;
    //cout << "inputImagePower:	" << inputImagePower(Rect(0, 0, 8, 8)) << endl;

    //输入的inputImagePower， detectFilterModule输入正确
    Mat detect_power;
    convolveDFT(inputImagePower, detectFilterModule, detect_power);

    double temp = threshold_radio / threshold_power_num;
    //cout << "detect_power:	" << detect_power(Rect(0, 0, 8, 8)) << endl;


    detect_power = detect_power * temp;
    //cout << "detect_power:	" << detect_power(Rect(0, 0, 8, 8)) << endl;

    Mat dstImage = Mat::zeros(src.size(), CV_8UC1);

    //双重循环，遍历所有的像素值  比较能量值得大小，重新获得检测出来的图像矩阵 Mat
    for(int i = 0;i < rows;	i++)  //行循环
    {
        for(int j = 0;j < cols	;j++)   //列循环
        {
            // ---------【开始处理每个像素】-------------
            if (abs(inputImagePower.at<float>(i, j)) > abs(detect_power.at<float>(i, j)))
            {
                dstImage.at<uchar>(i, j) = 255;
            }
            // ----------【处理结束】---------------------
        }  //行处理结束
    }

    //imshow("dstImage", dstImage);
    dstImage.copyTo(dst);
    return ;
}

Ruly_CfarDetect::~Ruly_CfarDetect(){}


//基于 高斯双参数分布 的快速CFAR检测算法

Gaussian_CfarDetect::Gaussian_CfarDetect(Size _protectWin, Size _beijingWin, double _varRatio, double _threShold, bool _isUseOverallThreshold):
                    protectWin(_protectWin),
                    beijingWin(_beijingWin),
                    varRatio(_varRatio),
                    threShold(_threShold),
                    isUseOverallThreshold(_isUseOverallThreshold){}



void Gaussian_CfarDetect::cfarDetect(InputArray src, OutputArray dst)
{
    qDebug() << "Gaussian_CfarDetect::cfarDetect(InputArray src, OutputArray dst)";

    Mat inputImage = Mat_<float>(src.getMat());
    const int rows = static_cast<const int>(inputImage.rows);
    const int cols = static_cast<const int>(inputImage.cols);

    //设置窗口大小
    int threshold_unit_a = beijingWin.width;
    int threshold_unit_r = beijingWin.height;
    int protect_unit_a = protectWin.width;
    int protect_unit_r = protectWin.height;

    int pro_azi_win=(protect_unit_a-1)/2;
    int pro_rang_win=(protect_unit_r-1)/2;
    int thre_azi_win=(threshold_unit_a-1)/2;
    int thre_rang_win=(threshold_unit_r-1)/2;
    int center_a=thre_azi_win+1;
    int center_r=thre_rang_win+1;


    int threshold_power_num = threshold_unit_a*threshold_unit_r - protect_unit_a*protect_unit_r;  //滤波模板内像素的个数

    //设置滤波模板
    Mat detectFilterModule = Mat::ones(threshold_unit_a, threshold_unit_r, CV_32FC1);
    detectFilterModule(Rect(center_a - pro_azi_win - 1,
        center_r - pro_rang_win - 1,
        protect_unit_a, protect_unit_r)) = Mat::zeros(protect_unit_a, protect_unit_r, CV_32FC1);

    //求去高斯分布中的 均值 和 标准差 的值

    Mat clutter = inputImage;
    Mat clutter_power = inputImage.mul(inputImage);   //用于统计杂波图像的能量值

    //获取杂波的均值
    Mat clutter_miu;
    convolveDFT(clutter, detectFilterModule, clutter_miu);
    clutter_miu = clutter_miu / threshold_power_num;
    //cout << "clutter_miu: \n" << clutter_miu(Rect(0,0,4,4)) << endl;

    Mat clutter_miu_pow = clutter_miu.mul(clutter_miu);
    //cout << "clutter_miu_pow: \n" << clutter_miu_pow(Rect(0,0,4,4)) << endl;

    //获取杂波的标准差
    Mat clutter_sigma;
    convolveDFT(clutter_power, detectFilterModule, clutter_sigma);
    //cout << "clutter_power: \n" << clutter_power(Rect(310,26,10,10)) << endl; //因为clutter为inputImage图像的能量值，其中没有很多零，所以DFT正确 测试正确

    //cout << "clutter_sigma: \n" << clutter_sigma(Rect(310,26,10,10)) << endl; //这里有正确了


    clutter_sigma = clutter_sigma / threshold_power_num;
    //cout << "clutter_sigma: \n" << clutter_sigma(Rect(310,26,20,20)) << endl;   //clutter_sigma 测试正确

    Mat temp_sigma;
    absdiff(clutter_sigma, clutter_miu_pow, temp_sigma);
    //cout << "temp_sigma: \n" << temp_sigma(Rect(310,26,20,20)) << endl;        //temp_sigma 测试正确
    sqrt(temp_sigma, clutter_sigma);
    //cout << "clutter_sigma: \n" << clutter_sigma(Rect(310,26,20,20)) << endl;  //clutter_sigma 测试正确

    //获取高斯分布 矩阵       （iamge = miu + t*sigma）
    Mat GaussianMat;
    scaleAdd(clutter_sigma, varRatio, clutter_miu, GaussianMat);
    //cout << "GaussianMat: \n" << GaussianMat(Rect(310,26,20,20)) << endl;



    Mat dstImage = Mat::zeros(src.size(), CV_8UC1);

    //双重循环，遍历所有的像素值  比较能量值得大小，重新获得检测出来的图像矩阵 Mat
    int count = 0;
    if (isUseOverallThreshold)   //如果设置了全局阈值变量
    {
        for(int i = 0;i < rows;	i++)  //行循环
        {
            for(int j = 0;j < cols; j++)   //列循环
            {
                // ---------【开始处理每个像素】-------------
                if ( abs(inputImage.at<float>(i, j)) > abs(GaussianMat.at<float>(i, j))
                    && abs(inputImage.at<float>(i, j)) > threShold)
                {
                    dstImage.at<uchar>(i, j) = 255;
                    count++;
                }
                // ----------【处理结束】---------------------
            }  //行处理结束
        }
    }
    else
    {
        for(int i = 0;i < rows;	i++)  //行循环
        {
            for(int j = 0;j < cols; j++)   //列循环
            {
                // ---------【开始处理每个像素】-------------
                if ( abs(inputImage.at<float>(i, j)) > abs(GaussianMat.at<float>(i, j)))
                {
                    dstImage.at<uchar>(i, j) = 255;
                    count++;
                }
                // ----------【处理结束】---------------------
            }  //行处理结束
        }
    }



    //imshow("dstImage", dstImage);
    dstImage.copyTo(dst);

    return ;
}


Gaussian_CfarDetect::~Gaussian_CfarDetect(){}





    //基于 高斯双参数分布 自适应窗口 的快速CFAR检测算法

Gaussian_Adaptive_Window_CfarDetect::Gaussian_Adaptive_Window_CfarDetect(Size _protectWin, Size _beijingWin,
    double _varRatio, double _threShold, bool _isUseOverallThreshold):
  protectWin(_protectWin),
      beijingWin(_beijingWin),
      varRatio(_varRatio),
      threShold(_threShold),
    isUseOverallThreshold(_isUseOverallThreshold){}



  void Gaussian_Adaptive_Window_CfarDetect::cfarDetect(InputArray src, OutputArray dst)
  {

      qDebug() << "Gaussian_Adaptive_Window_CfarDetect::cfarDetect(InputArray src, OutputArray dst)";

      //The first Step: 输入图像src，根据otsu算法，算出自适应全局阈值，获取src图像的二值化图像
      Mat binaryOut;

//      cv::threshold( src.getMat(), binaryOut, 100
//                     , 1, cv::THRESH_BINARY_INV | cv::THRESH_OTSU );  //得到原始图像的反二值化图像

      cv::threshold( src.getMat(), binaryOut, DateMgr::getDataParas()->cfarDetectParas->adaptiveThreshold
                     , 1, cv::THRESH_BINARY_INV);  //得到原始图像的反二值化图像

      /*cout << binaryOut(Rect(0,0,4,4)) << "\n" << endl;
      imshow("binaryOut:	", binaryOut);*/
      binaryOut = Mat_<float>(binaryOut);

      Mat inputImage = Mat_<float>(src.getMat());
      const int rows = static_cast<const int>(inputImage.rows);
      const int cols = static_cast<const int>(inputImage.cols);

      //设置窗口大小
      int threshold_unit_a = beijingWin.width;
      int threshold_unit_r = beijingWin.height;
      int protect_unit_a = protectWin.width;
      int protect_unit_r = protectWin.height;

      int pro_azi_win=(protect_unit_a-1)/2;
      int pro_rang_win=(protect_unit_r-1)/2;
      int thre_azi_win=(threshold_unit_a-1)/2;
      int thre_rang_win=(threshold_unit_r-1)/2;
      int center_a=thre_azi_win+1;
      int center_r=thre_rang_win+1;


      //设置滤波模板
      Mat detectFilterModule = Mat::ones(threshold_unit_a, threshold_unit_r, CV_32FC1);
      detectFilterModule(Rect(center_a - pro_azi_win - 1,
          center_r - pro_rang_win - 1,
          protect_unit_a, protect_unit_r)) = Mat::zeros(protect_unit_a, protect_unit_r, CV_32FC1);

      //滤波模板内杂波像素个数统计
      Mat threshold_power_num;
      convolveDFT(binaryOut, detectFilterModule, threshold_power_num);
      //cout << "threshold_power_num: \n" << threshold_power_num(Rect(310,26,10,10)) << endl;

      //求去高斯分布中的 均值 和 标准差 的值
      Mat clutter = inputImage.mul(binaryOut);
      Mat clutter_power = clutter.mul(clutter);   //用于统计杂波图像的能量值（导致下面错误的原因是，杂波能量赋值没有弄清楚 fuck）

      //获取杂波的均值
      Mat clutter_miu;
      convolveDFT(clutter, detectFilterModule, clutter_miu);
      //clutter_miu = clutter_miu / threshold_power_num;

      divide(clutter_miu, threshold_power_num, clutter_miu);

      //cout << "clutter_miu: \n" << clutter_miu(Rect(310,26,10,10)) << endl;



      //获取杂波的标准差
      Mat clutter_miu_pow = clutter_miu.mul(clutter_miu);
      //cout << "clutter_miu_pow: \n" << clutter_miu_pow(Rect(310,26,10,10)) << endl;


      Mat clutter_sigma;
      Mat clutter_sigma1;
      convolveDFT(clutter_power, detectFilterModule, clutter_sigma1);
      //clutter_sigma = clutter_sigma / threshold_power_num;
      //cout << "clutter_power: \n" << clutter_power(Rect(310,26,10,10)) << endl; //测试正确

      //cout << "clutter_sigma1: \n" << clutter_sigma1(Rect(310,26,10,10)) << endl; //测试不正确

      divide(clutter_sigma1, threshold_power_num, clutter_sigma);
      //cout << "clutter_sigma: \n" << clutter_sigma(Rect(310,26,10,10)) << endl;

      Mat temp_sigma;
      absdiff(clutter_sigma, clutter_miu_pow, temp_sigma);
      //cout << "temp_sigma: \n" << temp_sigma(Rect(0,0,4,4)) << endl;
      sqrt(temp_sigma, clutter_sigma);
      //cout << "clutter_sigma: \n" << clutter_sigma(Rect(310,26,10,10)) << endl;

      //获取高斯分布 矩阵       （iamge = miu + t*sigma）
      Mat GaussianMat;
      scaleAdd(clutter_sigma, varRatio, clutter_miu, GaussianMat);
      //cout << "GaussianMat(310,26,10,10): \n" << GaussianMat(Rect(310,26,10,10)) << endl;
      //cout << "inputImage: \n" << inputImage(Rect(310,26,10,10)) << endl;



      Mat dstImage = Mat::zeros(src.size(), CV_8UC1);

      //双重循环，遍历所有的像素值  比较能量值得大小，重新获得检测出来的图像矩阵 Mat
      int count = 0;
      if (isUseOverallThreshold)
      {
          for(int i = 0;i < rows;	i++)  //行循环
          {
              for(int j = 0;j < cols; j++)   //列循环
              {
                  // ---------【开始处理每个像素】-------------
                  if (abs(inputImage.at<float>(i, j)) > abs(GaussianMat.at<float>(i, j))
                      && abs(inputImage.at<float>(i, j)) > threShold)
                  {
                      dstImage.at<uchar>(i, j) = 255;
                      count++;
                  }
                  // ----------【处理结束】---------------------
              }  //行处理结束
          }
      }
      else
      {
          for(int i = 0;i < rows;	i++)  //行循环
          {
              for(int j = 0;j < cols; j++)   //列循环
              {
                  // ---------【开始处理每个像素】-------------
                  if (abs(inputImage.at<float>(i, j)) > abs(GaussianMat.at<float>(i, j)))
                  {
                      dstImage.at<uchar>(i, j) = 255;
                      count++;
                  }
                  // ----------【处理结束】---------------------
              }  //行处理结束
          }


      }


      //imshow("dstImage", dstImage);
      dstImage.copyTo(dst);

      return ;
  }

//	  virtual void cfarDetect(InputArray src, OutputArray dst)
//	  {
//		  //The first Step: 输入图像src，根据otsu算法，算出自适应全局阈值，获取src图像的二值化图像
//		  Mat binaryOut;
//
//		  cv::threshold( src.getMat(), binaryOut, 100, 1, cv::THRESH_BINARY_INV | cv::THRESH_OTSU );  //得到原始图像的反二值化图像
//
//		  /*cout << binaryOut(Rect(0,0,4,4)) << "\n" << endl;
//		  imshow("binaryOut:	", binaryOut);*/
//		  binaryOut = Mat_<double>(binaryOut);
//
//		  Mat inputImage = Mat_<double>(src.getMat());
//		  const int rows = static_cast<const int>(inputImage.rows);
//		  const int cols = static_cast<const int>(inputImage.cols);
//
//		  //设置窗口大小
//		  int threshold_unit_a = beijingWin.width;
//		  int threshold_unit_r = beijingWin.height;
//		  int protect_unit_a = protectWin.width;
//		  int protect_unit_r = protectWin.height;
//
//		  int pro_azi_win=(protect_unit_a-1)/2;
//		  int pro_rang_win=(protect_unit_r-1)/2;
//		  int thre_azi_win=(threshold_unit_a-1)/2;
//		  int thre_rang_win=(threshold_unit_r-1)/2;
//		  int center_a=thre_azi_win+1;
//		  int center_r=thre_rang_win+1;
//
//
//		  //设置滤波模板
//		  Mat detectFilterModule = Mat::ones(threshold_unit_a, threshold_unit_r, CV_64FC1);
//		  detectFilterModule(Rect(center_a - pro_azi_win - 1,
//			  center_r - pro_rang_win - 1,
//			  protect_unit_a, protect_unit_r)) = Mat::zeros(protect_unit_a, protect_unit_r, CV_64FC1);
//
//		  //滤波模板内杂波像素个数统计
//		  Mat threshold_power_num;
//		  convolveDFT_64F(binaryOut, detectFilterModule, threshold_power_num);
//		  cout << "threshold_power_num: \n" << threshold_power_num(Rect(310,26,10,10)) << endl;  //64F测试正确
//
//		  //求去高斯分布中的 均值 和 标准差 的值
//		  Mat clutter = inputImage.mul(binaryOut);
//		  Mat clutter_power = clutter.mul(clutter);   //用于统计杂波图像的能量值（导致下面错误的原因是，杂波能量赋值没有弄清楚 fuck）
//
//		  //获取杂波的均值
//		  Mat clutter_miu;
//		  convolveDFT_64F(clutter, detectFilterModule, clutter_miu);
//		  //clutter_miu = clutter_miu / threshold_power_num;
//
//		  divide(clutter_miu, threshold_power_num, clutter_miu);
//
//		  cout << "clutter_miu: \n" << clutter_miu(Rect(310,26,10,10)) << endl;
//
//
//
//		  //获取杂波的标准差
//		  Mat clutter_miu_pow = clutter_miu.mul(clutter_miu);
//		  cout << "clutter_miu_pow: \n" << clutter_miu_pow(Rect(310,26,10,10)) << endl;
//
//
//		  Mat clutter_sigma;
//		  Mat clutter_sigma1;
//		  convolveDFT_64F(clutter_power, detectFilterModule, clutter_sigma1);
//		  //clutter_sigma = clutter_sigma / threshold_power_num;
//		  cout << "clutter_power: \n" << clutter_power(Rect(0,0,10,10)) << endl; //测试不正确（查找了一下午错误的原因是，变量赋值没有弄清楚）
//
//		  cout << "clutter_sigma1: \n" << clutter_sigma1(Rect(310,26,10,10)) << endl; //测试不正确
//
//		  divide(clutter_sigma1, threshold_power_num, clutter_sigma);
//		  cout << "clutter_sigma: \n" << clutter_sigma(Rect(310,26,10,10)) << endl;
//
//		  Mat temp_sigma;
//		  absdiff(clutter_sigma, clutter_miu_pow, temp_sigma);
//		  //cout << "temp_sigma: \n" << temp_sigma(Rect(0,0,4,4)) << endl;
//		  sqrt(temp_sigma, clutter_sigma);
//		  cout << "clutter_sigma: \n" << clutter_sigma(Rect(310,26,10,10)) << endl;
//
//		  //获取高斯分布 矩阵       （iamge = miu + t*sigma）
//		  Mat GaussianMat;
//		  scaleAdd(clutter_sigma, varRatio, clutter_miu, GaussianMat);
//		  cout << "GaussianMat(310,26,10,10): \n" << GaussianMat(Rect(310,26,10,10)) << endl;
//		  cout << "inputImage: \n" << inputImage(Rect(310,26,10,10)) << endl;
//
//
//
//		  Mat dstImage = Mat::zeros(src.size(), CV_8UC1);
///*
//		  float maxVal = 0.0;
//		  for(int i = 0;i < rows;	i++)  //行循环
//		  {
//			  for(int j = 0;j < cols; j++)   //列循环
//			  {
//				  // ---------【开始处理每个像素】-------------
//				  if (GaussianMat.at<float>(i, j) > maxVal)
//				  {
//					  maxVal = GaussianMat.at<float>(i, j);
//					  cout << "(" << i << "," << j << ")" << " = " << maxVal << "\n";
//				  }
//				  // ----------【处理结束】---------------------
//			  }  //行处理结束
//		  }  */
//
//
//		  //双重循环，遍历所有的像素值  比较能量值得大小，重新获得检测出来的图像矩阵 Mat
//		  int count = 0;
//		  for(int i = 0;i < rows;	i++)  //行循环
//		  {
//			  for(int j = 0;j < cols; j++)   //列循环
//			  {
//				  // ---------【开始处理每个像素】-------------
//				  if (abs(inputImage.at<float>(i, j)) > abs(GaussianMat.at<float>(i, j))
//					  && abs(inputImage.at<float>(i, j)) > threShold)
//				  {
//					  dstImage.at<uchar>(i, j) = 255;
//					  count++;
//				  }
//				  // ----------【处理结束】---------------------
//			  }  //行处理结束
//		  }
//
//		  /*
//		  vector<vector<float>> vec2;
//		  vector<float> vec;
//		  for(int i = 0;i < rows;	i++)  //行循环
//		  {
//			  for(int j = 0;j < cols; j++)   //列循环
//			  {
//				  // ---------【开始处理每个像素】-------------
//				  if (abs(inputImage.at<float>(i, j)) > abs(GaussianMat.at<float>(i, j))
//					  && abs(inputImage.at<float>(i, j)) > threShold)
//				  {
//					  dstImage.at<uchar>(i, j) = 255;
//					  vec.push_back(i);
//					  vec.push_back(j);
//					  vec.push_back(inputImage.at<float>(i, j));
//					  vec.push_back(GaussianMat.at<float>(i, j));
//					  vec2.push_back(vec);
//					  vec.clear();
//					  cout << i << " " << j << " " << inputImage.at<float>(i, j) << " " << GaussianMat.at<float>(i, j) << " \n";
//				  }
//				  // ----------【处理结束】---------------------
//			  }  //行处理结束
//		  }  */
//
//
//
//
//		  //imshow("dstImage", dstImage);
//		  dstImage.copyTo(dst);
//
//		  return ;
//	  }

  Gaussian_Adaptive_Window_CfarDetect::~Gaussian_Adaptive_Window_CfarDetect(){}



//基于 对数正态分布 的快速CFAR检测算法

LogNormal_CfarDetect::LogNormal_CfarDetect(Size _protectWin, Size _beijingWin, double _varRatio, double _threShold, bool _isUseOverallThreshold):
  protectWin(_protectWin),
      beijingWin(_beijingWin),
      varRatio(_varRatio),
      threShold(_threShold),
  isUseOverallThreshold(_isUseOverallThreshold){}

void LogNormal_CfarDetect::cfarDetect(InputArray src, OutputArray dst)
{
    qDebug() << "LogNormal_CfarDetect::cfarDetect(InputArray src, OutputArray dst)";

    Mat inputImage = Mat_<float>(src.getMat());
    const int rows = static_cast<const int>(inputImage.rows);
    const int cols = static_cast<const int>(inputImage.cols);

    //设置窗口大小
    int threshold_unit_a = beijingWin.width;
    int threshold_unit_r = beijingWin.height;
    int protect_unit_a = protectWin.width;
    int protect_unit_r = protectWin.height;

    int pro_azi_win=(protect_unit_a-1)/2;
    int pro_rang_win=(protect_unit_r-1)/2;
    int thre_azi_win=(threshold_unit_a-1)/2;
    int thre_rang_win=(threshold_unit_r-1)/2;
    int center_a=thre_azi_win+1;
    int center_r=thre_rang_win+1;


    int threshold_power_num = threshold_unit_a*threshold_unit_r - protect_unit_a*protect_unit_r;  //滤波模板内像素的个数

    //设置滤波模板
    Mat detectFilterModule = Mat::ones(threshold_unit_a, threshold_unit_r, CV_32FC1);
    detectFilterModule(Rect(center_a - pro_azi_win - 1,
        center_r - pro_rang_win - 1,
        protect_unit_a, protect_unit_r)) = Mat::zeros(protect_unit_a, protect_unit_r, CV_32FC1);

    //确定杂波矩阵
    //在求对数前 先对矩阵进行预处理 将矩阵中为0的元素 改成1， 其它的元素不变
    Set_ZeroElementOfMatrix_To_One<float>(inputImage, inputImage);

    Mat original_image_power;
    //! computes natural logarithm of absolute value of each matrix element: dst = log(abs(src))
    log(inputImage, original_image_power);

    //cout <<"original_image_power:		\n" <<  original_image_power(Rect(0, 0, 4, 4)) << endl;
    //求去高斯分布中的 均值 和 标准差 的值
    Mat clutter = original_image_power;

    //cout <<"inputImage:		\n" <<  inputImage(Rect(0, 0, 4, 4)) << endl;

    Mat clutter_power = clutter.mul(clutter);
    //cout <<"clutter_power:		\n" << clutter_power(Rect(0, 0, 4, 4)) << endl;

    //获取杂波的均值
    Mat clutter_miu;
    convolveDFT(clutter, detectFilterModule, clutter_miu);
    clutter_miu = clutter_miu / threshold_power_num;
    //cout << "clutter_miu: \n" << clutter_miu(Rect(0,0,4,4)) << endl;

    Mat clutter_miu_pow = clutter_miu.mul(clutter_miu);
    //cout << "clutter_miu_pow: \n" << clutter_miu_pow(Rect(0,0,4,4)) << endl;

    //获取杂波的标准差
    Mat clutter_sigma;
    convolveDFT(clutter_power, detectFilterModule, clutter_sigma);
    clutter_sigma = clutter_sigma / threshold_power_num;
    //cout << "clutter_sigma: \n" << clutter_sigma(Rect(0,0,4,4)) << endl;

    Mat temp_sigma;
    absdiff(clutter_sigma, clutter_miu_pow, temp_sigma);
    //cout << "temp_sigma: \n" << temp_sigma(Rect(0,0,4,4)) << endl;
    sqrt(temp_sigma, clutter_sigma);
    //cout << "clutter_sigma: \n" << clutter_sigma(Rect(0,0,4,4)) << endl;

    //获取高斯分布 矩阵       （iamge = miu + t*sigma）
    Mat GaussianMat;
    scaleAdd(clutter_sigma, varRatio, clutter_miu, GaussianMat);
    //cout << "GaussianMat: \n" << GaussianMat(Rect(0,0,4,4)) << endl;



    Mat dstImage = Mat::zeros(src.size(), CV_8UC1);

    //双重循环，遍历所有的像素值  比较能量值得大小，重新获得检测出来的图像矩阵 Mat
    int count = 0;
    if (isUseOverallThreshold)
    {
        for(int i = 0;i < rows;	i++)  //行循环
        {
            for(int j = 0;j < cols; j++)   //列循环
            {
                // ---------【开始处理每个像素】-------------
                if (abs(original_image_power.at<float>(i, j)) > abs(GaussianMat.at<float>(i, j))
                    && abs(original_image_power.at<float>(i, j)) > log(static_cast<float>(threShold)))
                {
                    dstImage.at<uchar>(i, j) = 255;
                    count++;
                }
                // ----------【处理结束】---------------------
            }  //行处理结束
        }

    }
    else
    {
        for(int i = 0;i < rows;	i++)  //行循环
        {
            for(int j = 0;j < cols; j++)   //列循环
            {
                // ---------【开始处理每个像素】-------------
                if (abs(original_image_power.at<float>(i, j)) > abs(GaussianMat.at<float>(i, j)))
                {
                    dstImage.at<uchar>(i, j) = 255;
                    count++;
                }
                // ----------【处理结束】---------------------
            }  //行处理结束
        }

    }


    //imshow("dstImage", dstImage);
    dstImage.copyTo(dst);

    return ;

}

LogNormal_CfarDetect::~LogNormal_CfarDetect(){}

//将矩阵为零的元素  置换成1， 用于求对数   注意矩阵中0 浮点形的表现形式
template<typename _Tp>
void LogNormal_CfarDetect::Set_ZeroElementOfMatrix_To_One(InputArray _src, OutputArray _dst)
{
    Mat src = Mat_<_Tp>(_src.getMat());
    if (src.channels() == 1)     //处理单通道的图像，此函数不处理3通道的图像
    {
        for(int i = 0;i < src.rows;	i++)  //行循环
        {
            for(int j = 0;j < src.cols; j++)   //列循环
            {
                // ---------【开始处理每个像素】-------------
                if ((src.at<_Tp>(i, j) >= -FLOAT_ZERO_BOUNDS)
                    && (src.at<_Tp>(i, j) <= FLOAT_ZERO_BOUNDS))
                {
                    src.at<_Tp>(i, j) = 1;
                }
                // ----------【处理结束】---------------------
            }  //行处理结束
        }

        src.copyTo(_dst);
        return ;
    }else
    {
        cout << "This function can only deal with single channel image" << endl;
        return ;
    }

}





//基于 对数正态分布 的快速CFAR检测算法
LogNormal_Adaptive_Window_CfarDetect::LogNormal_Adaptive_Window_CfarDetect(Size _protectWin, Size _beijingWin,
    double _varRatio, double _threShold, bool _isUseOverallThreshold):
  LogNormal_CfarDetect(_protectWin, _beijingWin, _varRatio, _threShold, _isUseOverallThreshold){}

  void LogNormal_Adaptive_Window_CfarDetect::cfarDetect(InputArray src, OutputArray dst)
  {
      qDebug() << "LogNormal_Adaptive_Window_CfarDetect::cfarDetect(InputArray src, OutputArray dst)";

      //The first Step: 输入图像src，根据otsu算法，算出自适应全局阈值，获取src图像的二值化图像
      Mat binaryOut;



    cv::threshold( src.getMat(), binaryOut, DateMgr::getDataParas()->cfarDetectParas->adaptiveThreshold
                   , 1, cv::THRESH_BINARY_INV);  //得到原始图像的反二值化图像

//      cv::threshold( src.getMat(), binaryOut, 100, 1, cv::THRESH_BINARY_INV | cv::THRESH_OTSU );  //得到原始图像的反二值化图像

      /*cout << binaryOut(Rect(0,0,4,4)) << "\n" << endl;
      imshow("binaryOut:	", binaryOut);*/
      binaryOut = Mat_<float>(binaryOut);


      Mat inputImage = Mat_<float>(src.getMat());
      const int rows = static_cast<const int>(inputImage.rows);
      const int cols = static_cast<const int>(inputImage.cols);

      //设置窗口大小
      int threshold_unit_a = beijingWin.width;
      int threshold_unit_r = beijingWin.height;
      int protect_unit_a = protectWin.width;
      int protect_unit_r = protectWin.height;

      int pro_azi_win=(protect_unit_a-1)/2;
      int pro_rang_win=(protect_unit_r-1)/2;
      int thre_azi_win=(threshold_unit_a-1)/2;
      int thre_rang_win=(threshold_unit_r-1)/2;
      int center_a=thre_azi_win+1;
      int center_r=thre_rang_win+1;

      //设置滤波模板
      Mat detectFilterModule = Mat::ones(threshold_unit_a, threshold_unit_r, CV_32FC1);
      detectFilterModule(Rect(center_a - pro_azi_win - 1,
          center_r - pro_rang_win - 1,
          protect_unit_a, protect_unit_r)) = Mat::zeros(protect_unit_a, protect_unit_r, CV_32FC1);


      //滤波模板内杂波像素个数统计
      Mat threshold_power_num;
      convolveDFT(binaryOut, detectFilterModule, threshold_power_num);
      //cout << "threshold_power_num: \n" << threshold_power_num(Rect(310,26,10,10)) << endl;


      //确定杂波矩阵

      Mat pure_clutter_image = inputImage.mul(binaryOut);
      Set_ZeroElementOfMatrix_To_One<float>(pure_clutter_image, pure_clutter_image);
      Set_ZeroElementOfMatrix_To_One<float>(inputImage, inputImage);

      //在求对数前 先对矩阵进行预处理 将矩阵中为0的元素 改成1， 其它的元素不变
      Mat original_image_power;
      //! computes natural logarithm of absolute value of each matrix element: dst = log(abs(src))
      log(inputImage, original_image_power);
      //cout <<"original_image_power:		\n" <<  original_image_power(Rect(0, 0, 4, 4)) << endl;


      //求去高斯分布中的 均值 和 标准差 的值
      Mat clutter;
      log(pure_clutter_image, clutter);

      //cout <<"inputImage:		\n" <<  inputImage(Rect(0, 0, 4, 4)) << endl;

      Mat clutter_power = clutter.mul(clutter);
      //cout <<"clutter_power:		\n" << clutter_power(Rect(0, 0, 4, 4)) << endl;

      //获取杂波的均值
      Mat clutter_miu;
      convolveDFT(clutter, detectFilterModule, clutter_miu);
      //clutter_miu = clutter_miu / threshold_power_num;

      divide(clutter_miu, threshold_power_num, clutter_miu);

      //cout << "clutter_miu: \n" << clutter_miu(Rect(310,26,10,10)) << endl;



      //获取杂波的标准差
      Mat clutter_miu_pow = clutter_miu.mul(clutter_miu);
      //cout << "clutter_miu_pow: \n" << clutter_miu_pow(Rect(310,26,10,10)) << endl;


      Mat clutter_sigma;
      Mat clutter_sigma1;
      convolveDFT(clutter_power, detectFilterModule, clutter_sigma1);
      //clutter_sigma = clutter_sigma / threshold_power_num;
      //cout << "clutter_power: \n" << clutter_power(Rect(310,26,10,10)) << endl; //测试正确

      //cout << "clutter_sigma1: \n" << clutter_sigma1(Rect(310,26,10,10)) << endl; //测试不正确

      divide(clutter_sigma1, threshold_power_num, clutter_sigma);
      //cout << "clutter_sigma: \n" << clutter_sigma(Rect(310,26,10,10)) << endl;

      Mat temp_sigma;
      absdiff(clutter_sigma, clutter_miu_pow, temp_sigma);
      //cout << "temp_sigma: \n" << temp_sigma(Rect(0,0,4,4)) << endl;
      sqrt(temp_sigma, clutter_sigma);
      //cout << "clutter_sigma: \n" << clutter_sigma(Rect(310,26,10,10)) << endl;

      //获取高斯分布 矩阵       （iamge = miu + t*sigma）
      Mat GaussianMat;
      scaleAdd(clutter_sigma, varRatio, clutter_miu, GaussianMat);
      //cout << "GaussianMat(310,26,10,10): \n" << GaussianMat(Rect(310,26,10,10)) << endl;
      //cout << "inputImage: \n" << inputImage(Rect(310,26,10,10)) << endl;



      Mat dstImage = Mat::zeros(src.size(), CV_8UC1);

      //双重循环，遍历所有的像素值  比较能量值得大小，重新获得检测出来的图像矩阵 Mat
      int count = 0;
      if (isUseOverallThreshold)
      {
          for(int i = 0;i < rows;	i++)  //行循环
          {
              for(int j = 0;j < cols; j++)   //列循环
              {
                  // ---------【开始处理每个像素】-------------
                  if (abs(original_image_power.at<float>(i, j)) > abs(GaussianMat.at<float>(i, j))
                      && abs(original_image_power.at<float>(i, j)) > log(static_cast<float>(threShold)))
                  {
                      dstImage.at<uchar>(i, j) = 255;
                      count++;
                  }
                  // ----------【处理结束】---------------------
              }  //行处理结束
          }

      }
      else
      {
          for(int i = 0;i < rows;	i++)  //行循环
          {
              for(int j = 0;j < cols; j++)   //列循环
              {
                  // ---------【开始处理每个像素】-------------
                  if (abs(original_image_power.at<float>(i, j)) > abs(GaussianMat.at<float>(i, j)))
                  {
                      dstImage.at<uchar>(i, j) = 255;
                      count++;
                  }
                  // ----------【处理结束】---------------------
              }  //行处理结束
          }

      }

      //imshow("dstImage", dstImage);
      dstImage.copyTo(dst);

      return ;

  }

  LogNormal_Adaptive_Window_CfarDetect::~LogNormal_Adaptive_Window_CfarDetect(){}


ImageCfarDectect::ImageCfarDectect(enum IMAGE_FFT_TYPE type)
{
    switch (type)
    {
    case Ruly:
        imageFFTPtr = new Ruly_CfarDetect( QSizeToSize(DateMgr::getDataParas()->cfarDetectParas->protectWin)
                                       , QSizeToSize(DateMgr::getDataParas()->cfarDetectParas->backGroundWin)
                                       , DateMgr::getDataParas()->cfarDetectParas->RulyThreldRadio);
        break;

    case Gaussian:
        imageFFTPtr = new Gaussian_CfarDetect( QSizeToSize(DateMgr::getDataParas()->cfarDetectParas->protectWin)
                                           , QSizeToSize(DateMgr::getDataParas()->cfarDetectParas->backGroundWin)
                                           , DateMgr::getDataParas()->cfarDetectParas->falseAlarmRate
                                           , DateMgr::getDataParas()->cfarDetectParas->adaptiveThreshold
                                           , DateMgr::getDataParas()->cfarDetectParas->isUseOverallThreshold);
        break;

    case Gaussian_Adaptive_Window:
        imageFFTPtr = new Gaussian_Adaptive_Window_CfarDetect( QSizeToSize(DateMgr::getDataParas()->cfarDetectParas->protectWin),
            QSizeToSize(DateMgr::getDataParas()->cfarDetectParas->backGroundWin),
            DateMgr::getDataParas()->cfarDetectParas->falseAlarmRate,
            DateMgr::getDataParas()->cfarDetectParas->adaptiveThreshold,
            DateMgr::getDataParas()->cfarDetectParas->isUseOverallThreshold);
        break;

    case LogNormal:
        imageFFTPtr = new LogNormal_CfarDetect(
      QSizeToSize(DateMgr::getDataParas()->cfarDetectParas->protectWin),
      QSizeToSize(DateMgr::getDataParas()->cfarDetectParas->backGroundWin),
      DateMgr::getDataParas()->cfarDetectParas->falseAlarmRate,
      DateMgr::getDataParas()->cfarDetectParas->adaptiveThreshold,
      DateMgr::getDataParas()->cfarDetectParas->isUseOverallThreshold);
        break;

    case LogNormal_Adaptive_Window:
        imageFFTPtr = new LogNormal_Adaptive_Window_CfarDetect(
      QSizeToSize(DateMgr::getDataParas()->cfarDetectParas->protectWin),
      QSizeToSize(DateMgr::getDataParas()->cfarDetectParas->backGroundWin),
      DateMgr::getDataParas()->cfarDetectParas->falseAlarmRate,
      DateMgr::getDataParas()->cfarDetectParas->adaptiveThreshold,
      DateMgr::getDataParas()->cfarDetectParas->isUseOverallThreshold);
            break;

    default:
        break;
    }
}



void ImageCfarDectect::getImageCfarDectectResult(InputArray src, OutputArray dst)
{
    imageFFTPtr->cfarDetect(src, dst);    //通过多态调用各自的滤波函数
}


