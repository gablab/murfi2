/* ohinds: 2009-03-18
 * added 
 */
#if 0

#include "FrApplication.h"
#include "FrMainWindow.h"
#include "FrMainDocument.h"
#include "FrMainController.h"
#include "FrDataStore.h"
#include "RtConductor.h"

// Control block for leak detection
#ifdef WIN32
    //#define MEM_LEAK_TEST 

    #ifdef MEM_LEAK_TEST
        #include <crtdbg.h>
        //#define BREAK_ON_LEAK
        #define BREAK_LEAK_ID 24648
    #endif
#endif


//
// ------- MAIN ENTRY ---------
//
int ACE_TMAIN(int argc, char **argv){
#ifdef MEM_LEAK_TEST
    char* b = new char[111];
    strcpy(b, "TEST MEMORY LEAK");

    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDERR );

    int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
#ifdef BREAK_ON_LEAK
    tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
#endif
    tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);

#ifdef BREAK_ON_LEAK
    //_CrtSetBreakAlloc(3282);
#endif
#endif

    FrApplication application(argc, argv);

    // testing data receiving
	char *path[3];
	path[0] = "test";
	path[1] = "-f";
	path[2] = "test_config.xml";
    
    // Create backend stuff
	// ohinds 2009-02-02
	// changed this to reflect new conductor architecture
	RtConfigFmriRun config;
	config.parseConfigFile(path[2]);
	RtConductor conductor;
	conductor.configure(config);
	conductor.activate();

    // Create main view and document of app
    FrMainWindow* mainWindow = new FrMainWindow();
    FrMainDocument* document = new FrMainDocument();

    // Create main controller.
    // It takes care about all the stuff (i.e. init, manage and delete).
    FrMainController controller(mainWindow, document, &conductor);
    controller.Initialize();

    int result = 0;
    if(!application.exec()){
        result = 1;
    }
    
    // ohinds 2009-02-02
    // unnecessary
    //conductor.stop();

    return result;
}

#endif
