#ifndef FR_APPLICATION
#define FR_APPLICATION

// includes
#include <Qt/qapplication.h>
#include "RtDisplay.h"

class FrApplication : public QApplication, public RtDisplay {
Q_OBJECT
public:
    FrApplication(int& argc, char **argv, bool useInterface = true);

  // sets the window running
  virtual int svc();

protected:
};

#endif
