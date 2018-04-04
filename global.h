#ifndef _GLOBAL_H_
#define _GLOBAL_H_


#define  DELETE_OBJ(OBJ)		if (NULL != OBJ) {delete OBJ; OBJ = NULL;}

#define ITERATIVE_NUMBER 1000
#define FLT_EPSILON     1.192092896e-07F



#define   STR_FILE             QStringLiteral("Image File")
#define   STR_LOAD             QStringLiteral("Load Image File")
#define   STR_SAVE             QStringLiteral("Save Image File")
#define   STR_PROCESS            QStringLiteral("Process")
#define   STR_SAR_IMAGE_PREPROCESS             QStringLiteral("SAR Image preProcessing")
#define   STR_FILTER            QStringLiteral("Image Filter")
#define   STR_BINARY           QStringLiteral("Image Binary")
#define   STR_MORPH            QStringLiteral("Image morphology")
#define   STR_SHOWCONTROLS            QStringLiteral("Show controls")


#define   STR_TARGET_ROUGH_DETECTION           QStringLiteral("Target Rough Detection")
#define   STR_TARGET_PRECISION_DETECTION            QStringLiteral("Target Precision Detection")
#define   STR_OPTION            QStringLiteral("Option")

#define   STR_BOX_FILTER           QStringLiteral("BoxFilter")
#define   STR_MEAN_FILTER            QStringLiteral("MeanFilter")
#define   STR_GAUSSIAN_FILTER            QStringLiteral("GaussianFilter")
#define   STR_MEDIAN_FILTER            QStringLiteral("MedianFilter")
#define   STR_BILATERAL_FILTER            QStringLiteral("BilateralFilter")
#define   STR_BILATERAL_ADAPTIVE_FILTER            QStringLiteral("BilateralAdaptiveFilter")




#define   STR_BINARY_THRESHOLD            QStringLiteral("Threshold Binary")
#define   STR_BINARY_INVERSE            QStringLiteral("Threshold Binary Inverse")
#define   STR_BINARY_TRUNC            QStringLiteral("Threshold trunc")
#define   STR_BINARY_TO_ZERO            QStringLiteral("Threshold To Zero")
#define   STR_BINARY_TO_ZERO_INVERSE            QStringLiteral("Threshold To Zero Inverse")
#define   STR_BINARY_OTSU            QStringLiteral("Threshold Otsu")





#define   STR_MORPH_DILATE            QStringLiteral("Morph Dilate")
#define   STR_MORPH_ERODE            QStringLiteral("Morph Erode")
#define   STR_MORPH_OPEN            QStringLiteral("Morph Open")
#define   STR_MORPH_CLOSE            QStringLiteral("Morph Close")
#define   STR_MORPH_GRADIENT            QStringLiteral("Morph Gradient")
#define   STR_MORPH_TOPHAT            QStringLiteral("Morph TopHat")
#define   STR_MORPH_BLACKHAT            QStringLiteral("Morph BlackHat")





#define   STR_CFAR_RULY            QStringLiteral("Ruly CfarDetect")
#define   STR_CFAR_GAUSSIAN            QStringLiteral("Gaussian CfarDetection")
#define   STR_CFAR_GAUSSIAN_ADA            QStringLiteral("Gaussian Adaptive CfarDetection")
#define   STR_CFAR_LOGNORMAL            QStringLiteral("LogNormal CfarDetectrion")
#define   STR_CFAR_LOGNORMAL_ADA            QStringLiteral("LogNormal Adaptive CfarDetection")


#define   STR_SVM_HOG            QStringLiteral("Hog SVM")
#define   STR_SVM_DETECTOR            QStringLiteral("SVM Detector")




#define   STR_MACHINE_EVALUATION            QStringLiteral("Machine Learning Recognition Evaluation")
#define   STR_PARAMETER_SET            QStringLiteral("Detection Parameters Set")
#define   STR_GETROT            QStringLiteral("GetRoI")
#define   STR_DRAWCONTOUR            QStringLiteral("DrawContour")


