//--------------------------------------������˵����-------------------------------------------
//		Program description��Get the hog features of the image, use SVM for training and get the model 
//		IDE version��Visual Studio 2010
//		OpenCV version��	2.4.9
//		2018��1��17�� Created by @����
//		2018��X�� Revised by @
//------------------------------------------------------------------------------------------------

//---------------------------------��ͷ�ļ��������ռ�������֡�-----------------------------
//		����������������ʹ�õ�ͷ�ļ��������ռ�
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

//�̳���CvSVM���࣬��Ϊ����setSVMDetector()���õ��ļ���Ӳ���ʱ����Ҫ�õ�ѵ���õ�SVM��decision_func������  
//��ͨ���鿴CvSVMԴ���֪decision_func������protected���ͱ������޷�ֱ�ӷ��ʵ���ֻ�ܼ̳�֮��ͨ����������  
class MySVM : public CvSVM    
{    
public:    
	//���SVM�ľ��ߺ����е�alpha����    
    double * get_alpha_vector();

	//���SVM�ľ��ߺ����е�rho����,��ƫ����    
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
����SVMѵ����ɺ�õ���XML�ļ����棬��һ������: support vector������һ������: alpha,��һ��������: rho; 
��alpha����ͬsupport vector��ˣ�ע�⣬alpha*supportVector,���õ�һ����������֮���ٸ���������������һ��Ԫ��rho�� 
��ˣ���õ���һ�������������ø÷�������ֱ���滻opencv�����˼��Ĭ�ϵ��Ǹ���������cv::HOGDescriptor::setSVMDetector()���� 
�Ϳ����������ѵ������ѵ�������ķ������������˼���ˡ� 
    ***************************************************************************************************/
void getSvmDetector(string svmTrainModelXMLName, vector<float>& svmDetector);

//input original image and svmDetector
void detectMultiScaleByHogSvmDetector(Mat src, vector<float> svmDetector); //�ֱ��ʲ�һ��  ���Ч������

}
#endif
