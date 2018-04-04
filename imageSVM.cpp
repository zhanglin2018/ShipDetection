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

#include "imageSVM.h"
using namespace ObjectImageSVM;

//继承自CvSVM的类，因为生成setSVMDetector()中用到的检测子参数时，需要用到训练好的SVM的decision_func参数，
//但通过查看CvSVM源码可知decision_func参数是protected类型变量，无法直接访问到，只能继承之后通过函数访问

    //获得SVM的决策函数中的alpha数组
double * MySVM::get_alpha_vector()
{
    return this->decision_func->alpha;
}

//获得SVM的决策函数中的rho参数,即偏移量
double MySVM::get_rho()
{
    return this->decision_func->rho;
}


//The Hog parameters class

HogParameters::HogParameters(Size _winSize, Size _blockSize, Size _blockStride,
    Size _cellSize, int _nbins, Size _winStride)
    :winSize(_winSize), blockSize(_blockSize), blockStride(_blockStride),
    cellSize(_cellSize), nbins(_nbins), winStride(_winStride){}


//get all the image file names from the specified path
//It is representative to read error if the return result of the function is false;
//the variable _inputPathName : the root directory of the image file
bool ObjectImageSVM::getImageNamesSetFromSpecifiedPath(const string _inputPathName, vector<string>& _imageNameSet)
{
    //string inputPathName = "F:/svm_detection_project_SUN/ship_target_detection_project/testing_targets_set";
    //The input path name does not add the bashslash symbol at the end.
    //The input path name is absolute path.
    DIR *dir = opendir(_inputPathName.c_str());
    struct dirent *entry;
    if (dir == NULL)
    {
        //DEBUG_INFO("%s", "This file direction is opened error!!!\n");
        return false;
    }
    else
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                string imageAbsolutePath = _inputPathName + "/" + entry->d_name;
                _imageNameSet.push_back(imageAbsolutePath);   //get the absolute path of the image
            }
        }
    }

    return true;
}

//get image hog features from the specified path
//the variable _inputPathName:  the root directory of the image file
void ObjectImageSVM::getImageHogFeaturesFromSpecifiedPath(const string _inputPathName, vector<vector<float>>& imageHogFeaturesSet,
    HogParameters hogParameters)
{
    vector<string> imageNameSet;
    bool isReadCorrect = getImageNamesSetFromSpecifiedPath(_inputPathName, imageNameSet);  //get image names set
    if (!isReadCorrect)
    {
        //DEBUG_INFO("%s", "Error reading into the directory!");
        return;
    }

    vector<float> imageHogFeature;   //single image hog feature

    //_winSize, _blockSize, _blockStride, _cellSize, _nbins
    HOGDescriptor *hog = new HOGDescriptor(hogParameters.winSize, hogParameters.blockSize,
        hogParameters.blockStride, hogParameters.cellSize, hogParameters.nbins);
    hog->gammaCorrection = true;


    Mat imreadImage;
    for (int i=0; i<imageNameSet.size(); ++i)
    {
        imreadImage = imread(imageNameSet[i], 0);
        hog->compute(imreadImage,  imageHogFeature, hogParameters.winStride);
        //normalize(imageHogFeature, imageHogFeature, 0, 1, NORM_MINMAX);
        imageHogFeaturesSet.push_back((imageHogFeature));
    }

    return;
}

//The variable positiveSampleInputPathName: the root directory of the positive Sample
//The variable negativeSampleInputPathName: the root directory of the negative Sample
//The variable svmTrainModelSavePath: save path name after svm training

