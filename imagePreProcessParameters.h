#ifndef IMAGEPREPROCESSPARAMETERS_H
#define IMAGEPREPROCESSPARAMETERS_H

#include <QWidget>

namespace Ui {
class ImagePreProcessParameters;
}

class ImagePreProcessParameters : public QWidget
{
    Q_OBJECT

public:
    explicit ImagePreProcessParameters(QWidget *parent = 0);
    ~ImagePreProcessParameters();

private:
    Ui::ImagePreProcessParameters *ui;
};

#endif // IMAGEPREPROCESSPARAMETERS_H
