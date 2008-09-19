#ifndef FR_CONTROLLER
#define FR_CONTROLLER

#include "FrMacro.h"
#include "FrNotify.h"

// Base class for all controllers
class FrController {
public:
    FrController(FrController* owner=0);
    virtual ~FrController();
    
    // Used in notification mechanizm
    virtual void Notify(int notifyCode);

    // Owner controller
    FrPropMacro(FrController*,Owner);
};

#endif