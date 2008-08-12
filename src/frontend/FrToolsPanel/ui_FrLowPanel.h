/********************************************************************************
** Form generated from reading ui file 'frlowpanel.ui'
**
** Created: Tue 12. Aug 21:55:20 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FRLOWPANEL_H
#define UI_FRLOWPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLCDNumber>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

class Ui_LowPanel
{
public:
    QWidget *horizontalLayout;
    QHBoxLayout *hboxLayout;
    QGroupBox *groupBox;
    QSlider *brightnessSlider;
    QLCDNumber *lcdBrightness;
    QGroupBox *groupBox_3;
    QSlider *contrastSlider;
    QLCDNumber *lcdContrast;
    QGroupBox *groupBox_2;
    QSlider *thresholdSlider;
    QLCDNumber *lcdThreshold;

    void setupUi(QWidget *LowPanel)
    {
    if (LowPanel->objectName().isEmpty())
        LowPanel->setObjectName(QString::fromUtf8("LowPanel"));
    LowPanel->resize(720, 119);
    horizontalLayout = new QWidget(LowPanel);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setGeometry(QRect(20, 10, 671, 101));
    hboxLayout = new QHBoxLayout(horizontalLayout);
    hboxLayout->setSpacing(25);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    groupBox = new QGroupBox(horizontalLayout);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    brightnessSlider = new QSlider(groupBox);
    brightnessSlider->setObjectName(QString::fromUtf8("brightnessSlider"));
    brightnessSlider->setGeometry(QRect(10, 60, 181, 16));
    brightnessSlider->setOrientation(Qt::Horizontal);
    lcdBrightness = new QLCDNumber(groupBox);
    lcdBrightness->setObjectName(QString::fromUtf8("lcdBrightness"));
    lcdBrightness->setGeometry(QRect(10, 20, 64, 23));
    lcdBrightness->setSegmentStyle(QLCDNumber::Flat);

    hboxLayout->addWidget(groupBox);

    groupBox_3 = new QGroupBox(horizontalLayout);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    contrastSlider = new QSlider(groupBox_3);
    contrastSlider->setObjectName(QString::fromUtf8("contrastSlider"));
    contrastSlider->setGeometry(QRect(10, 60, 181, 16));
    contrastSlider->setOrientation(Qt::Horizontal);
    lcdContrast = new QLCDNumber(groupBox_3);
    lcdContrast->setObjectName(QString::fromUtf8("lcdContrast"));
    lcdContrast->setGeometry(QRect(10, 20, 64, 23));
    lcdContrast->setSegmentStyle(QLCDNumber::Flat);

    hboxLayout->addWidget(groupBox_3);

    groupBox_2 = new QGroupBox(horizontalLayout);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    thresholdSlider = new QSlider(groupBox_2);
    thresholdSlider->setObjectName(QString::fromUtf8("thresholdSlider"));
    thresholdSlider->setGeometry(QRect(10, 60, 181, 16));
    thresholdSlider->setOrientation(Qt::Horizontal);
    lcdThreshold = new QLCDNumber(groupBox_2);
    lcdThreshold->setObjectName(QString::fromUtf8("lcdThreshold"));
    lcdThreshold->setGeometry(QRect(10, 20, 64, 23));
    lcdThreshold->setSegmentStyle(QLCDNumber::Flat);

    hboxLayout->addWidget(groupBox_2);


    retranslateUi(LowPanel);

    QMetaObject::connectSlotsByName(LowPanel);
    } // setupUi

    void retranslateUi(QWidget *LowPanel)
    {
    LowPanel->setWindowTitle(QApplication::translate("LowPanel", "Form", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("LowPanel", "Brightness", 0, QApplication::UnicodeUTF8));
    groupBox_3->setTitle(QApplication::translate("LowPanel", "Contrast", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("LowPanel", "Threshold", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(LowPanel);
    } // retranslateUi

};

namespace Ui {
    class LowPanel: public Ui_LowPanel {};
} // namespace Ui

#endif // UI_FRLOWPANEL_H
