#include "FrApplication.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrMainController.h"

#include <crtdbg.h>


int main(int argc, char **argv){
    char* b = new char[111];
    strcpy(b, "TEST MEMORY LEAK");

    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDERR );

    int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    //tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
    tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);

    //_CrtSetBreakAlloc(1028);

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