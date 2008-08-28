#ifndef FR_TBC_FILTER
#define FR_TBC_FILTER

// Forward declarations
class vtkImageData;

// Includes
#include "vtkImageToImageFilter.h"


// some values
#define MIN_THRESHOLD   0.0
#define MAX_THRESHOLD   1.0
#define DEF_THRESHOLD   0.0
#define MIN_BRIGHTNESS -1.0
#define MAX_BRIGHTNESS  1.0
#define DEF_BRIGHTNESS  0.0
#define MIN_CONTRAST   -1.0
#define MAX_CONTRAST    1.0
#define DEF_CONTRAST    0.0

#define MIN_BYTE_VALUE 0.0
#define MAX_BYTE_VALUE 255.0

// This class is used to change 
// threshold, brightness and contrast
// of rendered image data
class FrTBCFilter : public vtkImageToImageFilter {
public:
    static FrTBCFilter* New();
public:
    // Get/Set threshold (in the range [0 .. 1])
    double GetThreshold(){ return m_threshold; }
    void SetThreshold(double value);

    // Get/Set brightness (in the range [-1 .. 1])
    double GetBrightness(){ return m_brightness; }
    void SetBrightness(double value);

    // Get/Set contrast (int the range [-1 .. 1])
    double GetContrast(){ return m_contrast; }
    void SetContrast(double value);

    // Overrides of base class
    virtual void ThreadedExecute(vtkImageData *inData, 
                                 vtkImageData *outData,
                                 int extent[6], int threadId);
private:
    template<class T>
    T ClampValue(T value, T min, T max){
        if(value < min) value = min;
        else if(value > max) value = max;
        return value;
    }

    void InitLookupTable();

private:
    double m_threshold;
    double m_brightness;
    double m_contrast;

private:
    FrTBCFilter();
    FrTBCFilter(const FrTBCFilter&);  // Not implemented.
    void operator=(const FrTBCFilter&);  // Not implemented.
};

#endif


