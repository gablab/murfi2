#include "FrApplication.h"
#include "FrDocument.h"

FrMainWindow* FrApplication::MainWindow = NULL;

FrApplication::FrApplication(int& argc, char **argv, bool useInterface)
    : QApplication( argc, argv, useInterface ) {
	
}

