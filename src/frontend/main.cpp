#include "FrApplication.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrMainController.h"

int main(int argc, char **argv){
	FrApplication application(argc, argv);
    // Create document
    FrMainDocument document;

    // Create view
	FrMainWindow mainWindow;
	mainWindow.showMaximized();

    // Create controller
    FrMainController controller(&mainWindow, &document);
    controller.Initialize();

    int result = 0;
    if(!application.exec()){
        result = 1;
    }
	return result;
}