void ObjectImageSVM::getSvmTrainingModel(const string positiveSampleInputPathName, const string negativeSampleInputPathName,
    string _svmTrainModelSavePath, string svmModelName, CvTermCriteria criteria, CvSVMParams svmParameters,
    HogParameters hogParameters)
{
    vector<vector<float>> positiveSamplesHogFeaturesSet;
    getImageHogFeaturesFromSpecifiedPath(positiveSampleInputPathName, positiveSamplesHogFeaturesSet,
        hogParameters);

    vector<vector<float>> negativeSamplesHogFeaturesSet;
    getImageHogFeaturesFromSpecifiedPath(negativeSampleInputPathName, negativeSamplesHogFeaturesSet,
        hogParameters);

    int positiveSamplesNum = positiveSamplesHogFeaturesSet.size();
    int negativeSamplesNum = negativeSamplesHogFeaturesSet.size();

    if (positiveSamplesNum == 0 || negativeSamplesNum == 0)
    {
        //DEBUG_INFO("%s", "The input path is error or empty!!!");
        return;
    }

    int samplesFeaturesCols = positiveSamplesHogFeaturesSet[0].size();


    //Assign a matix to a value
    Mat samplesFeaturesMat = Mat::zeros(positiveSamplesNum + negativeSamplesNum, samplesFeaturesCols, CV_32FC1);
    Mat samplesLabelMat = Mat::zeros(positiveSamplesNum + negativeSamplesNum, 1, CV_32FC1);

    for (int i=0; i<positiveSamplesNum; i++)
    {
        for (int j=0; j<samplesFeaturesCols; j++)
        {
            samplesFeaturesMat.at<float>(i, j) = positiveSamplesHogFeaturesSet[i][j];
        }
        samplesLabelMat.at<float>(i, 0) = POSITIVE_SAMPLE_LABEL;
    }

    for (int i=0; i<negativeSamplesNum; i++)
    {
        for (int j=0; j<samplesFeaturesCols; j++)
        {
            samplesFeaturesMat.at<float>(i + positiveSamplesNum, j) = negativeSamplesHogFeaturesSet[i][j];
        }
        samplesLabelMat.at<float>(i + positiveSamplesNum, 0) = NEGATIVE_SAMPLE_LABEL;
    }


    //iterative terminating condition， 当迭代满 1000 次或误差小于 FLT_EPSILON 时停止迭代
    //CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, ITERATIVE_NUMBER, FLT_EPSILON);

/*CvSVMParams parameters description:

    enum { C_SVC=100, NU_SVC=101, ONE_CLASS=102, EPS_SVR=103, NU_SVR=104 };
    enum { LINEAR=0, POLY=1, RBF=2, SIGMOID=3 };
    enum { C=0, GAMMA=1, P=2, NU=3, COEF=4, DEGREE=5 };

        CV_PROP_RW int         svm_type;
        CV_PROP_RW int         kernel_type;
        CV_PROP_RW double      degree; // for poly
        CV_PROP_RW double      gamma;  // for poly/rbf/sigmoid
        CV_PROP_RW double      coef0;  // for poly/sigmoid

        CV_PROP_RW double      C;    // for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
        CV_PROP_RW double      nu;   // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
        CV_PROP_RW double      p;    // for CV_SVM_EPS_SVR
        CvMat*      class_weights;   // for CV_SVM_C_SVC
        CV_PROP_RW CvTermCriteria term_crit;    // termination criteria
*/
    //CvSVMParams svmParameters(CvSVM::C_SVC, CvSVM::RBF, 3, 1.6, 0,
                    // 3, 0.5, 0.1, 0, criteria);

    CvSVM svm;   //creat a new svm

    svm.train(samplesFeaturesMat, samplesLabelMat, Mat(), Mat(), svmParameters); //svm train

    string svmTrainModelSavePath = _svmTrainModelSavePath + "/" + svmModelName;

    svm.save(svmTrainModelSavePath.c_str());

    //cout << "input SVM model XML: " << svmTrainModelSavePath << "\n" << endl;
}

