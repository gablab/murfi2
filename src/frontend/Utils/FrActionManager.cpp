#include "FrActionManager.h"

// Qt
#include "Qt/qstring.h"
#include "Qt/qaction.h"
#include "Qt/qapplication.h"



// Defines
#define TR_CONTEXT "MainWindow"
#define DO_TRANSLATE(context,name) \
    (QApplication::translate(context, name, 0, QApplication::UnicodeUTF8))

FrActionManager::FrActionManager(QObject* parent){
    this->Initialize(parent);
}

FrActionManager::~FrActionManager(){
    if (m_OpenImageAction) delete m_OpenImageAction;
    if (m_OpenConfigAction) delete m_OpenConfigAction;
    if (m_OpenDataStoreAction) delete m_OpenDataStoreAction;
    if (m_SaveTabsAction) delete m_SaveTabsAction;
    if (m_LoadTabsAction) delete m_LoadTabsAction;
    if (m_SaveGraphTabsAction) delete m_SaveGraphTabsAction;
    if (m_LoadGraphTabsAction) delete m_LoadGraphTabsAction;
    if (m_ExitAction) delete m_ExitAction;
    // Edit
    if (m_SaveToTabAction) delete m_SaveToTabAction;
    if (m_SaveGraphToTabAction) delete m_SaveGraphToTabAction;
    if (m_NewLayerAction) delete m_NewLayerAction;
    if (m_DeleteLayerAction) delete m_DeleteLayerAction;
    if (m_ChangeLayerAction) delete m_ChangeLayerAction;
    if (m_NewROIAction) delete m_NewROIAction;
    // View
    if (m_ViewSliceAction) delete m_ViewSliceAction;
    if (m_ViewMosaicAction) delete m_ViewMosaicAction;
    if (m_ViewOrthoAction) delete m_ViewOrthoAction;
    if (m_ResetImageAction) delete m_ResetImageAction;
    // Tool
    if (m_ManipulatorToolAction) delete m_ManipulatorToolAction;
    if (m_VoxelToolAction) delete m_VoxelToolAction;
    if (m_RoiToolAction) delete m_RoiToolAction;
    if (m_VoxelSelectionToolAction) delete m_VoxelSelectionToolAction;
    // Help
    if (m_ShowHelpAction) delete m_ShowHelpAction;
    if (m_ShowAboutAction) delete m_ShowAboutAction;
    // Test
    if (m_TestAction) delete m_TestAction;
}

