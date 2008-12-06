#ifndef FR_LAYERWIDGET
#define FR_LAYERWIDGET

// Forward declarations 
class FrLayerDocObj;
class QTableWidget;
class QHBoxLayout;
class QToolButton;
class QCheckBox;
class QLabel;

// includes
#include "FrSettings.h"
#include "FrMacro.h"

#include "Qt/qwidget.h"

// This class used to display layers
// available on image and also manage them
class FrLayerWidget: public QWidget {
	Q_OBJECT
public:
	FrLayerWidget(FrLayerDocObj* layerDO, QWidget* parent = 0);

    void SetLayerParams(FrLayerDocObj* layerDO);

    // Properties
    FrPropMacro(int, ID);
    FrPropMacro(bool, Visibility);

signals:
	void VisibilityChanged(int id);

private Q_SLOTS:
    // Add slots here
    void OnCheckBoxToggled();
	
private:
    // Widgets
	QHBoxLayout*    m_layout;
	QLabel*			picLabel;
	QLabel*			layerNameLabel;
	QCheckBox*		visibilityCheckBox;

};

#endif
