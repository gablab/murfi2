/********************************************************************************
** Form generated from reading ui file 'frtoolspanel2.ui'
**
** Created: Tue 12. Aug 00:53:17 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FRTOOLSPANEL_H
#define UI_FRTOOLSPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

class Ui_ToolsPanel
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QWidget *ToolsPanel)
    {
    if (ToolsPanel->objectName().isEmpty())
        ToolsPanel->setObjectName(QString::fromUtf8("ToolsPanel"));
    ToolsPanel->resize(233, 714);
    pushButton = new QPushButton(ToolsPanel);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    pushButton->setGeometry(QRect(50, 40, 131, 91));
    pushButton_2 = new QPushButton(ToolsPanel);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
    pushButton_2->setGeometry(QRect(50, 150, 131, 91));
    pushButton_3 = new QPushButton(ToolsPanel);
    pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
    pushButton_3->setGeometry(QRect(50, 260, 131, 91));

    retranslateUi(ToolsPanel);

    QMetaObject::connectSlotsByName(ToolsPanel);
    } // setupUi

    void retranslateUi(QWidget *ToolsPanel)
    {
    ToolsPanel->setWindowTitle(QApplication::translate("ToolsPanel", "Form", 0, QApplication::UnicodeUTF8));
    pushButton->setText(QApplication::translate("ToolsPanel", "Mode 1", 0, QApplication::UnicodeUTF8));
    pushButton_2->setText(QApplication::translate("ToolsPanel", "Mode 2", 0, QApplication::UnicodeUTF8));
    pushButton_3->setText(QApplication::translate("ToolsPanel", "Mode 3", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ToolsPanel);
    } // retranslateUi

};

namespace Ui {
    class ToolsPanel: public Ui_ToolsPanel {};
} // namespace Ui

#endif // UI_FRTOOLSPANEL_H
