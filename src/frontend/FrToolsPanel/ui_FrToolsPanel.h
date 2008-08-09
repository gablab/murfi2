/********************************************************************************
** Form generated from reading ui file 'frtoolspanel.ui'
**
** Created: Sat 9. Aug 12:28:20 2008
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
#include <QtGui/QToolBox>
#include <QtGui/QWidget>

class Ui_ToolsPanel
{
public:
    QToolBox *toolBox;
    QWidget *page;
    QWidget *page_3;
    QWidget *page_4;
    QWidget *page_5;
    QWidget *page_2;

    void setupUi(QDialog *ToolsPanel)
    {
    if (ToolsPanel->objectName().isEmpty())
        ToolsPanel->setObjectName(QString::fromUtf8("ToolsPanel"));
    ToolsPanel->resize(254, 810);
    toolBox = new QToolBox(ToolsPanel);
    toolBox->setObjectName(QString::fromUtf8("toolBox"));
    toolBox->setGeometry(QRect(0, 0, 251, 801));
    page = new QWidget();
    page->setObjectName(QString::fromUtf8("page"));
    page->setGeometry(QRect(0, 0, 251, 661));
    toolBox->addItem(page, QApplication::translate("ToolsPanel", "Page 1", 0, QApplication::UnicodeUTF8));
    page_3 = new QWidget();
    page_3->setObjectName(QString::fromUtf8("page_3"));
    page_3->setGeometry(QRect(0, 0, 251, 631));
    toolBox->addItem(page_3, QApplication::translate("ToolsPanel", "Page", 0, QApplication::UnicodeUTF8));
    page_4 = new QWidget();
    page_4->setObjectName(QString::fromUtf8("page_4"));
    page_4->setGeometry(QRect(0, 0, 96, 26));
    toolBox->addItem(page_4, QApplication::translate("ToolsPanel", "Page", 0, QApplication::UnicodeUTF8));
    page_5 = new QWidget();
    page_5->setObjectName(QString::fromUtf8("page_5"));
    page_5->setGeometry(QRect(0, 0, 96, 26));
    toolBox->addItem(page_5, QApplication::translate("ToolsPanel", "Page", 0, QApplication::UnicodeUTF8));
    page_2 = new QWidget();
    page_2->setObjectName(QString::fromUtf8("page_2"));
    page_2->setGeometry(QRect(0, 0, 96, 26));
    toolBox->addItem(page_2, QApplication::translate("ToolsPanel", "Page 2", 0, QApplication::UnicodeUTF8));

    retranslateUi(ToolsPanel);

    toolBox->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(ToolsPanel);
    } // setupUi

    void retranslateUi(QDialog *ToolsPanel)
    {
    ToolsPanel->setWindowTitle(QApplication::translate("ToolsPanel", "Dialog", 0, QApplication::UnicodeUTF8));
    toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("ToolsPanel", "Page 1", 0, QApplication::UnicodeUTF8));
    toolBox->setItemText(toolBox->indexOf(page_3), QApplication::translate("ToolsPanel", "Page", 0, QApplication::UnicodeUTF8));
    toolBox->setItemText(toolBox->indexOf(page_4), QApplication::translate("ToolsPanel", "Page", 0, QApplication::UnicodeUTF8));
    toolBox->setItemText(toolBox->indexOf(page_5), QApplication::translate("ToolsPanel", "Page", 0, QApplication::UnicodeUTF8));
    toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("ToolsPanel", "Page 2", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ToolsPanel);
    } // retranslateUi

};

namespace Ui {
    class ToolsPanel: public Ui_ToolsPanel {};
} // namespace Ui

#endif // UI_FRTOOLSPANEL_H
