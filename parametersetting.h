#ifndef PARAMETERSETTING_H
#define PARAMETERSETTING_H

#include <QWidget>
#include <QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QStackedWidget>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include"datemgr.h"


class ParameterSettingWidget : public QWidget
{
    Q_OBJECT
public:


    static ParameterSettingWidget* getParameterSettingWidget();
    ~ParameterSettingWidget();

private:
    explicit ParameterSettingWidget(QWidget *parent = 0);
    ParameterSettingWidget(const ParameterSettingWidget&);
    ParameterSettingWidget& operator=(const ParameterSettingWidget*);

    static ParameterSettingWidget* instance;
    static QMutex mutex;

        //void showParams();

private:
        void createControls();
        void layoutControls();
        void initTableHeaderWnd(QTableWidget *pTableWidget);				//设置table表头信息

        void initFilterTableItem();
        void initBinaryMorphItem();
        void initCfarTableItem();
        void initHogTableItem();
        void initSVMTableItem();
        void initRadarTableItem();

        void setFilterTableItem();    //set value
        void setBinaryMorphItem();
        void setCfarTableItem();
        void setHogTableItem();
        void setSVMTableItem();
        void setRadarTableItem();

        void setFilterParas();
        void setBinaryMorphParas();
        void setCfarParas();
        void setHogParas();
        void setSvmParas();
        void setRadarParas();

//        ImageFilterParas getFilterParas();
//        CfarDetectParas getCfarParas();
//        HogFeatureParas getHogParas();
//        SvmParas getSvmParas();

public:
        //parameter write and read
        QString str_to_lineEdit(int A, int B);

        QSize lineEdit_to_QSize(QString str);

private slots:
    void slot_ConfigBtnClicked();
    void slot_LoadDefaultParas();  //加载默认的参数

signals:
        void signal_ConfigDone();

private:
        QVBoxLayout *m_pMainLayout;
        QGridLayout *m_pGridLayout;

        QHBoxLayout *m_pFilterLayout;
        QHBoxLayout *m_pBinaryMorphLayout;
        QHBoxLayout *m_pCfarLayout;
        QHBoxLayout *m_pHogLayout;
        QHBoxLayout *m_pSVMLayout;
        QHBoxLayout *m_pConfigBtnLayout;
        QHBoxLayout *m_pRadarLayout;

        QGroupBox *m_pFilterBox;
        QGroupBox *m_pBinaryMorphBox;
        QGroupBox *m_pCfarBox;
        QGroupBox *m_pHogBox;
        QGroupBox *m_pSVMBox;
        QGroupBox *m_pRadarBox;

        QTableWidget *m_pFilterTableWnd;
        QTableWidget *m_pBinaryMorphTableWnd;
        QTableWidget *m_pCfarTableWnd;
        QTableWidget *m_pHogTableWnd;
        QTableWidget *m_pSVMTableWnd;
        QTableWidget *m_pRadarTableWnd;


        QPushButton *m_pConfigBtn;
        QPushButton *m_pLoadDefaultConfig;
        QDoubleValidator *m_pReg;
};




#endif // PARAMETERSSETTING_H
