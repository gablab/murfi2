#ifndef FR_CHANGETBC_CMD
#define FR_CHANGETBC_CMD

#include "FrBaseCmd.h"

// This command changes settings of camera
// of current view and then update view.
class FrChangeTbcCmd : public FrBaseCmd
{
public:
    // Constructor/destructor
    FrChangeTbcCmd();
    
    // Modifiers
    void SetThresholdDelta(double value);
    void SetBrightnessDelta(double value);
    void SetContrastDelta(double value);

    //
    // Overrides
    //
	virtual bool Execute();

    // Undo/redo section
	virtual bool CanUndo();
	virtual bool Undo();
	virtual bool Redo();

private:
    void SetupTbcSettings(void* settings);

private:
    bool m_isThreshold;
    double m_Threshold;

    bool m_isBrightness;
    double m_Brightness;

    bool m_isContrast;
    double m_Contrast;
};

#endif // FR_CHANGECAM_CMD