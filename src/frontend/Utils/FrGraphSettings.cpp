#include "FrGraphSettings.h"

bool FrGraphSettings::CopySettings(FrGraphSettings* src, FrGraphSettings* dst){
    if(src && dst && src->GetType() == dst->GetType()){
        switch(src->GetType()){
            case GT_Intencity:
                *((FrIntencityGraphSettings*)dst) = *((FrIntencityGraphSettings*)src);
                return true;
            case GT_Movements:
                *((FrMovementsGraphSettings*)dst) = *((FrMovementsGraphSettings*)src);
                return true;
            case GT_Stimulus:
                *((FrStimulusGraphSettings*)dst) = *((FrStimulusGraphSettings*)src);
                return true;
            case GT_RoiMean:
                *((FrRoiMeanGraphSettings*)dst) = *((FrRoiMeanGraphSettings*)src);
                return true;
            case GT_RoiStd:
                *((FrRoiStdGraphSettings*)dst) = *((FrRoiStdGraphSettings*)src);
                return true;
        }
    }
    return false;
}