void ObjectImageSVM::getSvmTrainingOptimalModel(const string positiveSampleInputPathName, const string negativeSampleInputPathName,
    string _svmTrainModelSavePath, string svmModelName, CvTermCriteria criteria, CvSVMParams svmParameters,
    HogParameters hogParameters)
{
    vector<vector<float>> positiveSamplesHogFeaturesSet;
    getImageHogFeaturesFromSpecifiedPath(positiveSampleInputPathName, positiveSamplesHogFeaturesSet,
        hogParameters);

    vector<vector<float>> negativeSamplesHogFeaturesSet;
    getImageHogFeaturesFromSpecifiedPath(negativeSampleInputPathName, negativeSamplesHogFeaturesSet,
        hogParameters);

    int positiveSamplesNum = positiveSamplesHogFeaturesSet.size();
    int negativeSamplesNum = negativeSamplesHogFeaturesSet.size();

    if (positiveSamplesNum == 0 || negativeSamplesNum == 0)
    {
        //DEBUG_INFO("%s", "The input path is error or empty!!!");
        return;
    }

    int samplesFeaturesCols = positiveSamplesHogFeaturesSet[0].size();


    //Assign a matix to a value
    Mat samplesFeaturesMat = Mat::zeros(positiveSamplesNum + negativeSamplesNum, samplesFeaturesCols, CV_32FC1);
    Mat samplesLabelMat = Mat::zeros(positiveSamplesNum + negativeSamplesNum, 1, CV_32FC1);

    for (int i=0; i<positiveSamplesNum; i++)
    {
        for (int j=0; j<samplesFeaturesCols; j++)
        {
            samplesFeaturesMat.at<float>(i, j) = positiveSamplesHogFeaturesSet[i][j];
        }
        samplesLabelMat.at<float>(i, 0) = POSITIVE_SAMPLE_LABEL;
    }

    for (int i=0; i<negativeSamplesNum; i++)
    {
        for (int j=0; j<samplesFeaturesCols; j++)
        {
            samplesFeaturesMat.at<float>(i + positiveSamplesNum, j) = negativeSamplesHogFeaturesSet[i][j];
        }
        samplesLabelMat.at<float>(i + positiveSamplesNum, 0) = NEGATIVE_SAMPLE_LABEL;
    }


    //iterative terminating condition， 当迭代满 1000 次或误差小于 FLT_EPSILON 时停止迭代
    //CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, ITERATIVE_NUMBER, FLT_EPSILON);

/*CvSVMParams parameters description:

    enum { C_SVC=100, NU_SVC=101, ONE_CLASS=102, EPS_SVR=103, NU_SVR=104 };
    enum { LINEAR=0, POLY=1, RBF=2, SIGMOID=3 };
    enum { C=0, GAMMA=1, P=2, NU=3, COEF=4, DEGREE=5 };

        CV_PROP_RW int         svm_type;
        CV_PROP_RW int         kernel_type;
        CV_PROP_RW double      degree; // for poly
        CV_PROP_RW double      gamma;  // for poly/rbf/sigmoid
        CV_PROP_RW double      coef0;  // for poly/sigmoid

        CV_PROP_RW double      C;    // for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
        CV_PROP_RW double      nu;   // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
        CV_PROP_RW double      p;    // for CV_SVM_EPS_SVR
        CvMat*      class_weights;   // for CV_SVM_C_SVC
        CV_PROP_RW CvTermCriteria term_crit;    // termination criteria
*/
    //CvSVMParams svmParameters(CvSVM::C_SVC, CvSVM::RBF, 3, 1.6, 0,
                    // 3, 0.5, 0.1, 0, criteria);

    CvSVM svm;   //creat a new svm

    /*
    train_auto( const cv::Mat& trainData, const cv::Mat& responses,
        const cv::Mat& varIdx, const cv::Mat& sampleIdx, CvSVMParams params,
        int k_fold = 10,
        CvParamGrid Cgrid      = CvSVM::get_default_grid(CvSVM::C),
        CvParamGrid gammaGrid  = CvSVM::get_default_grid(CvSVM::GAMMA),
        CvParamGrid pGrid      = CvSVM::get_default_grid(CvSVM::P),
        CvParamGrid nuGrid     = CvSVM::get_default_grid(CvSVM::NU),
        CvParamGrid coeffGrid  = CvSVM::get_default_grid(CvSVM::COEF),
        CvParamGrid degreeGrid = CvSVM::get_default_grid(CvSVM::DEGREE),
        bool balanced=false);
    */
//    CvParamGrid pGrid      = CvParamGrid(1, 1, 0.0);
//    CvParamGrid nuGrid     = CvParamGrid(1, 1, 0.0);
//    CvParamGrid coeffGrid  = CvParamGrid(1, 1, 0.0);
//    CvParamGrid degreeGrid = CvParamGrid(1, 1, 0.0);
//    = get_default_grid(CvSVM::P),
//            CvParamGrid nuGrid     = get_default_grid(CvSVM::NU),
//            CvParamGrid coeffGrid  = get_default_grid(CvSVM::COEF),
//            CvParamGrid degreeGrid = get_default_grid(CvSVM::DEGREE),

    svm.train_auto(samplesFeaturesMat, samplesLabelMat, Mat(), Mat(),
        svmParameters, 2, svm.get_default_grid(CvSVM::C), svm.get_default_grid(CvSVM::GAMMA)
                   , svm.get_default_grid(CvSVM::P)
                   , svm.get_default_grid(CvSVM::NU)
                   , svm.get_default_grid(CvSVM::COEF)
                   , svm.get_default_grid(CvSVM::DEGREE)); //svm train

    CvSVMParams svmOptimalParameters = svm.get_params();

    string svmTrainModelSavePath = _svmTrainModelSavePath + "/" + svmModelName;

    svm.save(svmTrainModelSavePath.c_str());
//    cout << "input SVM model XML: " << svmTrainModelSavePath << "\n" << endl;

    DateMgr::getDataParas()->svmParas->Cvalue = svmOptimalParameters.C;
    DateMgr::getDataParas()->svmParas->gamma = svmOptimalParameters.gamma;
    DateMgr::getDataParas()->svmParas->p = svmOptimalParameters.p;
    DateMgr::getDataParas()->svmParas->nu = svmOptimalParameters.nu;
    DateMgr::getDataParas()->svmParas->coef0 = svmOptimalParameters.coef0;
    DateMgr::getDataParas()->svmParas->degree = svmOptimalParameters.degree;

//    cout << "input Optimal parameters: \n" << endl;
//    cout << "C: " << svmOptimalParameters.C << "\n" << endl;
//    cout << "Gamma: " << svmOptimalParameters.gamma << "\n" << endl;
}


