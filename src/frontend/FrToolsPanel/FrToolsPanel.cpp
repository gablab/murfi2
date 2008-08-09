#include "FrToolsPanel.h"

FrToolsPanel::FrToolsPanel(QWidget *parent): QDialog(parent){
	setupUi(this);

	toolBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}
