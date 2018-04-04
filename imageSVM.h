//--------------------------------------【程序说明】-------------------------------------------
//		Program description：Get the hog features of the image, use SVM for training and get the model 
//		IDE version：Visual Studio 2010
//		OpenCV version：	2.4.9
//		2018年1月17日 Created by @章林
//		2018年X月 Revised by @
//------------------------------------------------------------------------------------------------

//---------------------------------【头文件、命名空间包含部分】-----------------------------
//		描述：包含程序所使用的头文件和命名空间
//-------------------------------------------------------------------------------------------------
#ifndef __IMAGE_SVM_H__
#define __IMAGE_SVM_H__

#include <opencv2/opencv.hpp> 
#include <string>
#include <vector>
#include "debugInfo.h"
#include "dirent.h"
#include "datemgr.h"
#include "parametersetting.h"
using namespace cv;
using namespace std;

namespace ObjectImageSVM{

//define positive and negative sample label 
#define POSITIVE_SAMPLE_LABEL 1
#define NEGATIVE_SAMPLE_LABEL -1

//define the number of iterative
#define ITERATIVE_NUMBER 1000

//define: is it necessary to retrain svm model  (default value: true)
#define IS_RETRAIN true

//继承自CvSVM的类，因为生成setSVMDetector()中用到的检测子参数时，需要用到训练好的SVM的decision_func参数，  
//但通过查看CvSVM源码可知decision_func参数是protected类型变量，无法直接访问到，只能继承之后通过函数访问  
class MySVM : public CvSVM    
{    
public:    
	//获得SVM的决策函数中的alpha数组    
    double * get_alpha_vector();

	//获得SVM的决策函数中的rho参数,即偏移量    
    double get_rho();
};

//The Hog parameters class
class HogParameters
{
public:
	HogParameters(Size _winSize = Size(64, 64), Size _blockSize = Size(16, 16), Size _blockStride = Size(8, 8), 
        Size _cellSize = Size(8, 8), int _nbins = 9, Size _winStride = Size());

public:
	Size winSize;
	Size blockSize;
	Size blockStride;
	Size cellSize;
	int nbins;
	Size winStride;
};






//get all the image file names from the specified path
//It is representative to read error if the return result of the function is false;
//the variable _inputPathName : the root directory of the image file 
bool getImageNamesSetFromSpecifiedPath(const string _inputPathName, vector<string>& _imageNameSet);


//get image hog features from the specified path 
//the variable _inputPathName:  the root directory of the image file
void getImageHogFeaturesFromSpecifiedPath(const string _inputPathName, vector<vector<float>>& imageHogFeaturesSet,
    HogParameters hogParameters = HogParameters());

//The variable positiveSampleInputPathName: the root directory of the positive Sample
//The variable negativeSampleInputPathName: the root directory of the negative Sample
//The variable svmTrainModelSavePath: save path name after svm training

void getSvmTrainingModel(const string positiveSampleInputPathName, const string negativeSampleInputPathName, 
	string _svmTrainModelSavePath, string svmModelName, CvTermCriteria criteria, CvSVMParams svmParameters,
    HogParameters hogParameters = HogParameters());

void getSvmTrainingOptimalModel(const string positiveSampleInputPathName, const string negativeSampleInputPathName, 
	string _svmTrainModelSavePath, string svmModelName, CvTermCriteria criteria, CvSVMParams svmParameters,
    HogParameters hogParameters = HogParameters());


float getSingleImageSvmPredictResult(const Mat &src, string svmTrainModelXMLName, 
    HogParameters hogParameters = HogParameters());

//get batch image svm predict result

void getImageInBatchSvmPredictResult(const string imageRootPathName, string svmTrainModelXMLName, 
    HogParameters hogParameters = HogParameters(), vector<float> &predictResult = vector<float>());


//This function is to get the svm Detector.
/************************************************************************************************* 
线性SVM训练完成后得到的XML文件里面，有一个数组: support vector，还有一个数组: alpha,有一个浮点数: rho; 
将alpha矩阵同support vector相乘，注意，alpha*supportVector,将得到一个列向量。之后，再该列向量的最后添加一个元素rho。 
如此，变得到了一个分类器，利用该分类器，直接替换opencv中行人检测默认的那个分类器（cv::HOGDescriptor::setSVMDetector()）， 
就可以利用你的训练样本训练出来的分类器进行行人检测了。 
    ***************************************************************************************************/
void getSvmDetector(string svmTrainModelXMLName, vector<float>& svmDetector);

//input original image and svmDetector
void detectMultiScaleByHogSvmDetector(Mat src, vector<float> svmDetector); //分辨率不一致  检测效果不佳

}
#endif
