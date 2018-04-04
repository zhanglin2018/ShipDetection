#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include <QWidget>

namespace Ui {
class ParameterWidget;
}


//singleton design pattern
class ParameterWidget : public QWidget
{
    Q_OBJECT

public:

    static ParameterWidget* getParameterWidget();
    ~ParameterWidget();

private:
    Ui::ParameterWidget *ui;
    explicit ParameterWidget(QWidget *parent = 0);
    ParameterWidget(const ParameterWidget&);
    ParameterWidget& operator=(const ParameterWidget*);

    static ParameterWidget* instance;
    static QMutex mutex;
};



#endif // PARAMETERWIDGET_H
