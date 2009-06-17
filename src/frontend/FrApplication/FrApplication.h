#ifndef FR_APPLICATION
#define FR_APPLICATION

class FrMainWindow;

// includes
#include <Qt/qapplication.h>

class FrApplication : public QApplication {
Q_OBJECT
public:
    static FrMainWindow* MainWindow;

public:
    FrApplication(int& argc, char **argv, bool useInterface = true);

protected:
};

#endif
