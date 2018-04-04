#include "imagePreProcessParameters.h"
#include "ui_imagePreProcessParameters.h"

ImagePreProcessParameters::ImagePreProcessParameters(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImagePreProcessParameters)
{
    ui->setupUi(this);
}

ImagePreProcessParameters::~ImagePreProcessParameters()
{
    delete ui;
}
