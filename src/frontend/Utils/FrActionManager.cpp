#include "FrActionManager.h"

// Qt
#include "Qt/QString.h"
#include "Qt/QAction.h"
#include "Qt/QApplication.h"

// Defines
#define TR_CONTEXT "MainWindow"
#define DO_TRANSLATE(context,name) \
    (QApplication::translate(context, name, 0, QApplication::UnicodeUTF8))

FrActionManager::FrActionManager(QObject* parent){
    this->Initialize(parent);
}

void FrActionManager::Initialize(QObject* parent){
    // File
    m_OpenImageAction = new QAction(parent);
    m_OpenImageAction->setObjectName(QString::fromUtf8("m_OpenImageAction"));
    m_SaveTabsAction = new QAction(parent);
    m_SaveTabsAction->setObjectName(QString::fromUtf8("m_SaveTabsAction"));
    m_LoadTabsAction = new QAction(parent);
    m_LoadTabsAction->setObjectName(QString::fromUtf8("m_LoadTabsAction"));
    m_ExitAction = new QAction(parent);
    m_ExitAction->setObjectName(QString::fromUtf8("m_ExitAction"));
    // Edit
    m_SaveToTabAction = new QAction(parent);
    m_SaveToTabAction->setObjectName(QString::fromUtf8("m_SaveToTabAction"));
    m_AddLayerAction = new QAction(parent);
    m_AddLayerAction->setObjectName(QString::fromUtf8("m_AddLayerAction"));
    m_RemoveLayerAction = new QAction(parent);
    m_RemoveLayerAction->setObjectName(QString::fromUtf8("m_RemoveLayerAction"));
	m_ChangeThresholdAction = new QAction(parent);
    m_ChangeThresholdAction->setObjectName(QString::fromUtf8("m_ChangeThresholdAction"));
    m_ChangeColormapAction = new QAction(parent);
    m_ChangeColormapAction->setObjectName(QString::fromUtf8("m_ChangeColormapAction"));
    // View
    m_ViewSliceAction = new QAction(parent);
    m_ViewSliceAction->setObjectName(QString::fromUtf8("m_ViewSliceAction"));
    m_ViewSliceAction->setCheckable(true);
    m_ViewMosaicAction = new QAction(parent);
    m_ViewMosaicAction->setObjectName(QString::fromUtf8("m_ViewMosaicAction"));
    m_ViewMosaicAction->setCheckable(true);
    m_ViewOrthoAction = new QAction(parent);
    m_ViewOrthoAction->setObjectName(QString::fromUtf8("m_ViewOrthoAction"));
    m_ViewOrthoAction->setCheckable(true);
    // Tool
    m_ManipulatorToolAction = new QAction(parent);
    m_ManipulatorToolAction->setObjectName(QString::fromUtf8("m_ManipulatorToolAction"));
    m_ManipulatorToolAction->setCheckable(true);
    m_VoxelToolAction = new QAction(parent);
    m_VoxelToolAction->setObjectName(QString::fromUtf8("m_VoxelToolAction"));
    m_VoxelToolAction->setCheckable(true);
    m_RoiToolAction = new QAction(parent);
    m_RoiToolAction->setObjectName(QString::fromUtf8("m_RoiToolAction"));
    m_RoiToolAction->setCheckable(true);
    // Help
    m_ShowHelpAction = new QAction(parent);
    m_ShowHelpAction->setObjectName(QString::fromUtf8("m_ShowHelpAction"));
    m_ShowAboutAction = new QAction(parent);
    m_ShowAboutAction->setObjectName(QString::fromUtf8("m_ShowAboutAction"));
    // Test
    m_TestAction = new QAction(parent);
    m_TestAction->setObjectName(QString::fromUtf8("m_TestAction"));
}

void FrActionManager::Retranslate(){
    // File
    m_OpenImageAction->setText(DO_TRANSLATE(TR_CONTEXT, "Open Image"));
    m_OpenImageAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Open Image"));
    m_SaveTabsAction->setText(DO_TRANSLATE(TR_CONTEXT, "Save Tabs"));
    m_SaveTabsAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Save Tabs"));
    m_LoadTabsAction->setText(DO_TRANSLATE(TR_CONTEXT, "Load Tabs"));
    m_LoadTabsAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Load Tabs"));
    m_ExitAction->setText(DO_TRANSLATE(TR_CONTEXT, "Exit"));
    m_ExitAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Exit"));
    // Edit
    m_SaveToTabAction->setText(DO_TRANSLATE(TR_CONTEXT, "Save To Tab"));
    m_SaveToTabAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Save To Tab"));
    m_AddLayerAction->setText(DO_TRANSLATE(TR_CONTEXT, "Add Layer"));
    m_AddLayerAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Add Layer"));
    m_RemoveLayerAction->setText(DO_TRANSLATE(TR_CONTEXT, "Remove Layer"));
    m_RemoveLayerAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Remove Layer"));
    m_ChangeThresholdAction->setText(DO_TRANSLATE(TR_CONTEXT, "Change Threshold"));
    m_ChangeThresholdAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Change Threshold"));
    m_ChangeColormapAction->setText(DO_TRANSLATE(TR_CONTEXT, "Change Colormap/Opacity"));
    m_ChangeColormapAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Change Colormap/Opacity"));
    // View
    m_ViewSliceAction->setText(DO_TRANSLATE(TR_CONTEXT, "Slice View"));
    m_ViewSliceAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Slice View"));
    m_ViewMosaicAction->setText(DO_TRANSLATE(TR_CONTEXT, "Mosaic View"));
    m_ViewMosaicAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Mosaic View"));
    m_ViewOrthoAction->setText(DO_TRANSLATE(TR_CONTEXT, "Ortho View"));
    m_ViewOrthoAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Ortho View"));
    // Tool
    m_ManipulatorToolAction->setText(DO_TRANSLATE(TR_CONTEXT, "Manipulation Tool"));
    m_ManipulatorToolAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Manipulation Tool"));
    m_VoxelToolAction->setText(DO_TRANSLATE(TR_CONTEXT, "Voxel Tool"));
    m_VoxelToolAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Voxel Tool"));
    m_RoiToolAction->setText(DO_TRANSLATE(TR_CONTEXT, "ROI Tool"));
    m_RoiToolAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "ROI Tool"));
    // Help
    m_ShowHelpAction->setText(DO_TRANSLATE(TR_CONTEXT, "Help"));
    m_ShowHelpAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Help"));
    m_ShowAboutAction->setText(DO_TRANSLATE(TR_CONTEXT, "About"));
    m_ShowAboutAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "About"));
    //Test
    m_TestAction->setText(DO_TRANSLATE(TR_CONTEXT, "Test"));
}
