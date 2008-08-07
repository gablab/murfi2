#include "FrApplication/FrApplication.h"
#include "FrMainWindow/FrMainWindow.h"


int main(int argc, char **argv){
	FrApplication app(argc, argv);

	FrMainWindow mainWidget;
	mainWidget.showMaximized();

	if(!app.exec())
		return 1;

	return 0;
}