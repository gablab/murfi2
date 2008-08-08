/********************************************************************************
** Form generated from reading ui file 'frtoolspanel.ui'
**
** Created: Fri 8. Aug 15:12:57 2008
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
#include <QtGui/QDialog>
#include <QtGui/QPushButton>

class Ui_ToolsPanel
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *ToolsPanel)
    {
    if (ToolsPanel->objectName().isEmpty())
        ToolsPanel->setObjectName(QString::fromUtf8("ToolsPanel"));
    ToolsPanel->resize(254, 770);
    pushButton = new QPushButton(ToolsPanel);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    pushButton->setGeometry(QRect(60, 60, 75, 24));
    pushButton_2 = new QPushButton(ToolsPanel);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
    pushButton_2->setGeometry(QRect(60, 100, 75, 24));

    retranslateUi(ToolsPanel);

    QMetaObject::connectSlotsByName(ToolsPanel);
    } // setupUi

    void retranslateUi(QDialog *ToolsPanel)
    {
    ToolsPanel->setWindowTitle(QApplication::translate("ToolsPanel", "Dialog", 0, QApplication::UnicodeUTF8));
    pushButton->setText(QApplication::translate("ToolsPanel", "PushButton", 0, QApplication::UnicodeUTF8));
    pushButton_2->setText(QApplication::translate("ToolsPanel", "PushButton", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ToolsPanel);
    } // retranslateUi

};

namespace Ui {
    class ToolsPanel: public Ui_ToolsPanel {};
} // namespace Ui

#endif // UI_FRTOOLSPANEL_H