#define   STR_GETSVM            QStringLiteral("GetSVM")
#define   STR_LAND_DETECT           QStringLiteral("Land Deletef")


#define   STR_ZOOMIN           QStringLiteral("ZoomIn")
#define   STR_ZOOMOUT            QStringLiteral("ZoomOut")
#define   STR_IMAGE_CUT           QStringLiteral("Image Cut")
#define   STR_ONEKEY_PROCESSING            QStringLiteral("ImageDetection OneKey Processing")
#define   STR_PROCESS_EXIT_ABORT            QStringLiteral("Processing Exit")
#define   STR_IMAGE_REFRESH            QStringLiteral("Image Refresh")
#define   STR_ADD_LITTLE_IMAGE       QStringLiteral("Add thumbnail")


// add controls layout options
#define STR_SHOW_NAVIGATION_BAR          QStringLiteral("NavigationBar")
#define STR_SHOW_IMAGE_PROCESS_AREA      QStringLiteral("Image Process Area")
#define STR_SHOW_THUMBNAIL_AREA       QStringLiteral("Thumbnail Area")
#define STR_SHOW_CENTRAL_IMAGE_AREA      QStringLiteral("Central Image Area")
#define STR_SHOW_CUT_IMAGE_AREA               QStringLiteral("Cut Image Area")

#define STR_SHOW_IMAGE_DESCRIPTION_AREA  QStringLiteral("Image Description Area")
#define STR_SHOW_DEFAULT_IMAGE_LAYOUT    QStringLiteral("Default Image Layout")


#define   PATH_LOADIMAGE         "../ObjectDetect/image/loadImage.png"
#define   PATH_SAVEIMAGE         "../ObjectDetect/image/saveImage.png"

#define   PATH_SETTINGICON       "../ObjectDetect/image/settings.png"
#define   PATH_CUTICON           "../ObjectDetect/image/cutImage.png"

#define   PATH_PREVIOUSICON      "../ObjectDetect/image/previous.png"
#define   PATH_NEXTICON          "../ObjectDetect/image/next.png"
#define   PATH_DRAGICON          "../ObjectDetect/image/drag.png"

#define   PATH_ZOOMIN            "../ObjectDetect/image/zoomin.png"
#define   PATH_ZOOMOUT           "../ObjectDetect/image/zoomout.png"
#define   PATH_GET_SVM          "../ObjectDetect/image/svm.jpg"
#define   PATH_GET_ROI          "../ObjectDetect/image/roi1.jpg"


#define   PATH_LAND_DELETE      "../ObjectDetect/image/land_delete.jpg"


#define   PATH_PLAY              "../ObjectDetect/image/play.png"
#define   PATH_STOP              "../ObjectDetect/image/stop.png"
#define   PATH_REFRESH            "../ObjectDetect/image/refresh.png"
#define   PATH_ADDLITTLEIMAGE    "../ObjectDetect/image/add_little_image_icon.JPG"


#define   PATH_IMAGE_PROCESS_RUN   "../ObjectDetect/image/run.png"
#define   PATH_IMAGE_PROCESS_RUN1   "../ObjectDetect/image/run1.png"
#define   PATH_IMAGE_PROCESS_RUN2   "../ObjectDetect/image/run2.png"
#define   PATH_IMAGE_PROCESS_RUN3   "../ObjectDetect/image/run3.png"
#define   PATH_IMAGE_PROCESS_RUN4   "../ObjectDetect/image/run4.png"

#define   PATH_IMAGE_FILTER_PROCESS_Navigation    "../ObjectDetect/image/filter.png"
#define   PATH_IMAGE_BINARY_PROCESS_Navigation   "../ObjectDetect/image/binary.jpg"
#define   PATH_IMAGE_MORPH_PROCESS_Navigation   "../ObjectDetect/image/morph.jpg"
#define   PATH_IMAGE_CFAR_DETECT_PROCESS_Navigation   "../ObjectDetect/image/cfarDetect.jpg"
#define   PATH_IMAGE_HOG_AND_SVM_PROCESS_Navigation   "../ObjectDetect/image/objectCorect.png"


#endif