void FrActionManager::Initialize(QObject* parent){
    // File
    m_OpenImageAction = new QAction(parent);
    m_OpenImageAction->setObjectName(QString::fromUtf8("m_OpenImageAction"));
    m_OpenConfigAction = new QAction(parent);
    m_OpenConfigAction->setObjectName(QString::fromUtf8("m_OpenConfigAction"));
    m_OpenDataStoreAction = new QAction(parent);
    m_OpenDataStoreAction->setObjectName(QString::fromUtf8("m_OpenDataStoreAction"));
    m_SaveTabsAction = new QAction(parent);
    m_SaveTabsAction->setObjectName(QString::fromUtf8("m_SaveTabsAction"));
    m_LoadTabsAction = new QAction(parent);
    m_LoadTabsAction->setObjectName(QString::fromUtf8("m_LoadTabsAction"));
    m_SaveGraphTabsAction = new QAction(parent);
    m_SaveGraphTabsAction->setObjectName(QString::fromUtf8("m_SaveGraphTabsAction"));
    m_LoadGraphTabsAction = new QAction(parent);
    m_LoadGraphTabsAction->setObjectName(QString::fromUtf8("m_LoadGraphTabsAction"));
    m_ExitAction = new QAction(parent);
    m_ExitAction->setObjectName(QString::fromUtf8("m_ExitAction"));
    // Edit
    m_SaveToTabAction = new QAction(parent);
    m_SaveToTabAction->setObjectName(QString::fromUtf8("m_SaveToTabAction"));
    m_SaveGraphToTabAction = new QAction(parent);
    m_SaveGraphToTabAction->setObjectName(QString::fromUtf8("m_SaveGraphToTabAction"));
    m_NewLayerAction = new QAction(parent);
    m_NewLayerAction->setObjectName(QString::fromUtf8("m_NewLayerAction"));
    m_DeleteLayerAction = new QAction(parent);
    m_DeleteLayerAction->setObjectName(QString::fromUtf8("m_DeleteLayerAction"));
    m_ChangeLayerAction = new QAction(parent);
    m_ChangeLayerAction->setObjectName(QString::fromUtf8("m_ChangeLayerAction"));
    m_NewROIAction = new QAction(parent);
    m_NewROIAction->setObjectName(QString::fromUtf8("m_NewROIAction"));
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
    m_ResetImageAction = new QAction(parent);
    m_ResetImageAction->setObjectName(QString::fromUtf8("m_ResetImageAction"));
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
    m_VoxelSelectionToolAction = new QAction(parent);
    m_VoxelSelectionToolAction->setObjectName(QString::fromUtf8("m_VoxelSelectionToolAction"));
    m_VoxelSelectionToolAction->setCheckable(true);
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
    m_OpenConfigAction->setText(DO_TRANSLATE(TR_CONTEXT, "Open Config"));
    m_OpenConfigAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Open Config"));
    m_OpenDataStoreAction->setText(DO_TRANSLATE(TR_CONTEXT, "Open Data Store"));
    m_OpenDataStoreAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Open Data Store"));
    m_SaveTabsAction->setText(DO_TRANSLATE(TR_CONTEXT, "Save Tabset"));
    m_SaveTabsAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Save Tabset"));
    m_LoadTabsAction->setText(DO_TRANSLATE(TR_CONTEXT, "Load Tabset"));
    m_LoadTabsAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Load Tabset"));
    m_SaveGraphTabsAction->setText(DO_TRANSLATE(TR_CONTEXT, "Save Graph Tabset"));
    m_SaveGraphTabsAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Save Graph Tabset"));
    m_LoadGraphTabsAction->setText(DO_TRANSLATE(TR_CONTEXT, "Load Graph Tabset"));
    m_LoadGraphTabsAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Load Graph Tabset"));
    m_ExitAction->setText(DO_TRANSLATE(TR_CONTEXT, "Exit"));
    m_ExitAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Exit"));
    // Edit
    m_SaveToTabAction->setText(DO_TRANSLATE(TR_CONTEXT, "Save Tab"));
    m_SaveToTabAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Save Tab"));
    m_SaveGraphToTabAction->setText(DO_TRANSLATE(TR_CONTEXT, "Save Graph Tab"));
    m_SaveGraphToTabAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Save Graph Tab"));
    m_NewLayerAction->setText(DO_TRANSLATE(TR_CONTEXT, "New Layer"));
    m_NewLayerAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "New Layer"));
    m_DeleteLayerAction->setText(DO_TRANSLATE(TR_CONTEXT, "Delete Layer"));
    m_DeleteLayerAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Delete Layer"));
    m_ChangeLayerAction->setText(DO_TRANSLATE(TR_CONTEXT, "Change Layer"));
    m_ChangeLayerAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Change Layer"));    
    m_NewROIAction->setText(DO_TRANSLATE(TR_CONTEXT, "Create ROI"));
    m_NewROIAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Create ROI"));
    // View
    m_ViewSliceAction->setText(DO_TRANSLATE(TR_CONTEXT, "Slice View"));
    m_ViewSliceAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Slice View"));
    m_ViewMosaicAction->setText(DO_TRANSLATE(TR_CONTEXT, "Mosaic View"));
    m_ViewMosaicAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Mosaic View"));
    m_ViewOrthoAction->setText(DO_TRANSLATE(TR_CONTEXT, "Ortho View"));
    m_ViewOrthoAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Ortho View"));
    m_ResetImageAction->setText(DO_TRANSLATE(TR_CONTEXT, "Reset Image"));
    m_ResetImageAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Reset Image"));
    // Tool
    m_ManipulatorToolAction->setText(DO_TRANSLATE(TR_CONTEXT, "Manipulation Tool"));
    m_ManipulatorToolAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Manipulation Tool"));
    m_VoxelToolAction->setText(DO_TRANSLATE(TR_CONTEXT, "Voxel Tool"));
    m_VoxelToolAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Voxel Tool"));
    m_RoiToolAction->setText(DO_TRANSLATE(TR_CONTEXT, "ROI Tool"));
    m_RoiToolAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "ROI Tool"));
    m_VoxelSelectionToolAction->setText(DO_TRANSLATE(TR_CONTEXT, "Voxel Selection Tool"));
    m_VoxelSelectionToolAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Voxel Selection Tool"));
    // Help
    m_ShowHelpAction->setText(DO_TRANSLATE(TR_CONTEXT, "Help"));
    m_ShowHelpAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "Help"));
    m_ShowAboutAction->setText(DO_TRANSLATE(TR_CONTEXT, "About"));
    m_ShowAboutAction->setToolTip(DO_TRANSLATE(TR_CONTEXT, "About"));
    //Test
    m_TestAction->setText(DO_TRANSLATE(TR_CONTEXT, "Test"));
}
