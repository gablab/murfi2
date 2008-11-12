#ifndef FR_LAYERWIDGET
#define FR_LAYERWIDGET

// Forward declarations 
class FrLayerSettings;
class FrRoiDocObj;
class QTableWidget;
class QHBoxLayout;
class QToolButton;
class QCheckBox;
class QLabel;

// includes
#include "FrSettings.h"
#include "Qt/qwidget.h"

// This class used to display layers
// available on image and also manage them
class FrLayerWidget: public QWidget {
	Q_OBJECT
public:
	FrLayerWidget(FrLayerSettings& params, QWidget* parent = 0);
    FrLayerWidget(FrRoiDocObj& roiDO, QWidget* parent = 0);

    void GetLayerParams(FrLayerSettings& params);
    void SetLayerParams(FrLayerSettings& params);

    bool IsRoiLayer(){ return m_isRoi; }
    int GetLayerID(){ return m_params.ID; }

signals:
	void VisibilityChanged(int id);

private Q_SLOTS:
    // Add slots here
    void OnCheckBoxToggled();
	
private:
    // Some vars
    bool m_isRoi;
    FrLayerSettings m_params;

    // Widgets
	QHBoxLayout*    m_layout;
	QLabel*			picLabel;
	QLabel*			layerNameLabel;
	QCheckBox*		visibilityCheckBox;

};

#endif
