#include "FrApplication.h"
#include "FrMainWindow.h"


int main(int argc, char **argv){
	FrApplication app(argc, argv);

	FrMainWindow mainWidget;
	mainWidget.showMaximized();

	if(!app.exec())
		return 1;

	return 0;
}