float ObjectImageSVM::getSingleImageSvmPredictResult(const Mat &src, string svmTrainModelXMLName,
    HogParameters hogParameters)
{
    //The first step is to get hog features of image
    HOGDescriptor *hog = new HOGDescriptor(hogParameters.winSize, hogParameters.blockSize,
        hogParameters.blockStride, hogParameters.cellSize, hogParameters.nbins);
    hog->gammaCorrection = true;

    vector<float> imageHogFeature;
    hog->compute(src,  imageHogFeature, hogParameters.winStride);
    //normalize(imageHogFeature, imageHogFeature, 0, 1, NORM_MINMAX);

    Mat svmTrainMat(1, imageHogFeature.size(), CV_32FC1);

    for (int i=0; i<imageHogFeature.size(); ++i)
    {
        svmTrainMat.at<float>(0, i) = imageHogFeature[i];
    }

    CvSVM svm;
    svm.load(svmTrainModelXMLName.c_str());

    float predictResult = svm.predict(svmTrainMat);

    return predictResult;
}

//get batch image svm predict result

void ObjectImageSVM::getImageInBatchSvmPredictResult(const string imageRootPathName, string svmTrainModelXMLName,
    HogParameters hogParameters, vector<float> &predictResult)
{
    //The first step is to get hog features of image
    vector<vector<float>> batchImageHogFeaturesSet;
    getImageHogFeaturesFromSpecifiedPath(imageRootPathName, batchImageHogFeaturesSet,
        hogParameters);

    int batchImageNum = batchImageHogFeaturesSet.size();

    if (batchImageNum == 0)
    {
        //DEBUG_INFO("%s", "The input path is error or empty!!!");
        return;
    }

    int batchImageHogFeatureDim = batchImageHogFeaturesSet[0].size();
    Mat svmTrainMat(batchImageNum, batchImageHogFeatureDim, CV_32FC1);

    for (int i=0; i<batchImageNum; i++)
    {
        for (int j=0; j<batchImageHogFeatureDim; j++)
        {
            svmTrainMat.at<float>(i, j) = batchImageHogFeaturesSet[i][j];
        }
    }

    CvSVM svm;
    svm.load(svmTrainModelXMLName.c_str());

    svm.predict(svmTrainMat, predictResult);

    return ;

}


