// rename this file! open-source header work, sasen@mit.edu

// murfi dev credits block

// includes

// defines

// class definition & constructor


// header fields
char8_t    magic[5];       // Must be "ERTI" for External RealTime Info
int32_t    headerVersion;  // Version of this header

char8_t    scanType[64];   // Type of scan, e.g., "MPRAGE" or "EPI"
char8_t    imageType[16];  // Valid types: {"2Dz", "2Dzt", "3D", "3Dt"}
                           // 2Dz: Single volume, 1 slice at a time
                           // 2Dzt: Multi-volume, 1 slice at a time
                           // 3D: Single volume all at once
                           // 3Dt: Multi-volume, 1 volume at a time
char8_t    note[256];      // Text. Suggested contents: scanner make & 
                           //   model, pulse sequence name, version of
                           //   image-sending software used, etc.
char8_t    datatype[16];   // Of image data. Valid (OS-independent) types:
                           // - char8_t, c_char8_t, uchar8_t, c_uchar8_t
                           // - int16_t, c_int16_t, uint16_t, c_uint16_t
                           // - int32_t, c_int32_t, uint32_t, c_uint32_t
                           // - float32_t, c_float32_t, float64_t, c_float64_t
bool       isLittleEndian; // True if image data uses little endian byte order.
bool       isMosaic;       // True if 3D or 3Dt images are mosaiced.

float64_t  pixelSpacingReadMM;  // Pixel dimension (mm) in each
float64_t  pixelSpacingPhaseMM; //   acquisition direction: read, phase,
float64_t  pixelSpacingSliceMM; //   & slice.
float64_t  sliceGapMM;          // Gap between slice excitations (mm)

int32_t    numPixelsRead;   // Number of pixels in read-direction
int32_t    numPixelsPhase;  // Number of pixels in phase-direction
int32_t    numSlices;       // Number of slices

float32_t  voxelToWorldMatrix[4][4];  // Transforms voxel indices to world coordinates

int32_t    repetitionTimeMS;  // Time (ms) of one volume acquisition
int32_t    repetitionDelayMS; // Time (ms) between acquisitions
int32_t    currentTR;         // Current TR number
int32_t    totalTR;           // Expected number of TRs

bool       isMotionCorrected; // True if this data has MC applied
char8_t    mcOrder[5];        // Order of MC operations. Eg "TZYX" or "YXZT"
                              //   T: Translation
                              //   X, Y, Z: Rotation in X, Y, Z

float64_t  mcTranslationXMM;  // Translation (mm) that was applied
float64_t  mcTranslationYMM;  //   by motion correction along each
float64_t  mcTranslationZMM;  //   axis

float64_t  mcRotationXRAD;    // Rotation (radians) that was applied
float64_t  mcRotationYRAD;    //   by motion correction along each
float64_t  mcRotationZRAD;    //   axis
