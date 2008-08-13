#include "FrApplication.h"
#include "FrDocument.h"

FrApplication::FrApplication(int argc, char **argv, bool useInterface)
    :	QApplication( argc, argv, useInterface ), m_doc(0){
	
}

FrApplication::~FrApplication(){

}