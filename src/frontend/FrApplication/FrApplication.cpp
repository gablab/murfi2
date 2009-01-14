#include "FrApplication.h"
#include "FrDocument.h"



FrApplication::FrApplication(int& argc, char **argv, bool useInterface)
    : QApplication( argc, argv, useInterface ) {
	
}

int FrApplication::svc(){
    this->exec();

    return 0;
}