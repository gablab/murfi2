#include "FrApplication.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrMainController.h"

int main(int argc, char **argv){
	FrApplication application(argc, argv);
    
    // Create document
    FrMainDocument document;

    // Create view
	FrMainWindow mainWindow(&document);
	mainWindow.showMaximized();

    // Create controller
    FrMainController controller(&mainWindow, &document);
    controller.Initialize();

	if(!application.exec()) return 1;
	return 0;
}