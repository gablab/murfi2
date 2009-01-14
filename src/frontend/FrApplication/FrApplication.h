#ifndef FR_APPLICATION
#define FR_APPLICATION

// includes
#include <Qt/qapplication.h>

class FrApplication : public QApplication {
Q_OBJECT
public:
    FrApplication(int& argc, char **argv, bool useInterface = true);

protected:
};

#endif