//This function is to get the svm Detector.
/*************************************************************************************************
线性SVM训练完成后得到的XML文件里面，有一个数组: support vector，还有一个数组: alpha,有一个浮点数: rho;
将alpha矩阵同support vector相乘，注意，alpha*supportVector,将得到一个列向量。之后，再该列向量的最后添加一个元素rho。
如此，变得到了一个分类器，利用该分类器，直接替换opencv中行人检测默认的那个分类器（cv::HOGDescriptor::setSVMDetector()），
就可以利用你的训练样本训练出来的分类器进行行人检测了。
    ***************************************************************************************************/
void ObjectImageSVM::getSvmDetector(string svmTrainModelXMLName, vector<float>& svmDetector)
{
    MySVM svm;
    svm.load(svmTrainModelXMLName.c_str());

    int featureDim = svm.get_var_count();
    int supportVectorNum = svm.get_support_vector_count();

    Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);
    Mat supportVectorMat = Mat::zeros(supportVectorNum, featureDim, CV_32FC1);
    Mat resultMat = Mat::zeros(1, featureDim, CV_32FC1);

    //copy the svm.get_support_vector(i) to supportVectorMat
    for (int i=0; i<supportVectorNum; ++i)
    {
        const float* supportVector = svm.get_support_vector(i);
        for (int j=0; j<featureDim; ++j)
        {
            supportVectorMat.at<float>(i, j) = supportVector[j];
        }
    }

    //copy alpha vector to the alphaMat
    double *alphaData = svm.get_alpha_vector();
    for (int i=0; i<supportVectorNum; ++i)
    {
        alphaMat.at<float>(0, i) = static_cast<float>(alphaData[i]);
    }

    resultMat = -1 * alphaMat * supportVectorMat;

    //得到最终的svmDetector(const vector<float>& svmDetector)参数中可用的检测子
    for (int i=0; i<featureDim; ++i)
    {
        svmDetector.push_back(resultMat.at<float>(0, i));
    }

    //最后添加偏移量rho，得到检测子
    svmDetector.push_back(static_cast<float>(svm.get_rho()));
}

//input original image and svmDetector
void ObjectImageSVM::detectMultiScaleByHogSvmDetector(Mat src, vector<float> svmDetector) //分辨率不一致  检测效果不佳
{
    namedWindow("原始图片",0);
    imshow("原始图片",src);

    HogParameters hogParameters;
    HOGDescriptor *hog = new HOGDescriptor(hogParameters.winSize, hogParameters.blockSize,
        hogParameters.blockStride, hogParameters.cellSize, hogParameters.nbins);
    hog->gammaCorrection = true;

    hog->setSVMDetector(svmDetector);

    vector<Rect> found, found_filtered;  //矩形框数组
    hog->detectMultiScale(src, found, 0, Size(8,8), Size(32,32), 1.05, 2);//对图片进行多尺度行人检测
    cout<<"找到的矩形框个数："<<found.size()<<endl;

    //找出所有没有嵌套的矩形框r,并放入found_filtered中,如果有嵌套的话,则取外面最大的那个矩形框放入found_filtered中
    for(int i=0; i<found.size(); i++)
    {
        Rect r = found[i];
        int j=0;
        for(; j<found.size(); j++)
            if(j != i && (r & found[j]) == r)
                break;
        if( j == found.size())
            found_filtered.push_back(r);
    }

    //画矩形框，因为hog检测出的矩形框比实际人体框要稍微大些,所以这里需要做一些调整
    Mat srcRGBImage;
    cvtColor( src, srcRGBImage, CV_GRAY2RGB );   // 将原始的图片转换为三通道


    for(int i=0; i<found_filtered.size(); i++)
    {
        Rect r = found_filtered[i];
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        rectangle(srcRGBImage, r.tl(), r.br(), Scalar(0,0,255), 3);
    }

    namedWindow("检测效果图片",0);
    imshow("检测效果图片",srcRGBImage);
}

