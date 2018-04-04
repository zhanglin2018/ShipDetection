#include "mainwindow.h"
#include <QApplication>
#include <QPainter>
#include <QPixmap>
#include "parametersetting.h"
#include "mainwindow.h"
#include "datemgr.h"
#include "imageview.h"
#include <QSpinBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include "global.h"
#include <QIcon>
#include "imageprocess.h"
#include <QToolButton>
#include "navigationbar.h"
#include "imagecomparearea.h"
#include <opencv2/opencv.hpp>  //头文件
using namespace cv;  //包含cv命名空间

#include <opencv2/opencv.hpp>


void rotateImage();
int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   MainWindow mainWindow;
   mainWindow.show();

   return a.exec();
}

void rotateImage()
{
    QString imageRootPath = "F:/temp/build-ObjectDetect-Desktop_Qt_5_8_0_MSVC2013_64bit-Debug/ship_positive_sample";      //输入图像的 根目录路径

    vector<string> _imageNameSet;
    getImageNamesSetFromSpecifiedPath(imageRootPath.toStdString(), _imageNameSet);

    cv::Mat matSrc;
    for (int i=0; i<_imageNameSet.size(); ++i)
    {

        matSrc = cv::imread(_imageNameSet[i], 2|4);
        if (!matSrc.data) {
            std::cout << "read image fail" << std::endl;
            return;
        }

        int width = matSrc.cols;
        int height = matSrc.rows;

        Mat mat1;
        matSrc.copyTo(mat1);
        Mat matTranspose;
        cv::transpose(mat1, matTranspose);

        // clockwise rotation  90
        Mat matRotate90;
        cv::flip(matTranspose, matRotate90, 1);
        cv::Mat tmp2(width, height, CV_8UC1, matRotate90.data);
        cv::imwrite(QString(QString::number(i)+"_90_.png").toStdString(), tmp2);

        // clockwise rotation 180
        Mat matRotate180;
        cv::flip(mat1, matRotate180, -1);
        cv::Mat tmp3(height, width, CV_8UC1, matRotate180.data);
        cv::imwrite(QString(QString::number(i)+"_180_.png").toStdString(), tmp3);

        // clockwise rotation 270
        Mat matRotate270;
        cv::flip(matTranspose, matRotate270, 0);
        cv::Mat tmp4(matTranspose.rows, matTranspose.cols, CV_8UC1, matRotate270.data);
        cv::imwrite(QString(QString::number(i)+"_270_.png").toStdString(), tmp4);
    }
}




