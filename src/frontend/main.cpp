#include "FrApplication.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrMainController.h"


int main(int argc, char **argv){
	FrApplication application(argc, argv);

    // Create main view and document of app
	FrMainWindow* mainWindow = new FrMainWindow();
    FrMainDocument* document = new FrMainDocument();

    // Create main controller.
    // It takes care about all the stuff (i.e. init, manage and delete).
    FrMainController controller(mainWindow, document);
    controller.Initialize();

    int result = 0;
    if(!application.exec()){
        result = 1;
    }
        
	return result;
}