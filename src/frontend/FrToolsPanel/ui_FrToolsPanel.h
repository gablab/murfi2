/********************************************************************************
** Form generated from reading ui file 'FrToolsPanel2.ui'
**
** Created: Thu 14. Aug 22:40:59 2008
**      by: Qt User Interface Compiler version 4.4.1
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

QT_BEGIN_NAMESPACE

class Ui_ToolsPanel
{
public:
    QPushButton *mode1Button;
    QPushButton *mode2Button;
    QPushButton *mode3Button;

    void setupUi(QWidget *ToolsPanel)
    {
    if (ToolsPanel->objectName().isEmpty())
        ToolsPanel->setObjectName(QString::fromUtf8("ToolsPanel"));
    ToolsPanel->resize(176, 714);
    mode1Button = new QPushButton(ToolsPanel);
    mode1Button->setObjectName(QString::fromUtf8("mode1Button"));
    mode1Button->setGeometry(QRect(20, 40, 131, 91));
    mode2Button = new QPushButton(ToolsPanel);
    mode2Button->setObjectName(QString::fromUtf8("mode2Button"));
    mode2Button->setGeometry(QRect(20, 150, 131, 91));
    mode3Button = new QPushButton(ToolsPanel);
    mode3Button->setObjectName(QString::fromUtf8("mode3Button"));
    mode3Button->setGeometry(QRect(20, 260, 131, 91));

    retranslateUi(ToolsPanel);

    QMetaObject::connectSlotsByName(ToolsPanel);
    } // setupUi

    void retranslateUi(QWidget *ToolsPanel)
    {
    ToolsPanel->setWindowTitle(QApplication::translate("ToolsPanel", "Form", 0, QApplication::UnicodeUTF8));
    mode1Button->setText(QApplication::translate("ToolsPanel", "Mode 1", 0, QApplication::UnicodeUTF8));
    mode2Button->setText(QApplication::translate("ToolsPanel", "Mode 2", 0, QApplication::UnicodeUTF8));
    mode3Button->setText(QApplication::translate("ToolsPanel", "Mode 3", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ToolsPanel);
    } // retranslateUi

};

namespace Ui {
    class ToolsPanel: public Ui_ToolsPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRTOOLSPANEL_H
