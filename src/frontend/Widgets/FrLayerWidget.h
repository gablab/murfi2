#ifndef FR_LAYERWIDGET
#define FR_LAYERWIDGET

// Forward declarations 
class FrLayerSettings;
class QTableWidget;
class QHBoxLayout;
class QToolButton;
class QCheckBox;
class QLabel;

// includes
#include "Qt/QWidget.h"


// This class used to display layers
// available on image and also manage them
class FrLayerWidget: public QWidget
{
	Q_OBJECT
public:
	FrLayerWidget(int cmtype, QString name, bool visible, QWidget *parent = 0);

	void SetName(QString name);
	void SetVisibility(bool visibile);
	void SetColormapType(int cmtype);

signals:
	void VisibilityChanged();

private Q_SLOTS:
    // Add slots here
	
private:
	QHBoxLayout*    m_layout;
	
	QLabel*			picLabel;
	QLabel*			layerNameLabel;
	QCheckBox*		visibilityCheckBox;

};

#endif